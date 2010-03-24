#include "LevelLoader.h"
#include "XMLParser.h"
#include "XMLGameObject.h"
#include "../Application.h"
#include "../Game/GameWorldManager.h"
#include "../Graphics/TrajectoryManager/Trajectory.h"
#include "../Graphics/TrajectoryManager/TrajectoryNode.h"
#include "../Component/Component.h"
#include "../Game/GameObject/GameObject.h"
#include "../Game/GameObject/GameObjectLight.h"
#include "../Game/GameObject/GameObjectBillboardSet.h"
#include "../Game/GameObject/GameObjectParticleSystem.h"
#include "../Game/GameObject/GameObjectTerrainTriangle.h"
#include "../Game/GameObject/GameObjectTerrainConvex.h"
#include "../Game/GameObject/GameObjectScene.h"
#include "../Game/GameObject/GameObjectOny.h"
#include "../Game/GameObject/GameObjectTripollo.h"
#include "../Game/GameObject/GameObjectItem1UP.h"
#include "../Game/GameObject/GameObjectItemMaxHP.h"
#include "../Game/GameObject/GameObjectEye.h"
#include "../Game/GameObject/GameObjectPortal.h"
#include "../Game/GameObject/GameObjectCamera.h"
#include "../Game/GameObject/GameObjectTriggerBox.h"
#include "../Game/GameObject/GameObjectTriggerCapsule.h"
#include "../Game/GameObject/GameObjectViewport.h"
#include "../Game/GameObject/GameObjectProvisionalEntity.h"
#include "../Graphics/RenderComponent/RenderComponent.h"
#include "../Graphics/RenderComponent/RenderComponentBillboardSet.h"
#include "../Graphics/RenderComponent/RenderComponentCamera.h"
#include "../Graphics/RenderComponent/RenderComponentEntity.h"
#include "../Graphics/RenderComponent/RenderComponentLight.h"
#include "../Graphics/RenderComponent/RenderComponentParticleSystem.h"
#include "../Graphics/RenderComponent/RenderComponentScene.h"
#include "../Graphics/RenderComponent/RenderComponentPositional.h"
#include "../Graphics/RenderComponent/RenderComponentViewport.h"
#include "../Physics/PhysicsComponent/PhysicsComponent.h"
#include "../Physics/PhysicsComponent/PhysicsComponentCharacter.h"
#include "../Physics/PhysicsComponent/PhysicsComponentComplex.h"
#include "../Physics/PhysicsComponent/PhysicsComponentComplexConvex.h"
#include "../Physics/PhysicsComponent/PhysicsComponentComplexTriangle.h"
#include "../Physics/PhysicsComponent/PhysicsComponentSimple.h"
#include "../Physics/PhysicsComponent/PhysicsComponentSimpleCapsule.h"
#include "../Physics/PhysicsComponent/PhysicsComponentSimpleBox.h"

using namespace OUAN;

LevelLoader::LevelLoader()
{
	DEFAULT_TIME_TO_NEXT_NODE = 3;
}
LevelLoader::~LevelLoader(){}

void LevelLoader::init(OUAN::ApplicationPtr app)
{
	this->mGameWorldManager=app->getGameWorldManager();
	this->mXMLParser.init();
}

void LevelLoader::loadLevel(String level)
{
	Ogre::LogManager::getSingleton().logMessage("[LevelLoader] Loading level "+level);

	//clear parser content
	mXMLParser.clearLevelInfo();

	//Parse Level's GameObjects
	mXMLParser.parseLevel(level);

	//Process Level's GameObjects
	processGameObjects();

	//Process Level's Trajectories
	processTrajectories();

	//clear information, as we do not need it anymore
	mXMLParser.clearLevelInfo();

	Ogre::LogManager::getSingleton().logMessage("[LevelLoader] Loading level "+level+" Done!");
}

void LevelLoader::processGameObjects()
{
	XMLGameObjectContainerIterator it;

	for(it = mXMLParser.XMLGameObjectContainer.begin(); it !=mXMLParser.XMLGameObjectContainer.end(); it++)
	{
		processGameObject(&it->second);
	}

}

void LevelLoader::processGameObject(XMLGameObject* gameObject)
{
	String gameObjectType;

	try
	{
		gameObjectType=gameObject->gameObjectType;

		Ogre::LogManager::getSingleton().logMessage("[LevelLoader] Loading GameObject "+gameObject->name);

		if( gameObjectType.compare(GAME_OBJECT_TYPE_ONY)==0)
		{
			processGameObjectOny(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_SCENE)==0)
		{
			processGameObjectScene(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_VIEWPORT)==0)
		{
			processGameObjectViewport(gameObject);
			//Process Viewport Camera
			processGameObjectViewportCamera(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TRIPOLLO)==0)
		{
			processGameObjectTripollo(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TERRAIN)==0)
		{
			processGameObjectTerrain(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_LIGHT)==0)
		{
			processGameObjectLight(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_PARTICLESYSTEM)==0)
		{
			processGameObjectParticleSystem(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_BILLBOARDSET)==0)
		{
			processGameObjectBillboardSet(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_EYE)==0)
		{
			processGameObjectEye(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_ITEM_1UP)==0)
		{
			processGameObjectItem1UP(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_ITEM_MAXHP)==0)
		{
			processGameObjectItemMaxHP(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_PORTAL)==0)
		{
			processGameObjectPortal(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_CAMERA)==0)
		{
			processGameObjectCamera(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TRIGGERBOX)==0)
		{
			processGameObjectTriggerBox(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TRIGGERCAPSULE)==0)
		{
			processGameObjectTriggerCapsule(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_NIGHTGOBLIN)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_CRYKING)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_DRAGON)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_MAGICCLOCK)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_PILLOW)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_FLASHLIGHT)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TENTETIESO)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_BEE_BUTTERFLY)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_DIAMONDTREE)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_STORYBOOK)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_HEART)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_DIAMOND)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_SCAREDPLANT)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_CLOCKPIECE)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TRIPOLLITO)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_SNAKECREEPER)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_CARNIVOROUSPLANT)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_SCEPTER)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TREE1)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TREE2)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TREE3)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TREE4)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TREE5)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TREE6)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TREE7)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_TREE8)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_DOOR)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_PLATAFORM)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else if( gameObjectType.compare(GAME_OBJECT_TYPE_BUSH)==0)
		{
			processGameObjectProvisionalEntity(gameObject);
		}
		else
		{
			Ogre::LogManager::getSingleton().logMessage("[LevelLoader] Error processing Game Object with type "+gameObjectType+" , the specified type does not exist");
		}
	}
	catch( std::string error )
	{
		Ogre::LogManager::getSingleton().logMessage("ERROR! [LevelLoader] Error processing Game Object "+gameObject->name+": "+error);
	}

}

void LevelLoader::processTrajectories()
{
	XMLTrajectoryContainerIterator it;

	try
	{
		for(it = mXMLParser.XMLTrajectoryContainer.begin(); it !=mXMLParser.XMLTrajectoryContainer.end(); it++)
		{
			Ogre::LogManager::getSingleton().logMessage("[LevelLoader] Loading Trajectory "+it->first);

			processTrajectory(&it->second);

		}
	}
	catch( std::string error )
	{
		Ogre::LogManager::getSingleton().logMessage("[LevelLoader] Error processing Trajectory "+it->first+": "+error);
	}

}
TTrajectoryNodeParameters LevelLoader::processTrajectoryNode(TiXmlElement *XMLNode)
{
	TTrajectoryNodeParameters tTrajectoryNodeParameters;

	//Get Trajectory node name
	tTrajectoryNodeParameters.nodeName=getAttrib(XMLNode, "name");

	//Get Trajectory Node properties
	tTrajectoryNodeParameters.position=getPropertyVector3(XMLNode,"position");
	tTrajectoryNodeParameters.orientation=getPropertyQuaternion(XMLNode,"orientation");

	//Ogitor Markers have an offset of 90 degrees for Y axis rotation
	tTrajectoryNodeParameters.orientation=tTrajectoryNodeParameters.orientation*Ogre::Quaternion(Ogre::Math::Sqrt(0.5),0,Ogre::Math::Sqrt(0.5),0);

	try
	{
		tTrajectoryNodeParameters.timeToNextNode=getPropertyReal(XMLNode,"trajectorynode::time to next node");
		if(tTrajectoryNodeParameters.timeToNextNode<0)
		{
			throw "time to next node must be positive";
		}
	}
	catch( std::string error )
	{
		tTrajectoryNodeParameters.timeToNextNode=DEFAULT_TIME_TO_NEXT_NODE;
	}

	return tTrajectoryNodeParameters;
}

void LevelLoader::processTrajectory(XMLTrajectory *pXMLTrajectory)
{
	OUAN::TTrajectoryParameters tTrajectoryParameters;
	unsigned int i;

	//Get Trajectory name
	tTrajectoryParameters.name=pXMLTrajectory->name;

	//Process Trajectory nodes
	for(i=0;i<pXMLTrajectory->trajectoryNodes.size();i++)
	{
		//Process Trajectory Node
		tTrajectoryParameters.tTrajectoryNodeParameters.push_back(processTrajectoryNode(pXMLTrajectory->trajectoryNodes[i]));
	}

	//Create Trajectory 
	mGameWorldManager->createTrajectory(tTrajectoryParameters);
}

void LevelLoader::processGameObjectScene(XMLGameObject* gameObject)
{
	OUAN::TGameObjectSceneParameters tGameObjectSceneParameters;

	try
	{
		//Check errors
		if(!gameObject->getMainXMLNode()) throw SCENE_NODE_NOT_FOUND;
		if(!gameObject->XMLNodeCustomProperties) throw SCENE_NODE_NOT_FOUND;

		//Get Scene name
		tGameObjectSceneParameters.name = gameObject->name;

		//Get SceneManager properties
		tGameObjectSceneParameters.tRenderComponentSceneParameters = processRenderComponentScene(gameObject->getMainXMLNode(),gameObject->XMLNodeCustomProperties);
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectScene(tGameObjectSceneParameters);
}

void LevelLoader::processGameObjectViewport(XMLGameObject* gameObject)
{
	OUAN::TGameObjectViewportParameters tGameObjectViewportParameters;
	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeDreams) throw VIEWPORT_NODE_NOT_FOUND;

		//Get name
		tGameObjectViewportParameters.name = gameObject->name;

		//Get RenderComponentViewport
		tGameObjectViewportParameters.tRenderComponentViewportParameters=processRenderComponentViewport(gameObject->getMainXMLNode());
	}
	catch( std::string error )
	{
		throw error;
		return;
	}
	//Create GameObject
	mGameWorldManager->createGameObjectViewport(tGameObjectViewportParameters);
}

void LevelLoader::processGameObjectViewportCamera(XMLGameObject* gameObject)
{
	OUAN::TGameObjectCameraParameters  tGameObjectCameraParameters;

	try
	{
		//Get names
		tGameObjectCameraParameters.name = "Camera#Viewport";

		//Get RenderComponentCamera
		tGameObjectCameraParameters.tRenderComponentCameraParameters=processRenderComponentCameraViewport(gameObject->getMainXMLNode());
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectCamera(tGameObjectCameraParameters);
}

void LevelLoader::processGameObjectOny(XMLGameObject* gameObject)
{
	OUAN::TGameObjectOnyParameters tGameObjectOnyParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeDreams) throw DREAMS_NODE_NOT_FOUND;
		if(!gameObject->XMLNodeNightmares) throw NIGHTMARES_NODE_NOT_FOUND;
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectOnyParameters.dreamsName = gameObject->dreamsName;
		tGameObjectOnyParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectOnyParameters.name = gameObject->name;

		//Get logic component
		tGameObjectOnyParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentEntityDreams
		tGameObjectOnyParameters.tRenderComponentEntityDreamsParameters = processRenderComponentEntity(gameObject->XMLNodeDreams);
		//Get RenderComponentEntityNightmares
		tGameObjectOnyParameters.tRenderComponentEntityNightmaresParameters = processRenderComponentEntity(gameObject->XMLNodeNightmares);

		//Get RenderComponentPositional
		tGameObjectOnyParameters.tRenderComponentPositionalParameters = processRenderComponentPositional(gameObject->getMainXMLNode());

		//Get PhysicsComponentCharacter
		tGameObjectOnyParameters.tPhysicsComponentCharacterParameters = processPhysicsComponentCharacter(gameObject->XMLNodeCustomProperties);
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectOny(tGameObjectOnyParameters);

}

void LevelLoader::processGameObjectProvisionalEntity(XMLGameObject* gameObject)
{
	OUAN::TGameObjectProvisionalEntityParameters tGameObjectProvisionalEntityParameters;

	try
	{
		//Get names
		tGameObjectProvisionalEntityParameters.dreamsName = gameObject->dreamsName;
		tGameObjectProvisionalEntityParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectProvisionalEntityParameters.name = gameObject->name;

		//Get Logic Component
		tGameObjectProvisionalEntityParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		if(tGameObjectProvisionalEntityParameters.logicComponentParameters.existsInDreams)
		{
			//Get RenderComponentEntityDreams
			tGameObjectProvisionalEntityParameters.tRenderComponentEntityDreamsParameters=processRenderComponentEntity(gameObject->XMLNodeDreams);
		}

		if(tGameObjectProvisionalEntityParameters.logicComponentParameters.existsInNightmares)
		{
			//Get RenderComponentEntityNightmares
			tGameObjectProvisionalEntityParameters.tRenderComponentEntityNightmaresParameters=processRenderComponentEntity(gameObject->XMLNodeNightmares);
		}

		//Get RenderComponentPositional
		tGameObjectProvisionalEntityParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->getMainXMLNode());

		//TODO: EXTRACT PHYSICS PARAMETERS FROM FILE
		tGameObjectProvisionalEntityParameters.tPhysicsComponentSimpleBoxParameters.mass=0;
		tGameObjectProvisionalEntityParameters.tPhysicsComponentSimpleBoxParameters.lengthX=6;
		tGameObjectProvisionalEntityParameters.tPhysicsComponentSimpleBoxParameters.lengthY=20;
		tGameObjectProvisionalEntityParameters.tPhysicsComponentSimpleBoxParameters.lengthZ=6;
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectProvisionalEntity(tGameObjectProvisionalEntityParameters);
}


void LevelLoader::processGameObjectTripollo(XMLGameObject* gameObject)
{
	OUAN::TGameObjectTripolloParameters tGameObjectTripolloParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeDreams) throw DREAMS_NODE_NOT_FOUND;
		if(!gameObject->XMLNodeNightmares) throw NIGHTMARES_NODE_NOT_FOUND;
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectTripolloParameters.dreamsName = gameObject->dreamsName;
		tGameObjectTripolloParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectTripolloParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectTripolloParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentEntityDreams
		tGameObjectTripolloParameters.tRenderComponentEntityDreamsParameters = processRenderComponentEntity(gameObject->XMLNodeDreams);
		
		//Get RenderComponentEntityNightmares
		tGameObjectTripolloParameters.tRenderComponentEntityNightmaresParameters = processRenderComponentEntity(gameObject->XMLNodeNightmares);

		//Get RenderComponentPositional
		tGameObjectTripolloParameters.tRenderComponentPositionalParameters = processRenderComponentPositional(gameObject->getMainXMLNode());

		//Get PhysicsComponentCharacter
		tGameObjectTripolloParameters.tPhysicsComponentCharacterParameters =  processPhysicsComponentCharacter(gameObject->XMLNodeCustomProperties);
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectTripollo(tGameObjectTripolloParameters);
}

void LevelLoader::processGameObjectTerrain(XMLGameObject* gameObject)
{
	std::string meshfile;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		if(gameObject->XMLNodeDreams)
		{
			meshfile = getPropertyString(gameObject->XMLNodeDreams, "meshfile");			
		}
		else if(gameObject->XMLNodeNightmares)
		{
			meshfile = getPropertyString(gameObject->XMLNodeNightmares, "meshfile");
		}

		std::string complexConvex="CONVEX_"+meshfile.substr(0,meshfile.size()-5)+".nxs";
		std::string complexTriangle="TRIANGLE_"+meshfile.substr(0,meshfile.size()-5)+".nxs";

		if(Ogre::ResourceGroupManager::getSingleton().resourceExists(DEFAULT_OGRE_RESOURCE_MANAGER_GROUP,complexConvex))
		{

			OUAN::TGameObjectTerrainConvexParameters  tGameObjectTerrainConvexParameters;
			try
			{
				//Get names
				tGameObjectTerrainConvexParameters.dreamsName = gameObject->dreamsName;
				tGameObjectTerrainConvexParameters.nightmaresName = gameObject->nightmaresName;
				tGameObjectTerrainConvexParameters.name = gameObject->name;

				//Get PhysicsComponentComplexConvex
				tGameObjectTerrainConvexParameters.tPhysicsComponentComplexConvexParameters = processPhysicsComponentComplexConvex(gameObject->XMLNodeCustomProperties,
					complexConvex);

				tGameObjectTerrainConvexParameters.logicComponentParameters= processLogicComponent(gameObject->XMLNodeDreams,
					gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

				tGameObjectTerrainConvexParameters.tRenderComponentPositionalParameters= processRenderComponentPositional(gameObject->getMainXMLNode());
				
				if(tGameObjectTerrainConvexParameters.logicComponentParameters.existsInDreams)
				{
					tGameObjectTerrainConvexParameters.tRenderComponentEntityDreamsParameters=processRenderComponentEntity(gameObject->XMLNodeDreams);
				}
				
				if(tGameObjectTerrainConvexParameters.logicComponentParameters.existsInNightmares)
				{
					tGameObjectTerrainConvexParameters.tRenderComponentEntityNightmaresParameters=processRenderComponentEntity(gameObject->XMLNodeNightmares);
				}
			}
			catch( std::string error )
			{
				throw error;
				return;
			}

			//Create GameObject
			mGameWorldManager->createGameObjectTerrainConvex(tGameObjectTerrainConvexParameters);

			Ogre::LogManager::getSingleton().logMessage("[LevelLoader] "+gameObject->name+" uses .nxs complex physics file "+complexConvex);

		}
		else if(Ogre::ResourceGroupManager::getSingleton().resourceExists(DEFAULT_OGRE_RESOURCE_MANAGER_GROUP,complexTriangle))
		{
			OUAN::TGameObjectTerrainTriangleParameters  tGameObjectTerrainTriangleParameters;
			try
			{
				//Check parsing errors
				if(!gameObject->XMLNodeDreams) throw DREAMS_NODE_NOT_FOUND;
				if(!gameObject->XMLNodeNightmares) throw NIGHTMARES_NODE_NOT_FOUND;

				//Get names
				tGameObjectTerrainTriangleParameters.dreamsName = gameObject->dreamsName;
				tGameObjectTerrainTriangleParameters.nightmaresName = gameObject->nightmaresName;
				tGameObjectTerrainTriangleParameters.name = gameObject->name;

				//Get PhysicsComponentComplexTriangle
				tGameObjectTerrainTriangleParameters.tPhysicsComponentComplexTriangleParameters = processPhysicsComponentComplexTriangle(gameObject->XMLNodeCustomProperties,
					complexTriangle);

				tGameObjectTerrainTriangleParameters.logicComponentParameters= processLogicComponent(gameObject->XMLNodeDreams,
					gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

				tGameObjectTerrainTriangleParameters.tRenderComponentPositionalParameters= processRenderComponentPositional(gameObject->getMainXMLNode());
				
				tGameObjectTerrainTriangleParameters.tRenderComponentEntityDreamsParameters=processRenderComponentEntity(gameObject->XMLNodeDreams);

				tGameObjectTerrainTriangleParameters.tRenderComponentEntityNightmaresParameters=processRenderComponentEntity(gameObject->XMLNodeNightmares);
			}
			catch( std::string error )
			{
				throw error;
				return;
			}

			//Create GameObject
			mGameWorldManager->createGameObjectTerrainTriangle(tGameObjectTerrainTriangleParameters);

			Ogre::LogManager::getSingleton().logMessage("[LevelLoader] "+gameObject->name+" uses .nxs complex physics file "+complexTriangle);
		}
		else
		{
			processGameObjectProvisionalEntity(gameObject);
			throw "Error reading .nxs complex physics file for mesh "+meshfile;
		}
	}
	catch( std::string error )
	{
		throw error;
		return;
	}
}

void LevelLoader::processGameObjectCamera(XMLGameObject* gameObject)
{
	OUAN::TGameObjectCameraParameters  tGameObjectCameraParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectCameraParameters.name = gameObject->name;

		//Get RenderComponentCamera
		tGameObjectCameraParameters.tRenderComponentCameraParameters=processRenderComponentCamera(gameObject->getMainXMLNode());
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectCamera(tGameObjectCameraParameters);
}

void LevelLoader::processGameObjectLight(XMLGameObject* gameObject)
{
	OUAN::TGameObjectLightParameters  tGameObjectLightParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectLightParameters.dreamsName = gameObject->dreamsName;
		tGameObjectLightParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectLightParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectLightParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentLight
		tGameObjectLightParameters.tRenderComponentLightParameters=processRenderComponentLight(gameObject->getMainXMLNode());

		//Get RenderComponentPositional
		tGameObjectLightParameters.tRenderComponentPositionalParameters=processRenderComponentPositionalNoScale(gameObject->getMainXMLNode());
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectLight(tGameObjectLightParameters);
}

void LevelLoader::processGameObjectParticleSystem(XMLGameObject* gameObject)
{
	OUAN::TGameObjectParticleSystemParameters  tGameObjectParticleSystemParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectParticleSystemParameters.dreamsName = gameObject->dreamsName;
		tGameObjectParticleSystemParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectParticleSystemParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectParticleSystemParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentParticleSystem
		tGameObjectParticleSystemParameters.tRenderComponentParticleSystemParameters=processRenderComponentParticleSystem(gameObject->getMainXMLNode());

		//Get RenderComponentPositional
		tGameObjectParticleSystemParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->getMainXMLNode());
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectParticleSystem(tGameObjectParticleSystemParameters);
}
void LevelLoader::processGameObjectBillboardSet(XMLGameObject* gameObject)
{
	OUAN::TGameObjectBillboardSetParameters  tGameObjectBillboardSetParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectBillboardSetParameters.dreamsName = gameObject->dreamsName;
		tGameObjectBillboardSetParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectBillboardSetParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectBillboardSetParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentBillboardSet
		tGameObjectBillboardSetParameters.tRenderComponentBillboardSetParameters=processRenderComponentBillboardSet(gameObject->getMainXMLNode());

		//Get RenderComponentPositional
		tGameObjectBillboardSetParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->getMainXMLNode());
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectBillboardSet(tGameObjectBillboardSetParameters);
}

void LevelLoader::processGameObjectEye(XMLGameObject* gameObject)
{
	OUAN::TGameObjectEyeParameters tGameObjectEyeParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;
		if(gameObject->XMLNodeDreams) throw DREAMS_SHOULD_NOT_EXIST;
		if(!gameObject->XMLNodeNightmares) throw NIGHTMARES_NODE_NOT_FOUND;

		//Get names
		tGameObjectEyeParameters.dreamsName = "None";
		tGameObjectEyeParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectEyeParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectEyeParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentEntity
		tGameObjectEyeParameters.tRenderComponentEntityNightmaresParameters=processRenderComponentEntity(gameObject->XMLNodeNightmares);

		//Get RenderComponentPositional
		tGameObjectEyeParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->XMLNodeNightmares);

		//Get PhysicsComponentCharacter
		tGameObjectEyeParameters.tPhysicsComponentCharacterParameters = processPhysicsComponentCharacter(gameObject->XMLNodeCustomProperties);
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectEye(tGameObjectEyeParameters);
}

void LevelLoader::processGameObjectItem1UP(XMLGameObject* gameObject)
{
	OUAN::TGameObjectItem1UPParameters tGameObjectItem1UPParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectItem1UPParameters.dreamsName = gameObject->dreamsName;
		tGameObjectItem1UPParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectItem1UPParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectItem1UPParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentEntity
		tGameObjectItem1UPParameters.tRenderComponentEntityParameters=processRenderComponentEntity(gameObject->getMainXMLNode());

		//Get RenderComponentPositional
		tGameObjectItem1UPParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->getMainXMLNode());

		//Get PhysicsComponentSimpleCapsule
		tGameObjectItem1UPParameters.tPhysicsComponentSimpleCapsuleParameters=processPhysicsComponentSimpleCapsule(gameObject->XMLNodeCustomProperties);

	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectItem1UP(tGameObjectItem1UPParameters);
}

void LevelLoader::processGameObjectItemMaxHP(XMLGameObject* gameObject)
{
	OUAN::TGameObjectItemMaxHPParameters tGameObjectItemMaxHPParameters;

	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectItemMaxHPParameters.dreamsName = gameObject->dreamsName;
		tGameObjectItemMaxHPParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectItemMaxHPParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectItemMaxHPParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentEntity
		tGameObjectItemMaxHPParameters.tRenderComponentEntityParameters=processRenderComponentEntity(gameObject->getMainXMLNode());

		//Get RenderComponentPositional
		tGameObjectItemMaxHPParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->getMainXMLNode());

		//Get PhysicsComponentSimpleCapsule
		tGameObjectItemMaxHPParameters.tPhysicsComponentSimpleCapsuleParameters=processPhysicsComponentSimpleCapsule(gameObject->XMLNodeCustomProperties);

	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectItemMaxHP(tGameObjectItemMaxHPParameters);
}

void LevelLoader::processGameObjectPortal(XMLGameObject* gameObject)
{
	OUAN::TGameObjectPortalParameters tGameObjectPortalParameters;
	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeDreams) throw DREAMS_NODE_NOT_FOUND;
		if(!gameObject->XMLNodeNightmares) throw NIGHTMARES_NODE_NOT_FOUND;
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectPortalParameters.dreamsName = gameObject->dreamsName;
		tGameObjectPortalParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectPortalParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectPortalParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentEntityDreams
		tGameObjectPortalParameters.tRenderComponentEntityDreamsParameters=processRenderComponentEntity(gameObject->XMLNodeDreams);

		//Get RenderComponentEntityNightmares
		tGameObjectPortalParameters.tRenderComponentEntityNightmaresParameters=processRenderComponentEntity(gameObject->XMLNodeNightmares);

		//Get RenderComponentPositional
		tGameObjectPortalParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->getMainXMLNode());
		
		//Get PhysicsComponentSimpleBox
		tGameObjectPortalParameters.tPhysicsComponentSimpleBoxParameters=processPhysicsComponentSimpleBox(gameObject->XMLNodeCustomProperties);
	}
	catch( std::string error )
	{
		throw error;
		return;
	}

	//Create GameObject
	mGameWorldManager->createGameObjectPortal(tGameObjectPortalParameters);
}

void LevelLoader::processGameObjectTriggerBox(XMLGameObject* gameObject)
{
	OUAN::TGameObjectTriggerBoxParameters tGameObjectTriggerBoxParameters;
	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectTriggerBoxParameters.dreamsName = gameObject->dreamsName;
		tGameObjectTriggerBoxParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectTriggerBoxParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectTriggerBoxParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentEntity
		tGameObjectTriggerBoxParameters.tRenderComponentEntityParameters=processRenderComponentEntity(gameObject->getMainXMLNode());

		//Get RenderComponentPositional
		tGameObjectTriggerBoxParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->getMainXMLNode());

		//Get PhysicsComponentVolumeBox
		tGameObjectTriggerBoxParameters.tPhysicsComponentVolumeBoxParameters=processPhysicsComponentVolumeBox(gameObject->XMLNodeCustomProperties,gameObject->getMainXMLNode());
	}
	catch( std::string error )
	{
		throw error;
		return;
	}
	//Create GameObject
	mGameWorldManager->createGameObjectTriggerBox(tGameObjectTriggerBoxParameters);
}

void LevelLoader::processGameObjectTriggerCapsule(XMLGameObject* gameObject)
{
	OUAN::TGameObjectTriggerCapsuleParameters tGameObjectTriggerCapsuleParameters;
	try
	{
		//Check parsing errors
		if(!gameObject->XMLNodeCustomProperties) throw CUSTOM_PROPERTIES_NODE_NOT_FOUND;

		//Get names
		tGameObjectTriggerCapsuleParameters.dreamsName = gameObject->dreamsName;
		tGameObjectTriggerCapsuleParameters.nightmaresName = gameObject->nightmaresName;
		tGameObjectTriggerCapsuleParameters.name = gameObject->name;

		//Get Logic component
		tGameObjectTriggerCapsuleParameters.logicComponentParameters=processLogicComponent(gameObject->XMLNodeDreams,
			gameObject->XMLNodeNightmares,gameObject->XMLNodeCustomProperties);

		//Get RenderComponentEntity
		tGameObjectTriggerCapsuleParameters.tRenderComponentEntityParameters=processRenderComponentEntity(gameObject->getMainXMLNode());

		//Get RenderComponentPositional
		tGameObjectTriggerCapsuleParameters.tRenderComponentPositionalParameters=processRenderComponentPositional(gameObject->getMainXMLNode());

		//Get PhysicsComponentVolumeCapsule
		tGameObjectTriggerCapsuleParameters.tPhysicsComponentVolumeCapsuleParameters=processPhysicsComponentVolumeCapsule(gameObject->XMLNodeCustomProperties);
	}
	catch( std::string error )
	{
		throw error;
		return;
	}
	//Create GameObject
	mGameWorldManager->createGameObjectTriggerCapsule(tGameObjectTriggerCapsuleParameters);
}

TRenderComponentViewportParameters LevelLoader::processRenderComponentViewport(TiXmlElement *XMLNode)
{
	OUAN::TRenderComponentViewportParameters tRenderComponentViewportParameters;

	//Get Viewport properties
	tRenderComponentViewportParameters.colour = getPropertyColourValue(XMLNode,"colour");
	tRenderComponentViewportParameters.compositorcount = getPropertyInt(XMLNode,"compositorcount");
	tRenderComponentViewportParameters.index = getPropertyInt(XMLNode,"index");
	tRenderComponentViewportParameters.overlays = getPropertyBool(XMLNode,"overlays");
	tRenderComponentViewportParameters.shadows = getPropertyBool(XMLNode,"shadows");
	tRenderComponentViewportParameters.skies = getPropertyBool(XMLNode,"skies");

	return tRenderComponentViewportParameters;
	
}

TRenderComponentSceneParameters LevelLoader::processRenderComponentScene(TiXmlElement *XMLOgitorNode,TiXmlElement *XMLCustomPropertiesNode)
{
	OUAN::TRenderComponentSceneParameters tRenderComponentSceneParameters;

	tRenderComponentSceneParameters.ambient=getPropertyColourValue(XMLOgitorNode,"ambient");

	//Process SkyBox
	tRenderComponentSceneParameters.tRenderComponentSkyBoxParameters=processRenderComponentSkyBox(XMLCustomPropertiesNode);

	//Process SkyDome
	tRenderComponentSceneParameters.tRenderComponentSkyDomeParameters=processRenderComponentSkyDome(XMLCustomPropertiesNode);

	//TODO: Process Fog
	//processFog(XMLNode);

	//TODO: Process Shadows
	//processShadows(XMLNode);

	return tRenderComponentSceneParameters;
}

TRenderComponentCameraParameters LevelLoader::processRenderComponentCamera(TiXmlElement *XMLNode)
{
	OUAN::TRenderComponentCameraParameters tRenderComponentCameraParameters;

	//Get Camera properties
	tRenderComponentCameraParameters.autotracktarget = getPropertyString(XMLNode,"autotracktarget");
	tRenderComponentCameraParameters.orientation = getPropertyQuaternion(XMLNode,"orientation");
	tRenderComponentCameraParameters.position = getPropertyVector3(XMLNode,"position");
	tRenderComponentCameraParameters.autoaspectratio = false;//getPropertyBool(XMLNode,"autoaspectratio");
	tRenderComponentCameraParameters.clipdistance = getPropertyVector2(XMLNode,"clipdistance");
	tRenderComponentCameraParameters.viewmode = getPropertyInt(XMLNode,"viewmode");

	//set FOVy
	//In Ogitor default value is 1, which in Ogitor is 55 degree. FOV has to be in (0,180)
	Real FOVy = getPropertyReal(XMLNode,"fov");
	FOVy=FOVy*55.0f;
	if(FOVy>180.0) FOVy=179.99;
	else if(FOVy<=0) FOVy=0.01;
	tRenderComponentCameraParameters.FOVy=FOVy;

		//PolygonModeConversion
	int polygonmode = getPropertyInt(XMLNode,"polygonmode");
	switch(polygonmode)
		{
			case OGITOR_PM_SOLID:
				tRenderComponentCameraParameters.polygonmode=Ogre::PM_SOLID;
				break;
			case OGITOR_PM_POINTS:
				tRenderComponentCameraParameters.polygonmode=Ogre::PM_POINTS;
				break;
			case OGITOR_PM_WIREFRAME:
				tRenderComponentCameraParameters.polygonmode=Ogre::PM_WIREFRAME;
				break;
			default:
				Ogre::LogManager::getSingleton().logMessage("Camera has unrecognised PolygonMode!");
				break;
		}

	return tRenderComponentCameraParameters;
}
TRenderComponentCameraParameters LevelLoader::processRenderComponentCameraViewport(TiXmlElement *XMLNode)
{

	OUAN::TRenderComponentCameraParameters tRenderComponentCameraParameters;

	//Get Camera properties
	tRenderComponentCameraParameters.autotracktarget = "None";
	tRenderComponentCameraParameters.orientation = getPropertyQuaternion(XMLNode,"camera::orientation");
	tRenderComponentCameraParameters.position = getPropertyVector3(XMLNode,"camera::position");
	tRenderComponentCameraParameters.autoaspectratio = false;
	tRenderComponentCameraParameters.clipdistance = getPropertyVector2(XMLNode,"camera::clipdistance");
	tRenderComponentCameraParameters.viewmode = getPropertyInt(XMLNode,"camera::viewmode");

	//set FOVy
	//In Ogitor default value is 1, which in Ogitor is 55 degree. FOV has to be in (0,180)
	Real FOVy = getPropertyReal(XMLNode,"camera::fov");
	FOVy=FOVy*55.0f;
	if(FOVy>180.0) FOVy=179.99;
	else if(FOVy<=0) FOVy=0.01;
	tRenderComponentCameraParameters.FOVy=FOVy;

		//PolygonModeConversion
	int polygonmode = getPropertyInt(XMLNode,"camera::polymode");
	switch(polygonmode)
		{
			case OGITOR_PM_SOLID:
				tRenderComponentCameraParameters.polygonmode=Ogre::PM_SOLID;
				break;
			case OGITOR_PM_POINTS:
				tRenderComponentCameraParameters.polygonmode=Ogre::PM_POINTS;
				break;
			case OGITOR_PM_WIREFRAME:
				tRenderComponentCameraParameters.polygonmode=Ogre::PM_WIREFRAME;
				break;
			default:
				Ogre::LogManager::getSingleton().logMessage("Camera has unrecognised PolygonMode!");
				break;
		}

	return tRenderComponentCameraParameters;
}
TRenderComponentPositionalParameters LevelLoader::processRenderComponentPositional(TiXmlElement *XMLNode)
{
	OUAN::TRenderComponentPositionalParameters tRenderComponentPositionalParameters;

	//Get parent SceneNode name
	tRenderComponentPositionalParameters.parentSceneNodeName = "SceneManager";

	//Get SceneNode parameters
	tRenderComponentPositionalParameters.position = getPropertyVector3(XMLNode,"position");
	tRenderComponentPositionalParameters.orientation = getPropertyQuaternion(XMLNode,"orientation");
	tRenderComponentPositionalParameters.scale = getPropertyVector3(XMLNode,"scale");
	tRenderComponentPositionalParameters.autotracktarget = getPropertyString(XMLNode,"autotracktarget");

	return tRenderComponentPositionalParameters;
}

TRenderComponentPositionalParameters LevelLoader::processRenderComponentPositionalNoScale(TiXmlElement *XMLNode)
{
	OUAN::TRenderComponentPositionalParameters tRenderComponentPositionalParameters;

	//Get parent SceneNode name
	tRenderComponentPositionalParameters.parentSceneNodeName = "SceneManager";//getAttrib(XMLNode, "parentnode");

	//Get SceneNode parameters
	tRenderComponentPositionalParameters.position = getPropertyVector3(XMLNode,"position");
	tRenderComponentPositionalParameters.orientation = getPropertyQuaternion(XMLNode,"orientation");
	tRenderComponentPositionalParameters.scale = Vector3(1,1,1);
	tRenderComponentPositionalParameters.autotracktarget = "None";

	return tRenderComponentPositionalParameters;
}

void LevelLoader::processRenderComponentSubEntities(std::vector<TRenderComponentSubEntityParameters> &tRenderComponentSubEntityParameters,TiXmlElement *XMLNode)
{
		int i;

		TRenderComponentSubEntityParameters currentRenderComponentSubEntityParameters;
 
		//process and load all Entity's SubEntites
		i=0;
		while(true)
		{
			//Process SubEntity
			currentRenderComponentSubEntityParameters.material=getPropertyString(XMLNode,"subentity"+StringConverter::toString(i)+"::material",false);
			currentRenderComponentSubEntityParameters.visible=getPropertyBool(XMLNode,"subentity"+StringConverter::toString(i)+"::visible",false);

			//there is no more sub entities
			if(currentRenderComponentSubEntityParameters.material.compare("")==0) break;

			//create SubEntity
			tRenderComponentSubEntityParameters.push_back(currentRenderComponentSubEntityParameters);

			i++;
		}
}

TRenderComponentEntityParameters LevelLoader::processRenderComponentEntity(TiXmlElement *XMLNode)
{

	TRenderComponentEntityParameters tRenderComponentEntityParameters;

	//Process entity properties
	tRenderComponentEntityParameters.meshfile = getPropertyString(XMLNode, "meshfile");
	tRenderComponentEntityParameters.castshadows = getPropertyBool(XMLNode, "castshadows");

	//process Entity's SubEntites
	processRenderComponentSubEntities(tRenderComponentEntityParameters.tRenderComponentSubEntityParameters,XMLNode);
	
	return tRenderComponentEntityParameters;
}

TRenderComponentLightParameters LevelLoader::processRenderComponentLight(TiXmlElement *XMLNode)
{

	OUAN::TRenderComponentLightParameters tRenderComponentLightParameters;

	//Get Light properties
	
	tRenderComponentLightParameters.diffuse = getPropertyColourValue(XMLNode,"diffuse");
	tRenderComponentLightParameters.specular = getPropertyColourValue(XMLNode,"specular");
	tRenderComponentLightParameters.direction = getPropertyVector3(XMLNode,"direction");
	tRenderComponentLightParameters.castshadows = getPropertyBool(XMLNode,"castshadows");
	tRenderComponentLightParameters.lightrange = getPropertyVector3(XMLNode,"lightrange");
	tRenderComponentLightParameters.attenuation = getPropertyVector4(XMLNode,"attenuation");
	tRenderComponentLightParameters.power = getPropertyReal(XMLNode,"power");

		//Lightype conversion
	int lighttype = getPropertyInt(XMLNode,"lighttype");
	switch(lighttype)
	{
			case OGITOR_LT_POINT:
				tRenderComponentLightParameters.lighttype=Ogre::Light::LT_POINT;
				break;
			case OGITOR_LT_DIRECTIONAL:
				tRenderComponentLightParameters.lighttype=Ogre::Light::LT_DIRECTIONAL;
				break;
			case OGITOR_LT_SPOTLIGHT:
				tRenderComponentLightParameters.lighttype=Ogre::Light::LT_SPOTLIGHT;
				break;
			default:
				Ogre::LogManager::getSingleton().logMessage("Light has unrecognised light type!");
				break;
	}

	return tRenderComponentLightParameters;

}

TRenderComponentParticleSystemParameters LevelLoader::processRenderComponentParticleSystem(TiXmlElement *XMLNode)
{
	OUAN::TRenderComponentParticleSystemParameters tRenderComponentParticleSystemParameters;

	//Process ParticleSystem properties
	tRenderComponentParticleSystemParameters.particle = getPropertyString(XMLNode, "particle");
	tRenderComponentParticleSystemParameters.castshadows = getPropertyBool(XMLNode, "castshadows");
	
	return tRenderComponentParticleSystemParameters;
}

TRenderComponentBillboardSetParameters LevelLoader::processRenderComponentBillboardSet(TiXmlElement *XMLNode)
{
	OUAN::TRenderComponentBillboardSetParameters tRenderComponentBillboardSetParameters;

	//Process BillboardSet properties
	tRenderComponentBillboardSetParameters.material = getPropertyString(XMLNode, "material");
	tRenderComponentBillboardSetParameters.defaultheight = getPropertyReal(XMLNode, "defaultheight");
	tRenderComponentBillboardSetParameters.defaultwidth = getPropertyReal(XMLNode, "defaultwidth");
	tRenderComponentBillboardSetParameters.pointrendering = getPropertyBool(XMLNode, "pointrendering");
	tRenderComponentBillboardSetParameters.renderdistance = getPropertyReal(XMLNode, "renderdistance");
	tRenderComponentBillboardSetParameters.sorting = getPropertyBool(XMLNode, "sorting");

		//BillboardType Conversion
	int billboardtype = getPropertyInt(XMLNode, "billboardtype");
	switch(billboardtype)
	{
		case OGITOR_BBT_ORIENTED_COMMON:
			tRenderComponentBillboardSetParameters.billboardtype=Ogre::BBT_ORIENTED_COMMON;
			break;
		case OGITOR_BBT_ORIENTED_SELF:
			tRenderComponentBillboardSetParameters.billboardtype=Ogre::BBT_ORIENTED_SELF;
			break;
		case OGITOR_BBT_PERPENDICULAR_COMMON:
			tRenderComponentBillboardSetParameters.billboardtype=Ogre::BBT_PERPENDICULAR_COMMON;
			break;
		case OGITOR_BBT_PERPENDICULAR_SELF:
			tRenderComponentBillboardSetParameters.billboardtype=Ogre::BBT_PERPENDICULAR_SELF;
			break;
		case OGITOR_BBT_POINT:
			tRenderComponentBillboardSetParameters.billboardtype=Ogre::BBT_POINT;
			break;
		default:
			Ogre::LogManager::getSingleton().logMessage("Billboard has unrecognised BillboardType!");
			break;
	}
		//BillboardOrigin Conversion
	int billboardorigin = getPropertyInt(XMLNode, "billboardorigin");
	switch(billboardorigin)
	{
		case OGITOR_BBO_BOTTOM_CENTER:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_BOTTOM_CENTER;
			break;
		case OGITOR_BBO_BOTTOM_LEFT:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_BOTTOM_LEFT;
			break;
		case OGITOR_BBO_BOTTOM_RIGHT:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_BOTTOM_RIGHT;
			break;
		case OGITOR_BBO_CENTER:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_CENTER;
			break;
		case OGITOR_BBO_CENTER_LEFT:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_CENTER_LEFT;
			break;
		case OGITOR_BBO_CENTER_RIGHT:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_CENTER_RIGHT;
			break;
		case OGITOR_BBO_TOP_CENTER:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_TOP_CENTER;
			break;
		case OGITOR_BBO_TOP_LEFT:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_TOP_LEFT;
			break;
		case OGITOR_BBO_TOP_RIGHT:
			tRenderComponentBillboardSetParameters.billboardorigin=Ogre::BBO_TOP_RIGHT;
			break;
		default:
			Ogre::LogManager::getSingleton().logMessage("Billboard has unrecognised BillboardOrigin!");
			break;
	}

	//Billboard Rotation Conversion
	int billboardrotation = getPropertyInt(XMLNode, "billboardrotation");
	switch(billboardrotation)
	{
		case OGITOR_BBR_TEXCOORD:
			tRenderComponentBillboardSetParameters.billboardrotation=Ogre::BBR_TEXCOORD;
			break;
		case OGITOR_BBR_VERTEX:
			tRenderComponentBillboardSetParameters.billboardrotation=Ogre::BBR_VERTEX;
			break;
		default:
			Ogre::LogManager::getSingleton().logMessage("Billboard has unrecognised BillboardRotationType!");
			break;
	}

	//process BillboardSet's Billboards
	processRenderComponentBillboards(tRenderComponentBillboardSetParameters.tRenderComponentBillboardParameters,XMLNode);

	return tRenderComponentBillboardSetParameters;
}

void LevelLoader::processRenderComponentBillboards(std::vector<TRenderComponentBillboardParameters>  &tRenderComponentBillboardParameters ,TiXmlElement *XMLNode)
{
	int i;
	OUAN::TRenderComponentBillboardParameters currentTRenderComponentBillboardParameters;

	//get the number of BillboardSet's Billboards
	int billboardcount=getPropertyInt(XMLNode,"billboardcount");

	//process and load all BillboardSet's Billboards
	for(i=0;i<billboardcount;i++)
	{
		//Process Billboards
		currentTRenderComponentBillboardParameters.colour=getPropertyColourValue(XMLNode,"billboard"+StringConverter::toString(i)+"::colour");
		currentTRenderComponentBillboardParameters.dimensions=getPropertyVector2(XMLNode,"billboard"+StringConverter::toString(i)+"::dimensions");
		currentTRenderComponentBillboardParameters.position=getPropertyVector3(XMLNode,"billboard"+StringConverter::toString(i)+"::position");
		currentTRenderComponentBillboardParameters.rotation=getPropertyReal(XMLNode,"billboard"+StringConverter::toString(i)+"::rotation");
		currentTRenderComponentBillboardParameters.texcoordindex=getPropertyInt(XMLNode,"billboard"+StringConverter::toString(i)+"::texcoordindex");
		currentTRenderComponentBillboardParameters.texrect=getPropertyVector4(XMLNode,"billboard"+StringConverter::toString(i)+"::texrect");

		tRenderComponentBillboardParameters.push_back(currentTRenderComponentBillboardParameters);
	}
}

TRenderComponentSkyBoxParameters LevelLoader::processRenderComponentSkyBox(TiXmlElement *XMLCustomPropertiesNode)
{
	TRenderComponentSkyBoxParameters TRenderComponentSkyBoxParameters;
	// Process SkyBox properties
	TRenderComponentSkyBoxParameters.active = getPropertyBool(XMLCustomPropertiesNode, "RenderComponentSkyBox::active");
	TRenderComponentSkyBoxParameters.distance = getPropertyReal(XMLCustomPropertiesNode, "RenderComponentSkyBox::distance");
	// Process SkyBox materials
	TRenderComponentSkyBoxParameters.materialDreams = getPropertyString(XMLCustomPropertiesNode, "RenderComponentSkyBox::materialDreams");
	TRenderComponentSkyBoxParameters.materialNightmares = getPropertyString(XMLCustomPropertiesNode, "RenderComponentSkyBox::materialNightmares");


	return TRenderComponentSkyBoxParameters;
}

TRenderComponentSkyDomeParameters LevelLoader::processRenderComponentSkyDome(TiXmlElement *XMLCustomPropertiesNode)
{
	TRenderComponentSkyDomeParameters TRenderComponentSkyDomeParameters;
	// Process SkyDome properties
	TRenderComponentSkyDomeParameters.active = getPropertyBool(XMLCustomPropertiesNode, "RenderComponentSkyDome::active");
	TRenderComponentSkyDomeParameters.curvature = getPropertyReal(XMLCustomPropertiesNode, "RenderComponentSkyDome::curvature");
	TRenderComponentSkyDomeParameters.tiling = getPropertyReal(XMLCustomPropertiesNode, "RenderComponentSkyDome::tiling");
	TRenderComponentSkyDomeParameters.distance = getPropertyReal(XMLCustomPropertiesNode, "RenderComponentSkyDome::distance");
	// Process SkyDome materials
	TRenderComponentSkyDomeParameters.materialDreams = getPropertyString(XMLCustomPropertiesNode, "RenderComponentSkyDome::materialDreams");
	TRenderComponentSkyDomeParameters.materialNightmares = getPropertyString(XMLCustomPropertiesNode, "RenderComponentSkyDome::materialNightmares");

	return TRenderComponentSkyDomeParameters;
}

TPhysicsComponentCharacterParameters LevelLoader::processPhysicsComponentCharacter(TiXmlElement *XMLNode,std::string suffix)
{
	TPhysicsComponentCharacterParameters tPhysicsComponentCharacterParameters;

	//Get Component properties
	tPhysicsComponentCharacterParameters.mass= getPropertyReal(XMLNode, "PhysicsComponentCharacter"+suffix+"::mass");
	tPhysicsComponentCharacterParameters.radius= getPropertyReal(XMLNode, "PhysicsComponentCharacter"+suffix+"::radius");
	tPhysicsComponentCharacterParameters.height= getPropertyReal(XMLNode, "PhysicsComponentCharacter"+suffix+"::height");

	return tPhysicsComponentCharacterParameters;

}

TPhysicsComponentComplexConvexParameters LevelLoader::processPhysicsComponentComplexConvex(TiXmlElement *XMLNode,std::string nxsFile,std::string suffix)
{
	TPhysicsComponentComplexConvexParameters tPhysicsComponentComplexConvexParameters;
	
	//Get Component properties
	tPhysicsComponentComplexConvexParameters.mass= getPropertyReal(XMLNode, "PhysicsComponentComplex"+suffix+"::mass");

	//Set nxs file
	tPhysicsComponentComplexConvexParameters.nxsFile="nxs:"+nxsFile;

	return tPhysicsComponentComplexConvexParameters;
}

TPhysicsComponentComplexTriangleParameters LevelLoader::processPhysicsComponentComplexTriangle(TiXmlElement *XMLNode,std::string nxsFile,std::string suffix)
{
	TPhysicsComponentComplexTriangleParameters tPhysicsComponentComplexTriangleParameters;
	
	//Get Component properties
	tPhysicsComponentComplexTriangleParameters.mass= getPropertyReal(XMLNode, "PhysicsComponentComplex"+suffix+"::mass");

	//Set nxs file
	tPhysicsComponentComplexTriangleParameters.nxsFile="nxs:"+nxsFile;

	return tPhysicsComponentComplexTriangleParameters;
}

TPhysicsComponentSimpleBoxParameters LevelLoader::processPhysicsComponentSimpleBox(TiXmlElement *XMLNode,std::string suffix)
{
	TPhysicsComponentSimpleBoxParameters tPhysicsComponentSimpleBoxParameters;

	//Get Component properties
	tPhysicsComponentSimpleBoxParameters.mass=getPropertyReal(XMLNode, "PhysicsComponentSimpleBox"+suffix+"::mass");
	Vector3 length=getPropertyVector3(XMLNode, "PhysicsComponentSimpleBox"+suffix+"::length");
	tPhysicsComponentSimpleBoxParameters.lengthX=length.x;
	tPhysicsComponentSimpleBoxParameters.lengthY=length.y;
	tPhysicsComponentSimpleBoxParameters.lengthZ=length.z;

	return tPhysicsComponentSimpleBoxParameters;

}

TPhysicsComponentSimpleCapsuleParameters LevelLoader::processPhysicsComponentSimpleCapsule(TiXmlElement *XMLNode,std::string suffix)
{
	TPhysicsComponentSimpleCapsuleParameters tPhysicsComponentSimpleCapsuleParameters;

	//Get Component properties
	tPhysicsComponentSimpleCapsuleParameters.mass= getPropertyReal(XMLNode, "PhysicsComponentSimpleCapsule"+suffix+"::mass");
	tPhysicsComponentSimpleCapsuleParameters.radius= getPropertyReal(XMLNode, "PhysicsComponentSimpleCapsule"+suffix+"::radius");
	tPhysicsComponentSimpleCapsuleParameters.height= getPropertyReal(XMLNode, "PhysicsComponentSimpleCapsule"+suffix+"::height");

	return tPhysicsComponentSimpleCapsuleParameters;
}

TPhysicsComponentVolumeBoxParameters LevelLoader::processPhysicsComponentVolumeBox(TiXmlElement *XMLCustomPropertiesNode,TiXmlElement *XMLRenderInfoNode,std::string suffix)
{
	TPhysicsComponentVolumeBoxParameters tPhysicsComponentVolumeBoxParameters;

	//Get Component properties
	tPhysicsComponentVolumeBoxParameters.mass=getPropertyReal(XMLCustomPropertiesNode, "PhysicsComponentVolumeBox"+suffix+"::mass");
	Vector3 length=getPropertyVector3(XMLRenderInfoNode, "scale");
	tPhysicsComponentVolumeBoxParameters.lengthX=length.x;
	tPhysicsComponentVolumeBoxParameters.lengthY=length.y;
	tPhysicsComponentVolumeBoxParameters.lengthZ=length.z;

	return tPhysicsComponentVolumeBoxParameters;
}

TPhysicsComponentVolumeCapsuleParameters LevelLoader::processPhysicsComponentVolumeCapsule(TiXmlElement *XMLNode,std::string suffix)
{
	TPhysicsComponentVolumeCapsuleParameters tPhysicsComponentVolumeCapsuleParameters;

	//Get Component properties
	tPhysicsComponentVolumeCapsuleParameters.mass= getPropertyReal(XMLNode, "PhysicsComponentVolumeCapsule"+suffix+"::mass");
	tPhysicsComponentVolumeCapsuleParameters.radius= getPropertyReal(XMLNode, "PhysicsComponentVolumeCapsule"+suffix+"::radius");
	tPhysicsComponentVolumeCapsuleParameters.height= getPropertyReal(XMLNode, "PhysicsComponentVolumeCapsule"+suffix+"::height");

	return tPhysicsComponentVolumeCapsuleParameters;
}
TLogicComponentParameters LevelLoader::processLogicComponent(TiXmlElement *XMLNodeDreams,
												TiXmlElement *XMLNodeNightmares, TiXmlElement* XMLNodeCustomProperties)
{
	TLogicComponentParameters logicComponentParameters;
	//Object exists both in dreams and nightmares
	if(XMLNodeDreams && XMLNodeNightmares)
	{
		logicComponentParameters.existsInDreams=true;
		logicComponentParameters.existsInNightmares=true;
	}
	//Object exists only in dreams
	else if(XMLNodeDreams && !XMLNodeNightmares)
	{
		logicComponentParameters.existsInDreams=true;
		logicComponentParameters.existsInNightmares=false;
	}
	//Object exists only in nightmares
	else if(!XMLNodeDreams && XMLNodeNightmares)
	{
		logicComponentParameters.existsInDreams=false;
		logicComponentParameters.existsInNightmares=true;
	}
	if (XMLNodeCustomProperties)
	{
		try{
			logicComponentParameters.scriptFilename=getPropertyString(XMLNodeCustomProperties,
				"LogicComponent::scriptFilename");
		}
		catch(std::string error)
		{
			logicComponentParameters.scriptFilename="";
		}
		try{
			logicComponentParameters.defaultState=getPropertyInt(XMLNodeCustomProperties,
				"LogicComponent::defaultState");
		}
		catch (std::string error)
		{
			logicComponentParameters.defaultState=0;
		}
		try{
			logicComponentParameters.healthPoints=getPropertyInt(XMLNodeCustomProperties,
				"LogicComponent::healthPoints");
		}
		catch (std::string error)
		{
			logicComponentParameters.healthPoints=-1;
		}
		try{
			logicComponentParameters.numLives=getPropertyInt(XMLNodeCustomProperties,
				"LogicComponent::numLives");
		}
		catch (std::string error)
		{
			logicComponentParameters.numLives=-1;	
		}
	}
	return logicComponentParameters;
}

String LevelLoader::getAttrib(TiXmlElement *XMLNode, const String &attrib, const String &defaultValue)
{
	if(XMLNode->Attribute(attrib.c_str()))
		return XMLNode->Attribute(attrib.c_str());
	else
		return defaultValue;
}

String LevelLoader::getPropertyString(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	TiXmlElement *pElement;
	TiXmlElement *XMLNodeCustomProperties;
	String propertyName;
	String result="";
	bool found=false;

	if(!XMLNode)
	{
		throw std::string("Missing XML Node!");
	}
	else
	{

		// Process all PROPERTY of the XML node
		pElement = XMLNode->FirstChildElement("PROPERTY");
		while(pElement)
		{
			propertyName = getAttrib(pElement, "id");

			//Ogre::LogManager::getSingleton().logMessage("[LevelLoader] parsing "+propertyName+" property!");

			if(propertyName.compare(attrib_name)==0)
			{
				// Get the attribute value
				result = getAttrib(pElement, "value");
				found=true;
			}
			pElement = pElement->NextSiblingElement("PROPERTY");
		}

		XMLNodeCustomProperties = XMLNode->FirstChildElement("CUSTOMPROPERTIES");

		// Process all CUSTOMPROPERTIES
		if(XMLNodeCustomProperties)
		{
			pElement = XMLNodeCustomProperties->FirstChildElement("PROPERTY");
			while(pElement)
			{
				propertyName = getAttrib(pElement, "id");

				//Ogre::LogManager::getSingleton().logMessage("[LevelLoader] parsing "+propertyName+" property!");

				if(propertyName.compare(attrib_name)==0)
				{
					// Get the attribute value
					result = getAttrib(pElement, "value");
					found=true;
				}
				pElement = pElement->NextSiblingElement("PROPERTY");
			}
		}

		if(!found)
		{
			if(logErrors)
			{
				throw std::string("Error parsing "+attrib_name+" attribute!");
			}
		}
	}
	return result;
}

Vector2 LevelLoader::getPropertyVector2(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	return StringConverter::parseVector2(getPropertyString(XMLNode,attrib_name,logErrors));
}

Vector3 LevelLoader::getPropertyVector3(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	return StringConverter::parseVector3(getPropertyString(XMLNode,attrib_name,logErrors));
}

Vector4 LevelLoader::getPropertyVector4(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	return StringConverter::parseVector4(getPropertyString(XMLNode,attrib_name,logErrors));
}

Quaternion LevelLoader::getPropertyQuaternion(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	return StringConverter::parseQuaternion(getPropertyString(XMLNode,attrib_name,logErrors));
}

ColourValue LevelLoader::getPropertyColourValue(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	return StringConverter::parseColourValue(getPropertyString(XMLNode,attrib_name,logErrors));
}

bool LevelLoader::getPropertyBool(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	 return StringConverter::parseBool(getPropertyString(XMLNode,attrib_name,logErrors));
}

int LevelLoader::getPropertyInt(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	 return StringConverter::parseInt(getPropertyString(XMLNode,attrib_name,logErrors));
}

Real LevelLoader::getPropertyReal(TiXmlElement *XMLNode, const String &attrib_name, bool logErrors)
{
	 return StringConverter::parseReal(getPropertyString(XMLNode,attrib_name,logErrors));
}