#include "GalleryController.h"

static GalleryController* _instance = NULL;

GalleryController::GalleryController(void)
	:_view(NULL)
	,_model(NULL)
{
}
GalleryController::~GalleryController(void)
{
	delete this->_model;
}
bool GalleryController::init()
{
	CCScene::init();
	this->_view = GalleryView::create();
	this->addChild(this->_view);

	this->_model = new GalleryModel();
	this->_model->setDelegate(this->_view);

	return true;
}

void GalleryController::onEnter()
{
	CCScene::onEnter();
	this->_model->init();
}

GalleryController* GalleryController::create()
{
	if(!_instance)
	{
		_instance = new GalleryController();
		if (_instance && _instance->init())
		{
			_instance->autorelease();
			_instance->retain();
			return _instance;
		}
		CC_SAFE_DELETE(_instance);
		return NULL;
	}
	return _instance;
}

void GalleryController::destory()
{
	if(_instance != NULL)
	{
		_instance->release();
		_instance = NULL;
	}
}
