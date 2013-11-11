#ifndef GAMERUN_CONTROLLER__HH
#define  GAMERUN_CONTROLLER__HH

#include "cocos2d.h"
#include "GameRunController.h"
#include "GameRunModel.h"
#include "GameRunView.h"
#include "../GameModel_H.h"

USING_NS_CC;

class GameRunController:public CCScene,public GameRunViewDelegate
{
private:
	GameRunModel *_model;
	GameRunView *_view;

public:
	static GameRunController* create();
	static void destory();

	GameRunController();
	virtual ~GameRunController();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);

	GameRunModel * getModel();
	GameRunView* getView();

private:
	virtual void gotoGamePlay();
	virtual void gotoPayTime();
	virtual void gotoPayGuide();
	virtual void gotoPaySleep();
	virtual void gotoCloseBuy();
	virtual void gotoBuyTime1();
	virtual void gotoBuyTimeAll();
	virtual void gotoBuySleepAll();
	virtual void gotoBuyGuide1();
	virtual void gotoBuyGuideAll();
	virtual void gotoRestart();

};

class GameRunUserInputLayer:public CCLayer
{
public:
	GameRunUserInputLayer();
	virtual ~GameRunUserInputLayer();
private:
	GameRunModel *_model;
	GameRunView *_view;
public:
	CREATE_FUNC(GameRunUserInputLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
	virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
	virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
	//virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, CCEvent* event);
};


#endif
