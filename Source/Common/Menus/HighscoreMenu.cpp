#include "HighscoreMenu.h"

//http://www.alcove-games.com/opengl-es-2-tutorials/lightmap-shader-fire-effect-glsl/

HighscoreMenu::HighscoreMenu() : Menu(NULL, NULL)
{
    //Add the menu title
    //addMenuTitle(new OpenGLTexture(HighscoreMenu_SCREEN_MENU_TITLE));
     m_MenuTitle = new OpenGLTexture(HIGHSCORES_MENU_TITLE);

    //Add the menu's background image.
    //addMenuBackground(new OpenGLTexture(GAME_BACKGROUND_TEXTURE));
    
    //Add menu option
    //addMenuOption(new OpenGLTexture(MENU_OPTION_PRESS_PLAY));
}

HighscoreMenu::~HighscoreMenu()
{
    //Everything get destroyed in the Menu class
	if(m_MenuTitle != NULL)
  {
    delete m_MenuTitle;
    m_MenuTitle = NULL;
  }
}

const char* HighscoreMenu::getName()
{
    return HIGHSCORES_MENU_SCREEN_NAME;
}

void HighscoreMenu::menuAction(int aIndex)
{

}

void HighscoreMenu::keyUpEvent(int aKeyCode)
{
    if(aKeyCode == KEYCODE_SPACE)
    {
		ScreenManager::getInstance()->switchScreen(MAIN_MENU_SCREEN_NAME);
    }
}

void HighscoreMenu::addMenuTitle(OpenGLTexture* aMenuTitle)
{
  if(aMenuTitle != NULL)
  {
    m_MenuTitle = aMenuTitle;
  }
}

void HighscoreMenu::paint()
{
 //Draw the menu title
  int x = (getWidth() - m_MenuTitle->getSourceWidth()) *0.5;
  int y = (getHeight() - m_MenuTitle->getSourceHeight()) *0.01;
  OpenGLRenderer::getInstance()->drawTexture(m_MenuTitle, (float)x, (float)y);
}