#include "OUAN_Precompiled.h"

#include "LogicComponent.h"
#include "../LogicSubsystem.h"
#include "../../Application.h"
#include "../../Game/GameWorldManager.h"
#include "../../Game/GameObject/GameObject.h"

using namespace OUAN;

LogicComponent::LogicComponent(const std::string& type)
:Component(type)
,mTimeSpent(0)
{
	//mPatrolTrajectory.reset();
	mStateChanged=true;
	mOnWater=false;
	mLastFrameState=-1;
}

LogicComponent::~LogicComponent()
{

}

void LogicComponent::processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal)
{

}

void LogicComponent::processEnterTrigger(GameObjectPtr pGameObject)
{

}

void LogicComponent::processExitTrigger(GameObjectPtr pGameObject)
{

}

bool LogicComponent::isOnWater() const
{
	return mOnWater;
}

void LogicComponent::setOnWater(bool onWater)
{
	mOnWater=onWater;
}

void LogicComponent::update(double elapsedTime)
{
	//mStateChanged=false;

	if (!mScriptFunction.empty())
	{
		LogicSubsystemPtr logicSS= mParent->getGameWorldManager()->getParent()->getLogicSubsystem();
		int newState=logicSS->invokeStateFunction(mScriptFunction,mState,this);
		setState(newState);
	}

	//if(mState!=mLastFrameState)
	//{
	//	mStateChanged=true;
	//	mLastFrameState=mState;
	//}

	if (mTimeSpent>=0 && mTimeSpent<MAX_TIME_SPENT)
	{
		mTimeSpent+=elapsedTime;
	}
}

double LogicComponent::getTimeSpent() const
{
	return mTimeSpent;
}
void LogicComponent::setTimeSpent(double timeSpent)
{
	mTimeSpent=timeSpent;
}

void LogicComponent::initStateHistory()
{
	for (int i=0;i<GAMESTATE_HISTORY_SIZE;++i)
	{
		stateHistory[i]=-1;
	}
}

bool LogicComponent::existsInDreams() const
{
	return mExistsInDreams;
}

void LogicComponent::setExistsInDreams(bool existsInDreams)
{
	mExistsInDreams=existsInDreams;
}

bool LogicComponent::existsInNightmares() const
{
	return mExistsInNightmares;
}

void LogicComponent::setExistsInNightmares(bool existsInNightmares)
{
	mExistsInNightmares=existsInNightmares;
}

int LogicComponent::getState() const
{
	return mState;
}

int LogicComponent::getOldState(int index) const
{
	if (index>=0 && index<GAMESTATE_HISTORY_SIZE)
	{
		return stateHistory[index];
	}

	return -1;
}

void LogicComponent::setState(int state)
{
	int oldState=mState;
	mState=state;

	for (int i=GAMESTATE_HISTORY_SIZE-1;i>0;--i)
	{
		stateHistory[i]=stateHistory[i-1];
	}

	stateHistory[0]=oldState;
	setStateChanged(oldState!=mState);
}

std::string LogicComponent::getScriptFilename() const
{
	return mScriptFilename;
}

void LogicComponent::setScriptFilename(const std::string& scriptFilename)
{
	mScriptFilename=scriptFilename;
}

std::string LogicComponent::getScriptFunction() const
{
	return mScriptFunction;
}

void LogicComponent::setScriptFunction(const std::string& scriptFunction)
{
	mScriptFunction=scriptFunction;
}

bool LogicComponent::isStateChanged() const
{
	return mStateChanged;
}

void LogicComponent::setStateChanged(bool stateChanged)
{
	mStateChanged=stateChanged;
}

int LogicComponent::getLastFrameState() const
{
	return mLastFrameState;
}

void LogicComponent::printMessage(const std::string& msg,double time)
{
	//TODO: Replace with dialogue overlays
	std::string translation = getParent()->getTranslation(msg);
	getParent()->displayText(translation,time);
}

bool LogicComponent::isEnabled() const
{
	return mParent->isEnabled();
}

void LogicComponent::setCurrentWorldVisibility(bool visibility)
{
	mParent->setCurrentWorldVisibility(visibility);
}

void LogicComponent::changeAnimation(const std::string& animationName)
{
	mParent->changeAnimation(animationName);
}

bool LogicComponent::isLoopingAnimation(const std::string& animationName) const
{
	return mParent->isLoopingAnimation(animationName);
}

bool LogicComponent::hasFinishedAnimation(const std::string& animationName) const
{
	return mParent->hasFinishedAnimation(animationName);
}

TLogicComponentParameters::TLogicComponentParameters() : TComponentParameters()
{

}

TLogicComponentParameters::~TLogicComponentParameters()
{

}
