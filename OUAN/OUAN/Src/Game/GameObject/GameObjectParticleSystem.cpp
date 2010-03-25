#include "GameObjectParticleSystem.h"
#include "../GameWorldManager.h"

using namespace OUAN;

GameObjectParticleSystem::GameObjectParticleSystem(const std::string& name)
:GameObject(name,GAME_OBJECT_TYPE_PARTICLESYSTEM)
{

}

GameObjectParticleSystem::~GameObjectParticleSystem()
{

}

RenderComponentParticleSystemPtr GameObjectParticleSystem::getRenderComponentParticleSystem() const
{
	return mRenderComponentParticleSystem;
}

void GameObjectParticleSystem::setRenderComponentParticleSystem(RenderComponentParticleSystemPtr pRenderComponentParticleSystem)
{
	mRenderComponentParticleSystem=pRenderComponentParticleSystem;
}
void GameObjectParticleSystem::setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional)
{
	mRenderComponentPositional=pRenderComponentPositional;
}

RenderComponentPositionalPtr GameObjectParticleSystem::getRenderComponentPositional() const
{
	return mRenderComponentPositional;
}

void GameObjectParticleSystem::changeWorld(int world)
{
	if (!isEnabled()) return;

	if(mLogicComponent->existsInDreams() && mLogicComponent->existsInNightmares())
	{
		mRenderComponentParticleSystem->setVisible(true);
	}
	else
	{
		switch(world)
		{
		case DREAMS:
			if(mLogicComponent->existsInDreams())
			{
				mRenderComponentParticleSystem->setVisible(true);
			}
			else
			{
				mRenderComponentParticleSystem->setVisible(false);

			}		
			break;
		case NIGHTMARES:
			if(mLogicComponent->existsInNightmares())
			{
				mRenderComponentParticleSystem->setVisible(true);
			}
			else
			{
				mRenderComponentParticleSystem->setVisible(false);
			}
			break;
		default:
			break;
		}
	}
}

TGameObjectParticleSystemParameters::TGameObjectParticleSystemParameters() : TGameObjectParameters()
{

}

TGameObjectParticleSystemParameters::~TGameObjectParticleSystemParameters()
{

}