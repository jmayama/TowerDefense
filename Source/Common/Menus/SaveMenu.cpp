//
//  SaveMenu.cpp
//  GAM-1514 OSX Game
//
//  Created by Bradley Flood on 2013-10-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
// Modified by Dan Lingman 2013-11-09
// cloned from MainMenu
//

#include "SaveMenu.h"
#include "../UI/UIButton.h"
#include "../Game/Game.h"
#include "../Game/Level.h"
#include "../Screen Manager/ScreenManager.h"
#include "../Level Editor/LevelEditor.h"

SaveMenu::SaveMenu() : Menu(NULL, NULL)
{
    addButton(new UIButton("Tile1"));
    addButton(new UIButton("Tile2"));
    addButton(new UIButton("Tile3"));
    addButton(new UIButton("Tile4"));
    addButton(new UIButton("Tile5"));
    addButton(new UIButton("Tile6"));
}

SaveMenu::~SaveMenu()
{

}

const char* SaveMenu::getName()
{
    return SAVE_MENU_SCREEN_NAME;
}

void SaveMenu::buttonAction(UIButton* button)
{
    int index = getIndexForButton(button);
	std::string fileName;
	fileName.append("Level");
	fileName +='0'+index;
	fileName.append("json");
	// save the level from the appropriate file
	LevelEditor *levelEditor = (LevelEditor*)ScreenManager::getInstance()->getScreenForName(LEVEL_EDITOR_SCREEN_NAME);
	levelEditor->saveLevel(fileName.c_str());
	// and then switch back to the main menu
	ScreenManager::getInstance()->switchScreen(MAIN_MENU_SCREEN_NAME);
}
