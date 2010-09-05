#include "OUAN_Precompiled.h"

#include "GameObjectDiamondTree.h"
#include "GameObjectDiamond.h"
#include "../GameWorldManager.h"
#include "../../Logic/LogicSubsystem.h"
#include "../../Audio/AudioComponent/AudioComponent.h"

using namespace OUAN;

GameObjectDiamondTree::GameObjectDiamondTree(const std::string& name)
:GameObject(name,GAME_OBJECT_TYPE_DIAMONDTREE)
{

}

GameObjectDiamondTree::~GameObjectDiamondTree()
{

}

void GameObjectDiamondTree::setRenderComponentEntityDreams(RenderComponentEntityPtr pRenderComponentEntity)
{
	mRenderComponentEntityDreams=pRenderComponentEntity;
}

void GameObjectDiamondTree::setRenderComponentEntityNightmares(RenderComponentEntityPtr pRenderComponentEntity)
{
	mRenderComponentEntityNightmares=pRenderComponentEntity;
}

AudioComponentPtr GameObjectDiamondTree::getAudioComponent() const
{
	return mAudioComponent;
}

void GameObjectDiamondTree::setAudioComponent(AudioComponentPtr audioComponent)
{
	mAudioComponent=audioComponent;
}


RenderComponentEntityPtr GameObjectDiamondTree::getRenderComponentEntityDreams() const
{
	return mRenderComponentEntityDreams;
}

RenderComponentEntityPtr GameObjectDiamondTree::getRenderComponentEntityNightmares() const
{
	return mRenderComponentEntityNightmares;
}

void GameObjectDiamondTree::setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional)
{
	mRenderComponentPositional=pRenderComponentPositional;
}

void GameObjectDiamondTree::setRenderComponentInitial(RenderComponentInitialPtr pRenderComponentInitial)
{
	mRenderComponentInitial=pRenderComponentInitial;
}

RenderComponentPositionalPtr GameObjectDiamondTree::getRenderComponentPositional() const
{
	return mRenderComponentPositional;
}

RenderComponentInitialPtr GameObjectDiamondTree::getRenderComponentInitial() const
{
	return mRenderComponentInitial;
}

void GameObjectDiamondTree::setPhysicsComponentSimpleBox(PhysicsComponentSimpleBoxPtr physicsComponentSimpleBox)
{
	mPhysicsComponentSimpleBox=physicsComponentSimpleBox;
}

PhysicsComponentSimpleBoxPtr GameObjectDiamondTree::getPhysicsComponentSimpleBox() const
{
	return mPhysicsComponentSimpleBox;
}

void GameObjectDiamondTree::changeWorldFinished(int newWorld)
{
	if (!isEnabled()) return;

	switch(newWorld)
	{
		case DREAMS:
			if(mLogicComponent->existsInDreams() && !mLogicComponent->existsInNightmares())
			{
				mRenderComponentEntityDreams->setVisible(true);
				if (mPhysicsComponentSimpleBox.get() && !mPhysicsComponentSimpleBox->isInUse())
				{
					mPhysicsComponentSimpleBox->create();
				}

				std::string nodeName=getRenderComponentPositional()->getSceneNode()->getName();
				
				//mRenderComponentEntityDreams->changeAnimation((nodeName.find("tree8")!=std::string::npos)?TREE8_ANIM_IDLE_UP:TREE_ANIM_IDLE_UP);
			}
			else if(!mLogicComponent->existsInDreams() && mLogicComponent->existsInNightmares())
			{
				mRenderComponentEntityNightmares->setVisible(false);
				if (mPhysicsComponentSimpleBox.get() && mPhysicsComponentSimpleBox->isInUse())
				{
					mPhysicsComponentSimpleBox->destroy();
				}
				std::string nodeName=getRenderComponentPositional()->getSceneNode()->getName();

				//mRenderComponentEntityNightmares->changeAnimation((nodeName.find("tree8")!=std::string::npos)?TREE8_ANIM_IDLE_DOWN:TREE_ANIM_IDLE_DOWN);
			}		
			break;
		case NIGHTMARES:
			if(!mLogicComponent->existsInDreams() && mLogicComponent->existsInNightmares())
			{
				mRenderComponentEntityNightmares->setVisible(true);
				if (mPhysicsComponentSimpleBox.get() && !mPhysicsComponentSimpleBox->isInUse())
				{
					mPhysicsComponentSimpleBox->create();
				}

				std::string nodeName=getRenderComponentPositional()->getSceneNode()->getName();

				//mRenderComponentEntityNightmares->changeAnimation((nodeName.find("tree8")!=std::string::npos)?TREE8_ANIM_IDLE_UP:TREE_ANIM_IDLE_UP);	
			}
			else if(mLogicComponent->existsInDreams() && !mLogicComponent->existsInNightmares())
			{
				mRenderComponentEntityDreams->setVisible(false);
				if (mPhysicsComponentSimpleBox.get() && mPhysicsComponentSimpleBox->isInUse())
				{
					mPhysicsComponentSimpleBox->destroy();
				}
				std::string nodeName=getRenderComponentPositional()->getSceneNode()->getName();

				//mRenderComponentEntityDreams->changeAnimation((nodeName.find("tree8")!=std::string::npos)?TREE8_ANIM_IDLE_DOWN:TREE_ANIM_IDLE_DOWN);	
			}
			break;
		default:
			break;
	}
}

void GameObjectDiamondTree::changeWorldStarted(int newWorld)
{
	if (!isEnabled()) return;

	Logger::getInstance()->log("[changeWorldStarted] CHANGING "+getName());


	if(mLogicComponent->existsInDreams())
	{
		mRenderComponentEntityDreams->setVisible(true);
	}
	if(mLogicComponent->existsInNightmares())
	{
		mRenderComponentEntityNightmares->setVisible(true);
	}

	switch(newWorld)
	{
	case DREAMS:
		if(mLogicComponent->existsInDreams()&& !mLogicComponent->existsInNightmares())
		{
			std::string nodeName=getRenderComponentPositional()->getSceneNode()->getName();

			//mRenderComponentEntityDreams->changeAnimation((nodeName.find("tree8")!=std::string::npos)?TREE8_ANIM_UP:TREE_ANIM_UP);
		}
		else if(!mLogicComponent->existsInDreams()&& mLogicComponent->existsInNightmares())
		{

			std::string nodeName=getRenderComponentPositional()->getSceneNode()->getName();

			//mRenderComponentEntity->changeAnimation((nodeName.find("tree8")!=std::string::npos)?TREE8_ANIM_DOWN:TREE_ANIM_DOWN);
		}
		break;
	case NIGHTMARES:
		if(mLogicComponent->existsInDreams()&& !mLogicComponent->existsInNightmares())
		{
			std::string nodeName=getRenderComponentPositional()->getSceneNode()->getName();

			//mRenderComponentEntity->changeAnimation((nodeName.find("tree8")!=std::string::npos)?TREE8_ANIM_DOWN:TREE_ANIM_DOWN);
		}
		else if(!mLogicComponent->existsInDreams()&& mLogicComponent->existsInNightmares())
		{	
			std::string nodeName=getRenderComponentPositional()->getSceneNode()->getName();
			//mRenderComponentEntity->changeAnimation((nodeName.find("tree8")!=std::string::npos)?TREE8_ANIM_UP:TREE_ANIM_UP);
		}
		break;
	default:
		break;
	}
}

void GameObjectDiamondTree::changeToWorld(int newWorld, double perc)
{
	if (!isEnabled()) return;
	
	//std::string currentAnimName=mRenderComponentEntity->getCurrentAnimationName();
	//float currentAnimLen=mRenderComponentEntity->getCurrentAnimationLength();
	//if(!mRenderComponentEntity->getCurrentAnimation()) return;

	//switch(newWorld)
	//{
	//case DREAMS:
	//	if(mLogicComponent->existsInDreams()&& !mLogicComponent->existsInNightmares())
	//	{
	//		if(currentAnimName.compare(TREE_ANIM_DOWN)==0)
	//		{
	//			mRenderComponentEntity->setAnimationPosition(currentAnimLen*(1-perc));
	//		}
	//		else if(currentAnimName.compare(TREE_ANIM_UP)==0)
	//		{
	//			mRenderComponentEntity->setAnimationPosition(currentAnimLen*(perc));
	//		}
	//	}
	//	else if(!mLogicComponent->existsInDreams()&& mLogicComponent->existsInNightmares())
	//	{
	//		if(currentAnimName.compare(TREE_ANIM_DOWN)==0)
	//		{
	//			mRenderComponentEntity->setAnimationPosition(currentAnimLen*(perc));
	//		}
	//		else if(currentAnimName.compare(TREE_ANIM_UP)==0)
	//		{
	//			mRenderComponentEntity->setAnimationPosition(currentAnimLen*(1-perc));
	//		}
	//	}
	//	break;
	//case NIGHTMARES:
	//	if(mLogicComponent->existsInDreams()&& !mLogicComponent->existsInNightmares())
	//	{
	//		if(currentAnimName.compare(TREE_ANIM_DOWN)==0)
	//		{
	//			mRenderComponentEntity->setAnimationPosition(currentAnimLen*(perc));
	//		}
	//		else if(currentAnimName.compare(TREE_ANIM_UP)==0)
	//		{
	//			mRenderComponentEntity->setAnimationPosition(currentAnimLen*(1-perc));
	//		}
	//	}
	//	else if(!mLogicComponent->existsInDreams()&& mLogicComponent->existsInNightmares())
	//	{
	//		if(currentAnimName.compare(TREE_ANIM_DOWN)==0)
	//		{
	//			mRenderComponentEntity->setAnimationPosition(currentAnimLen*(1-perc));
	//		}
	//		else if(currentAnimName.compare(TREE_ANIM_UP)==0)
	//		{
	//			mRenderComponentEntity->setAnimationPosition(currentAnimLen*(perc));
	//		}
	//	}
	//	break;
	//default:
	//	break;
	//}
}

void GameObjectDiamondTree::calculateChangeWorldTotalTime(double changeWorldTotalTime)
{
	mChangeWorldTotalTime=changeWorldTotalTime*0.25f;
}

void GameObjectDiamondTree::calculateChangeWorldDelay(double totalElapsedTime,double totalTime,int newWorld,double delay_factor,double intersection)
{
	double fraction=0.25f;

	switch(newWorld)
	{
	case DREAMS:
		if(mLogicComponent->existsInDreams())
		{
			mChangeWorldDelay=(fraction+intersection)*totalTime*delay_factor+(2*fraction-intersection)*totalTime;
		}
		else if(mLogicComponent->existsInNightmares())
		{
			mChangeWorldDelay=(fraction+intersection)*totalTime*delay_factor;
		}
		break;
	case NIGHTMARES:
		if(mLogicComponent->existsInDreams())
		{
			mChangeWorldDelay=(fraction+intersection)*totalTime*delay_factor;
		}
		else if(mLogicComponent->existsInNightmares())
		{
			mChangeWorldDelay=(fraction+intersection)*totalTime*delay_factor+(2*fraction-intersection)*totalTime;
		}
		break;
	default:
		break;
	}	
}

void GameObjectDiamondTree::reset()
{
	GameObject::reset();

	mLogicComponent->setState(mGameWorldManager->getParent()->getLogicSubsystem()->getGlobalInt(DT_STATE_IDLE));
	mLogicComponent->setTimeSpent(-1.0);

	if (mLogicComponent->existsInNightmares())
	{
		mRenderComponentEntityNightmares->changeAnimation(DT_ANIM_IDLE);		
		mRenderComponentEntityNightmares->setVisible(mWorld==NIGHTMARES);
	}
	else if (mLogicComponent->existsInDreams())
	{
		mRenderComponentEntityDreams->changeAnimation(DT_ANIM_IDLE);
		mRenderComponentEntityDreams->setVisible(mWorld==DREAMS);
	}
}

bool GameObjectDiamondTree::hasPositionalComponent() const
{
	return true;
}

RenderComponentPositionalPtr GameObjectDiamondTree::getPositionalComponent() const
{
	return getRenderComponentPositional();
}

bool GameObjectDiamondTree::hasPhysicsComponent() const
{
	return true;
}

PhysicsComponentPtr GameObjectDiamondTree::getPhysicsComponent() const
{
	return mPhysicsComponentSimpleBox;
}

/// Set logic component
void GameObjectDiamondTree::setLogicComponent(LogicComponentPropPtr logicComponent)
{
	mLogicComponent=logicComponent;
}

/// return logic component
LogicComponentPropPtr GameObjectDiamondTree::getLogicComponent()
{
	return mLogicComponent;
}

void GameObjectDiamondTree::processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processCollision(pGameObject, pNormal);
	}
}

void GameObjectDiamondTree::processEnterTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processEnterTrigger(pGameObject);
	}
}

void GameObjectDiamondTree::processExitTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processExitTrigger(pGameObject);
	}
}

bool GameObjectDiamondTree::hasRenderComponentEntity() const
{
	return true;
}
RenderComponentEntityPtr GameObjectDiamondTree::getEntityComponent() const
{
	return (mWorld==DREAMS)?mRenderComponentEntityDreams:mRenderComponentEntityNightmares;
}
void GameObjectDiamondTree::processAnimationEnded(const std::string& animationName)
{
	if (animationName.compare(DT_ANIM_HIT)==0)
	{
		mLogicComponent->setHasTakenHit(false);
		mLogicComponent->setReload(true);
	}
}
void GameObjectDiamondTree::update(double elapsedSeconds)
{
	GameObject::update(elapsedSeconds);

	if (isEnabled())
	{	
		LogicSubsystemPtr logicSS = mGameWorldManager->getParent()->getLogicSubsystem();

		RenderComponentEntityPtr entityToUpdate = (mWorld==DREAMS)
			?mRenderComponentEntityDreams
			:mRenderComponentEntityNightmares;

		int currentState=mLogicComponent->getState();
		
		if (currentState==logicSS->getGlobalInt(DT_STATE_IDLE))
		{
			if (entityToUpdate.get() && mLogicComponent->isStateChanged())
			{
				mLogicComponent->setStateChanged(false);
				mLogicComponent->setHasTakenHit(false);
				mLogicComponent->setReload(false);
				entityToUpdate->changeAnimation(DT_ANIM_IDLE);
				mLogicComponent->setTimeSpent(-1.0);
			}
		}
		else if (currentState==logicSS->getGlobalInt(DT_STATE_HIT) && entityToUpdate.get() && mLogicComponent->isStateChanged())
		{	
			entityToUpdate->changeAnimation(DT_ANIM_HIT);			
			if (mLogicComponent->getTimeSpent()<0)
			{
				mLogicComponent->setTimeSpent(0.0);
			}
			mGameWorldManager->increaseOnyDiamonds(1);

			mAudioComponent->playSound("tree");
			//play sound and particles
		}
		else if (currentState==logicSS->getGlobalInt(DT_STATE_MAY_HIT) && entityToUpdate.get() && mLogicComponent->isStateChanged())
		{					
			entityToUpdate->changeAnimation(DT_ANIM_IDLE);
		}
		else if (currentState==logicSS->getGlobalInt(DT_STATE_DEPLETED) &&
			entityToUpdate.get() && mLogicComponent->isStateChanged())
		{
			//TODO: Replace with depletion animation when it is done
			//TODO: Add particles
			entityToUpdate->changeAnimation(DT_ANIM_IDLE);
		}
		//Last, update the entity
		if (entityToUpdate.get())
		{
			entityToUpdate->update(elapsedSeconds*0.1);
		}
	}
}

void GameObjectDiamondTree::updatePhysicsComponents(double elapsedSeconds)
{
	GameObject::updatePhysicsComponents(elapsedSeconds);
	//mPhysicsComponentVolumeBox->update(elapsedSeconds);
}

void GameObjectDiamondTree::setVisible(bool visible)
{	switch(mWorld)
	{
	case DREAMS:
		if(mLogicComponent->existsInDreams())
		{
			mRenderComponentEntityDreams->setVisible(visible);
		}
		break;
	case NIGHTMARES:
		if(mLogicComponent->existsInNightmares())
		{
			mRenderComponentEntityNightmares->setVisible(visible);
		}
		break;
	default:
		break;
	}

}
bool GameObjectDiamondTree::hasLogicComponent() const
{
	return true;
}
LogicComponentPtr GameObjectDiamondTree::getLogicComponentInstance() const
{
	return mLogicComponent;
}
//-------------------------------------------------------------------------------------------

TGameObjectDiamondTreeParameters::TGameObjectDiamondTreeParameters() : TGameObjectParameters()
{

}

TGameObjectDiamondTreeParameters::~TGameObjectDiamondTreeParameters()
{

}