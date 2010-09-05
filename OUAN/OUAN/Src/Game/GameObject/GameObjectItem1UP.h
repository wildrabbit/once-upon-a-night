#ifndef GameObjectItem1UPH_H
#define GameObjectItem1UPH_H

#include "GameObject.h"
#include "../../Graphics/RenderComponent/RenderComponentEntity.h"
#include "../../Graphics/RenderComponent/RenderComponentInitial.h"
#include "../../Graphics/RenderComponent/RenderComponentPositional.h"
#include "../../Physics/PhysicsComponent/PhysicsComponentVolumeBox.h"
#include "../../Logic/LogicComponent/LogicComponentItem.h"

namespace OUAN
{

	const std::string ITEM1UP_ANIM_IDLE="idle_Clip";

	/// Class to hold GameObjectItem1UP information
	class GameObjectItem1UP : public GameObject, public boost::enable_shared_from_this<GameObjectItem1UP>
	{
	private:
		
		/// Visual information
		RenderComponentEntityPtr mRenderComponentEntity;
		/// Position information
		RenderComponentInitialPtr mRenderComponentInitial;
		RenderComponentPositionalPtr mRenderComponentPositional;

		//// Particle Systems
		RenderComponentParticleSystemPtr mRenderComponentParticleSystemStarsCloud;

		/// Physics information
		PhysicsComponentVolumeBoxPtr mPhysicsComponentVolumeBox;

		/// Logic component: it'll represent the 'brains' of the game object
		/// containing information on its current state, its life and health(if applicable),
		/// or the world(s) the object belongs to
		LogicComponentItemPtr mLogicComponentItem;
		/// Audio component
		AudioComponentPtr mAudioComponent;
	public:
		//Constructor
		GameObjectItem1UP(const std::string& name);
		//Destructor
		~GameObjectItem1UP();

		/// Set logic component
		void setLogicComponentItem(LogicComponentItemPtr logicComponentItem);

		/// return logic component
		LogicComponentItemPtr getLogicComponentItem();

		/// Set audio component
		/// @param pRenderComponentEntity
		AudioComponentPtr getAudioComponent() const;
		void setAudioComponent(AudioComponentPtr audioComponent);


		/// Return render component entity 
		/// @return render component entity
		RenderComponentEntityPtr getRenderComponentEntity() const;

		/// Set render component
		/// @param pRenderComponentEntity
		void setRenderComponentEntity(RenderComponentEntityPtr pRenderComponentEntity);

		void setVisible(bool visible);

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

		/// Set Particle Systems
		void setRenderComponentParticleSystemStarsCloud(RenderComponentParticleSystemPtr mRenderComponentParticleSystemStarsCloud);

		/// Get Particle Systems
		RenderComponentParticleSystemPtr getRenderComponentParticleSystemStarsCloud() const;

		/// Set physics component
		void setPhysicsComponentVolumeBox(PhysicsComponentVolumeBoxPtr pPhysicsComponentVolumeBox);

		/// Get physics component
		PhysicsComponentVolumeBoxPtr getPhysicsComponentVolumeBox() const;

		/// React to a world change to the one given as a parameter
		/// @param world world to change to
		void changeToWorld(int newWorld, double perc);
		void changeWorldFinished(int newWorld);
		void changeWorldStarted(int newWorld);
		void calculateChangeWorldTotalTime(double changeWorldTotalTime);
		void calculateChangeWorldDelay(double totalElapsedTime,double totalTime,int newWorld,double delay_factor,double intersection);

		void setDreamsRender();
		void setNightmaresRender();
		void setChangeWorldRender();
		void setChangeWorldFactor(double factor);
		
		virtual void disable();

		/// Reset object
		virtual void reset();
		virtual void enable();

		bool hasPositionalComponent() const;
		RenderComponentPositionalPtr getPositionalComponent() const;

		bool hasPhysicsComponent() const;
		PhysicsComponentPtr getPhysicsComponent() const;

		bool hasRenderComponentEntity() const;
		RenderComponentEntityPtr getEntityComponent() const;

		/// Process collision event
		/// @param gameObject which has collision with
		void processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal);

		/// Process collision event
		/// @param gameObject which has collision with
		void processEnterTrigger(GameObjectPtr pGameObject);

		/// Process collision event
		/// @param gameObject which has collision with
		void processExitTrigger(GameObjectPtr pGameObject);

		/// Update object
		void update(double elapsedSeconds);

		bool hasLogicComponent() const;
		LogicComponentPtr getLogicComponentInstance() const;

		void startCollisionEffects();
	};

	class TGameObjectItem1UPParameters: public TGameObjectParameters
	{
	public:
		TGameObjectItem1UPParameters();
		~TGameObjectItem1UPParameters();
		
		///Parameters specific to an Ogre Entity
		TRenderComponentEntityParameters tRenderComponentEntityParameters;

		///Positional parameters
		TRenderComponentPositionalParameters tRenderComponentPositionalParameters;

		///Physics parameters
		TPhysicsComponentVolumeBoxParameters tPhysicsComponentVolumeBoxParameters;

		/// Audio component params
		TAudioComponentMap tAudioComponentParameters;


		///Logic parameters
		TLogicComponentItemParameters tLogicComponentItemParameters;
	};
}
#endif