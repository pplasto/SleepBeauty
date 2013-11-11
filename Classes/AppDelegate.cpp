#include "AppDelegate.h"
#include "gamerun/GameRunController.h"
#include "selectmenu/SelectMenuController.h"
#include "mainmenu/MainMenuController.h"
#include "gallery/GalleryController.h"
#include "SimpleAudioEngine.h"
#include "GameGlobal.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#endif

USING_NS_CC;

void reloadSound()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( MUSIC_GAME );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( MUSIC_MENU );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_CLICK );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_FAILED );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_WIN );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_UNLOCK );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( EFFECT_STAGE_PASS );  
}

void unloadSound()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect( EFFECT_CLICK );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect( EFFECT_FAILED );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect( EFFECT_WIN );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect( EFFECT_UNLOCK );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect( EFFECT_STAGE_PASS );  
	CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
}

bool soundEnable;
bool effectEnable;
const char* currMusic;

void toggleSound()
{
	if(soundEnable)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	soundEnable = !soundEnable;
}

void toggleEffect()
{
	effectEnable = !effectEnable;
}

void playBackgroundMusic(const char* name)
{
	if(soundEnable)
	{
		if(currMusic != name)
		{
			if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			}
			currMusic = name;
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(name, true);
		}
	}
	else
	{
		if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		}
	}
}

void playEffect(const char* name)
{
	if(effectEnable)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(name);
}

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
	unloadSound();
	SelectMenuController::destory();
	MainMenuController::destory();
	GalleryController::destory();
	GameRunController::destory();
	BeautyStaticData::detroy();
	GameModel::destoryInstance();

	//// cocostdio
	//UIHelper::purgeUIHelper();
	SceneReader::sharedSceneReader()->purgeSceneReader();
	GUIReader::purgeGUIReader();
	ActionManager::purgeActionManager();
	//CCActionManager::

	//// reCache
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	CCTextureCache::purgeSharedTextureCache();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
	CCSpriteFrameCache::purgeSharedSpriteFrameCache();

	//cocos
	CCScriptEngineManager::purgeSharedManager();

	//
	DictionaryHelper::purgeDictionaryHelper();
	SceneReader::sharedSceneReader()->purgeSceneReader();
	//CCUIHELPER->purgeUIHelper();
}

bool AppDelegate::applicationDidFinishLaunching() 
{
	soundEnable = true;
	effectEnable = true;

	// SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MainMenu0.plist");
    // initialize director
    CCDirector* director = CCDirector::sharedDirector();
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();

    director->setOpenGLView(eglView);

	eglView->setDesignResolutionSize(320, 480, kResolutionShowAll);
	
    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	
    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
	MainMenuController* scene = MainMenuController::create();
	//auto scene = GameRunController::create();
	//auto scene = AtlasSplit::createScene();
    // run
    director->runWithScene(scene);

	// UIWidget* playGroundUI = cocos2d::extension::UIHelper::instance()->createWidgetFromJsonFile("PlayGroundUI.ExportJson");

	//reloadSound();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() 
{
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() 
{
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
bool ispre_exit =false;
extern "C" {
	void Java_com_goodgame_sleepBeauty_sleepBeauty_nativecloseApp(JNIEnv*  env, jobject thiz)
	{
		ispre_exit =false;
		CCDirector::sharedDirector()->end();
	}
	void Java_com_goodgame_sleepBeauty_sleepBeauty_nativecancleCloseApp(JNIEnv*  env, jobject thiz)
	{
		ispre_exit =false;
	}
}
#endif