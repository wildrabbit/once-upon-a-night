#include "GameObjectPortal.h"
#include "../GameWorldManager.h"

#include "../../Logic/LogicSubsystem.h"

using namespace OUAN;

GameObjectPortal::GameObjectPortal(const std::string& name)
:GameObject(name,GAME_OBJECT_TYPE_PORTAL)
{

}

GameObjectPortal::~GameObjectPortal()
{

}

RenderComponentEntityPtr GameObjectPortal::getRenderComponentEntityDreams() const
{
	return mRenderComponentEntityDreams;
}

void GameObjectPortal::setRenderComponentEntityDreams(RenderComponentEntityPtr pRenderComponentEntityDreams)
{
	mRenderComponentEntityDreams=pRenderComponentEntityDreams;
}

RenderComponentEntityPtr GameObjectPortal::getRenderComponentEntityNightmares() const
{
	return mRenderComponentEntityNightmares;
}

void GameObjectPortal::setRenderComponentEntityNightmares(RenderComponentEntityPtr pRenderComponentEntityNightmares)
{
	mRenderComponentEntityNightmares=pRenderComponentEntityNightmares;
}

void GameObjectPortal::setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional)
{
	mRenderComponentPositional=pRenderComponentPositional;
}

void GameObjectPortal::setRenderComponentInitial(RenderComponentInitialPtr pRenderComponentInitial)
{
	mRenderComponentInitial=pRenderComponentInitial;
}

RenderComponentPositionalPtr GameObjectPortal::getRenderComponentPositional() const
{
	return mRenderComponentPositional;
}

RenderComponentInitialPtr GameObjectPortal::getRenderComponentInitial() const
{
	return mRenderComponentInitial;
}

void GameObjectPortal::setRenderComponentParticleSystemChangeWorld(RenderComponentParticleSystemPtr pRenderComponentParticleSystemChangeWorld)
{
	mRenderComponentParticleSystemChangeWorld = pRenderComponentParticleSystemChangeWorld;
}

RenderComponentParticleSystemPtr GameObjectPortal::getRenderComponentParticleSystemChangeWorld() const
{
	return mRenderComponentParticleSystemChangeWorld;
}

void GameObjectPortal::setPhysicsComponentSimpleBox(PhysicsComponentSimpleBoxPtr pPhysicsComponentSimpleBox)
{
	mPhysicsComponentSimpleBox=pPhysicsComponentSimpleBox;
}

PhysicsComponentSimpleBoxPtr GameObjectPortal::getPhysicsComponentSimpleBox() const
{
	return mPhysicsComponentSimpleBox;
}

void GameObjectPortal::changeWorldFinished(int newWorld)
{
	if (!isEnabled()) return;

	switch(newWorld)
	{
		case DREAMS:
			//Ogre::LogManager::getSingleton().logMessage("DREAMS " + getName());

			mRenderComponentEntityDreams->setVisible(true);
			mRenderComponentEntityNightmares->setVisible(false);
			mRenderComponentEntityDreams->changeAnimation("turn_Clip");
			break;
		case NIGHTMARES:
			//Ogre::LogManager::getSingleton().logMessage("NIGHT " + getName());
	
			mRenderComponentEntityDreams->setVisible(false);
			mRenderComponentEntityNightmares->setVisible(true);
			mRenderComponentEntityNightmares->changeAnimation("turn_Clip");
			break;
		default:
			break;
	}
}

void GameObjectPortal::changeWorldStarted(int newWorld)
{
	if (!isEnabled()) return;

	switch(newWorld)
	{
	case DREAMS:
		break;
	case NIGHTMARES:
		break;
	default:
		break;
	}
}

void GameObjectPortal::changeToWorld(int newWorld, double perc)
{
	if (!isEnabled()) return;

	switch(newWorld)
	{
	case DREAMS:
		break;
	case NIGHTMARES:
		break;
	default:
		break;
	}

	displayText("CHANGING WORLD "+Ogre::StringConverter::toString(Ogre::Real(perc))+" delay:"+Ogre::StringConverter::toString(Ogre::Real(mChangeWorldDelay)));
}

void GameObjectPortal::reset()
{
	GameObject::reset();
}

bool GameObjectPortal::hasPositionalComponent() const
{
	return true;
}

RenderComponentPositionalPtr GameObjectPortal::getPositionalComponent() const
{
	return getRenderComponentPositional();
}

bool GameObjectPortal::hasPhysicsComponent() const
{
	return true;
}

PhysicsComponentPtr GameObjectPortal::getPhysicsComponent() const
{
	return getPhysicsComponentSimpleBox();
}

/// Set logic component
void GameObjectPortal::setLogicComponentUsable(LogicComponentUsablePtr logicComponentUsable)
{
	mLogicComponentUsable=logicComponentUsable;
}

/// return logic component
LogicComponentUsablePtr GameObjectPortal::getLogicComponentUsable()
{
	return mLogicComponentUsable;
}

void GameObjectPortal::processCollision(GameObjectPtr pGameObject)
{
	if (mLogicComponentUsable.get())
	{
		mLogicComponentUsable->processCollision(pGameObject);
	}
}

void GameObjectPortal::processEnterTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponentUsable.get())
	{
		mLogicComponentUsable->processEnterTrigger(pGameObject);
	}
}

void GameObjectPortal::processExitTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponentUsable.get())
	{
		mLogicComponentUsable->processExitTrigger(pGameObject);
	}
}

void GameObjectPortal::updateLogic(double elapsedSeconds)
{
	if (mLogicComponentUsable.get())
	{
		mLogicComponentUsable->update(elapsedSeconds);
	}
}

void GameObjectPortal::activate()
{
	if (mLogicComponentUsable.get())
	{
		mLogicComponentUsable->setCanBeActivated(false);
		mLogicComponentUsable->setIsActivated(true);
	}
}
bool GameObjectPortal::canBeActivated() const
{
	int activationState = mGameWorldManager->getParent()->getLogicSubsystem()->getGlobalInt(PORTAL_STATE_ONY_MAY_ACTIVATE);
	return mLogicComponentUsable.get() && mLogicComponentUsable->getState()==activationState && mLogicComponentUsable->canBeActivated();
}

void GameObjectPortal::update(double elapsedSeconds)
{
	GameObject::update(elapsedSeconds);
	if (isEnabled())
	{
		RenderComponentEntityPtr entityToUpdate = (mGameWorldManager->getWorld()==DREAMS)
			?mRenderComponentEntityDreams
			:mRenderComponentEntityNightmares;

		if (isFirstUpdate())
		{
			entityToUpdate->changeAnimation("turn_Clip");
		}

		LogicSubsystemPtr logicSS = mGameWorldManager->getParent()->getLogicSubsystem();	

		int currentState=mLogicComponentUsable->getState();
		if (mPhysicsComponentSimpleBox.get() && entityToUpdate.get())
		{
			if (currentState==logicSS->getGlobalInt(PORTAL_STATE_IDLE))
			{
				if (mLogicComponentUsable->isStateChanged())
				{
					mLogicComponentUsable->setCanBeActivated(false);
					//particleSystem->hide();
					//entityToUpdate()->setAnimationState(PORTAL_ANIMATION_IDLE);
					//overlay->hide();
				}
			}
			else if (currentState==logicSS->getGlobalInt(PORTAL_STATE_ONY_APPROACHING))
			{				
				if (mLogicComponentUsable->isStateChanged())
				{
					mLogicComponentUsable->setCanBeActivated(false);
					displayText("ONY IS CLOSE");
					//particleSystem->show(approaching);
					//overlay->hide();
					//audio->playSound("sparks")
				}
			}
			else if (currentState==logicSS->getGlobalInt(PORTAL_STATE_ONY_MAY_ACTIVATE))
			{
				if (mLogicComponentUsable->isStateChanged())
				{
					displayText("PRESS ACTION TO CHANGE WORLD");
					mLogicComponentUsable->setCanBeActivated(true);
					//overlay->show(getTexture(ActionButton)/"Press action button");
					//particleSystem->show(may_activate)
				}
			}
			else if (currentState==logicSS->getGlobalInt(PORTAL_STATE_CHANGING_WORLD))
			{
				if (mLogicComponentUsable->isStateChanged())
				{
					getGameWorldManager()->changeWorld();				
					//NOTE: Maybe this flag could be reset after the changeWorld animation has ended.
					mLogicComponentUsable->setIsActivated(false);
					mRenderComponentParticleSystemChangeWorld->start();
				}
			}
			entityToUpdate->update(elapsedSeconds);
			if (mPhysicsComponentSimpleBox->isInUse())
			{
				mPhysicsComponentSimpleBox->update(elapsedSeconds);
			}
		}
	}			
}

bool GameObjectPortal::hasRenderComponentEntity() const
{
	return true;
}

RenderComponentEntityPtr GameObjectPortal::getEntityComponent() const
{
	return (mGameWorldManager->getWorld()==DREAMS)?mRenderComponentEntityDreams:mRenderComponentEntityNightmares;
}
//-------------------------------------------------------------------------------------------

TGameObjectPortalParameters::TGameObjectPortalParameters() : TGameObjectParameters()
{

}

TGameObjectPortalParameters::~TGameObjectPortalParameters()
{

}