#ifndef SELECT_MENU_CONTROLLER__HH
#define  SELECT_MENU_CONTROLLER__HH

#include "cocos2d.h"
#include "SelectMenuModel.h"
#include "SelectMenuView.h"

USING_NS_CC;

class SelectMenuController:public CCScene,public SelectMenuViewDelegate
{
public:
	SelectMenuModel *_model;
	SelectMenuView *_view;
	//CREATE_FUNC(SelectMenuController);
	static SelectMenuController* create();
	static void destory();

public:
	SelectMenuController();
	virtual ~SelectMenuController();
	virtual bool init();
	virtual void onEnter();

	virtual void gotoMainMenu();
	virtual void gotoBeauty( int index );
	virtual void gotoBoss();
	virtual void gotoUnlockBeauty( int beautyTouched );

};

#endif
