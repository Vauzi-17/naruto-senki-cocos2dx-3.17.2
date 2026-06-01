#include "HudLayer.h"
#include <cstdlib>
#include "MyUtils/CCShake.h"
#include "MyUtils/KTools.h"

using namespace CocosDenshion;




MiniIcon::MiniIcon(void)
{

}


MiniIcon::~MiniIcon(void)
{

	NotificationCenter::getInstance()->removeObserver(this,"updateMap");
}


bool MiniIcon::init(const char *szImage,bool isNotification){
	bool bRet=false;
	do{

		CC_BREAK_IF(!Sprite::init());
		this->initWithSpriteFrameName(szImage);


		if(isNotification){
			this->setAnchorPoint(Vec2(0.5f,0));
			NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MiniIcon::updateMap), "updateMap", nullptr);
		}else{
			this->setAnchorPoint(Vec2(0.5f,0.5f));

		}

		bRet=true;
	}while(0);

	return bRet;

}


void MiniIcon::updateMap(Ref* sender){
	NarutoSenki::ActionManager* poster=(NarutoSenki::ActionManager*) sender;

	if(this->getCharNO()==poster->getCharNO()){
		if(poster->getActionState()==ACTION_STATE_DEAD || poster->_isVisable==false){
			this->setVisible(false);
		}else{
			this->setVisible(true);
			this->updatePosition(poster->getPosition());
		}

	}

}
void MiniIcon::updateState(){
	SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName("tower_icon3.png");
	this->setSpriteFrame(frame);
}

void MiniIcon::updatePosition(Vec2 location){
	int x=floor(location.x/32);
	int y=floor(location.y/32)*5;

	this->setPosition(Vec2(x,y));
	this->_delegate->reorderChild(this,500);

}


MiniIcon*  MiniIcon::create( const char *szImage,bool isNotification ){
	MiniIcon* mi=new  MiniIcon();
	if (mi && mi->init(szImage,isNotification))
	{
		mi->autorelease();
		return mi;
	}
	else
	{
		delete mi;
		return nullptr;
	}
}


HudLayer::HudLayer(void)
{
	_joyStick=nullptr;


	_reportListArray=nullptr;
	_buffCount=0;
	_towerIconArray=nullptr;
	reportSPCSprite=nullptr;

	_isAllButtonLocked=false;

	isOpenSQL=false;
	skill4Button=nullptr;
	skill5Button=nullptr;
	ougisLayer=nullptr;

	gear1Button=nullptr;
	gear2Button=nullptr;
	gear3Button=nullptr;
	gearMenu=nullptr;

	item1Button=nullptr;
	item2Button=nullptr;
	item3Button=nullptr;
	item4Button=nullptr;
	Kaichang=nullptr;

	bcdLabel1=nullptr;
	bcdLabel2=nullptr;
	enemy_status_bar=nullptr;
	enemy_status_hpbar=nullptr;
	enemy_status_hpMark=nullptr;
	enemyHpLabel=nullptr;
}


HudLayer::~HudLayer(void)
{

}


void HudLayer::onEnter(){
	if(_delegate->_isExiting){
		return;
	}
	Layer::onEnter();
};
void HudLayer::onExit(){
	Layer::onExit();
	if(_delegate->_isExiting){

	CC_SAFE_RELEASE(_reportListArray);
	CC_SAFE_RELEASE(_towerIconArray);
	}
};


bool HudLayer::init(){
	bool bRet=false;
	do{

		CC_BREAK_IF(!Layer::init());


		texUI = Director::getInstance()->getTextureCache()->addImage("UI.png");
		uiBatch= SpriteBatchNode::createWithTexture(texUI);






		this->addChild(uiBatch);

		bRet=true;

	}while(0);

	return bRet;



}


void HudLayer::setDelegate(GameLayer* layer){
	_delegate=layer;
};


void HudLayer::JoyStickRelease(){
	_delegate->JoyStickRelease();
}

void HudLayer::JoyStickUpdate(Vec2 direction){
	_delegate->JoyStickUpdate(direction);
}


void HudLayer::initGearButton(){
	if(gearMenu){
		gearMenu->removeFromParentAndCleanup(true);
	}

	MenuItem* gear_button=MenuItemSprite::create(Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_avator.png",_delegate->currentPlayer->getCharacter()->getCString())->getCString()),nullptr,nullptr,[this](Ref* sender){ this->gearButtonClick(sender); });
	gear_button->setAnchorPoint(Vec2(0,0));
	gearMenu=Menu::create(gear_button,nullptr);
	gearMenu->setPosition(Vec2(0,winSize.height-gear_button->getContentSize().height));
	this->addChild(gearMenu,100);
}


void HudLayer::onKaichang(){

	Vector<SpriteFrame*> tempArray;

	int i=1;

	while (i<9)
	{
		CCString* path=CCString::createWithFormat("gameStart_00%d.png",i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->spriteFrameByName(path->getCString());
		tempArray.pushBack(frame);
		i+=1;
	}



	Animation* tempAnimation= Animation::createWithSpriteFrames(tempArray, 0.1f);
	Action* tempAction=Animate::create(tempAnimation);

	Kaichang=Sprite::createWithSpriteFrameName(CCString::createWithFormat("gameStart_001.png")->getCString());

	Kaichang->setAnchorPoint(Vec2(0.5f,0.5f));
	Kaichang->setPosition(Vec2(winSize.width/2+32,winSize.height/2));
	this->addChild(Kaichang,5000);
	Kaichang->runAction(tempAction);
}

void HudLayer::initHeroInterface(){


	//init the joyStick
	_joyStick=JoyStick::create();
	_joyStick->setPosition(Vec2(28,16));
	_joyStick->setDelegate(this);
	uiBatch->addChild(_joyStick);


	//produce status bar
	status_bar=Sprite::createWithSpriteFrameName("status_bar_bg.png");
	status_bar->setAnchorPoint(Vec2(0,0));
	status_bar->setPosition(Vec2(0,winSize.height-status_bar->getContentSize().height));
	uiBatch->addChild(status_bar);

	status_hpbar=Sprite::createWithSpriteFrameName("status_hpbar.png");
	status_hpbar->setPosition(Vec2(53,winSize.height-54));
	this->addChild(status_hpbar,40);

	status_hpMark=Sprite::createWithSpriteFrameName("status_hpMark.png");
	status_hpMark->setAnchorPoint(Vec2(0,0)) ;
	status_hpMark->setPosition(Vec2(54,winSize.height-105));
	this->addChild(status_hpMark,45);


	Sprite* tmpSprite =Sprite::createWithSpriteFrameName("status_ckrbar.png");
	status_expbar=ProgressTimer::create(tmpSprite);
	status_expbar->setType(ProgressTimer::Type::RADIAL);
	status_expbar->setPercentage(0);
	status_expbar->setReverseDirection(true);
	status_expbar->setPosition(Vec2(54,winSize.height-54));
	this->addChild(status_expbar,50);

	if(_delegate->_isDuelMode){
		NarutoSenki::ActionManager* enemy=nullptr;
		Ref* enemyObject=nullptr;
		CCARRAY_FOREACH(_delegate->_CharacterArray,enemyObject){
			NarutoSenki::ActionManager* tempHero=(NarutoSenki::ActionManager*)enemyObject;
			if(strcmp(tempHero->getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0 &&
				(strcmp(tempHero->getRole()->getCString(),"Player")==0 || strcmp(tempHero->getRole()->getCString(),"Com")==0)){
				enemy=tempHero;
				break;
			}
		}

		if(enemy){
			Sprite* enemyAvator=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_avator.png",enemy->getCharacter()->getCString())->getCString());
			if(enemyAvator){
				enemyAvator->setAnchorPoint(Vec2(1,0));
				enemyAvator->setFlipX(true);
				enemyAvator->setPosition(Vec2(winSize.width,winSize.height-enemyAvator->getContentSize().height));
				this->addChild(enemyAvator,100);
			}
		}

		enemy_status_bar=Sprite::createWithSpriteFrameName("status_bar_bg.png");
		enemy_status_bar->setAnchorPoint(Vec2(1,0));
		enemy_status_bar->setFlipX(true);
		enemy_status_bar->setPosition(Vec2(winSize.width,winSize.height-enemy_status_bar->getContentSize().height));
		uiBatch->addChild(enemy_status_bar);

		enemy_status_hpbar=Sprite::createWithSpriteFrameName("status_hpbar.png");
		enemy_status_hpbar->setFlipX(true);
		enemy_status_hpbar->setPosition(Vec2(winSize.width-53,winSize.height-54));
		this->addChild(enemy_status_hpbar,40);

		enemy_status_hpMark=Sprite::createWithSpriteFrameName("status_hpMark.png");
		enemy_status_hpMark->setAnchorPoint(Vec2(1,0));
		enemy_status_hpMark->setFlipX(true);
		enemy_status_hpMark->setPosition(Vec2(winSize.width-54,winSize.height-105));
		this->addChild(enemy_status_hpMark,45);

		enemyHpLabel=LabelBMFont::create(enemy ? enemy->getHP()->getCString() : "0","Fonts/1.fnt");
		enemyHpLabel->setScale(0.35f);
		enemyHpLabel->setAnchorPoint(Vec2(1,0));
		enemyHpLabel->setPosition(Vec2(winSize.width,winSize.height-54));
		this->addChild(enemyHpLabel,5000);
	}


	//��ͣ�˵�
	MenuItem* menu_button=MenuItemSprite::create(Sprite::createWithSpriteFrameName("minimap_bg.png"),nullptr,nullptr,CC_CALLBACK_1(HudLayer::pauseButtonClick, this));
	menu_button->setAnchorPoint(Vec2(1,1));
	pauseNenu=Menu::create(menu_button,nullptr);
	if(_delegate->_isDuelMode){
		pauseNenu->setPosition(Vec2(winSize.width/2+60,winSize.height));
	}else{
		pauseNenu->setPosition(Vec2(winSize.width,winSize.height));
	}
	this->addChild(pauseNenu);


	Sprite* killIcon=Sprite::createWithSpriteFrameName("kill_icon.png");
	killIcon->setAnchorPoint(Vec2(0,1));
	if(_delegate->_isDuelMode){
		killIcon->setPosition(Vec2(winSize.width/2-60,winSize.height-46));
	}else{
		killIcon->setPosition(Vec2(winSize.width-114,winSize.height-46));
	}
	this->addChild(killIcon,5000);

	killLabel=LabelBMFont::create("0","Fonts/1.fnt");
	killLabel->setScale(0.26f);
	killLabel->setAnchorPoint(Vec2(0,1));
	killLabel->setPosition(Vec2(killIcon->getPositionX()+killIcon->getContentSize().width/2+5,winSize.height-56));
	this->addChild(killLabel,5001);

	Sprite* deadIcon=Sprite::createWithSpriteFrameName("dead_icon.png");
	deadIcon->setAnchorPoint(Vec2(0,1));
	if(_delegate->_isDuelMode){
		deadIcon->setPosition(Vec2(winSize.width/2+28,winSize.height-47));
	}else{
		deadIcon->setPosition(Vec2(winSize.width-114+26,winSize.height-47));
	}
	this->addChild(deadIcon,5000);

	deadLabel=LabelBMFont::create("0","Fonts/1.fnt");
	deadLabel->setScale(0.26f);
	deadLabel->setAnchorPoint(Vec2(0,1));
	deadLabel->setPosition(Vec2(deadIcon->getPositionX()+deadIcon->getContentSize().width/2+5,winSize.height-56));
	this->addChild(deadLabel,5001);





	KonoLabel=LabelBMFont::create("0","Fonts/blue.fnt");
	KonoLabel->setScale(0.35f);





	AkaLabel=LabelBMFont::create("0","Fonts/red.fnt");
	AkaLabel->setScale(0.35f);




	if(_delegate->_isDuelMode){
		KonoLabel->setAnchorPoint(Vec2(1,1));
		AkaLabel->setAnchorPoint(Vec2(0,1));
		KonoLabel->setPosition(Vec2(winSize.width/2-5,winSize.height-6));
		AkaLabel->setPosition(Vec2(winSize.width/2+5,winSize.height-6));
	}else if(_delegate->zhenying>0){
		KonoLabel->setAnchorPoint(Vec2(1,1));
		AkaLabel->setAnchorPoint(Vec2(0,1));
		KonoLabel->setPosition(Vec2(winSize.width-42,winSize.height-46));
		AkaLabel->setPosition(Vec2(winSize.width-32,winSize.height-46));

	}else{
		AkaLabel->setAnchorPoint(Vec2(1,1));
		KonoLabel->setAnchorPoint(Vec2(0,1));
		AkaLabel->setPosition(Vec2(winSize.width-42,winSize.height-46));
		KonoLabel->setPosition(Vec2(winSize.width-32,winSize.height-46));
	}

	this->addChild(KonoLabel,5000);
	this->addChild(AkaLabel,5000);
	LabelBMFont* hengLabel=LabelBMFont::create(":","Fonts/1.fnt");
	hengLabel->setScale(0.5f);
	hengLabel->setAnchorPoint(Vec2(0.5f,1));
	if(_delegate->_isDuelMode){
		hengLabel->setPosition(Vec2(winSize.width/2,KonoLabel->getPositionY()+2));
	}else{
		hengLabel->setPosition(Vec2(winSize.width-37,KonoLabel->getPositionY()+2));
	}
	this->addChild(hengLabel,5000);

	this->initGearButton();

	hpLabel=LabelBMFont::create(this->_delegate->currentPlayer->getHP()->getCString(),"Fonts/1.fnt");
	hpLabel->setScale(0.35f);
	hpLabel->setPosition(Vec2(0,winSize.height-54));
	hpLabel->setAnchorPoint(Vec2(0,0));

	this->addChild(hpLabel,5000);



	expLabel=LabelBMFont::create(CCString::createWithFormat("%d%%",
		this->_delegate->currentPlayer->getEXP()-((this->_delegate->currentPlayer->getLV() -1 )*500)/500)->getCString(),"Fonts/1.fnt");
	expLabel->setScale(0.35f);
	expLabel->setPosition(Vec2(94,winSize.height-54));
	expLabel->setAnchorPoint(Vec2(0.5f,0));

	this->addChild(expLabel,5000);


	gameClock=LabelBMFont::create("00:00","Fonts/1.fnt");
    gameClock->setAnchorPoint(Vec2(0.5f,0));
	gameClock->setScale(0.4f);
	gameClock->setPosition(Vec2(25,0));
	this->addChild(gameClock,5000);



	coinLabel=LabelBMFont::create("50","Fonts/arial.fnt");
	coinLabel->setAnchorPoint(Vec2(0,0));
	coinLabel->setPosition(Vec2(121,winSize.height-61));

// 		if (_delegate && _delegate->currentPlayer)
// {
//     _delegate->currentPlayer->setCoin(CCString::create("9999"));
// }

	this->addChild(coinLabel,5000);


	//init NATTACK Button
	nAttackButton = ActionButton::create("n_attack.png");
	nAttackButton ->setPosition(Vec2(winSize.width-60, 8));
	nAttackButton->setDelegate(this);
	nAttackButton->setABType(NAttack);
	uiBatch->addChild(nAttackButton);

	//init SKIll Button

	skill1Button = ActionButton::create(CCString::createWithFormat("%s_skill1.png",_delegate->currentPlayer->getCharacter()->getCString())->getCString());
	skill1Button->setPosition(Vec2(winSize.width-skill1Button->getContentSize().width-64,2));
	skill1Button->setCD(static_cast<int>(_delegate->currentPlayer->_sattackcoldDown1*1000));
	skill1Button->setDelegate(this);
	skill1Button->setABType(SKILL1);
	if(_delegate->currentPlayer->_sattack1isDouble){
		skill1Button->setDoubleSkill(true);

	};
	skill1Button->setMarkSprite("skill_freeze.png");
	uiBatch->addChild(skill1Button);


	skill2Button = ActionButton::create(CCString::createWithFormat("%s_skill2.png",_delegate->currentPlayer->getCharacter()->getCString())->getCString());
	skill2Button->setPosition(Vec2(winSize.width-95,50));
	skill2Button->setCD(static_cast<int>(_delegate->currentPlayer->_sattackcoldDown2*1000));
	skill2Button->setDelegate(this);
	skill2Button->setABType(SKILL2);
	if(_delegate->currentPlayer->_sattack2isDouble){
		skill2Button->setDoubleSkill(true);
	};
	skill2Button->setMarkSprite("skill_freeze.png");
	if(strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Kankuro")== 0 ||
		strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Chiyo")==0 ||
		strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Kiba")== 0
		){
		skill2Button->setLock();
	}
	uiBatch->addChild(skill2Button);


	skill3Button = ActionButton::create(CCString::createWithFormat("%s_skill3.png",_delegate->currentPlayer->getCharacter()->getCString())->getCString());
	skill3Button->setPosition(Vec2(winSize.width-44,8+nAttackButton->getContentSize().height+8));
	skill3Button->setCD(static_cast<int>(_delegate->currentPlayer->_sattackcoldDown3*1000));
	skill3Button->setDelegate(this);
	skill3Button->setABType(SKILL3);
	skill3Button->setMarkSprite("skill_freeze.png");
	if(_delegate->currentPlayer->_sattack3isDouble){
		skill3Button->setDoubleSkill(true);

	};
	if(strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Kiba")==0||
		strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Chiyo")==0 ||
		strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Lee")== 0
		){
		skill3Button->setLock();
	}

	uiBatch->addChild(skill3Button);



	skill4Button = ActionButton::create(CCString::createWithFormat("%s_skill4.png",_delegate->currentPlayer->getCharacter()->getCString())->getCString());
	skill4Button->setPosition(Vec2(skill1Button->getPositionX()-skill4Button->getContentSize().width-8,2));
	skill4Button->setDelegate(this);
	skill4Button->setABType(OUGIS1);
	uiBatch->addChild(skill4Button);

	skill5Button = ActionButton::create(CCString::createWithFormat("%s_skill5.png",_delegate->currentPlayer->getCharacter()->getCString())->getCString());
	skill5Button->setPosition(Vec2(skill4Button->getPositionX()-skill5Button->getContentSize().width-8,2));
	skill5Button->setDelegate(this);
	skill5Button->setABType(OUGIS2);
	uiBatch->addChild(skill5Button);

	skill4Button->setOugisMark();
	skill5Button->setOugisMark();




	//init Item
	item1Button=ActionButton::create("item1.png");
	item1Button->setPosition(Vec2(8, skill3Button->getPositionY()+skill3Button->getContentSize().height));
	if(_delegate->_isHardCoreGame){
		item1Button->setCD(5000);
	}else{
		item1Button->setCD(3000);
	}

	item1Button->setDelegate(this);
	item1Button->setABType(Item1);
	item1Button->_cost="50";
	item1Button->setMarkSprite("skill_freeze.png");
	uiBatch->addChild(item1Button);


	//init Item
	item2Button=ActionButton::create("item2.png");
	item2Button->setPosition(Vec2(winSize.width-44, skill3Button->getPositionY()+skill3Button->getContentSize().height+8));
	item2Button->setCD(15000);
	item2Button->setDelegate(this);
	item2Button->setABType(GearItem);
	item2Button->_gearType=None;
	item2Button->_isLock=true;
	item2Button->setMarkSprite("skill_freeze.png");
	uiBatch->addChild(item2Button);
	item2Button->setVisible(false);

	item3Button=ActionButton::create("item3.png");
	if(skill5Button){
		item3Button->setPosition(Vec2( skill5Button->getPositionX()-skill5Button->getContentSize().width-8,2));
	}else if(skill4Button){
		item3Button->setPosition(Vec2( skill4Button->getPositionX()-skill4Button->getContentSize().width-8,2));
	}else{
		item3Button->setPosition(Vec2( skill1Button->getPositionX()-skill1Button->getContentSize().width-8,2));
	}
	item3Button->setCD(15000);
	item3Button->setDelegate(this);
	item3Button->setABType(GearItem);
	item3Button->_gearType=None;
	item3Button->_isLock=true;
	item3Button->setMarkSprite("skill_freeze.png");
	uiBatch->addChild(item3Button);
	item3Button->setVisible(false);

	item4Button=ActionButton::create("item4.png");
	item4Button->setPosition(Vec2( skill2Button->getPositionX()-skill2Button->getContentSize().width-8,skill2Button->getPositionY()));
	item4Button->setCD(15000);
	item4Button->setDelegate(this);
	item4Button->setABType(GearItem);
	item4Button->_gearType=None;
	item4Button->setMarkSprite("skill_freeze.png");
	item4Button->_isLock=true;
	uiBatch->addChild(item4Button);
	item4Button->setVisible(false);

	//init gear
	this->updateGears();

	//��ʼ�������ͼ
	miniLayer=Layer::create();
	miniLayer->setAnchorPoint(Vec2(0,0));
	if(_delegate->_isDuelMode){
		miniLayer->setPosition(Vec2(winSize.width/2-52,winSize.height-38));
	}else{
		miniLayer->setPosition(Vec2(winSize.width-112,winSize.height-38));
	}

	//���ӵ�ͼicon
	Ref* pObject;
	CCARRAY_FOREACH(_delegate->_TowerArray,pObject){
		NarutoSenki::ActionManager* player=(NarutoSenki::ActionManager*) pObject;
		const char* path="";
		MiniIcon* mi;

		if(strcmp(player->getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())==0){
			path="tower_icon1.png";
		}else{
			path="tower_icon2.png";
		}
		mi=MiniIcon::create(path,false);
		mi->_delegate=miniLayer;
		miniLayer->addChild(mi);
		mi->updatePosition(player->getSpawnPoint());
		mi->setCharNO(player->getCharNO());

		if(!_towerIconArray){
			_towerIconArray=CCArray::create();
			_towerIconArray->retain();
		}
		_towerIconArray->addObject(mi);

	}


	//�������icon

	CCARRAY_FOREACH(_delegate->_CharacterArray,pObject){
		NarutoSenki::ActionManager* player=(NarutoSenki::ActionManager*) pObject;
		if(strcmp(player->getRole()->getCString(),"Player")==0 ||
			strcmp(player->getRole()->getCString(),"Com")==0
			){
				MiniIcon* mi;
				const char* path="";
				if(strcmp(player->getRole()->getCString(),"Player")==0){
					path="player_icon.png";
				}else if(strcmp(player->getRole()->getCString(),"Com")==0){
					if(strcmp(player->getCharacter()->getCString(),"Han")==0 ||
						strcmp(player->getCharacter()->getCString(),"Roshi")==0){
							path="guardian_icon.png";
					}else{
						if(strcmp(player->getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())==0){
							path="com_icon.png";

						}else{
							path="enemy_icon.png";
						}
					}


				}

				mi=MiniIcon::create(path,true);
				mi->_delegate=miniLayer;
				miniLayer->addChild(mi);
				mi->updatePosition(player->getSpawnPoint());
				mi->setCharNO(player->getCharNO());

		}

	}

	this->addChild(miniLayer,500);


}

void HudLayer::addMapIcon(){

	Ref* pObject;
	CCARRAY_FOREACH(_delegate->_CharacterArray,pObject){
		NarutoSenki::ActionManager* player=(NarutoSenki::ActionManager*) pObject;
		if(strcmp(player->getCharacter()->getCString(),"Han")==0 ||
			strcmp(player->getCharacter()->getCString(),"Roshi")==0){
				MiniIcon* mi;
				const char* path="guardian_icon.png";
				mi=MiniIcon::create(path,true);
				mi->_delegate=miniLayer;
				miniLayer->addChild(mi);
				mi->updatePosition(player->getSpawnPoint());
				mi->setCharNO(player->getCharNO());

		}
	}
}

void HudLayer::updateGears(){

	if(!gear1Button && !gear2Button && !gear3Button){

		gear1Button=ActionButton::create("gearbg.png");
		gear1Button->setPosition(Vec2(0,winSize.height-92));
		gear1Button->setDelegate(this);
		gear1Button->setCD(15000);
		gear1Button->setABType(GearBtn);

		gear1Button->setMarkSprite("gear_freeze.png");
		gear1Button->_gearType=None;
		this->addChild(gear1Button,200);

		gear2Button=ActionButton::create("gearbg.png");
		gear2Button->setPosition(Vec2(35,winSize.height-112));
		gear2Button->setDelegate(this);
		gear2Button->setCD(15000);
		gear2Button->_gearType=None;
			gear2Button->setABType(GearBtn);
		gear2Button->setMarkSprite("gear_freeze.png");

		this->addChild(gear2Button,200);


		gear3Button=ActionButton::create("gearbg.png");
		gear3Button->setPosition(Vec2(70,winSize.height-92));
		gear3Button->setCD(15000);
		gear3Button->setDelegate(this);
		gear3Button->_gearType=None;

		gear3Button->setABType(GearBtn);
		gear3Button->setMarkSprite("gear_freeze.png");
		this->addChild(gear3Button,200);

		return;
	}


	if(_delegate->currentPlayer->getGearArray() && _delegate->currentPlayer->getGearArray()->count()>0){
		Ref* pObject;
		int i=0;
		CCARRAY_FOREACH(_delegate->currentPlayer->getGearArray(),pObject){
			CCString* tmpGear=(CCString*) pObject;
			if(tmpGear->intValue()!=gear1Button->_gearType && i==0){
				gear1Button->setGearType(tmpGear->intValue());
			}else if(tmpGear->intValue()!=gear2Button->_gearType && i==1){
				gear2Button->setGearType(tmpGear->intValue());
			}else if(tmpGear->intValue()!=gear3Button->_gearType && i==2){
				gear3Button->setGearType(tmpGear->intValue());
			}

			if(tmpGear->intValue()==6 && this->getItem2Button()->_gearType==None){
				this->getItem2Button()->_gearType=gearType(tmpGear->intValue());
				this->getItem2Button()->setVisible(true);
				this->getItem2Button()->_isLock=false;
			}else if(tmpGear->intValue()==0 && this->getItem3Button()->_gearType==None){


				this->getItem3Button()->_gearType=gearType(tmpGear->intValue());
				this->getItem3Button()->_isLock=false;
				this->getItem3Button()->setVisible(true);

			}else if(tmpGear->intValue()==3 && this->getItem4Button()->_gearType==None){

				this->getItem4Button()->setVisible(true);
				this->getItem4Button()->_gearType=gearType(tmpGear->intValue());
				this->getItem4Button()->_isLock=false;
			}




			i++;

		}
	}



}


void HudLayer::setHPLose(float percent){
	CCRotateTo* ra=CCRotateTo::create(0.2f,-((1-percent)*180),-((1-percent)*180));
	status_hpbar->runAction(ra);

	int hp=std::atoi(_delegate->currentPlayer->getHP()->getCString());
	hpLabel->setString(CCString::createWithFormat("%d",int(hp))->getCString());

}

void HudLayer::setEnemyHPLose(float percent){
	if(!enemy_status_hpbar){
		return;
	}
	CCRotateTo* ra=CCRotateTo::create(0.2f,((1-percent)*180),((1-percent)*180));
	enemy_status_hpbar->runAction(ra);

	Ref* pObject=nullptr;
	CCARRAY_FOREACH(_delegate->_CharacterArray,pObject){
		NarutoSenki::ActionManager* tempHero=(NarutoSenki::ActionManager*)pObject;
		if(strcmp(tempHero->getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0 &&
			(strcmp(tempHero->getRole()->getCString(),"Player")==0 || strcmp(tempHero->getRole()->getCString(),"Com")==0)){
			if(enemyHpLabel){
				enemyHpLabel->setString(CCString::createWithFormat("%d",std::atoi(tempHero->getHP()->getCString()))->getCString());
			}
			return;
		}
	}
}

void HudLayer::setCKRLose(bool isCRK2){

	if(!isCRK2){

		if(skill4Button){

			if(skill4Button->lockLabel1){
				skill4Button->ougismarkSprite->setVisible(false);
				skill4Button->_isMarkVisable=false;
				ActionInterval*  su=ScaleBy::create(0.2f,1.1f);
				FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(ActionButton::setProgressMark, skill4Button));
				Action* seq=Sequence::create(su,su->reverse(),call,nullptr);
				skill4Button->runAction(seq);
			}else{
				skill4Button->updateProgressMark();
			}

		}

	}else{

		if(skill5Button){

			if(skill5Button->lockLabel1){
				skill5Button->ougismarkSprite->setVisible(false);
				skill5Button->_isMarkVisable=false;
				ActionInterval*  su=ScaleBy::create(0.2f,1.1f);
				FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(ActionButton::setProgressMark, skill5Button));
				Action* seq=Sequence::create(su,su->reverse(),call,nullptr);
				skill5Button->runAction(seq);
			}else{
				skill5Button->updateProgressMark();
			}

		}
	}


}

void HudLayer::setEXPLose(float percent){


	int exp=_delegate->currentPlayer->getEXP();
	int lvExp=(_delegate->currentPlayer->getLV() -1 )*500;
	float Percent=(exp-lvExp)/500.0*100;
	if (Percent>100){
		Percent=100;
	}
	//CCRotateTo* ra=CCRotateTo::create(0.2f,((1-Percent/100)*90),((1-Percent/100)*90));
	//status_expbar->runAction(ra);
	status_expbar->setPercentage((1+Percent/100)*50);


	if(exp>=2500){
		status_expbar->setPercentage(100);
		expLabel->setString("Max");
	}else{
		expLabel->setString(CCString::createWithFormat("%d%%",int(Percent))->getCString());
	}


}

void HudLayer::setTowerState(unsigned int charNO){
	Ref* pObject;
	CCARRAY_FOREACH(_towerIconArray,pObject){
		MiniIcon* mi=(MiniIcon *) pObject;
		if(mi->getCharNO()==charNO){
			mi->updateState();
		}

	}
}


void HudLayer::setCoin(const char* value){

	std::string cl=coinLabel->getString();
	int tempCoin=std::atoi(cl);
	tempCoin+=std::atoi(value);
	CCString* realCoin=CCString::createWithFormat("%d",tempCoin);
	coinLabel->setString(realCoin->getCString());

}

bool HudLayer::offCoin(const char* value){

	std::string cl=coinLabel->getString();
	int tempCoin=std::atoi(cl);
	if(tempCoin-std::atoi(value)>=0){
		tempCoin-=std::atoi(value);
		CCString* realCoin=CCString::createWithFormat("%d",tempCoin);
		coinLabel->setString(realCoin->getCString());
		_delegate->currentPlayer->minusCoin(std::atoi(value));
		return true;
	}else {
		return false;
	}

}


void HudLayer::setReport(const char* name1, const char* name2,CCString* killNum){
	float length;

	if(!_reportListArray){
		_reportListArray=CCArray::create();
		_reportListArray->retain();
	}else if(_reportListArray->count()>0){
		CCDictionary* dic=CCDictionary::create();
		dic->setObject(CCString::create(name1),"name1");
		dic->setObject(CCString::create(name2),"name2");
		dic->setObject(CCString::createWithFormat("%d",1),"num");
		dic->setObject(killNum,"kills");
		_reportListArray->addObject(dic);
		return;
	}


    reportSprite=this->createReport(name1,name2,length);
	reportSprite->setPosition(Vec2(winSize.width/2-length/2,winSize.height-80));
	this->addChild(reportSprite,500);


	ActionInterval*  su=ScaleBy::create(0.2f,1.0f);
	FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(HudLayer::setReportCache, this));
	DelayTime* delay=DelayTime::create(2.0f);
	Action* seq=Sequence::create(su,su->reverse(),delay,call,nullptr);
	reportSprite->runAction(seq);

	CCDictionary* dic=CCDictionary::create();
	dic->setObject(CCString::create(name1),"name1");
	dic->setObject(CCString::create(name2),"name2");
	dic->setObject(CCString::createWithFormat("%d",1),"num");
	dic->setObject(killNum,"kills");
	dic->setObject(CCString::create("True"),"isDisplay");
	_reportListArray->addObject(dic);



}


void HudLayer::setBuffDisplay(const char* buffName, float buffStayTime){


	Sprite* buffSprite=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_%s.png",_delegate->currentPlayer->getCharacter()->getCString(),buffName)->getCString());
    buffSprite->setScale(24.0f / buffSprite->getContentSize().width);

    CCLOG("buff=%s", buffName);
    CCLOG("size=%f", buffSprite->getContentSize().width);
    CCLOG("scale=%f", buffSprite->getScale());
	buffSprite->setAnchorPoint(Vec2(0,0));
	buffSprite->setPosition(Vec2(8,item1Button->getPositionY()+76-28*_buffCount));
	if(buffStayTime !=0){
		if(_buffCount==0){

			bcdLabel1=LabelBMFont::create(CCString::createWithFormat("%d",(int)buffStayTime)->getCString(),"Fonts/1.fnt");
			bcdLabel1->setScale(0.35f);
			bcdLabel1->setPosition(Vec2(5,5));
			bcdLabel1->setAnchorPoint(Vec2(0.5,0.5));
			buffSprite->addChild(bcdLabel1);
			this->schedule(CC_SCHEDULE_SELECTOR(HudLayer::updateBuffDisplay),1.0f);
		}else{
			bcdLabel2=LabelBMFont::create(CCString::createWithFormat("%d",(int)buffStayTime)->getCString(),"Fonts/1.fnt");
			bcdLabel2->setScale(0.35f);
			bcdLabel2->setPosition(Vec2(5,5));
			bcdLabel2->setAnchorPoint(Vec2(0.5,0.5));
			buffSprite->addChild(bcdLabel2);
			this->schedule(CC_SCHEDULE_SELECTOR(HudLayer::updateBuffDisplay2),1.0f);

		}
	}


	FiniteTimeAction* call=CallFuncN::create(CC_CALLBACK_1(HudLayer::clearBuffDisplay, this));
	DelayTime* delay=DelayTime::create(buffStayTime-3);

	Action* seq=Sequence::create(delay,call,nullptr);
	buffSprite->runAction(seq);
	buffSprite->setTag(0);

	buffSprite->retain();
	this->addChild(buffSprite,500);
	_buffCount++;



}

void HudLayer::updateBuffDisplay(float dt){

	if(bcdLabel1){
		if( std::atoi(bcdLabel1->getString()) >0){
			bcdLabel1->setString(CCString::createWithFormat("%d",std::atoi(bcdLabel1->getString())-1)->getCString());
		}else{
			bcdLabel1->removeFromParentAndCleanup(true);
			bcdLabel1=nullptr;
			_buffCount--;

		}

	}





}

void HudLayer::updateBuffDisplay2(float dt){



	if(bcdLabel2){
		if( std::atoi(bcdLabel2->getString()) >0){
			bcdLabel2->setString(CCString::createWithFormat("%d",std::atoi(bcdLabel2->getString())-1)->getCString());

		}else{
			bcdLabel2->removeFromParentAndCleanup(true);
			bcdLabel2=nullptr;
			_buffCount--;

		}
	}



}

void  HudLayer::clearBuffDisplay(Node* sender){
	Sprite* bs=(Sprite*) sender;
	if(bs->getTag()==0){
		FiniteTimeAction* call=CallFuncN::create(CC_CALLBACK_1(HudLayer::clearBuffDisplay, this));
		DelayTime* delay=DelayTime::create(4.0f);
		bs->setTag(1);
		FadeOut* fade=FadeOut::create(0.5f);
		Action* fadeseq=RepeatForever::create(Sequence::createWithTwoActions(fade,fade->reverse()));
		Action* seq=Sequence::createWithTwoActions(delay,call);
		bs->runAction(seq);
		bs->runAction(fadeseq);
	}else{
		bs->removeFromParentAndCleanup(true);

	}



}


void HudLayer::clearSPCReport(){
	if(reportSPCSprite){
		reportSPCSprite->stopAllActions();
		reportSPCSprite->removeFromParent();
		reportSPCSprite=nullptr;
	}
}


void HudLayer::stopSPCReport(){
	if(reportSPCSprite){
		reportSPCSprite->stopAllActions();
		this->unschedule(CC_SCHEDULE_SELECTOR(HudLayer::updateSPCReprot));
		reportSPCSprite->setPosition(Vec2(winSize.width/2,winSize.height-105));
		reportSPCSprite->setScale(1.0f);
		DelayTime* delay=DelayTime::create(1.6f);
		FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(HudLayer::clearSPCReport, this));
		reportSPCSprite->runAction(Sequence::createWithTwoActions(delay,call));

	}
}

void HudLayer::updateSPCReprot(float dt){
	if(reportSPCSprite){
		reportSPCSprite->setPosition(Vec2(winSize.width/2,winSize.height-105));
	}

}

void HudLayer::setReportCache(){
	reportSprite->removeFromParent();
	Ref* pObject;

	CCARRAY_FOREACH(_reportListArray,pObject){
		CCDictionary* tempdict=(CCDictionary *) pObject;
		CCString* isDisplay=CCString::create(tempdict->valueForKey("isDisplay")->getCString());

		if(strcmp(isDisplay->getCString(),"True")!=0){

			CCString* name1=CCString::create(tempdict->valueForKey("name1")->getCString());
			CCString* name2=CCString::create(tempdict->valueForKey("name2")->getCString());
			CCString* num=CCString::create(tempdict->valueForKey("num")->getCString());
			CCString* killnum=CCString::create(tempdict->valueForKey("kills")->getCString());

			float length;
			reportSprite=this->createReport(name1->getCString(),name2->getCString(),length);
			reportSprite->setPosition(Vec2(winSize.width/2-length/2,winSize.height-80));
			this->addChild(reportSprite,500);

			ActionInterval*  su=ScaleBy::create(0.2f,1.1f);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(HudLayer::setReportCache, this));
			DelayTime* delay=DelayTime::create(2.0f);
			Action* seq=Sequence::create(su,su->reverse(),delay,call,nullptr);

			reportSprite->runAction(seq);


			if(strcmp(name2->getCString(),"Tower")!=0){
				Ref* pObject2;
				int num2=0;
				CCARRAY_FOREACH(_reportListArray,pObject2){
					CCDictionary* tempdict2=(CCDictionary *) pObject2;
					CCString* tempName=CCString::create(tempdict2->valueForKey("name1")->getCString());
					CCString* tempName2=CCString::create(tempdict2->valueForKey("name2")->getCString());
					CCString* isDisplay2=CCString::create(tempdict2->valueForKey("isDisplay")->getCString());

					if(strcmp(tempName->getCString(),name1->getCString())==0 &&
						strcmp(tempName2->getCString(),"Tower")!=0 &&
						strcmp(isDisplay2->getCString(),"True")==0
						){
						num2+=1;
					}
				}

				// �����ɱ
				if(num2==1){

					if(reportSPCSprite){
						reportSPCSprite->stopAllActions();
						reportSPCSprite->removeFromParent();
					}
					reportSPCSprite=this->createSPCReport(killnum->getCString(),num2);
					reportSPCSprite->setPosition(Vec2(winSize.width/2,winSize.height-115));
					this->addChild(reportSPCSprite,500);
					FadeIn* fn=FadeIn::create(0.8f);
					MoveBy* mv=MoveBy::create(0.8f,Vec2(0,10));
					Spawn* sp=Spawn::createWithTwoActions(fn,mv);
					DelayTime* delay2=DelayTime::create(1.6f);
					FiniteTimeAction* call2=CallFunc::create(CC_CALLBACK_0(HudLayer::clearSPCReport, this));
					Action* seq2=Sequence::create(sp,delay2,call2,nullptr);
					reportSPCSprite->runAction(seq2);
				}else if(num2>=2){

					if(reportSPCSprite){
						reportSPCSprite->stopAllActions();
						reportSPCSprite->removeFromParent();
					}

					reportSPCSprite=this->createSPCReport(killnum->getCString(),num2);
					reportSPCSprite->setScale(2.0);
					reportSPCSprite->setPosition(Vec2(winSize.width/2,winSize.height-105));
					this->addChild(reportSPCSprite,500);
					ScaleTo* st=ScaleTo::create(0.8f,1.0f,1.0f);
					FiniteTimeAction* call2=CallFunc::create(CC_CALLBACK_0(HudLayer::stopSPCReport, this));
					Action* seq2=Sequence::createWithTwoActions(st,call2);
					reportSPCSprite->runAction(seq2);

					FiniteTimeAction* sk=RepeatForever::create(CCShake::create(0.2f,4));
					reportSPCSprite->runAction(sk);

					this->schedule(CC_SCHEDULE_SELECTOR(HudLayer::updateSPCReprot),0.2f);
				}


			}

			tempdict->setObject(CCString::create("True"),"isDisplay");
			return;


		}
	}


	if(_reportListArray){

		_reportListArray->removeAllObjects();
		_reportListArray=nullptr;
	}

}
Sprite* HudLayer::createSPCReport(const char* killNum,int num){

	bool isBrocast=false;
	if(UserDefault::getInstance()->getBoolForKey("isVoice")!=false){
		isBrocast=true;
	}

	if(num==1){

		if(std::atoi(killNum)<10){
			reportSPCSprite=Sprite::createWithSpriteFrameName("DoubleKill_rpf.png");
			if(isBrocast)
			KTools::playEffectOptimized("Audio/Menu/kill2_1.mp3", 20, 2);
		}else if(std::atoi(killNum)<20){
			reportSPCSprite=Sprite::createWithSpriteFrameName("KillingSpree_rpf.png");
			if(isBrocast)
			KTools::playEffectOptimized("Audio/Menu/kill2_2.mp3", 20, 2);
		}else if(std::atoi(killNum)<30){
			reportSPCSprite=Sprite::createWithSpriteFrameName("Unstoppable_rpf.png");
			if(isBrocast)
			KTools::playEffectOptimized("Audio/Menu/kill2_3.mp3", 20, 2);
		}else if(std::atoi(killNum)>=30){
			reportSPCSprite=Sprite::createWithSpriteFrameName("Godlike_rpf.png");
			if(isBrocast)
			KTools::playEffectOptimized("Audio/Menu/kill2_4.mp3", 20, 2);

		}

	}else if(num==2){

		if(std::atoi(killNum)<10){
			reportSPCSprite=Sprite::createWithSpriteFrameName("TripeKill_rpf.png");
			if(isBrocast)
			KTools::playEffectOptimized("Audio/Menu/kill3_1.mp3", 20, 2);

		}else if(std::atoi(killNum)<20){
			reportSPCSprite=Sprite::createWithSpriteFrameName("Domination_rpf.png");
			if(isBrocast)
			KTools::playEffectOptimized("Audio/Menu/kill3_2.mp3", 20, 2);
		}else if(std::atoi(killNum)<30){
			reportSPCSprite=Sprite::createWithSpriteFrameName("Rampage_rpf.png");
			if(isBrocast)
			KTools::playEffectOptimized("Audio/Menu/kill3_3.mp3", 20, 2);
		}else if(std::atoi(killNum)>=30){
			reportSPCSprite=Sprite::createWithSpriteFrameName("Holyshit_rpf.png");
			if(isBrocast)
			KTools::playEffectOptimized("Audio/Menu/kill3_4.mp3", 20, 2);

		}

	}else {
		reportSPCSprite=Sprite::createWithSpriteFrameName("MonsterKill_rpf.png");
		if(isBrocast)
		KTools::playEffectOptimized("Audio/Menu/kill4.mp3", 20, 2);

	}

	return reportSPCSprite;
}
Sprite* HudLayer::createReport(const char* name1,const char* name2,float &length){



	Sprite* reportSprite=Sprite::create();

	Sprite* slain_p=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_rp.png",name1)->getCString());
	slain_p->setAnchorPoint(Vec2(0,0.5f));
	slain_p->setPosition(Vec2(0,0));
	reportSprite->addChild(slain_p);

	Sprite* slain_pf=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_rpf.png",name1)->getCString());
	slain_pf->setPosition(Vec2(slain_p->getContentSize().width,0));
	slain_pf->setAnchorPoint(Vec2(0,0.5f));
	reportSprite->addChild(slain_pf);


	Sprite* slain;
	if(strcmp(name2,"Tower")!=0){
		slain=Sprite::createWithSpriteFrameName("slain.png");

		reportSprite->addChild(slain);
	}else {
		slain=Sprite::createWithSpriteFrameName("destroy.png");
		reportSprite->addChild(slain);
	}
	slain->setAnchorPoint(Vec2(0,0.5f));
	slain->setPosition(Vec2(slain_pf->getPositionX()+slain_pf->getContentSize().width,0));


	Sprite* death_pf=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_rpf.png",name2)->getCString());
	death_pf->setAnchorPoint(Vec2(0,0.5f));
	death_pf->setPosition(Vec2(slain->getPositionX()+slain->getContentSize().width,0));
	reportSprite->addChild(death_pf);
	Sprite* death_p;
	if(strcmp(name2,"Tower")!=0){

		death_p=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_rp.png",name2)->getCString());
		death_p->setAnchorPoint(Vec2(0,0.5f));
		death_p->setPosition(Vec2(death_pf->getPositionX()+death_pf->getContentSize().width,0));
		reportSprite->addChild(death_p);
	}

	if(strcmp(name2,"Tower")!=0){
		length= slain_p->getContentSize().width+ slain_pf->getContentSize().width+ slain->getContentSize().width
		+death_pf->getContentSize().width+death_p->getContentSize().width;

	}else{
		length= slain_p->getContentSize().width+ slain_pf->getContentSize().width+ slain->getContentSize().width
			+death_pf->getContentSize().width;
	}

	return reportSprite;


}



void HudLayer::attackButtonClick(abType type){
	_delegate->attackButtonClick(type);
}


void HudLayer::gearButtonClick(gearType type){
	_delegate->gearButtonClick(type);
}

void HudLayer::attackButtonRelease(){
	_delegate->attackButtonRelease();
}

void HudLayer::pauseButtonClick(Ref* sender){
	_delegate->onPause();
}

void HudLayer::gearButtonClick(Ref* sender){
	if(!_isAllButtonLocked){
		_delegate->onGear();
	}

}



bool HudLayer::getSkillFinish(){

	if(!_delegate->getSkillFinish()){
		return false;
	};

	return true;

};

bool HudLayer::getOugisEnable(bool isCKR2){
	if(!isCKR2){
		float ckr=atof(_delegate->currentPlayer->getCKR()->getCString());
		if (ckr>=15000){
			return true;
		}else{
			return false;
		}
	}else{

		float ckr=atof(_delegate->currentPlayer->getCKR2()->getCString());
		if (ckr>=25000){
			return true;
		}else{
			return false;
		}

	}


};
void HudLayer::costCKR(int value,bool isCKR2){
	if(_delegate->_isUnlimitedCKR){
		if(!isCKR2){
			_delegate->fillCKRToMax(_delegate->currentPlayer);
			this->setCKRLose(false);
		}else{
			_delegate->fillCKRToMax(_delegate->currentPlayer);
			this->setCKRLose(true);
		}
		return;
	}

	if(!isCKR2){
		float ckr=atof(_delegate->currentPlayer->getCKR()->getCString());

		if(ckr-value>0){
			ckr-=value;
		}else{
			ckr=0;
		}

		_delegate->currentPlayer->setCKR(CCString::createWithFormat("%f",ckr));
		this->setCKRLose(false);

	}else{

		float  ckr=atof(_delegate->currentPlayer->getCKR2()->getCString());

		if(ckr-value>0){
			ckr-=value;
		}else{
			ckr=0;
		}

		_delegate->currentPlayer->setCKR2(CCString::createWithFormat("%f",ckr));
		this->setCKRLose(true);

	}



}

void HudLayer::setOugis(CCString* character,CCString* group){

	if(!ougisLayer){
		const auto& childArray = this->getChildren();
		for (auto pObject : childArray){
			Node* object=(Node*) pObject;
			object->pauseSchedulerAndActions();
		}


		ougisLayer=Layer::create();
		int startPosY=0;
		int startPosX=0;
		int endPosX=0;
		Sprite* CutBg;
		const char* cutPath1;
		const char* cutPath2;
		if(_delegate->zhenying>0){
			cutPath1="CutBg.png";
			cutPath2="CutBg2.png";
		}else{
			cutPath1="CutBg2.png";
			cutPath2="CutBg.png";

		}

		if(strcmp(group->getCString(),"Konoha")==0){
			startPosY=0;
			startPosX=-48;
			endPosX=0;
			CutBg=Sprite::createWithSpriteFrameName(cutPath1);
			CutBg->setAnchorPoint(Vec2(0,0));
		}else{
			startPosY=winSize.height/2;
			startPosX=winSize.width+48;
			endPosX=winSize.width;
			CutBg=Sprite::createWithSpriteFrameName(cutPath2);
			CutBg->setFlipX(true);
			CutBg->setAnchorPoint(Vec2(1,0.5f));
		}


		CutBg->setPosition(Vec2(startPosX,startPosY));
		CutBg->runAction(MoveTo::create(0.3f,Vec2(endPosX,startPosY)));
		ougisLayer->addChild(CutBg);


		Vector<SpriteFrame*> animeFrames;
		for (int i=1;i<=8; i++){
			SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(CCString::createWithFormat("CutLine_%02d.png",i)->getCString());
			animeFrames.pushBack(frame);
		}

		Animation* tempAnimation= Animation::createWithSpriteFrames(animeFrames,0.1f);
		Action* tempAction=Animate::create(tempAnimation);

		Sprite* CutLine=Sprite::createWithSpriteFrameName("CutLine_01.png");
		if(strcmp(group->getCString(),"Konoha")==0){
			CutLine->setAnchorPoint(Vec2(0,0));
			CutLine->setPosition(Vec2(-48,0));
			CutLine->runAction(tempAction);
			CutLine->runAction(MoveTo::create(0.3f,Vec2(0,0)));
		}else{
			CutLine->setFlipX(true);
			CutLine->setAnchorPoint(Vec2(1,0.5f));
			CutLine->setPosition(Vec2(winSize.width+48,winSize.height/2));
			CutLine->runAction(tempAction);
			CutLine->runAction(MoveTo::create(0.3f,Vec2(winSize.width,winSize.height/2)));
		}

		ougisLayer->addChild(CutLine);

		Sprite* CutLineUP=Sprite::createWithSpriteFrameName("CutLineUP.png");
		if(strcmp(group->getCString(),"Konoha")==0){
			CutLineUP->setAnchorPoint(Vec2(0,0));
			CutLineUP->setPosition(Vec2(-48,2));
			CutLineUP->runAction(MoveTo::create(0.3f,Vec2(-1,1)));
		}else{
			CutLineUP->setFlipX(true);
			CutLineUP->setAnchorPoint(Vec2(1,0.5f));
			CutLineUP->setPosition(Vec2(winSize.width+48,winSize.height/2+2));
			CutLineUP->runAction(MoveTo::create(0.3f,Vec2(winSize.width+1,winSize.height/2+1)));
		}

		ougisLayer->addChild(CutLineUP);



		Sprite* CutIn=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_CutIn.png",character->getCString())->getCString());
		if(strcmp(group->getCString(),"Konoha")==0){
			CutIn->setAnchorPoint(Vec2(0,0));
			CutIn->setPosition(Vec2(-200,10));
			CutIn->runAction(MoveTo::create(0.3f,Vec2(0,10)));
		}else{
			CutIn->setAnchorPoint(Vec2(1,0));
			CutIn->setPosition(Vec2(winSize.width+200,winSize.height/2+10-CutBg->getContentSize().height/2));
			CutIn->runAction(MoveTo::create(0.3f,Vec2(winSize.width,winSize.height/2+10-CutBg->getContentSize().height/2)));
		}
		ougisLayer->addChild(CutIn);



		Sprite* CutLineDown=Sprite::createWithSpriteFrameName("CutLineDown.png");
		if(strcmp(group->getCString(),"Konoha")==0){
			CutLineDown->setAnchorPoint(Vec2(0,0));
			CutLineDown->setPosition(Vec2(-48,0));
			CutLineDown->runAction(MoveTo::create(0.3f,Vec2(0,0)));
		}else{
			CutLineDown->setFlipX(true);
			CutLineDown->setAnchorPoint(Vec2(1,0));
			CutLineDown->setPosition(Vec2(winSize.width+48,winSize.height/2-CutBg->getContentSize().height/2));
			CutLineDown->runAction(MoveTo::create(0.3f,Vec2(winSize.width,winSize.height/2-CutBg->getContentSize().height/2)));
		}

		ougisLayer->addChild(CutLineDown);


		ougisLayer->setPosition(Vec2(0,0));
		this->addChild(ougisLayer,-100);

		DelayTime* delay=DelayTime::create(1.0f);
		FiniteTimeAction *call = CallFuncN::create(CC_CALLBACK_1(HudLayer::removeOugis, this));
		ougisLayer->runAction(Sequence::createWithTwoActions(delay,call));
	}

}

void HudLayer::removeOugis(Node* sender){

	ougisLayer->removeFromParent();
	ougisLayer=nullptr;

	const auto& childArray = this->getChildren();
	for (auto pObject : childArray){
		Node* object=(Node*) pObject;
		object->resumeSchedulerAndActions();
	}


	_delegate->removeOugis();

}



