#ifndef CameraManagerH_H
#define CameraManagerH_H
#include "../../OUAN.h"
#include "../../Event/EventDefs.h"
namespace OUAN
{
	class CameraManager
	{
	public:
		CameraManager();
		~CameraManager();

		void init(RenderSubsystemPtr pRenderSubsystem,TrajectoryManagerPtr pTrajectoryManager,PhysicsSubsystemPtr pPhysicsSubsystem,RayCastingPtr pRayCasting, GameWorldManagerPtr pGameWorldManager);

		/// Free resources
		void cleanUp();

		//void createCamera(std::string name,TRenderComponentCameraParameters tRenderComponentCameraParameters);

		void clear();

		Ogre::Camera * getCamera(std::string name);

		void setCameraType(TCameraControllerType cameraControllerType);

		void update(double elapsedTime);

		void processMouseInput(const OIS::MouseEvent &e);
		void processCameraRotation(Ogre::Vector2 cameraRotation);
		void processSimpleTranslation(Ogre::Vector3 nextMovement);

		void resetActiveCameraController();

		// Process a movement vector in order to fit with the current active camera.
		Ogre::Vector3 rotateMovementVector(Ogre::Vector3 movement);

		/// Return read-only pointer to the viewport
		/// @return viewport
		Ogre::Viewport* getViewport() const;

		/// Activates next camera controller type
		void changeCameraController();

		//Sets camera target for applicable controllers
		void setCameraTarget(RenderComponentPositionalPtr target);

		//Sets camera trajectory for trajectory controller
		void setCameraTrajectory(std::string name);

		//Returns active camera controller type
		TCameraControllerType getActiveCameraControllerType();

		Ogre::Viewport* setViewportParameters(Ogre::String name,TRenderComponentViewportParameters tRenderComponentViewportParameters);

		Ogre::Camera * getCamera() const;

		//register/unregister
		void registerEventHandlers(EventManagerPtr evtMgr);
		void unregisterEventHandlers(EventManagerPtr evtMgr);
		void processChangeWorld(ChangeWorldEventPtr evt);

	private:
		/// GameWorldManager
		GameWorldManagerPtr mGameWorldManager;
		/// GameWorldManager
		Ogre::SceneManager* mSceneManager;
		/// Viewport
		Ogre::Viewport* mViewport;
		/// Trajectory Manager
		TrajectoryManagerPtr mTrajectoryManager;

		//Creates main camera, which always exists
		void createMainCamera();

		Ogre::Camera * mCamera;

		CameraController * activeCameraController;
		CameraControllerFirstPerson * mCameraControllerFirstPerson;
		CameraControllerThirdPerson * mCameraControllerThirdPerson;
		CameraControllerFixedThirdPerson * mCameraControllerFixedThirdPerson;
		CameraControllerFixedFirstPerson * mCameraControllerFixedFirstPerson;
		CameraControllerTrajectory * mCameraControllerTrajectory;

		//Changeworld functions and variables
		void changeToWorld(int newWorld, double perc);
		void changeWorldFinished(int newWorld);
		void changeWorldStarted(int newWorld);
		void activateChangeWorld();
		void activateChangeWorldFast();

		double mChangeWorldTotalTime;
		double mChangeWorldElapsedTime;
		bool mIsChangingWorld;
		int mWorld;

		
	};
}

#endif