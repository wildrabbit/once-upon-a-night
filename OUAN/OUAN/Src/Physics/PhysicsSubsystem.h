#ifndef PHYSICSSUBSYSTEMH_H
#define PHYSICSSUBSYSTEMH_H

#include "../OUAN.h"
#include <NxOgre.h>
#include <NxOgreOGRE3D.h>
#include <NxOgreAddonCharacterController.h>

namespace OUAN
{
	//This class encapsulates the physics logic
	//of our game. It uses NxOgre BloodyMess v.1.5.5
	class PhysicsSubsystem
	{
	public:

		//Constructor
		PhysicsSubsystem();

		//Destructor
		~PhysicsSubsystem();
		
		//Initialize physics subsystem elements
		virtual void initialise(ApplicationPtr app,OUAN::ConfigurationPtr config);

		//Free memory used by the physics subsystem
		virtual void cleanUp();

		//Getters
		virtual NxOgre::World* getNxOgreWorld();
		virtual NxOgre::Scene* getNxOgreScene();
		virtual OGRE3DRenderSystem* getNxOgreRenderSystem();
		virtual NxOgre::TimeController* getNxOgreTimeController();
		virtual NxOgre::ControllerManager* getNxOgreControllerManager();

	protected:
		
		/// the application
		OUAN::ApplicationPtr mApp;

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
	};
}
#endif