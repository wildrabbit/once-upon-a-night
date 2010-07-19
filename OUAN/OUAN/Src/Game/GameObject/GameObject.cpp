#include "OUAN_Precompiled.h"

#include "GameObject.h"
#include "../../Application.h"
#include "../../Graphics/RenderComponent/RenderComponentInitial.h"
#include "../../Graphics/RenderComponent/RenderComponentEntity.h"
#include "../../Graphics/RenderComponent/RenderComponentPositional.h"
#include "../../Graphics/ObjectTextOverlay/ObjectTextDisplay.h"
#include "../../Graphics/CameraManager/CameraManager.h"
#include "../../Logic/LogicComponent/WeaponComponent.h"
#include "../../Audio/AudioComponent/AudioComponent.h"
#include "../../Physics/PhysicsComponent/PhysicsComponent.h"
#include "../GameWorldManager.h"
#include "../../Utils/Utils.h"

using namespace OUAN;

GameObjectPtr GameObject::Null;

GameObject::GameObject(const std::string& name,const std::string& type)
{
	mName=name;
	mType=type;
	reset();
	mDisplayMsg=NULL;
	mDisplayLifetime=0;
}

GameObject::~GameObject()
{
	if (mDisplayMsg)
	{
		delete mDisplayMsg;
		mDisplayMsg=NULL;
	}
}

void GameObject::destroy()
{
	
}

void GameObject::reset()
{
	mEnabled=true;
	mNumUpdates=0;
	mChangeWorldDelay=0;
	mChangeWorldElapsedTime=0;
	mIsChangingWorld=false;
}

bool GameObject::isFirstUpdate()
{
	return mNumUpdates <= 1;
}

void GameObject::activateChangeWorldFast()
{
	changeWorldFinished(mGameWorldManager->getWorld());
	mIsChangingWorld=false;
}

void GameObject::activateChangeWorld()
{
	//Logger::getInstance()->log("Activate ChangeWorld" + mName + " to "+Ogre::StringConverter::toString(mGameWorldManager->getWorld()));
	if(mIsChangingWorld)
	{
		mChangeWorldElapsedTime=mChangeWorldTotalTime-mChangeWorldElapsedTime;
	}
	else
	{
		mChangeWorldElapsedTime=0;
		mIsChangingWorld=true;
		changeWorldStarted(mGameWorldManager->getWorld());
	}
}

void GameObject::changeWorldFinished(int newWorld)
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

void GameObject::changeWorldStarted(int newWorld)
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

void GameObject::changeToWorld(int newWorld, double perc)
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


void GameObject::update(double elapsedSeconds)
{
	mNumUpdates++;
	if (mDisplayMsg)
	{
		if (mDisplayLifetime>=0) 
		{
			mDisplayMsg->update();
			mDisplayLifetime-=elapsedSeconds;
		}
		else 
		{
			mDisplayMsg->enable(false);
			delete mDisplayMsg;
			mDisplayMsg=NULL;			
		}
	}

	if(mIsChangingWorld)
	{
		mChangeWorldElapsedTime+=elapsedSeconds;
		if(mChangeWorldElapsedTime>=mChangeWorldTotalTime)
		{
			mIsChangingWorld=false;
			changeToWorld(mGameWorldManager->getWorld(),1.0f);
			changeWorldFinished(mGameWorldManager->getWorld());
			mWorld=mGameWorldManager->getWorld();
		}
		else
		{
			changeToWorld(mGameWorldManager->getWorld(),mChangeWorldElapsedTime/mChangeWorldTotalTime);
		}
	}
}

const std::string& GameObject::getName()
{
	return mName;
}

const std::string& GameObject::getType() const
{
	return mType;
}

void GameObject::disable()
{
	Logger::getInstance()->log("Disabling Game Object '" + mName + "'");
	mEnabled = false;
}

void GameObject::enable()
{
	Logger::getInstance()->log("Enabling Game Object '" + mName + "'");
	mEnabled = true;
}

bool GameObject::isEnabled()
{
	return mEnabled;
}

GameWorldManagerPtr GameObject::getGameWorldManager()
{
	return mGameWorldManager;
}

void GameObject::setGameWorldManager(GameWorldManagerPtr gameWorldManager)
{
	mGameWorldManager=gameWorldManager;
}

void GameObject::processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal)
{

}

void GameObject::processEnterTrigger(GameObjectPtr pGameObject)
{

}

void GameObject::processExitTrigger(GameObjectPtr pGameObject)
{

}

void GameObject::processAnimationEnded(const std::string& animationName)
{

}

void GameObject::updateLogic(double elapsedSeconds)
{

}

bool GameObject::hasPositionalComponent() const
{
	return false;
}

RenderComponentPositionalPtr GameObject::getPositionalComponent() const
{
	return RenderComponentPositionalPtr();
}

bool GameObject::hasPhysicsComponent() const
{
	return false;
}

PhysicsComponentPtr GameObject::getPhysicsComponent() const
{
	return PhysicsComponentPtr();
}

bool GameObject::hasRenderComponentEntity() const
{
	return false;
}

RenderComponentEntityPtr GameObject::getEntityComponent() const
{
	return RenderComponentEntityPtr();
}

double GameObject::computeDistanceTo(GameObjectPtr other) const
{
	if (hasPositionalComponent() && other->hasPositionalComponent())
	{
		return getPositionalComponent()->computeDistanceTo(other->getPositionalComponent());
	}
	return -1;
}

void GameObject::setAttack(const std::string& newAttack)
{

}

void GameObject::beginAttack()
{

}

void GameObject::switchOff()
{

}

std::string GameObject::translateWeaponMode(TWeaponMode weaponmode)
{
	return "";
}

std::string GameObject::getDefaultAttack()
{
	return "";
}

void GameObject::activate()
{

}

bool GameObject::canBeActivated() const
{
	return false;
}

void GameObject::postUpdate()
{

}

bool GameObject::canInitiateAttack()
{
	return true;
}

WeaponComponentPtr GameObject::getParentWeaponComponent() const
{
	return WeaponComponentPtr();
}

bool GameObject::hasParentWeaponComponent() const
{
	return false;
}

void GameObject::displayText(const std::string& msg, const double& displayLifetime)
{
	if (mDisplayMsg) 
	{
		delete mDisplayMsg;
		mDisplayMsg=NULL;
	}	

	if (hasRenderComponentEntity())
	{
		RenderComponentEntityPtr entityComp = getEntityComponent();
		Ogre::Camera* camera = Application::getInstance()->getCameraManager()->getCamera();
		mDisplayMsg = new ObjectTextDisplay(entityComp->getEntity(),camera);
		mDisplayMsg->enable(true);
		mDisplayMsg->setText(msg);
	}

	mDisplayLifetime=displayLifetime;
}

void GameObject::disableDisplayMsg()
{
	if (mDisplayMsg)
	{
		mDisplayMsg->enable(false);
		delete mDisplayMsg;
		mDisplayMsg=NULL;
	}
}

bool GameObject::isMoving()
{
	return false;
}

bool GameObject::isJumping()
{
	return false;
}

bool GameObject::cancelAutoCameraMovement()
{
	return false;
}

void GameObject::setDreamsRender()
{

}

void GameObject::setNightmaresRender()
{

}

void GameObject::setChangeWorldRender()
{

}

void GameObject::setChangeWorldDelay(double delay)
{
	mChangeWorldDelay=delay;
}

double GameObject::getChangeWorldDelay() const
{
	return mChangeWorldDelay;
}

bool GameObject::isChangingWorld() const
{
	return mIsChangingWorld;
}

void GameObject::setChangeWorldElapsedTime(double time)
{
	mChangeWorldElapsedTime=time;
}

double GameObject::getChangeWorldElapsedTime() const
{
	return mChangeWorldElapsedTime;
}

double GameObject::getChangeWorldTotalTime() const
{
	return mChangeWorldTotalTime;
}

int GameObject::getWorld()
{
	return mWorld;
}

void GameObject::setWorld(int world)
{
	mWorld=world;
}

void GameObject::calculateChangeWorldTotalTime(double changeWorldTotalTime)
{
	mChangeWorldTotalTime=changeWorldTotalTime;
}

void GameObject::calculateChangeWorldDelay(double totalElapsedTime,double totalTime,int newWorld,double delay_factor,double intersection)
{
	mChangeWorldDelay=0.0f;
}

void GameObject::playSound(const std::string& soundID)
{
	if (hasAudioComponent() && getAudioComponentInstance().get())
	{
		getAudioComponentInstance()->playSound(soundID);
	}
}

bool GameObject::hasAudioComponent() const
{
	return false;
}

AudioComponentPtr GameObject::getAudioComponentInstance() const
{
	return AudioComponentPtr();
}

void GameObject::updatePhysicsComponents(double elapsedSeconds)
{
	if (hasPhysicsComponent() && getPhysicsComponent().get() && getPhysicsComponent()->isInUse())
	{
		getPhysicsComponent()->update(elapsedSeconds);
	}
}

void GameObject::setMaxRenderRadium(double maxRenderRadium)
{
	mMaxRenderRadium = maxRenderRadium;
}


void GameObject::setMaxUpdateRadium(double maxUpdateRadium)
{
	mMaxUpdateRadium = maxUpdateRadium;
}

bool GameObject::isWorthUpdatingProbability(double point, double lowerPoint, double higherPoint)
{
	if (point <= lowerPoint)
	{
		return true;
	
	} 
	else if (point >= higherPoint)
	{
		return false;
	}
	else
	{
		// Normalising point into [0, 1]
		point -= lowerPoint;
		point /= (higherPoint - lowerPoint); 

		// Inverting probability 
		point = 1 - point; 

		double randomPoint = Utils::Random::getInstance()->getRandomDouble(0, 1);

		return randomPoint < point;
	}
}

bool GameObject::isWorthUpdatingPhysicsComponents()
{
	// Initial conditions
	if (mMaxUpdateRadium < 0 || !getPositionalComponent())
	{
		return true;
	}

	Ogre::Vector3 positionOny = Application::getInstance()->getGameWorldManager()->getGameObjectOnyPosition();

	//if(mName.find("tripollo") != std::string::npos)
	//{
	//	Logger::getInstance()->log("##### " + mName + " #####");
	//	Logger::getInstance()->log("getPosition: " + Ogre::StringConverter::toString(getPositionalComponent()->getPosition()));
	//	Logger::getInstance()->log("positionOny: " + Ogre::StringConverter::toString(positionOny));
	//	Logger::getInstance()->log("DISTANCE: " + Ogre::StringConverter::toString(Ogre::Real(getPositionalComponent()->getPosition().distance(positionOny))));
	//	Logger::getInstance()->log("MAX_UPDATE_RADIO: " + Ogre::StringConverter::toString(Ogre::Real(mMaxUpdateRadium)));
	//	Logger::getInstance()->log("#####");
	//}

	return isWorthUpdatingProbability(
		getPositionalComponent()->getPosition().distance(positionOny),
		mMaxUpdateRadium,
		mGameWorldManager->DEFAULT_MAX_UPDATE_RADIO_UPPER_LIMIT);
}

bool GameObject::isWorthUpdatingLogicComponents()
{
	// Initial conditions
	if (mMaxUpdateRadium < 0 || !getPositionalComponent())
	{
		return true;
	}

	Ogre::Vector3 positionOny = Application::getInstance()->getGameWorldManager()->getGameObjectOnyPosition();

	return isWorthUpdatingProbability(
		getPositionalComponent()->getPosition().distance(positionOny),
		mMaxUpdateRadium,
		mGameWorldManager->DEFAULT_MAX_UPDATE_RADIO_UPPER_LIMIT);
}

bool GameObject::isWorthRendering()
{
	// Initial conditions
	if (mMaxRenderRadium < 0 || !getPositionalComponent())
	{
		return true;
	}

	Ogre::Vector3 positionOny = Application::getInstance()->getGameWorldManager()->getGameObjectOnyPosition();
	return getPositionalComponent()->getPosition().distance(positionOny) < mMaxRenderRadium;	
}

std::string GameObject::getTranslation(const std::string& baseString)
{
	ConfigurationPtr ingameStrings=mGameWorldManager->getParent()->getIngameTextStrings();
	if (ingameStrings.get())
	{
		std::string translatedString="";
		ingameStrings->getOption(baseString,translatedString);
		return translatedString;
	}
	return baseString;
}

void GameObject::setParentNest(const std::string& parentNest)
{
	mParentNest=parentNest;
}

std::string GameObject::getParentNest() const
{
	return mParentNest;
}

void GameObject::setParentNestInstance(GameObjectNestPtr instance)
{
	mParentNestInstance=instance;
}
GameObjectNestPtr GameObject::getParentNestInstance() const
{
	return mParentNestInstance;
}

void GameObject::setSpawnProbability(double spawnProbability)
{
	mSpawnProbability=spawnProbability;
}

double GameObject::getSpawnProbability()
{
	return mSpawnProbability;
}

void GameObject::setVisible(bool visible)
{

}


double GameObject::getMovingSpeed()
{
	return 0;
}
//-------------------------------------------------------

TGameObjectParameters::TGameObjectParameters()
{
	this->name="";
	this->dreamsName="";
	this->nightmaresName="";
	this->mMaxUpdateRadium=-1;
}

TGameObjectParameters::~TGameObjectParameters()
{

}