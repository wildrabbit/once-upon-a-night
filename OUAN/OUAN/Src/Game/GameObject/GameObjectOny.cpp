#include "GameObjectOny.h"
#include "../GameWorldManager.h"
#include "../../Event/Event.h"

using namespace OUAN;

GameObjectOny::GameObjectOny(const std::string& name)
:GameObject(name,GAME_OBJECT_TYPE_ONY)
{
	mHitRecoveryTime=-1;
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

void GameObjectOny::setPhysicsComponentCharacter(PhysicsComponentCharacterPtr pPhysicsComponentCharacter)
{
	mPhysicsComponentCharacter=pPhysicsComponentCharacter;
}

PhysicsComponentCharacterPtr GameObjectOny::getPhysicsComponentCharacter()
{
	return mPhysicsComponentCharacter;
}

void GameObjectOny::update(double elapsedSeconds)
{
	if (mHitRecoveryTime>=0)
	{
		mHitRecoveryTime-=elapsedSeconds*1000;
	}

	GameObject::update(elapsedSeconds);

	mPhysicsComponentCharacter->update(elapsedSeconds);

	if (mPhysicsComponentCharacter->getNxOgreController()->getPosition().y < 
		Application::getInstance()->getPhysicsSubsystem()->mMinAllowedY)
	{
		OnyFallsEventPtr evt(new OnyFallsEvent());
		mGameWorldManager->addEvent(evt);
	}
}

void GameObjectOny::reset()
{
	GameObject::reset();

	changeWorld(DREAMS);

	if (mPhysicsComponentCharacter.get() && mPhysicsComponentCharacter->isInUse())
	{
		mPhysicsComponentCharacter->getNxOgreController()->setPosition(mRenderComponentInitial->getPosition());
		mPhysicsComponentCharacter->getNxOgreController()->setDisplayYaw(mRenderComponentInitial->getOrientation().getYaw().valueRadians());
	}
}

void GameObjectOny::changeWorld(int world)
{
	if (isEnabled())
	{
		if(mPhysicsComponentCharacter.get() && !mPhysicsComponentCharacter->isInUse())
		{
			mPhysicsComponentCharacter->create();
		}

		mWeaponComponent->changeActiveWeapon(world);
		mWeaponComponent->updateWeaponMode();
		mWeaponComponent->switchOff();
	}
}

void GameObjectOny::decreaseHP(int amount)
{
	if (mHitRecoveryTime<=0)
	{
		GameObject::decreaseHP();
		mHitRecoveryTime=HIT_RECOVERY_TIME;
	}
}

void GameObjectOny::setMaxHP()
{
	getLogicComponent()->setHealthPoints(getLogicComponent()->getInitialHealthPoints());
}

void GameObjectOny::increaseWeaponPower(int powerUnits)
{
	if (mWeaponComponent.get())
		mWeaponComponent->increaseWeaponPower(powerUnits);
}

void GameObjectOny::decreaseWeaponPower(int powerUnits)
{
	if (mWeaponComponent.get())
		mWeaponComponent->decreaseWeaponPower(powerUnits);
}

void GameObjectOny::die()
{
	GameOverEventPtr evt=GameOverEventPtr(new GameOverEvent(false));
	mGameWorldManager->addEvent(evt);
}

void GameObjectOny::loseLife()
{
	GameObject::loseLife();
	OnyDiesEventPtr evt=OnyDiesEventPtr(new OnyDiesEvent(getLogicComponent()->getNumLives()));
	mGameWorldManager->addEvent(evt);
}

bool GameObjectOny::hasPositionalComponent() const
{
	return true;
}
void GameObjectOny::useWeapon()
{
	mWeaponComponent->switchOn();
}
void GameObjectOny::stopUsingWeapon()
{
	mWeaponComponent->switchOff();
}

RenderComponentPositionalPtr GameObjectOny::getPositionalComponent() const
{
	return getRenderComponentPositional();
}

void GameObjectOny::setWeaponComponent(WeaponComponentPtr weaponComponent)
{
	mWeaponComponent=weaponComponent;
}

WeaponComponentPtr GameObjectOny::getWeaponComponent() const
{
	return mWeaponComponent;
}

void GameObjectOny::setWeaponMode(TWeaponMode weaponMode)
{
	mWeaponComponent->setActiveWeaponMode(weaponMode);
}

//-------

TGameObjectOnyParameters::TGameObjectOnyParameters() : TGameObjectParameters()
{

}

TGameObjectOnyParameters::~TGameObjectOnyParameters()
{

}
