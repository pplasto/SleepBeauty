#include "SelectMenuController.h"
#include "mainmenu/MainMenuController.h"

static SelectMenuController* _instance = NULL;

SelectMenuController::SelectMenuController(void)
	:_view(NULL)
	,_model(NULL)
{
}

SelectMenuController::~SelectMenuController(void)
{
	if(_model) delete this->_model;
}

SelectMenuController* SelectMenuController::create()
{
	if(!_instance)
	{
		_instance = new SelectMenuController();
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

void SelectMenuController::destory()
{
	if(_instance != NULL)
	{
		_instance->release();
		_instance = NULL;
	}
}

bool SelectMenuController::init()
{
	CCScene::init();

	this->_view = SelectMenuView::create();
	this->_view->setDelegate(this);
	this->addChild(_view);

	this->_model = new SelectMenuModel();
	this->_model->setDelegate(this->_view);

	return true;
}

void SelectMenuController::onEnter()
{
	CCScene::onEnter();
	this->_model->onEnter();
}

///////-------------------delegate-------------------//////
void SelectMenuController::gotoMainMenu()
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1,MainMenuController::create()));
	//throw std::exception("The method or operation is not implemented.");
}

void SelectMenuController::gotoBeauty( int index )
{
	_model->gotoBeauty(index);
	//throw std::exception("The method or operation is not implemented.");
}

void SelectMenuController::gotoBoss()
{
	_model->gotoBoss();
	//throw std::exception("The method or operation is not implemented.");
}

void SelectMenuController::gotoUnlockBeauty( int beautyTouched )
{
	_model->gotoUnlockBeauty(beautyTouched);
	//throw std::exception("The method or operation is not implemented.");
}






