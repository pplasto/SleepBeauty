#include "GalleryView.h"
#include "mainmenu/MainMenuController.h"
#include "GameModel_H.h"

GalleryView::GalleryView(void)
{
}

GalleryView::~GalleryView(void)
{

}

bool GalleryView::init()
{
	if(UILayer::init())
	{
		_uiGallery = (UIWidget*)GUIReader::shareReader()->widgetFromJsonFile("PanelGallery.ExportJson");

		//
		_btnBack = static_cast<UIButton*>(_uiGallery->getChildByName("Button_PanelGallery_top_1"));
		_btnBack->setTouchEnabled(true);
		_btnBack->setPressedActionEnabled(true);
		_btnBack->addTouchEventListener(this , toucheventselector(GalleryView::onTouchBtn));

		//
		char name[32];
		for(int i = 0; i<9; i++)
		{
			sprintf(name,"Gallery_clip_%d",i+1);
			UIWidget* widget = _uiGallery->getChildByName(name);
			UIImageView* head = (UIImageView*)widget->getChildByName("head_Clone");
			sprintf(name,"head_%d.png",i);
			head->loadTexture(name,UI_TEX_TYPE_PLIST);
			UIImageView* mask = UIImageView::create();
			mask->loadTexture("head_mask.png",UI_TEX_TYPE_PLIST);
			mask->setName("mask");
			mask->setOpacity(200);
			head->addChild(mask);
		}
		this->addWidget(_uiGallery);
	}
	return true;
}

void GalleryView::onEnter()
{
	UILayer::onEnter();
}

void GalleryView::onExit()
{
	UILayer::onExit();
}

/************************************************************************/
/* game  logic update								                                                       */
/************************************************************************/
int galleryData[REAL_BEAUTY_NUM];
void GalleryView::noticeView( int* data)
{
	for(int i = 0;i<REAL_BEAUTY_NUM;i++)
	{
		galleryData[i] = data[i];
	}
	//
	char name[32];
	for(int i = 0; i < REAL_BEAUTY_NUM; i++)
	{
		sprintf(name,"Gallery_clip_%d",i + 1);
		UIWidget* widget = _uiGallery->getChildByName(name);
		widget->addTouchEventListener(this,toucheventselector(GalleryView::onTouchClip));
		UIImageView* mask =( UIImageView*)widget->getChildByName("mask");
		if(galleryData[i] > 0)
		{
			mask->setVisible(false);
			widget->setTouchEnabled(true);
		}
		else
		{
			mask->setVisible(true);
			widget->setTag(false);
		}
		for(int j = 0; j<MAX_HEART;j++)
		{
			sprintf(name,"heart_%d_Clone",j + 1);
			UIWidget* heart = widget->getChildByName(name);
			if(j<galleryData[i])
			{
				heart->setVisible(true);
			}else
			{
				heart->setVisible(false);
			}
		}
	}
}

void GalleryView::onTouchBtn(CCObject *pSender, TouchEventType type)
{
	UIButton* button = static_cast<UIButton*>(pSender);
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1,MainMenuController::create()));
	}
}

void GalleryView::noticeShowBeautyPicture(int index)
{
	char name[32];
	UIImageView* beautyPic = (UIImageView*)_uiGallery->getChildByName("Gallery_beauty_picture");
	sprintf(name, "beauty/1-%d/%d-%d.jpg", index+1, index+1, BeautyStaticData::getInstance()->getMaxPic(index));
	beautyPic->loadTexture(name);

	beautyPic->setScale(0.1f);
	beautyPic->setVisible(true);
	beautyPic->runAction(CCScaleTo::create(0.3f,1));
	beautyPic->setTouchEnabled(true);
	beautyPic->addTouchEventListener(this,toucheventselector(GalleryView::onTouchPicture));
}

void GalleryView::onTouchPicture(CCObject *pSender, TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		UIImageView* beautyPic = (UIImageView*)pSender;
		beautyPic->setTouchEnabled(false);
		beautyPic->runAction(CCSequence::create(CCScaleTo::create(0.3f,0.1f),CCHide::create(),NULL));
	}
}

void GalleryView::onTouchClip(CCObject *pSender, TouchEventType type)
{
	
	if(type == TOUCH_EVENT_ENDED)
	{playEffect(EFFECT_CLICK);
		char name[32];
		UIWidget* clip = (UIWidget*)pSender;
		for(int i = 0 ;i <9;i++)
		{
			sprintf(name,"Gallery_clip_%d",i + 1);
			if(strcmp(clip->getName(),name) == 0)
			{
				noticeShowBeautyPicture(i);
				break;
			}
		}
	}
}