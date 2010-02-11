#include "OgSceneLoader.h"
#include "tinyxml.h"
#include <Ogre.h>

using namespace std;
using namespace Ogre;

void OgSceneLoader::parseOgScene(const String &SceneName, const String &groupName, SceneManager *yourSceneMgr, SceneNode *pAttachNode, const String &sPrependNode)
{
	// set up shared object values
	m_sGroupName = groupName;
	mSceneMgr = yourSceneMgr;
	m_sPrependNode = sPrependNode;
	staticObjects.clear();
	dynamicObjects.clear();

	TiXmlDocument   *XMLDoc = 0;
	TiXmlElement   *XMLRoot;

	try
	{
		// Strip the path
		Ogre::String basename, path;
		Ogre::StringUtil::splitFilename(SceneName, basename, path);

		DataStreamPtr pStream = ResourceGroupManager::getSingleton().
			openResource( basename, groupName );

		String data = pStream->getAsString();
		// Open the .scene File
		XMLDoc = new TiXmlDocument();
		XMLDoc->Parse( data.c_str() );
		pStream->close();
		pStream.setNull();

		if( XMLDoc->Error() )
		{
			//We'll just log, and continue on gracefully
			LogManager::getSingleton().logMessage("[OgSceneLoader] The TiXmlDocument reported an error");
			delete XMLDoc;
			return;
		}
	}
	catch(...)
	{
		//We'll just log, and continue on gracefully
		LogManager::getSingleton().logMessage("[OgSceneLoader] Error creating TiXmlDocument");
		delete XMLDoc;
		return;
	}

	// Validate the File
	XMLRoot = XMLDoc->RootElement();
	if( String( XMLRoot->Value()) != "OGITORSCENE"  ) {
		LogManager::getSingleton().logMessage( "[OgSceneLoader] Error: Invalid .ogscene File. Missing <OGITORSCENE>" );
		delete XMLDoc;      
		return;
	}

	// figure out where to attach any nodes we create
	mAttachNode = pAttachNode;
	if(!mAttachNode)
		mAttachNode = mSceneMgr->getRootSceneNode();

	// Process the scene
	processScene(XMLRoot);

	// Close the XML File
	delete XMLDoc;
}

void OgSceneLoader::processScene(TiXmlElement *XMLRoot)
{
	// Process the scene parameters
	String version = getAttrib(XMLRoot, "version", "unknown");

	String message = "[OgSceneLoader] Parsing ogScene file with version " + version;
	//if(XMLRoot->Attribute("ID"))
	//	message += ", id " + String(XMLRoot->Attribute("ID"));
	//if(XMLRoot->Attribute("sceneManager"))
	//	message += ", scene manager " + String(XMLRoot->Attribute("sceneManager"));
	//if(XMLRoot->Attribute("minOgreVersion"))
	//	message += ", min. Ogre version " + String(XMLRoot->Attribute("minOgreVersion"));
	//if(XMLRoot->Attribute("author"))
	//	message += ", author " + String(XMLRoot->Attribute("author"));

	LogManager::getSingleton().logMessage(message);

	TiXmlElement *pElement;

	// Process PROJECT (?)
	pElement = XMLRoot->FirstChildElement("PROJECT");
	if(pElement)
		processProject(pElement);

	// Process OBJECT (?)
	pElement = XMLRoot->FirstChildElement("OBJECT");
	while(pElement)
	{
		processObject(pElement);
		pElement = pElement->NextSiblingElement("OBJECT");
	}
		

}

void OgSceneLoader::processProject(TiXmlElement *XMLNode)
{
	//TiXmlElement *pElement;
	//// Process resourceLocations (?)
	//pElement = XMLNode->FirstChildElement("resourceLocations");
	//if(pElement)
	//	processResourceLocations(pElement);


	//// Process environment (?)
	//pElement = XMLNode->FirstChildElement("environment");
	//if(pElement)
	//	processEnvironment(pElement);
}

void OgSceneLoader::processObject(TiXmlElement *XMLNode)
{
	String type = getAttrib(XMLNode, "typename");
	if( type.compare("SceneManager")==0)
	{
	}
	else if( type.compare("ViewPort Object")==0)
	{
	}
	else if( type.compare("Node Object")==0)
	{
		processSceneNode(XMLNode);
	}
	else if( type.compare("Light Object")==0)
	{
		processLight(XMLNode);
	}
	else if( type.compare("Entity Object")==0)
	{
		processEntity(XMLNode);
	}
	else if( type.compare("Particle Object")==0)
	{
		processParticleSystem(XMLNode);
	}
	else
	{
		LogManager::getSingleton().logMessage("Error reading "+type+" OBJECT");
	}
}

void OgSceneLoader::processResourceLocations(TiXmlElement *XMLNode)
{
	//TODO
}

void OgSceneLoader::processExternals(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void OgSceneLoader::processEnvironment(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process fog (?)
	pElement = XMLNode->FirstChildElement("fog");
	if(pElement)
		processFog(pElement);

	// Process skyBox (?)
	pElement = XMLNode->FirstChildElement("skyBox");
	if(pElement)
		processSkyBox(pElement);

	// Process skyDome (?)
	pElement = XMLNode->FirstChildElement("skyDome");
	if(pElement)
		processSkyDome(pElement);

	// Process skyPlane (?)
	pElement = XMLNode->FirstChildElement("skyPlane");
	if(pElement)
		processSkyPlane(pElement);

	// Process clipping (?)
	pElement = XMLNode->FirstChildElement("clipping");
	if(pElement)
		processClipping(pElement);

	// Process colourAmbient (?)
	pElement = XMLNode->FirstChildElement("colourAmbient");
	if(pElement)
//		mSceneMgr->setAmbientLight(parseColour(pElement));

	// Process colourBackground (?)
	//! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
	pElement = XMLNode->FirstChildElement("colourBackground");
	if(pElement)
		;//mSceneMgr->set(parseColour(pElement));

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if(pElement)
		processUserDataReference(pElement);
}

void OgSceneLoader::processTerrain(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void OgSceneLoader::processUserDataReference(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Implement this
}

void OgSceneLoader::processOctree(TiXmlElement *XMLNode)
{
	//! @todo Implement this
}

void OgSceneLoader::processLight(TiXmlElement *XMLNode)
{

	TiXmlElement *pElement;
	SceneNode *lightNode = 0;
	Entity *pEntity = 0;

	enum LightTypes
	{
		OGITOR_LT_POINT,
		OGITOR_LT_DIRECTIONAL,
		OGITOR_LT_SPOTLIGHT
	};

	int lighttype;
	ColourValue diffuse;
	ColourValue specular;
	Vector3 direction;
	bool castshadows;
	Vector3 lightrange;
	Vector4 attenuation;
	Real power;

	//Get Light name
	String name = getAttrib(XMLNode, "name");

	//Parse and create Scene node
	processSceneNode(XMLNode);

	// Process PROPERTY (?)
	pElement = XMLNode->FirstChildElement("PROPERTY");
	while(pElement)
	{
		// Get PropertyName
		String propertyName = getAttrib(pElement, "id");

		if( propertyName.compare("lighttype")==0)
		{
			lighttype=parseInt(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("diffuse")==0)
		{
			diffuse=parseColour(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("specular")==0)
		{
			specular=parseColour(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("direction")==0)
		{
			direction=parseVector3(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("castshadows")==0)
		{
			castshadows=parseBool(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("lightrange")==0)
		{
			lightrange=parseVector3(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("attenuation")==0)
		{
			attenuation=parseVector4(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("power")==0)
		{
			power=parseFloat(getAttrib(pElement, "value"));
		}
		pElement = pElement->NextSiblingElement("PROPERTY");
	}

	// Set light parameters and create it
	try
	{
		// Create the light
		Light *pLight = mSceneMgr->createLight(name);

		lightNode=mSceneMgr->getSceneNode(name);
		lightNode->attachObject(pLight);

		switch(lighttype)
		{
			case OGITOR_LT_POINT:
				pLight->setType(Light::LT_POINT);
				break;
			case OGITOR_LT_DIRECTIONAL:
				pLight->setType(Light::LT_DIRECTIONAL);
				break;
			case OGITOR_LT_SPOTLIGHT:
				pLight->setType(Light::LT_SPOTLIGHT);
				pLight->setSpotlightRange(Angle(lightrange.x), Angle(lightrange.y), lightrange.z);
				break;
			default:
				LogManager::getSingleton().logMessage("Light "+name+" has unrecognised light type!");
				break;
		}

		pLight->setDiffuseColour(diffuse);
		pLight->setSpecularColour(specular);
		pLight->setDirection(direction);
		pLight->setCastShadows(castshadows);
		pLight->setAttenuation(attenuation.x, attenuation.y, attenuation.z, attenuation.w);
		pLight->setPowerScale(power);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[OgSceneLoader] Error loading "+name+" light!");
	}
}

void OgSceneLoader::processCamera(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Process attributes
	String name = getAttrib(XMLNode, "name");
	String id = getAttrib(XMLNode, "id");
	Real fov = getAttribReal(XMLNode, "fov", 45);
	Real aspectRatio = getAttribReal(XMLNode, "aspectRatio", 1.3333);
	String projectionType = getAttrib(XMLNode, "projectionType", "perspective");

	// Create the camera
	Camera *pCamera = mSceneMgr->createCamera(name);
	if(pParent)
		pParent->attachObject(pCamera);
	
	// Set the field-of-view
	//! @todo Is this always in degrees?
	pCamera->setFOVy(Ogre::Degree(fov));

	// Set the aspect ratio
	pCamera->setAspectRatio(aspectRatio);
	
	// Set the projection type
	if(projectionType == "perspective")
		pCamera->setProjectionType(PT_PERSPECTIVE);
	else if(projectionType == "orthographic")
		pCamera->setProjectionType(PT_ORTHOGRAPHIC);


	TiXmlElement *pElement;

	// Process clipping (?)
	pElement = XMLNode->FirstChildElement("clipping");
	if(pElement)
	{
		Real nearDist = getAttribReal(pElement, "near");
		pCamera->setNearClipDistance(nearDist);

		Real farDist =  getAttribReal(pElement, "far");
		pCamera->setFarClipDistance(farDist);
	}

	// Process position (?)
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
		pCamera->setPosition(parseVector3(pElement));

	// Process rotation (?)
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
		pCamera->setOrientation(parseQuaternion(pElement));

	// Process normal (?)
	pElement = XMLNode->FirstChildElement("normal");
	if(pElement)
		;//!< @todo What to do with this element?

	// Process lookTarget (?)
	pElement = XMLNode->FirstChildElement("lookTarget");
	if(pElement)
		;//!< @todo Implement the camera look target

	// Process trackTarget (?)
	pElement = XMLNode->FirstChildElement("trackTarget");
	if(pElement)
		;//!< @todo Implement the camera track target

	// Process userDataReference (?)
	pElement = XMLNode->FirstChildElement("userDataReference");
	if(pElement)
		;//!< @todo Implement the camera user data reference
}

void OgSceneLoader::processSceneNode(TiXmlElement *XMLNode)
{

	TiXmlElement *pElement;
	String propertyName;

	SceneNode *pParent;
	SceneNode *sceneNode = 0;

	Vector3 position;
	Quaternion orientation;
	Vector3 scale;
	String autotracktarget="None";

	// Get Parent node
	pParent = getParentSceneNode(XMLNode);
	
	//Get SceneNode name
	String name = getAttrib(XMLNode, "name");

	// Process PROPERTY (?)
	pElement = XMLNode->FirstChildElement("PROPERTY");
	while(pElement)
	{
		// Get PropertyName
		String propertyName = getAttrib(pElement, "id");

		if( propertyName.compare("position")==0)
		{
			position= parseVector3(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("orientation")==0)
		{
			orientation= parseQuaternion(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("scale")==0)
		{
			scale= parseVector3(getAttrib(pElement, "value"));
		}
		else if( propertyName.compare("autotracktarget")==0)
		{
			autotracktarget= getAttrib(pElement, "value");
		}
		pElement = pElement->NextSiblingElement("PROPERTY");
	}
	
	// Set node parameters and create it
	try
	{
		sceneNode = pParent->createChildSceneNode(name);
		sceneNode->setPosition(position);
		sceneNode->setOrientation(orientation);
		sceneNode->setScale(scale);
		if(autotracktarget.compare("None")!=0)
		{
			//TODO test this
			SceneNode *trackTarget;
			trackTarget=mSceneMgr->getSceneNode(autotracktarget);
			sceneNode->setAutoTracking(true,trackTarget);
		}
		
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[OgSceneLoader] Error loading "+name+" node!");
	}

}

void OgSceneLoader::processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Is this correct? Cause I don't have a clue actually

	// Process attributes
	String nodeName = getAttrib(XMLNode, "nodeName");

	Node::TransformSpace relativeTo = Node::TS_PARENT;
	String sValue = getAttrib(XMLNode, "relativeTo");
	if(sValue == "local")
		relativeTo = Node::TS_LOCAL;
	else if(sValue == "parent")
		relativeTo = Node::TS_PARENT;
	else if(sValue == "world")
		relativeTo = Node::TS_WORLD;

	TiXmlElement *pElement;

	// Process position (?)
	Vector3 position;
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
		position = parseVector3(pElement);

	// Process localDirection (?)
	Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
	pElement = XMLNode->FirstChildElement("localDirection");
	if(pElement)
		localDirection = parseVector3(pElement);

	// Setup the look target
	try
	{
		if(!nodeName.empty())
		{
			SceneNode *pLookNode = mSceneMgr->getSceneNode(nodeName);
			position = pLookNode->_getDerivedPosition();
		}

		pParent->lookAt(position, relativeTo, localDirection);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[OgSceneLoader] Error processing a look target!");
	}
}

void OgSceneLoader::processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Process attributes
	String nodeName = getAttrib(XMLNode, "nodeName");

	TiXmlElement *pElement;

	// Process localDirection (?)
	Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
	pElement = XMLNode->FirstChildElement("localDirection");
	if(pElement)
		localDirection = parseVector3(pElement);

	// Process offset (?)
	Vector3 offset = Vector3::ZERO;
	pElement = XMLNode->FirstChildElement("offset");
	if(pElement)
		offset = parseVector3(pElement);

	// Setup the track target
	try
	{
		SceneNode *pTrackNode = mSceneMgr->getSceneNode(nodeName);
		pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[OgSceneLoader] Error processing a track target!");
	}
}

SceneNode* OgSceneLoader::getParentSceneNode(TiXmlElement *XMLNode)
{
	// Get Parent node
	String parentName = getAttrib(XMLNode, "parentnode");

	if(parentName.compare("SceneManager")==0)
	{
		return mSceneMgr->getRootSceneNode();
	}
	else
	{
		return mSceneMgr->getSceneNode(parentName);
	}

}

void OgSceneLoader::processEntity(TiXmlElement *XMLNode)
{
	unsigned int i;

	TiXmlElement *pElement;
	String propertyName;

	SceneNode *entityNode = 0;
	Entity *pEntity = 0;

	String meshfile;
	bool castshadows;
	String autotracktarget;

	struct SubEntityInfo
	{
		string material;
		bool visible;
	};
	std::vector<SubEntityInfo> subEntityInfo;

	//Parse and create Scene node
	processSceneNode(XMLNode);
	
	//Get Entity name
	String name = getAttrib(XMLNode, "name");

	// Process PROPERTY (?)
	pElement = XMLNode->FirstChildElement("PROPERTY");
	while(pElement)
	{
		// Get PropertyName
		String propertyName = getAttrib(pElement, "id");

		if( propertyName.compare("meshfile")==0)
		{
			meshfile= getAttrib(pElement, "value");
			//load the mesh file and create entity
			if (!MeshManager::getSingleton().resourceExists(meshfile))
			{
				MeshManager::getSingleton().load(meshfile, m_sGroupName);
			}
			pEntity = mSceneMgr->createEntity(name, meshfile);

			SubEntityInfo blank;
			for (i=0;i<pEntity->getNumSubEntities();i++)
			{
				subEntityInfo.push_back(blank);
			}
		}
		else if( propertyName.compare("castshadows")==0)
		{
			castshadows= parseBool(getAttrib(pElement, "value"));
		}
		else if( propertyName.substr(0,9).compare("subentity")==0)
		{
			int index=parseInt(propertyName.substr(9,1));

			if( propertyName.substr(12,8).compare("material")==0)
			{
				subEntityInfo[index].material=getAttrib(pElement, "value");
			}
			else if( propertyName.substr(12,7).compare("visible")==0)
			{
				subEntityInfo[index].visible=parseBool(getAttrib(pElement, "value"));
			}
		}
		pElement = pElement->NextSiblingElement("PROPERTY");
	}
	
	// Set entity parameters and create its node
	try
	{
		SubEntity *pSubEntity;
		pEntity->setCastShadows(castshadows);
		for(i=0;i<subEntityInfo.size();i++)
		{
			pSubEntity=pEntity->getSubEntity(i);
			pSubEntity->setMaterialName(subEntityInfo[i].material);
			pSubEntity->setVisible(subEntityInfo[i].visible);
			
		}

		entityNode=mSceneMgr->getSceneNode(name);
		entityNode->attachObject(pEntity);

	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[OgSceneLoader] Error loading "+name+" entity!");
	}

	//LogManager::getSingleton().logMessage("Entity "+name+" loaded");

	subEntityInfo.clear();
}

void OgSceneLoader::processParticleSystem(TiXmlElement *XMLNode)
{

	TiXmlElement *pElement;
	String propertyName;

	SceneNode *particleSystemNode = 0;
	ParticleSystem *particleSystem = 0;

	bool castshadows;
	String particle;

	//Parse and create Scene node
	processSceneNode(XMLNode);
	
	//Get Entity name
	String name = getAttrib(XMLNode, "name");

	// Process PROPERTY (?)
	pElement = XMLNode->FirstChildElement("PROPERTY");
	while(pElement)
	{
		// Get PropertyName
		String propertyName = getAttrib(pElement, "id");

		if( propertyName.compare("particle")==0)
		{
			particle=getAttrib(pElement, "value");
		}
		else if( propertyName.compare("castshadows")==0)
		{
			castshadows=parseBool(getAttrib(pElement, "value"));
		}
		pElement = pElement->NextSiblingElement("PROPERTY");
	}
	
	// Set particle system parameters
	try
	{
		particleSystem = mSceneMgr->createParticleSystem(name, particle);
		particleSystemNode=mSceneMgr->getSceneNode(name);
		particleSystemNode->attachObject(particleSystem);

		particleSystem->setCastShadows(castshadows);

	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[OgSceneLoader] Error loading "+name+" particle system!");
	}
}

void OgSceneLoader::processBillboardSet(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Implement this
}

void OgSceneLoader::processPlane(TiXmlElement *XMLNode, SceneNode *pParent)
{
	//! @todo Implement this
}

void OgSceneLoader::processFog(TiXmlElement *XMLNode)
{
	// Process attributes
	Real expDensity = getAttribReal(XMLNode, "expDensity", 0.001);
	Real linearStart = getAttribReal(XMLNode, "linearStart", 0.0);
	Real linearEnd = getAttribReal(XMLNode, "linearEnd", 1.0);

	FogMode mode = FOG_NONE;
	String sMode = getAttrib(XMLNode, "mode");
	if(sMode == "none")
		mode = FOG_NONE;
	else if(sMode == "exp")
		mode = FOG_EXP;
	else if(sMode == "exp2")
		mode = FOG_EXP2;
	else if(sMode == "linear")
		mode = FOG_LINEAR;

	TiXmlElement *pElement;

	// Process colourDiffuse (?)
	ColourValue colourDiffuse = ColourValue::White;
	pElement = XMLNode->FirstChildElement("colourDiffuse");
	if(pElement)
		colourDiffuse = parseColour(pElement);

	// Setup the fog
	mSceneMgr->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
}

void OgSceneLoader::processSkyBox(TiXmlElement *XMLNode)
{
	// Process attributes
	String material = getAttrib(XMLNode, "material");
	Real distance = getAttribReal(XMLNode, "distance", 5000);
	bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	TiXmlElement *pElement;

	// Process rotation (?)
	Quaternion rotation = Quaternion::IDENTITY;
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
		rotation = parseQuaternion(pElement);

	// Setup the sky box
	;//mSceneMgr->setSkyBox(true, material, distance, drawFirst, rotation, m_sGroupName);
}

void OgSceneLoader::processSkyDome(TiXmlElement *XMLNode)
{
	// Process attributes
	String material = XMLNode->Attribute("material");
	Real curvature = getAttribReal(XMLNode, "curvature", 10);
	Real tiling = getAttribReal(XMLNode, "tiling", 8);
	Real distance = getAttribReal(XMLNode, "distance", 4000);
	bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	TiXmlElement *pElement;

	// Process rotation (?)
	Quaternion rotation = Quaternion::IDENTITY;
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
		rotation = parseQuaternion(pElement);

	// Setup the sky dome
	;//mSceneMgr->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation, 16, 16, -1, m_sGroupName);
}

void OgSceneLoader::processSkyPlane(TiXmlElement *XMLNode)
{
	// Process attributes
	String material = getAttrib(XMLNode, "material");
	Real planeX = getAttribReal(XMLNode, "planeX", 0);
	Real planeY = getAttribReal(XMLNode, "planeY", -1);
	Real planeZ = getAttribReal(XMLNode, "planeX", 0);
	Real planeD = getAttribReal(XMLNode, "planeD", 5000);
	Real scale = getAttribReal(XMLNode, "scale", 1000);
	Real bow = getAttribReal(XMLNode, "bow", 0);
	Real tiling = getAttribReal(XMLNode, "tiling", 10);
	bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	// Setup the sky plane
	Plane plane;
	plane.normal = Vector3(planeX, planeY, planeZ);
	plane.d = planeD;
	mSceneMgr->setSkyPlane(true, plane, material, scale, tiling, drawFirst, bow, 1, 1, m_sGroupName);
}

void OgSceneLoader::processClipping(TiXmlElement *XMLNode)
{
	//! @todo Implement this

	// Process attributes
	Real fNear = getAttribReal(XMLNode, "near", 0);
	Real fFar = getAttribReal(XMLNode, "far", 1);
}

String OgSceneLoader::getAttrib(TiXmlElement *XMLNode, const String &attrib, const String &defaultValue)
{
	if(XMLNode->Attribute(attrib.c_str()))
		return XMLNode->Attribute(attrib.c_str());
	else
		return defaultValue;
}

Real OgSceneLoader::getAttribReal(TiXmlElement *XMLNode, const String &attrib, Real defaultValue)
{
	if(XMLNode->Attribute(attrib.c_str()))
		return StringConverter::parseReal(XMLNode->Attribute(attrib.c_str()));
	else
		return defaultValue;
}

bool OgSceneLoader::getAttribBool(TiXmlElement *XMLNode, const String &attrib, bool defaultValue)
{
	if(!XMLNode->Attribute(attrib.c_str()))
		return defaultValue;

	if(String(XMLNode->Attribute(attrib.c_str())) == "true")
		return true;

	return false;
}


Vector3 OgSceneLoader::parseVector3(String value)
{
	return StringConverter::parseVector3(value);
}

Vector4 OgSceneLoader::parseVector4(String value)
{
	return StringConverter::parseVector4(value);
}

Quaternion OgSceneLoader::parseQuaternion(String value)
{

	Vector4 orientation=StringConverter::parseVector4(value);
	return Quaternion(orientation.x,orientation.y,orientation.z,orientation.w);
}

ColourValue OgSceneLoader::parseColour(String value)
{
	Vector4 color=StringConverter::parseVector4(value);
	return ColourValue(color.x,color.y,color.z,color.w);
}

bool OgSceneLoader::parseBool(String value)
{
	return StringConverter::parseBool(value);
}

int OgSceneLoader::parseInt(String value)
{
	return StringConverter::parseInt(value);
}

float OgSceneLoader::parseFloat(String value)
{
	return StringConverter::parseReal(value);
}

Vector3 OgSceneLoader::parseVector3(TiXmlElement *XMLNode)
{
	return Vector3(
		StringConverter::parseReal(XMLNode->Attribute("x")),
		StringConverter::parseReal(XMLNode->Attribute("y")),
		StringConverter::parseReal(XMLNode->Attribute("z"))
	);
}

Quaternion OgSceneLoader::parseQuaternion(TiXmlElement *XMLNode)
{
	//! @todo Fix this crap!

	Quaternion orientation;

	if(XMLNode->Attribute("qx"))
	{
		orientation.x = StringConverter::parseReal(XMLNode->Attribute("qx"));
		orientation.y = StringConverter::parseReal(XMLNode->Attribute("qy"));
		orientation.z = StringConverter::parseReal(XMLNode->Attribute("qz"));
		orientation.w = StringConverter::parseReal(XMLNode->Attribute("qw"));
	}
	else if(XMLNode->Attribute("axisX"))
	{
		Vector3 axis;
		axis.x = StringConverter::parseReal(XMLNode->Attribute("axisX"));
		axis.y = StringConverter::parseReal(XMLNode->Attribute("axisY"));
		axis.z = StringConverter::parseReal(XMLNode->Attribute("axisZ"));
		Real angle = StringConverter::parseReal(XMLNode->Attribute("angle"));;
		orientation.FromAngleAxis(Ogre::Angle(angle), axis);
	}
	else if(XMLNode->Attribute("angleX"))
	{
		Vector3 axis;
		axis.x = StringConverter::parseReal(XMLNode->Attribute("angleX"));
		axis.y = StringConverter::parseReal(XMLNode->Attribute("angleY"));
		axis.z = StringConverter::parseReal(XMLNode->Attribute("angleZ"));
		//orientation.FromAxes(&axis);
		//orientation.F
	}

	return orientation;
}

ColourValue OgSceneLoader::parseColour(TiXmlElement *XMLNode)
{
	return ColourValue(
		StringConverter::parseReal(XMLNode->Attribute("r")),
		StringConverter::parseReal(XMLNode->Attribute("g")),
		StringConverter::parseReal(XMLNode->Attribute("b")),
		XMLNode->Attribute("a") != NULL ? StringConverter::parseReal(XMLNode->Attribute("a")) : 1
	);
}

String OgSceneLoader::getProperty(const String &ndNm, const String &prop)
{
	for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
	{
		if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
		{
			return nodeProperties[i].valueName;
		}
	}

	return "";
}

void OgSceneLoader::processUserDataReference(TiXmlElement *XMLNode, Entity *pEntity)
{
	String str = XMLNode->Attribute("id");
	pEntity->setUserAny(Any(str));
}
