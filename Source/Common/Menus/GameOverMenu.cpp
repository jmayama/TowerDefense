#include "GameOverMenu.h"

//http://www.alcove-games.com/opengl-es-2-tutorials/lightmap-shader-fire-effect-glsl/

GameOverMenu::GameOverMenu() : Menu(NULL, NULL)
{
    //Add the menu title
    //addMenuTitle(new OpenGLTexture(GameOverMenu_SCREEN_MENU_TITLE));
    
    //Add the menu's background image.
    //addMenuBackground(new OpenGLTexture(GAME_BACKGROUND_TEXTURE));
    
    //Add menu option
    //addMenuOption(new OpenGLTexture(MENU_OPTION_PRESS_PLAY));
}

GameOverMenu::~GameOverMenu()
{
    //Everything get destroyed in the Menu class
}

const char* GameOverMenu::getName()
{
    return GAMEOVER_MENU_SCREEN_NAME;
}

void GameOverMenu::menuAction(int aIndex)
{

}

void GameOverMenu::keyUpEvent(int aKeyCode)
{
    if(aKeyCode == KEYCODE_SPACE)
    {
		ScreenManager::getInstance()->switchScreen(MAIN_MENU_SCREEN_NAME);
    }
}
