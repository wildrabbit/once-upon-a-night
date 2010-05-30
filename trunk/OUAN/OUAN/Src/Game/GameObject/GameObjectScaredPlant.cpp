#include "GameObjectScaredPlant.h"
#include "../GameWorldManager.h"
#include "../../Event/Event.h"
#include "../../Logic/LogicSubsystem.h"
#include "../../Graphics/ObjectTextOverlay/ObjectTextDisplay.h"

using namespace OUAN;

GameObjectScaredPlant::GameObjectScaredPlant(const std::string& name)
:GameObject(name,GAME_OBJECT_TYPE_SCAREDPLANT)
{

}

GameObjectScaredPlant::~GameObjectScaredPlant()
{

}

RenderComponentEntityPtr GameObjectScaredPlant::getRenderComponentEntityDreams() const
{
	return mRenderComponentEntityDreams;
}

void GameObjectScaredPlant::setRenderComponentEntityDreams(RenderComponentEntityPtr pRenderComponentEntityDreams)
{
	mRenderComponentEntityDreams=pRenderComponentEntityDreams;
}

void GameObjectScaredPlant::setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional)
{
	mRenderComponentPositional=pRenderComponentPositional;
}

void GameObjectScaredPlant::setRenderComponentInitial(RenderComponentInitialPtr pRenderComponentInitial)
{
	mRenderComponentInitial=pRenderComponentInitial;
}

RenderComponentPositionalPtr GameObjectScaredPlant::getRenderComponentPositional() const
{
	return mRenderComponentPositional;
}

RenderComponentInitialPtr GameObjectScaredPlant::getRenderComponentInitial() const
{
	return mRenderComponentInitial;
}

void GameObjectScaredPlant::setPhysicsComponentSimpleBox(PhysicsComponentSimpleBoxPtr pPhysicsComponentSimpleBox)
{
	mPhysicsComponentSimpleBox=pPhysicsComponentSimpleBox;
}

PhysicsComponentSimpleBoxPtr GameObjectScaredPlant::getPhysicsComponentSimpleBox() const
{
	return mPhysicsComponentSimpleBox;
}

void GameObjectScaredPlant::update(double elapsedSeconds)
{
	GameObject::update(elapsedSeconds);
	if(isEnabled() && mGameWorldManager->getWorld()==DREAMS)
	{
		LogicSubsystemPtr logicSS = mGameWorldManager->getParent()->getLogicSubsystem();
		//mRenderComponentEntityDreams;
		int currentState=mLogicComponent->getState();
		if (currentState==logicSS->getGlobalInt(SCAREDPLANT_STATE_IDLE) && mLogicComponent->isStateChanged())
		{
			//mRenderComponentEntityDreams->changeAnimation(SCAREDPLANT_ANIM_IDLE);
			displayText("I'M NOT AFRAID ANYMORE! ^_^",0.75);
		}
		else if (currentState==logicSS->getGlobalInt(SCAREDPLANT_STATE_ALERT) && mLogicComponent->isStateChanged())
		{
			//mRenderComponentEntityDreams->changeAnimation(SCAREDPLANT_ANIM_ALERT);
			displayText("I'M AFRAID!! >_<");
		}
		else if (currentState==logicSS->getGlobalInt(SCAREDPLANT_STATE_CAUTION))
		{
			if (mLogicComponent->isStateChanged())
			{
				//Notice that the animation will not change from the alert state
				mLogicComponent->setTimeSpent(0);
				std::stringstream msg("");
				msg<<"I'M STILL AFRAID! T_T ("<<std::setprecision(2)<<mLogicComponent->getTimeSpent()<<")";
				displayText(msg.str(),mLogicComponent->getDelay());
			}
			else
			{
				if (mDisplayMsg)
				{
					std::stringstream msg("");
					msg<<"I'M STILL AFRAID! ("<<std::fixed<<std::setprecision(2)<<mLogicComponent->getTimeSpent()<<")";
					mDisplayMsg->setText(msg.str());
				}
			}
		}
		if(mRenderComponentEntityDreams.get())
		{
			mRenderComponentEntityDreams->update(elapsedSeconds);
		}

	}
}

void GameObjectScaredPlant::reset()
{
	GameObject::reset();
	mLogicComponent->setState(mGameWorldManager->getParent()->getLogicSubsystem()->getGlobalInt(SCAREDPLANT_STATE_IDLE));
}

void GameObjectScaredPlant::changeWorldFinished(int newWorld)
{
	if (!isEnabled()) return;

	mRenderComponentEntityDreams->setOriginalMaterials();

	switch(newWorld)
	{
		case DREAMS:
			if (mPhysicsComponentSimpleBox.get() && mPhysicsComponentSimpleBox->isInUse())
			{
				mPhysicsComponentSimpleBox->create();
			}
			mRenderComponentEntityDreams->setVisible(true);
			break;
		case NIGHTMARES:
			if (mPhysicsComponentSimpleBox.get() && !mPhysicsComponentSimpleBox->isInUse())
			{
				mPhysicsComponentSimpleBox->destroy();
			}
			mRenderComponentEntityDreams->setVisible(false);
			break;
		default:break;
	}
}

void GameObjectScaredPlant::changeWorldStarted(int newWorld)
{
	if (!isEnabled()) return;

	mRenderComponentEntityDreams->randomizeChangeWorldMaterials();
	mRenderComponentEntityDreams->setChangeWorldMaterials();
	mRenderComponentEntityDreams->setVisible(true);
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

void GameObjectScaredPlant::changeToWorld(int newWorld, double perc)
{
	if (!isEnabled()) return;

	switch(newWorld)
	{
	case DREAMS:
		mRenderComponentEntityDreams->setChangeWorldFactor(perc);
		break;
	case NIGHTMARES:
		mRenderComponentEntityDreams->setChangeWorldFactor(1.0f-perc);
		break;
	default:
		break;
	}
}

void GameObjectScaredPlant::calculateChangeWorldTotalTime(double changeWorldTotalTime)
{
	mChangeWorldTotalTime=changeWorldTotalTime*0.25f;
}

void GameObjectScaredPlant::calculateChangeWorldDelay(double totalElapsedTime,double totalTime,int newWorld,double delay_factor,double intersection)
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

bool GameObjectScaredPlant::hasPositionalComponent() const
{
	return true;
}

RenderComponentPositionalPtr GameObjectScaredPlant::getPositionalComponent() const
{
	return getRenderComponentPositional();
}

bool GameObjectScaredPlant::hasPhysicsComponent() const
{
	return true;
}
PhysicsComponentPtr GameObjectScaredPlant::getPhysicsComponent() const
{
	return getPhysicsComponentSimpleBox();
}

/// Set logic component
void GameObjectScaredPlant::setLogicComponent(LogicComponentPropPtr logicComponent)
{
	mLogicComponent=logicComponent;
}

/// return logic component
LogicComponentPropPtr GameObjectScaredPlant::getLogicComponent()
{
	return mLogicComponent;
}


void GameObjectScaredPlant::processCollision(GameObjectPtr pGameObject)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processCollision(pGameObject);
	}
}

void GameObjectScaredPlant::processEnterTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processEnterTrigger(pGameObject);
	}
}

void GameObjectScaredPlant::processExitTrigger(GameObjectPtr pGameObject)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->processExitTrigger(pGameObject);
	}
}

void GameObjectScaredPlant::updateLogic(double elapsedSeconds)
{
	if (mLogicComponent.get())
	{
		mLogicComponent->update(elapsedSeconds);
	}
}
bool GameObjectScaredPlant::hasRenderComponentEntity() const
{
	return true;
}
RenderComponentEntityPtr GameObjectScaredPlant::getEntityComponent() const
{
	return  mRenderComponentEntityDreams;
}
//-------------------------------------------------------------------------------------------

TGameObjectScaredPlantParameters::TGameObjectScaredPlantParameters() : TGameObjectParameters()
{

}

TGameObjectScaredPlantParameters::~TGameObjectScaredPlantParameters()
{

}