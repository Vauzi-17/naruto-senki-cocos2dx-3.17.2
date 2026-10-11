#include "cocos2d.h"
#include "StartMenu.h"
#include "MyUtils/KTools.h"
#include "SimpleAudioEngine.h"
#include "Defines.h"

class GameScene : public cocos2d::Scene {
public:
    GameScene();
    ~GameScene();

    virtual bool init();

    CREATE_FUNC(GameScene);

    cocos2d::Layer* introLayer;
CC_SYNTHESIZE(StartMenu*, _menuLayer, StartMenu);

    void onPlayEffect2();
    void onFinish();
    void onLogo();
    void onPush(cocos2d::Ref* sender);

    cocos2d::Menu* logoMenu;
    cocos2d::Menu* pushMenu;
    void onLogoClick(cocos2d::Ref* sender);
};