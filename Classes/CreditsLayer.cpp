#include "CreditsLayer.h"
using namespace CocosDenshion;
#include "StartMenu.h"




CreditsLayer::CreditsLayer(void)
{

}


CreditsLayer::~CreditsLayer(void)
{

}

bool CreditsLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());

		Sprite* bgSprite = Sprite::create("blue_bg.png");
		bgSprite->setAnchorPoint(Vec2(0,0));

		scaleSpriteToScreen(bgSprite);
		bgSprite->setPosition(Vec2(0,0));
		this->addChild(bgSprite, -5);


		//produce the cloud
		Sprite* cloud_left=Sprite::createWithSpriteFrameName("cloud.png");
		cloud_left->setPosition(Vec2(0,15));
		cloud_left->setFlipX(true);
		cloud_left->setFlipY(true);
		cloud_left->setAnchorPoint(Vec2(0,0));
		this->addChild(cloud_left,1);

		ActionInterval* cmv1=MoveBy::create(1,Vec2(-15,0));
		Action* cseq1=RepeatForever::create(Sequence::create(cmv1, cmv1->reverse(), nullptr));
		cloud_left->runAction(cseq1);

		Sprite* cloud_right=Sprite::createWithSpriteFrameName("cloud.png");
		cloud_right->setPosition(Vec2(winSize.width-cloud_right->getContentSize().width,
			winSize.height-(cloud_right->getContentSize().height+15)));
		cloud_right->setAnchorPoint(Vec2(0,0));
		this->addChild(cloud_right,1);

		ActionInterval* cmv2=MoveBy::create(1,Vec2(15,0));
		Action* cseq2=RepeatForever::create(Sequence::create(cmv2, cmv2->reverse(), nullptr));
		cloud_right->runAction(cseq2);


		//produce the menu_bar
		Sprite* menu_bar_b = Sprite::create("menu_bar2.png");
		menu_bar_b->setAnchorPoint(Vec2(0,0));

		scaleSpriteToScreenWidth(menu_bar_b);
		this->addChild(menu_bar_b,2);


		Sprite* menu_bar_t = Sprite::create("menu_bar3.png");
		menu_bar_t->setAnchorPoint(Vec2(0,0));

		scaleSpriteToScreenWidth(menu_bar_t);
		menu_bar_t->setPosition(Vec2(0,winSize.height-menu_bar_t->getContentSize().height));
		this->addChild(menu_bar_t,2);

		Sprite* staff_title=Sprite::createWithSpriteFrameName("staff_title.png");
		staff_title->setAnchorPoint(Vec2(0,0));
		staff_title->setPosition(Vec2(2,winSize.height-staff_title->getContentSize().height-2));
		this->addChild(staff_title,3);


		Sprite* credit01=Sprite::createWithSpriteFrameName("credits01.png");
		credit01->setPosition(Vec2(winSize.width/2-20,winSize.height/2+80));
		this->addChild(credit01);

		Sprite* credit02=Sprite::createWithSpriteFrameName("credits02.png");
		credit02->setPosition(Vec2(winSize.width/2+15,winSize.height/2-60));
		this->addChild(credit02);
		if(UserDefault::getInstance()->getBoolForKey("isBGM")!=false){
			SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/credits_music.mp3",true);
		}

		bRet=true;
	}while(0);

	return bRet;

}

void CreditsLayer::onEnter(){
	Layer::onEnter();
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(CreditsLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void CreditsLayer::onExit(){
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();
}

void CreditsLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		onBackKeyClicked();
	}
}

void CreditsLayer::onBackKeyClicked(){

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
	Scene* menuScene =Scene::create();
	Layer* menuLayer=StartMenu::create();
	menuScene->addChild(menuLayer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,menuScene));

}


