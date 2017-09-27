#include "SplashMenu.h"

//http://www.alcove-games.com/opengl-es-2-tutorials/lightmap-shader-fire-effect-glsl/

SplashMenu::SplashMenu() : Menu(NULL, NULL)
{
    //Add the menu title
    //addMenuTitle(new OpenGLTexture(SplashMenu_SCREEN_MENU_TITLE));
    
    //Add the menu's background image.
    //addMenuBackground(new OpenGLTexture(GAME_BACKGROUND_TEXTURE));
    
    //Add menu option
    //addMenuOption(new OpenGLTexture(MENU_OPTION_PRESS_PLAY));
}

SplashMenu::~SplashMenu()
{
    //Everything get destroyed in the Menu class
}

const char* SplashMenu::getName()
{
    return SPLASH_MENU_SCREEN_NAME;
}

void SplashMenu::menuAction(int aIndex)
{

}

void SplashMenu::keyUpEvent(int aKeyCode)
{
    if(aKeyCode == KEYCODE_SPACE)
    {
		ScreenManager::getInstance()->switchScreen(MAIN_MENU_SCREEN_NAME);
    }
}
