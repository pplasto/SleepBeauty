#ifndef SELECT_MENU_MODEL_HH
#define  SELECT_MENU_MODEL_HH

#include "cocos2d.h"
#include "GameModel_H.h"

USING_NS_CC;
class SelectMenuModelDelegate;

class BeautyData
{
public:
	const char* name;
	int difficult;
	int time;
};

class SelectMenuModel
{
private:
	SelectMenuModelDelegate *_delegate;

public:
	SelectMenuModel();
	~SelectMenuModel();

	void setDelegate(SelectMenuModelDelegate *pDelegate);
	BeautyData getBeautyData(int index);
	int getUserGainHeartNum(int index);
	void onEnter();

	void gotoBeauty(int index);
	void gotoBoss();
	void gotoUnlockBeauty( int beautyTouched );
};

class SelectMenuModelDelegate
{
public:
	virtual void noticeView( SelectMenuModel* model ) = 0;
	virtual void noticeMoveLevel( int level ) = 0;
	virtual void noticeGotoBeauty() = 0;
	virtual void noticeGotoBoss( int maskNum ) = 0;
	virtual void noticeShowLock(int beautyIndex, int needHeart) = 0;
	virtual void noticeUnlock( int beautyIndex ) = 0;
};

#endif
