#include "GameObjectHeart.h"
#include "../GameWorldManager.h"

using namespace OUAN;

GameObjectHeart::GameObjectHeart(const std::string& name)
:GameObject(name,GAME_OBJECT_TYPE_HEART)
{
	
}

GameObjectHeart::~GameObjectHeart()
{

}

RenderComponentEntityPtr GameObjectHeart::getRenderComponentEntity() const
{
	return mRenderComponentEntity;
}

void GameObjectHeart::setRenderComponentEntity(RenderComponentEntityPtr pRenderComponentEntity)
{
	mRenderComponentEntity=pRenderComponentEntity;
}

void GameObjectHeart::setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional)
{
	mRenderComponentPositional=pRenderComponentPositional;
}

void GameObjectHeart::setRenderComponentInitial(RenderComponentInitialPtr pRenderComponentInitial)
{
	mRenderComponentInitial=pRenderComponentInitial;
}

RenderComponentPositionalPtr GameObjectHeart::getRenderComponentPositional() const
{
	return mRenderComponentPositional;
}

RenderComponentInitialPtr GameObjectHeart::getRenderComponentInitial() const
{
	return mRenderComponentInitial;
}

void GameObjectHeart::setPhysicsComponentVolumeBox(PhysicsComponentVolumeBoxPtr pPhysicsComponentVolumeBox)
{
	mPhysicsComponentVolumeBox=pPhysicsComponentVolumeBox;
}

PhysicsComponentVolumeBoxPtr GameObjectHeart::getPhysicsComponentVolumeBox() const
{
	return mPhysicsComponentVolumeBox;
}

void GameObjectHeart::changeWorldFinished(int newWorld)
{
	if (!isEnabled()) return;

	mRenderComponentEntity->setOriginalMaterials();

	switch(newWorld)
	{
	case DREAMS:

		if(mLogicComponentItem->existsInDreams())
		{
			mRenderComponentEntity->setVisible(true);
			if (mPhysicsComponentVolumeBox.get() && !mPhysicsComponentVolumeBox->isInUse())
			{
				mPhysicsComponentVolumeBox->create();
			}
		}
		else
		{
			mRenderComponentEntity->setVisible(false);
			if (mPhysicsComponentVolumeBox.get() && mPhysicsComponentVolumeBox->isInUse())
			{
				mPhysicsComponentVolumeBox->destroy();
			}
		}		
		break;
	case NIGHTMARES:

		if(mLogicComponentItem->existsInNightmares())
		{
			mRenderComponentEntity->setVisible(true);
			if (mPhysicsComponentVolumeBox.get() && !mPhysicsComponentVolumeBox->isInUse())
			{
				mPhysicsComponentVolumeBox->create();
			}
		}
		else
		{
			mRenderComponentEntity->setVisible(false);
			if (mPhysicsComponentVolumeBox.get() && mPhysicsComponentVolumeBox->isInUse())
			{
				mPhysicsComponentVolumeBox->destroy();
			}
		}
		break;
	default:
		break;
	}
}

void GameObjectHeart::changeWorldStarted(int newWorld)
{
	if (!isEnabled()) return;

	if(mLogicComponentItem->existsInDreams()&& !mLogicComponentItem->existsInNightmares())
	{
		mRenderComponentEntity->setChangeWorldMaterials();
		mRenderComponentEntity->randomizeChangeWorldMaterials();
	}

	if(mLogicComponentItem->existsInNightmares()&& !mLogicComponentItem->existsInNightmares())
	{
		mRenderComponentEntity->setChangeWorldMaterials();
		mRenderComponentEntity->randomizeChangeWorldMaterials();
	}

	switch(newWorld)
	{
	case DREAMS:
		if(mLogicComponentItem->existsInDreams()&& !mLogicComponentItem->existsInNightmares())
		{
			mRenderComponentEntity->setVisible(true);
		}
		break;
	case NIGHTMARES:
		if(!mLogicComponentItem->existsInDreams()&& mLogicComponentItem->existsInNightmares())
		{
			mRenderComponentEntity->setVisible(true);
		}	
		break;
	default:
		break;
	}
}

void GameObjectHeart::changeToWorld(int newWorld, double perc)
{
	if (!isEnabled()) return;

	switch(newWorld)
	{
		case DREAMS:
			if(mLogicComponentItem->existsInDreams()&& !mLogicComponentItem->existsInNightmares())
			{
				mRenderComponentEntity->setChangeWorldFactor(1-perc);
			}
			else if(!mLogicComponentItem->existsInDreams()&& mLogicComponentItem->existsInNightmares())
			{
				mRenderComponentEntity->setChangeWorldFactor(perc);
			}		
			break;
		case NIGHTMARES:
			if(mLogicComponentItem->existsInDreams()&& !mLogicComponentItem->existsInNightmares())
			{
				mRenderComponentEntity->setChangeWorldFactor(perc);
			}
			else if(!mLogicComponentItem->existsInDreams()&& mLogicComponentItem->existsInNightmares())
			{
				mRenderComponentEntity->setChangeWorldFactor(1-perc);
			}		
			break;
		default:
			break;
	}
}

void GameObjectHeart::calculateChangeWorldTotalTime(double changeWorldTotalTime)
{
	mChangeWorldTotalTime=changeWorldTotalTime*0.25f;
}

void GameObjectHeart::calculateChangeWorldDelay(double totalElapsedTime,double totalTime,int newWorld,double delay_factor,double intersection)
{
	double fraction=0.25f;

	switch(newWorld)
	{
	case DREAMS:
		if(mLogicComponentItem->existsInDreams())
		{
			mChangeWorldDelay=(fraction+intersection)*totalTime*delay_factor+(2*fraction-intersection)*totalTime;
		}
		else if(mLogicComponentItem->existsInNightmares())
		{
			mChangeWorldDelay=(fraction+intersection)*totalTime*delay_factor;
		}
		break;
	case NIGHTMARES:
		if(mLogicComponentItem->existsInDreams())
		{
			mChangeWorldDelay=(fraction+intersection)*totalTime*delay_factor;
		}
		else if(mLogicComponentItem->existsInNightmares())
		{
			mChangeWorldDelay=(fraction+intersection)*totalTime*delay_factor+(2*fraction-intersection)*totalTime;
		}
		break;
	default:
		break;
	}	
}

/// Set logic component
void GameObjectHeart::setLogicComponentItem(LogicComponentItemPtr logicComponentItem)
{
	mLogicComponentItem=logicComponentItem;
}

/// return logic component
LogicComponentItemPtr GameObjectHeart::getLogicComponentItem()
{
	return mLogicComponentItem;
}

void GameObjectHeart::reset()
{
	GameObject::reset();
	mLogicComponentItem->setState(STATE_ITEM_NOT_TAKEN);
}

bool GameObjectHeart::hasPositionalComponent() const
{
	return true;
}

RenderComponentPositionalPtr GameObjectHeart::getPositionalComponent() const
{
	return getRenderComponentPositional();
}

bool GameObjectHeart::hasPhysicsComponent() const
{
	return true;
}
PhysicsComponentPtr GameObjectHeart::getPhysicsComponent() const
{
	return getPhysicsComponentVolumeBox();
}

void GameObjectHeart::processCollision(GameObjectPtr pGameObject)
{
	if (isEnabled() && mLogicComponentItem.get()&& !mLogicComponentItem->getIsTaken())
	{
		mLogicComponentItem->processCollision(pGameObject);
	}
}

void GameObjectHeart::processEnterTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponentItem.get())
	{
		mLogicComponentItem->processEnterTrigger(pGameObject);
	}
}

void GameObjectHeart::processExitTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponentItem.get())
	{
		mLogicComponentItem->processExitTrigger(pGameObject);
	}
}

void GameObjectHeart::updateLogic(double elapsedSeconds)
{
	if (mLogicComponentItem.get())
	{
		mLogicComponentItem->update(elapsedSeconds);
	}
}
void GameObjectHeart::update(double elapsedSeconds)
{
	GameObject::update(elapsedSeconds);

	if (mLogicComponentItem->isStateChanged())
	{
		if (mLogicComponentItem->getState()==STATE_ITEM_TAKEN)
		{
			mRenderComponentEntity->setVisible(false);
			mPhysicsComponentVolumeBox->destroy();
			mLogicComponentItem->setStateChanged(false);
		}
	}
	if (isEnabled())
	{
		if (isFirstUpdate())
		{
			mRenderComponentEntity->changeAnimation("animation_prop");
		}
		else
		{
			mRenderComponentEntity->update(elapsedSeconds);
		}
	}
}
bool GameObjectHeart::hasRenderComponentEntity() const
{
	return true;
}
RenderComponentEntityPtr GameObjectHeart::getEntityComponent() const
{
	return mRenderComponentEntity;
}
TGameObjectHeartParameters::TGameObjectHeartParameters() : TGameObjectParameters()
{

}

TGameObjectHeartParameters::~TGameObjectHeartParameters()
{

}