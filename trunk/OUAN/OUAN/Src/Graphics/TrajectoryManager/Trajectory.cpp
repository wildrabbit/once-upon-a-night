#include "Trajectory.h"
#include "TrajectoryNode.h"
using namespace OUAN;

Trajectory::Trajectory()
{
	totalTime=0;
	currentNode=0;
	loopTrajectory=true;
}

Trajectory::~Trajectory()
{

}
bool Trajectory::getLoopTrajectory() const
{
	return loopTrajectory;
}
void Trajectory::setLoopTrajectory(bool loopTrajectory)
{
	this->loopTrajectory=loopTrajectory;
}

int Trajectory::getNextNode()
{
	unsigned int nextNode=currentNode+1;
	if(nextNode>=trajectoryNodes.size())
	{
		return 0;
	}
	else
	{
		return nextNode;
	}
}

std::vector<TrajectoryNode *>  Trajectory::getTrajectoryNodes() const
{
	return trajectoryNodes;
}

void Trajectory::update(double elapsedTime)
{
	unsigned int nextNode;

	//Calculate total time
	totalTime+=elapsedTime;
	Ogre::LogManager::getSingleton().logMessage("Updating trajectory camera "+Ogre::StringConverter::toString(Ogre::Real(totalTime)));

	//Calculate current node
	if(totalTime>=trajectoryNodes[currentNode]->getTimeToNextNode())
	{
		currentNode++;

		if(currentNode>= trajectoryNodes.size())
		{
			//If we do not want to loop the trajectory we return and leave the camera where it is
			if (!loopTrajectory)
			{
				currentNode=trajectoryNodes.size()-1;
				return;
			}
			else
			{
				//We set current node to the starting trajectory node
				currentNode=0;
			}
		}

		totalTime=0;
	}

	//Calculate next node
	nextNode=getNextNode();

	//Calculate current orientation
	currentOrientation= Quaternion::Slerp((totalTime/trajectoryNodes[currentNode]->getTimeToNextNode()), 
		trajectoryNodes[currentNode]->getSceneNode()->getOrientation(), 
		trajectoryNodes[nextNode]->getSceneNode()->getOrientation(),true);

	Ogre::LogManager::getSingleton().logMessage("Updating orientation "+Ogre::StringConverter::toString(currentPosition));

	//Calculate current position
	currentPosition= (1-totalTime/trajectoryNodes[currentNode]->getTimeToNextNode())*trajectoryNodes[currentNode]->getSceneNode()->getPosition()+
		(totalTime/trajectoryNodes[currentNode]->getTimeToNextNode())*trajectoryNodes[nextNode]->getSceneNode()->getPosition();

	Ogre::LogManager::getSingleton().logMessage("Updating position "+Ogre::StringConverter::toString(currentPosition));
}

Quaternion Trajectory::getCurrentOrientation()
{
	return currentOrientation;
}

Vector3 Trajectory::getCurrentPosition()
{
	return currentPosition;
}

void Trajectory::reset()
{
	totalTime = 0;
	currentNode = 0;
}

void Trajectory::addTrajectoryNode(Ogre::SceneNode * sceneNode, double timeToNextNode)
{
	TrajectoryNode * pTrajectoryNode;

	pTrajectoryNode = new TrajectoryNode();

	pTrajectoryNode->setSceneNode(sceneNode);
	pTrajectoryNode->setTimeToNextNode(timeToNextNode);

	this->trajectoryNodes.push_back(pTrajectoryNode);
}

TTrajectoryParameters::TTrajectoryParameters()
{

}

TTrajectoryParameters::~TTrajectoryParameters()
{

}



