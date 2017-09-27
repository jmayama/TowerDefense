//
//  LoadLevelMenu.h
//  GAM-1514 OSX Game
//
//  Created by Bradley Flood on 2013-10-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
// Modified by Dan Lingman on 2013-11-09
// cloned from MainMenu
//

#ifndef LOAD_LEVEL_MENU_H
#define LOAD_LEVEL_MENU_H

#include "Menu.h"


class LoadLevelMenu : public Menu
{
public:
    LoadLevelMenu();
    ~LoadLevelMenu();
    
    const char* getName();

private:
    void buttonAction(UIButton* button);
};

#endif 
