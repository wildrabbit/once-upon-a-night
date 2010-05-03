#ifndef PHYSICSSUBSYSTEMH_H
#define PHYSICSSUBSYSTEMH_H

#include "../OUAN.h"
#include "../Loader/Configuration.h"
#include <NxOgre.h>
#include <NxOgreOGRE3D.h>
#include <NxOgreAddonCharacterController.h>

namespace OUAN
{
	//This class encapsulates the physics logic
	//of our game. It uses NxOgre BloodyMess v.1.5.5
	class PhysicsSubsystem : public NxOgre::Callback, public NxOgre::ControllerCallback
	{
	public:

		//Constructor
		PhysicsSubsystem();
	
		//Destructor
		~PhysicsSubsystem();
		
		//Initialize physics subsystem elements
		virtual void init(ApplicationPtr app,OUAN::ConfigurationPtr config);

		//Destroy and free memory
		virtual void cleanUp();

		//Initialize physics level elements
		virtual void initLevel(std::string sceneName);

		//Destroy physics level elements
		virtual void clear();

		// Update physics subsystem elements
		virtual void update(double elapsedSeconds);

		//stabilize objects,  needs to be called at load level and change world
		void stabilize();

		//Getters
		virtual NxOgre::World* getNxOgreWorld();
		virtual NxOgre::Scene* getNxOgreScene();
		virtual OGRE3DRenderSystem* getNxOgreRenderSystem();
		virtual NxOgre::TimeController* getNxOgreTimeController();
		virtual NxOgre::ControllerManager* getNxOgreControllerManager();

		//General physics callbacks
		virtual bool onHitEvent(const NxOgre::RaycastHit& raycastHit);
		virtual void onContact(const NxOgre::ContactPair& contactPair);
		virtual void onVolumeEvent( NxOgre::Shape * volume, NxOgre::String collisionName, unsigned int collisionEventType );

		//Specific physics character callbacks
		virtual NxOgre::Enums::ControllerAction onShape(const NxOgre::ControllerShapeHit& hit);
		virtual NxOgre::Enums::ControllerAction onController(const NxOgre::ControllerControllerHit& hit);

		/// param read from config file, gravity force
		NxOgre::Vec3 mGravity;

		/// param
		double mStaticFriction;

		/// param
		double mDynamicFriction;

		/// param
		double mRestitution;

		/// param read from config file, applied to move()
		double mDisplacementScale;

		/// param read from config file, applied to move()
		double mMinDistance;

		/// param read from config file, applied to move()
		double mMovementUnitsPerSecond;

		/// param read from config file, applied to move()
		double mMovementLimitUnitsPerSecond;

		/// param read from config file, applied to move()
		double mTurnDegreesPerSecond;

		/// param read from config file, applied to jump
		double mInitialJumpSpeed;

		/// param read from config file
		double mMinAllowedY;

		/// param read from config file
		double mMinCollisionAngle;

		/// param read from config file
		double mMinSlidingAngle;

		/// param read from config file
		double mMinSlidingAngleFall;

		/// param read from config file
		double mSlidingFactor;

		/// param read from config file
		double mSlidingFactorFall;

		/// param read from config file
		double mDashFactor;
		double mDashMax;
		double mDashAccelerationIncrement;
		double mMinDashAccelerationFactor;
		double mMaxDashAccelerationFactor;

		/// param read from config file
		double mAccelerationIncrement;
		double mMaxAccelerationFactor;
		double mMinAccelerationFactor;

		/// param read from config file
		double mMaxSameDirectionAngle;

		/// param read from config file
		double mWalkSpeed;

		/// Load params from config file
		virtual bool loadConfig();

		bool raycastClosestGeometry(const Vector3 &point,const Vector3 &normal,Vector3 &result,double maxDistance=-1,QueryFlags flags=QUERYFLAGS_NONE);
		bool raycastClosestBoundings(const Vector3 &point,const Vector3 &normal,Vector3 &result,double maxDistance=-1,QueryFlags flags=QUERYFLAGS_NONE);

		//returns the number of hits
		int raycastAllBoundings(const Vector3 &point,const Vector3 &normal,std::vector<GameObjectPtr> &result,double maxDistance=-1,QueryFlags flags=QUERYFLAGS_NONE);

	protected:

		/// the application
		OUAN::ApplicationPtr mApp;

		/// the configuration
		OUAN::ConfigurationPtr mConfig;

		/// NxOgre world
		NxOgre::World* mNxOgreWorld;
		
		/// NxOgre main scene
		NxOgre::Scene* mNxOgreScene;

		/// NxOgre render system
		OGRE3DRenderSystem* mNxOgreRenderSystem;

		/// NxOgre time controller
		NxOgre::TimeController* mNxOgreTimeController;

		/// NxOgre controller manager
		NxOgre::ControllerManager* mNxOgreControllerManager;

		/// Auxiliar function
		bool setGameObjectSlidingFromController(NxOgre::Controller* controller, NxOgre::Vec3 normal, double normalAngle);

		/// Auxiliar function
		bool areClose(NxOgre::Vec3 position1, NxOgre::Vec3 position2, double radius);

		/// Auxiliar function
		bool isOnyCloseFromPosition(NxOgre::Vec3 position, double radius);

		/// Fetch function
		GameObjectPtr getGameObject(NxOgre::String name);

		/// Sends collisions to the EventManager function
		bool sendCollision(GameObjectPtr object1, GameObjectPtr object2);
		bool sendEnterTrigger(GameObjectPtr object1, GameObjectPtr object2);
		bool sendExitTrigger(GameObjectPtr object1, GameObjectPtr object2);
	};
}
#endif