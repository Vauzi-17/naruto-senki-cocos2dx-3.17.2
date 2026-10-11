#include "PauseLayer.h"
#include "Defines.h"
#include "GameLayer.h"
using namespace CocosDenshion;




PauseLayer::PauseLayer(void)
{
	
}


PauseLayer::~PauseLayer(void)
{
}

bool PauseLayer::init(RenderTexture* snapshoot){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());

		SimpleAudioEngine::getInstance()->stopAllEffects();
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

		Texture2D* bgTexture=snapshoot->getSprite()->getTexture();
		Sprite* bg=Sprite::createWithTexture(bgTexture);
		bg->setAnchorPoint(Vec2(0,0));
		bg->setFlipY(true);
		this->addChild(bg,0);

		Layer* blend=LayerColor::create(Color4B(0, 0, 0, 150),winSize.width,winSize.height);
		this->addChild(blend,1);

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

		Sprite* pause_title=Sprite::createWithSpriteFrameName("pause_title.png");
		pause_title->setAnchorPoint(Vec2(0,0));
		pause_title->setPosition(Vec2(2,winSize.height-pause_title->getContentSize().height-2));
		this->addChild(pause_title,3);

		MenuItem* resume_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("resume.png"),nullptr,nullptr,CC_CALLBACK_1(PauseLayer::onResume, this));
		MenuItem* btm_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("btm.png"),nullptr,nullptr,CC_CALLBACK_1(PauseLayer::onBackToMenu, this));
	
		pauseMenu =Menu::create(resume_btn,btm_btn,nullptr);
		pauseMenu->alignItemsVerticallyWithPadding(26);
		pauseMenu->setPosition(Vec2(winSize.width/2,winSize.height/2+30));
		this->addChild(pauseMenu,3);

		Sprite* surrender_text=Sprite::createWithSpriteFrameName("surrender_tips.png");
		surrender_text->setPosition(Vec2(winSize.width/2,winSize.height/2-23));
		this->addChild(surrender_text,4);

		bgm_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("bgm_on.png"),Sprite::createWithSpriteFrameName("bgm_off.png"),nullptr,CC_CALLBACK_1(PauseLayer::onBGM, this));
		voice_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("voice_on.png"),Sprite::createWithSpriteFrameName("voice_off.png"),nullptr,CC_CALLBACK_1(PauseLayer::onVoice, this));
		soundMenu =Menu::create(bgm_btn,voice_btn,nullptr);
		soundMenu->alignItemsHorizontallyWithPadding(25);
		soundMenu->setPosition(Vec2(pauseMenu->getPositionX(),pauseMenu->getPositionY()-80));
		this->addChild(soundMenu,4);

		pre_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("preload_on.png"),Sprite::createWithSpriteFrameName("preload_off.png"),nullptr,CC_CALLBACK_1(PauseLayer::onPreload, this));
		preMenu =Menu::create(pre_btn,nullptr);
		preMenu->alignItemsHorizontallyWithPadding(25);
		preMenu->setPosition(Vec2(pauseMenu->getPositionX(),preMenu->getPositionY()-84));
		this->addChild(preMenu,4);

		if(UserDefault::getInstance()->getBoolForKey("isBGM")==false){
			bgm_btn->selected();
			
		}
		if(UserDefault::getInstance()->getBoolForKey("isVoice")==false){
			voice_btn->selected();
		}

		if(UserDefault::getInstance()->getBoolForKey("isPreload")==false){
			pre_btn->selected();
		}
	

		bRet=true;
	}while(0);

	return bRet;

}
void PauseLayer::onBGM(Ref* sender){
	if(UserDefault::getInstance()->getBoolForKey("isBGM")==true){
			UserDefault::getInstance()->setBoolForKey("isBGM", false);
			bgm_btn->selected();
	}else{
		UserDefault::getInstance()->setBoolForKey("isBGM", true);
		bgm_btn->unselected();
	}
}


void PauseLayer::onVoice(Ref* sender){
	if(UserDefault::getInstance()->getBoolForKey("isVoice")==true){
		UserDefault::getInstance()->setBoolForKey("isVoice", false);
		voice_btn->selected();
	}else{
		UserDefault::getInstance()->setBoolForKey("isVoice", true);
		voice_btn->unselected();
	}
}

void PauseLayer::onPreload(Ref* sender){
	if(UserDefault::getInstance()->getBoolForKey("isPreload")==true){
		UserDefault::getInstance()->setBoolForKey("isPreload", false);
		pre_btn->selected();
	}else{
		UserDefault::getInstance()->setBoolForKey("isPreload", true);
		pre_btn->unselected();
	}
}

void PauseLayer::onResume(Ref* sender){
	
	if(UserDefault::getInstance()->getBoolForKey("isBGM")!=false){
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	
	Director::getInstance()->popScene();
	
};

void PauseLayer::onBackToMenu(Ref* sender){

	pauseMenu->setVisible(false);
	soundMenu->setVisible(false);
    SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/select.mp3");
	exitLayer=Layer::create();

	Sprite* exit_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
	exit_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));

	Sprite* comfirm_title=Sprite::createWithSpriteFrameName("confirm_title.png");
	comfirm_title->setPosition(Vec2(winSize.width/2,winSize.height/2+38));

	Sprite* surrender_text=Sprite::createWithSpriteFrameName("surrender_text.png");
	surrender_text->setPosition(Vec2(winSize.width/2,winSize.height/2+8));

	MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(PauseLayer::onLeft, this));
    MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(PauseLayer::onCancel, this));

	Menu* confirm_menu = Menu::create(yes_btn,no_btn,nullptr);
	confirm_menu->alignItemsHorizontallyWithPadding(24);
	confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-30));

	exitLayer->addChild(exit_bg,1);
	exitLayer->addChild(confirm_menu,2);
	exitLayer->addChild(comfirm_title,2);
	exitLayer->addChild(surrender_text,2);
	this->addChild(exitLayer,500);

};

void PauseLayer::onLeft(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
	_delegate->_isSurrender=true;
	Director::getInstance()->popScene();
}

void PauseLayer::onCancel(Ref* sender){

	pauseMenu->setVisible(true);
	soundMenu->setVisible(true);
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
	exitLayer->removeFromParent();

}


PauseLayer*  PauseLayer::create(RenderTexture* snapshoot){
	PauseLayer* pl=new  PauseLayer();
	if (pl && pl->init(snapshoot))
	{
		pl->autorelease();
		return pl;
	}
	else
	{
		delete pl;
		return nullptr;
	}
}


