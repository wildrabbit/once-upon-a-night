#include "GameObjectItemMaxHP.h"
#include "../GameWorldManager.h"

using namespace OUAN;

GameObjectItemMaxHP::GameObjectItemMaxHP(const std::string& name)
:GameObject(name)
{

}

GameObjectItemMaxHP::~GameObjectItemMaxHP()
{

}

RenderComponentEntityPtr GameObjectItemMaxHP::getRenderComponentEntity() const
{
	return mRenderComponentEntity;
}

void GameObjectItemMaxHP::setRenderComponentEntity(RenderComponentEntityPtr pRenderComponentEntity)
{
	mRenderComponentEntity=pRenderComponentEntity;
}

void GameObjectItemMaxHP::setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional)
{
	mRenderComponentPositional=pRenderComponentPositional;
}

RenderComponentPositionalPtr GameObjectItemMaxHP::getRenderComponentPositional() const
{
	return mRenderComponentPositional;
}

void GameObjectItemMaxHP::setPhysicsComponentSimpleCapsule(PhysicsComponentSimpleCapsulePtr pPhysicsComponentSimpleCapsule)
{
	mPhysicsComponentSimpleCapsule=pPhysicsComponentSimpleCapsule;
}

PhysicsComponentSimpleCapsulePtr GameObjectItemMaxHP::getPhysicsComponentSimpleCapsule()
{
	return mPhysicsComponentSimpleCapsule;
}

void GameObjectItemMaxHP::setLogicComponentWorldExistance(LogicComponentWorldExistancePtr pLogicComponentWorldExistance)
{
	mLogicComponentWorldExistance=pLogicComponentWorldExistance;
}

LogicComponentWorldExistancePtr GameObjectItemMaxHP::getLogicComponentWorldExistance()
{
	return mLogicComponentWorldExistance;
}

void GameObjectItemMaxHP::changeWorld(int world)
{
	if(mLogicComponentWorldExistance->getExistsInDreams() && mLogicComponentWorldExistance->getExistsInNightmares())
	{
		if (mPhysicsComponentSimpleCapsule.get() && !mPhysicsComponentSimpleCapsule->isInUse())
		{
			mPhysicsComponentSimpleCapsule->create();
		}
		return;
	}
	else
	{
		switch(world)
		{
		case DREAMS:
			
			if(mLogicComponentWorldExistance->getExistsInDreams())
			{
				mRenderComponentEntity->setVisible(true);
				if (mPhysicsComponentSimpleCapsule.get() && !mPhysicsComponentSimpleCapsule->isInUse())
				{
					mPhysicsComponentSimpleCapsule->create();
				}
			}
			else
			{
				mRenderComponentEntity->setVisible(false);
				if (mPhysicsComponentSimpleCapsule.get() && mPhysicsComponentSimpleCapsule->isInUse())
				{
					mPhysicsComponentSimpleCapsule->destroy();
				}
			}		
			break;
		case NIGHTMARES:
			
			if(mLogicComponentWorldExistance->getExistsInNightmares())
			{
				mRenderComponentEntity->setVisible(true);
				if (mPhysicsComponentSimpleCapsule.get() && !mPhysicsComponentSimpleCapsule->isInUse())
				{
					mPhysicsComponentSimpleCapsule->create();
				}
			}
			else
			{
				mRenderComponentEntity->setVisible(false);
				if (mPhysicsComponentSimpleCapsule.get() && mPhysicsComponentSimpleCapsule->isInUse())
				{
					mPhysicsComponentSimpleCapsule->destroy();
				}
			}
			break;
		default:
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------

void GameObjectItemMaxHP::processChangeWorld(ChangeWorldEventPtr evt)
{
	changeWorld(evt->getNewWorld());
}

void GameObjectItemMaxHP::registerHandlers()
{
	GameObjectItemMaxHPPtr _this =shared_from_this();

	registerEventHandler<GameObjectItemMaxHP,ChangeWorldEvent,EVENT_TYPE_CHANGEWORLD>(_this,&GameObjectItemMaxHP::processChangeWorld,
		mGameWorldManager->getEventManager());
}

void GameObjectItemMaxHP::unregisterHandlers()
{
	GameObjectItemMaxHPPtr _this =shared_from_this();

	unregisterEventHandler<GameObjectItemMaxHP,ChangeWorldEvent,EVENT_TYPE_CHANGEWORLD>(_this,&GameObjectItemMaxHP::processChangeWorld,
		mGameWorldManager->getEventManager());
}

//-------------------------------------------------------------------------------------------

TGameObjectItemMaxHPParameters::TGameObjectItemMaxHPParameters() : TGameObjectParameters()
{

}

TGameObjectItemMaxHPParameters::~TGameObjectItemMaxHPParameters()
{

}