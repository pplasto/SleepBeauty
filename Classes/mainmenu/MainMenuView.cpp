#include "MainMenuView.h"
#include "selectmenu/SelectMenuController.h"
#include "gallery/GalleryController.h"
#include "GameGlobal.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#endif

MainMenuView::MainMenuView(void)
{

}

MainMenuView::~MainMenuView(void)
{
	if(_panelSound)_panelSound->release();
}

bool MainMenuView::init()
{
	if(UILayer::init())
	{
		UIWidget* mainmenu = cocos2d::extension::UIHelper::instance()->createWidgetFromJsonFile("MainMenu.ExportJson");

		UIButton* button = static_cast<UIButton*>(mainmenu->getChildByName("mainmenu_btn_start"));
		button->setTouchEnabled(true);
		button->setPressedActionEnabled(true);
		button->addTouchEventListener(this , toucheventselector(MainMenuView::eventBtn));

		button = static_cast<UIButton*>(mainmenu->getChildByName("mainmenu_btn_ill"));
		button->setTouchEnabled(true);
		button->setPressedActionEnabled(true);
		button->addTouchEventListener(this , toucheventselector(MainMenuView::eventBtn));
		
		button = static_cast<UIButton*>(mainmenu->getChildByName("mainmenu_btn_set"));
		button->setTouchEnabled(true);
		button->setPressedActionEnabled(true);
		button->addTouchEventListener(this , toucheventselector(MainMenuView::eventBtn));

		addWidget(mainmenu);

		_panelSound = cocos2d::extension::UIHelper::instance()->createWidgetFromJsonFile("PanelSet.ExportJson");
		_panelSound->retain();
		_panelSound->setTouchEnabled(true);
		button = static_cast<UIButton*>(_panelSound->getChildByName("Button_effect"));
		button->setTouchEnabled(true);
		button->setPressedActionEnabled(true);
		button->addTouchEventListener(this , toucheventselector(MainMenuView::eventBtn));
		button = static_cast<UIButton*>(_panelSound->getChildByName("Button_sound"));
		button->setTouchEnabled(true);
		button->setPressedActionEnabled(true);
		button->addTouchEventListener(this , toucheventselector(MainMenuView::eventBtn));
		button = static_cast<UIButton*>(_panelSound->getChildByName("Button_close"));
		button->setTouchEnabled(true);
		button->setPressedActionEnabled(true);
		button->addTouchEventListener(this , toucheventselector(MainMenuView::eventBtn));
	}
	return true;
}

void MainMenuView::onEnter()
{
	UILayer::onEnter();
	playBackgroundMusic(MUSIC_MENU);
	setKeypadEnabled(true);
	//this->setKeyboardEnabled(true);
}

void MainMenuView::onExit()
{
	UILayer::onExit();
	setKeypadEnabled(false);
	//this->setKeyboardEnabled(false);
}

/************************************************************************/
/* game logic update								                                                                */
/************************************************************************/
void MainMenuView::noticeView(MainMenuModel* model)
{

}

void MainMenuView::eventBtn(CCObject *pSender, TouchEventType type)
{
	UIButton* button = static_cast<UIButton*>(pSender);
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		if(strcmp(button->getName(),"mainmenu_btn_start") == 0)
		{
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1,SelectMenuController::create()));
		}
		else if(strcmp(button->getName(),"mainmenu_btn_ill") == 0)
		{
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1,GalleryController::create()));
		}
		else if(strcmp(button->getName(),"mainmenu_btn_set") == 0)
		{// Button_effect Button_sound Button_close PanelSet_bg PanelSet
			// icon_sound_on.png  icon_music_on.png
			addWidget(_panelSound);
		}
		else if(strcmp(button->getName(),"Button_effect") == 0)
		{
			toggleEffectButton(button);
		}
		else if(strcmp(button->getName(),"Button_sound") == 0)
		{
			toggleSoundButton(button);
		}
		else if(strcmp(button->getName(),"Button_close") == 0)
		{
			addWidget(_panelSound);
		}
	}
}

void changeUIButtonTexture(UIButton* button, const char* name)
{
	button->loadTextureNormal(name,UI_TEX_TYPE_PLIST);
	button->loadTexturePressed(name,UI_TEX_TYPE_PLIST);
}

void MainMenuView::toggleEffectButton(UIButton* effectButton)
{
	toggleEffect();
	changeUIButtonTexture(effectButton,effectEnable?"icon_music_on.png":"icon_music_off.png");
}

void MainMenuView::toggleSoundButton(UIButton* soundButton)
{
	toggleSound();
	changeUIButtonTexture(soundButton,soundEnable?"icon_sound_on.png":"icon_sound_off.png");
}

void MainMenuView::keyBackClicked()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;
	bool isHave = 
	JniHelper::getStaticMethodInfo(minfo,"com/pplasto/dreambeauty/DreamBeauty",
		"sendCloseAppMessage","()V");
	if(isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID,minfo.methodID);
	}
#endif
}

//void UIWidgetBatch::initRenderer()
//{
//	const char* fileImage = _fileImage;
//	this->m_pRenderer = CCSpriteBatchNode::create( fileImage, _capacity );
//}
//
//UIWidgetBatch* UIWidgetBatch::create(const char* fileImage, unsigned int capacity)
//{
//	UIWidgetBatch* widget = new UIWidgetBatch();
//	if (widget && widget->initWithImage(fileImage,capacity))
//	{
//		widget->autorelease();
//		return widget;
//	}
//	CC_SAFE_DELETE(widget);
//	return NULL;
//}

//UIWidgetBatch::~UIWidgetBatch()
//{
//	delete _fileImage;
//}
//
//bool UIWidgetBatch::initWithImage(const char* fileImage, unsigned int capacity)
//{
//	this->_fileImage = new char( strlen(fileImage)+1 );
//	strcpy(this->_fileImage, fileImage);
//	this->_capacity = capacity;
//	return UIWidget::init();
//}
//
//UIWidgetBatch::UIWidgetBatch()
//	:_fileImage(NULL)
//	,_capacity(0)
//{
//}
