//
//  SaveMenu.h
//  GAM-1514 OSX Game
//
//  Created by Bradley Flood on 2013-10-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
// Modified by Dan Lingman on 2013-11-09
// cloned from MainMenu
//

#ifndef SAVE_MENU_H
#define SAVE_MENU_H

#include "Menu.h"


class SaveMenu : public Menu
{
public:
    SaveMenu();
    ~SaveMenu();
    
    const char* getName();

private:
    void buttonAction(UIButton* button);
};

#endif
