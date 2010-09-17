#include "OUAN_Precompiled.h"

#include "GameObjectOny.h"
#include "../GameWorldManager.h"
#include "../../Event/Event.h"
#include "../../Audio/AudioComponent/AudioComponent.h"
#include "../../Application.h"
#include "../../Graphics/RenderSubsystem.h"
#include "GameObjectViewport.h"
#include "GameObjectPillow.h"
#include "GameObjectFlashLight.h"
#include "GameObjectPlataform.h"
#include "../../Utils/Utils.h"

using namespace OUAN;

GameObjectOny::GameObjectOny(const std::string& name)
:GameObject(name,GAME_OBJECT_TYPE_ONY)
,mUsingTrajectory(false)
,mIdleTime(-1)
{
	mDreamsWeapon="pillow#0";
	mNightmaresWeapon="flashlight#1";
	mCurrentWeaponMode=WEAPON_MODE_0;
	mOnWater=false;
	mAttackAnimationName = ONY_ANIM_ATTACK03;
}

GameObjectOny::~GameObjectOny()
{

}

void GameObjectOny::setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional)
{
	mRenderComponentPositional=pRenderComponentPositional;
}

void GameObjectOny::setRenderComponentInitial(RenderComponentInitialPtr pRenderComponentInitial)
{
	mRenderComponentInitial=pRenderComponentInitial;
}

RenderComponentPositionalPtr GameObjectOny::getRenderComponentPositional() const
{
	return mRenderComponentPositional;
}

RenderComponentInitialPtr GameObjectOny::getRenderComponentInitial() const
{
	return mRenderComponentInitial;
}

void GameObjectOny::setRenderComponentEntity(RenderComponentEntityPtr pRenderComponentEntity)
{
	mRenderComponentEntity=pRenderComponentEntity;
}

RenderComponentEntityPtr GameObjectOny::getRenderComponentEntity() const
{
	return mRenderComponentEntity;
}

void GameObjectOny::setParticleSystemsComponent(const TParticleSystemsMap& particleSystems)
{
	mParticleSystemsComponent=particleSystems;
}
GameObjectOny::TParticleSystemsMap& GameObjectOny::getParticleSystemsComponent()
{
	return mParticleSystemsComponent;
}
void GameObjectOny::addParticleSystem(OnyParticleSystemID id,RenderComponentParticleSystemPtr particleSystem)
{
	mParticleSystemsComponent[id]=particleSystem;
}
RenderComponentParticleSystemPtr GameObjectOny::getParticleSystem(OnyParticleSystemID id)
{
	if (mParticleSystemsComponent.find(id)!=mParticleSystemsComponent.end())
	{
		return mParticleSystemsComponent[id];
	}
	return RenderComponentParticleSystemPtr();
}

void GameObjectOny::startParticleSystem(OnyParticleSystemID id)
{
	if (mParticleSystemsComponent.find(id)!=mParticleSystemsComponent.end())
	{
		mParticleSystemsComponent[id]->start();
	}
}
void GameObjectOny::stopParticleSystem(OnyParticleSystemID id)
{
	if (mParticleSystemsComponent.find(id)!=mParticleSystemsComponent.end())
	{
		mParticleSystemsComponent[id]->stop();
	}
}

void GameObjectOny::setRenderComponentQuadHalo(RenderComponentQuadHaloPtr pRenderComponentQuadHalo)
{
	mRenderComponentQuadHalo = pRenderComponentQuadHalo;
}

RenderComponentQuadHaloPtr GameObjectOny::getRenderComponentQuadHalo() const
{
	return mRenderComponentQuadHalo;
}

void GameObjectOny::setPhysicsComponentCharacterOny(PhysicsComponentCharacterOnyPtr pPhysicsComponentCharacterOny)
{
	mPhysicsComponentCharacterOny=pPhysicsComponentCharacterOny;
}

AudioComponentPtr GameObjectOny::getAudioComponent() const
{
	return mAudioComponent;
}
void GameObjectOny::setAudioComponent(AudioComponentPtr audioComponent)
{
	mAudioComponent=audioComponent;
}

PhysicsComponentCharacterOnyPtr GameObjectOny::getPhysicsComponentCharacterOny() const
{
	return mPhysicsComponentCharacterOny;
}
void GameObjectOny::setTrajectoryComponent(TrajectoryComponentPtr trajectoryComponent)
{
	mTrajectoryComponent=trajectoryComponent;
}
TrajectoryComponentPtr GameObjectOny::getTrajectoryComponent() const
{
	return mTrajectoryComponent;
}

bool GameObjectOny::isMoving()
{
	return mPhysicsComponentCharacterOny->isMoving();
}

bool GameObjectOny::isJumping()
{
	return mPhysicsComponentCharacterOny->isJumping();
}

bool GameObjectOny::cancelAutoCameraMovement()
{
	if(mWeaponComponent->isActiveWeaponInUse() && getGameWorldManager()->getWorld()==NIGHTMARES)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameObjectOny::update(double elapsedSeconds)
{
	GameObject::update(elapsedSeconds);

	if (isFirstUpdate())
	{
		mLogicComponentOny->setNewState(ONY_STATE_IDLE);
		mIdleTime=0;
	}

	if (mLogicComponentOny->getState()==ONY_STATE_IDLE)
	{
		mIdleTime+=elapsedSeconds;
	}

	//double animationTime=elapsedSeconds;
	//TODO: Move the scale factor to a configurable parameter
	mRenderComponentEntity->update(elapsedSeconds);
	
	if (mRenderComponentEntity->isTintBeingApplied())
	{
		double invulnerabilityTime = POST_HIT_INVULNERABILITY-mLogicComponentOny->getHitRecoveryTime();
		mRenderComponentEntity->setTintFactor(0.5*(1+Ogre::Math::Sin(invulnerabilityTime*10)));
	}

	if (mPhysicsComponentCharacterOny->getNxOgreController()->getPosition().y < 
		Application::getInstance()->getPhysicsSubsystem()->mMinAllowedY)
	{
		if (!mGameWorldManager->isGodMode())
		{
			OnyFallsEventPtr evt(new OnyFallsEvent());
			mGameWorldManager->addEvent(evt);
		}
		//else
		//{
		//	mPhysicsComponentCharacterOny->jump();
		//	int newState = mLogicComponentOny->getNewState();
		//	newState=SET_BIT(newState,ONY_STATE_BIT_FIELD_JUMP);
		//	mLogicComponentOny->setNewState(newState);
		//}
	}

	//QuadHalo god mode effect
	mRenderComponentQuadHalo->setVisible(Application::getInstance()->getGameWorldManager()->isGodMode());
	if (Application::getInstance()->getGameWorldManager()->isGodMode())
	{
		mRenderComponentQuadHalo->update(elapsedSeconds);
	}

	if (mRunParticlesElapsed<mRunParticlesNextInterval)
	{
		mRunParticlesElapsed+=elapsedSeconds;
		if (mRunParticlesElapsed>mRunParticlesNextInterval)
		{
			startRunParticleSystem();
			mRunParticlesElapsed=0.0;
			mRunParticlesNextInterval=Utils::Random::getInstance()->getRandomDouble(mRunParticlesMin,mRunParticlesMax);
		}
	}
	if (mMsgBoxComponent.get() && mMsgBoxComponent->isVisible())
	{
		mMsgBoxComponent->update(elapsedSeconds);
	}
	if (mUsingTrajectory)
	{
		mTrajectoryComponent->update(elapsedSeconds);
		if (mPhysicsComponentCharacterOny->isInUse())
		{
			mPhysicsComponentCharacterOny->setOuternMovement(mTrajectoryComponent->getNextMovementAbsolute());
		}	
	}
}

bool GameObjectOny::isOnPlataform() const
{
	return mIsOnPlataform;
}

void GameObjectOny::setOnPlataform(bool onPlataform,GameObjectPlataformPtr plataform)
{
	mIsOnPlataform=onPlataform;
	mPlataform=plataform;
}

void GameObjectOny::reset()
{
	GameObject::reset();

	mPhysicsComponentCharacterOny->reset();
	mPhysicsComponentCharacterOny->getNxOgreController()->setPosition(mGameWorldManager->getCheckPointPosition());
	//add 180 since it's the back
	mPhysicsComponentCharacterOny->getNxOgreController()->setDisplayYaw(mGameWorldManager->getCheckPointOrientation().getYaw().valueDegrees()+180);
	mRenderComponentPositional->setPosition(mGameWorldManager->getCheckPointPosition());
	mRenderComponentPositional->setOrientation(Ogre::Quaternion::IDENTITY);
	mRenderComponentPositional->getSceneNode()->yaw(
		Ogre::Radian(
		mPhysicsComponentCharacterOny->getNxOgreController()->getOrientation().as<Ogre::Quaternion>().getYaw().valueRadians())
		);

	mLogicComponentOny->initStateHistory();
	mLogicComponentOny->setState(ONY_STATE_IDLE);
	mLogicComponentOny->setNewState(ONY_STATE_IDLE);
	mLogicComponentOny->setHealthPoints(mLogicComponentOny->getInitialHealthPoints());
	mRenderComponentEntity->changeAnimation(ONY_ANIM_IDLE01);
	mRenderComponentQuadHalo->setVisible(false);

	mIsOnPlataform=false;
	mLastFrameIsOnPlataform=false;
}

void GameObjectOny::changeWorldFinished(int newWorld)
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

	mWeaponComponent->changeActiveWeapon((newWorld==DREAMS)?mDreamsWeapon:mNightmaresWeapon);
	mWeaponComponent->updateAttackType();

	mWeaponComponent->switchOff();
}

void GameObjectOny::changeWorldStarted(int newWorld)
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

void GameObjectOny::changeToWorld(int newWorld, double perc)
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

void GameObjectOny::setLogicComponentOny(LogicComponentOnyPtr pLogicComponentOny)
{
	mLogicComponentOny=pLogicComponentOny;
}

LogicComponentOnyPtr GameObjectOny::getLogicComponentOny()
{
	return mLogicComponentOny;
}

void GameObjectOny::increaseWeaponPower(int powerUnits)
{
	if (mWeaponComponent.get())
	{
		mWeaponComponent->increaseWeaponPower(powerUnits);
	}
}

void GameObjectOny::decreaseWeaponPower(int powerUnits)
{
	if (mWeaponComponent.get())
	{
		mWeaponComponent->decreaseWeaponPower(powerUnits);
	}
}

bool GameObjectOny::hasPositionalComponent() const
{
	return true;
}

RenderComponentPositionalPtr GameObjectOny::getPositionalComponent() const
{
	return getRenderComponentPositional();
}

bool GameObjectOny::hasPhysicsComponent() const
{
	return true;
}

PhysicsComponentPtr GameObjectOny::getPhysicsComponent() const
{
	return getPhysicsComponentCharacterOny();
}

void GameObjectOny::setWeaponComponent(WeaponComponentPtr weaponComponent)
{
	mWeaponComponent=weaponComponent;
}

WeaponComponentPtr GameObjectOny::getWeaponComponent() const
{
	return mWeaponComponent;
}

void GameObjectOny::setAttack(TWeaponMode weaponMode)
{
	mCurrentWeaponMode = weaponMode;

	if (mWeaponComponent.get() && mWeaponComponent->getActiveWeapon().get())
	{
		std::string attackType = mWeaponComponent->getActiveWeapon()->translateWeaponMode(weaponMode);
		mWeaponComponent->setAttackType(attackType);
	}
}

void GameObjectOny::setInitialWeaponComponent(int world)
{
	mWeaponComponent->changeActiveWeapon(world==DREAMS?mDreamsWeapon:mNightmaresWeapon);
}

void GameObjectOny::beginAttack()
{
	mWeaponComponent->switchOn();
}

void GameObjectOny::switchOff()
{
	mWeaponComponent->switchOff();
}

void GameObjectOny::processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal)
{
	//Logger::getInstance()->log("[GameObjectOny::processCollision] "+pGameObject->getName());
	if (!Application::getInstance()->isPlayingCutscene() && mLogicComponentOny.get())
	{
		mLogicComponentOny->processCollision(pGameObject, pNormal);
	}

	if(mPhysicsComponentCharacterOny.get() && mPhysicsComponentCharacterOny->isInUse() &&
		 (pGameObject->getType().compare(GAME_OBJECT_TYPE_TERRAINCONVEX)==0
		 || pGameObject->getType().compare(GAME_OBJECT_TYPE_TERRAINTRIANGLE)==0
		 || pGameObject->getType().compare(GAME_OBJECT_TYPE_INVISIBLE_WALL)==0
		 ))
	{
		mPhysicsComponentCharacterOny->calculateSliding(pNormal);
	}
}

void GameObjectOny::processEnterTrigger(GameObjectPtr pGameObject)
{
	if (!Application::getInstance()->isPlayingCutscene() && mLogicComponentOny.get())
	{
		mLogicComponentOny->processEnterTrigger(pGameObject);
	}
}

void GameObjectOny::processExitTrigger(GameObjectPtr pGameObject)
{
	if (!Application::getInstance()->isPlayingCutscene() && mLogicComponentOny.get())
	{
		mLogicComponentOny->processExitTrigger(pGameObject);
	}
}

void GameObjectOny::processAnimationEnded(const std::string& animationName)
{
	if(!Application::getInstance()->isPlayingCutscene() && mLogicComponentOny.get())
	{
		mLogicComponentOny->processAnimationEnded(animationName);
	}
}

bool GameObjectOny::hasRenderComponentEntity() const
{
	return true;
}

RenderComponentEntityPtr GameObjectOny::getEntityComponent() const
{
	return mRenderComponentEntity;
}

bool GameObjectOny::isDying() const
{
	return CHECK_BIT(mLogicComponentOny->getState(),ONY_STATE_BIT_FIELD_DIE);
}

bool GameObjectOny::isHit() const
{
	return CHECK_BIT(mLogicComponentOny->getState(),ONY_STATE_BIT_FIELD_HIT);
}

void GameObjectOny::resetStepSounds()
{
	mPlayedStep00=false;
	mPlayedStep01=true;
}

void GameObjectOny::playStepSounds()
{
	if(!mRenderComponentEntity->getCurrentAnimation()) return;

	std::string currentAnimName=mRenderComponentEntity->getCurrentAnimationName();
	float currentAnimLen=mRenderComponentEntity->getCurrentAnimationLength();
	float currentAnimPosition=mRenderComponentEntity->getAnimationPosition();

	float animPerc=currentAnimPosition/currentAnimLen;

	//STEP SOUNDS
	if(animPerc>=ONY_SOUND_STEP_TIME_00 && animPerc<ONY_SOUND_STEP_TIME_01 && mPlayedStep01 )
	{
		mAudioComponent->playSound(ONY_SOUND_STEP_WOOD_00);
		mPlayedStep00=true;
		mPlayedStep01=false;
	}
	else if(animPerc>=ONY_SOUND_STEP_TIME_01 && mPlayedStep00)
	{
		mAudioComponent->playSound(ONY_SOUND_STEP_WOOD_01);
		mPlayedStep00=false;
		mPlayedStep01=true;
	}
}

void GameObjectOny::startRunParticleSystem()
{
	if (mPhysicsComponentCharacterOny->isOnSurface())
	{
		if (isOnWater())
		{
			Logger::getInstance()->log("Launching ONY_PS_RUN_WATER");
			startParticleSystem(ONY_PS_RUN_WATER);
		}
		else
		{
			//Logger::getInstance()->log("Launching ONY_PS_RUN_SAND");
			startParticleSystem(ONY_PS_RUN_SAND);
		}
	}
}

void GameObjectOny::postUpdate()
{
	if (!Application::getInstance()->isPlayingCutscene())
	{
		GameObject::postUpdate();

		int currentState=mLogicComponentOny->getState();
		int lastState=mLogicComponentOny->getOldState();

		//TO REALLOCATE BETTER
		if (CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_MOVEMENT) && CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_IDLE)){
			startParticleSystem(ONY_PS_RUN_SAND);
		}
		if (mPhysicsComponentCharacterOny->isSliding()){
			startParticleSystem(ONY_PS_RUN_SAND);
		}

		if (currentState==ONY_STATE_IDLE || currentState==(1<<ONY_STATE_BIT_FIELD_INVULNERABLE))
		{
			if (mLogicComponentOny->isStateChanged())
			{
				mRenderComponentEntity->changeAnimation(ONY_ANIM_IDLE01);
				mIdleTime=0.0;

				if (CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_JUMP))
				{
					if (mGameWorldManager->getWorld() == DREAMS)
					{
						startParticleSystem(ONY_PS_LAND_DREAMS);
						startParticleSystem(ONY_PS_LAND_WAVE);
					}
					else if (mGameWorldManager->getWorld() == NIGHTMARES)
					{
						startParticleSystem(ONY_PS_LAND_NIGHTMARES);
						startParticleSystem(ONY_PS_LAND_WAVE);
					}
				}
			}
			else
			{
				if (mIdleTime>=IDLE_SECONDS_TO_NAP)
				{
					if (mRenderComponentEntity->getCurrentAnimationName().compare(ONY_ANIM_IDLE02)!=0)
					{
						mRenderComponentEntity->changeAnimation(ONY_ANIM_IDLE02);
					}
				}
			}
		}		
		else if (CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_DIE) && !CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_DIE) && 
			mRenderComponentEntity->getCurrentAnimationName().compare(ONY_ANIM_DIE)!=0)
		{
			mAudioComponent->playSound(ONY_SOUND_DIE);
			mRenderComponentEntity->changeAnimation(ONY_ANIM_DIE);
		}

		else if (CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_HIT) && 
			!CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_HIT) && 
			mRenderComponentEntity->getCurrentAnimationName().compare(ONY_ANIM_HIT01)!=0)
		{
			mAudioComponent->playSound(ONY_SOUND_HIT_B);
			mRenderComponentEntity->changeAnimation(ONY_ANIM_HIT01);
		}
		else if (CHECK_BIT(currentState, ONY_STATE_BIT_FIELD_ATTACK) && !CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_ATTACK))
		{
			if (mWorld == DREAMS)
			{
				mRenderComponentEntity->changeAnimation(mAttackAnimationName);

				GameObjectPillowPtr pillow = BOOST_PTR_CAST(GameObjectPillow,
					mWeaponComponent->getActiveWeapon());

				if (pillow.get())
				{
					pillow->startAttackParticles();
				}
			} 
			else if (mWorld == NIGHTMARES) 
			{
				//get Camera Direction and change animation accordingly:
				mRenderComponentEntity->changeAnimation(ONY_ANIM_SHOOT_CENTER);

				GameObjectFlashLightPtr flashLight = BOOST_PTR_CAST(GameObjectFlashLight,
					mWeaponComponent->getActiveWeapon());
			}
		}
		else if (CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_JUMP)
			&& !CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_JUMP))
		{
			mRenderComponentEntity->changeAnimation(ONY_ANIM_JUMP);	
			mAudioComponent->playSound(ONY_SOUND_JUMP);
		}
		else if (CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_MOVEMENT))
		{
			if(mLogicComponentOny->isStateChanged())
			{
				if (!CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_MOVEMENT)) //beginning movement
				{
					resetStepSounds();

					if (CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_WALK))
						mRenderComponentEntity->changeAnimation(ONY_ANIM_WALK);
					else
						mRenderComponentEntity->changeAnimation(ONY_ANIM_RUN);
				}
				else //Walk/run toggle
				{
					bool toWalk=CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_WALK) && !CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_WALK);
					bool toRun=CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_WALK) && !CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_WALK);
					bool fromJump = !CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_JUMP) && CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_JUMP);

					if (toWalk || toRun || fromJump)
					{
						if (CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_WALK))
							mRenderComponentEntity->changeAnimation(ONY_ANIM_WALK);
						else
							mRenderComponentEntity->changeAnimation(ONY_ANIM_RUN);
					}
				}
			}
			playStepSounds();
		}
		if (CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_INVULNERABLE) && !CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_INVULNERABLE))
		{
			mRenderComponentEntity->applyTint(Ogre::ColourValue::Red);
		}
		else if (!CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_INVULNERABLE) && CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_INVULNERABLE))
		{		
			mRenderComponentEntity->removeTint();
		}
		if (!CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_FALL)
			&& CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_FALL))
		{
			if (mGameWorldManager->getWorld() == DREAMS)
			{
				startParticleSystem(ONY_PS_LAND_DREAMS);
				startParticleSystem(ONY_PS_LAND_WAVE);
			}
			else if (mGameWorldManager->getWorld() == NIGHTMARES)
			{
				startParticleSystem(ONY_PS_LAND_NIGHTMARES);
				startParticleSystem(ONY_PS_LAND_WAVE);
			}
			mAudioComponent->playSound(ONY_SOUND_JUMP_ONTO_HARD_SURFACE);
		}
		bool wasRunning = CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_MOVEMENT) && !CHECK_BIT(lastState,ONY_STATE_BIT_FIELD_WALK);
		bool hasStoppedRunning = !CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_MOVEMENT) || CHECK_BIT(currentState,ONY_STATE_BIT_FIELD_WALK);
		if (wasRunning && hasStoppedRunning)
		{
			mRunParticlesElapsed=-1.0;
			mRunParticlesNextInterval=-1.0;
		}

		bool hasBegunRunning = !wasRunning && !hasStoppedRunning;		
		if (hasBegunRunning)
		{
			mRunParticlesElapsed=0.0;
			mRunParticlesNextInterval = Utils::Random::getInstance()->getRandomDouble(mRunParticlesMin,mRunParticlesMax);

			startRunParticleSystem();
		}

		//Apply radial blur effect when reached fall speed limit
		mGameWorldManager->getGameObjectViewport()->
		setRadialBlurEffectIfPossible(
		mGameWorldManager->getWorld(),
		mPhysicsComponentCharacterOny->isFallingLimit());
		if(mPhysicsComponentCharacterOny->isFallingLimit() && !mAudioComponent->isPlaying(ONY_SOUND_FALLS))
		{
			mAudioComponent->playSound(ONY_SOUND_FALLS);
		}

		if(mPhysicsComponentCharacterOny->isSliding() && !mAudioComponent->isPlaying(ONY_SOUND_SLIDE))
		{
			mAudioComponent->playSound(ONY_SOUND_SLIDE);
		}
	}

	applyPlataformEffect();

}

void GameObjectOny::applyPlataformEffect()
{
	if(isOnPlataform())
	{
		mPlataform->resetElapsedTimeSinceLastCollision();
		
		//Logger::getInstance()->log("///////////////////////");
		//Logger::getInstance()->log("ONY: " + Ogre::StringConverter::toString(physicsOny->getPosition()));
		//Logger::getInstance()->log("PLATFORM: " + Ogre::StringConverter::toString(physicsPlataform->getPosition()));
		//Logger::getInstance()->log("LAST_POS_DIFF: " + Ogre::StringConverter::toString(physicsPlataform->getLastPositionDifference()));

		//IT IS A MESS BUT IT FINALLY WORKS! LETS HOPE THAT 10 and THAT 100 doesnt affect anything important...

		Ogre::Vector3 posOny = mPhysicsComponentCharacterOny->getPosition();
		Ogre::Vector3 oldPosition = posOny;

		if(!mPhysicsComponentCharacterOny->isJumping())
		{
			posOny.z += mPlataform->getLastPositionDifference().z;
			posOny.x += mPlataform->getLastPositionDifference().x;
			posOny.y += PLATAFORM_COLLISION_HEIGHT_CORRECTION;

			mPhysicsComponentCharacterOny->setPosition(posOny);
			mPhysicsComponentCharacterOny->stabilize(Application::getInstance()->getPhysicsSubsystem()->mStabilizeSeconds);
			mPhysicsComponentCharacterOny->updateSceneNode();

			if(Ogre::Math::Abs(mPhysicsComponentCharacterOny->getPosition().y-oldPosition.y)>PLATAFORM_MAX_DIFFERENCE_FALL)
			{
				mPhysicsComponentCharacterOny->setPosition(Vector3(posOny.x,oldPosition.y,posOny.z));
				mPhysicsComponentCharacterOny->updateSceneNode();
			}
		}

		mPlataform->resetLastPositionDifference();

		if(!mLastFrameIsOnPlataform)
		{
			mPlataform->activateHit();
		}
	}
	else
	{
		if(mLastFrameIsOnPlataform)
		{
			mPlataform->activateHit();
		}
	}

	mLastFrameIsOnPlataform=mIsOnPlataform;
	mIsOnPlataform=false;
}

AudioComponentPtr GameObjectOny::getAudioComponentInstance() const
{
	return mAudioComponent;
}

bool GameObjectOny::hasAudioComponent() const
{
	return true;
}

double GameObjectOny::getMovingSpeed()
{
	return Application::getInstance()->getPhysicsSubsystem()->mMovementUnitsPerSecond;
}

int GameObjectOny::getLogicNewState() const
{
	return mLogicComponentOny->getNewState();
}
void GameObjectOny::setLogicNewState(int newState)
{
	mLogicComponentOny->setNewState(newState);
}
int GameObjectOny::getLogicCurrentState() const
{
	return mLogicComponentOny->getState();
}
bool GameObjectOny::hasLogicComponent() const
{
	return true;
}
LogicComponentPtr GameObjectOny::getLogicComponentInstance() const
{
	return mLogicComponentOny;
}
double GameObjectOny::getRunParticlesElapsed() const
{
	return mRunParticlesElapsed;
}
void GameObjectOny::setRunParticlesElapsed(double runParticlesElapsed)
{
	mRunParticlesElapsed=runParticlesElapsed;
}
double GameObjectOny::getRunParticlesNextInterval() const
{
	return mRunParticlesNextInterval;
}
void GameObjectOny::setRunParticlesNextInterval(double runParticlesNextInterval)
{
	mRunParticlesNextInterval=runParticlesNextInterval;
}

//Random interval Bounds
double GameObjectOny::getRunParticlesMin() const
{
	return mRunParticlesMin;
}
void GameObjectOny::setRunParticlesMin(double runParticlesMin)
{
	mRunParticlesMin=runParticlesMin;
}
double GameObjectOny::getRunParticlesMax() const
{
	return mRunParticlesMax;
}
void GameObjectOny::setRunParticlesMax(double runParticlesMax)
{
	mRunParticlesMax=runParticlesMax;
}
void GameObjectOny::addDiamonds(int amount)
{
	mLogicComponentOny->increaseCollectableItemAmount(GAME_OBJECT_TYPE_DIAMOND,amount);
}

void GameObjectOny::activateTrajectory(const std::string& name,bool looping)
{
	mTrajectoryComponent->activatePathfindingToPredefinedTrajectory(name,getWorld(),false);
	mTrajectoryComponent->setAs2DTrajectory();
	mTrajectoryComponent->setLoopingTrajectory(looping);
	mUsingTrajectory=true;
}
void GameObjectOny::deactivateTrajectory()
{
	mTrajectoryComponent->activateIdle(getName(),getWorld());
	mUsingTrajectory=false;
}
bool GameObjectOny::isTrajectoryActive() const
{
	return mUsingTrajectory;
}
bool GameObjectOny::isTrajectoryFinished() const
{
	if (mUsingTrajectory)
	{
		return mTrajectoryComponent->hasEnded();
	}

	return true;
}

bool GameObjectOny::isOnWater() const
{
	return mOnWater;
}

void GameObjectOny::setOnWater(bool onWater)
{
	mOnWater = onWater;
}

RenderComponentMessageBoxPtr GameObjectOny::getMsgBoxComponent() const
{
	return mMsgBoxComponent;
}
void GameObjectOny::setMsgBoxComponent(RenderComponentMessageBoxPtr msgBoxComponent)
{
	mMsgBoxComponent=msgBoxComponent;
}

void GameObjectOny::changeMessage(const std::string& stringKey, double duration)
{	
	if (mMsgBoxComponent.get())
	{
		mMsgBoxComponent->setMessage(stringKey);
		mMsgBoxComponent->setDuration(duration);
		showMessage();
	}
}
void GameObjectOny::changeMessage(const std::string& stringKey)
{
	if (mMsgBoxComponent.get())
	{
		mMsgBoxComponent->setMessage(stringKey);
		showMessage();
	}
}
void GameObjectOny::showMessage()
{
	if (mMsgBoxComponent.get())
	{
		mMsgBoxComponent->setMessageBoxText();
		mMsgBoxComponent->show();
	}
}
void GameObjectOny::hideMessage()
{
	if (mMsgBoxComponent.get())
	{
		mMsgBoxComponent->hide();
	}
}
bool GameObjectOny::isMessageVisible() const
{
	if (mMsgBoxComponent.get())
	{
		return mMsgBoxComponent->isVisible();
	}

	return false;
}
void GameObjectOny::setCurrentAttackAnimation(const std::string& animationName)
{
	mAttackAnimationName=animationName;
}
//-------

TGameObjectOnyParameters::TGameObjectOnyParameters() : TGameObjectParameters()
{

}

TGameObjectOnyParameters::~TGameObjectOnyParameters()
{

}
