#ifndef __GAM_1514_OSX_Game__SplashMenu__
#define __GAM_1514_OSX_Game__SplashMenu__

#include "Menu.h"
#include "MainMenu.h"
#include "LoadLevelMenu.h"
#include "../UI/UIButton.h"
#include "../Game/Game.h"
#include "../Game/Level.h"
#include "../Constants/Game/GameConstants.h"
#include "../Screen Manager/ScreenManager.h"

//SplashMenuScreen, inherits from the Menu class, contains "Press space to play"
class SplashMenu : public Menu
{
public:
	SplashMenu();
	~SplashMenu();

	//Screen name, must be implemented, it's a pure virtual
	//method in the Screen class
	const char* getName();

	//Pure Virtual function, inherited from the Menu class,
	//called when a menu option is selected
	void menuAction(int index);

	//void addButton(UIButton* button);

	//int getIndexForButton(UIButton* button);

	//Press space to continue.
	void keyUpEvent(int keyCode);

protected:
	//void buttonIsSelected(UIButton* button);
	//void buttonAction(UIButton* button);

	OpenGLTexture* m_Background;
	OpenGLTexture* m_Title;
	std::vector<UIButton*> m_Buttons;
	int m_SelectedIndex;

};



#endif