#include "PhysicsComponentCharacter.h"
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
	mNextMovement = NxOgre::Vec3(0,0,0);
	mLastMovement = NxOgre::Vec3::ZERO;

	mJumping = false;
	mFalling = true;
	mSliding = false;
	mOnSurface = false;
	mIsWalking = false;

	mFallTime = 0;

	mJumpSpeed = 0;
	mFallSpeed = 0;
	mSlideDisplacement = NxOgre::Vec3(0,0,0);
	mNormalAngle = 0;

	mDash = 0;

	mLastYaw=0;
	mDashDirection=Ogre::Vector3::ZERO;
	mIsNotApplyingDash=true;

	mDashAccelerationFactor=Application::getInstance()->getPhysicsSubsystem()->mMinDashAccelerationFactor;
	mAccelerationFactor=Application::getInstance()->getPhysicsSubsystem()->mMinAccelerationFactor;
}

void PhysicsComponentCharacter::create()
{
	PhysicsComponent::create();

	setNxOgreController(
		Application::getInstance()->getPhysicsSubsystem()->getNxOgreControllerManager()->createCapsuleController(
			getNxOgreControllerDescription(), 
			getNxOgreSize(), 
			Application::getInstance()->getPhysicsSubsystem()->getNxOgreScene(), 
			Application::getInstance()->getPhysicsSubsystem()->getNxOgreRenderSystem()->
				createPointRenderable(getSceneNode()),			
			NxOgre::String(this->getParent()->getName().c_str()),
			getMass(),
			getSceneNode()->getOrientation().getYaw().valueDegrees()));
}

void PhysicsComponentCharacter::destroy()
{
	PhysicsComponent::destroy();
	Application::getInstance()->getPhysicsSubsystem()->getNxOgreControllerManager()->destroyController(getNxOgreController());
}

void PhysicsComponentCharacter::update(double elapsedSeconds)
{
	//TODO: TESTING PURPOSE, THIS IF BLOCK MUST BE REMOVED
	if(getParent()->getType().compare(GAME_OBJECT_TYPE_ONY)==0)
	{
		return;
	}

	unsigned int collisionFlags = GROUP_COLLIDABLE_MASK;

	if(mIsWalking)
	{
		Vector3 mWalkDirection;
		mWalkDirection=Vector3(mNextMovement.x,0,mNextMovement.z);
		mWalkDirection.normalise();
		
		mNextMovement.x=mWalkDirection.x*Application::getInstance()->getPhysicsSubsystem()->mWalkSpeed;
		mNextMovement.z=mWalkDirection.z*Application::getInstance()->getPhysicsSubsystem()->mWalkSpeed;
	}

	if (mNextMovement!=Vector3::ZERO)
	{
		// Scale next movement using time and speed
		if(getParent()->getType().compare(GAME_OBJECT_TYPE_ONY)==0)
		{
			mNextMovement=mNextMovement+mNextMovement * Application::getInstance()->getPhysicsSubsystem()->mMovementUnitsPerSecond * elapsedSeconds;
		}
		// Apply gravity
		mNextMovement+=Application::getInstance()->getPhysicsSubsystem()->mGravity * elapsedSeconds;
	}

	if (mJumping)
	{
		mNextMovement.y += mJumpSpeed * elapsedSeconds;
		applyGravity(elapsedSeconds);
	} 
	else if (mFalling) 
	{
		if(!getParent()->getGameWorldManager()->getGodMode())
		{
			applyGravity(elapsedSeconds);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// Perform last frame sliding displacement
	if (mSliding && mNormalAngle > Application::getInstance()->getPhysicsSubsystem()->mMinSlidingAngle)
	{
		mNextMovement = mSlideDisplacement * Application::getInstance()->getPhysicsSubsystem()->mMovementUnitsPerSecond * elapsedSeconds;
		resetSliding();
	}

	////////////////////////////////////////////////////////////////////////////
	// Applying global factor to displacement and calculate dash
	if(getParent()->getType().compare(GAME_OBJECT_TYPE_ONY)==0)
	{
		calculateAngleDifference();

		if(mOnSurface)
		{
			calculateAcceleration(elapsedSeconds);
		}

		mNextMovement.x *= mAccelerationFactor;
		mNextMovement.z *= mAccelerationFactor;

		//Ogre::LogManager::getSingleton().logMessage("Application::getInstance()->getPhysicsSubsystem()->mDisplacementScale "+Ogre::StringConverter::toString(Ogre::Real(Application::getInstance()->getPhysicsSubsystem()->mDisplacementScale)));
	}

	if (isInUse())
	{
		setCharactersDisplayYaw();

		if(getParent()->getType().compare(GAME_OBJECT_TYPE_ONY)==0)
		{
			applyDash(elapsedSeconds);
			mNextMovement *= Application::getInstance()->getPhysicsSubsystem()->mDisplacementScale;
		}

		setOnSurface(false);

		getNxOgreController()->move(
			mNextMovement,
			GROUP_COLLIDABLE_MASK,
			Application::getInstance()->getPhysicsSubsystem()->mMinDistance,
			collisionFlags);		

		//Ogre::LogManager::getSingleton().logMessage("* * Moving!");
	}

	setLastMovement(mNextMovement);
	//Set movement to zero for the next frame
	mNextMovement=Vector3::ZERO;
	mIsWalking=false;
}

void PhysicsComponentCharacter::applyGravity(double elapsedSeconds)
{
	setFallSpeed(mFallSpeed + Application::getInstance()->getPhysicsSubsystem()->mGravity.y * mFallTime);
	mFallTime += elapsedSeconds;
	mNextMovement.y += mFallSpeed * elapsedSeconds;
}

bool PhysicsComponentCharacter::isMoving()
{
	return (Ogre::Math::Abs(mNextMovement.x)>0.1 || Ogre::Math::Abs(mNextMovement.z)>0.1);
}	

void PhysicsComponentCharacter::walk()
{
	mIsWalking=true;
}

void PhysicsComponentCharacter::calculateAngleDifference()
{
	double mCurrentYaw;
	if(isMoving())
	{
		mCurrentYaw=getYawFromMovement(mNextMovement);
		mAngleDifference=Ogre::Math::Abs(mCurrentYaw-mLastYaw);
		mLastYaw=mCurrentYaw;
	}
}

void PhysicsComponentCharacter::calculateAcceleration(double elapsedSeconds)
{
	Vector3 mXZMovement;
	Vector3 mLastXZMovement;

	if(isMoving() && mIsNotApplyingDash && !mIsWalking)
	{
		if(mAngleDifference<Application::getInstance()->getPhysicsSubsystem()->mMaxSameDirectionAngle)
		{
			mXZMovement=Vector3(mNextMovement.x,0,mNextMovement.z);
			mLastXZMovement=Vector3(mLastMovement.x,0,mLastMovement.z);

			if(	mXZMovement.length()>=mLastXZMovement.length())
			{
				mAccelerationFactor+=mXZMovement.length()*elapsedSeconds*Application::getInstance()->getPhysicsSubsystem()->mAccelerationIncrement;

				if(mAccelerationFactor>Application::getInstance()->getPhysicsSubsystem()->mMaxAccelerationFactor)
				{
					mAccelerationFactor=Application::getInstance()->getPhysicsSubsystem()->mMaxAccelerationFactor;
				}
			}
			else
			{
				mAccelerationFactor-=(mLastXZMovement-mXZMovement).length()*elapsedSeconds*Application::getInstance()->getPhysicsSubsystem()->mAccelerationIncrement;
			}
		}
	}
	else
	{
		mAccelerationFactor=Application::getInstance()->getPhysicsSubsystem()->mMinAccelerationFactor;
	}
}

Vector3 PhysicsComponentCharacter::getDashMovement(double elapsedSeconds)
{
	double dashPower;
	Vector3 dashMovement;

	dashMovement = Vector3::ZERO;
	dashPower=0;

	if(mDash>0)
	{
		dashPower=elapsedSeconds*mDashAccelerationFactor*Application::getInstance()->getPhysicsSubsystem()->mDashFactor;
		mDashAccelerationFactor-=elapsedSeconds*Application::getInstance()->getPhysicsSubsystem()->mDashAccelerationIncrement;
		
		if(mDashAccelerationFactor<Application::getInstance()->getPhysicsSubsystem()->mMinDashAccelerationFactor)
		{
			mDashAccelerationFactor=Application::getInstance()->getPhysicsSubsystem()->mMinDashAccelerationFactor;
		}

		mDash-=dashPower;
		dashMovement = mDashDirection * dashPower;
	}

	if(mDash<0.1)
	{
		mDashAccelerationFactor=Application::getInstance()->getPhysicsSubsystem()->mMinDashAccelerationFactor;
		mDash=0;
		mIsNotApplyingDash=true;
	}
	else
	{
		mIsNotApplyingDash=false;
	}

	//Ogre::LogManager::getSingleton().logMessage("dashMovement "+Ogre::StringConverter::toString(dashMovement));
	//Ogre::LogManager::getSingleton().logMessage("mNextMovement "+Ogre::StringConverter::toString(Vector3(mNextMovement.x,mNextMovement.y,mNextMovement.z)));

	return dashMovement;
}


void PhysicsComponentCharacter::applyDash(double elapsedSeconds)
{
		//if(mNextMovement!=Vector3::ZERO)
		//{
		if(isMoving())
		{
			if(mAngleDifference<Application::getInstance()->getPhysicsSubsystem()->mMaxSameDirectionAngle && mIsNotApplyingDash)
			{
				//same direction
				mDashAccelerationFactor+=elapsedSeconds*Application::getInstance()->getPhysicsSubsystem()->mDashAccelerationIncrement;

				if(mDashAccelerationFactor>Application::getInstance()->getPhysicsSubsystem()->mMaxDashAccelerationFactor*mAccelerationFactor)
				{
					mDashAccelerationFactor=Application::getInstance()->getPhysicsSubsystem()->mMaxDashAccelerationFactor*mAccelerationFactor;
				}

				mDash+=elapsedSeconds*Application::getInstance()->getPhysicsSubsystem()->mDashFactor;
	
				if(mDash>Application::getInstance()->getPhysicsSubsystem()->mDashMax)
				{
					mDash=Application::getInstance()->getPhysicsSubsystem()->mDashMax;
				}

				mDashDirection=Vector3(mNextMovement.x,0,mNextMovement.z);
				mDashDirection.normalise();

			}
			else
			{
				//different direction
				mNextMovement+=getDashMovement(elapsedSeconds);
			}
		}
		else
		{
			//different direction
			mNextMovement+=getDashMovement(elapsedSeconds);
		}

		//Ogre::LogManager::getSingleton().logMessage("mDash "+Ogre::StringConverter::toString(Ogre::Real(mDash)));
		//Ogre::LogManager::getSingleton().logMessage("mIsNotApplyingDash "+Ogre::StringConverter::toString(mIsNotApplyingDash));
}

void PhysicsComponentCharacter::jump()
{
	if(!mJumping && !mSliding && mOnSurface)
	{
		initJump();
	}
	else if(getParent()->getGameWorldManager()->getGodMode())
	{
		initJump();
	}
}

double PhysicsComponentCharacter::getYawFromMovement(NxOgre::Vec3 movement)
{
	double characterYaw=getNxOgreController()->getDisplayYaw();

	if(mNextMovement.z<0 && mNextMovement.x<0)
	{
		characterYaw = Ogre::Math::ATan(mNextMovement.x/mNextMovement.z).valueDegrees();
	}
	else if(mNextMovement.z<0 && mNextMovement.x>0)
	{
		characterYaw = Ogre::Math::ATan(mNextMovement.x/mNextMovement.z).valueDegrees();
	}
	else if(mNextMovement.z>0 && mNextMovement.x>0)
	{
		characterYaw = Ogre::Math::ATan(mNextMovement.x/mNextMovement.z).valueDegrees();
		characterYaw+=180;
	}
	else if(mNextMovement.z>0 && mNextMovement.x<0)
	{
		characterYaw = Ogre::Math::ATan(mNextMovement.x/mNextMovement.z).valueDegrees();
		characterYaw+=180;
	}
	else if(mNextMovement.z!=0)
	{
		if(mNextMovement.z<0)
		{
			characterYaw=0;
		}
		else if(mNextMovement.z>0)
		{
			characterYaw=180;
		}
	}
	else if(mNextMovement.x!=0)
	{
		if(mNextMovement.x<0)
		{
			characterYaw=90;
		}
		else if(mNextMovement.x>0)
		{
			characterYaw=270;
		}
	}

	if(getParent()->getType().compare(GAME_OBJECT_TYPE_ONY)==0 && (mNextMovement.x!=0 || mNextMovement.z!=0))
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

	return characterYaw;
}

void PhysicsComponentCharacter::setCharactersDisplayYaw()
{
	double characterYaw;
	characterYaw=getYawFromMovement(mNextMovement);
	getNxOgreController()->setDisplayYaw(characterYaw);
}

//void PhysicsComponentCharacter::setQueryFlags(QueryFlags queryFlags)
//{
//	unsigned int i;
//	for(i=0;i<getNxOgreBody()->getNxActor()->getShapes().size();i++)
//	{
//		getNxOgreBody()->getNxActor()->getShapes()[i]->setFlag(queryFlags);
//	}
//}

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

void PhysicsComponentCharacter::resetSliding()
{
	mSlideDisplacement = NxOgre::Vec3(0,0,0);
	mNormalAngle = 0;
	mSliding = false;
}

void PhysicsComponentCharacter::setSlidingValues(NxOgre::Vec3 pNormal, double pNormalAngle)
{
	//Ogre::LogManager::getSingleton().logMessage("* * Setting sliding!");
	if(!getParent()->getGameWorldManager()->getGodMode())
	{
		if (pNormalAngle > Application::getInstance()->getPhysicsSubsystem()->mMinSlidingAngle && 
			pNormalAngle < Application::getInstance()->getPhysicsSubsystem()->mMinSlidingAngleFall)
		{
			mSliding = true;
			mNormalAngle = pNormalAngle;

			mSlideDisplacement.x = pNormal.x;
			mSlideDisplacement.y = -pNormal.y * Application::getInstance()->getPhysicsSubsystem()->mSlidingFactor;
			mSlideDisplacement.z = pNormal.z;
		}

		if(mSlideDisplacement.y>0)
		{
			mSlideDisplacement.y=0;
		}
	}
}

void PhysicsComponentCharacter::initJump()
{
	mJumping = true;
	mJumpSpeed = Application::getInstance()->getPhysicsSubsystem()->mInitialJumpSpeed;

	initFall();
}

void PhysicsComponentCharacter::initFall()
{
	mFalling = true;
	mFallTime = 0;
	mFallSpeed = 0;
}

void PhysicsComponentCharacter::setJumpSpeed(double pJumpSpeed)
{
	mJumpSpeed = pJumpSpeed;

	if(mJumpSpeed>Application::getInstance()->getPhysicsSubsystem()->mMovementLimitUnitsPerSecond)
	{
		mJumpSpeed=Application::getInstance()->getPhysicsSubsystem()->mMovementLimitUnitsPerSecond;
	}
}

void PhysicsComponentCharacter::setFallSpeed(double pFallSpeed)
{
	mFallSpeed = pFallSpeed;

	if(mFallSpeed>Application::getInstance()->getPhysicsSubsystem()->mMovementLimitUnitsPerSecond)
	{
		mFallSpeed=Application::getInstance()->getPhysicsSubsystem()->mMovementLimitUnitsPerSecond;
	}
}

void PhysicsComponentCharacter::setOnSurface(bool pOnSurface)
{
	mOnSurface=pOnSurface;

	if (pOnSurface)
	{
		mJumping = false;
		initFall();
	}
}

void PhysicsComponentCharacter::setNextMovement(NxOgre::Vec3 nextMovement)
{
	mNextMovement=nextMovement;
}

NxOgre::Vec3 PhysicsComponentCharacter::getNextMovement()
{
	return mNextMovement;
}

void PhysicsComponentCharacter::setLastMovement(NxOgre::Vec3 lastMovement)
{
	mLastMovement=lastMovement;
}

NxOgre::Vec3 PhysicsComponentCharacter::getLastMovement()
{
	return mLastMovement;
}

bool PhysicsComponentCharacter::isJumping() const
{
	return mJumping;
}

bool PhysicsComponentCharacter::isFalling() const
{
	return mFalling;
}

bool PhysicsComponentCharacter::isSliding() const
{
	return mSliding;
}

bool PhysicsComponentCharacter::isMoving() const
{	
	return mNextMovement!=NxOgre::Vec3::ZERO;
}

bool PhysicsComponentCharacter::isOnSurface() const
{
	return mOnSurface;
}

TPhysicsComponentCharacterParameters::TPhysicsComponentCharacterParameters() : TPhysicsComponentParameters()
{

}

TPhysicsComponentCharacterParameters::~TPhysicsComponentCharacterParameters()
{

}