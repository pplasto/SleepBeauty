#ifndef GALLERY_CONTROLLER__HH
#define  GALLERY_CONTROLLER__HH

#include "cocos2d.h"
#include "GalleryView.h"
#include "GalleryModel.h"

USING_NS_CC;

class GalleryController:public CCScene
{
public:
	static GalleryController* create();
	static void destory();
public:
	GalleryModel* _model;
	GalleryView* _view;
	//CREATE_FUNC(MainMenuController);

public:
	GalleryController();
	virtual ~GalleryController();
	virtual bool init();
	virtual void onEnter();
};

#endif
