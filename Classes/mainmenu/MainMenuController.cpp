#include "MainMenuController.h"

static MainMenuController* _instance = NULL;

MainMenuController::MainMenuController(void)
	:_view(NULL)
	,_model(NULL)
{
}
MainMenuController::~MainMenuController(void)
{
	delete this->_model;
	this->_model = NULL;
}
bool MainMenuController::init()
{
	this->_view = MainMenuView::create();
	this->addChild(this->_view);

	this->_model = new MainMenuModel();
	this->_model->setDelegate(this->_view);

	return true;
}

MainMenuController* MainMenuController::create()
{
	if(!_instance)
	{
		_instance = new MainMenuController();
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

void MainMenuController::destory()
{
	if(_instance != NULL)
	{
		_instance->release();
		_instance = NULL;
	}
}
