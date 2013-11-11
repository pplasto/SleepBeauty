#ifndef ___Gallery_VIEW__HH
#define  ___Gallery_VIEW__HH

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GalleryModel.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GalleryView:public UILayer,public GalleryModelDelegate
{
private:
	UIButton* _btnBack;
	UIWidget* _uiGallery;

public:
	GalleryView();
	virtual ~GalleryView();

	CREATE_FUNC(GalleryView);
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void onTouchBtn(CCObject *pSender, TouchEventType type);
	void onTouchClip(CCObject *pSender, TouchEventType type);
	void onTouchPicture(CCObject *pSender, TouchEventType type);

	// delegate view interface
	virtual void noticeView(int* data);
	virtual void noticeShowBeautyPicture(int index);

	
};

#endif
