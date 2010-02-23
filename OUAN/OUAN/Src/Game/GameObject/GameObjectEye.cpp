#include "GameObjectEye.h"

using namespace OUAN;

GameObjectEye::GameObjectEye(const std::string& name, const std::string& id)
:GameObject(name,id)
{

}

GameObjectEye::~GameObjectEye()
{

}

RenderComponentEntityPtr GameObjectEye::getRenderComponentEntity() const
{
	return mRenderComponentEntity;
}

void GameObjectEye::setRenderComponentEntity(RenderComponentEntityPtr pRenderComponentEntity)
{
	mRenderComponentEntity=pRenderComponentEntity;
}

void GameObjectEye::setRenderComponentPositional(RenderComponentPositionalPtr pRenderComponentPositional)
{
	mRenderComponentPositional=pRenderComponentPositional;
}

RenderComponentPositionalPtr GameObjectEye::getRenderComponentPositional() const
{
	return mRenderComponentPositional;
}

void GameObjectEye::setPhysicsComponentComplexNonMovable(PhysicsComponentComplexNonMovablePtr pPhysicsComponentComplexNonMovable)
{
	mPhysicsComponentComplexNonMovable=pPhysicsComponentComplexNonMovable;
}

PhysicsComponentComplexNonMovablePtr GameObjectEye::getPhysicsComponentComplexNonMovable()
{
	return mPhysicsComponentComplexNonMovable;
}


TGameObjectEyeParameters::TGameObjectEyeParameters() : TGameObjectParameters()
{
}

TGameObjectEyeParameters::~TGameObjectEyeParameters()
{
}