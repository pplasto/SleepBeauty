#ifndef MAINMENU_MODEL_HH
#define  MAINMENU_MODEL_HH

#include "cocos2d.h"

USING_NS_CC;

class MainMenuModelDelegate;

class MainMenuModel
{
private:
	MainMenuModelDelegate *_modelDelegate;

public:
	MainMenuModel();
	~MainMenuModel();

	void setDelegate(MainMenuModelDelegate *pDelegate);
};

class MainMenuModelDelegate
{
public:
	virtual void noticeView(MainMenuModel* model) = 0;
};

#endif
