#ifndef __GAM_1514_OSX_Game__SettingMenu__
#define __GAM_1514_OSX_Game__SettingMenu__

#include "Menu.h"
#include "MainMenu.h"
#include "LoadLevelMenu.h"
#include "../UI/UIButton.h"
#include "../Game/Game.h"
#include "../Game/Level.h"
#include "../Constants/Menu/MenuConstants.h"
#include "../Constants/Game/GameConstants.h"
#include "../Screen Manager/ScreenManager.h"
#include "../OpenGL/OpenGL.h"


//SettingMenuScreen, inherits from the Menu class, contains "Press space to play"
class SettingMenu : public Menu
{
public:
	SettingMenu();
	~SettingMenu();

	//Screen name, must be implemented, it's a pure virtual
	//method in the Screen class
	const char* getName();

	//Pure Virtual function, inherited from the Menu class,
	//called when a menu option is selected
	void menuAction(int index);
	void paint();
	//void addButton(UIButton* button);

	//int getIndexForButton(UIButton* button);
	void addMenuTitle(OpenGLTexture* aMenuTitle);

	//Press space to continue.
	void keyUpEvent(int keyCode);

protected:
	//void buttonIsSelected(UIButton* button);
	//void buttonAction(UIButton* button);
	OpenGLTexture* m_MenuTitle;
	OpenGLTexture* m_Background;
	OpenGLTexture* m_Title;
	std::vector<UIButton*> m_Buttons;
	int m_SelectedIndex;

};



#endif