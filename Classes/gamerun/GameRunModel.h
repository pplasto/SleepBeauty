#ifndef GAMERUN_MODEL_HH
#define  GAMERUN_MODEL_HH

#include "cocos2d.h"
#include "../GameModel_H.h"

USING_NS_CC;

class GameRunModelDelegate;

enum TIPTYPE
{
	SLEEP,
	TIME,
	GUIDE
};

enum GAMESTATE
{
	STATE_BEGIN,
	STATE_RUN,
	STATE_WIN,
	STATE_LOSE,
	STATE_BUY
};

enum TOUCHMODE
{
	UNTOUCH,
	TOUCH_OUT,
	TOUCH_IN
};

enum BUYSTATE
{
	NO_BUY,
	BUY_TIME,
	BUY_SLEEP,
	BUY_GUIDE
};

enum LOSESTATE
{
	NO_LOSE,
	EMPTY_SLEEP,
	EMPTY_TIME
};

class GameRunModel
{
private:
	friend class GameRunController;

private:
	GAMESTATE _state;// var
	BUYSTATE _buyState;// var
	LOSESTATE _loseState;// var
	float _currentTimeMillis;// timer
	bool _isTouchedMineField;
	float _reduceSleep;
	int _level;// current level
	int _nextLevel;//
	int _index_arrHotspot;// current hotspots index
	int _index_hotspot; //  current hotspot index
	float _sleeping; // sleeping degrees
	float _limitTimeMillis; // limit time
	float _untouchTime;// untouch time
	bool _showSleepTip;
	bool _showTimeTip;
	/////////////////////////////////////////////////////////////////////////
	HotSpot* _curHotspot;// current Hotspot pointer
	GameArray<MineField*>* _currMineFields;// current mine field, just store pointer of mineField
	float _x,_y,_tx,_ty;// current hotspot begin point & end point
	bool _isTouchedHotspot;// is touched hotspot
	bool _isTouched;// is touched screen
	float _touchX,_touchY;// record touch position to use View draw
	float _beginTouchTime;// record touched time to juage timeout
	
	// view interface
	GameRunModelDelegate *_modelDelegate;

// function
private:
	void resetHotspot(); // reset hotspot
	void resetMineField();
	void nextHotspot(); // 
	void nextArr(); // 
	void setState(GAMESTATE state); // 
	void setPay(BUYSTATE buyState);
	void runningTouchMove( const CCPoint&  point);
	void runningTouchUp();

public:
	GameRunModel();
	~GameRunModel();
	void setDelegate(GameRunModelDelegate *pDelegate);
	void levelBeginInit();
	void touchDownAndMove( const CCPoint&  point);
	void touchUp();
	void update(float delta);
	int getCurLevel();
	int getCurPic();
	CCPoint getBeginPoint();
	CCPoint getLastPoint();
	TOUCHMODE getTouchInfo(CCPoint* outPoint);
	const char* getCurTip(CCRect* outRect);
	GAMESTATE getState();
	bool getCurGuide();
	bool saveScore();
	int calcOpen( int gainHeart );
	void updateTip();
};

class GameRunModelDelegate
{
public:
	// init all children
	virtual void noticeInit(int level , int maxPic) = 0;
	// pass level ,  refresh picture
	virtual void noticePass(GameRunModel* model) = 0;
	// pass part levels, show pass animation 
	virtual void noticePhasePass(GameRunModel* model) = 0;
	// update touch position
	virtual void noticeTouchPoint(GameRunModel* model) = 0;
	virtual void noticeWin( int gainHeart, int openBeauty, int evilMask ) = 0;
	virtual void noticeLose(LOSESTATE loseState) = 0;
	virtual void noticeBuy(BUYSTATE buyState) = 0;
	virtual void noticeSleep( float percent ) = 0;
	virtual void noticeQuit() = 0;
	virtual void noticeTime(int second) = 0;
	virtual void noticeBeginTip( int picID, const char* tip ) = 0;
	virtual void noticePlay( GameRunModel* model ) = 0;
	virtual void noticeCloseBuy() = 0;
	virtual void noticeShowGuide() = 0;
	virtual void noticeShowTip(TIPTYPE type) = 0;
	virtual void noticeHideTip(TIPTYPE type) = 0;
};

#endif
