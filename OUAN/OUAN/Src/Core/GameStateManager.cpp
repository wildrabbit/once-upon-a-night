#include "GameStateManager.h"
#include "GameState.h"

using namespace OUAN;

void GameStateManager::changeState(GameStatePtr state, ApplicationPtr app) 
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		mStates.back()->cleanUp();
		mStates.pop_back();
	}
	// store and init the new state
	mStates.push_back(state);
	mStates.back()->init(app);
}

void GameStateManager::pushState(GameStatePtr state,ApplicationPtr app)
{
	// pause current state
	if ( !mStates.empty() ) {
		mStates.back()->pause();
	}
	// store and init the new state
	mStates.push_back(state);
	mStates.back()->init(app);
}

void GameStateManager::popState()
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		mStates.back()->cleanUp();
		mStates.pop_back();
	}
	// resume previous state
	if ( !mStates.empty() ) {
		mStates.back()->resume();
	}
}
GameStatePtr GameStateManager::getCurrentState() const
{
	if (!mStates.empty())
	{
		return mStates.back();
	}
	return GameStatePtr();
}
