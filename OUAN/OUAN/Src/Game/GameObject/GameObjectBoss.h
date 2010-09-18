#ifndef GameObjectBossH_H
#define GameObjectBossH_H

#include "GameObject.h"
#include "../../Graphics/RenderComponent/RenderComponentInitial.h"
#include "../../Graphics/RenderComponent/RenderComponentPositional.h"
#include "../../Graphics/RenderComponent/RenderComponentEntity.h"
#include "../../Physics/PhysicsComponent/PhysicsComponentCharacter.h"
#include "../../Logic/LogicComponent/LogicComponentEnemy.h"
#include "../../Graphics/TrajectoryManager/TrajectoryComponent.h"

namespace OUAN
{
	/// Class modelling a particular enemy type
	class GameObjectBoss : public GameObject, public boost::enable_shared_from_this<GameObjectBoss>
	{
	private:
		/// Visual component
		RenderComponentEntityPtr mRenderComponentEntity;

		/// Positional component
		RenderComponentInitialPtr mRenderComponentInitial;
		RenderComponentPositionalPtr mRenderComponentPositional;
		/// Physics information
		PhysicsComponentCharacterPtr mPhysicsComponentCharacter;
		/// Logic component: it'll represent the 'brains' of the game object
		/// containing information on its current state, its life and health(if applicable),
		/// or the world(s) the object belongs to
		LogicComponentEnemyPtr mLogicComponentEnemy;

		/// TrajectoryComponent
		TrajectoryComponentPtr mTrajectoryComponent;

		/// Audio component
		AudioComponentPtr mAudioComponent;
	public:
		/// Constructor
		/// @param name name of the game object, specific to this class
		/// @param id unique id of the game object
		GameObjectBoss(const std::string& name);

		//Destructor
		~GameObjectBoss();
		/// Set logic component
		void setLogicComponentEnemy(LogicComponentEnemyPtr logicComponentEnemy);
		/// Set audio component
		/// @param pRenderComponentEntity
		AudioComponentPtr getAudioComponent() const;
		void setAudioComponent(AudioComponentPtr audioComponent);
		/// return logic component
		LogicComponentEnemyPtr getLogicComponentEnemy();
		/// Return render component entity 
		/// @return render component entity
		RenderComponentEntityPtr getRenderComponentEntity() const;

		/// Set render component
		/// @param pRenderComponentEntity
		void setRenderComponentEntity(RenderComponentEntityPtr pRenderComponentEntity);

		/// Set trajectory component
		/// @param pTrajectoryComponent
		void setTrajectoryComponent(TrajectoryComponentPtr pTrajectoryComponent);
		TrajectoryComponentPtr getTrajectoryComponent();

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

		/// Set physics component
		void setPhysicsComponentCharacter(PhysicsComponentCharacterPtr pPhysicsComponentCharacter);

		/// Get physics component
		PhysicsComponentCharacterPtr getPhysicsComponentCharacter() const;

		/// Update object
		virtual void update(double elapsedSeconds);

		/// Reset object
		virtual void reset();

		/// React to a world change to the one given as a parameter
		/// @param world world to change to
		void changeToWorld(int newWorld, double perc);
		void changeWorldFinished(int newWorld);
		void changeWorldStarted(int newWorld);


		bool hasPositionalComponent() const;
		RenderComponentPositionalPtr getPositionalComponent() const;

		bool hasPhysicsComponent() const;
		PhysicsComponentPtr getPhysicsComponent() const;
		
		bool hasRenderComponentEntity() const;
		RenderComponentEntityPtr getEntityComponent() const;

		int getTimeStunned() const;

		/// Process collision event
		/// @param gameObject which has collision with
		void processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal);

		/// Process collision event
		/// @param gameObject which has collision with
		void processEnterTrigger(GameObjectPtr pGameObject);

		/// Process collision event
		/// @param gameObject which has collision with
		void processExitTrigger(GameObjectPtr pGameObject);

		
		

		bool hasLogicComponent() const;
		LogicComponentPtr getLogicComponent() const;
	};
	
	/// Information data structure to carry around data between the
	/// level loader and the "GameObjectBoss"
	class TGameObjectBossParameters: public TGameObjectParameters
	{
	public:
		/// Default constructor
		TGameObjectBossParameters();
		/// Default destructor
		~TGameObjectBossParameters();

		///Parameters specific to an Ogre Entity
		TRenderComponentEntityParameters tRenderComponentEntityParameters;

		///Positional parameters
		TRenderComponentPositionalParameters tRenderComponentPositionalParameters;

		///Physics parameters
		TPhysicsComponentCharacterParameters tPhysicsComponentCharacterParameters;

		/// Audio component params
		TAudioComponentMap tAudioComponentParameters;

		///Logic parameters
		TTrajectoryComponentParameters tTrajectoryComponentParameters;

		///Logic parameters
		TLogicComponentEnemyParameters tLogicComponentEnemyParameters;
	};

}
#endif