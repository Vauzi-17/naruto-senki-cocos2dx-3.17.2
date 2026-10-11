#include "GameScene.h"
using namespace CocosDenshion;


USING_NS_CC;


int adResult=0;


GameScene::GameScene()
{
	_menuLayer=nullptr;
	pushMenu=nullptr;
}
GameScene::~GameScene()
{
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init()); //�Ը����ʼ��

		if(!UserDefault::getInstance()->getBoolForKey("isHavingSave")){

			UserDefault::getInstance()->setBoolForKey("isHavingSave", true);
			UserDefault::getInstance()->setBoolForKey("isBGM", true);
			UserDefault::getInstance()->setBoolForKey("isVoice", true);

			UserDefault::getInstance()->flush();
		}

		if(!UserDefault::getInstance()->getBoolForKey("isPreloadConfigured")){
			UserDefault::getInstance()->setBoolForKey("isPreload", true);
			UserDefault::getInstance()->setBoolForKey("isPreloadConfigured", true);
			UserDefault::getInstance()->flush();
		}

		introLayer=Layer::create();

		Sprite* zakume=Sprite::create("zakume.png");
		zakume->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
		ActionInterval* fadein=FadeIn::create(1.5f);
		ActionInterval* fadeout=FadeOut::create(1.5f);
        FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(GameScene::onLogo, this));
        auto seq = Sequence::create(fadein, fadeout, call, nullptr);
		introLayer->addChild(zakume);
		zakume->runAction(seq);
		this->addChild(introLayer);


		//�ڲ����ԡ�


		/*KTools* tool= KTools::create();
		tool->initTableInDB();


		tool->initColumeInDB();


		Layer* menuLayer=StartMenu::create();
		this->addChild(menuLayer);*/

	
		bRet=true;


	}while(0);

	return bRet;

}

void GameScene::onLogo(){

    auto visibleSize = Director::getInstance()->getVisibleSize();
	MenuItem* logo_btn=MenuItemSprite::create(Sprite::create("logo.png"), nullptr,
                                              CC_CALLBACK_1(GameScene::onLogoClick, this));
	logo_btn->setAnchorPoint(Vec2(0.5f,0.5f));
	
	logoMenu =Menu::create(logo_btn,nullptr);
	logoMenu->setPosition(Vec2(visibleSize.width/2,visibleSize.height-logo_btn->getContentSize().height/2));
	introLayer->addChild(logoMenu,3);


	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/intro.ogg");

    ActionInterval* fade = FadeIn::create(1.5f);
    FiniteTimeAction* call1 = CallFunc::create(CC_CALLBACK_0(GameScene::onPlayEffect2, this));
    FiniteTimeAction* call2 = CallFunc::create(CC_CALLBACK_0(GameScene::onFinish, this));

    auto seq = Sequence::create(
            fade,
            call1,
            DelayTime::create(2.0f),
            call2,
            nullptr
    );
    logo_btn->runAction(seq);

}

void GameScene::onLogoClick(Ref* /*sender*/){
    auto visibleSize = Director::getInstance()->getVisibleSize();
	Cheats+=1;
	if (Cheats==10){
		SimpleAudioEngine::getInstance()->stopAllEffects();
		logoMenu->removeFromParent();

		MenuItem* logo_btn=MenuItemSprite::create(Sprite::create("logo2.png"), nullptr,
                                                  CC_CALLBACK_1(GameScene::onLogoClick, this));
		logo_btn->setAnchorPoint(Vec2(0.5f,0.5f));
		
		logoMenu =Menu::create(logo_btn,nullptr);
		logoMenu->setPosition(Vec2(visibleSize.width/2,visibleSize.height-logo_btn->getContentSize().height/2));
		introLayer->addChild(logoMenu,3);

	}else if (Cheats>10){
		SimpleAudioEngine::getInstance()->stopAllEffects();
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/chang_btn.ogg");
		this->onFinish();
	}
}

void GameScene::onPlayEffect2(){

	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/intro2.ogg");
}


void GameScene::onFinish(){
    auto visibleSize = Director::getInstance()->getVisibleSize();

	if(!pushMenu){
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/intro_music.mp3",true);
		MenuItem* btm_btn=MenuItemSprite::create(Sprite::create("push_start.png"), nullptr,
                                                 CC_CALLBACK_1(GameScene::onPush, this));
		pushMenu=Menu::create(btm_btn,nullptr);


		pushMenu->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2-100));
		introLayer->addChild(pushMenu);
		ActionInterval* fade=FadeOut::create(0.5f);
        auto seq = Sequence::create(fade, fade->reverse(), nullptr);
		btm_btn->runAction(RepeatForever::create(seq)) ;
	}
	

	


}

void GameScene::onPush(Ref* /*sender*/){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
	
	KTools* tool= KTools::create();
	
	tool->initTableInDB();

	tool->initColumeInDB();

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Scene* menuScene =Scene::create();
	Layer* menuLayer=StartMenu::create();
	menuScene->addChild(menuLayer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,menuScene));
	
	
}








