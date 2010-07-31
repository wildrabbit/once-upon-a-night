#ifndef GameObjectDiamondH_H
#define GameObjectDiamondH_H

#include "GameObject.h"
#include "../../Graphics/RenderComponent/RenderComponentEntity.h"
#include "../../Graphics/RenderComponent/RenderComponentInitial.h"
#include "../../Graphics/RenderComponent/RenderComponentPositional.h"
#include "../../Physics/PhysicsComponent/PhysicsComponentVolumeBox.h"
#include "../../Physics/PhysicsComponent/PhysicsComponentSimpleBox.h"
#include "../../Logic/LogicComponent/LogicComponentItem.h"

namespace OUAN
{
	const std::string DIAMOND_ANIM_IDLE="idle";
	/// Class to hold GameObjectDiamond information
	class GameObjectDiamond : public GameObject, public boost::enable_shared_from_this<GameObjectDiamond>
	{
	private:
		
		/// Visual information
		RenderComponentEntityPtr mRenderComponentEntity;
		/// Position information
		RenderComponentInitialPtr mRenderComponentInitial;
		RenderComponentPositionalPtr mRenderComponentPositional;
		/// Physics information

		// The volume box is the default physics component, that will enable
		// the main character to pick the item up.
		PhysicsComponentVolumeBoxPtr mPhysicsComponentVolumeBox;

		// A simpleBox component will be used with diamonds that are spawned from trees.
		//
		PhysicsComponentSimpleBoxPtr mPhysicsComponentSimpleBox;

		/// Logic component: it'll represent the 'brains' of the game object
		/// containing information on its current state, its life and health(if applicable),
		/// or the world(s) the object belongs to
		LogicComponentItemPtr mLogicComponentItem;

		/// Empty if the diamond isn't part of a diamond tree's set
		std::string mParentDiamondTreeName;
		GameObjectDiamondTreePtr mParentDiamondTree;

	public:
		//Constructor
		GameObjectDiamond(const std::string& name);
		//Destructor
		~GameObjectDiamond();

		/// Set logic component
		void setLogicComponentItem(LogicComponentItemPtr logicComponentItem);

		/// return logic component
		LogicComponentItemPtr getLogicComponentItem();

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

		/// Set physics component
		void setPhysicsComponentVolumeBox(PhysicsComponentVolumeBoxPtr pPhysicsComponentVolumeBox);

		/// Get physics component
		PhysicsComponentVolumeBoxPtr getPhysicsComponentVolumeBox() const;

		/// Set physics component
		void setPhysicsComponentSimpleBox(PhysicsComponentSimpleBoxPtr pPhysicsComponentSimpleBox);

		/// Get physics component
		PhysicsComponentSimpleBoxPtr getPhysicsComponentSimpleBox() const;

		/// Tell if this game object is bound to a diamond tree.
		bool isBoundToTree() const;

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

		/// Reset object
		virtual void reset();

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

		
		
		void update(double elapsedSeconds);

		std::string& getParentDiamondTreeName();
		void setParentDiamondTreeName(const std::string& parentName);
		
		GameObjectDiamondTreePtr getParentDiamondTree() const;
		void setParentDiamondTree(GameObjectDiamondTreePtr parent) ;

		void disable();
		void enable();

		void updatePhysicsComponents(double elapsedSeconds);

		bool hasLogicComponent() const;
		LogicComponentPtr getLogicComponentInstance() const;
	};

	class TGameObjectDiamondParameters: public TGameObjectParameters
	{
	public:
		TGameObjectDiamondParameters();
		~TGameObjectDiamondParameters();
		
		///Parameters specific to an Ogre Entity
		TRenderComponentEntityParameters tRenderComponentEntityParameters;

		///Positional parameters
		TRenderComponentPositionalParameters tRenderComponentPositionalParameters;

		///Physics parameters
		TPhysicsComponentVolumeBoxParameters tPhysicsComponentVolumeBoxParameters;

		TPhysicsComponentSimpleBoxParameters tPhysicsComponentSimpleBoxParameters;

		///Logic parameters
		TLogicComponentItemParameters tLogicComponentItemParameters;

		std::string tParentDiamondTree;

	};
}
#endif