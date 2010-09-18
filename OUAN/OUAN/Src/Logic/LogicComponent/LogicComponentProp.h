#ifndef LOGIC_COMPONENT_PROPH_H
#define LOGIC_COMPONENT_PROPH_H
#include "LogicComponent.h"
namespace OUAN
{
	const double MAX_TIME_SPENT=100;

	class LogicComponentProp: public LogicComponent
	{
	public:
		LogicComponentProp(const std::string& type="");
		~LogicComponentProp();

		void processCollision(GameObjectPtr pGameObject, Ogre::Vector3 pNormal);
		void processEnterTrigger(GameObjectPtr pGameObject);
		void processExitTrigger(GameObjectPtr pGameObject);

		//void processActivate(ActivateEventPtr evt);
		//void processAnimationEnded(AnimationEndedEventPtr evt);
		//...

		void update(double elapsedTime);

		double getApproachDistance() const;
		void setApproachDistance(double approachDistance);

		double getTimeSpent() const;
		void setTimeSpent(double timeSpent);

		double getDelay() const;
		void setDelay(double delay);

		bool hasTakenHit() const;
		void setHasTakenHit(bool hasTakenHit);

		bool isReload() const;
		void setReload(bool reload);

		bool isRecovered() const;
		void setRecovered(bool recovered);

	private:
		double mApproachDistance;
		double mDelay;
		double mTimeSpent;
		bool mHasTakenHit;
		bool mReload;
		bool mRecovered;
	};

	class TLogicComponentPropParameters: public TLogicComponentParameters
	{
	public:
		TLogicComponentPropParameters();
		~TLogicComponentPropParameters();

		double approachDistance;
		double delay;
	};
}
#endif