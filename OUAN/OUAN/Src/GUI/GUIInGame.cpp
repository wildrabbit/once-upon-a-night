#include "OUAN_Precompiled.h"

#include "GUIInGame.h"
#include "GUISubsystem.h"
#include "../Core/InGameMenuState.h"
#include "../Application.h"

using namespace OUAN;

void GUIInGame::initGUI(GameStatePtr parentGameState)
{
	GUIWindow::initGUI(parentGameState);
	bindEvents();
	setStrings(parentGameState->getApp()->getCurrentLanguage());
}
void GUIInGame::bindEvents()
{
	mParentGameState->getApp()->getGUISubsystem()->bindEvent(CEGUI::PushButton::EventClicked,
		"OUANInGameMenu/Return",
		CEGUI::Event::Subscriber(&GUIInGame::onBackToGame,this));
	mParentGameState->getApp()->getGUISubsystem()->bindEvent(CEGUI::PushButton::EventClicked,
		"OUANInGameMenu/Options",
		CEGUI::Event::Subscriber(&GUIInGame::onOptions,this));
	mParentGameState->getApp()->getGUISubsystem()->bindEvent(CEGUI::PushButton::EventClicked,
		"OUANInGameMenu/GameQuit",
		CEGUI::Event::Subscriber(&GUIInGame::onBackToMenu,this));
	mParentGameState->getApp()->getGUISubsystem()->bindEvent(CEGUI::PushButton::EventClicked,
		"OUANInGameMenu/AppQuit",
		CEGUI::Event::Subscriber(&GUIInGame::onQuit,this));
}
bool GUIInGame::onBackToGame(const CEGUI::EventArgs& args)
{
	(static_cast<InGameMenuState*>(mParentGameState.get()))->backToGame();
	return true;
}
bool GUIInGame::onOptions(const CEGUI::EventArgs& args)
{
	(static_cast<InGameMenuState*>(mParentGameState.get()))->goToOptions();
	return true;
}
bool GUIInGame::onBackToMenu(const CEGUI::EventArgs& args)
{
	(static_cast<InGameMenuState*>(mParentGameState.get()))->backToMenu();
	return true;
}
bool GUIInGame::onQuit(const CEGUI::EventArgs& args)
{
	(static_cast<InGameMenuState*>(mParentGameState.get()))->quit();
	return true;
}
void GUIInGame::setStrings(const std::string& language)
{
	ConfigurationPtr texts=mParentGameState->getApp()->getMenusTextStrings();
	if (texts.get())
	{

		std::string windowNames[] = {INGAME_CEGUIWIN_ID_BTG,INGAME_CEGUIWIN_ID_BTM,
			INGAME_CEGUIWIN_ID_EXIT,INGAME_CEGUIWIN_ID_OPTIONS};
		int windowNamesLen=4;
		std::string stringKey="";
		std::string stringVal="";
		CEGUI::Window* win=NULL;
		for (int i=0;i<windowNamesLen;i++)
		{
			win=CEGUI::WindowManager::getSingletonPtr()->getWindow(windowNames[i]);
			if (win)
			{
				stringKey=win->getText().c_str();
				texts->getOption(stringKey,stringVal);
				win->setText(stringVal);
			}
		}
	}	 
}