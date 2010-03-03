#ifndef GAMEWORLDMANAGERH_H
#define GAMEWORLDMANAGERH_H
#include "../OUAN.h"

namespace OUAN
{

	/// Manages the game scene:
	/// the landscape, all of its objects (creatures, props,...)
	class GameWorldManager
	{
	public:
		GameWorldManager();
		~GameWorldManager();
		/// find gameObject by Id
		/// @param objectId identifier of the object to retrieve
		/// @return pointer to the object, if found
		GameObjectPtr getObject(const std::string& objectId);

		TGameObjectMovableContainer getGameObjectMovableContainer();
		TGameObjectMovableEntityContainer getGameObjectMovableEntityContainer();
		TGameObjectNonMovableContainer getGameObjectNonMovableContainer();
		TGameObjectNonMovableEntityContainer getGameObjectNonMovableEntityContainer();
		TGameObjectTerrainContainer getGameObjectTerrainContainer();
		TGameObjectLightContainer getGameObjectLightContainer();
		TGameObjectParticleSystemContainer getGameObjectParticleSystemContainer();
		TGameObjectBillboardSetContainer getGameObjectBillboardSetContainer();
		TGameObjectOnyContainer getGameObjectOnyContainer();
		TGameObjectPositionalContainer getGameObjectPositionalContainer();
		TGameObjectSceneContainer getGameObjectSceneContainer();
		TGameObjectTripolloContainer getGameObjectTripolloContainer();
		TGameObjectEyeContainer getGameObjectEyeContainer();
		TGameObjectVolumeBoxContainer getGameObjectVolumeBoxContainer();
		TGameObjectVolumeCapsuleContainer getGameObjectVolumeCapsuleContainer();
		TGameObjectViewportContainer getGameObjectViewportContainer();

		TGameObjectPhysicsCharacterContainer getGameObjectPhysicsCharacterContainer();
		TGameObjectPhysicsComplexConvexContainer getGameObjectPhysicsComplexConvexContainer();
		TGameObjectPhysicsComplexTriangleContainer getGameObjectPhysicsComplexTriangleContainer();
		TGameObjectPhysicsSimpleContainer getGameObjectPhysicsSimpleContainer();
		TGameObjectPhysicsVolumeContainer getGameObjectPhysicsVolumeContainer();

		/// Load level from the given file
		/// @param levelFileName name of the level file
		void loadLevel (const std::string& levelFileName);

		/// Unload data from current level
		/// (i.e, clear all object lists, destroy landscape
		/// and so on)
		void unloadLevel();

		/// init object
		/// @param pointer to the application object
		void init(ApplicationPtr app);
		/// Free resources
		void cleanUp();

		/// Load global parameters
		void initGlobalWorldData( /*const TGlobalWorldParameters& worldParams*/);

		//Create Ogre component for the game object
		RenderComponentPtr createRenderComponent(GameObjectPtr gameObject);

		/// Create a game object using the given 'parameters' object
		/// @param objectParams parameters to init the game object and its components

		//void createGameObjectMovableEntity(TGameObjectMovableEntityParameters tGameObjectMovableEntityParameters);
		void createGameObjectOny(TGameObjectOnyParameters tGameObjectOnyParameters);
		void createGameObjectTripollo(TGameObjectTripolloParameters tGameObjectTripolloParameters);
		void createGameObjectLight(TGameObjectLightParameters tGameObjectLightParameters);
		void createGameObjectBillboardSet(TGameObjectBillboardSetParameters tGameObjectBillboardSetParameters);
		void createGameObjectParticleSystem(TGameObjectParticleSystemParameters tGameObjectParticleSystemParameters);
		void createGameObjectTerrain(TGameObjectTerrainParameters tGameObjectTerrainParameters);
		void createGameObjectItem1UP(TGameObjectItem1UPParameters tGameObjectItem1UPParameters);
		void createGameObjectEye(TGameObjectEyeParameters tGameObjectEyeParameters);
		void createGameObjectPortal(TGameObjectPortalParameters tGameObjectPortalParameters);
		void createGameObjectItemMaxHP(TGameObjectItemMaxHPParameters tGameObjectItemMaxHPParameters);
		void createGameObjectCamera(TGameObjectCameraParameters tGameObjectCameraParameters);
		void createGameObjectVolumeBox(TGameObjectVolumeBoxParameters tGameObjectVolumeBoxParameters);
		void createGameObjectVolumeCapsule(TGameObjectVolumeCapsuleParameters tGameObjectVolumeCapsuleParameters);
		void createGameObjectViewport(TGameObjectViewportParameters tGameObjectViewportParameters);

		//void createSceneNode(TSceneNodeParameters tSceneNodeParameters);
		//void createLight(TLightParameters tLightParameters);
		//void createParticleSystem(TParticleSystemParameters tParticleSystemParameters);
		//void createCamera(TCameraParameters tCameraParameters);
		//void createBillboardSet(TBillboardSetParameters tBillboardSetParameters);
		//void createViewport(TViewportParameters tViewportParameters);
		//void createSceneManager(TSceneManagerParameters tSceneManagerParameters);


		/// Update world state
		/// @param elapsedTime time since last update
		void update(long elapsedTime);

		/// [TODO] Add input event handlers
		/// so that objects using a inputController
		/// (i.e, Ony, Free-lookaround cameras)
		/// can be notified

		/// Return an id value and increment it
		/// @return id value
		inline unsigned long nextId() {return mNextIdNum++;}

		/// Return the last id value used
		/// @return last id value used
		inline unsigned long lastId() {return mNextIdNum;}

		/// Tell if the game over flag is set to true
		/// @return value of the "game over" flag
		bool isGameOver() const;

		/// Set the value of the game over flag
		/// @param gameOver	value to set
		void setGameOver(bool gameOver);

	private:

		/// Add the game object pointer to the objects List
		/// @param gameObject pointer to the object to add to the main list
		void addGameObject(GameObjectPtr gameObject);
		void addGameObjectOny(GameObjectOnyPtr pGameObjectOny);
		void addGameObjectTripollo(GameObjectTripolloPtr pGameObjectTripollo);
		void addGameObjectTerrain(GameObjectTerrainPtr pGameObjectTerrain);
		void addGameObjectLight(GameObjectLightPtr pGameObjectLight);
		void addGameObjectParticleSystem(GameObjectParticleSystemPtr pGameObjectParticleSystem);
		void addGameObjectBillboardSet(GameObjectBillboardSetPtr pGameObjectBillboardSet);
		void addGameObjectItem1UP(GameObjectItem1UPPtr pGameObjectItem1UP);
		void addGameObjectItemMaxHP(GameObjectItemMaxHPPtr pGameObjectItemMaxHP);
		void addGameObjectPortal(GameObjectPortalPtr pGameObjectPortal);
		void addGameObjectEye(GameObjectEyePtr pGameObjectEye);
		void addGameObjectCamera(GameObjectCameraPtr pGameObjectCamera);
		void addGameObjectVolumeBox(GameObjectVolumeBoxPtr pGameObjectVolumeBox);
		void addGameObjectVolumeCapsule(GameObjectVolumeCapsulePtr pGameObjectVolumeCapsule);
		void addGameObjectViewport(GameObjectViewportPtr pGameObjectViewport);

		//void addGameObjectMovableEntity(GameObjectMovableEntityPtr gameObjectMovableEntity);
		//[TODO]: Do the same with the toAdd/toDelete lists?

		/// Generate a unique id string with the following format:
		/// "baseString"_"00value"
		/// @param baseString	reference string to make the id
		/// @param padding		number of spaces for the value.
		///						if the number is too small, it'll be padded
		///						with 'padding' zeroes
		/// @param value		the value to append to the string
		/// @return id string
		std::string makeIdString(const std::string& baseString,const int& padding, const unsigned long& value);
		//[TODO]: Uncomment line with the LevelLoader member object
		// when available (unless it's a singleton?)
		/// Level file loader
		//LevelLoaderPtr mLoader;

		/// Pointer to the main application object
		ApplicationPtr mApp;
		
		///number used to generate a unique identifier
		///for every game object that is created
		static unsigned long mNextIdNum;

		/// Game objects currently existing in the world
		TGameObjectContainer mGameObjects;

		/// Game object containers to access specific game object types
		TGameObjectMovableContainer mGameObjectMovableContainer;
		TGameObjectMovableEntityContainer mGameObjectMovableEntityContainer;
		TGameObjectNonMovableContainer mGameObjectNonMovableContainer;
		TGameObjectNonMovableEntityContainer mGameObjectNonMovableEntityContainer;
		TGameObjectLightContainer mGameObjectLightContainer;
		TGameObjectParticleSystemContainer mGameObjectParticleSystemContainer;
		TGameObjectBillboardSetContainer mGameObjectBillboardSetContainer;
		TGameObjectTerrainContainer mGameObjectTerrainContainer;
		TGameObjectOnyContainer mGameObjectOnyContainer;
		TGameObjectPositionalContainer mGameObjectPositionalContainer;
		TGameObjectSceneContainer mGameObjectSceneContainer;
		TGameObjectTripolloContainer mGameObjectTripolloContainer;
		TGameObjectEyeContainer mGameObjectEyeContainer;
		TGameObjectVolumeBoxContainer mGameObjectVolumeBoxContainer;
		TGameObjectVolumeCapsuleContainer mGameObjectVolumeCapsuleContainer;

		TGameObjectPhysicsCharacterContainer mGameObjectPhysicsCharacterContainer;
		TGameObjectPhysicsComplexConvexContainer mGameObjectPhysicsComplexConvexContainer;
		TGameObjectPhysicsComplexTriangleContainer mGameObjectPhysicsComplexTriangleContainer;
		TGameObjectPhysicsSimpleContainer mGameObjectPhysicsSimpleContainer;
		TGameObjectPhysicsVolumeContainer mGameObjectPhysicsVolumeContainer;

		TGameObjectViewportContainer mGameObjectViewportContainer;

		void clearContainers();

		// Lists of objects to be created/deleted as a result of an
		// update frame. 
		// Addition/Deletion will be effective just before the next
		// update cycle
		TGameObjectContainer mGameObjectsToAdd;
		TGameObjectContainer mGameObjectsToDelete;

		bool mGameOver;
		

	};
}
#endif