#include "OUAN_Precompiled.h"

#include "GameObjectPortal.h"
#include "../GameWorldManager.h"

#include "../../Logic/LogicSubsystem.h"
#include "../../Audio/AudioComponent/AudioComponent.h"

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

void GameObjectPortal::setRenderComponentGlowDreams(RenderComponentGlowPtr pRenderComponentGlow)
{
	mRenderComponentGlowDreams=pRenderComponentGlow;
}

void GameObjectPortal::setRenderComponentGlowNightmares(RenderComponentGlowPtr pRenderComponentGlow)
{
	mRenderComponentGlowNightmares=pRenderComponentGlow;
}

RenderComponentGlowPtr GameObjectPortal::getRenderComponentGlowDreams() const
{
	return mRenderComponentGlowDreams;
}

RenderComponentGlowPtr GameObjectPortal::getRenderComponentGlowNightmares() const
{
	return mRenderComponentGlowNightmares;
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

void GameObjectPortal::setDreamsRender()
{
	if (!isEnabled()) 
	{
		return;
	}

	mRenderComponentEntityDreams->setVisible(true);
	mRenderComponentEntityDreams->setDreamsMaterials();
	mRenderComponentEntityNightmares->setVisible(false);
	
	//mRenderComponentGlowDreams->setVisible(mRenderComponentGlowNightmares->isVisible());
	mRenderComponentGlowDreams->setVisible(false);
	mRenderComponentGlowNightmares->setVisible(false);
}

void GameObjectPortal::setNightmaresRender()
{
	if (!isEnabled()) 
	{
		return;
	}

	mRenderComponentEntityDreams->setVisible(false);
	mRenderComponentEntityNightmares->setVisible(true);
	mRenderComponentEntityNightmares->setNightmaresMaterials();

	//mRenderComponentGlowNightmares->setVisible(mRenderComponentGlowDreams->isVisible());
	mRenderComponentGlowNightmares->setVisible(false);
	mRenderComponentGlowDreams->setVisible(false);
}

void GameObjectPortal::setChangeWorldRender()
{
	if (!isEnabled())
	{
		return;
	}

	mRenderComponentEntityDreams->setChangeWorldMaterials();
	mRenderComponentEntityNightmares->setChangeWorldMaterials();
}

void GameObjectPortal::changeWorldFinished(int newWorld)
{
	if (!isEnabled()) 
	{
		return;
	}

	switch(newWorld)
	{
		case DREAMS:
			setDreamsRender();
			mRenderComponentEntityDreams->changeAnimation(PORTAL_ANIMATION_IDLE);
			break;
		case NIGHTMARES:
			setNightmaresRender();
			mRenderComponentEntityNightmares->changeAnimation(PORTAL_ANIMATION_IDLE);
			break;
		default:
			break;
	}
}

void GameObjectPortal::changeWorldStarted(int newWorld)
{
	if (!isEnabled())
	{
		return;
	}

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
	if (!isEnabled()) 
	{
		return;
	}

	switch(newWorld)
	{
	case DREAMS:
		break;
	case NIGHTMARES:
		break;
	default:
		break;
	}

	//displayText("CHANGING WORLD "+Ogre::StringConverter::toString(Ogre::Real(perc))+" delay:"+Ogre::StringConverter::toString(Ogre::Real(mChangeWorldDelay)));
}

void GameObjectPortal::reset()
{
	GameObject::reset();

	if (mAudioComponent->isPlaying("portal_close"))
	{
		mAudioComponent->stopSound("portal_close");
	}
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
void GameObjectPortal::setLogicComponentProp(LogicComponentPropPtr logicComponentProp)
{
	mLogicComponent=logicComponentProp;
}

/// return logic component
LogicComponentPropPtr GameObjectPortal::getLogicComponentProp()
{
	return mLogicComponent;
}

void GameObjectPortal::processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processCollision(pGameObject, pNormal);
	}
}

void GameObjectPortal::processEnterTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processEnterTrigger(pGameObject);
	}
}

void GameObjectPortal::processExitTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processExitTrigger(pGameObject);
	}
}

void GameObjectPortal::updateLogic(double elapsedSeconds)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->update(elapsedSeconds);
	}
}

void GameObjectPortal::update(double elapsedSeconds)
{
	GameObject::update(elapsedSeconds);

	if (isEnabled())
	{
		RenderComponentEntityPtr entityToUpdate = (mWorld==DREAMS)
			?mRenderComponentEntityDreams
			:mRenderComponentEntityNightmares;

		//RenderComponentGlowPtr glowToUpdate = (mWorld==DREAMS)
		//	?mRenderComponentGlowDreams
		//	:mRenderComponentGlowNightmares;

		if (isFirstUpdate())
		{
			entityToUpdate->changeAnimation("turn_Clip");
		}

		LogicSubsystemPtr logicSS = mGameWorldManager->getParent()->getLogicSubsystem();	

		int currentState=mLogicComponent->getState();
		if (mPhysicsComponentSimpleBox.get() && entityToUpdate.get())
		{
			if (currentState==logicSS->getGlobalInt(PORTAL_STATE_IDLE))
			{
				if (mLogicComponent->isStateChanged())
				{
					if (mAudioComponent->isPlaying("portal_close"))
					{
						mAudioComponent->stopSound("portal_close");
					}
					//glowToUpdate->setVisible(false);
				}
			}
			else if (currentState==logicSS->getGlobalInt(PORTAL_STATE_ONY_APPROACHING))
			{				
				if (mLogicComponent->isStateChanged())
				{
					mAudioComponent->playSound("portal_close");
					displayText("ONY IS CLOSE");
					//glowToUpdate->setVisible(true);
				}
			}
			else if (currentState==logicSS->getGlobalInt(PORTAL_STATE_HIT))
			{				
				if (mLogicComponent->isStateChanged())
				{
					getGameWorldManager()->changeWorld();				
					mRenderComponentParticleSystemChangeWorld->start();
				}
			}
			else if (currentState==logicSS->getGlobalInt(PORTAL_STATE_CHANGING_WORLD))
			{
				if (mLogicComponent->isStateChanged())
				{

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
	return (mWorld==DREAMS) ? mRenderComponentEntityDreams : mRenderComponentEntityNightmares;
}

bool GameObjectPortal::hasAudioComponent() const
{
	return true;
}

AudioComponentPtr GameObjectPortal::getAudioComponentInstance() const
{
	return mAudioComponent;
}

AudioComponentPtr GameObjectPortal::getAudioComponent() const
{
	return mAudioComponent;
}
void GameObjectPortal::setAudioComponent(AudioComponentPtr audioComponent)
{
	mAudioComponent=audioComponent;
}
void GameObjectPortal::setVisible(bool visible)
{

}
void GameObjectPortal::setCurrentWorldVisibility(bool visibility)
{	
	RenderComponentEntityPtr currentWorldEntity = (mGameWorldManager->getWorld()==DREAMS)
		? mRenderComponentEntityDreams
		: mRenderComponentEntityNightmares;

	currentWorldEntity->setVisible(visibility);
}
//-------------------------------------------------------------------------------------------

TGameObjectPortalParameters::TGameObjectPortalParameters() : TGameObjectParameters()
{

}

TGameObjectPortalParameters::~TGameObjectPortalParameters()
{

}