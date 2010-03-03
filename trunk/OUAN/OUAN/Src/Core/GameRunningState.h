#ifndef GAMERUNNINGSTATEH_H
#define GAMERUNNINGSTATEH_H

#include "../OUAN.h"
#include "GameState.h"
namespace OUAN
{
	const std::string OVERLAY_INGAME_HUD = "OUAN/HUDOverlay";
	const std::string OVERLAY_DEBUG_PANEL = "Core/DebugOverlay";
	const std::string SAVED_RTT_FILENAME="../../Resources/Graphics/Textures/savedRTT.png";

	///State corresponding to the game's main menu
	class GameRunningState: public GameState{
	public:
		/// init main menu's resources
		void init(ApplicationPtr app);

		/// Clean up main menu's resources
		void cleanUp();

		/// pause state
		void pause();
		/// resume state
		void resume();

		/// process input events
		/// @param app	the parent application
		void handleEvents();
		/// Update game according to the current state
		/// @param app	the parent app
		void update(long elapsedTime);
		
		/// Renders game's visuals to the screen
		/// @param app the parent app
		bool render();

		/// Default constructor
		GameRunningState();
		/// Destructor
		~GameRunningState();
	};
}
#endif