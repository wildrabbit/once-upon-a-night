#include "OUAN_Precompiled.h"

#include "PhysicsComponentCharacter.h"
#include "../../RayCasting/RayCasting.h"
#include "../../Graphics/CameraManager/CameraManager.h"

using namespace OUAN;

PhysicsComponentCharacter::PhysicsComponentCharacter(const std::string& type)
:PhysicsComponent(type)
{
	reset();
}

PhysicsComponentCharacter::~PhysicsComponentCharacter()
{

}

void PhysicsComponentCharacter::reset()
{
	resetJumpingVars();
	initFallingVars();
	resetMovementVars();
	resetCyclicVars();

	setOnSurface(false);
	setMoving(false);
	setWalking(false);
}

void PhysicsComponentCharacter::update(double elapsedSeconds)
{
	if (getParent()->isFirstUpdate())
	{
		//DO NOTHING
	}
	else if(!getParent()->isEnabled())
	{
		resetMovementVars();
	} 
	else if (isCyclicCharacter())
	{
		performCyclicMovement(elapsedSeconds);
	}
	else
	{
		performClassicMovement(elapsedSeconds);
	}
}

void PhysicsComponentCharacter::performCyclicMovement(double elapsedSeconds)
{
	unsigned int collisionFlags = GROUP_COLLIDABLE_MASK;

	setNextMovement(NxOgre::Vec3(0, Application::getInstance()->getPhysicsSubsystem()->mCyclicSpeed * mCyclicDirection, 0));
	scaleNextMovement(elapsedSeconds);

	double cyclicLastPositionY = getNxOgreController()->getPosition().y;
	double sceneNodeLastPositionY = getSceneNode()->getPosition().y;

	getNxOgreController()->move(
		getNextMovement(),
		collisionFlags,
		Application::getInstance()->getPhysicsSubsystem()->mMinDistance,
		collisionFlags);

	getSceneNode()->setPosition(
		getSceneNode()->getPosition().x,
		sceneNodeLastPositionY,
		getSceneNode()->getPosition().z);

	mCyclicDirection = (collisionFlags & NxOgre::Enums::ControllerFlag_Down) ? 1 : mCyclicDirection;
	mCyclicDirection = (collisionFlags & NxOgre::Enums::ControllerFlag_Up) ? -1 : mCyclicDirection;

	setOnSurface((collisionFlags & NxOgre::Enums::ControllerFlag_Down) ? true : false);

	mCyclicOffset += getNxOgreController()->getPosition().y - cyclicLastPositionY;

	if (mCyclicOffset >= Application::getInstance()->getPhysicsSubsystem()->mCyclicMaxOffset)
	{
		mCyclicDirection = -1;
	}
	else if (mCyclicOffset <= -Application::getInstance()->getPhysicsSubsystem()->mCyclicMaxOffset)
	{
		mCyclicDirection = 1;
	}

	setLastMovement(getNextMovement());
	setNextMovement(NxOgre::Vec3::ZERO);
}

void PhysicsComponentCharacter::performClassicMovement(double elapsedSeconds)
{	
	//This se must be called at the beginning of this function, 
	//otherwise isWorthUpdating won't return a reliable result
	logStatus("PERFORMING CLASSIC MOVEMENT - BEGINNING", elapsedSeconds);
	setNextMovement(Application::getInstance()->getCameraManager()->rotateMovementVector(getOuternMovement().as<Ogre::Vector3>(), elapsedSeconds));
	setNextMovement(getNextMovement() * Application::getInstance()->getPhysicsSubsystem()->mOuternMovementFactor);

	if (!isWorthUpdating())
	{
		logStatus("isWorthUpdating() == FALSE", elapsedSeconds);
		resetMovementVars();
	} 
	else
	{
		logStatus("isWorthUpdating() == TRUE", elapsedSeconds);
		unsigned int collisionFlags = GROUP_COLLIDABLE_MASK;

		if (isWalking())
		{
			logStatus("Within isWalking(), beginning", elapsedSeconds);

			mNextMovement.x *= Application::getInstance()->getPhysicsSubsystem()->mWalkSpeedFactor;
			mNextMovement.z *= Application::getInstance()->getPhysicsSubsystem()->mWalkSpeedFactor;

			logStatus("Within isWalking(), end", elapsedSeconds);
		}

		if (isJumping())
		{
			logStatus("Within isJumping(), beginning", elapsedSeconds);

			setJumpingSpeed(mJumpingSpeed);
			mJumpingTime += elapsedSeconds;
			mNextMovement.y += mJumpingSpeed;			

			logStatus("Within isJumping(), end", elapsedSeconds);
		}

		if (!isOnSurface())
		{
			logStatus("Within !isOnSurface(), beginning", elapsedSeconds);

			setFallingSpeed(mFallingSpeed + 
				Application::getInstance()->getPhysicsSubsystem()->mGravity.y * mFallingTime);
			mFallingTime += elapsedSeconds;
			mNextMovement.y += mFallingSpeed;

			logStatus("Within !isOnSurface(), end", elapsedSeconds);
		}

		logStatus("Before setNewYaw()", elapsedSeconds);

		setNewYaw();

		logStatus("Before scaleNextMovement()", elapsedSeconds);

		scaleNextMovement(elapsedSeconds);

		logStatus("Before move()", elapsedSeconds);

		getNxOgreController()->move(
			getNextMovement(),
			collisionFlags,
			Application::getInstance()->getPhysicsSubsystem()->mMinDistance,
			collisionFlags);

		logStatus("Before setOnSurface(), setWalking(), setMoving()", elapsedSeconds);

		setOnSurface((collisionFlags & NxOgre::Enums::ControllerFlag_Down) ? true : false);

		setWalking(false);
		setMoving((getNextMovement().x >= 0.1 && getNextMovement().z >= 0.1));
	}

	logStatus("Before setLastMovement(), setNextMovement(), setOuternMovement()", elapsedSeconds);

	setLastMovement(getNextMovement());
	setNextMovement(NxOgre::Vec3::ZERO);
	setOuternMovement(NxOgre::Vec3::ZERO);

	logStatus("PERFORMING CLASSIC MOVEMENT - END", elapsedSeconds);

	updateSceneNode();
}

void PhysicsComponentCharacter::scaleNextMovement(double elapsedSeconds)
{
	setNextMovement(
		getNextMovement() *
		Application::getInstance()->getPhysicsSubsystem()->mMovementUnitsPerSecond * 
		elapsedSeconds);
}

bool PhysicsComponentCharacter::isWorthUpdating()
{
	return isInUse() && getNextMovement() != NxOgre::Vec3::ZERO;
}

void PhysicsComponentCharacter::walk()
{
	setWalking(true);
}

void PhysicsComponentCharacter::jump()
{
	if(canJump())
	{
		initJumpingVars();
		initFallingVars();
	}
}

bool PhysicsComponentCharacter::canJump()
{
	return !isJumping() && isOnSurface();
}

void PhysicsComponentCharacter::resetJumpingVars()
{
	mJumping = false;
	mJumpingTime = 0;
	mJumpingSpeed = 0;
}

void PhysicsComponentCharacter::resetFallingVars()
{
	mFalling = false;
	mFallingTime = 0;
	mFallingSpeed = 0;
}

void PhysicsComponentCharacter::resetCyclicVars()
{
	mCyclicCharacter = false;
	mCyclicDirection = 1;
	mCyclicOffset = 0;
}

void PhysicsComponentCharacter::resetMovementVars()
{
	setNextMovement(NxOgre::Vec3::ZERO);
	setLastMovement(NxOgre::Vec3::ZERO);
}

void PhysicsComponentCharacter::initJumpingVars()
{
	resetJumpingVars();
	mJumping = true;
	mJumpingSpeed = Application::getInstance()->getPhysicsSubsystem()->mInitialJumpSpeed;
}

void PhysicsComponentCharacter::initFallingVars()
{
	resetFallingVars();
	mFalling = true;
}	

bool PhysicsComponentCharacter::isJumping() const
{
	return mJumping;
}

bool PhysicsComponentCharacter::isFalling() const
{
	return mFalling;
}

bool PhysicsComponentCharacter::isFallingLimit() const
{
	return isFalling() && 
		mFallingSpeed <= -Application::getInstance()->getPhysicsSubsystem()->mFallingSpeedLimit;
}

bool PhysicsComponentCharacter::isWalking() const
{
	return mWalking;
}

bool PhysicsComponentCharacter::isMoving() const
{
	return mMoving;
}

void PhysicsComponentCharacter::setJumpingSpeed(double pJumpingSpeed)
{
	mJumpingSpeed = pJumpingSpeed;
}

void PhysicsComponentCharacter::setFallingSpeed(double pFallingSpeed)
{
	mFallingSpeed = (pFallingSpeed <= -Application::getInstance()->getPhysicsSubsystem()->mFallingSpeedLimit) 
		? -Application::getInstance()->getPhysicsSubsystem()->mFallingSpeedLimit
		: pFallingSpeed;
}

void PhysicsComponentCharacter::setOnSurface(bool pOnSurface)
{
	PhysicsComponent::setOnSurface(pOnSurface);

	if (pOnSurface)
	{
		resetJumpingVars();
		resetFallingVars();
	}
}

void PhysicsComponentCharacter::setMoving(bool pMoving)
{
	mMoving = pMoving;
}

void PhysicsComponentCharacter::setWalking(bool pWalking)
{
	mWalking = pWalking;
}

void PhysicsComponentCharacter::setNewYaw()
{
	double characterYaw = getNxOgreController()->getDisplayYaw();

	if(getNextMovement().z<0 && getNextMovement().x<0)
	{
		characterYaw = Ogre::Math::ATan(getNextMovement().x/getNextMovement().z).valueDegrees();
	}
	else if(getNextMovement().z<0 && getNextMovement().x>0)
	{
		characterYaw = Ogre::Math::ATan(getNextMovement().x/getNextMovement().z).valueDegrees();
	}
	else if(getNextMovement().z>0 && getNextMovement().x>0)
	{
		characterYaw = Ogre::Math::ATan(getNextMovement().x/getNextMovement().z).valueDegrees();
		characterYaw+=180;
	}
	else if(getNextMovement().z>0 && getNextMovement().x<0)
	{
		characterYaw = Ogre::Math::ATan(getNextMovement().x/getNextMovement().z).valueDegrees();
		characterYaw+=180;
	}
	else if(getNextMovement().z!=0)
	{
		if(getNextMovement().z<0)
		{
			characterYaw=0;
		}
		else if(getNextMovement().z>0)
		{
			characterYaw=180;
		}
	}
	else if(getNextMovement().x!=0)
	{
		if(getNextMovement().x<0)
		{
			characterYaw=90;
		}
		else if(getNextMovement().x>0)
		{
			characterYaw=270;
		}
	}

	if(getParent()->getType().compare(GAME_OBJECT_TYPE_ONY)==0 && (getNextMovement().x!=0 || getNextMovement().z!=0))
	{
		characterYaw+=180;
	}

	if(characterYaw>=360)
	{
		characterYaw-=360;
	}
	else if(characterYaw<0)
	{
		characterYaw+=360;
	}

	getNxOgreController()->setDisplayYaw(characterYaw);
}

void PhysicsComponentCharacter::create()
{
	PhysicsComponent::create();
	
	setNxOgreController(
		Application::getInstance()->getPhysicsSubsystem()->getNxOgreControllerManager()->createCapsuleController(
			getNxOgreControllerDescription(), 
			getNxOgreSize(), 
			Application::getInstance()->getPhysicsSubsystem()->getNxOgreScene(), 		
			NxOgre::String(getParent()->getName().c_str()),
			getMass(),
			getSceneNode()->getOrientation().getYaw().valueDegrees(),
			Ogre::Math::Cos(Application::getInstance()->getPhysicsSubsystem()->mSlopeLimit * TO_RADIANS),   
			Application::getInstance()->getPhysicsSubsystem()->mStepOffset,
			Application::getInstance()->getPhysicsSubsystem()->mSkinWidth));
}

void PhysicsComponentCharacter::destroy()
{
	PhysicsComponent::destroy();
	Application::getInstance()->getPhysicsSubsystem()->getNxOgreControllerManager()->destroyController(getNxOgreController());
}

void PhysicsComponentCharacter::setOuternMovement(NxOgre::Vec3 outernMovement)
{
	mOuternMovement = outernMovement;
}

NxOgre::Vec3 PhysicsComponentCharacter::getOuternMovement()
{
	return mOuternMovement;
}

void PhysicsComponentCharacter::setNextMovement(NxOgre::Vec3 nextMovement)
{
	mNextMovement = nextMovement;
}

NxOgre::Vec3 PhysicsComponentCharacter::getNextMovement()
{
	return mNextMovement;
}

void PhysicsComponentCharacter::setLastMovement(NxOgre::Vec3 lastMovement)
{
	mLastMovement = lastMovement;
}

Ogre::Vector3 PhysicsComponentCharacter::getLastMovement()
{
	return mLastMovement.as<Ogre::Vector3>();
}

void PhysicsComponentCharacter::updateSceneNode()
{
	getSceneNode()->setPosition(
		getNxOgreController()->getPosition().as<Ogre::Vector3>() + 
		mOffsetRenderPosition);

	getSceneNode()->setOrientation(
		getNxOgreController()->getOrientation().as<Ogre::Quaternion>());
}

void PhysicsComponentCharacter::setOffsetRenderPosition(Vector3 offsetRenderPosition)
{
	mOffsetRenderPosition = offsetRenderPosition;
}

Vector3 PhysicsComponentCharacter::getOffsetRenderPosition() const
{
	return mOffsetRenderPosition;
}

void PhysicsComponentCharacter::setCyclicCharacter(bool pCyclicCharacter)
{
	mCyclicCharacter = pCyclicCharacter;
}

bool PhysicsComponentCharacter::isCyclicCharacter()
{
	return mCyclicCharacter;
}

void PhysicsComponentCharacter::setNxOgreController(NxOgre::Controller* pNxOgreController)
{
	mNxOgreController = pNxOgreController;
}

NxOgre::Controller* PhysicsComponentCharacter::getNxOgreController()
{
	return mNxOgreController;
}

void PhysicsComponentCharacter::setNxOgreSize(NxOgre::Vec2 pNxOgreSize)
{
	mNxOgreSize = pNxOgreSize;
}

NxOgre::Vec2 PhysicsComponentCharacter::getNxOgreSize()
{
	return mNxOgreSize;
}

void PhysicsComponentCharacter::setNxOgreControllerDescription(NxOgre::ControllerDescription pNxOgreControllerDescription)
{
	mNxOgreControllerDescription = pNxOgreControllerDescription;
}

NxOgre::ControllerDescription PhysicsComponentCharacter::getNxOgreControllerDescription()
{
	return mNxOgreControllerDescription;
}

Ogre::Vector3 PhysicsComponentCharacter::getPosition()
{
	return getNxOgreController()->getPosition().as<Ogre::Vector3>();
}

double PhysicsComponentCharacter::getYaw()
{
	return getNxOgreController()->getDisplayYaw();
}

void PhysicsComponentCharacter::logStatus(Ogre::String label, double elapsedSeconds)
{
	//TODO REMOVE THIS LINE WHEN THIS LOG IS REQUIRED
	return;

	if (getParent()->getType().compare(GAME_OBJECT_TYPE_ONY)==0) 
	{
		//Logger::getInstance()->log("PPC: ## LOG STATUS INIT ## " + label + " ##");
		//
		//Logger::getInstance()->log("PPC: mInUse -> " + Ogre::StringConverter::toString(mInUse));
		//Logger::getInstance()->log("PPC: mOnSurface -> " + Ogre::StringConverter::toString(mOnSurface));

		//Logger::getInstance()->log("PPC: isWalking() -> " + Ogre::StringConverter::toString(isWalking()));
		//Logger::getInstance()->log("PPC: isMoving() -> " + Ogre::StringConverter::toString(isMoving()));
		//Logger::getInstance()->log("PPC: isJumping() -> " + Ogre::StringConverter::toString(isJumping()));
		//Logger::getInstance()->log("PPC: isFalling() -> " + Ogre::StringConverter::toString(isFalling()));
		//Logger::getInstance()->log("PPC: isFallingLimit() -> " + Ogre::StringConverter::toString(isFallingLimit()));

		//Logger::getInstance()->log("PPC: mJumpingTime -> " + Ogre::StringConverter::toString(Ogre::Real(mJumpingTime)));
		//Logger::getInstance()->log("PPC: mJumpingSpeed -> " + Ogre::StringConverter::toString(Ogre::Real(mJumpingSpeed)));
		//Logger::getInstance()->log("PPC: mFallingTime -> " + Ogre::StringConverter::toString(Ogre::Real(mFallingTime)));
		//Logger::getInstance()->log("PPC: mFallingSpeed -> " + Ogre::StringConverter::toString(Ogre::Real(mFallingSpeed)));

		//Logger::getInstance()->log("PPC: mNxOgreControllerPosition -> " + Ogre::StringConverter::toString(Ogre::Real(getNxOgreController()->getPosition().x)) + " " + Ogre::StringConverter::toString(Ogre::Real(getNxOgreController()->getPosition().y)) + " " + Ogre::StringConverter::toString(Ogre::Real(getNxOgreController()->getPosition().z)));
		//Logger::getInstance()->log("PPC: mNxOgreControllerYaw -> " + Ogre::StringConverter::toString(Ogre::Real(getNxOgreController()->getDisplayYaw())));
		//
		//Logger::getInstance()->log("PPC: mOuternMovement -> " + Ogre::StringConverter::toString(Ogre::Real(mOuternMovement.x)) + " " + Ogre::StringConverter::toString(Ogre::Real(mOuternMovement.y)) + " " + Ogre::StringConverter::toString(Ogre::Real(mOuternMovement.z)));
		//Logger::getInstance()->log("PPC: mNextMovement -> " + Ogre::StringConverter::toString(Ogre::Real(mNextMovement.x)) + " " + Ogre::StringConverter::toString(Ogre::Real(mNextMovement.y)) + " " + Ogre::StringConverter::toString(Ogre::Real(mNextMovement.z)));

		//Logger::getInstance()->log("PPC: mLastMovement -> " + Ogre::StringConverter::toString(Ogre::Real(mLastMovement.x)) + " " + Ogre::StringConverter::toString(Ogre::Real(mLastMovement.y)) + " " + Ogre::StringConverter::toString(Ogre::Real(mLastMovement.z)));

		//Logger::getInstance()->log("PPC: elapsedSeconds -> " + Ogre::StringConverter::toString(Ogre::Real(elapsedSeconds)));
		//
		//Logger::getInstance()->log("PPC: ## LOG STATUS END ## " + label + " ##");

	}
}

TPhysicsComponentCharacterParameters::TPhysicsComponentCharacterParameters() : TPhysicsComponentParameters()
{

}

TPhysicsComponentCharacterParameters::~TPhysicsComponentCharacterParameters()
{

}