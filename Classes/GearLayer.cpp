#include "GearLayer.h"
#include <cstdlib>
#include <string>
#include "GameLayer.h"
#include "HudLayer.h"
#include "HeroPayload.h"
#include "Characters.h"
using namespace CocosDenshion;

/*----------------------
init GearButton ;
----------------------*/
GearButton::GearButton(void)
{
	soIcon=nullptr;
	_isBuyed=false;
}


GearButton::~GearButton(void)
{

}


bool GearButton::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());
		if(strcmp(szImage,"")!=0)
		this->initWithSpriteFrameName(szImage);


		this->setAnchorPoint(Vec2(0,0));

		bRet=true;

	}while(0);

	return bRet;

}

void GearButton::onEnter(){
	Sprite::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GearButton::ccTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GearButton::ccTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GearButton::ccTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void GearButton::onExit(){
	Sprite::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

Rect GearButton::getRect(){
	Size size=this->getContentSize();
	if(size.width<1.0f || size.height<1.0f){
		float maxW=0.0f;
		float maxH=0.0f;
		const auto& children=this->getChildren();
		for(auto childObj : children){
			Node* child=dynamic_cast<Node*>(childObj);
			if(!child){
				continue;
			}
			Size childSize=child->getContentSize();
			maxW=MAX(maxW,childSize.width);
			maxH=MAX(maxH,childSize.height);
		}
		if(maxW>0.0f && maxH>0.0f){
			return Rect(0,0,maxW,maxH);
		}
	}
	return Rect(0,0,size.width,size.height);

}
bool GearButton::containsTouchLocation(Touch *touch){
	if(!this->isVisible()){
		return false;
	}
	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}


void GearButton::setBtnType(gearType type,gearbtnType type2,bool isBuyed){

	_gearType=type;
	_btnType=type2;

	if(_btnType==0){
		std::string gearPath=cocos2d::StringUtils::format("gear_%02d.png",gearType(_gearType));
		Sprite* gearIcon=Sprite::createWithSpriteFrameName(gearPath);
		gearIcon->setAnchorPoint(Vec2(0.5f,0.5f));
		gearIcon->setPosition(Vec2(20,30));
		this->addChild(gearIcon);

	}else{
		std::string gearPath=cocos2d::StringUtils::format("gear_%02d.png",gearType(_gearType));
		Sprite* gearIcon=Sprite::createWithSpriteFrameName(gearPath);
		gearIcon->setAnchorPoint(Vec2(0.5f,0.5f));
		gearIcon->setScale(0.75f);
		this->addChild(gearIcon);
	}

	if(isBuyed){
		_isBuyed=true;
		soIcon=Sprite::createWithSpriteFrameName("gear_so.png");
		soIcon->setAnchorPoint(Vec2(0.5f,0.5f));
		soIcon->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
		this->addChild(soIcon);

	}


}

gearType	GearButton::getBtnType(){
	return _gearType;
}

bool GearButton::ccTouchBegan(Touch* touch,Event* event){
	if (!this->containsTouchLocation(touch) || soIcon){
		return false;
	}
	return true;
}

void GearButton::ccTouchMoved(Touch* touch,Event* event){
	// touch area


}

void GearButton::ccTouchEnded(Touch* touch,Event* event){
	if(UserDefault::getInstance()->getBoolForKey("isVoice")!=false){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/select.mp3");
	}
	if(!_isBuyed){
		_delegate->currentGear=this->_gearType;
	}


	std::string detailPath=cocos2d::StringUtils::format("gearDetail_%02d.png",gearType(_gearType));
	SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName(detailPath);
	_delegate->gearDetail->setSpriteFrame(frame);
};



void GearButton::playSound(){


}

GearButton* GearButton::create( const char *szImage ){
	GearButton* mb=new GearButton();
	if (mb && mb->init(szImage))
	{
		mb->autorelease();
		return mb;
	}
	else
	{
		delete mb;
		return nullptr;
	}
}




ScrewLayer::ScrewLayer(void)
{
	_delegate=nullptr;

}


ScrewLayer::~ScrewLayer(void)
{
}


bool ScrewLayer::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Layer::init());

		bRet=true;

	}while(0);

	return bRet;

}

void ScrewLayer::onEnter(){
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ScrewLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ScrewLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ScrewLayer::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
};

void ScrewLayer::onExit(){
	Layer::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
};

bool ScrewLayer::isTouchInScrollArea(Touch* touch){
	Vec2 local=this->convertToNodeSpace(touch->getLocation());
	int rows=gearNum>0 ? (gearNum+2)/3 : totalRow;
	if(rows<1){
		rows=1;
	}
	float minY=-(rows-1)*60.0f-12.0f;
	float maxY=44.0f;
	Rect area(-8,minY,3*46.0f+24.0f,maxY-minY);
	return area.containsPoint(local);
}

bool ScrewLayer::onTouchBegan(Touch* touch,Event* event){
	prePosY=0.0f;
	return isTouchInScrollArea(touch);
};

void ScrewLayer::onTouchMoved(Touch* touch,Event* event){
	// touch area
	Vec2 curPoint = touch->getLocation();
	if(prePosY==0.0f){
		prePosY=curPoint.y;
	}else{
		float distanceY=curPoint.y-prePosY;
		if(this->getPositionY()<totalRow*74 || distanceY<0){
			this->setPositionY(this->getPositionY()+distanceY);

		}
		if((this->screwBar->getPositionY()>90|| distanceY<0 ) && this->screwBar->getPositionY()<=122 ){
			this->screwBar->setPositionY(this->screwBar->getPositionY()-distanceY);
		}

		if(this->screwBar->getPositionY()>122 ){
			this->screwBar->setPositionY(122);
		}

		if(this->screwBar->getPositionY()<90 ){
			this->screwBar->setPositionY(90);
		}


		prePosY=curPoint.y;

	}

};


void ScrewLayer::onTouchEnded(Touch* touch,Event* event){
	prePosY=0.0f;
	//CCLOG("%f",this->getPositionY());
	//CCLOG("Height:%f",this->getContentSize().height);

	if(this->getPositionY()>totalRow*74-74){
		this->setPositionY(totalRow*65);
	}
	if(this->getPositionY()<76){
		this->setPositionY(76);
	}

	if(this->screwBar->getPositionY()>122 ){
		this->screwBar->setPositionY(122);
	}

	if(this->screwBar->getPositionY()<90 ){
		this->screwBar->setPositionY(90);
	}
};





GearLayer::GearLayer(void)
{
	currentGear_layer=nullptr;
	currentGear=None;
	_buyBtn=nullptr;
}


GearLayer::~GearLayer(void)
{
}

bool GearLayer::init(RenderTexture* snapshoot){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());

		SimpleAudioEngine::getInstance()->stopAllEffects();


		Texture2D* bgTexture=snapshoot->getSprite()->getTexture();
		Sprite* bg=Sprite::createWithTexture(bgTexture);
		bg->setAnchorPoint(Vec2(0,0));
		bg->setFlipY(true);
		this->addChild(bg,0);

		Layer* blend=LayerColor::create(Color4B(0, 0, 0, 150),winSize.width,winSize.height);
		this->addChild(blend,1);


		//??




		Layer* gears_layer=Layer::create();

		gears_bg=Sprite::createWithSpriteFrameName("gears_bg.png");
		gears_bg->setAnchorPoint(Vec2(0.5f,0.5f));
		gears_bg->setPosition(Vec2(winSize.width/2,winSize.height/2-12));
		gears_layer->addChild(gears_bg,1);

		coinLabel=LabelBMFont::create("0","Fonts/arial.fnt");
		coinLabel->setAnchorPoint(Vec2(0,0));
		coinLabel->setPosition(Vec2(gears_bg->getPositionX()+2,58));
		gears_layer->addChild(coinLabel,2);


		this->addChild(gears_layer,10);



		ClippingNode* clipper = ClippingNode::create();
		Node *stencil = Sprite::createWithSpriteFrameName("screwMask.png");
		stencil->setAnchorPoint(Vec2(0,0));
		clipper->setStencil(stencil);

		_screwLayer=ScrewLayer::create();
		_screwLayer->setAnchorPoint(Vec2(0,0));
		_screwLayer->setPositionY(76);
		_screwLayer->gearNum=9;


		_screwLayer->screwBar=Sprite::createWithSpriteFrameName("screwBar.png");
		_screwLayer->screwBar->setAnchorPoint(Vec2(0.5f,0));
		_screwLayer->screwBar->setPosition(Vec2(gears_bg->getPositionX()+25,126));
		this->addChild(_screwLayer->screwBar,600);


		gearDetail=Sprite::createWithSpriteFrameName("gearDetail_00.png");
		gearDetail->setAnchorPoint(Vec2(0.5f,1));
		gearDetail->setPosition(Vec2(gears_bg->getPositionX()+gears_bg->getContentSize().width/2-54,210));
		this->addChild(gearDetail,600);

		clipper->setPosition(Vec2(gears_bg->getPositionX()-gears_bg->getContentSize().width/2+4,85));
		clipper->addChild(_screwLayer);
		this->addChild(clipper,600);

		this->setupBuyButton(gears_bg);

		MenuItem* btm_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("close_btn1.png"),
			Sprite::createWithSpriteFrameName("close_btn2.png"),CC_CALLBACK_1(GearLayer::onResume, this));
		Menu* overMenu=Menu::create(btm_btn,nullptr);
		overMenu->setPosition(Vec2(winSize.width/2+gears_bg->getContentSize().width/2-12,winSize.height/2+gears_bg->getContentSize().height/2-20));
		this->addChild(overMenu,800);





		bRet=true;
	}while(0);

	return bRet;

}



void GearLayer::setupBuyButton(Sprite* gearsBg){
	_buyBtn=Sprite::createWithSpriteFrameName("gearBuy_btn.png");
	_buyBtn->setAnchorPoint(Vec2(0.5f,0.5f));
	_buyBtn->setPosition(Vec2(gearsBg->getPositionX()+78,gearsBg->getPositionY()-gearsBg->getContentSize().height/2+35));
	this->addChild(_buyBtn,750);

	auto listener=EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan=[this](Touch* touch,Event* event)->bool{
		if(!_buyBtn || !_buyBtn->isVisible()){
			return false;
		}
		Vec2 local=_buyBtn->convertToNodeSpace(touch->getLocation());
		Rect rect(0,0,_buyBtn->getContentSize().width,_buyBtn->getContentSize().height);
		if(!rect.containsPoint(local)){
			return false;
		}
		SpriteFrame* pressed=SpriteFrameCache::getInstance()->spriteFrameByName("gearBuy_btn2.png");
		if(pressed){
			_buyBtn->setSpriteFrame(pressed);
		}
		return true;
	};
	listener->onTouchEnded=[this](Touch* touch,Event* event){
		if(!_buyBtn){
			return;
		}
		Vec2 local=_buyBtn->convertToNodeSpace(touch->getLocation());
		Rect rect(0,0,_buyBtn->getContentSize().width,_buyBtn->getContentSize().height);
		if(rect.containsPoint(local)){
			this->onGearBuy(nullptr);
		}
		SpriteFrame* normal=SpriteFrameCache::getInstance()->spriteFrameByName("gearBuy_btn.png");
		if(normal){
			_buyBtn->setSpriteFrame(normal);
		}
	};
	listener->onTouchCancelled=listener->onTouchEnded;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,_buyBtn);
}

void GearLayer::onResume(Ref* sender){

	this->getDelegate()->getHudLayer()->updateGears();
	Director::getInstance()->popScene();

};


void GearLayer::onGearBuy(Ref* sender){

	if(UserDefault::getInstance()->getBoolForKey("isVoice")!=false){
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
	}

	if( _delegate->currentPlayer->setGear(currentGear)){
		this->updatePlayerGear();
	}

};

void GearLayer::updatePlayerGear(){


	if(_delegate->currentPlayer->getGearArray() && _delegate->currentPlayer->getGearArray()->count()>0){

		if(currentGear_layer!=nullptr){
			currentGear_layer->removeFromParentAndCleanup(true);
		}
		currentGear_layer=Layer::create();
		currentGear_layer->setAnchorPoint(Vec2(0,0));
		int i=0;
		for(unsigned int gearIndex=0; gearIndex<_delegate->currentPlayer->getGearArray()->count(); ++gearIndex){
			Ref* pObject=_delegate->currentPlayer->getGearArray()->objectAtIndex(gearIndex);
			int tmpGear=NarutoSenkiPayload::gearValue(pObject);
			GearButton* btn=GearButton::create("");
			btn->setBtnType(gearType(tmpGear),gearbtnType(1),false);
			btn->setPositionX(13+i*34);
			btn->setDelegate(this);
			currentGear_layer->addChild(btn);
			i++;
		}
		currentGear_layer->setPosition(Vec2(gears_bg->getPositionX()-gears_bg->getContentSize().width/2+8,64));
		this->addChild(currentGear_layer,650);
	}


	std::string coinText=cocos2d::StringUtils::format("%d",_delegate->currentPlayer->getCoin());
	coinLabel->setString(coinText);
	this->updateGearList();

};

void GearLayer::updateGearList(){

		for(GearButton* btn : _screwLayer->_gearArray){
			btn->removeFromParentAndCleanup(true);
		}
		_screwLayer->_gearArray.clear();
		currentGear=None;
		_screwLayer->totalRow=3;
		for(int i=0;i<_screwLayer->gearNum;i++){
			int row=int(floor(float(i)/3));
			int column=abs(3*row-i);

			GearButton* btn;
			if(column>=1){
				btn=GearButton::create("value_1000.png");
			}else{
				btn=GearButton::create("value_500.png");
			}
			bool isBuyed=false;
			if(_delegate->currentPlayer->getGearArray() && _delegate->currentPlayer->getGearArray()->count()>0){
				for(unsigned int gearIndex=0; gearIndex<_delegate->currentPlayer->getGearArray()->count(); ++gearIndex){
					Ref* pObject=_delegate->currentPlayer->getGearArray()->objectAtIndex(gearIndex);
					int tmpGear=NarutoSenkiPayload::gearValue(pObject);
					if (tmpGear== i){
						isBuyed=true;
					}
				}
			}

			if(currentGear==None && !isBuyed){
				currentGear=gearType(i);
				std::string detailPath=cocos2d::StringUtils::format("gearDetail_%02d.png",i);
				SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName(detailPath);
				gearDetail->setSpriteFrame(frame);
			}

			btn->setPosition(Vec2(6+column*46,-row*60));
			btn->setAnchorPoint(Vec2(0,0));
			btn->setBtnType(gearType(i),gearbtnType(0),isBuyed);
			btn->setDelegate(this);
			_screwLayer->_gearArray.push_back(btn);
			_screwLayer->addChild(btn);

		}


}


GearLayer*  GearLayer::create(RenderTexture* snapshoot){
	GearLayer* grl=new  GearLayer();
	if (grl && grl->init(snapshoot))
	{
		grl->autorelease();
		return grl;
	}
	else
	{
		delete grl;
		return nullptr;
	}
}


