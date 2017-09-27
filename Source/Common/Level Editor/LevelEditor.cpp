//
//  LevelEditor.cpp
//  GAM-1514 OSX Game
//
//  Created by Bradley Flood on 2013-10-25.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
// Modifiedby Dan Lingman on 2013-11-09
// added more tiles
// added command menu
//

#include "LevelEditor.h"
#include "../Constants/Constants.h"
#include "../Game/Level.h"
#include "../Screen Manager/ScreenManager.h"
#include "../UI/UIButton.h"
#include "../UI/UIToggle.h"


LevelEditor::LevelEditor() :
    m_TilesMenu(NULL),
	m_CommandMenu(NULL),
    m_Level(NULL),
    m_IsMouseDown(false),
    m_SelectedTileIndex(-1)
{
    //Create the Tiles menu items
    m_TilesMenu = new UISideMenu(this, SideMenuRight);
    m_TilesMenu->addButton(new UIToggle("MenuTileGround"));
    m_TilesMenu->addButton(new UIToggle("MenuTileGrass"));
    m_TilesMenu->addButton(new UIToggle("MenuTileRoad"));
    m_TilesMenu->addButton(new UIToggle("MenuTileRock"));
    m_TilesMenu->addButton(new UIToggle("MenuTileTree"));
    m_TilesMenu->addButton(new UIToggle("MenuTileWater"));

	// create the command menu on the left
	m_CommandMenu = new UISideMenu(this,SideMenuLeft);
	m_CommandMenu->addButton(new UIButton("MenuButtonClear"));
	m_CommandMenu->addButton(new UIButton("MenuButtonLoad"));
	m_CommandMenu->addButton(new UIButton("MenuButtonSave"));
	m_CommandMenu->addButton(new UIButton("MenuButtonExit"));

    //Create the level object
    m_Level = new Level(true);
    
    //Reset everything
    reset();
}

LevelEditor::~LevelEditor()
{
    if(m_Level != NULL)
    {
        delete m_Level;
        m_Level = NULL;
    }
    
    if(m_TilesMenu != NULL)
    {
        delete m_TilesMenu;
        m_TilesMenu = NULL;
    }
	   if(m_CommandMenu != NULL)
    {
        delete m_CommandMenu;
        m_CommandMenu = NULL;
    }
}

void LevelEditor::loadLevel(const char * levelName)
{
	m_Level->load(levelName);
}

Level *LevelEditor::getLevel()
{
	return m_Level;
}

void LevelEditor::setLevel(Level* level)
{
	if (m_Level != NULL)
	{
		delete m_Level;
		m_Level = NULL;
	}
	m_Level = level;
}

void LevelEditor::saveLevel(const char * levelName)
{
	m_Level->save(levelName);
}
const char* LevelEditor::getName()
{
    return LEVEL_EDITOR_SCREEN_NAME;
}

void LevelEditor::update(double delta)
{
    if(m_Level != NULL)
    {
        m_Level->update(delta);
    }

    if(m_TilesMenu != NULL)
    {
        m_TilesMenu->update(delta);
    }
    if(m_CommandMenu != NULL)
    {
        m_CommandMenu->update(delta);
    }
}

void LevelEditor::paint()
{
    if(m_Level != NULL)
    {
        m_Level->paint();
    }

    if(m_TilesMenu != NULL)
    {
        m_TilesMenu->paint();
    }
	if(m_CommandMenu != NULL)
    {
        m_CommandMenu->paint();
    }
}

void LevelEditor::reset()
{
    if(m_Level != NULL)
    {
        m_Level->reset();
    }
}

void LevelEditor::mouseMovementEvent(float deltaX, float deltaY, float positionX, float positionY)
{    
    if(m_TilesMenu != NULL)
    {
        m_TilesMenu->mouseMovementEvent(deltaX, deltaY, positionX, positionY);
    }
    if(m_CommandMenu != NULL)
    {
        m_CommandMenu->mouseMovementEvent(deltaX, deltaY, positionX, positionY);
    }    
    if(m_Level != NULL)
    {        
        if(m_SelectedTileIndex != -1 && m_IsMouseDown == true)
        {
            m_Level->setTileTypeAtPosition((TileType)m_SelectedTileIndex, (int)positionX, (int)positionY);
        }
    }
}

void LevelEditor::mouseLeftClickDownEvent(float positionX, float positionY)
{
    //Set the mouse down flag
    m_IsMouseDown = true;
}

void LevelEditor::mouseLeftClickUpEvent(float positionX, float positionY)
{
    //Set the mouse up flag
    m_IsMouseDown = false;
    
    //Safety check the level pointer, then set the new tile type in the index
    if(m_Level != NULL)
    {
        if(m_SelectedTileIndex != -1 && m_TilesMenu->isShowing() == false)
        {
            m_Level->setTileTypeAtPosition((TileType)m_SelectedTileIndex, (int)positionX, (int)positionY);
        }
    }
    
    //Notify the tiles menu of the mouse event
    if(m_TilesMenu != NULL)
    {
        m_TilesMenu->mouseLeftClickUpEvent(positionX, positionY);
    }
    if(m_CommandMenu != NULL)
    {
        m_CommandMenu->mouseLeftClickUpEvent(positionX, positionY);
    }
}

void LevelEditor::keyUpEvent(int keyCode)
{
    if(keyCode == KEYCODE_TAB)
    {
        if(m_TilesMenu != NULL)
        {
            m_TilesMenu->isShowing() == true ? m_TilesMenu->hide() : m_TilesMenu->show();
        }
        if(m_CommandMenu != NULL)
        {
            m_CommandMenu->isShowing() == true ? m_CommandMenu->hide() : m_CommandMenu->show();
        }
    }
    else
    {
        if(m_Level != NULL)
        {
            m_Level->keyUpEvent(keyCode);
        }
    }
}

void LevelEditor::sideMenuButtonAction(UISideMenu* sideMenu, UIButton* button, int buttonIndex)
{
	// need to exit, save, etc.
	switch(buttonIndex)
	{
	case 0:
		m_Level->clear();
		break;
	case 1:
		ScreenManager::getInstance()->switchScreen(LOAD_MENU_SCREEN_NAME);
		break;
	case 2:
		ScreenManager::getInstance()->switchScreen(SAVE_MENU_SCREEN_NAME);
		break;
	case 3:
		ScreenManager::getInstance()->switchScreen(MAIN_MENU_SCREEN_NAME);
		break;
	default:
		break;
	}
}

void LevelEditor::sideMenuToggleAction(UISideMenu* sideMenu, UIToggle* toggle, int toggleIndex)
{
    if(sideMenu == m_TilesMenu)
    {
        //Un-toggled the previously selected toggle
        UIToggle* previousToggle = (UIToggle*)m_TilesMenu->getButtonForIndex(m_SelectedTileIndex);
        if(previousToggle != NULL)
        {
            previousToggle->setIsToggled(false);
        }
    
        //Set the selected tile index
        m_SelectedTileIndex = toggle->isToggled() == true ? toggleIndex : -1;
        
        //Hide the options and tiles menus
        m_TilesMenu->hide();
		m_CommandMenu->hide();
    }
}
