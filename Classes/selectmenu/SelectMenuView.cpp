#include "SelectMenuView.h"
#include "gamerun/GameRunController.h"

static const int CARD_TAG  = 1000;

static int currMaskNum = 0;

static int maskState = 0;

static int beautyTouched = 0;

//TODO: hide code~
#pragma region INIT
SelectMenuView::SelectMenuView(void)
{
	// loading
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("MainMenu0.plist");
	CCTextureCache::sharedTextureCache()->addImage("w12p.png");
}

SelectMenuView::~SelectMenuView(void)
{
	_panelEvil->release();
	_panelUnlock->release();
}

void SelectMenuView::onEnter()
{
	UILayer::onEnter();
	this->scheduleUpdate();
}

void SelectMenuView::onExit()
{
	this->unscheduleUpdate();
	UILayer::onExit();
}
#pragma endregion

void SelectMenuView::setDelegate( SelectMenuViewDelegate *pDelegate )
{
	this->_delegate = pDelegate;
}

bool SelectMenuView::init()
{
	char tmp[32];
	UILayer::init();

	// background
	UIImageView* background = UIImageView::create();
	background->loadTexture("level_bg.jpg",UI_TEX_TYPE_PLIST);
	background->setAnchorPoint(CCPoint(0,0));
	this->addWidget(background);

	// Beauty Card
	UIPageView* scrollView = UIPageView::create();
	scrollView->setTouchEnabled(true);
	scrollView->setSize( CCSize( GAME_WIDTH, GAME_HEIGHT * 3 / 4 ) );
	scrollView->setPosition(CCPoint( 0, GAME_HEIGHT/8 ));
	scrollView->setName("scrollView");

	UILayout* widgetBeautyCard = static_cast<UILayout*>(UIHelper::instance()->createWidgetFromJsonFile("BeautyCard.ExportJson"));

	for( int i = 0; i < MAX_BEAUTY_NUM; i++ )
	{
		Layout* layout = Layout::create();
		layout->setSize(CCSize(GAME_WIDTH, GAME_HEIGHT * 3 / 4));

		ScrollButton* button = ScrollButton::create();
		button->setSize(CCSize(160,200));

		UILayout* widget = NULL;
		if(i == 0)
		{
			widget = widgetBeautyCard;
		}
		else
		{
			widget =  static_cast<UILayout*>(widgetBeautyCard->clone());
		}

		widget->setAnchorPoint(CCPoint( 0.5f, 0.5f));
		button->setPosition(CCPoint(GAME_WIDTH/2,GAME_HEIGHT* 3 / 8));
		sprintf(tmp,"beautyCard%d",i);
		button->setName(tmp);
		//widget->setPosition( Point( GAME_WIDTH * i, 0 ) );
		button->setTag( i + CARD_TAG );

		button->addChild(widget);
		layout->addChild( button );
		scrollView->addPage( layout );
	}

	//CC_SAFE_DELETE(widgetTree);
	//CC_SAFE_DELETE(jsonDict);
	//CC_SAFE_DELETE_ARRAY(des);

	scrollView->addEventListener(this, pagevieweventselector(SelectMenuView::onPageViewMove));
	this->addWidget( scrollView );
	
	// bottom 
	UIWidget* bottomWidget = cocos2d::extension::UIHelper::instance()->createWidgetFromJsonFile("SelectUI.ExportJson");
	bottomWidget->setName("bottomUIWidget");
	this->addWidget( bottomWidget );

	// buttton
	UIButton* button_back = static_cast<UIButton*>( bottomWidget->getChildByName("Button_back") );
	UIButton* button_seal = static_cast<UIButton*>( bottomWidget->getChildByName("Button_seal") );
	button_back->addTouchEventListener(this,toucheventselector(SelectMenuView::onTouchButton));
	button_back->setTouchEnabled(true);
	button_back->setPressedActionEnabled(true);
	button_seal->addTouchEventListener(this,toucheventselector(SelectMenuView::onTouchButton));
	button_seal->setTouchEnabled(true);
	button_seal->setPressedActionEnabled(true);

	// Evil Mask panel
	_panelEvil = cocos2d::extension::UIHelper::instance()->createWidgetFromJsonFile("PanelEvil.ExportJson");
	_panelEvil->retain();

	// unlock panel
	_panelUnlock = cocos2d::extension::UIHelper::instance()->createWidgetFromJsonFile("PanelSelectLock.ExportJson");
	_panelUnlock->retain();

	return true;
}

/************************************************************************/
/* game logic update								                                            */
/************************************************************************/
void SelectMenuView::noticeView(SelectMenuModel* model)
{
#pragma region RESET_SCROLL_RECT
	char tmp[32];
	UIPageView* scrollView = static_cast<UIPageView*>(this->getWidgetByName("scrollView"));
	for( int i = 0; i < MAX_BEAUTY_NUM; i++ )
	{
		UIWidget* widget = scrollView->getChildByTag( i + CARD_TAG );
		widget->addTouchEventListener(this , toucheventselector(SelectMenuView::onTouchBeautyCard));
		//UIImageView* card_bg = static_cast<UIImageView*>(widget->getChildByName("card_bg"));
		//card_bg->setTouchEnabled(true);
		//card_bg->addTouchEventListener(this , toucheventselector(SelectMenuView::touchBeautyCard));

		UILabelBMFont* nameTxt = static_cast<UILabelBMFont*>(widget->getChildByName("txt_name"));
		UILabelBMFont* timeTxt = static_cast<UILabelBMFont*>(widget->getChildByName("txt_time"));
		UIImageView* beautyHead = static_cast<UIImageView*>(widget->getChildByName("head"));
		UIImageView* beContinue = static_cast<UIImageView*>(widget->getChildByName("moreLevel"));
		UIImageView* passRibbon = static_cast<UIImageView*>(widget->getChildByName("lock"));
		UIImageView* hearts[MAX_HEART];
		UIImageView* stars[MAX_STAR];
		for( int n = 0; n < MAX_HEART; n++ )
		{
			sprintf( tmp, "heart%d",n+1 );
			hearts[n] = static_cast<UIImageView*>(widget->getChildByName(tmp));
			hearts[n]->setVisible(false);
			sprintf( tmp, "star%d",n+1 );
			stars[n] = static_cast<UIImageView*>(widget->getChildByName(tmp));
			stars[n]->setVisible(false);
		}

		if( i < REAL_BEAUTY_NUM )
		{
			BeautyData data = model->getBeautyData(i);
			// --------------------------- Setting ---------------------------
			//if( i < REAL_BEAUTY_NUM )
			//{
				sprintf( tmp , "head_%d.png" , i);
				beautyHead->loadTexture(tmp ,UI_TEX_TYPE_PLIST);
			//}
			nameTxt->setText( data.name );
			if(data.time>9) sprintf( tmp,"%d:00", data.time );
			else sprintf( tmp,"0%d:00", data.time );
			timeTxt->setVisible(true);
			timeTxt->setText(tmp);
			for ( int n = 0; n < MAX_STAR; n++ )
			{
				if( n < data.difficult ) stars[n]->setVisible(true);
				else stars[n]->setVisible(false);
			}
			int heartNum = model->getUserGainHeartNum(i);
			for ( int n = 0; n < MAX_HEART; n++ )
			{
				if( n < heartNum ) hearts[n]->setVisible(true);
				else hearts[n]->setVisible(false);
			}
			beContinue->setVisible(false);
			if(heartNum < 0)
			{// not pass && close
				passRibbon->setVisible(true);
			}
			else
			{// pass || open
				passRibbon->setVisible(false);
			}
		}
		/////////////
		else
		{
			if(i == MAX_BEAUTY_NUM - 1 )
			{
				beContinue->setVisible(true);
				beautyHead->loadTexture("head_9.png" ,UI_TEX_TYPE_PLIST);
			}
			else
			{
				sprintf( tmp , "head_%d.png" , i);
				beautyHead->loadTexture(tmp ,UI_TEX_TYPE_PLIST);
			}
			nameTxt->setText( "???????????" );
			timeTxt->setText("--:--");
			passRibbon->setVisible(false);
		}
	}
#pragma endregion
}

void SelectMenuView::noticeGotoBeauty()
{
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1,GameRunController::create()));
}

void SelectMenuView::noticeMoveLevel( int level )
{
	UIPageView* scrollView = static_cast<UIPageView*>(this->getWidgetByName("scrollView"));
	scrollView->scrollToPage(level);
	// scrollView->getInnerContainer()->setPosition(Point( 0, level*GAME_WIDTH));
	// throw std::exception("The method or operation is not implemented.");
}

void SelectMenuView::noticeGotoBoss( int maskNum )
{
	char filename[32];
	currMaskNum = maskNum;
	this->addWidget(_panelEvil);
	//PanelEvilGain_img_bg

	_panelEvil->setVisible(true);
	_panelEvil->setTouchEnabled(true);
	_panelEvil->addTouchEventListener(this , toucheventselector(SelectMenuView::onTouchEvil));
	_panelEvil->setPosition(CCPoint(GAME_WIDTH,0));
	_panelEvil->runAction(CCMoveTo::create(0.5f,CCPoint(0,0)));

	UIImageView* panelEvilBG = static_cast<UIImageView*>(_panelEvil->getChildByName("PanelEvil_img_bg"));
	sprintf(filename,"beauty/1-boss/boss_%d.jpg",maskNum == 0?1:maskNum);
	panelEvilBG->loadTexture(filename);

	_panelEvil->getChildByName("PanelEvil_text_1")->setVisible(true);
	for(int i = 0;i<REAL_BEAUTY_NUM;i++)
	{
		sprintf(filename,"evil_mask_%d",i+1);
		if(i<maskNum)
		{
			_panelEvil->getChildByName(filename)->setVisible(true);
		}
		else
		{
			_panelEvil->getChildByName(filename)->setVisible(false);
		}
	}
	UIWidget* evil_font = _panelEvil->getChildByName("PanelEvil_font_bg");
	UIWidget* evil_circle = _panelEvil->getChildByName("evilCircle_bg");
	evil_font->setOpacity(255);
	evil_circle->setOpacity(255);
	evil_circle->setScale(1);
	maskState = 1;
}

void SelectMenuView::noticeShowLock(int beautyIndex, int needHeart)
{
	_panelUnlock->setVisible(true);
	_panelUnlock->setTouchEnabled(true);

	// setting head 
	UIImageView* head = (UIImageView*)_panelUnlock->getChildByName("panelLock_head");
	char tmp[32];
	sprintf(tmp,"head_%d.png",beautyIndex);
	head->loadTexture(tmp,UI_TEX_TYPE_PLIST);

	// setting heart number
	for(int i=0; i<MAX_HEART; i++)
	{
		sprintf(tmp,"panelLock_heart_%d",i+1);
		if(i<needHeart)
		{
			_panelUnlock->getChildByName(tmp)->setVisible(true);
		}
		else
		{
			_panelUnlock->getChildByName(tmp)->setVisible(false);
		}
	}

	// setting beauty name
	UILabelBMFont* labelName = (UILabelBMFont*) _panelUnlock->getChildByName("panelLock_txt_name");
	labelName->setText(BeautyStaticData::getInstance()->getName(beautyIndex));

	// setting unlock price
	UILabelBMFont* labelPrice = (UILabelBMFont*) _panelUnlock->getChildByName("panelLock_txt_price");
	sprintf(tmp,"$%d",BeautyStaticData::getInstance()->getRequiement(beautyIndex+1));
	labelPrice->setText(tmp);

	// animation
	UIWidget* bg = _panelUnlock->getChildByName("panelLock_bg");
	bg->setScale(0.1f);
	bg->runAction(CCScaleTo::create(0.3f,1));

	UIButton* button = (UIButton*)_panelUnlock->getChildByName("panelLock_btn_back");
	button->setTouchEnabled(true);
	button->setPressedActionEnabled(true);
	button->addTouchEventListener(this,toucheventselector(SelectMenuView::onTouchUnlock));
	button = (UIButton*)_panelUnlock->getChildByName("panelLock_btn_unlock");
	button->setTouchEnabled(true);
	button->setPressedActionEnabled(true);
	button->addTouchEventListener(this,toucheventselector(SelectMenuView::onTouchUnlock));

	this->addWidget(_panelUnlock);
}

void SelectMenuView::noticeUnlock( int beautyIndex )
{
	if( beautyIndex > 0 )
	{// unlock update
		UIWidget* widget = this->getWidgetByName("scrollView")->getChildByTag( beautyIndex + CARD_TAG );
		widget->getChildByName("lock")->setVisible(false);
	}
	UIWidget* bg = _panelUnlock->getChildByName("panelLock_bg");
	bg->stopAllActions();
	bg->runAction(CCSequence::create(CCScaleTo::create(0.3f,0.1f),CCCallFunc::create(this,callfunc_selector(SelectMenuView::_hidePanelUnlock)),NULL));
}
///////////////////////////////// Event /////////////////////////////////////////
#pragma region EVENTS
void SelectMenuView::onTouchBeautyCard( CCObject* sender , TouchEventType type )
{
	UIWidget* widget = static_cast<UIWidget*>(sender);

	if( type == TOUCH_EVENT_ENDED )
	{playEffect(EFFECT_CLICK);
		int beautyIndex = widget->getTag() - CARD_TAG;
		if( beautyIndex < REAL_BEAUTY_NUM )
		{
			beautyTouched = beautyIndex;
			_delegate->gotoBeauty(beautyIndex);
		}
		CCLOG("touch %s",widget->getName());
	}
}

void SelectMenuView::onTouchButton( CCObject* sender , TouchEventType type )
{
	UIButton* widget = static_cast<UIButton*>(sender);

	if( type == TOUCH_EVENT_ENDED )
	{playEffect(EFFECT_CLICK);
		if(strcmp(widget->getName(),"Button_back") == 0)
		{
			_delegate->gotoMainMenu();
		}
		else
		{
			_delegate->gotoBoss();
		}
		CCLOG("touch %s",widget->getName());
	}
}

void SelectMenuView::onPageViewMove( CCObject *pSender, PageViewEventType type )
{
	switch (type)
	{
	case PAGEVIEW_EVENT_TURNING:
		{
			char tmp[32];
			UIPageView* pageView = dynamic_cast<UIPageView*>(pSender);
			// pageView->getCurPageIndex()
			UIWidget* widget = this->getWidgetByName("bottomUIWidget");
			UIWidget* selectedPoint = widget->getChildByName("selected");
			sprintf(tmp,"unselect%d",pageView->getCurPageIndex() + 1);
			UIWidget* unselectedPoint = widget->getChildByName(tmp);
			selectedPoint->setPosition(unselectedPoint->getPosition());
		}
		break;
	}
}

void SelectMenuView::onTouchEvil(CCObject *pSender, TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		if(_panelEvil->getActionManager()->numberOfRunningActionsInTarget(_panelEvil) == 0)
		{
			if(maskState == 1)
			{
				// PanelEvil_font_bg && evilCircle_bg
				UIWidget* evil_font = _panelEvil->getChildByName("PanelEvil_font_bg");
				UIWidget* evil_circle = _panelEvil->getChildByName("evilCircle_bg");
				evil_font->setOpacity(255);
				evil_circle->setOpacity(255);
				evil_font->runAction(CCFadeOut::create(0.3f));
				evil_circle->runAction(CCSpawn::create(CCFadeOut::create(0.3f),CCScaleTo::create(0.3f,0.1f),NULL));
				maskState++;
			}
			else if(maskState == 2)
			{
				_panelEvil->runAction(CCSequence::create(CCMoveTo::create(0.3f,CCPoint(-GAME_WIDTH,0)),CCCallFunc::create(this,callfunc_selector(SelectMenuView::_hidePanelEvil)),NULL));
			}
		}
	}
}

void SelectMenuView::onTouchUnlock(CCObject *pSender, TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		UIButton* button = (UIButton*)pSender;
		if(strcmp(button->getName(),"panelLock_btn_back") == 0)
		{
			noticeUnlock(0);
		}
		else if(strcmp(button->getName(),"panelLock_btn_unlock") == 0)
		{
			_delegate->gotoUnlockBeauty(beautyTouched);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void SelectMenuView::_hidePanelEvil()
{
	_panelEvil->setVisible(false);
	_panelEvil->setTouchEnabled(false);
	this->removeWidget(_panelEvil);
	// release boss_1
	UIImageView* panelEvilBG = static_cast<UIImageView*>(_panelEvil->getChildByName("PanelEvil_img_bg"));
	CCSprite* evilBG = static_cast<CCSprite*>(panelEvilBG->getRenderer());
	char filename[32];
	sprintf(filename,"beauty/1-boss/boss_%d.jpg",currMaskNum == 0?1:currMaskNum);
	CCTextureCache::sharedTextureCache()->removeTextureForKey(filename);
}

void SelectMenuView::_hidePanelUnlock()
{
	_panelUnlock->setVisible(false);
	_panelUnlock->setTouchEnabled(false);
	this->removeWidget(_panelUnlock);
}
#pragma endregion

///////////////////////////ScrollButton/////////////////////////////////
#pragma region SCROLL_BUTTON
ScrollButton* ScrollButton::create()
{
	ScrollButton* widget = new ScrollButton();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return NULL;
}

ScrollButton::ScrollButton()
{
}

ScrollButton::~ScrollButton()
{
}

void ScrollButton::onPressStateChangedToNormal()
{
	m_pRenderer->stopAllActions();
	CCAction *zoomAction = CCScaleTo::create(0.1f, 1.0f);
	m_pRenderer->runAction(zoomAction);
}

void ScrollButton::onPressStateChangedToPressed()
{
	m_pRenderer->stopAllActions();
	CCAction *zoomAction = CCScaleTo::create(0.1f, 1.1f);
	m_pRenderer->runAction(zoomAction);
}

bool ScrollButton::onTouchBegan( const CCPoint &touchPoint )
{
	return UIWidget::onTouchBegan(touchPoint);
}

bool ScrollButton::init()
{
	if(UIWidget::init())
	{
		ignoreContentAdaptWithSize(false);
		setTouchEnabled(true);
		return true;
	}
	return false;
}
#pragma endregion