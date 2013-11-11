#include "UILevelsScrollView.h"

UILevelsScrollView* UILevelsScrollView::create()
{
	UILevelsScrollView* widget = new UILevelsScrollView();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return NULL;
}

bool UILevelsScrollView::onTouchBegan( const Point &touchPoint )
{
	_isPress = true;
	_slidTime = 0;
	Point nsp = _renderer->convertToNodeSpace(touchPoint);
	_touchStartLocation = nsp.x;
	_touchMoveStartLocation = _touchStartLocation;
	this->stopAllActions();
	return true;
}

void UILevelsScrollView::onTouchMoved( const Point &touchPoint )
{
	Point nsp = _renderer->convertToNodeSpace(touchPoint);
	float offset = nsp.x - _touchMoveStartLocation;
	// 
	Point p = this->getPosition();
	p.x += offset;
	this->setPosition(p);

	_touchMoveStartLocation = nsp.x;
}

void UILevelsScrollView::onTouchEnded( const Point &touchPoint )
{
	_isPress = false;
	_slidTime = 0;
	if (_children->count() <= 0)
	{
		return;
	}
	if(_slidTime < 0.16f)
	{
		return;
	}
	Point nsp = _renderer->convertToNodeSpace(touchPoint);
	_touchEndLocation = nsp.x;
	float totalDis = 0;
	totalDis = _touchEndLocation - _touchStartLocation;
	float orSpeed = fabs(totalDis)/(_slidTime);
	Point p = getPosition();
	float targetX;
	if(orSpeed > 40)
	{
		//float currX = p.x;
		float pageNum = floor( _touchStartLocation / _pageWidth + 0.5f );
		if(totalDis < 0 ) {
			targetX = (pageNum+1)*_pageWidth;
		}else
		{
			targetX = (pageNum)*_pageWidth;
		}
	}
	else
	{
		float pageNum = floor( p.x / _pageWidth + 0.5f );
		targetX = (pageNum)*_pageWidth;
	}
	if(targetX<0)targetX = 0;
	if(targetX>_pageWidth*(_pageNum-1))targetX = _pageWidth*(_pageNum-1);
	p.x = targetX;
	this->runAction(EaseIn::create(MoveTo::create(0.5f,p),0.16f));
}

void UILevelsScrollView::update( float delta )
{
	if(_isPress)
	{
		_slidTime+=delta;
	}
}

void UILevelsScrollView::setPage( int pageNum, float pageWidth )
{
	this->_pageNum = pageNum;
	this->_pageWidth = pageWidth;
}

UILevelsScrollView::UILevelsScrollView()
	:_pageWidth(0)
	,_pageNum(0)
	,_isPress(false)
	,_slidTime(0)
	,_touchStartLocation(0)
	,_touchEndLocation(0)
	,_touchMoveStartLocation(0)
{

}

UILevelsScrollView::~UILevelsScrollView()
{

}

bool UILevelsScrollView::init()
{
	if (UIWidget::init())
	{
		setUpdateEnabled(true);
		setTouchEnabled(true);
		ignoreContentAdaptWithSize(false);
		return true;
	}
	return false;
}

//bool UILevelsScrollView::hitTest( const Point &pt )
//{
//	return UIWidget::hitTest(pt);
//}
