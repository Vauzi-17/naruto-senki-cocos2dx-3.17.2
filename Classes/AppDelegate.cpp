#include "AppDelegate.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#include "platform/desktop/CCGLViewImpl-desktop.h"
#endif

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    Director* pDirector = Director::getInstance();
    auto pEGLView = pDirector->getOpenGLView();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    if (!pEGLView)
    {
        pEGLView = GLViewImpl::create("NarutoSenki");
        pEGLView->setFrameSize(1280, 720);
        pDirector->setOpenGLView(pEGLView);
    }
#endif

    pEGLView = pDirector->getOpenGLView();

    if (pEGLView)
    {
        pEGLView->setDesignResolutionSize(480, 320, ResolutionPolicy::FIXED_HEIGHT); 
        // 480 x 320
    }

    // turn off display FPS
    pDirector->setDisplayStats(false);

    // set FPS
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene
    Scene* pScene = GameScene::create();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}