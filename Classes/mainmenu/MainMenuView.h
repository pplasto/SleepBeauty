#ifndef ___MAINMENU_VIEW__HH
#define  ___MAINMENU_VIEW__HH

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MainMenuModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

//class UIWidgetBatch : public UIWidget
//{
//private:
//	char* _fileImage;
//	unsigned int _capacity;
//
//public:
//	UIWidgetBatch();
//	virtual ~UIWidgetBatch();
//	bool initWithImage(const char* fileImage, unsigned int capacity);
//	static UIWidgetBatch* create(const char* fileImage, unsigned int capacity);
//
//protected:
//	virtual void initRenderer();
//	
//};

class MainMenuView:public UILayer,public MainMenuModelDelegate
{
private:
	//UIWidgetBatch* _widgetBatch;
	UIWidget* _panelSound;

public:
	MainMenuView();
	virtual ~MainMenuView();

	CREATE_FUNC(MainMenuView);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void eventBtn(CCObject *pSender, TouchEventType type);
	// delegate view interface
	virtual void noticeView(MainMenuModel* model);
	//virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void toggleEffectButton(UIButton* effectButton);
	void toggleSoundButton(UIButton* soundButton);
	void keyBackClicked();
};

#endif
