#include "OUAN_Precompiled.h"

#include "CameraControllerThirdPerson.h"
#include "CameraInput.h"
#include "CameraParameters.h"
#include "../../Physics/PhysicsComponent/PhysicsComponentCharacter.h"
#include "../RenderComponent/RenderComponentPositional.h"

using namespace OUAN;
using namespace Ogre;

CameraControllerThirdPerson::CameraControllerThirdPerson()
{
}
CameraControllerThirdPerson::~CameraControllerThirdPerson()
{
}

void CameraControllerThirdPerson::init(Ogre::SceneManager * pSceneManager)
{
	loadInfo();
}

TCameraControllerType CameraControllerThirdPerson::getControllerType()
{
	return OUAN::CAMERA_THIRD_PERSON;
}

void CameraControllerThirdPerson::update(Ogre::Camera *pCamera,CameraInputPtr pCameraInput,double elapsedTime)
{
	pCamera->setPosition(calculateCameraPosition(pCamera,pCameraInput));
	pCamera->lookAt(calculateTargetPosition(pCameraInput));
}

Ogre::Vector3 CameraControllerThirdPerson::calculateTargetPosition(CameraInputPtr pCameraInput)
{
	return pCameraInput->mTarget->getSceneNode()->getPosition()+pCameraInput->mTargetOffset;
}

Ogre::Vector3 CameraControllerThirdPerson::calculateCameraPosition(Ogre::Camera * pCamera,CameraInputPtr pCameraInput)
{
	Vector3 targetPosition;
	Vector3 cameraPosition;
	targetPosition=pCameraInput->mTarget->getSceneNode()->getPosition();
	cameraPosition=pCamera->getPosition();

	double distance=calculateDistanceToTarget(pCamera,pCameraInput,true);

	Vector3 direction=Vector3(cameraPosition.x-targetPosition.x,
							0.0f,
							cameraPosition.z-targetPosition.z);
	direction.normalise();
	
	if(distance>pCameraInput->mCameraParameters->mMaxDistance)
	{
		cameraPosition=Vector3(direction.x*pCameraInput->mCameraParameters->mMaxDistance+targetPosition.x,
						   0.0f,
						   direction.z*pCameraInput->mCameraParameters->mMaxDistance+targetPosition.z);

	}

	if(distance<pCameraInput->mCameraParameters->mMinDistance)
	{
		cameraPosition=Vector3(direction.x*pCameraInput->mCameraParameters->mMinDistance+targetPosition.x,
						   0.0f,
						   direction.z*pCameraInput->mCameraParameters->mMinDistance+targetPosition.z);

	}

	cameraPosition.y=calculateCameraHeight(pCamera,pCameraInput);

	return cameraPosition;
}

void CameraControllerThirdPerson::loadInfo()
{
	CameraController::loadInfo();
}

Ogre::Vector3 CameraControllerThirdPerson::rotateMovementVector(Ogre::Vector3 movement,Ogre::Camera * pCamera,CameraInputPtr pCameraInput)
{
	//Logger::getInstance()->log("ROTATE MOVEMENT VECTOR");
	//Logger::getInstance()->log("Original movement : "+Ogre::StringConverter::toString(movement));

	double radium;
	radium=calculateDistanceToTarget(pCamera,pCameraInput,true);
	if(radium!=0)
	{
		movement = Ogre::Quaternion(Ogre::Degree(((Ogre::Math::TWO_PI)*movement.x)/radium),Ogre::Vector3::UNIT_Y) * movement;
	}
	//Logger::getInstance()->log("Rotated movement1 : "+Ogre::StringConverter::toString(movement));

	movement = Ogre::Quaternion(Ogre::Degree(pCamera->getOrientation().getYaw().valueDegrees()-180),Ogre::Vector3::UNIT_Y) * movement;

	//Logger::getInstance()->log("Rotated movement2 : "+Ogre::StringConverter::toString(movement));

	//Logger::getInstance()->log("radium : "+Ogre::StringConverter::toString(Ogre::Real(radium)));

	return movement;
}

double CameraControllerThirdPerson::calculateCameraHeight(Ogre::Camera *pCamera,CameraInputPtr pCameraInput)
{
	double h;
	double distance;
	double relative_distance;

	//To avoid division by 0
	if(pCameraInput->mCameraParameters->mMinDistance==pCameraInput->mCameraParameters->mMaxDistance)
		return pCameraInput->mCameraParameters->mMaxH;
	if(pCameraInput->mCameraParameters->mMaxH==pCameraInput->mCameraParameters->mMinH)
		return pCameraInput->mCameraParameters->mMaxH;

	//calculate relative distance between min and max distance
	distance=calculateDistanceToTarget(pCamera,pCameraInput,true);

	relative_distance=(distance-pCameraInput->mCameraParameters->mMinDistance)/(pCameraInput->mCameraParameters->mMaxDistance-pCameraInput->mCameraParameters->mMinDistance);

	//normalize relative distance
	if(relative_distance<0)
		relative_distance=0;

	if(relative_distance>1)
		relative_distance=1;

	//calculate h
	h=pCameraInput->mCameraParameters->mMinH+relative_distance*(pCameraInput->mCameraParameters->mMaxH-pCameraInput->mCameraParameters->mMinH);

	return h;
}

void CameraControllerThirdPerson::setCameraParameters(Ogre::Camera *pCamera,CameraInputPtr pCameraInput)
{
	Vector3 cameraPosition;
	Vector3 targetPosition;
	Vector3 direction;

	targetPosition=pCameraInput->mTarget->getSceneNode()->getPosition();

	direction=Vector3(pCameraInput->mCameraParameters->mDirection.x,0,pCameraInput->mCameraParameters->mDirection.z);
	direction.normalise();

	cameraPosition=Vector3(direction.x*pCameraInput->mCameraParameters->mMaxDistance+targetPosition.x,
						   calculateCameraHeight(pCamera,pCameraInput),
						   direction.z*pCameraInput->mCameraParameters->mMaxDistance+targetPosition.z);

	pCamera->setPosition(cameraPosition);
}


//#include "OUAN_Precompiled.h"
//
//#include "CameraControllerThirdPerson.h"
//#include "TransparentEntityManager.h"
//#include "../RenderComponent/RenderComponentPositional.h"
//#include "../RenderSubsystem.h"
//#include "../../RayCasting/RayCasting.h"
//#include "../../Loader/Configuration.h"
//#include "../../Game/GameObject/GameObject.h"
//#include "../../Game/GameObject/GameObjectViewport.h"
//#include "../../Game/GameWorldManager.h"
//
//using namespace OUAN;
//using namespace Ogre;
//
//CameraControllerThirdPerson::CameraControllerThirdPerson() : CameraController()
//{
//	//Set CameraControllerThirdPerson Initial Parameters
//
//	loadConfig();
//
//	rotX=0;
//	rotY=0;
//	cameraMoved=false;
//
//	currentCollisionTime=0;
//	currentDistance=maxDistance;
//	mCurrentCollisionMargin=0;
//
//}
//
//CameraControllerThirdPerson::~CameraControllerThirdPerson()
//{
//
//}
//
//void CameraControllerThirdPerson::setChangeWorldMaxDistance()
//{
//	maxDistance=maxDistanceChangeWorld;
//	targetHeight=changeWorldHeight;
//}
//void CameraControllerThirdPerson::setOriginalMaxDistance()
//{
//	maxDistance=originalMaxDistance;
//	targetHeight=orignalHeight;
//}
//
//void CameraControllerThirdPerson::calculateCurrentHeight(double elapsedTime)
//{
//	if(currentHeight<(targetHeight-heightMargin))
//	{
//		currentHeight+=elapsedTime*heightSpeed;
//	}
//	else if(currentHeight>(targetHeight+heightMargin))
//	{
//		currentHeight-=elapsedTime*heightSpeed;
//	}
//	else
//	{
//		currentHeight=targetHeight;
//	}
//}
//
//
//void CameraControllerThirdPerson::init(RenderSubsystemPtr pRenderSubsystem,PhysicsSubsystemPtr pPhysicsSubsystem,RayCastingPtr pRayCasting,GameWorldManagerPtr pGameWorldManager)
//{
//	mSceneManager = pRenderSubsystem->getSceneManager();
//
//	mRenderSubsystem = pRenderSubsystem;
//
//	mRayCasting = pRayCasting;
//
//	mTransparentEntityManager.reset(new TransparentEntityManager());
//	mTransparentEntityManager->init();
//
//	mGameWorldManager = pGameWorldManager;
//
//	CameraController::init(pRenderSubsystem->getSceneManager());
//}
//
//Ogre::Vector3 CameraControllerThirdPerson::calculateCameraPosition(double distance,bool y_correction,bool applyCollisionMargin)
//{
//	Ogre::Vector3 newCameraPosition;
//
//	newCameraPosition=distance*initialDirection;
//
//	//Calculate Camera position in relation to the target
//	if(rotX<0)
//	{
//		newCameraPosition = newCameraPosition-rotXDistanceAttenuationNegative*newCameraPosition*rotX/double(minRotX);
//	}
//	else
//	{
//		newCameraPosition = newCameraPosition-rotXDistanceAttenuationPositive*newCameraPosition*rotX/double(maxRotX);
//	}
//
//	newCameraPosition = Quaternion(Ogre::Degree(rotX), Vector3::UNIT_X) * newCameraPosition;
//	newCameraPosition = Quaternion(Ogre::Degree(rotY), Vector3::UNIT_Y) * newCameraPosition;
//
//	//Calculate Camera position in the world
//	newCameraPosition = target->getPosition()+newCameraPosition;
//
//	if(applyCollisionMargin)
//	{
//		newCameraPosition+=Vector3(0,mCurrentCollisionMargin,0);
//	}
//
//	//jumping correction
//	//if(target->getParent()->isJumping() && y_correction && Ogre::Math::Abs(newCameraPosition.y-mCamera->getPosition().y)>maxYMovementPerFrame)
//	//{
//	//	if(newCameraPosition.y>mCamera->getPosition().y)
//	//	{
//	//		newCameraPosition.y=mCamera->getPosition().y+maxYMovementPerFrame;
//	//	}
//	//	else if(newCameraPosition.y<mCamera->getPosition().y)
//	//	{
//	//		newCameraPosition.y=mCamera->getPosition().y-maxYMovementPerFrame;
//	//	}
//	//	//newCameraPosition.y=mCamera->getPosition().y;
//	//}
//
//	//Logger::getInstance()->log("y_correction "+Ogre::StringConverter::toString(y_correction));
//
//	return newCameraPosition;
//}
//
//Ogre::Vector3 CameraControllerThirdPerson::calculateCameraLookAt()
//{
//	Ogre::Vector3 cameraLookAt;
//
//	//Calculate Camera look at
//	cameraLookAt=target->getPosition()+Vector3(0,currentHeight,0);
//
//	return cameraLookAt;
//}
//
//bool CameraControllerThirdPerson::calculateCameraCollisions(Ogre::Vector3 & cameraPosition, Ogre::Vector3 & cameraLookAt, Ogre::uint32 & collisionType)
//{
//	Ogre::Vector3 direction;
//	Ogre::Vector3 newCameraPosition;
//	double currentDistance;
//	Ogre::Entity * pEntity;
//	std::vector<Ogre::Entity*> allCollisions;
//	std::vector<Ogre::Entity*> transparentEntities;
//	unsigned int i;
//
//	bool hasBeenCollision=false;
//
//	currentDistance=cameraLookAt.distance(cameraPosition);
//
//	direction=cameraPosition-cameraLookAt;
//	direction.normalise();
//
//	newCameraPosition=cameraPosition;
//
//	allCollisions.clear();
//	transparentEntities.clear();
//
//	mRayCasting->raycastRenderAllGeometry(cameraLookAt,direction,newCameraPosition,pEntity,allCollisions,currentDistance,QUERYFLAGS_CAMERA_COLLISION);
//
//	if(cameraLookAt.distance(newCameraPosition)<currentDistance)
//	{
//		collisionType=pEntity->getQueryFlags();
//		cameraPosition=newCameraPosition;
//		hasBeenCollision=true;
//
//		for(i=0;i<allCollisions.size();i++)
//		{
//			if((allCollisions[i]->getQueryFlags() & OUAN::QUERYFLAGS_CAMERA_COLLISION_TRANSLUCID) && allCollisions[i]->isVisible())
//			{
//				transparentEntities.push_back(allCollisions[i]);
//			}
//		}
//	}
//
//	mTransparentEntityManager->addCurrentCollisionTransparentEntities(transparentEntities);
//
//	return hasBeenCollision;
//}
//
//void CameraControllerThirdPerson::rotateX(double rotation)
//{
//	rotX+=rotation;
//	//check if rotation exceeds limits
//	if(rotX>maxRotX)
//	{
//		rotX=maxRotX;
//	}
//	else if(rotX<minRotX)
//	{
//		rotX=minRotX;
//	}
//}
//
//void CameraControllerThirdPerson::rotateY(double rotation)
//{
//	rotY+=rotation;
//}
//
//void CameraControllerThirdPerson::calculateCollisionRotXNegative(double elapsedTime)
//{
//	if(lastRotX!=0)
//	{
//		rotateX(-lastRotX);
//	}
//	else if(rotX>minAutoRotX)
//	{
//		rotateX(-elapsedTime*autoRotXNegativeSpeed);
//	}
//	else
//	{
//		//if(!target->getParent()->isMoving())
//		//{
//		//	rotateY(elapsedTime*autoRotYSpeed);
//		//}
//	}
//}
//
//void CameraControllerThirdPerson::calculateCollisionRotXPositive(double elapsedTime)
//{
//	if(lastRotX!=0)
//	{
//		rotateX(-lastRotX);
//	}
//	else if(rotX<maxAutoRotX)
//	{
//		rotateX(elapsedTime*autoRotXPositiveSpeed);
//	}
//	else
//	{
//		//if(!target->getParent()->isMoving())
//		//{
//		//	rotateY(elapsedTime*autoRotYSpeed);
//		//}
//	}
//}
//
//void CameraControllerThirdPerson::calculateMotionBlur(double elapsedTime)
//{
//	if((Ogre::Math::Abs(lastRotX)>=mMinCameraRotationMotionBlurActivation) || (Ogre::Math::Abs(lastRotY)>=mMinCameraRotationMotionBlurActivation))
//	{
//		mGameWorldManager->getGameObjectViewport()->setEffect(mRenderSubsystem->MOTION_BLUR,false);
//		//Logger::getInstance()->log("MOTION BLUR ACTIVATED x "+Ogre::StringConverter::toString(Ogre::Math::Abs(lastRotX))+" y "+Ogre::StringConverter::toString(Ogre::Math::Abs(lastRotY)));
//
//	}
//	else if((Ogre::Math::Abs(lastRotX)<=mMinCameraRotationMotionBlurDisactivation) || (Ogre::Math::Abs(lastRotY)<=mMinCameraRotationMotionBlurDisactivation))
//	{
//		mGameWorldManager->getGameObjectViewport()->setEffect(mRenderSubsystem->MOTION_BLUR,false);
//		//Logger::getInstance()->log("MOTION BLUR DIS-ACTIVATED x "+Ogre::StringConverter::toString(Ogre::Math::Abs(lastRotX))+" y "+Ogre::StringConverter::toString(Ogre::Math::Abs(lastRotY)));
//	}
//}
//
//void CameraControllerThirdPerson::update(Ogre::Camera *pCamera,CameraInputPtr pCameraInput,double elapsedTime)
//{
//	Vector3 cameraCollisionPosition;
//	Vector3 newCameraPosition;
//	Vector3 cameraLookAt;
//	Ogre::uint32 collisionType;
//
//	//calculateCurrentHeight(elapsedTime);
//
//	newCameraPosition=calculateCameraPosition(maxDistance);
//	cameraLookAt=calculateCameraLookAt();
//
//	//moving rotX correction
//	if(target->getParent()->isMoving() && !target->getParent()->cancelAutoCameraMovement())
//	{
//		if(rotX>maxCameraCenterRotX)
//		{
//			rotateX(-elapsedTime*autoRotXCenterSpeed);
//		}
//		else if(rotX<minCameraCenterRotX)
//		{
//			rotateX(elapsedTime*autoRotXCenterSpeed);
//		}
//	}
//
//	cameraCollisionPosition=newCameraPosition;
//	//Calculate camera collisions
//	if(calculateCameraCollisions(cameraCollisionPosition,cameraLookAt,collisionType))
//	{
//		currentCollisionTime+=elapsedTime;
//		if(currentCollisionTime>=minCollisionTime)
//		{
//			mCorrectingCameraPosition=true;
//			if(collisionType & OUAN::QUERYFLAGS_CAMERA_COLLISION_MOVE_TO_TARGET)
//			{
//				mCurrentCollisionMargin=mMaxCollisionYMargin;
//				currentDistance=calculateCameraMoveToTarget(currentDistance,pCamera->getPosition(),cameraCollisionPosition,elapsedTime);
//			}
//			else if(collisionType & OUAN::QUERYFLAGS_CAMERA_COLLISION_ROTX_NEGATIVE)
//			{
//				if(!target->getParent()->cancelAutoCameraMovement())
//				{
//					calculateCollisionRotXNegative(elapsedTime);
//				}
//			}
//			else if(collisionType & OUAN::QUERYFLAGS_CAMERA_COLLISION_ROTX_POSITIVE)
//			{
//				if(!target->getParent()->cancelAutoCameraMovement())
//				{
//					calculateCollisionRotXPositive(elapsedTime);
//				}
//			}
//		}
//	}
//	else
//	{
//		mCorrectingCameraPosition=false;
//		currentCollisionTime=0;
//		if(currentDistance<=maxDistance)
//		{
//			currentDistance=calculateCameraReturningFromTarget(currentDistance,pCamera->getPosition(),newCameraPosition,elapsedTime);
//		}
//		else
//		{
//			currentDistance=calculateCameraMoveToTarget(currentDistance,pCamera->getPosition(),newCameraPosition,elapsedTime);
//		}
//	}
//
//	//set camera position
//	newCameraPosition=calculateCameraPosition(currentDistance,true,true);
//
//	pCamera->setPosition(newCameraPosition);
//
//	//set camera to look at target
//	pCamera->lookAt(cameraLookAt);
//
//	mTransparentEntityManager->update(elapsedTime);
//
//	calculateMotionBlur(elapsedTime);
//}
//
//void CameraControllerThirdPerson::clear()
//{
//	mTransparentEntityManager->clear();
//}
//
//double CameraControllerThirdPerson::calculateCameraReturningFromTarget(double currentCameraDistance, Ogre::Vector3 cameraPosition,Ogre::Vector3 newCameraPosition,double elapsedTime)
//{
//	double newCameraDistance;
//
//	newCameraDistance=currentCameraDistance+=collisionReturningSpeed*elapsedTime;
//	if(newCameraDistance>=maxDistance)
//	{
//		newCameraDistance=maxDistance;
//	}
//
//	mCurrentCollisionMargin-=collisionReturningSpeed*elapsedTime;
//	if(mCurrentCollisionMargin<=0)
//	{
//		mCurrentCollisionMargin=0;
//	}
//	return newCameraDistance;
//}
//
//double CameraControllerThirdPerson::calculateCameraMoveToTarget(double currentCameraDistance, Ogre::Vector3 cameraPosition,Ogre::Vector3 newCameraPosition,double elapsedTime)
//{
//
//	double newCameraDistance;
//
//	newCameraDistance=currentCameraDistance-=collisionMoveSpeed*elapsedTime;
//	if (newCameraDistance<=(cameraPosition.distance(newCameraPosition)))
//	{
//		newCameraDistance=cameraPosition.distance(newCameraPosition);
//	}
//	else if (newCameraDistance<=minDistance)
//	{
//		newCameraDistance=minDistance;
//	}
//
//	return newCameraDistance;
//}
//
//
//void CameraControllerThirdPerson::processCameraRotation(Ogre::Vector2 cameraRotation)
//{
//
//	//process Relative Motion
//	if(cameraRotation.x==0 && cameraRotation.y==0) 
//	{
//		lastRotX=0;
//		lastRotY=0;
//		cameraMoved=false;
//		return;
//	}
//	else
//	{
//		cameraMoved=true;
//		rotateY(-cameraRotation.x*speedY);
//		rotateX(-cameraRotation.y*speedX);
//		lastRotX=-cameraRotation.x*speedY;
//		lastRotY=-cameraRotation.y*speedX;
//	}
//
//
//	//Logger::getInstance()->log("rotations "+Ogre::StringConverter::toString(Ogre::Real(rotX))+" "+Ogre::StringConverter::toString(Ogre::Real(rotY)));
//
//}
//void CameraControllerThirdPerson::setTarget(RenderComponentPositionalPtr target)
//{
//	this->target=target;
//	reset();
//}
//
//void CameraControllerThirdPerson::reset()
//{
//	rotX=0;
//	rotY=0;
//	cameraMoved=false;
//
//	currentCollisionTime=0;
//	currentDistance=maxDistance;
//	mCurrentCollisionMargin=0;
//
//
//	rotY=target->getOrientation().getYaw().valueDegrees();
//
//	//mCamera->setPosition(calculateCameraPosition(maxDistance));
//	//mCamera->lookAt(calculateCameraLookAt());
//}
//
//TCameraControllerType CameraControllerThirdPerson::getControllerType()
//{
//	return OUAN::CAMERA_THIRD_PERSON;
//}