#include "GameRunView.h"
#include "GameUtil.h"
#include "selectmenu/SelectMenuController.h"
#include "models/BeautyStaticData.h"

// record level to release pictures
static int _currLevel;
static int _maxPic;

GameRunView::GameRunView(void)
{
}

GameRunView::~GameRunView(void)
{
	_playBeginTipUI->release();
	_playBuyGuide->release();
	_playBuySleep->release();
	_playBuyTime->release();
	_panelWin->release();
	_panelLoseTime->release();
	_panelLoseSleep->release();
	_panelEvilGain->release();
	_panelEvil->release();
	_panelNewBeauty->release();
}

bool GameRunView::init()
{
	if( UILayer::init() )
	{
		_initView();
		return true;
	}
	return false;
}

void GameRunView::onEnter()
{
	UILayer::onEnter();
	playBackgroundMusic(MUSIC_GAME);
	//CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameRunView::onExit()
{
	UILayer::onExit();
}

void GameRunView::setDelegate( GameRunViewDelegate* delegate )
{
	this->_delegate = delegate;
}

/************************************************************************/
/* init view  sprites																				*/
/************************************************************************/
void GameRunView::_initView()
{
#pragma region INIT
	/////////////////
	UILayer* playGround = this;

	// add picture
	_imageIndex = -1;
	CCSprite * backGround = CCSprite::create();
	backGround->setUserData(&_imageIndex);//set picture index
	playGround->addChild(backGround,0,PIC);

	// guide arrow
	CCSprite* guideArrow = CCSprite::createWithSpriteFrameName("guide_arrow.png"); 
	playGround->addChild(guideArrow, 0, GUIDE_ARROW);
	// guide shine
	CCSprite* guideShine = CCSprite::createWithSpriteFrameName("guide_shine.png");
	playGround->addChild(guideShine,0,GUIDE_SHINE);

	// finger picture
	CCSprite* fingerOut = CCSprite::createWithSpriteFrameName("finger_out.png");
	fingerOut->setVisible(false);
	fingerOut->setAnchorPoint(CCPoint(0.2f,-0.2f));
	playGround->addChild(fingerOut,0,FINGER_OUT);
	CCSprite* fingerIn = CCSprite::createWithSpriteFrameName("finger_in.png");
	fingerIn->setVisible(false);
	fingerIn->setAnchorPoint(CCPoint(0.5f,-0.15f));
	playGround->addChild(fingerIn,0,FINGER_IN);

	// pass level sprite
	CCSprite* stageFinishHeart = CCSprite::createWithSpriteFrameName("stage_finish_h.png"); // heart
	CCSprite* stage_finish_f = CCSprite::createWithSpriteFrameName("stage_finish_f.png"); // font finish
	stageFinishHeart->addChild(stage_finish_f);
	stageFinishHeart->setCascadeOpacityEnabled(true);
	stage_finish_f->setCascadeOpacityEnabled(true);
	stage_finish_f->setPosition(CCPoint(97/2,0));
	stageFinishHeart->setPosition(CCPoint(160,240));
	stageFinishHeart->setVisible(false);
	playGround->addChild( stageFinishHeart, 0, PASS_HEART );

	// tips sprites
	CCSprite* tip = CCSprite::create();
	tip->setVisible(false);
	playGround->addChild(tip,0,TIP);
	CCScale9Sprite* scale9Sprite = CCScale9Sprite::createWithSpriteFrameName( "tip_bg.png", CCRect(10,10,10,10) );
	tip->addChild( scale9Sprite, 0, TIP_BG );
	CCLabelBMFont* label = CCLabelBMFont::create( "" , "w12p.fnt" );//WStrToUTF8(std::wstring(L"xx")).c_str(), "fonts/w12p.fnt" );
	label->setColor(ccBLACK);
	tip->addChild(label , 0, TIP_TEXT);
	label->setPositionY(5);
#pragma endregion
	///////////////// PlayGroundUI
	this->getRootWidget()->getRenderer()->setZOrder(100);

	UIWidget* playGroundUI = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("PlayGroundUI.ExportJson");
	this->addWidget(playGroundUI);

	_buttonBack = static_cast<UIButton*>(playGroundUI->getChildByName("Button_back"));
	_buttonBack->setTouchEnabled(true);
	_buttonBack->setPressedActionEnabled(true);
	_buttonBack->addTouchEventListener(this,toucheventselector(GameRunView::onTouchUIButton));

	_buttonTime = static_cast<UIButton*>(playGroundUI->getChildByName("Button_time"));
	_buttonTime->setTouchEnabled(true);
	_buttonTime->setPressedActionEnabled(true);
	_buttonTime->addTouchEventListener(this,toucheventselector(GameRunView::onTouchUIButton));

	_buttonGuide = static_cast<UIButton*>(playGroundUI->getChildByName("Button_guide"));
	_buttonGuide->setTouchEnabled(true);
	_buttonGuide->setPressedActionEnabled(true);
	_buttonGuide->addTouchEventListener(this,toucheventselector(GameRunView::onTouchUIButton));

	_buttonSleep = static_cast<UIButton*>(playGroundUI->getChildByName("Button_sleep"));
	_buttonSleep->setTouchEnabled(true);
	_buttonSleep->setPressedActionEnabled(true);
	_buttonSleep->addTouchEventListener(this,toucheventselector(GameRunView::onTouchUIButton));

	_uiSleepBar = static_cast<UIImageView*>(playGroundUI->getChildByName("image_sleep_bar"));
	_uiTimeTxt = static_cast<UILabelBMFont*>(playGroundUI->getChildByName("txt_time"));

	_uiRedTime = playGroundUI->getChildByName("Button_time_red");
	_uiRedSleep = playGroundUI->getChildByName("image_sleep_red");

	_uiTipSleep = (UIButton*)playGroundUI->getChildByName("Button_sleep_tip");
	_uiTipSleep->setPressedActionEnabled(true);
	_uiTipSleep->addTouchEventListener(this,toucheventselector(GameRunView::onTouchTip));

	_uiTipTime =  (UIButton*)playGroundUI->getChildByName("Button_time_tip");
	_uiTipTime->setPressedActionEnabled(true);
	_uiTipTime->addTouchEventListener(this,toucheventselector(GameRunView::onTouchTip));

	_uiTipGuide =  (UIButton*)playGroundUI->getChildByName("Button_guide_tip");
	_uiTipGuide->setPressedActionEnabled(true);
	_uiTipGuide->addTouchEventListener(this,toucheventselector(GameRunView::onTouchTip));

	//////////////////////PlayBeginTip/////////////////////////
	_playBeginTipUI =  (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("PlayBeginTip.ExportJson");
	_playBeginTipUI->retain();
	/////////////////////////////// Buy ///////////////////////////////////////////
	UIButton* button;
	_playBuyGuide = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("BuyGuide.ExportJson");
	_playBuyGuide->retain();
	registerButton(_playBuyGuide,"btn_more_guide_1",onTouchPayButton);
	registerButton(_playBuyGuide,"btn_more_guide_all",onTouchPayButton);
	_playBuyGuide->getChildByName("btn_more_guide_all")->setVisible(false); // TODO 
	_playBuyGuide->getChildByName("btn_more_guide_all")->setTouchEnabled(false);// TODO 
	registerButton(_playBuyGuide,"buyguide_btn_back",onTouchPayButton);
	_playBuySleep = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("BuySleep.ExportJson");
	_playBuySleep->retain();
	registerButton(_playBuySleep,"btn_more_sleep",onTouchPayButton);
	registerButton(_playBuySleep,"buysleep_btn_back",onTouchPayButton);
	_playBuyTime = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("BuyTime.ExportJson");
	_playBuyTime->retain();
	registerButton(_playBuyTime,"btn_more_time_1",onTouchPayButton);
	registerButton(_playBuyTime,"btn_more_time_all",onTouchPayButton);
	registerButton(_playBuyTime,"buytime_btn_back",onTouchPayButton);

	//////////////////////////// win lose ///////////////////////////////////
	_panelWin = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("PanelWin.ExportJson");
	_panelWin->retain();
	registerButton(_panelWin,"win_btn_sure",onTouchResultButton);
	_panelLoseTime = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("PanelLoseTimeOver.ExportJson");
	_panelLoseTime->retain();
	registerButton(_panelLoseTime,"loseTime_btn_restart",onTouchResultButton);
	registerButton(_panelLoseTime,"loseTime_btn_back",onTouchResultButton);
	_panelLoseSleep = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("PanelLoseSleepOver.ExportJson");
	_panelLoseSleep->retain();
	registerButton(_panelLoseSleep,"loseSleep_btn_restart",onTouchResultButton);
	registerButton(_panelLoseSleep,"loseSleep_btn_back",onTouchResultButton);

	//////////////////////////////////////////////////////////////////////////
	_panelNewBeauty = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("BeautyCard.ExportJson");
	_panelNewBeauty->retain();
	_panelNewBeauty->getChildByName("img_unlock")->setVisible(true);
	_panelNewBeauty->getChildByName("img_unlock_font")->setVisible(true);
	_panelNewBeauty->getChildByName("lock")->setVisible(false);
	//_panelNewBeauty->addTouchEventListener()

	//////////////////////////////////////////////////////////////////////////
	_panelEvilGain = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("PanelEvilGain.ExportJson");
	_panelEvilGain->retain();

	//////////////////////////////////////////////////////////////////////////
	_panelEvil = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("PanelEvil.ExportJson");
	_panelEvil->retain();
	_panelEvil->getChildByName("PanelEvil_text_1")->setVisible(true);
	_panelEvil->getChildByName("evil_mask_1")->setVisible(true);
	
}

void GameRunView::_updatePicture( int picIndex)
{
	// refresh picture
	//int level = model->getCurLevel();
	//int picIndex = model->getCurPic();
	CCSprite* img = static_cast<CCSprite*>(this->getChildByTag(PIC));
	if(*(static_cast<int*>(img->getUserData())) != picIndex)
	{
		char filename[64];
		sprintf(filename, "beauty/1-%d/%d-%d.jpg", _currLevel+1, _currLevel+1, picIndex+1);
		img->initWithFile(filename);
		img->setAnchorPoint(CCPoint(0,0));
	}
}
void GameRunView::_updateTip(GameRunModel* model = NULL)
{
	CCSprite* tip = static_cast<CCSprite*>(this->getChildByTag(TIP));
	if( !model )
	{// dispear
		tip->setVisible(false);
	} 
	else
	{
		CCRect rect;
		const char* tipText = model->getCurTip(&rect);
		if (tipText)
		{
			tip->setPosition( CCPoint(rect.getMidX(), 480 - rect.getMidY()));
			CCScale9Sprite* tipSprite = static_cast<CCScale9Sprite*>(tip->getChildByTag(TIP_BG));
			tipSprite->setPreferredSize( rect.size + CCSize( 20, 20 ) );
			CCLabelBMFont* label =  static_cast<CCLabelBMFont*>(tip->getChildByTag(TIP_TEXT));
			label->setWidth( rect.size.width );
			label->setLineBreakWithoutSpace(true);
			label->setString( tipText );

			tip->setVisible(true);
		}
		else
		{
			tip->setVisible(false);
		}
	}
}
void GameRunView::_updateHotSpotPosition(GameRunModel* model = NULL)
{
	CCSprite* guide_shine =  static_cast<CCSprite*>(this->getChildByTag(GUIDE_SHINE));
	CCSprite* guide_arrow = static_cast<CCSprite*>(this->getChildByTag(GUIDE_ARROW));

	if( !model)
	{
		guide_shine->setVisible(false);
		guide_arrow->setVisible(false);
	}
	else
	{
		if(model->getCurGuide())
		{
			guide_shine->setVisible(true);
			guide_arrow->setVisible(true);
		}
		else
		{
			guide_shine->setVisible(false);
			guide_arrow->setVisible(false);
		}

		CCPoint beginPoint = model->getBeginPoint();
		CCPoint lastPoint = model->getLastPoint();

		// set shine position
		guide_shine->setPosition(beginPoint);
		if(!guide_shine->getActionByTag(0))
		{// if not set action , 
			CCAction* alphaAction = CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.5f,64),CCFadeTo::create(0.3f,255),NULL));
			//sprite->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.2f),FadeIn::create(0.2f))));	// animation
			alphaAction->setTag(0);
			guide_shine->runAction(alphaAction);	// 	const action
		}

		// set arrow action
		guide_arrow->setPosition(beginPoint);
		guide_arrow->setRotation((lastPoint-beginPoint).getAngle()*(-180)/3.1415926);
		guide_arrow->stopAllActions();
		CCMoveBy* action_moveTo = CCMoveBy::create(1,lastPoint-beginPoint);
		CCPlace* action_place = CCPlace::create(beginPoint);
		guide_arrow->runAction(CCRepeatForever::create(CCSequence::create(action_place,action_moveTo,NULL)));
	}
}
void GameRunView::_updateFingerState(GameRunModel* model)
{
	CCPoint touchedPoint;
	TOUCHMODE mode = UNTOUCH;
	if(model->getState() == STATE_RUN)
	{
		mode = model->getTouchInfo(&touchedPoint);
	}
	
	CCNode* finger_out_sprite = this->getChildByTag(FINGER_OUT);
	CCNode* finger_in_sprite = this->getChildByTag(FINGER_IN);
	switch (mode)
	{
	case UNTOUCH:
		finger_out_sprite->setVisible(false);
		finger_in_sprite->setVisible(false);
		break;
	case TOUCH_OUT:
		finger_out_sprite->setVisible(true);
		finger_in_sprite->setVisible(false);
		finger_out_sprite->setPosition(touchedPoint);
		break;
	case TOUCH_IN:
		finger_out_sprite->setVisible(false);
		finger_in_sprite->setVisible(true);
		finger_in_sprite->setPosition(touchedPoint);
		break;
	default:
		break;
	}
}

void GameRunView::showHeart()
{
	CCSprite* passHeart = static_cast<CCSprite*>(this->getChildByTag(PASS_HEART));
	passHeart->stopAllActions();
	passHeart->setVisible(true);
	passHeart->runAction(CCSequence::create(CCFadeOut::create(1),CCHide::create(),NULL));
}
void GameRunView::showWinHeart()
{
	char name[16];
	for( int i = 0; i <MAX_HEART; i++ )
	{
		sprintf( name, "win_heart_%d", i+1 );
		UIWidget* heartImage = _panelWin->getChildByName(name);
		heartImage->stopAllActions();
		if( i < _winHeart )
		{
			heartImage->setVisible(true);
			heartImage->setOpacity(0);
			heartImage->setScale(1.5f);
			heartImage->runAction(CCSequence::create(CCDelayTime::create(i*0.3f),CCSpawn::create(CCScaleTo::create(.5f,1),CCEaseIn::create(CCFadeIn::create(0.5f),0.16f),NULL),NULL));
		}
	}
}
void GameRunView::showNewBeauty()
{
	playEffect(EFFECT_UNLOCK);

	char tmp[32];
	this->addWidget(_panelNewBeauty);

	// TODO WARNING
	// set value
	
	const char* name = BeautyStaticData::getInstance()->getName(_openNewBeauty);
	int difficult = BeautyStaticData::getInstance()->getDifficult(_openNewBeauty);
	int time = BeautyStaticData::getInstance()->getLimitTime(_openNewBeauty);
	
	for( int n = 0; n < MAX_STAR; n++ )
	{
		sprintf( tmp, "star%d",n+1 );
		UIWidget* star = static_cast<UIImageView*>(_panelNewBeauty->getChildByName(tmp));
		if(n<difficult) star->setVisible(true);
		else star->setVisible(false);
	}

	UILabelBMFont* nameTxt = static_cast<UILabelBMFont*>(_panelNewBeauty->getChildByName("txt_name"));
	UILabelBMFont* timeTxt = static_cast<UILabelBMFont*>(_panelNewBeauty->getChildByName("txt_time"));
	nameTxt->setText( name );
	if(time > 9) sprintf( tmp,"%d:00", time );
	else sprintf( tmp,"0%d:00", time );
	timeTxt->setVisible(true);
	timeTxt->setText(tmp);

	UIImageView* beautyHead = static_cast<UIImageView*>(_panelNewBeauty->getChildByName("head"));
	sprintf( tmp , "head_%d.png" , _openNewBeauty);
	beautyHead->loadTexture(tmp ,UI_TEX_TYPE_PLIST);

	// animation
	_panelNewBeauty->setVisible( true );
	_panelNewBeauty->setTouchEnabled( true );
	UIWidget* bg = _panelNewBeauty->getChildByName("card_bg");
	bg->setTouchEnabled(true);
	bg->addTouchEventListener(this,toucheventselector(GameRunView::onTouchNewBeauty));
	bg->setScale( 0.1f );
	bg->runAction( CCScaleTo::create( 0.3f, 1 ) );
}

void GameRunView::showEvilMask()
{
	//throw std::exception("The method or operation is not implemented.");
	this->addWidget(_panelEvilGain);
	_panelEvilGain->setVisible(true);
	//PanelEvilGain_img_bg
	UIWidget* bg = _panelEvilGain->getChildByName("PanelEvilGain_img_bg");
	bg->setScale(0.1f);
	bg->runAction(CCEaseElasticOut::create(CCScaleTo::create( 0.5f, 1 )));
	_panelEvilGain->setTouchEnabled(true);
	_panelEvilGain->addTouchEventListener(this , toucheventselector(GameRunView::onTouchEvil));
}
void GameRunView::showEvilMaskGuide()
{
	//throw std::exception("The method or operation is not implemented.");
	this->addWidget(_panelEvil);
	//PanelEvilGain_img_bg
	_panelEvil->setVisible(true);
	_panelEvil->setTouchEnabled(true);
	_panelEvil->addTouchEventListener(this , toucheventselector(GameRunView::onTouchEvil));
	_panelEvil->setPosition(CCPoint(GAME_WIDTH,0));
	_panelEvil->runAction(CCMoveTo::create(0.5f,CCPoint(0,0)));
	UIImageView* panelEvilBG = static_cast<UIImageView*>(_panelEvil->getChildByName("PanelEvil_img_bg"));
	panelEvilBG->loadTexture("beauty/1-boss/boss_1.jpg");
}

void GameRunView::hideTip()
{
	_playBeginTipUI->setVisible(false);
	_playBeginTipUI->setTouchEnabled(false);
	this->removeWidget(_playBeginTipUI);
}
void GameRunView::hideBuyGuide()
{
	_playBuyGuide->setVisible(false);
	_playBuyGuide->setTouchEnabled(false);
	this->removeWidget(_playBuyGuide);
	//this->removeWidget(this->getWidgetByName("playGroundUI"));
}
void GameRunView::hideBuySleep()
{
	_playBuySleep->setVisible(false);
	_playBuySleep->setTouchEnabled(false);
	this->removeWidget(_playBuySleep);
}
void GameRunView::hideBuyTime()
{
	_playBuyTime->setVisible(false);
	_playBuyTime->setTouchEnabled(false);
	this->removeWidget(_playBuyTime);
}
void GameRunView::hidePanelWin()
{
	_panelWin->setVisible(false);
	_panelWin->setTouchEnabled(false);
	this->removeWidget(_panelWin);

	if( _openNewBeauty>0 )
	{
		showNewBeauty();
	}
	else if( _gainEvilMask > 0 )
	{
		showEvilMask();
	}
}
void GameRunView::hidePanelLoseTime()
{
	_panelLoseTime->setVisible(false);
	_panelLoseTime->setTouchEnabled(false);
	this->removeWidget(_panelLoseTime);
}
void GameRunView::hidePanelLoseSleep()
{
	_panelLoseSleep->setVisible(false);
	_panelLoseSleep->setTouchEnabled(false);
	this->removeWidget(_panelLoseSleep);
}
void GameRunView::hidePanelNewBeauty()
{
	_panelNewBeauty->setVisible(false);
	_panelNewBeauty->setTouchEnabled(false);
	this->removeWidget(_panelNewBeauty);

	if( _gainEvilMask > 0 )
	{
		showEvilMask();
	}
}
void GameRunView::hidePanelEvilGain()
{
	_panelEvilGain->setVisible(false);
	_panelEvilGain->setTouchEnabled(false);
	this->removeWidget(_panelEvilGain);

	if( _gainEvilMask == 1)
	{
		showEvilMaskGuide();
	}
}
void GameRunView::hidePanelEvil()
{
	_panelEvil->setVisible(false);
	_panelEvil->setTouchEnabled(false);
	this->removeWidget(_panelEvil);
	// release boss_1
	CCTextureCache::sharedTextureCache()->removeTextureForKey("beauty/1-boss/boss_1.jpg");
}
/************************************************************************/
/* receive model update																		*/
/************************************************************************/
void unableWidget(UIWidget* widget)
{
	widget->setVisible(false);
	widget->setTouchEnabled(false);
	widget->stopAllActions();
}

void GameRunView::noticeInit(int level , int maxPic)
{
	_currLevel = level;
	_maxPic = maxPic;
	_buttonGuide->setVisible(true);
	_buttonGuide->setTouchEnabled(true);
	_buttonSleep->setVisible(true);
	_buttonSleep->setTouchEnabled(true);
	_buttonTime->setVisible(true);
	_buttonTime->setTouchEnabled(true);
	_playBeginTipUI->setVisible(false);
	_playBuyGuide->setVisible(false);
	_playBuySleep->setVisible(false);
	_playBuyTime->setVisible(false);

	unableWidget(_uiRedTime);
	unableWidget(_uiRedSleep);
	unableWidget(_uiTipSleep);
	unableWidget(_uiTipTime);
	unableWidget(_uiTipGuide);

	_updatePicture(0);
	_updateTip();
	_updateHotSpotPosition();
}

void GameRunView::noticePhasePass( GameRunModel* model )
{//throw std::exception("The method or operation is not implemented.");
	playEffect(EFFECT_STAGE_PASS);
	showHeart();
}

void GameRunView::noticeTouchPoint( GameRunModel* model )
{//throw std::exception("The method or operation is not implemented.");
	_updateFingerState(model);
}

void GameRunView::noticePass( GameRunModel* model )
{
	_updatePicture(model->getCurPic());// reset picture
	_updateTip(model);// reset tip
	_updateHotSpotPosition(model);// reset hostspot position
	_updateFingerState(model);// reset finger state
}

void GameRunView::noticeWin( int gainHeart, int openBeauty, int evilMask )
{

	playEffect(EFFECT_WIN);

	_winHeart = gainHeart;
	_openNewBeauty = openBeauty;
	_gainEvilMask = evilMask;

	// dispear
	_updatePicture(_maxPic);// reset picture
	_updateTip();// dispear tip
	_updateHotSpotPosition();// dispear spot
	_buttonGuide->setVisible(false);
	_buttonGuide->setTouchEnabled(false);
	_buttonSleep->setVisible(false);
	_buttonSleep->setTouchEnabled(false);
	_buttonTime->setVisible(false);
	_buttonTime->setTouchEnabled(false);

	// show win panel
	this->addWidget(_panelWin);
	_panelWin->setVisible(true);
	_panelWin->setTouchEnabled(true);
	// animation
	char name[16];
	for(int i = 0; i <MAX_HEART; i++)
	{
		sprintf( name, "win_heart_%d", i+1 );
		_panelWin->getChildByName(name)->setVisible(false);
	}
	UIWidget* bg = _panelWin->getChildByName("PanelWin_image_bg");
	bg->setScale(0.1f);
	bg->runAction(CCSequence::create(CCScaleTo::create(0.3f,1),CCCallFunc::create(this,callfunc_selector(GameRunView::showWinHeart)),NULL));
}

void GameRunView::noticeSleep( float percent )
{
	_uiSleepBar->setScaleX(percent);
}

void GameRunView::noticeQuit()
{
	char filename[64];
	for(int i = 0; i < _maxPic; i++)
	{
		sprintf(filename, "beauty/1-%d/%d-%d.jpg", _currLevel+1, _currLevel+1, i+1);
		CCTextureCache::sharedTextureCache()->removeTextureForKey(filename);
	}
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1,SelectMenuController::create()));
}

void GameRunView::noticeLose( LOSESTATE loseState )
{
	playEffect(EFFECT_FAILED);

	switch (loseState)
	{
	case EMPTY_SLEEP:
		{
			this->addWidget(_panelLoseSleep);
			_panelLoseSleep->setVisible(true);
			_panelLoseSleep->setTouchEnabled(true);
			UIWidget* bg = _panelLoseSleep->getChildByName("PanelLose_image_bg");
			bg->setScale(0.1f);
			bg->runAction(CCScaleTo::create(0.3f,1));
		}
		break;
	case EMPTY_TIME:
		{
			this->addWidget(_panelLoseTime);
			_panelLoseTime->setVisible(true);
			_panelLoseTime->setTouchEnabled(true);
			UIWidget* bg = _panelLoseTime->getChildByName("PanelLose_image_bg");
			bg->setScale(0.1f);
			bg->runAction(CCScaleTo::create(0.3f,1));
		}
		break;
	default:
		break;
	}
}

void GameRunView::noticeBuy( BUYSTATE buyState )
{
	UIWidget* buyPanel = NULL;
	UIWidget* bg = NULL;
	UIButton* button = NULL;
	const char* bgName;
	switch (buyState)
	{
	case BUY_TIME:
		buyPanel = _playBuyTime;
		bgName = "more_time_bg";
		break;
	case BUY_SLEEP:
		buyPanel = _playBuySleep;
		bgName = "more_sleep_bg";
		break;
	case BUY_GUIDE:
		buyPanel = _playBuyGuide;
		bgName = "more_guide_bg";
		break;
	default:
		break;
	}
	//buyPanel = _playBeginTipUI;
	//bgName = "beginTip_BG";
	this->addWidget(buyPanel);
	buyPanel->setVisible(true);
	buyPanel->setTouchEnabled(true);
	bg = static_cast<UIButton*>(buyPanel->getChildByName(bgName));
	bg->setScale( 0.1f );
	bg->runAction( CCScaleTo::create(0.3f,1) );
}

void GameRunView::noticeCloseBuy()
{
	if(this->getWidgetByName("BuyGuidePanel") != NULL)
	{
		UIImageView* bg = static_cast<UIImageView*>(_playBuyGuide->getChildByName("more_guide_bg"));
		bg->stopAllActions();
		bg->runAction(CCSequence::create(CCScaleTo::create(0.3f,0.1f),CCCallFunc::create(this,callfunc_selector(GameRunView::hideBuyGuide)),NULL));
	}
	if(this->getWidgetByName("BuySleepPanel") != NULL)
	{
		UIImageView* bg = static_cast<UIImageView*>(_playBuySleep->getChildByName("more_sleep_bg"));
		bg->stopAllActions();
		bg->runAction(CCSequence::create(CCScaleTo::create(0.3f,0.1f),CCCallFunc::create(this,callfunc_selector(GameRunView::hideBuySleep)),NULL));
	}
	if(this->getWidgetByName("BuyTimePanel") != NULL)
	{
		UIImageView* bg = static_cast<UIImageView*>(_playBuyTime->getChildByName("more_time_bg"));
		bg->stopAllActions();
		bg->runAction(CCSequence::create(CCScaleTo::create(0.3f,0.1f),CCCallFunc::create(this,callfunc_selector(GameRunView::hideBuyTime)),NULL));
	}
}

void GameRunView::noticeTime( int second )
{
	char textmin[16];
	char textsec[16];
	char text[16];
	int min = static_cast<int>(floor(second/60));
	if(min<10)
		sprintf(textmin,"0%d",min);
	else
		sprintf(textmin,"%d",min);
	int sec = static_cast<int>(floor(second%60));
	if(sec<10)
		sprintf(textsec,"0%d",sec);
	else
		sprintf(textsec,"%d",sec);
	sprintf(text,"%s:%s",textmin ,textsec );
	_uiTimeTxt->setText(text);
}

void GameRunView::noticeBeginTip( int picID, const char* tip )
{
	char name[32];
	this->addWidget(_playBeginTipUI);
	_playBeginTipUI->setVisible(true);
	_playBeginTipUI->setTouchEnabled(true);
	UIImageView* bg = static_cast<UIImageView*>(_playBeginTipUI->getChildByName("beginTip_BG"));
	bg->setScale(0.1f);
	bg->runAction(CCScaleTo::create(1,1));
	UIImageView* image = static_cast<UIImageView*>(_playBeginTipUI->getChildByName("image_tip"));
	sprintf( name, "level_tip_%d.png", picID);
	image->loadTexture(name, UI_TEX_TYPE_PLIST);
	UILabelBMFont* tipTxt = static_cast<UILabelBMFont*>(_playBeginTipUI->getChildByName("txt_tip"));
	CCLabelBMFont* labelTip = static_cast<CCLabelBMFont*>(tipTxt->getVirtualRenderer());
	labelTip->setWidth( 200 );
	labelTip->setLineBreakWithoutSpace(true);
	tipTxt->setText( tip );
	UIButton* buttonSure = static_cast<UIButton*>(_playBeginTipUI->getChildByName("beginTip_button_sure"));
	buttonSure->setTouchEnabled(true);
	buttonSure->setPressedActionEnabled(true);
	buttonSure->addTouchEventListener(this , toucheventselector(GameRunView::onTouchUIButton));
}

void GameRunView::noticePlay( GameRunModel* model )
{
	_imageIndex = -1;
	_updateHotSpotPosition(model);
	_updateTip(model);
	//_updateSleep
	if(_playBeginTipUI->isVisible())
	{
		UIImageView* bg = static_cast<UIImageView*>(_playBeginTipUI->getChildByName("beginTip_BG"));
		bg->stopAllActions();
		bg->runAction(CCSequence::create(CCScaleTo::create(0.5f,0.1f),CCCallFunc::create(this,callfunc_selector(GameRunView::hideTip)),NULL));
	}
}

void GameRunView::noticeShowGuide()
{
	CCSprite* guide_shine =  static_cast<CCSprite*>(this->getChildByTag(GUIDE_SHINE));
	CCSprite* guide_arrow = static_cast<CCSprite*>(this->getChildByTag(GUIDE_ARROW));
	guide_shine->setVisible(true);
	guide_arrow->setVisible(true);
	//throw std::exception("The method or operation is not implemented.");
}

void GameRunView::noticeShowTip(TIPTYPE type)
{
	switch (type)
	{
	case SLEEP:
		_uiRedSleep->setVisible(true);
		_uiRedSleep->runAction(CCRepeatForever::create(CCSequence::create(CCFadeIn::create(0.3f),CCFadeOut::create(0.3f),NULL)));
		_uiTipSleep->setVisible(true);
		_uiTipSleep->setTouchEnabled(true);
		break;
	case TIME:
		_uiRedTime->setVisible(true);
		_uiRedTime->runAction(CCRepeatForever::create(CCSequence::create(CCFadeIn::create(0.3f),CCFadeOut::create(0.3f),NULL)));
		_uiTipTime->setVisible(true);
		_uiTipTime->setTouchEnabled(true);
		break;
	case GUIDE:
		_uiTipGuide->setVisible(true);
		_uiTipGuide->setTouchEnabled(true);
		break;
	default:
		break;
	}
}

void GameRunView::noticeHideTip( TIPTYPE type )
{
	switch (type)
	{
	case SLEEP:
		unableWidget(_uiRedSleep);
		unableWidget(_uiTipSleep);
		break;
	case TIME:
		unableWidget(_uiRedTime);
		unableWidget(_uiTipTime);
		break;
	case GUIDE:
		unableWidget(_uiTipGuide);
		break;
	default:
		break;
	}
	//throw std::exception("The method or operation is not implemented.");
}

/************************************************************************/
/* UI Event																							*/
/************************************************************************/
void GameRunView::onTouchUIButton(CCObject *pSender, TouchEventType type)
{
	UIButton* button = static_cast<UIButton*>(pSender);
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		if(strcmp(button->getName(),"Button_back") == 0)
		{
			noticeQuit();
		}
		else if(strcmp(button->getName(),"Button_time") == 0)
		{
			_delegate->gotoPayTime();
			unableWidget(_uiTipTime);
		}
		else if(strcmp(button->getName(),"Button_guide") == 0)
		{
			_delegate->gotoPayGuide();
			unableWidget(_uiTipGuide);
		}
		else if(strcmp(button->getName(),"Button_sleep") == 0)
		{
			_delegate->gotoPaySleep();
			unableWidget(_uiTipSleep);
		}
		else if(strcmp(button->getName(),"beginTip_button_sure") == 0 )
		{
			_delegate->gotoGamePlay();
		}
		
		CCLOG("touch down button %s", button->getName());
	}
}

void GameRunView::onTouchPayButton(CCObject *pSender, TouchEventType type)
{
	UIButton* button = static_cast<UIButton*>(pSender);
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		if(strcmp(button->getName(),"buyguide_btn_back") == 0 || (strcmp(button->getName(),"buysleep_btn_back") == 0 ||strcmp(button->getName(),"buytime_btn_back") == 0 ))
		{
			_delegate->gotoCloseBuy();
		}
		else if(strcmp(button->getName(),"btn_more_time_1") == 0)
		{
			_delegate->gotoBuyTime1();
		}
		else if(strcmp(button->getName(),"btn_more_time_all") == 0)
		{
			_delegate->gotoBuyTimeAll();
		}
		else if(strcmp(button->getName(),"btn_more_sleep") == 0)
		{
			_delegate->gotoBuySleepAll();
		}
		else if(strcmp(button->getName(),"btn_more_guide_1") == 0)
		{
			_delegate->gotoBuyGuide1();
		}
		else if(strcmp(button->getName(),"btn_more_guide_all") == 0)
		{
			_delegate->gotoBuyGuideAll();
		}
	}
}

void GameRunView::onTouchResultButton(CCObject *pSender, TouchEventType type)
{
	UIButton* button = static_cast<UIButton*>(pSender);
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		if(strcmp(button->getName(),"loseTime_btn_back") == 0 || strcmp(button->getName(),"loseSleep_btn_back") == 0)
		{
			this->removeWidget(_panelLoseSleep);
			this->removeWidget(_panelLoseTime);
			noticeQuit();
		}
		else if( strcmp(button->getName(),"loseTime_btn_restart") == 0 || strcmp( button->getName(), "loseSleep_btn_restart" ) == 0)
		{// restart game
			this->removeWidget(_panelLoseSleep);
			this->removeWidget(_panelLoseTime);
			_delegate->gotoRestart();
		}
		else if( strcmp(button->getName(),"win_btn_sure") == 0 )
		{
			UIImageView* bg = static_cast<UIImageView*>(_panelWin->getChildByName("PanelWin_image_bg"));
			_panelWin->stopAllActions();
			bg->stopAllActions();
			bg->runAction(CCSequence::create(CCScaleTo::create(0.3f,0.1f),CCCallFunc::create(this,callfunc_selector(GameRunView::hidePanelWin)),NULL));
		}
	}
}

void GameRunView::onTouchNewBeauty(CCObject *pSender, TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		UIWidget* widget = static_cast<UIWidget*>(pSender);
		if(widget->getActionManager()->numberOfRunningActionsInTarget(widget) == 0)
		{
			widget->runAction(CCSequence::create(CCScaleTo::create(0.3f,0.1f),CCCallFunc::create(this,callfunc_selector(GameRunView::hidePanelNewBeauty)),NULL));
		}
	}
}

void GameRunView::onTouchEvil(CCObject *pSender, TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		UIWidget* widget = static_cast<UIWidget*>(pSender);
		if(widget->getActionManager()->numberOfRunningActionsInTarget(widget) == 0)
		{
			if(widget == _panelEvilGain )
			{
				UIWidget* bg = _panelEvilGain->getChildByName("PanelEvilGain_img_bg");
				bg->runAction(CCSequence::create(CCEaseElasticIn::create(CCScaleTo::create( 0.5f, 0.1f )),CCCallFunc::create(this,callfunc_selector(GameRunView::hidePanelEvilGain)),NULL));
			}
			else if(widget == _panelEvil)
			{
				widget->runAction(CCSequence::create(CCMoveTo::create(0.3f,CCPoint(-GAME_WIDTH,0)),CCCallFunc::create(this,callfunc_selector(GameRunView::hidePanelEvil)),NULL));
			}
		}
		CCLOG("touched %s",widget->getName());
	}
}

void GameRunView::onTouchTip(CCObject *pSender, TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		UIWidget* button=(UIWidget*)pSender;
		unableWidget(button);
	}
}

