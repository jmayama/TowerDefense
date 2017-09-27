//
//  MainMenu.cpp
//  GAM-1514 OSX Game
//
//  Created by Bradley Flood on 2013-10-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "MainMenu.h"
#include "SettingMenu.h"
#include "../UI/UIButton.h"
#include "../Game/Game.h"
#include "../Game/Level.h"
#include "../Screen Manager/ScreenManager.h"


MainMenu::MainMenu() : Menu(NULL, NULL)
{
    addButton(new UIButton("ButtonLoadLevel"));
    addButton(new UIButton("ButtonLevelEditor"));
    addButton(new UIButton("ButtonSettingMenu"));
	addButton(new UIButton("ButtonHighscores"));
	addButton(new UIButton("ButtonExit"));
}

MainMenu::~MainMenu()
{

}

const char* MainMenu::getName()
{
    return MAIN_MENU_SCREEN_NAME;
}

void MainMenu::buttonAction(UIButton* button)
{
    int index = getIndexForButton(button);
    if(index == 0)
    {
		ScreenManager::getInstance()->switchScreen(LOAD_MENU_SCREEN_NAME);
    }
    else if(index == 1)
    {
        ScreenManager::getInstance()->switchScreen(LEVEL_EDITOR_SCREEN_NAME);
    }
     else if (index == 2)
    {
        ScreenManager::getInstance()->switchScreen(SETTING_MENU_SCREEN_NAME);
    }
	 else if (index == 3)
    {
        ScreenManager::getInstance()->switchScreen(HIGHSCORES_MENU_SCREEN_NAME);
    } 
	else if (index == 3)
    {
        exit(1);
    }
}
