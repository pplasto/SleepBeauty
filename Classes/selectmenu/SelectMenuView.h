#ifndef SELECT_MENU_VIEW__HH
#define  SELECT_MENU_VIEW__HH

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameModel_H.h"
#include "SelectMenuModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SelectMenuViewDelegate;

class SelectMenuView:public UILayer,public SelectMenuModelDelegate
{
private:
	SelectMenuViewDelegate* _delegate;
	UIWidget* _panelEvil;
	UIWidget* _panelUnlock;

public:
	SelectMenuView();
	~SelectMenuView();

	CREATE_FUNC(SelectMenuView);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void setDelegate(SelectMenuViewDelegate *pDelegate);

public:
	// delegate view interface
	virtual void noticeView(SelectMenuModel* model);
	virtual void noticeGotoBeauty();
	virtual void noticeMoveLevel( int level );
	virtual void noticeGotoBoss( int maskNum );
	virtual void noticeShowLock(int beautyIndex, int needHeart);
	virtual void noticeUnlock( int beautyIndex );

private:
	void _hidePanelEvil();
	void _hidePanelUnlock();

	void onPageViewMove(CCObject *pSender, PageViewEventType type);
	void onTouchBeautyCard(CCObject* sender , TouchEventType type);
	void onTouchButton( CCObject* sender , TouchEventType type );
	void onTouchEvil(CCObject *pSender, TouchEventType type);
	void onTouchUnlock(CCObject *pSender, TouchEventType type);
	
};

class SelectMenuViewDelegate
{
public:
	virtual void gotoMainMenu() = 0;
	virtual void gotoBeauty(int index) = 0;
	virtual void gotoBoss() = 0;
	virtual void gotoUnlockBeauty( int beautyTouched ) = 0;
};

class ScrollButton:public UIWidget
{
public:
	ScrollButton();
	virtual ~ScrollButton();

	static ScrollButton* create();
	virtual bool init();

	virtual void onPressStateChangedToNormal();
	virtual void onPressStateChangedToPressed();
	virtual bool onTouchBegan( const CCPoint &touchPoint );
};

#endif
