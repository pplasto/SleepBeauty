#ifndef UILevelsScrollView__HH
#define	 UILevelsScrollView__HH

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class UILevelsScrollView : public UIWidget
{
public:
	UILevelsScrollView();
	virtual ~UILevelsScrollView();
	static UILevelsScrollView* create();
	virtual bool init();
	virtual bool onTouchBegan(const Point &touchPoint);
	virtual void onTouchMoved(const Point &touchPoint);
	virtual void onTouchEnded(const Point &touchPoint);
	virtual void update(float delta);
	void setPage(int pageNum, float pageWidth);
	// virtual bool hitTest(const Point &pt);

protected:
	float _pageWidth;
	int _pageNum;
	bool _isPress;
	float _slidTime;
	float _touchStartLocation;
	float _touchEndLocation;
	float _touchMoveStartLocation;
};

#endif