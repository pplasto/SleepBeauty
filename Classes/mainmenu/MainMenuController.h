#ifndef MAINMENU_CONTROLLER__HH
#define  MAINMENU_CONTROLLER__HH

#include "cocos2d.h"
#include "MainMenuView.h"
#include "MainMenuModel.h"

USING_NS_CC;

class MainMenuController:public CCScene
{
public:
	static MainMenuController* create();
	static void destory();
public:
	MainMenuModel* _model;
	MainMenuView* _view;
	//CREATE_FUNC(MainMenuController);

public:
	MainMenuController();
	virtual ~MainMenuController();
	virtual bool init();
};

#endif
