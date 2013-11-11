#ifndef GAMERUNVIEW_VIEW__HH
#define  GAMERUNVIEW_VIEW__HH

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameRunModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameRunViewDelegate;

class GameRunView:public UILayer,public GameRunModelDelegate
{
public:
	enum TAGS
	{
		PIC,
		GUIDE_SHINE,
		GUIDE_ARROW,
		FINGER_OUT,
		FINGER_IN,
		PASS_HEART,
		TIP,
		TIP_BG,
		TIP_TEXT
	};

private:
	GameRunViewDelegate* _delegate;

private:
	int _imageIndex;
	int _winHeart;
	int _openNewBeauty;
	int _gainEvilMask;

	UIButton* _buttonBack;
	UIButton* _buttonTime;
	UIButton* _buttonGuide;
	UIButton* _buttonSleep;

	UIImageView* _uiSleepBar;
	UILabelBMFont* _uiTimeTxt;

	UIWidget* _playBeginTipUI;
	UIWidget* _playBuyTime;
	UIWidget* _playBuyGuide;
	UIWidget* _playBuySleep;
	UIWidget* _panelWin;
	UIWidget* _panelLoseTime;
	UIWidget* _panelLoseSleep;
	UIWidget* _panelNewBeauty;
	UIWidget* _panelEvilGain;
	UIWidget* _panelEvil;

	// show
	UIWidget* _uiRedTime;
	UIWidget* _uiRedSleep;
	UIButton* _uiTipSleep;
	UIButton* _uiTipTime;
	UIButton* _uiTipGuide;

public:
	GameRunView();
	virtual ~GameRunView();

	CREATE_FUNC(GameRunView);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void setDelegate(GameRunViewDelegate* delegate);

public:// delegate view interface
	virtual void noticeInit(int level , int maxPic);
	virtual void noticePass( GameRunModel* model );
	virtual void noticePhasePass( GameRunModel* model );
	virtual void noticeTouchPoint( GameRunModel* model );
	virtual void noticeWin( int gainHeart, int openBeauty, int evilMask );
	virtual void noticeLose( LOSESTATE loseState );
	virtual void noticeBuy( BUYSTATE buyState );
	virtual void noticeSleep( float percent );
	virtual void noticeQuit();
	virtual void noticeTime( int second );
	virtual void noticeBeginTip( int picID, const char* tip );
	virtual void noticePlay( GameRunModel* model );
	virtual void noticeCloseBuy();
	virtual void noticeShowGuide();
	virtual void noticeShowTip( TIPTYPE type );
	virtual void noticeHideTip( TIPTYPE type );

private:
	void _initView();
	void _updateHotSpotPosition(GameRunModel* model);
	void _updateTip(GameRunModel* model);
	void _updatePicture( int picIndex);
	void _updateFingerState(GameRunModel* model);

	void onTouchUIButton(CCObject *pSender, TouchEventType type);
	void onTouchPayButton(CCObject *pSender, TouchEventType type);
	void onTouchResultButton(CCObject *pSender, TouchEventType type);
	void onTouchNewBeauty(CCObject *pSender, TouchEventType type);
	void onTouchEvil(CCObject *pSender, TouchEventType type);
	void onTouchTip(CCObject *pSender, TouchEventType type);

	void showHeart();
	void showWinHeart();
	void showNewBeauty();
	void showEvilMask();
	void showEvilMaskGuide();

	void hideTip();
	void hideBuyGuide();
	void hideBuySleep();
	void hideBuyTime();
	void hidePanelWin();
	void hidePanelLoseTime();
	void hidePanelLoseSleep();
	void hidePanelNewBeauty();
	void hidePanelEvilGain();
	void hidePanelEvil();
	
	void restartGame();
};

class GameRunViewDelegate
{
public:
	virtual void gotoGamePlay() = 0;
	virtual void gotoPayTime() = 0;
	virtual void gotoPayGuide() = 0;
	virtual void gotoPaySleep() = 0;
	virtual void gotoCloseBuy() = 0;
	virtual void gotoBuyTime1() = 0;
	virtual void gotoBuyTimeAll() = 0;
	virtual void gotoBuySleepAll() = 0;
	virtual void gotoBuyGuide1() = 0;
	virtual void gotoBuyGuideAll() = 0;
	virtual void gotoRestart() = 0;
};

#define registerButton(root_,buttonName_,function_) button=static_cast<UIButton*>(root_->getChildByName(buttonName_));\
	button->setTouchEnabled(true);\
	button->setPressedActionEnabled(true);\
	button->addTouchEventListener(this , toucheventselector(GameRunView::function_))

#endif
