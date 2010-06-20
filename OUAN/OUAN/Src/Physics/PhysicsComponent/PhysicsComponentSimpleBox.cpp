#include "OUAN_Precompiled.h"

#include "PhysicsComponentSimpleBox.h"
using namespace OUAN;

PhysicsComponentSimpleBox::PhysicsComponentSimpleBox(const std::string& type)
:PhysicsComponentSimple(type)
{

}

PhysicsComponentSimpleBox::~PhysicsComponentSimpleBox()
{

}

void PhysicsComponentSimpleBox::create()
{
	PhysicsComponentSimple::create();

	NxOgre::String name=NxOgre::String(this->getParent()->getName().c_str());

	NxOgre::Box* pBox = 
		new NxOgre::Box(	getNxOgreSize().x,
							getNxOgreSize().y,
							getNxOgreSize().z);

	pBox->setName(name);

	if (getMass() > 0)
	{
		setNxOgreKinematicBody(NULL);

		NxOgre::RigidBodyDescription pDesc = NxOgre::RigidBodyDescription();
		pDesc.mMass = getMass();
		pDesc.mName = name;

		setNxOgreBody(
			Application::getInstance()->getPhysicsSubsystem()->getNxOgreRenderSystem()->createBody(
				pBox,
				getSceneNode()->getPosition(),
				getSceneNode(),
				pDesc));

		getNxOgreBody()->setGlobalOrientationQuat(NxOgre::Quat(getSceneNode()->getOrientation()));

		//Logger::getInstance()->log(getParent()->getName() + " OGRE3DBODY: " + Ogre::StringConverter::toString(
		//	getNxOgreBody()->getContactReportFlags()));
	}
	else
	{
		pBox->setGroup(GROUP_COLLIDABLE_NON_PUSHABLE);
		setStatic(true);
		setNxOgreBody(NULL);

		setNxOgreKinematicBody(
			Application::getInstance()->getPhysicsSubsystem()->getNxOgreRenderSystem()->createKinematicBody(
				pBox,
				getSceneNode()->getPosition(),
				getSceneNode()));

		getNxOgreKinematicBody()->setGlobalOrientationQuat(NxOgre::Quat(getSceneNode()->getOrientation()));

		//Logger::getInstance()->log(getParent()->getName() + " OGRE3DKINEMATICBODY: " + Ogre::StringConverter::toString(
		//	pBox->getFlag()));
	}
}

void PhysicsComponentSimpleBox::destroy()
{
	PhysicsComponentSimple::destroy();
}

NxOgre::Vec3 PhysicsComponentSimpleBox::getNxOgreSize()
{
	return mNxOgreSize;
}

void PhysicsComponentSimpleBox::setNxOgreSize(NxOgre::Vec3 pNxOgreSize)
{
	mNxOgreSize=pNxOgreSize;
	mHeight=pNxOgreSize.y;
}

//void PhysicsComponentSimpleBox::setQueryFlags(QueryFlags queryFlags)
//{
//	mQueryFlags=queryFlags;
//}

TPhysicsComponentSimpleBoxParameters::TPhysicsComponentSimpleBoxParameters() : TPhysicsComponentSimpleParameters()
{

}

TPhysicsComponentSimpleBoxParameters::~TPhysicsComponentSimpleBoxParameters()
{

}