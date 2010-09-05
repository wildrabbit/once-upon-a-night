#ifndef GameObjectWaterH_H
#define GameObjectWaterH_H

#include "GameObject.h"
#include "../../Graphics/RenderComponent/RenderComponentWater.h"
#include "../../Graphics/RenderComponent/RenderComponentInitial.h"
#include "../../Graphics/RenderComponent/RenderComponentPositional.h"
#include "../../Physics/PhysicsComponent/PhysicsComponentVolumeConvex.h"
#include "../../Logic/LogicComponent/LogicComponent.h"
namespace OUAN
{
	/// Class to hold terrain information
	class GameObjectWater : public GameObject, public boost::enable_shared_from_this<GameObjectWater>
	{
	private:
		/// Visual information
		RenderComponentWaterPtr mRenderComponentWaterDreams;
		RenderComponentWaterPtr mRenderComponentWaterNightmares;
		/// Position information
		RenderComponentInitialPtr mRenderComponentInitial;
		RenderComponentPositionalPtr mRenderComponentPositional;
		/// Physics information
		PhysicsComponentVolumeConvexPtr mPhysicsComponentVolumeConvex;
		/// Logic component: it'll represent the 'brains' of the game object
		/// containing information on its current state, its life and health(if applicable),
		/// or the world(s) the object belongs to
		LogicComponentPtr mLogicComponent;
		//TODO: think what happens when world changes with the rendercomponent
		/// Audio component
		AudioComponentPtr mAudioComponent;
	public:
		//Constructor
		GameObjectWater(const std::string& name);
		//Destructor
		~GameObjectWater();
		/// Return render component entity 
		/// @return render component entity
		RenderComponentWaterPtr getRenderComponentWaterDreams() const;
		RenderComponentWaterPtr getRenderComponentWaterNightmares() const;
		/// Set logic component
		void setLogicComponent(LogicComponentPtr logicComponent);

		/// return logic component
		LogicComponentPtr getLogicComponent();

		/// Set render component
		/// @param pRenderComponentWater
		void setRenderComponentWaterDreams(RenderComponentWaterPtr pRenderComponentWaterDreams);
		void setRenderComponentWaterNightmares(RenderComponentWaterPtr pRenderComponentWaterNightmares);

		/// Set audio component
		/// @param pRenderComponentEntity
		AudioComponentPtr getAudioComponent() const;
		void setAudioComponent(AudioComponentPtr audioComponent);


		/// Set positional component
		/// @param pRenderComponentPositional the component containing the positional information
		void setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional);

		/// Set initial component
		void setRenderComponentInitial(RenderComponentInitialPtr pRenderComponentInitial);

		/// Return positional component 
		/// @return positional component
		RenderComponentPositionalPtr getRenderComponentPositional() const;

		/// Return initial component 
		/// @return initial component
		RenderComponentInitialPtr getRenderComponentInitial() const;

		void setVisible(bool visible);

		/// Set physics component
		void setPhysicsComponentVolumeConvex(PhysicsComponentVolumeConvexPtr pPhysicsComponentVolumeConvex);

		/// Get physics component
		PhysicsComponentVolumeConvexPtr getPhysicsComponentVolumeConvex() const;

		/// React to a world change to the one given as a parameter
		/// @param world world to change to
		void changeToWorld(int newWorld, double perc);
		void changeWorldFinished(int newWorld);
		void changeWorldStarted(int newWorld);

		void setDreamsRender();
		void setNightmaresRender();
		void setChangeWorldRender();
		void setChangeWorldFactor(double factor);

		/// Reset object
		virtual void reset();

		bool hasPositionalComponent() const;
		RenderComponentPositionalPtr getPositionalComponent() const;

		bool hasPhysicsComponent() const;
		PhysicsComponentPtr getPhysicsComponent() const;

		//bool hasRenderComponentEntity() const;
		//RenderComponentEntityPtr getEntityComponent() const;

		/// Process collision event
		/// @param gameObject which has collision with
		void processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal);

		/// Process collision event
		/// @param gameObject which has collision with
		void processEnterTrigger(GameObjectPtr pGameObject);

		/// Process collision event
		/// @param gameObject which has collision with
		void processExitTrigger(GameObjectPtr pGameObject);

		
		

		void postUpdate();
		void update(double elapsedSeconds);

		bool hasLogicComponent() const;
		LogicComponentPtr getLogicComponentInstance() const;
	};

	class TGameObjectWaterParameters: public TGameObjectParameters
	{
	public:
		TGameObjectWaterParameters();
		~TGameObjectWaterParameters();
		
		///Parameters specific to an Ogre Entity
		TRenderComponentWaterParameters tRenderComponentWaterDreamsParameters;
		TRenderComponentWaterParameters tRenderComponentWaterNightmaresParameters;

		///Positional parameters
		TRenderComponentPositionalParameters tRenderComponentPositionalParameters;

		///Physics parameters
		TPhysicsComponentVolumeConvexParameters tPhysicsComponentVolumeConvexParameters;

		/// Audio component params
		TAudioComponentMap tAudioComponentParameters;


		///Logic parameters
		TLogicComponentParameters tLogicComponentParameters;
	};
}
#endif