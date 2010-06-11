#include "OUAN_Precompiled.h"

#include "RenderSubsystem.h"
#include "../Application.h"
#include "../Loader/Configuration.h"
#include "../Game/GameWorldManager.h"
#include "../Physics/PhysicsSubsystem.h"
#include "../GUI/GUISubsystem.h"
#include "CameraManager/CameraManager.h"
#include "ChangeWorldRenderer.h"
#include "CameraManager/CameraControllerFirstPerson.h"
#include "TrajectoryManager/TrajectoryManager.h"
#include "RenderComponent/RenderComponent.h"
#include "RenderComponent/RenderComponentBillboardSet.h"
#include "RenderComponent/RenderComponentCamera.h"
#include "RenderComponent/RenderComponentEntity.h"
#include "RenderComponent/RenderComponentLight.h"
#include "RenderComponent/RenderComponentParticleSystem.h"
#include "RenderComponent/RenderComponentScene.h"
#include "RenderComponent/RenderComponentPositional.h"
#include "RenderComponent/RenderComponentViewport.h"
#include "RenderComponent/RenderComponentDecal.h"
#include "RenderComponent/RenderComponentPlane.h"

using namespace OUAN;
using namespace Ogre;

RenderSubsystem::RenderSubsystem(std::string windowName)
: mWindow( NULL )
, mWindowName(windowName)
, mTexturesInitialized(false)
{
	
}

RenderSubsystem::~RenderSubsystem()
{

}

void RenderSubsystem::create(ApplicationPtr app,OUAN::ConfigurationPtr config)
{
	mApp=app;
	debugMessage = "";

	createRoot(config);
}

bool RenderSubsystem::init(ConfigurationPtr config)
{
	loadConfig();

	defineResources(config);
	
	if (!setupRenderSystem(config))
	{
		return false;
	}

	createRenderWindow(config);

	initResourceGroups(config);

	initTextures3D();

	return true;
}

void RenderSubsystem::initChangeWorldRenderer()
{
	mChangeWorldRenderer.reset(new ChangeWorldRenderer());
	mChangeWorldRenderer->init(mSceneManager,mWindow,mApp->getCameraManager()->getActiveCamera());
}

ChangeWorldRendererPtr RenderSubsystem::getChangeWorldRenderer()
{
	return mChangeWorldRenderer;
}

bool RenderSubsystem::loadConfig()
{
	Configuration config;
	std::string value;
	bool successBar, successCompositor;

	if (config.loadFromFile(BAR_CFG))
	{
		config.getOption("NUM_GROUPS", value); 
		mNumGroups = atoi(value.c_str());

		config.getOption("READ_PROPORTION", value); 
		mReadProportion = atof(value.c_str());

		successBar = true;
	}
	else
	{
		mNumGroups = 1;
		mReadProportion = 0.5f;
		successBar = false;
	}

	if (config.loadFromFile(COMPOSITOR_CFG))
	{
		config.getOption("BLOOM", BLOOM); 
		config.getOption("HDR", HDR); 
		config.getOption("RADIAL_BLUR", RADIAL_BLUR); 
		config.getOption("MOTION_BLUR", MOTION_BLUR); 
		config.getOption("CHANGEWORLD", CHANGEWORLD); 

		config.getOption("BLOOM_ACTIVATED_ALWAYS_DREAMS", value); 
		BLOOM_ACTIVATED_ALWAYS_DREAMS=Ogre::StringConverter::parseBool(value);

		config.getOption("BLOOM_ACTIVATED_ALWAYS_NIGHTMARES", value); 
		BLOOM_ACTIVATED_ALWAYS_NIGHTMARES=Ogre::StringConverter::parseBool(value);

		config.getOption("BLOOM_BLEND_MATERIAL", BLOOM_BLEND_MATERIAL); 
		config.getOption("MAX_BLOOM_BLEND", value); 
		MAX_BLOOM_BLEND=atof(value.c_str());

		config.getOption("HDR_ACTIVATED_ALWAYS_DREAMS", value); 
		HDR_ACTIVATED_ALWAYS_DREAMS=Ogre::StringConverter::parseBool(value);

		config.getOption("HDR_ACTIVATED_ALWAYS_NIGHTMARES", value); 
		HDR_ACTIVATED_ALWAYS_NIGHTMARES=Ogre::StringConverter::parseBool(value);

		config.getOption("RADIAL_BLUR_ACTIVATED_ALWAYS_DREAMS", value); 
		RADIAL_BLUR_ACTIVATED_ALWAYS_DREAMS=Ogre::StringConverter::parseBool(value);

		config.getOption("RADIAL_BLUR_ACTIVATED_ALWAYS_NIGHTMARES", value); 
		RADIAL_BLUR_ACTIVATED_ALWAYS_NIGHTMARES=Ogre::StringConverter::parseBool(value);

		config.getOption("MOTION_BLUR_ACTIVATED_ALWAYS_DREAMS", value); 
		MOTION_BLUR_ACTIVATED_ALWAYS_DREAMS=Ogre::StringConverter::parseBool(value);

		config.getOption("MOTION_BLUR_ACTIVATED_ALWAYS_NIGHTMARES", value); 
		MOTION_BLUR_ACTIVATED_ALWAYS_NIGHTMARES=Ogre::StringConverter::parseBool(value);

		successCompositor = true;
	}
	else
	{
		successCompositor = false;
	}

	return successBar && successCompositor;
}

void RenderSubsystem::cleanUp()
{
	clearScene();
}

void RenderSubsystem::createRoot(ConfigurationPtr config)
{
	std::string pluginsPath=DEFAULT_OGRE_PLUGINS_PATH;
	std::string configPath=DEFAULT_OGRE_CONFIG_PATH;
	std::string logPath=DEFAULT_OGRE_LOG_PATH;

	if (config.get())
	{
		if(config->hasOption(CONFIG_KEYS_OGRE_PLUGINS_PATH))
		{
			config->getOption(CONFIG_KEYS_OGRE_PLUGINS_PATH,pluginsPath);
		}
		if(config->hasOption(CONFIG_KEYS_OGRE_CONFIG_PATH))
		{
			config->getOption(CONFIG_KEYS_OGRE_CONFIG_PATH,configPath);
		}
		if(config->hasOption(CONFIG_KEYS_OGRE_LOG_PATH))
		{
			config->getOption(CONFIG_KEYS_OGRE_LOG_PATH,logPath);
		}
	}

	mRoot.reset(new Ogre::Root(pluginsPath, configPath, logPath));
}

RootPtr RenderSubsystem::getRoot() const
{
	return mRoot;
}

void RenderSubsystem::defineResources(ConfigurationPtr config)
{
	Ogre::String sectionName, typeName, resName;
	Ogre::ConfigFile cFile;
	std::string resourcesPath;

	if(config.get() && config->hasOption(CONFIG_KEYS_OGRE_RESOURCES_PATH))
	{
		config->getOption(CONFIG_KEYS_OGRE_RESOURCES_PATH,resourcesPath);
	}
	else 
	{
		resourcesPath=DEFAULT_OGRE_RESOURCES_PATH;
	}

	cFile.load(resourcesPath);
	Ogre::ConfigFile::SectionIterator secIt = cFile.getSectionIterator();
	while (secIt.hasMoreElements())
	{
		sectionName = secIt.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;
		for (it=settings->begin();it!=settings->end();++it)
		{
			typeName= it->first;
			resName = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resName,typeName,sectionName);
		}
	}
}

bool RenderSubsystem::setupRenderSystem(ConfigurationPtr config)
{
	//[TODO - Get rid of config dialog]
	return mRoot->showConfigDialog();
}

void RenderSubsystem::createRenderWindow(ConfigurationPtr config)
{
	mRoot->initialise(true, mWindowName);
	mWindow=mRoot->getAutoCreatedWindow();
	mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default Scene Manager");
}

void RenderSubsystem::createVisualDebugger(ConfigurationPtr config)
{	
	mNxOgreVisualDebugger = mApp->getPhysicsSubsystem()->getNxOgreWorld()->getVisualDebugger();
	mNxOgreVisualDebuggerRenderable = new OGRE3DRenderable(NxOgre::Enums::RenderableType_VisualDebugger);
	mNxOgreVisualDebugger->setRenderable(mNxOgreVisualDebuggerRenderable);
	mNxOgreVisualDebuggerNode = mSceneManager->getRootSceneNode()->createChildSceneNode(
		"visual_debugger#" + Application::getInstance()->getStringUniqueId());
	mNxOgreVisualDebuggerNode->attachObject(mNxOgreVisualDebuggerRenderable);
	mNxOgreVisualDebugger->setVisualisationMode(mApp->getDebugMode()!=DEBUGMODE_NONE?
		NxOgre::Enums::VisualDebugger_ShowAll:
		NxOgre::Enums::VisualDebugger_ShowNone);	
}

void RenderSubsystem::createDebugFloor(ConfigurationPtr config)
{
	//Initializing main floor
	Ogre::Plane *plane = new Ogre::Plane;
	plane->normal = Ogre::Vector3::UNIT_Y;
	plane->d = 0;

	Ogre::MeshManager::getSingleton().createPlane("debugFloorPlane",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *plane,
		1000, 1000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* pPlaneEnt = mSceneManager->createEntity("debugFloorEntity", "debugFloorPlane");
	pPlaneEnt->setCastShadows(false);
	pPlaneEnt->setMaterialName("GrassFloor");
	SceneNode * pPlaneNode = mSceneManager->getRootSceneNode()->createChildSceneNode(
		"plane_node#" + Application::getInstance()->getStringUniqueId());
	pPlaneNode->attachObject(pPlaneEnt);
}

void RenderSubsystem::initResourceGroups(ConfigurationPtr config)
{
	std::string mipmapNumber;

	if(config.get() && config->hasOption(CONFIG_KEYS_OGRE_DEFAULT_MIPMAPS_NUMBER))
	{
		config->getOption(CONFIG_KEYS_OGRE_DEFAULT_MIPMAPS_NUMBER,mipmapNumber);
	}
	else 
	{
		mipmapNumber=DEFAULT_OGRE_MIPMAPS_NUMBER;
	}
	
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(DEFAULT_OGRE_MIPMAPS_NUMBER);
	//Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	///////////////////////////////////////////////

	Camera* mCamera = mSceneManager->createCamera("LoadingBarCam");

	Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));

	mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    
	mLoadingBar.start(mWindow, (unsigned short)mNumGroups, (unsigned short)mNumGroups, mReadProportion);

	// Turn off rendering of everything except overlays
	mSceneManager->clearSpecialCaseRenderQueues();
	mSceneManager->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
	mSceneManager->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);

	// Initialise resource groups, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	ResourceGroupManager::getSingleton().loadResourceGroup(
		ResourceGroupManager::getSingleton().getWorldResourceGroupName(),
		false, true);

	// Back to full rendering
	mSceneManager->clearSpecialCaseRenderQueues();
	mSceneManager->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

	mLoadingBar.finish();
	mWindow->removeAllViewports();
}

void RenderSubsystem::createOverlays()
{
	//Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay")->show();
}

void RenderSubsystem::clear()
{
	/// Clear Scene manager
	mSceneManager->clearScene();
	//Prevent weird bug where a null pointer exception is thrown inside
	// sceneManager::setShadowTechnique() when resetting the game
	// (using soft shadowing)
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
}

RenderWindow* RenderSubsystem::getWindow() const
{
	return mWindow;
}

void RenderSubsystem::updateVisualDebugger()
{	
	mNxOgreVisualDebugger->draw();
	mNxOgreVisualDebuggerNode->needUpdate();
}

bool RenderSubsystem::render()
{
	//Logger::getInstance()->log("[PHYSICS RENDER]");
	return mRoot->renderOneFrame();
}

bool RenderSubsystem::isWindowClosed() const
{
	return !mWindow || mWindow->isClosed();
}

Ogre::SceneManager* RenderSubsystem::getSceneManager() const
{
	return mSceneManager;
}

Ogre::SceneManager * RenderSubsystem::setSceneParameters(Ogre::String name,TRenderComponentSceneParameters tRenderComponentSceneParameters)
{
	try
	{
		//initShadows();
		mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);

		//Set SceneManager parameters
		//mSceneManager->setAmbientLight(tRenderComponentSceneParameters.ambient);

		if(tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.active)
		{
			//Set SkyDome Dreams
			mSceneManager->setSkyDome(tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.active,
				tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.materialDreams,
				tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.curvature,
				tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.tiling,
				tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.distance);

			//Set SkyDome Nightmares
			mSceneManager->setSkyDome(tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.active,
				tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.materialNightmares,
				tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.curvature,
				tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.tiling,
				tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters.distance);
		}
	}
	catch(Ogre::Exception &/*e*/)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+name+" SceneManager!");
	}
	return mSceneManager;
}

Ogre::Light* RenderSubsystem::createLight(Ogre::String name,TRenderComponentLightParameters tRenderComponentLightParameters,
										  Ogre::String sceneNodeName)
{
	SceneNode *lightNode=0;
	Light *pLight=0;
	// Set light parameters and create it
	try
	{
		// Create the light
		pLight = mSceneManager->createLight(name);

		pLight->setQueryFlags(QUERYFLAGS_NONE);

		// Attach to Scene Manager
		if (sceneNodeName.empty())
			sceneNodeName=name;
		lightNode=mSceneManager->getSceneNode(sceneNodeName);
		lightNode->attachObject(pLight);

		//Set Light Parameters
		pLight->setType(tRenderComponentLightParameters.lighttype);
		pLight->setDiffuseColour(tRenderComponentLightParameters.diffuse);
		pLight->setSpecularColour(tRenderComponentLightParameters.specular);
		pLight->setDirection(tRenderComponentLightParameters.direction);
		pLight->setCastShadows(tRenderComponentLightParameters.castshadows);
		pLight->setAttenuation(
			tRenderComponentLightParameters.attenuation.x, 
			tRenderComponentLightParameters.attenuation.y, 
			tRenderComponentLightParameters.attenuation.z, 
			tRenderComponentLightParameters.attenuation.w);
		if (pLight->getType()==Ogre::Light::LT_SPOTLIGHT)
		{
			pLight->setSpotlightRange(Ogre::Degree(tRenderComponentLightParameters.lightrange.x),
				Ogre::Degree(tRenderComponentLightParameters.lightrange.y),
				tRenderComponentLightParameters.lightrange.z);
		}

		pLight->setPowerScale(tRenderComponentLightParameters.power);
	}
	catch(Ogre::Exception &/*e*/)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+name+" Light!");
	}
	return pLight;
}

Ogre::Entity* RenderSubsystem::createPlane(Ogre::String nodeName,Ogre::String name,TRenderComponentPlaneParameters tPlaneParameters)
{
	SceneNode *planeNode=0;
	Plane plane;
	Ogre::Entity * pPlaneEntity;
	// Set plane parameters and create it
	try
	{
		plane.d=tPlaneParameters.distance;
		plane.normal=tPlaneParameters.normal;
		// Create the plane
        MeshManager::getSingleton().createPlane(name,
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane,
            tPlaneParameters.width,tPlaneParameters.height,tPlaneParameters.Xsegments,tPlaneParameters.Ysegments,
			tPlaneParameters.hasNormals,tPlaneParameters.numCoordSets,tPlaneParameters.Utile,tPlaneParameters.Vtile, 
			Ogre::Vector3::UNIT_Z);

        pPlaneEntity = mSceneManager->createEntity( name, name );
        pPlaneEntity->setMaterialName(tPlaneParameters.material);

		//set Query flags
		pPlaneEntity->setQueryFlags(tPlaneParameters.cameraCollisionType);

		//set RenderQueue priortiy and group
		pPlaneEntity->setRenderQueueGroup(tPlaneParameters.queueID);

		//attach to Scene Manager
		planeNode=mSceneManager->getSceneNode(nodeName);
		planeNode->attachObject(pPlaneEntity);

	}
	catch(Ogre::Exception &/*e*/)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+name+" Plane!");
	}
	return pPlaneEntity;
}

Ogre::SceneNode * RenderSubsystem::createSceneNode(Ogre::String name,TRenderComponentPositionalParameters tRenderComponentPositionalParameters)
{

	SceneNode *pParentSceneNode = 0;
	SceneNode *sceneNode = 0;
	
	// Set SceneNode parameters and create it
	try
	{
		//Get Parent SceneNode
		if(tRenderComponentPositionalParameters.parentSceneNodeName.compare("SceneManager")==0)
		{
			pParentSceneNode = mSceneManager->getRootSceneNode();
		}
		else
		{
			pParentSceneNode = mSceneManager->getSceneNode(tRenderComponentPositionalParameters.parentSceneNodeName);
		}

		//Create SceneNode
		sceneNode = pParentSceneNode->createChildSceneNode(name);

		//Set SceneNode parameters
		sceneNode->setPosition(tRenderComponentPositionalParameters.position);
		sceneNode->setOrientation(tRenderComponentPositionalParameters.orientation);
		sceneNode->setScale(tRenderComponentPositionalParameters.scale);
		if(tRenderComponentPositionalParameters.autotracktarget.compare("None")!=0)
		{
			//TODO test this
			SceneNode *trackTarget;
			trackTarget=mSceneManager->getSceneNode(tRenderComponentPositionalParameters.autotracktarget);
			sceneNode->setAutoTracking(true,trackTarget);
		}
	}
	catch(Ogre::Exception &e)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+name+" SceneNode!: "+e.getDescription());
	}
	return sceneNode;
}

void RenderSubsystem::createMeshFile(OUAN::String meshfile, bool prepareForNormalMapping)
{
	MeshPtr mesh;
	Ogre::Mesh::LodDistanceList distanceList;

	distanceList.push_back(100);
	//distanceList.push_back(200);
	//distanceList.push_back(500);
	//distanceList.push_back(1000);
	//distanceList.push_back(1000);
	try
	{
		//Create the mesh file
		if (!MeshManager::getSingleton().resourceExists(meshfile))
		{
			//Logger::getInstance()->log("[LevelLoader] creating "+meshfile+" meshfile");
			mesh=MeshManager::getSingleton().load(meshfile,"General");
			//mesh->generateLodLevels(distanceList,Ogre::ProgressiveMesh::VRQ_CONSTANT,0.0001);
		    //mesh->createManualLodLevel(500, "node.mesh");
			//mesh->createManualLodLevel(500, "animallod3.mesh");
		}
	}
	catch(Ogre::Exception &/*e*/)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+meshfile+" mesh!");
	}
}

void RenderSubsystem::initMaterials()
{
	Ogre::MaterialManager::ResourceMapIterator it = Ogre::MaterialManager::getSingleton().getResourceIterator();
	Ogre::MaterialPtr material;

	while (it.hasMoreElements())
	{
		material=it.getNext();
		if(material->isLoaded())
		{
			if((material->getName().compare("red")!=0) &&
				(material->getName().compare("blue")!=0) &&
				(material->getName().compare("green")!=0) &&
				(material->getName().compare("yellow")!=0) &&
				(material->getName().compare("black")!=0) &&
				(material->getName().compare("white")!=0))
			{
				material->setLightingEnabled(false);
			}
		}
	}
	//TODO: DO THIS PROPERLY
	//LOAD DECAL TEXTURES
	Ogre::TextureManager::getSingleton().load("FlashlightDecal02.png","General");
	Ogre::TextureManager::getSingleton().load("Decal_filter.png","General");
}

void RenderSubsystem::initTextures3D()
{
	if (!mTexturesInitialized)
	{
		mTexture3D_1_8 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_1_8", "General", Ogre::TEX_TYPE_3D, 8, 8, 8, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_1_16 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_1_16", "General", Ogre::TEX_TYPE_3D, 16, 16, 16, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_1_32 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_1_32", "General", Ogre::TEX_TYPE_3D, 32, 32, 32, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_1_64 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_1_64", "General", Ogre::TEX_TYPE_3D, 64, 64, 64, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_1_128 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_1_128", "General", Ogre::TEX_TYPE_3D, 128, 128, 128, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_2_8 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_2_8", "General", Ogre::TEX_TYPE_3D, 8, 8, 8, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_2_16 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_2_16", "General", Ogre::TEX_TYPE_3D, 16, 16, 16, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_2_32 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_2_32", "General", Ogre::TEX_TYPE_3D, 32, 32, 32, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_2_64 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_2_64", "General", Ogre::TEX_TYPE_3D, 64, 64, 64, 0, Ogre::PF_A8R8G8B8);

		mTexture3D_2_128 = Ogre::TextureManager::getSingleton().createManual(
			"texture3D_2_128", "General", Ogre::TEX_TYPE_3D, 128, 128, 128, 0, Ogre::PF_A8R8G8B8);

		mTexturesInitialized = true;
	}
}

//TODO DO IT USING A MAP
Ogre::TexturePtr RenderSubsystem::getTexture3D(std::string texture3D){

	if (texture3D.compare("texture3D_8")==0)
	{
		return mTexture3D_1_8;
	} 
	else if (texture3D.compare("texture3D_1_16")==0)
	{
		return mTexture3D_1_16;
	} 
	else if (texture3D.compare("texture3D_1_32")==0)
	{
		return mTexture3D_1_32;
	} 
	else if (texture3D.compare("texture3D_1_64")==0)
	{
		return mTexture3D_1_64;
	} 
	else if (texture3D.compare("texture3D_1_128")==0)
	{
		return mTexture3D_1_128;
	} 
	else if (texture3D.compare("texture3D_2_8")==0)
	{
		return mTexture3D_2_8;
	} 
	else if (texture3D.compare("texture3D_2_16")==0)
	{
		return mTexture3D_2_16;
	} 
	else if (texture3D.compare("texture3D_2_32")==0)
	{
		return mTexture3D_2_32;
	} 
	else if (texture3D.compare("texture3D_2_64")==0)
	{
		return mTexture3D_2_64;
	} 
	else if (texture3D.compare("texture3D_2_128")==0)
	{
		return mTexture3D_2_128;
	} 
	else 
	{
		Ogre::TexturePtr nullPtr;
		return nullPtr;
	}
}
//
//void RenderSubsystem::setLightmaps(Ogre::Entity * pEntity)
//{
//	unsigned int i;
//	Ogre::String materialName = LIGHTMAP_PREFIX+pEntity->getName();
//	Ogre::String lightmapName = materialName+".dds";
//
//	Ogre::TexturePtr original_texture;
//	Ogre::TexturePtr lightmap_texture;
//	Ogre::TexturePtr new_texture;
//
//	Ogre::HardwarePixelBufferSharedPtr original_pixel_buffer;
//	Ogre::HardwarePixelBufferSharedPtr lightmap_pixel_buffer;
//	Ogre::HardwarePixelBufferSharedPtr new_pixel_buffer;
//
//	Ogre::Technique * technique;
//	Ogre::Pass * pass;
//	Ogre::TextureUnitState * texture_unit;
//
//	if( Ogre::ResourceGroupManager::getSingleton().resourceExists(DEFAULT_OGRE_RESOURCE_MANAGER_GROUP,lightmapName))
//	{
//		try
//		{
//			//Logger::getInstance()->log("[setLightmaps] Adding "+lightmapName+" Lightmap...");
//			for ( i = 0; i < pEntity->getNumSubEntities(); ++i)
//			{
//				// Get the material of this sub entity and build the clone material name
//				Ogre::SubEntity* subEnt = pEntity->getSubEntity(i);
//				Ogre::MaterialPtr material = subEnt->getMaterial();
//
//				// Get/Create the clone material
//				Ogre::MaterialPtr clone;
//				if (Ogre::MaterialManager::getSingleton().resourceExists(materialName))
//				{
//					clone = Ogre::MaterialManager::getSingleton().getByName(materialName);
//				}
//				else
//				{
//					// Clone the material
//					clone = material->clone(materialName);
//				}
//
//				// Apply the lightmap
//
//					//get technique
//				technique = clone->getTechnique(0);
//					//set current pass attributes
//				pass = technique->getPass(0);
//				texture_unit = pass->getTextureUnitState(0);
//
//				//clone original texture
//				original_texture=Ogre::TextureManager::getSingleton().getByName(texture_unit->getTextureName());
//				original_texture->copyToTexture(new_texture);
//
//				//apply lightmap
//				lightmap_texture=Ogre::TextureManager::getSingleton().getByName(lightmapName);
//				original_pixel_buffer = original_texture->getBuffer();
//				lightmap_pixel_buffer = lightmap_texture->getBuffer();
//				new_pixel_buffer = new_texture->getBuffer();
//
//				// create pixel boxes
//				//PixelBox original_pixelbox(
//				//	original_pixel_buffer->getWidth(), 
//				//	original_pixel_buffer->getHeight(),
//				//	original_pixel_buffer->getDepth(), 
//				//	original_pixel_buffer->getFormat(), 
//				//	buffer);          
//				//original_pixel_buffer->blitToMemory(original_pixelbox);
//
//				//PixelBox lightmap_pixelbox(
//				//	lightmap_pixel_buffer->getWidth(), 
//				//	lightmap_pixel_buffer->getHeight(),
//				//	lightmap_pixel_buffer->getDepth(), 
//				//	lightmap_pixel_buffer->getFormat(), 
//				//	buffer);          
//				//lightmap_pixel_buffer->blitToMemory(lightmap_pixelbox);
//
//				//PixelBox new_pixelbox(
//				//	new_pixel_buffer->getWidth(), 
//				//	new_pixel_buffer->getHeight(),
//				//	new_pixel_buffer->getDepth(), 
//				//	new_pixel_buffer->getFormat(), 
//				//	buffer); 
//
//				//PixelUtil::get
//
//				//new_pixelbox.
//				//original_pixel_buffer->blitToMemory(new_pixelbox);
//
//				//new_pixel_buffer->
//
//				//new_texture->setB
//
//				//load lightmap
//				new_texture->load();
//
//				//unload original texture and lightmap
//				original_texture->unload();
//				lightmap_texture->unload();
//
//				texture_unit->setTextureCoordSet(0);
//				texture_unit->setColourOperationEx(Ogre::LBX_MODULATE);
//					//create lightmap pass
//				pass = technique->createPass();
//				pass->setSceneBlending(Ogre::SBT_MODULATE);
//				pass->setDepthBias(1);
//				texture_unit = pass->createTextureUnitState();
//				texture_unit->setTextureName(lightmapName);
//				texture_unit->setTextureCoordSet(0);
//				texture_unit->setColourOperationEx(Ogre::LBX_MODULATE);
//
//				// Apply the cloned material to the sub entity.
//				subEnt->setMaterial(clone);
//			}
//		}
//		catch(Ogre::Exception &/*e*/)
//		{
//			Logger::getInstance()->log("[setLightmaps] Error adding "+lightmapName+" Lightmap!");
//		}
//	}
//	else
//	{
//		Logger::getInstance()->log("[setLightmaps] "+lightmapName+" Lightmap does not exist");
//	}
//}

Ogre::Entity* RenderSubsystem::createEntity(Ogre::String nodeName,Ogre::String name,TRenderComponentEntityParameters tRenderComponentEntityParameters)
{
	unsigned int i;
	Entity *pEntity = 0;
	SceneNode *pEntityNode = 0;
	try
	{
		//Create meshfile
		createMeshFile(tRenderComponentEntityParameters.meshfile);

		//create entity and set its parameters
		pEntity = mSceneManager->createEntity(name, tRenderComponentEntityParameters.meshfile);
		pEntity->setCastShadows(tRenderComponentEntityParameters.castshadows);

		//set subentities parameters
		for(i=0;i<tRenderComponentEntityParameters.tRenderComponentSubEntityParameters.size();i++)
		{
			createSubEntity(pEntity,
				i,
				tRenderComponentEntityParameters.tRenderComponentSubEntityParameters[i].material,
				tRenderComponentEntityParameters.tRenderComponentSubEntityParameters[i].visible);
		}

		//set Query flags
		pEntity->setQueryFlags(tRenderComponentEntityParameters.cameraCollisionType);

		//set RenderQueue priortiy and group
		pEntity->setRenderQueueGroup(tRenderComponentEntityParameters.queueID);

		//attach to Scene Manager
		pEntityNode=mSceneManager->getSceneNode(nodeName);
		pEntityNode->attachObject(pEntity);
	}
	catch(Ogre::Exception &/*e*/)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+name+" Entity!");
	}
	return pEntity;
}

void RenderSubsystem::createSubEntity(Ogre::Entity *pEntity,int num,OUAN::String material,bool visible)
{
	SubEntity *pSubEntity = 0;
		
	try
	{
		//get the SubEntity
		pSubEntity=pEntity->getSubEntity(num);

		//set SubEntity attributes
		pSubEntity->setMaterialName(material);
		pSubEntity->setVisible(visible);
	}
	catch(Ogre::Exception &/*e*/)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+pEntity->getName()+"'s SubEntity #"+StringConverter::toString(num)+"!");
	}
}

std::vector<ParticleUniverse::ParticleSystem*> RenderSubsystem::createParticleSystems(Ogre::String name,TRenderComponentParticleSystemParameters tRenderComponentParticleSystemParameters, RenderComponentPositionalPtr pRenderComponentPositional)
{
	std::vector<ParticleUniverse::ParticleSystem*> pParticleSystems(tRenderComponentParticleSystemParameters.poolSize);

	for (int i=0; i<tRenderComponentParticleSystemParameters.poolSize; i++)
	{
		ParticleUniverse::ParticleSystem* pParticleSystem = 0;
		Ogre::SceneNode* particleSystemNode = 0;
		Ogre::String particleName = name + "#" + Application::getInstance()->getStringUniqueId();
		/*
		Logger::getInstance()->log("INNER CREATION OF PARTICLE SYSTEM");
		Logger::getInstance()->log("PS INIT INFO");
		Logger::getInstance()->log(particleName);
		Logger::getInstance()->log(tRenderComponentParticleSystemParameters.templateName);
		Logger::getInstance()->log("PS INFO END");
		*/
		try
		{
			// Create ParticleSystem
			pParticleSystem = ParticleUniverse::ParticleSystemManager::getSingleton().createParticleSystem(
				particleName, 
				tRenderComponentParticleSystemParameters.templateName, 
				mApp->getRenderSubsystem()->getSceneManager());

			pParticleSystem->setQueryFlags(OUAN::QUERYFLAGS_NONE);

			//set RenderQueue priortiy and group
			pParticleSystem->setRenderQueueGroup(tRenderComponentParticleSystemParameters.queueID);

			// Create Particle System scene node where required
			if (tRenderComponentParticleSystemParameters.attached)
			{
				particleSystemNode=pRenderComponentPositional->getSceneNode()->createChildSceneNode(particleName);
			}
			else
			{
				particleSystemNode=mSceneManager->getRootSceneNode()->createChildSceneNode(particleName);
			}

			// Attach particle system to the created scene node
			particleSystemNode->attachObject(pParticleSystem);
		}
		catch(Ogre::Exception &/*e*/)
		{
			Logger::getInstance()->log("[LevelLoader] Error creating "+particleName+" ParticleSystem!");
		}
		
		pParticleSystems[i] = pParticleSystem;
	}

	return pParticleSystems;
}

void RenderSubsystem::createBillboard(Ogre::BillboardSet * pBillboardSet,OUAN::ColourValue colour,OUAN::Vector2 dimensions,OUAN::Vector3 position,OUAN::Real rotation,int texcoordindex,OUAN::Vector4 texrect)
{
	Billboard *pBillboard = 0;
		
	try
	{
		//create Billboard
		pBillboard = pBillboardSet->createBillboard(position);

		//set Billboard attributes
		pBillboard->setColour(colour);
		pBillboard->setDimensions(dimensions.x,dimensions.y);
		pBillboard->setRotation(Angle(rotation));
		pBillboard->setTexcoordIndex(texcoordindex);
		pBillboard->setTexcoordRect(texrect.x,texrect.y,texrect.z,texrect.w);

	}
	catch(Ogre::Exception &/*e*/)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+pBillboardSet->getName()+"'s Billboard!");
	}
}

Ogre::BillboardSet * RenderSubsystem::createBillboardSet(Ogre::String nodeName,Ogre::String name,TRenderComponentBillboardSetParameters tRenderComponentBillboardSetParameters)
{
	BillboardSet *billBoardSet = 0;
	SceneNode *billBoardSetNode = 0;
	try
	{
		//Create BillboardSet
		billBoardSet = mSceneManager->createBillboardSet(name);

		//Attach BillboardSet to SceneNode
		billBoardSetNode = mSceneManager->getSceneNode(nodeName);
		billBoardSetNode->attachObject(billBoardSet);

		//Set BillboardSet Attributes
		billBoardSet->setMaterialName(tRenderComponentBillboardSetParameters.material);
		billBoardSet->setDefaultHeight(tRenderComponentBillboardSetParameters.defaultheight);
		billBoardSet->setDefaultWidth(tRenderComponentBillboardSetParameters.defaultwidth);
		billBoardSet->setPointRenderingEnabled(tRenderComponentBillboardSetParameters.pointrendering);
		billBoardSet->setRenderingDistance(tRenderComponentBillboardSetParameters.renderdistance);
		billBoardSet->setSortingEnabled(tRenderComponentBillboardSetParameters.sorting);
		billBoardSet->setBillboardType(tRenderComponentBillboardSetParameters.billboardtype);
		billBoardSet->setBillboardOrigin(tRenderComponentBillboardSetParameters.billboardorigin);
		billBoardSet->setBillboardRotationType(tRenderComponentBillboardSetParameters.billboardrotation);

		billBoardSet->setQueryFlags(QUERYFLAGS_NONE);

		//set RenderQueue priortiy and group
		billBoardSet->setRenderQueueGroup(tRenderComponentBillboardSetParameters.queueID);

		// Create BillboardSet's Billboards
		for(unsigned int i=0;i<tRenderComponentBillboardSetParameters.tRenderComponentBillboardParameters.size();i++)
		{
			createBillboard(billBoardSet,
				tRenderComponentBillboardSetParameters.tRenderComponentBillboardParameters[i].colour,
				tRenderComponentBillboardSetParameters.tRenderComponentBillboardParameters[i].dimensions,
				tRenderComponentBillboardSetParameters.tRenderComponentBillboardParameters[i].position,
				tRenderComponentBillboardSetParameters.tRenderComponentBillboardParameters[i].rotation,
				tRenderComponentBillboardSetParameters.tRenderComponentBillboardParameters[i].texcoordindex,
				tRenderComponentBillboardSetParameters.tRenderComponentBillboardParameters[i].texrect);
		}

	}
	catch(Ogre::Exception &/*e*/)
	{
		Logger::getInstance()->log("[LevelLoader] Error creating "+name+" BillboardSet!");
	}
	return billBoardSet;
}
		
RenderComponentDecalPtr RenderSubsystem::createDecal(GameObjectPtr gameObject,TRenderComponentDecalParameters tRenderComponentDecalParameters)
{
	RenderComponentDecalPtr pRenderComponentDecal = RenderComponentDecalPtr(new RenderComponentDecal());

	Frustum * pDecalFrustum;
	SceneNode * pProjectorNode;

	pDecalFrustum = new Frustum();
    pProjectorNode = mSceneManager->getRootSceneNode()->createChildSceneNode("DecalProjectorNode");
    pProjectorNode->attachObject(pDecalFrustum);
    pProjectorNode->setPosition(0,5,0);


	//NOT WORKING YET

	return pRenderComponentDecal;
}

Ogre::String RenderSubsystem::getDebugMessage()
{
	return debugMessage;
}

void RenderSubsystem::setDebugMessage(Ogre::String debugMessage)
{
	this->debugMessage = debugMessage;
}

void RenderSubsystem::updateStats()
{
	static Ogre::String currFps = "Current FPS: ";
	static Ogre::String avgFps = "Average FPS: ";
	static Ogre::String bestFps = "Best FPS: ";
	static Ogre::String worstFps = "Worst FPS: ";
	static Ogre::String tris = "Triangle Count: ";
	static Ogre::String batches = "Batch Count: ";
	
	// update stats when necessary
	try {
		Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
		guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS)
			+" "+Ogre::StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS)
			+" "+Ogre::StringConverter::toString(stats.worstFrameTime)+" ms");

		Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));

		Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + Ogre::StringConverter::toString(stats.batchCount));
	}
	catch(...) { /* ignore */ }
}

void RenderSubsystem::updateDebugInfo()
{
	try {
		Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setTop(0);
		//Ogre::StringConverter::toString(x);
		guiDbg->setCaption(debugMessage);
	}
	catch(...) { /* ignore */ }
}

void RenderSubsystem::showVisualDebugger()
{
	if (mNxOgreVisualDebugger)
		mNxOgreVisualDebugger->setVisualisationMode(NxOgre::Enums::VisualDebugger_ShowAll);	
}

void RenderSubsystem::hideVisualDebugger()
{
	if (mNxOgreVisualDebugger)
		mNxOgreVisualDebugger->setVisualisationMode(NxOgre::Enums::VisualDebugger_ShowNone);		
}

void RenderSubsystem::showOverlay(const std::string& overlayName)
{
	Ogre::Overlay* ovl;
	if (ovl=Ogre::OverlayManager::getSingleton().getByName(overlayName))
		ovl->show();
	else Logger::getInstance()->log("[ShowOverlay] Error loading "+overlayName+" Overlay!");
}

void RenderSubsystem::hideOverlay(const std::string& overlayName)
{
	Ogre::Overlay* ovl;
	if (ovl=Ogre::OverlayManager::getSingleton().getByName(overlayName))
		ovl->hide();
	else Logger::getInstance()->log("[ShowOverlay] Error loading "+overlayName+" Overlay!");
}

void RenderSubsystem::pauseRendering()
{
	Ogre::ControllerManager::getSingleton().setTimeFactor(0);
}

void RenderSubsystem::resumeRendering()
{
	Ogre::ControllerManager::getSingleton().setTimeFactor(1.0);
}

void RenderSubsystem::clearScene()
{
	//mSceneManager->destroyAllCameras(); 
	//mSceneManager->clearScene();
	mRoot->destroySceneManager(mSceneManager);
	mWindow->removeAllViewports();
	//mApp->getGUISubsystem()->clearRenderer();
}

void RenderSubsystem::resetScene()
{
	clearScene();
	mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default Scene Manager");
}

void RenderSubsystem::captureScene(const std::string& name)
{
	Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual("RttTex",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, mWindow->getWidth(), mWindow->getHeight(), 0, PF_R8G8B8,
		TU_RENDERTARGET);
	Ogre::RenderTexture *renderTexture = texture->getBuffer()->getRenderTarget();

	renderTexture->addViewport(mApp->getCameraManager()->getActiveCamera());
	renderTexture->getViewport(0)->setClearEveryFrame(true);
	renderTexture->getViewport(0)->setBackgroundColour(ColourValue::Black);
	renderTexture->getViewport(0)->setOverlaysEnabled(false);		
	renderTexture->update();
	renderTexture->writeContentsToFile(name);
}

void RenderSubsystem::hideOverlayElement(const std::string& overlayName)
{
	Ogre::OverlayElement* overlayElem = Ogre::OverlayManager::getSingleton().getOverlayElement(overlayName);
	if (overlayElem)
	{
		overlayElem->hide();
	}
}

void RenderSubsystem::showOverlayElement(const std::string& overlayName)
{
	Ogre::OverlayElement* overlayElem = Ogre::OverlayManager::getSingleton().getOverlayElement(overlayName);
	if (overlayElem)
	{
		overlayElem->show();
	}
}

void RenderSubsystem::initShadows()
{
	// enable integrated additive shadows
	// actually, since we render the shadow map ourselves, it doesn't
	// really matter whether they are additive or modulative
	// as long as they are integrated v(O_o)v
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);	
	// we'll be self shadowing
	mSceneManager->setShadowTextureSelfShadow(true);

	std::string shadowTextureMaterial=DEFAULT_SHADOW_TEXTURE_CASTER_MATERIAL;
	int shadowTextureCount=DEFAULT_SHADOW_TEXTURE_COUNT;
	int shadowTextureSize=DEFAULT_SHADOW_TEXTURE_SIZE;
	Ogre::PixelFormat shadowTexturePixelFormat=DEFAULT_SHADOW_TEXTURE_PIXEL_FORMAT;
	bool shadowRenderBackFaces=DEFAULT_SHADOW_CASTER_RENDER_BACK_FACES;

	Configuration config;
	//TODO: Replace this with a more general graphics-config.
	if (config.loadFromFile(SHADOWS_CONFIG_PATH))
	{
		config.getOption(CONFIG_KEYS_SHADOW_TEXTURE_CASTER_MATERIAL,shadowTextureMaterial);
		shadowTextureCount=config.parseInt(CONFIG_KEYS_SHADOW_TEXTURE_COUNT);
		shadowTextureSize=config.parseInt(CONFIG_KEYS_SHADOW_TEXTURE_SIZE);
		shadowTexturePixelFormat=(Ogre::PixelFormat)config.parseInt(CONFIG_KEYS_SHADOW_TEXTURE_PIXEL_FORMAT);
		shadowRenderBackFaces=config.parseBool(CONFIG_KEYS_SHADOW_TEXTURE_CASTER_RENDER_BACK_FACES);
	}
	// our caster material
	mSceneManager->setShadowTextureCasterMaterial(shadowTextureMaterial);
	// note we have no "receiver".  all the "receivers" are integrated.

	// get the shadow texture count from the cfg file
	// (each light needs a shadow text.)
	mSceneManager->setShadowTextureCount(shadowTextureCount);
	// the size, too (1024 looks good with 3x3 or more filtering)
	mSceneManager->setShadowTextureSize(shadowTextureSize);

	// float 16 here.  we need the R and G channels.
	// float 32 works a lot better with a low/none VSM epsilon (wait till the shaders)
	// but float 16 is good enough and supports bilinear filtering on a lot of cards
	// (we should use _GR, but OpenGL doesn't really like it for some reason)
	mSceneManager->setShadowTexturePixelFormat(shadowTexturePixelFormat);

	// big NONO to render back faces for VSM.  it doesn't need any biasing 
	// so it's worthless (and rather problematic) to use the back face hack that
	// works so well for normal depth shadow mapping (you know, so you don't
	// get surface acne)
	mSceneManager->setShadowCasterRenderBackFaces(shadowRenderBackFaces);

	const unsigned numShadowRTTs = mSceneManager->getShadowTextureCount();
	for (unsigned i = 0; i < numShadowRTTs; ++i)
	{
		Ogre::TexturePtr tex = mSceneManager->getShadowTexture(i);
		Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
		vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1, 1));
		vp->setClearEveryFrame(true);
	}

	// and add the shader listener
	mSceneManager->addListener(&shadowListener);
}

//---------------------------
SSAOListener::SSAOListener()
:mCam(NULL)
{

}

void SSAOListener::setCamera(Ogre::Camera* cam)
{
	mCam=cam;
}

// this callback we will use to modify SSAO parameters
void SSAOListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
	if (pass_id != 42 || !mCam) // not SSAO, return
	{
		return;
	}

	// calculate the far-top-right corner in view-space
	Ogre::Vector3 farCorner = mCam->getViewMatrix(true) * mCam->getWorldSpaceCorners()[4];

	// get the pass
	Ogre::Pass *pass = mat->getBestTechnique()->getPass(0);

	// get the vertex shader parameters
	Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
	// set the camera's far-top-right corner
	if (params->_findNamedConstantDefinition("farCorner"))
	{
		params->setNamedConstant("farCorner", farCorner);
	}

	// get the fragment shader parameters
	params = pass->getFragmentProgramParameters();
	// set the projection matrix we need
	static const Ogre::Matrix4 CLIP_SPACE_TO_IMAGE_SPACE(
		0.5,    0,    0,  0.5,
		0,   -0.5,    0,  0.5,
		0,      0,    1,    0,
		0,      0,    0,    1);

	if (params->_findNamedConstantDefinition("ptMat"))
	{
		params->setNamedConstant("ptMat", CLIP_SPACE_TO_IMAGE_SPACE * mCam->getProjectionMatrixWithRSDepth());
	}

	if (params->_findNamedConstantDefinition("far"))
	{
		params->setNamedConstant("far", mCam->getFarClipDistance());
	}
}

//---------------------------
// this is a callback we'll be using to set up our shadow camera
void ShadowListener::shadowTextureCasterPreViewProj(Ogre::Light *light, Ogre::Camera *cam, size_t)
{
	// basically, here we do some forceful camera near/far clip attenuation
	// yeah.  simplistic, but it works nicely.  this is the function I was talking
	// about you ignoring above in the Mgr declaration.
	float range = light->getAttenuationRange();
	cam->setNearClipDistance(0.01);	
	cam->setFarClipDistance(range);
	// we just use a small near clip so that the light doesn't "miss" anything
	// that can shadow stuff.  and the far clip is equal to the lights' range.
	// (thus, if the light only covers 15 units of objects, it can only
	// shadow 15 units - the rest of it should be attenuated away, and not rendered)
}

// these are pure virtual but we don't need them...  so just make them empty
// otherwise we get "cannot declare of type Mgr due to missing abstract
// functions" and so on
void ShadowListener::shadowTexturesUpdated(size_t)
{

}

void ShadowListener::shadowTextureReceiverPreViewProj(Ogre::Light*, Ogre::Frustum*)
{

}

void ShadowListener::preFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*) 
{

}

void ShadowListener::postFindVisibleObjects(Ogre::SceneManager*, Ogre::SceneManager::IlluminationRenderStage, Ogre::Viewport*) 
{

}