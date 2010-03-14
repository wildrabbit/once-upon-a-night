#ifndef CameraManagerH_H
#define CameraManagerH_H
#include "../../OUAN.h"
namespace OUAN
{
	class CameraManager
	{
	public:
		CameraManager();
		~CameraManager();

		void init(RootPtr pRoot,Ogre::SceneManager * pSceneManager,TrajectoryManager * pTrajectoryManager);

		/// Free resources
		void cleanUp();

		RenderComponentCameraPtr createCamera(std::string name,TRenderComponentCameraParameters tRenderComponentCameraParameters);

		void clear();

		void setActiveCamera(std::string name);
		std::string getActiveCameraName();

		Ogre::Camera * getActiveCamera();

		void setCameraType(TCameraControllerType cameraControllerType);

		void resetActiveCameraPosition();

		void update(double elapsedTime);

		void processMouseInput(const OIS::MouseEvent &e);
		void processRelativeMotion(double xRel,double yRel,double zRel);
		void processSimpleTranslation(int movementFlags);

		/// Return read-only pointer to the viewport
		/// @return viewport
		Ogre::Viewport* getViewport() const;

		/// Activates next camera
		void changeCamera();
		/// Activates next camera controller type
		void changeCameraController();

		//Sets camera target for applicable controllers
		void setCameraTarget(RenderComponentPositional * target);

		//Sets camera trajectory for trajectory controller
		void setCameraTrajectory(std::string name);

		//Returns active camera controller type
		TCameraControllerType getActiveCameraControllerType();

	private:
		/// GameWorldManager
		GameWorldManagerPtr mGameWorldManager;
		/// GameWorldManager
		Ogre::SceneManager* mSceneManager;
		/// Viewport
		Ogre::Viewport* mViewport;
		/// Trajectory Manager
		TrajectoryManager * mTrajectoryManager;

		//Creates main camera, which always exists
		void createMainCamera();

		std::map<std::string,RenderComponentCameraPtr> camera;
		typedef std::map<std::string,RenderComponentCameraPtr>::iterator TCameraIterator;

		CameraController * activeCameraController;
		CameraControllerFirstPerson * mCameraControllerFirstPerson;
		CameraControllerThirdPerson * mCameraControllerThirdPerson;
		CameraControllerFixedThirdPerson * mCameraControllerFixedThirdPerson;
		CameraControllerFixedFirstPerson * mCameraControllerFixedFirstPerson;
		CameraControllerTrajectory * mCameraControllerTrajectory;
		
	};
}

#endif