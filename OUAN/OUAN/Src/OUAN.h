#ifndef __OUAN__H__
#define __OUAN__H__

#include <map>
#include <string>
#include <iostream>
#include <fstream> 
#include <sys/stat.h>
#include <Ogre.h>
#include <ois/OIS.h>

#include "ConfigKeys.h"

#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>

#include "TinyXML/tinyxml.h"

#include "Logic/LogicDefs.h"

#define SAFEDELETE(ptr) if(ptr){ delete ptr;ptr=NULL;}
#define _CRT_SECURE_NO_WARNINGS

namespace OUAN
{
	//-------------------------------------
	//	Basic Types
	//-------------------------------------
		typedef Ogre::String String;
		typedef Ogre::StringConverter StringConverter;
		typedef Ogre::Vector2 Vector2;
		typedef Ogre::Vector3 Vector3;
		typedef Ogre::Vector4 Vector4;
		typedef Ogre::Quaternion Quaternion;
		typedef Ogre::Real Real;
		typedef Ogre::ColourValue ColourValue;


	//-------------------------------------
	//	Input module-related constants, type definitions and forwarded declarations
	//-------------------------------------

		class FullInputManager;
		typedef boost::shared_ptr<FullInputManager> FullInputManagerPtr;

	//-------------------------------------
	//	Core module-related constants, type definitions and forwarded declarations
	//-------------------------------------
		const int AI_FPS=60;
		const int SKIP_TICKS=1000000/AI_FPS;
		const int MAX_FRAMESKIP=5;
		const int DEFAULT_KEY_BUFFER=500000;

		const int DEBUGMODE_NONE=0;
		const int DEBUGMODE_PERFORMANCE=1;
		const int DEBUGMODE_PHYSICS=2;
		const int DEBUGMODE_WIREFRAME=3;
		const int DEBUGMODE_POINTS=4;
		const int DEBUGMODE_VOLUMES=5;

		const std::string DEFAULT_WIN_NAME="OUAN";

		//forward declarations
		class GameStateManager;
		class GameState;
		class GameRunningState;
		class MainMenuState;
		class Application;

		typedef boost::shared_ptr<GameStateManager> GameStateManagerPtr;
		typedef boost::shared_ptr<GameState> GameStatePtr;
		typedef boost::shared_ptr<Application> ApplicationPtr;

	//---------------
	// Game module-related constants, type definitions and forwarded declarations
	//---------------

		class GameWorldManager;
		class Component;

		const int DREAMS=0;
		const int NIGHTMARES=1;
		const int BOTH_WORLDS=2;

		const std::string LEVEL_NONE="None";
		const std::string LEVEL_TEST="TestLevel";
		const std::string LEVEL_SANDBOX="Sandbox";
		const std::string LEVEL_CLOCK="Clock";
		const std::string LEVEL_1="Level1";
		const std::string LEVEL_2="Level2";

		//Render Components
		class RenderComponent;
		class TRenderComponentParameters;
		typedef boost::shared_ptr<RenderComponent> RenderComponentPtr;

		class RenderComponentBillboardSet;
		class TRenderComponentBillboardSetParameters;
		class TRenderComponentBillboardParameters;
		typedef boost::shared_ptr<RenderComponentBillboardSet> RenderComponentBillboardSetPtr;

		class RenderComponentCamera;
		class TRenderComponentCameraParameters;
		typedef boost::shared_ptr<RenderComponentCamera> RenderComponentCameraPtr;

		class RenderComponentEntity;
		class TRenderComponentEntityParameters;
		class TRenderComponentSubEntityParameters;
		class TRenderComponentEntityAnimParams;
		typedef boost::shared_ptr<RenderComponentEntity> RenderComponentEntityPtr;

		class RenderComponentLight;
		class TRenderComponentLightParameters;
		typedef boost::shared_ptr<RenderComponentLight> RenderComponentLightPtr;

		class RenderComponentViewport;
		class TRenderComponentViewportParameters;
		typedef boost::shared_ptr<RenderComponentViewport> RenderComponentViewportPtr;

		class RenderComponentParticleSystem;
		class TRenderComponentParticleSystemParameters;
		typedef boost::shared_ptr<RenderComponentParticleSystem> RenderComponentParticleSystemPtr;

		class RenderComponentQuadHalo;
		class TRenderComponentQuadHaloParameters;
		typedef boost::shared_ptr<RenderComponentQuadHalo> RenderComponentQuadHaloPtr;

		class RenderComponentScene;
		class TRenderComponentSkyDomeParameters;
		class TRenderComponentSkyBoxParameters;
		class TRenderComponentSceneParameters;
		typedef boost::shared_ptr<RenderComponentScene> RenderComponentScenePtr;

		class RenderComponentPositional;
		class TRenderComponentPositionalParameters;
		typedef boost::shared_ptr<RenderComponentPositional> RenderComponentPositionalPtr;

		class RenderComponentInitial;
		typedef boost::shared_ptr<RenderComponentInitial> RenderComponentInitialPtr;

		class RenderComponentViewport;
		class TRenderComponentViewportParameters;
		typedef boost::shared_ptr<RenderComponentViewport> RenderComponentViewportPtr;

		class RenderComponentDecal;
		class TRenderComponentDecalParameters;
		typedef boost::shared_ptr<RenderComponentDecal> RenderComponentDecalPtr;

		const std::string COMPONENT_TYPE_RENDER="RenderComponent";
		const std::string COMPONENT_TYPE_RENDER_BILLBOARD_SET="RenderComponentBillboardSet";
		const std::string COMPONENT_TYPE_RENDER_CAMERA="RenderComponentCamera";
		const std::string COMPONENT_TYPE_RENDER_ENTITY="RenderComponentEntity";
		const std::string COMPONENT_TYPE_RENDER_INITIAL="RenderComponentInitial";
		const std::string COMPONENT_TYPE_RENDER_LIGHT="RenderComponentLight";
		const std::string COMPONENT_TYPE_RENDER_PARTICLE_SYSTEM="RenderComponentParticleSystem";
		const std::string COMPONENT_TYPE_RENDER_POSITIONAL="RenderComponentPositional";
		const std::string COMPONENT_TYPE_RENDER_QUAD_HALO="RenderComponentQuadHalo";
		const std::string COMPONENT_TYPE_RENDER_SCENE="RenderComponentScene";
		const std::string COMPONENT_TYPE_RENDER_VIEWPORT="RenderComponentViewport";
		const std::string COMPONENT_TYPE_RENDER_DECAL="RenderComponentDecal";

		const std::string LIGHTMAP_PREFIX="lightmap#";

		//RayCasting
		class RayCasting;
		typedef boost::shared_ptr<RayCasting> RayCastingPtr;

		enum QueryFlags
		{
			QUERYFLAGS_NONE = 0,
			QUERYFLAGS_STATIC = 1<<0,
			QUERYFLAGS_DYNAMIC = 1<<1,
			QUERYFLAGS_CAMERA_COLLISION_MOVE_TO_TARGET = 1<<2,
			QUERYFLAGS_CAMERA_COLLISION_ROTX_POSITIVE = 1<<3,
			QUERYFLAGS_CAMERA_COLLISION_ROTX_NEGATIVE = 1<<4,
			QUERYFLAGS_CAMERA_COLLISION_TRANSLUCID = 1<<5,
			QUERYFLAGS_FLASHLIGHT_LIGHT = 1<<6,
			QUERYFLAGS_CAMERA_COLLISION=QUERYFLAGS_CAMERA_COLLISION_MOVE_TO_TARGET | QUERYFLAGS_CAMERA_COLLISION_ROTX_POSITIVE | QUERYFLAGS_CAMERA_COLLISION_ROTX_NEGATIVE | QUERYFLAGS_CAMERA_COLLISION_TRANSLUCID
			//QUERYFLAGS... = 1<<3
		};


		//Physics components
		class PhysicsComponent;
		class TPhysicsComponentParameters;
		typedef boost::shared_ptr<PhysicsComponent> PhysicsComponentPtr;

		class PhysicsComponentCharacter;
		class TPhysicsComponentCharacterParameters;
		typedef boost::shared_ptr<PhysicsComponentCharacter> PhysicsComponentCharacterPtr;

		class PhysicsComponentComplex;
		class TPhysicsComponentComplexParameters;
		typedef boost::shared_ptr<PhysicsComponentComplex> PhysicsComponentComplexPtr;

		class PhysicsComponentComplexConvex;
		class TPhysicsComponentComplexConvexParameters;
		typedef boost::shared_ptr<PhysicsComponentComplexConvex> PhysicsComponentComplexConvexPtr;

		class PhysicsComponentComplexTriangle;
		class TPhysicsComponentComplexTriangleParameters;
		typedef boost::shared_ptr<PhysicsComponentComplexTriangle> PhysicsComponentComplexTrianglePtr;

		class PhysicsComponentSimpleCapsule;
		class TPhysicsComponentSimpleCapsuleParameters;
		typedef boost::shared_ptr<PhysicsComponentSimpleCapsule> PhysicsComponentSimpleCapsulePtr;

		class PhysicsComponentSimpleBox;
		class TPhysicsComponentSimpleBoxParameters;
		typedef boost::shared_ptr<PhysicsComponentSimpleBox> PhysicsComponentSimpleBoxPtr;

		class PhysicsComponentVolumeCapsule;
		class TPhysicsComponentVolumeCapsuleParameters;
		typedef boost::shared_ptr<PhysicsComponentVolumeCapsule> PhysicsComponentVolumeCapsulePtr;

		class PhysicsComponentVolumeBox;
		class TPhysicsComponentVolumeBoxParameters;
		typedef boost::shared_ptr<PhysicsComponentVolumeBox> PhysicsComponentVolumeBoxPtr;

		class PhysicsComponentVolumeConvex;
		class TPhysicsComponentVolumeConvexParameters;
		typedef boost::shared_ptr<PhysicsComponentVolumeConvex> PhysicsComponentVolumeConvexPtr;

		const std::string COMPONENT_TYPE_PHYSICS="PhysicsComponent";
		const std::string COMPONENT_TYPE_PHYSICS_CHARACTER="PhysicsComponentCharacter";
		const std::string COMPONENT_TYPE_PHYSICS_COMPLEX="PhysicsComponentComplex";
		const std::string COMPONENT_TYPE_PHYSICS_COMPLEX_CONVEX="PhysicsComponentComplexConvex";
		const std::string COMPONENT_TYPE_PHYSICS_COMPLEX_TRIANGLE="PhysicsComponentComplexTriangle";
		const std::string COMPONENT_TYPE_PHYSICS_SIMPLE="PhysicsComponentSimple";
		const std::string COMPONENT_TYPE_PHYSICS_SIMPLE_BOX="PhysicsComponentSimpleBox";
		const std::string COMPONENT_TYPE_PHYSICS_SIMPLE_CAPSULE="PhysicsComponentSimpleCapsule";
		const std::string COMPONENT_TYPE_PHYSICS_VOLUME="PhysicsComponentVolume";
		const std::string COMPONENT_TYPE_PHYSICS_VOLUME_BOX="PhysicsComponentVolumeBox";
		const std::string COMPONENT_TYPE_PHYSICS_VOLUME_CAPSULE="PhysicsComponentVolumeCapsule";
		const std::string COMPONENT_TYPE_PHYSICS_VOLUME_CONVEX="PhysicsComponentVolumeConvex";

		//Logic Components
		class LogicComponentOny;
		class TLogicComponentOnyParameters;
		typedef boost::shared_ptr<LogicComponentOny> LogicComponentOnyPtr;

		class LogicComponentEnemy;
		class TLogicComponentEnemyParameters;
		typedef boost::shared_ptr<LogicComponentEnemy> LogicComponentEnemyPtr;

		class LogicComponentItem;
		class TLogicComponentItemParameters;
		typedef boost::shared_ptr<LogicComponentItem> LogicComponentItemPtr;

		class LogicComponentUsable;
		class TLogicComponentUsableParameters;
		typedef boost::shared_ptr<LogicComponentUsable> LogicComponentUsablePtr;

		class LogicComponentTrigger;
		class TLogicComponentTriggerParameters;
		typedef boost::shared_ptr<LogicComponentTrigger> LogicComponentTriggerPtr;

		class LogicComponent;
		class TLogicComponentParameters;
		typedef boost::shared_ptr<LogicComponent> LogicComponentPtr;

		class WeaponComponent;
		class TWeaponComponentParameters;
		typedef boost::shared_ptr<WeaponComponent> WeaponComponentPtr;

		const int DEFAULT_MAX_WEAPON_POWER=10;
		typedef enum
		{
			WEAPON_MODE_INVALID=-1,
			WEAPON_MODE_0,
			WEAPON_MODE_1,
			WEAPON_MODE_2,
			//...
			WEAPON_MODE_SPECIAL
		} TWeaponMode;

		class AttackComponent;
		class TAttackComponentParameters;
		typedef boost::shared_ptr<AttackComponent> AttackComponentPtr;

		const std::string COMPONENT_TYPE_LOGIC="LogicComponent";
		const std::string COMPONENT_TYPE_LOGIC_ONY="LogicComponentOny";
		const std::string COMPONENT_TYPE_LOGIC_ENEMY="LogicComponentEnemy";
		const std::string COMPONENT_TYPE_LOGIC_ITEM="LogicComponentItem";
		const std::string COMPONENT_TYPE_LOGIC_USABLE="LogicComponentUsable";
		const std::string COMPONENT_TYPE_LOGIC_TRIGGER="LogicComponentTrigger";
		const std::string COMPONENT_TYPE_WEAPON="WeaponComponent";
		const std::string COMPONENT_TYPE_ATTACK="AttackComponent";

		///////////////////////////////////////////////////////////////////

		//Game Object Specializations
		class GameObject;
		class TGameObjectParameters;
		typedef boost::shared_ptr<GameObject> GameObjectPtr;

		typedef std::map<std::string,GameObjectPtr> TGameObjectContainer;
		typedef TGameObjectContainer::iterator TGameObjectContainerIterator;

		typedef std::vector<GameObjectPtr> TGameObjectPositionalContainer;
		typedef std::vector<GameObjectPtr> TGameObjectMovableContainer;
		typedef std::vector<GameObjectPtr> TGameObjectNonMovableContainer;
		typedef std::vector<GameObjectPtr> TGameObjectMovableEntityContainer;
		typedef std::vector<GameObjectPtr> TGameObjectNonMovableEntityContainer;
		typedef std::vector<GameObjectPtr> TGameObjectEntityContainer;
		
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsCharacterContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsComplexContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsComplexConvexContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsComplexTriangleContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsSimpleContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsSimpleBoxContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsSimpleCapsuleContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsVolumeContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsVolumeBoxContainer;
		typedef std::vector<GameObjectPtr> TGameObjectPhysicsVolumeCapsuleContainer;

		typedef std::vector<GameObjectPtr> TGameObjectLogicContainer;
		typedef std::vector<GameObjectPtr> TGameObjectUsableContainer;

		////////////////////////////////////////////////////////////////////

		const int HIT_RECOVERY_TIME=500;
		const int MIN_RANDOM_MOVEMENT_DELAY=500;
		const int MAX_RANDOM_MOVEMENT_DELAY=800;

		class GameObjectScene;
		class TGameObjectSceneParameters;
		typedef boost::shared_ptr<GameObjectScene> GameObjectScenePtr;

		class GameObjectViewport;
		class TGameObjectViewportParameters;
		typedef boost::shared_ptr<GameObjectViewport> GameObjectViewportPtr;

		class GameObjectProvisionalEntity;
		class TGameObjectProvisionalEntityParameters;
		typedef boost::shared_ptr<GameObjectProvisionalEntity> GameObjectProvisionalEntityPtr;

		class GameObjectTerrainTriangle;
		class TGameObjectTerrainTriangleParameters;
		typedef boost::shared_ptr<GameObjectTerrainTriangle> GameObjectTerrainTrianglePtr;

		class GameObjectTerrainConvex;
		class TGameObjectTerrainConvexParameters;
		typedef boost::shared_ptr<GameObjectTerrainConvex> GameObjectTerrainConvexPtr;

		class GameObjectLight;
		class TGameObjectLightParameters;
		typedef boost::shared_ptr<GameObjectLight> GameObjectLightPtr;

		class GameObjectCamera;
		class TGameObjectCameraParameters;
		typedef boost::shared_ptr<GameObjectCamera> GameObjectCameraPtr;

		class GameObjectBillboardSet;
		class TGameObjectBillboardSetParameters;
		typedef boost::shared_ptr<GameObjectBillboardSet> GameObjectBillboardSetPtr;

		class GameObjectParticleSystem;
		class TGameObjectParticleSystemParameters;
		typedef boost::shared_ptr<GameObjectParticleSystem> GameObjectParticleSystemPtr;

		class GameObjectOny;
		class TGameObjectOnyParameters;
		typedef boost::shared_ptr<GameObjectOny> GameObjectOnyPtr;

		class GameObjectTripolloDreams;
		class TGameObjectTripolloDreamsParameters;
		typedef boost::shared_ptr<GameObjectTripolloDreams> GameObjectTripolloDreamsPtr;

		class GameObjectTripollito;
		class TGameObjectTripollitoParameters;
		typedef boost::shared_ptr<GameObjectTripollito> GameObjectTripollitoPtr;

		class GameObjectPortal;
		class TGameObjectPortalParameters;
		typedef boost::shared_ptr<GameObjectPortal> GameObjectPortalPtr;

		class GameObjectEye;
		class TGameObjectEyeParameters;
		typedef boost::shared_ptr<GameObjectEye> GameObjectEyePtr;

		class GameObjectItem1UP;
		class TGameObjectItem1UPParameters;
		typedef boost::shared_ptr<GameObjectItem1UP> GameObjectItem1UPPtr;

		class GameObjectItemMaxHP;
		class TGameObjectItemMaxHPParameters;
		typedef boost::shared_ptr<GameObjectItemMaxHP> GameObjectItemMaxHPPtr;

		class GameObjectTriggerBox;
		class TGameObjectTriggerBoxParameters;
		typedef boost::shared_ptr<GameObjectTriggerBox> GameObjectTriggerBoxPtr;

		class GameObjectTriggerCapsule;
		class TGameObjectTriggerCapsuleParameters;
		typedef boost::shared_ptr<GameObjectTriggerCapsule> GameObjectTriggerCapsulePtr;

		class GameObjectNightGoblin;
		class TGameObjectNightGoblinParameters;
		typedef boost::shared_ptr<GameObjectNightGoblin> GameObjectNightGoblinPtr;

		class GameObjectCryKing;
		class TGameObjectCryKingParameters;
		typedef boost::shared_ptr<GameObjectCryKing> GameObjectCryKingPtr;

		class GameObjectDragon;
		class TGameObjectDragonParameters;
		typedef boost::shared_ptr<GameObjectDragon> GameObjectDragonPtr;

		class GameObjectMagicClock;
		class TGameObjectMagicClockParameters;
		typedef boost::shared_ptr<GameObjectMagicClock> GameObjectMagicClockPtr;

		class GameObjectPillow;
		class TGameObjectPillowParameters;
		typedef boost::shared_ptr<GameObjectPillow> GameObjectPillowPtr;

		class GameObjectFlashLight;
		class TGameObjectFlashLightParameters;
		typedef boost::shared_ptr<GameObjectFlashLight> GameObjectFlashLightPtr;

		class GameObjectTentetieso;
		class TGameObjectTentetiesoParameters;
		typedef boost::shared_ptr<GameObjectTentetieso> GameObjectTentetiesoPtr;

		class GameObjectBee_Butterfly;
		class TGameObjectBee_ButterflyParameters;
		typedef boost::shared_ptr<GameObjectBee_Butterfly> GameObjectBee_ButterflyPtr;

		class GameObjectSnakeCreeper;
		class TGameObjectSnakeCreeperParameters;
		typedef boost::shared_ptr<GameObjectSnakeCreeper> GameObjectSnakeCreeperPtr;

		class GameObjectCarnivorousPlant;
		class TGameObjectCarnivorousPlantParameters;
		typedef boost::shared_ptr<GameObjectCarnivorousPlant> GameObjectCarnivorousPlantPtr;

		class GameObjectDiamondTree;
		class TGameObjectDiamondTreeParameters;
		typedef boost::shared_ptr<GameObjectDiamondTree> GameObjectDiamondTreePtr;

		class GameObjectStoryBook;
		class TGameObjectStoryBookParameters;
		typedef boost::shared_ptr<GameObjectStoryBook> GameObjectStoryBookPtr;

		class GameObjectHeart;
		class TGameObjectHeartParameters;
		typedef boost::shared_ptr<GameObjectHeart> GameObjectHeartPtr;

		class GameObjectDiamond;
		class TGameObjectDiamondParameters;
		typedef boost::shared_ptr<GameObjectDiamond> GameObjectDiamondPtr;

		class GameObjectScaredPlant;
		class TGameObjectScaredPlantParameters;
		typedef boost::shared_ptr<GameObjectScaredPlant> GameObjectScaredPlantPtr;

		class GameObjectTreeDreams;
		class TGameObjectTreeDreamsParameters;
		typedef boost::shared_ptr<GameObjectTreeDreams> GameObjectTreeDreamsPtr;

		class GameObjectTreeNightmares;
		class TGameObjectTreeNightmaresParameters;
		typedef boost::shared_ptr<GameObjectTreeNightmares> GameObjectTreeNightmaresPtr;

		class GameObjectClockPiece;
		class TGameObjectClockPieceParameters;
		typedef boost::shared_ptr<GameObjectClockPiece> GameObjectClockPiecePtr;

		class GameObjectScepter;
		class TGameObjectScepterParameters;
		typedef boost::shared_ptr<GameObjectScepter> GameObjectScepterPtr;

		class GameObjectDoor;
		class TGameObjectDoorParameters;
		typedef boost::shared_ptr<GameObjectDoor> GameObjectDoorPtr;

		class GameObjectPlataform;
		class TGameObjectPlataformParameters;
		typedef boost::shared_ptr<GameObjectPlataform> GameObjectPlataformPtr;

		class GameObjectWoodBox;
		class TGameObjectWoodBoxParameters;
		typedef boost::shared_ptr<GameObjectWoodBox> GameObjectWoodBoxPtr;

		class GameObjectBush;
		class TGameObjectBushParameters;
		typedef boost::shared_ptr<GameObjectBush> GameObjectBushPtr;

		class GameObjectCloud;
		class TGameObjectCloudParameters;
		typedef boost::shared_ptr<GameObjectCloud> GameObjectCloudPtr;

		class GameObjectTree;
		class TGameObjectTreeParameters;
		typedef boost::shared_ptr<GameObjectTree> GameObjectTreePtr;

		class GameObjectWater;
		class TGameObjectWaterParameters;
		typedef boost::shared_ptr<GameObjectWater> GameObjectWaterPtr;

		typedef std::vector<GameObjectScenePtr> TGameObjectSceneContainer;
		typedef std::vector<GameObjectViewportPtr> TGameObjectViewportContainer;
		typedef std::vector<GameObjectTerrainConvexPtr> TGameObjectTerrainConvexContainer;
		typedef std::vector<GameObjectTerrainTrianglePtr> TGameObjectTerrainTriangleContainer;
		typedef std::vector<GameObjectLightPtr> TGameObjectLightContainer;
		typedef std::vector<GameObjectCameraPtr> TGameObjectCameraContainer;
		typedef std::vector<GameObjectBillboardSetPtr> TGameObjectBillboardSetContainer;
		typedef std::vector<GameObjectParticleSystemPtr> TGameObjectParticleSystemContainer;
		typedef std::vector<GameObjectOnyPtr> TGameObjectOnyContainer;
		typedef std::vector<GameObjectTripolloDreamsPtr> TGameObjectTripolloDreamsContainer;
		typedef std::vector<GameObjectTripollitoPtr> TGameObjectTripollitoContainer;
		typedef std::vector<GameObjectPortalPtr> TGameObjectPortalContainer;
		typedef std::vector<GameObjectEyePtr> TGameObjectEyeContainer;
		typedef std::vector<GameObjectItem1UPPtr> TGameObjectItem1UPContainer;
		typedef std::vector<GameObjectItemMaxHPPtr> TGameObjectItemMaxHPContainer;
		typedef std::vector<GameObjectTriggerBoxPtr> TGameObjectTriggerBoxContainer;
		typedef std::vector<GameObjectTriggerCapsulePtr> TGameObjectTriggerCapsuleContainer;
		typedef std::vector<GameObjectNightGoblinPtr> TGameObjectNightGoblinContainer;
		typedef std::vector<GameObjectCryKingPtr> TGameObjectCryKingContainer;
		typedef std::vector<GameObjectDragonPtr> TGameObjectDragonContainer;
		typedef std::vector<GameObjectMagicClockPtr> TGameObjectMagicClockContainer;
		typedef std::vector<GameObjectPillowPtr> TGameObjectPillowContainer;
		typedef std::vector<GameObjectFlashLightPtr> TGameObjectFlashLightContainer;
		typedef std::vector<GameObjectTentetiesoPtr> TGameObjectTentetiesoContainer;
		typedef std::vector<GameObjectBee_ButterflyPtr> TGameObjectBee_ButterflyContainer;
		typedef std::vector<GameObjectDiamondTreePtr> TGameObjectDiamondTreeContainer;
		typedef std::vector<GameObjectStoryBookPtr> TGameObjectStoryBookContainer;
		typedef std::vector<GameObjectHeartPtr> TGameObjectHeartContainer;
		typedef std::vector<GameObjectDiamondPtr> TGameObjectDiamondContainer;
		typedef std::vector<GameObjectScaredPlantPtr> TGameObjectScaredPlantContainer;
		typedef std::vector<GameObjectTreeDreamsPtr> TGameObjectTreeDreamsContainer;
		typedef std::vector<GameObjectTreeNightmaresPtr> TGameObjectTreeNightmaresContainer;
		typedef std::vector<GameObjectClockPiecePtr> TGameObjectClockPieceContainer;
		typedef std::vector<GameObjectCarnivorousPlantPtr> TGameObjectCarnivorousPlantContainer;
		typedef std::vector<GameObjectSnakeCreeperPtr> TGameObjectSnakeCreeperContainer;
		typedef std::vector<GameObjectWoodBoxPtr> TGameObjectWoodBoxContainer;
		typedef std::vector<GameObjectWaterPtr> TGameObjectWaterContainer;

		//Some object have types in mayus for Ogitor convenience
		const std::string GAME_OBJECT_TYPE="object";
		const std::string GAME_OBJECT_TYPE_SCENE="SceneManager";
		const std::string GAME_OBJECT_TYPE_TERRAIN="terrain";
		const std::string GAME_OBJECT_TYPE_TERRAINCONVEX="terrain-convex";
		const std::string GAME_OBJECT_TYPE_TERRAINTRIANGLE="terrain-triangle";
		const std::string GAME_OBJECT_TYPE_LIGHT="Light";
		const std::string GAME_OBJECT_TYPE_CAMERA="Camera";
		const std::string GAME_OBJECT_TYPE_BILLBOARDSET="BillboardSet";
		const std::string GAME_OBJECT_TYPE_PARTICLESYSTEM="Particle";
		const std::string GAME_OBJECT_TYPE_ONY="ony";
		const std::string GAME_OBJECT_TYPE_TRIPOLLO="tripollo";
		const std::string GAME_OBJECT_TYPE_PORTAL="changeworld";
		const std::string GAME_OBJECT_TYPE_EYE="eye";
		const std::string GAME_OBJECT_TYPE_ITEM_1UP="item1up";
		const std::string GAME_OBJECT_TYPE_ITEM_MAXHP="heartbag";
		const std::string GAME_OBJECT_TYPE_TRIGGERBOX="volume-box";
		const std::string GAME_OBJECT_TYPE_TRIGGERCAPSULE="volume-capsule";
		const std::string GAME_OBJECT_TYPE_VIEWPORT="Viewport1";
		const std::string GAME_OBJECT_TYPE_NIGHTGOBLIN="nightgoblin";
		const std::string GAME_OBJECT_TYPE_CRYKING="cryking";
		const std::string GAME_OBJECT_TYPE_DRAGON="dragon";
		const std::string GAME_OBJECT_TYPE_MAGICCLOCK="magicclock";
		const std::string GAME_OBJECT_TYPE_PILLOW="pillow";
		const std::string GAME_OBJECT_TYPE_FLASHLIGHT="flashlight";
		const std::string GAME_OBJECT_TYPE_TENTETIESO="tentetieso";
		const std::string GAME_OBJECT_TYPE_BEE_BUTTERFLY="bee%butterfly";
		const std::string GAME_OBJECT_TYPE_DIAMONDTREE="diamond_tree";
		const std::string GAME_OBJECT_TYPE_STORYBOOK="storybook";
		const std::string GAME_OBJECT_TYPE_HEART="heart";
		const std::string GAME_OBJECT_TYPE_DIAMOND="diamond";
		const std::string GAME_OBJECT_TYPE_SCAREDPLANT="scaredplant";
		const std::string GAME_OBJECT_TYPE_CLOCKPIECE="clockpiece";
		const std::string GAME_OBJECT_TYPE_PROVISIONALENTITY="provisional-entity";

		const std::string GAME_OBJECT_TYPE_TRIPOLLITO="tripollito";
		const std::string GAME_OBJECT_TYPE_SNAKECREEPER="snakecreeper";
		const std::string GAME_OBJECT_TYPE_CARNIVOROUSPLANT="carnivorousplant";
		const std::string GAME_OBJECT_TYPE_SCEPTER="scepter";
		const std::string GAME_OBJECT_TYPE_TREE="tree";
		const std::string GAME_OBJECT_TYPE_TREE1="tree1";
		const std::string GAME_OBJECT_TYPE_TREE2="tree2";
		const std::string GAME_OBJECT_TYPE_TREE3="tree3";
		const std::string GAME_OBJECT_TYPE_TREE4="tree4";
		const std::string GAME_OBJECT_TYPE_TREE5="tree5";
		const std::string GAME_OBJECT_TYPE_TREE6="tree6";
		const std::string GAME_OBJECT_TYPE_TREE7="tree7";
		const std::string GAME_OBJECT_TYPE_TREE8="tree8";
		const std::string GAME_OBJECT_TYPE_TREE9="tree9";
		const std::string GAME_OBJECT_TYPE_TREE10="tree10";
		const std::string GAME_OBJECT_TYPE_TREE11="tree11";
		const std::string GAME_OBJECT_TYPE_TREE12="tree12";
		const std::string GAME_OBJECT_TYPE_DOOR="door";
		const std::string GAME_OBJECT_TYPE_PLATAFORM="plataform";
		const std::string GAME_OBJECT_TYPE_BUSH="bush";
		const std::string GAME_OBJECT_TYPE_BUSH1="bush1";
		const std::string GAME_OBJECT_TYPE_BUSH2="bush2";
		const std::string GAME_OBJECT_TYPE_BUSH3="bush3";
		const std::string GAME_OBJECT_TYPE_CLOUD="cloud";
		const std::string GAME_OBJECT_TYPE_WOODBOX="woodbox";
		const std::string GAME_OBJECT_TYPE_WATER="water";

		const std::string PRESET_PATROL_TRAJECTORY_PREFIX="patrol_";
		const std::string SUFFIX_TRAJECTORY_DREAMS="_d";
		const std::string SUFFIX_TRAJECTORY_NIGHTMARES="_n";	

		///////////////////////////////////////////////////////////////////

		typedef boost::shared_ptr<GameWorldManager> GameWorldManagerPtr;
		typedef boost::shared_ptr<Component> ComponentPtr;
		
		typedef std::map<std::string,ComponentPtr> TComponentContainer;
		typedef TComponentContainer::iterator TComponentContainerIterator;

		const int GAMEOBJECT_ID_ZERO_PADDING=6;

		///////////////////////////////////////////////////////////////////

		//-------------------------------------
		//	Trajectory module-related constants, type definitions and forwarded declarations
		//-------------------------------------

		class Trajectory;
		class TTrajectoryParameters;

		class TrajectoryNode;
		class TTrajectoryNodeParameters;

		class WalkabilityMap;
		class TWalkabilityMapParameters;
		class TWalkabilityMapNodeParameters;

		class Line3D;

		class TrajectoryManager;
		typedef boost::shared_ptr<TrajectoryManager> TrajectoryManagerPtr;

		class TrajectoryComponent;
		typedef boost::shared_ptr<TrajectoryComponent> TrajectoryComponentPtr;

		const std::string COMPONENT_TYPE_TRAJECTORY="TrajectoryComponent";
		const std::string DEFAULT_WALKABILITY_MAP_DREAMS="dreams";
		const std::string DEFAULT_WALKABILITY_MAP_NIGHTMARES="nightmares";

		//-------------------------------------
		//	Loader module-related constants, type definitions and forwarded declarations
		//-------------------------------------

		class XMLGameObject;
		typedef std::map<std::string,XMLGameObject> XMLGameObjectContainer;
		typedef std::map<std::string,XMLGameObject>::iterator XMLGameObjectContainerIterator;

		class XMLTrajectory;
		typedef std::map<std::string,XMLTrajectory> XMLTrajectoryContainer;
		typedef std::map<std::string,XMLTrajectory>::iterator XMLTrajectoryContainerIterator;

		class XMLWalkabilityMap;
		typedef std::map<std::string,XMLWalkabilityMap> XMLWalkabilityMapContainer;
		typedef std::map<std::string,XMLWalkabilityMap>::iterator XMLWalkabilityMapContainerIterator;

		class XMLParser;
		class Configuration;
		class LevelLoader;

		const std::string DEFAULT_OGRE_RESOURCE_MANAGER_GROUP="General";

		//Exception messages
		const std::string SCENE_NODE_NOT_FOUND="Scene Parameters not found";
		const std::string VIEWPORT_NODE_NOT_FOUND="Viewport Parameters not found";
		const std::string DREAMS_NODE_NOT_FOUND="Dreams node does not exist or has wrong name in Ogitor";   
		const std::string NIGHTMARES_NODE_NOT_FOUND="Nightmares node does not exist or has wrong name in Ogitor";
		const std::string CUSTOM_PROPERTIES_NODE_NOT_FOUND="Custom Properties node does not exist or has wrong .ctp filename";
		const std::string DREAMS_SHOULD_NOT_EXIST="This object can't have dreams node";  
		const std::string NIGHTMARES_SHOULD_NOT_EXIST="This object can't have nightmares node";  

		typedef boost::shared_ptr<Configuration> ConfigurationPtr;
		typedef boost::shared_ptr<LevelLoader> LevelLoaderPtr;

		const std::string GAME_OBJECT_PARAMETERS_PATH="../../Resources/Levels/GameObjects/";
		const std::string LEVELS_PATH="../../Resources/Levels/";

		///	Type definition for the data structure that'll hold the options:
		/// an STL map of pairs of strings, representing the key and value, respectively
		typedef std::map<std::string, std::string> TConfigMap;

		///Alias for a Config map iterator
		typedef TConfigMap::iterator TConfigMapIterator;
		///Alias for a Config map constant iterator
		typedef TConfigMap::const_iterator TConfigMapConstIterator;

		//-------------------------------------
		//	Graphics module-related constants, type definitions and forwarded declarations
		//-------------------------------------

		#define COMPOSITOR_CFG "../../Config/compositor-cfg.xml"

		const int DEFAULT_OGRE_MIPMAPS_NUMBER=5;
		const std::string MAIN_CAMERA_NAME="Camera";
		const std::string RUNNING_CAMERA_NAME="Camera";
		// Default path constants
		const std::string DEFAULT_OGRE_RESOURCES_PATH="../../Config/resources.cfg";
		const std::string DEFAULT_OGRE_CONFIG_PATH  = "../../Config/ogre.cfg";
		const std::string DEFAULT_OGRE_LOG_PATH = "Ogre.log";
		//Fetch the plugins file conditionally, depending on whether we're on debug mode or not
		#ifdef OUAN_DEBUG
				const std::string DEFAULT_OGRE_PLUGINS_PATH="../../Config/plugins-dbg.cfg";
		#else
				const std::string DEFAULT_OGRE_PLUGINS_PATH="../../Config/plugins.cfg";
		#endif	

		const double DEFAULT_MOVE_SPEED = 1;
		const double DEFAULT_ROTATE_SPEED=0.3f;
		const double MOUSE_MOVE_ROTATION_INCREMENT=0.1f;

		typedef enum
		{
			AXIS_POS_X,
			AXIS_NEG_X,
			AXIS_POS_Y,
			AXIS_NEG_Y,
			AXIS_POS_Z,
			AXIS_NEG_Z
		}TCoordinateAxis;

		class RenderSubsystem;
		
		typedef boost::shared_ptr<RenderSubsystem> RenderSubsystemPtr;

		///Camera Manager
		const std::string CAMERA_CFG="../../Config/camera-cfg.xml";
		class CameraManager;
		typedef boost::shared_ptr<CameraManager> CameraManagerPtr;

		class CameraController;
		class CameraControllerFirstPerson;
		class CameraControllerThirdPerson;
		class CameraControllerFixedFirstPerson;
		class CameraControllerFixedThirdPerson;
		class CameraControllerTrajectory;

		typedef std::map<std::string,TRenderComponentCameraParameters> TCameraParamsContainer;
		typedef TCameraParamsContainer::iterator TCameraParamsContainerIterator;

		/// Ogre root object
		typedef boost::shared_ptr<Ogre::Root> RootPtr;

		typedef enum 
		{
			CAMERA_NONE,
			CAMERA_FIXED_FIRST_PERSON,
			CAMERA_FIXED_THIRD_PERSON,
			CAMERA_FIRST_PERSON,
			CAMERA_THIRD_PERSON,
			CAMERA_TRAJECTORY
		}TCameraControllerType;

		//-------------------------------------
		//	Physics module-related constants, type definitions and forwarded declarations
		//-------------------------------------

		class PhysicsSubsystem;
		typedef boost::shared_ptr<PhysicsSubsystem> PhysicsSubsystemPtr;

		enum GameGroup
		{
			GROUP_NON_COLLIDABLE,
			GROUP_COLLIDABLE_NON_PUSHABLE,
			GROUP_COLLIDABLE_PUSHABLE,
		};

		#define GROUP_COLLIDABLE_MASK (1 << GROUP_COLLIDABLE_NON_PUSHABLE) | (1 << GROUP_COLLIDABLE_PUSHABLE)
		#define NXS_PATH "file:../../Resources/Graphics/NxModels"
		#define PHYSICS_CFG "../../Config/physics-cfg.xml"

		#define COLLISION_TYPE_TRIGGER_UNKNOWN 0
		#define COLLISION_TYPE_TRIGGER_ENTER 1
		#define COLLISION_TYPE_TRIGGER_PRESENCE 2
		#define COLLISION_TYPE_TRIGGER_EXIT 3

		#define PI 3.14159265
		#define TO_DEGREES 57.296 // 180/PI
		#define TO_RADIANS 0.0174532925 // PI/180

		//-------------------------------------
		//  Particle Systems
		//-------------------------------------

		#define PARTICLES_CFG "../../Config/particles-cfg.xml"

		//-------------------------------------
		//  Movements masks
		//-------------------------------------
		
		const int MOV_NOWHERE = 0;
		const int MOV_GO_FORWARD = 1;
		const int MOV_GO_BACK = 2;
		const int MOV_GO_LEFT = 4;
		const int MOV_GO_RIGHT = 8;
		const int MOV_JUMP = 16;
		const int MOV_WALK = 32;

		const int MOV_FORWARD_OR_BACK = MOV_GO_FORWARD | MOV_GO_BACK;
		const int MOV_LEFT_OR_RIGHT = MOV_GO_LEFT | MOV_GO_RIGHT;
}

#endif