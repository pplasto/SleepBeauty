#include "GameRunController.h"

static GameRunController* _controllerInstance = NULL;

GameRunController::GameRunController()
	:_view(NULL)
	,_model(NULL)
{
}

GameRunController::~GameRunController()
{
	delete this->_model;
	this->_model = NULL;
}

GameRunController* GameRunController::create()
{
	if(!_controllerInstance)
	{
		_controllerInstance = new GameRunController();
		if (_controllerInstance && _controllerInstance->init())
		{
			_controllerInstance->autorelease();
			_controllerInstance->retain();
			return _controllerInstance;
		}
		CC_SAFE_DELETE(_controllerInstance);
		return NULL;
	}
	return _controllerInstance;
}


bool GameRunController::init()
{
	// controller
	this->addChild(GameRunUserInputLayer::create());
	
	// view
	this->_view = GameRunView::create();
	this->_view->setDelegate(this);
	this->addChild(this->_view);

	// model
	this->_model = new GameRunModel();
	this->_model->setDelegate(this->_view);

	return true;
}

GameRunModel* GameRunController::getModel()
{
	return this->_model;
}

GameRunView* GameRunController::getView()
{
	return this->_view;
}

void GameRunController::update( float delta )
{
	//Scene::update(delta);
	this->_model->update(delta);
}

void GameRunController::onExit()
{
	CCScene::onExit();
	this->unscheduleUpdate();
}

void GameRunController::onEnter()
{
	CCScene::onEnter();
	this->scheduleUpdate();

	// init
	this->_model->levelBeginInit();
}

/************************************************************************/
/* user controller                                                                     */
/************************************************************************/
void GameRunController::gotoGamePlay()
{
	this->_model->setState(STATE_RUN);
	//1throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoPayTime()
{
	this->_model->setPay(BUY_TIME);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoPayGuide()
{
	this->_model->setPay(BUY_GUIDE);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoPaySleep()
{
	this->_model->setPay(BUY_SLEEP);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoCloseBuy()
{
	this->_model->setState(STATE_RUN);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoBuyTime1()
{
	this->_model->_limitTimeMillis += 60;
	this->_model->setState(STATE_RUN);
	this->_model->_modelDelegate->noticeHideTip(TIME);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoBuyTimeAll()
{
	int totalTime = BeautyStaticData::getInstance()->getLimitTime(this->_model->_level) * 60;
	int relayTime = this->_model->_limitTimeMillis;
	this->_model->_limitTimeMillis = (relayTime - totalTime > 0)?(totalTime+relayTime - totalTime):totalTime;
	this->_model->_modelDelegate->noticeTime(floor(this->_model->_limitTimeMillis));
	this->_model->setState(STATE_RUN);
	this->_model->_modelDelegate->noticeHideTip(TIME);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoBuySleepAll()
{
	this->_model->_sleeping = MAX_SLEEP;
	this->_model->_modelDelegate->noticeSleep(1);
	this->_model->setState(STATE_RUN);
	this->_model->_modelDelegate->noticeHideTip(SLEEP);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoBuyGuide1()
{
	this->_model->_modelDelegate->noticeShowGuide();
	this->_model->setState(STATE_RUN);
	this->_model->_modelDelegate->noticeHideTip(GUIDE);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoBuyGuideAll()
{
	this->_model->setState(STATE_RUN);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::gotoRestart()
{
	this->_model->levelBeginInit();
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunController::destory()
{
	if(_controllerInstance != NULL)
	{
		_controllerInstance->release();
		_controllerInstance = NULL;
	}
}

//////////////////////////////////// ***** GameRunUserInputLayer ***** ////////////////////////////////////////////////
void GameRunUserInputLayer::onEnter()
{
	CCLayer::onEnter();

	this->setTouchMode(kCCTouchesOneByOne);
	this->setTouchEnabled(true);
	//this->setKeyboardEnabled(true);

	_model = static_cast<GameRunController*>(getParent())->getModel();
	_view = static_cast<GameRunController*>(getParent())->getView();
}

void GameRunUserInputLayer::onExit()
{
	this->setTouchEnabled(false);
	//this->setKeyboardEnabled(false);

	_model = NULL;
	_view = NULL;

	CCLayer::onExit();
}

bool GameRunUserInputLayer::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
	_model->touchDownAndMove(touch->getLocation());
	return true;
}

void GameRunUserInputLayer::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
	_model->touchDownAndMove(touch->getLocation());
}

void GameRunUserInputLayer::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
	_model->touchUp();
}

//void GameRunUserInputLayer::onKeyReleased( EventKeyboard::KeyCode keyCode, Event* event )
//{
//	if(keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
//	{
//		Director::getInstance()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//		exit(0);
//#endif
//	}
//}


bool GameRunUserInputLayer::init()
{
	if(CCLayer::init())
	{
		return true;
	}
	return false;
}

GameRunUserInputLayer::GameRunUserInputLayer()
{
}

GameRunUserInputLayer::~GameRunUserInputLayer()
{
}



