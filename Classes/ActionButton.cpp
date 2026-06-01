#include "ActionButton.h"
#include "HudLayer.h"

#include <chrono>

namespace
{
	float currentTimeMilliseconds()
	{
		using namespace std::chrono;
		static const auto startTime = steady_clock::now();
		return static_cast<float>(duration_cast<milliseconds>(
			steady_clock::now() - startTime).count());
	}
}

ActionButton::ActionButton(void)
{
	_isDoubleSkill=false;
	_freezeAction=NULL;
	markSprite=NULL;
	_clickTime=0;
	_clickNum=0;
	cdLabel=NULL;
	_isMarkVisable=true;
	_timeCout=0;
	_isLock=false;
	_coldDown=0;
	_isColdChanged=false;
	_gearType=None;

	proressblinkSprite=NULL;
	proressmarkSprite=NULL;
	proressblinkSprite=NULL;
	progressPointSprite=NULL;
	proressblinkMask=NULL;
	gearSign =NULL;
}


ActionButton::~ActionButton(void)
{
}

bool ActionButton::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());
		this->initWithSpriteFrameName(szImage);
		this->setAnchorPoint(Vec2(0,0));

		bRet=true;

	}while(0);

	return bRet;

}

void ActionButton::setCD(CCString* value){
	_coldDown = value ? value->intValue() : 0;
}

void ActionButton::setCD(int value){
	_coldDown = value;
}

int ActionButton::getCD() const{
	return _coldDown;
}

void ActionButton::setTimeCout(int value){
	_timeCout = value;
}

int ActionButton::getTimeCout() const{
	return _timeCout;
}

void ActionButton::onEnter(){
	Sprite::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ActionButton::ccTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(ActionButton::ccTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void ActionButton::onExit(){
	Sprite::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

Rect ActionButton::getRect(){
	Size size=this->getContentSize();
	return Rect(0,0, size.width, size.height);

}
bool ActionButton::containsTouchLocation(Touch *touch){
	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}

bool  ActionButton::ccTouchBegan(Touch* touch,Event* event){
	
	// touch area 
	if (!this->containsTouchLocation(touch) || _delegate->_isAllButtonLocked){
		return false;
	}
	click();
	
	return true;

}
void ActionButton::ccTouchEnded(Touch* touch,Event* event){
	if(_abType==NAttack)
	_delegate->attackButtonRelease();
}

void ActionButton::click(){
	if( (_delegate && this->isCanClick()) ){

		if(!_isDoubleSkill ){
			this->beganAnimation();
		}

		if(!_delegate->_isAllButtonLocked){
			if(_abType==GearItem ){
				_delegate->gearButtonClick(_gearType);
			}else{
				_delegate->attackButtonClick(_abType);
			}
			
		}
		
	}
};



bool ActionButton::isCanClick(){
	//recorde current time sec format;
	

	float curTime = currentTimeMilliseconds();
	
	 if(this->_abType!=NAttack ){

		if(this->_isDoubleSkill){
			//double click solution
				if(_clickNum==0 && _delegate->getSkillFinish() && _timeCout <= 0 && !_delegate->ougisLayer ){
					return true;
				}else if(_clickNum==1 && _delegate->getSkillFinish() && !_delegate->ougisLayer ){
					return true;
				}

		 }else{
			 
	 // isSkillFinish consider the AttackAction is done or not to prevent the skill invalid release 
			 if(this->getABType()==Item1 ){
				
				 if(!_delegate->ougisLayer && _timeCout <= 0  && !_isLock &&_delegate->_delegate->currentPlayer->getActionState()!=ACTION_STATE_DEAD){
					 if(_delegate->offCoin(_cost)){
						 return true;
					 }
				 }
				
			 }else if(this->_abType==GearBtn){
				  if(!_isLock){
					  this->_delegate->_delegate->onGear();

				  }
				   return false;

			 }else if(this->_abType==GearItem ){

				 if(!_delegate->ougisLayer && _timeCout <= 0  && !_isLock){
					 if(this->_gearType== gear06  && _delegate->_delegate->currentPlayer->getActionState()!=ACTION_STATE_DEAD ){
						 return true;
					 }else if(this->_gearType== gear00 && _delegate->getSkillFinish()){
						 return true;
			
					 }else if( this->_gearType== gear03){
						 return true;
					 }
				 }


			 }
			 //ougis click solution
			 else if(this->_abType==OUGIS1){
				 if(_delegate->getSkillFinish() && _delegate->getOugisEnable(false) && !_isLock && !_delegate->ougisLayer ){
					 _delegate->costCKR(15000,false);	
					 return true;
				 }
			 }else if(this->_abType==OUGIS2){

				 if(_delegate->getSkillFinish() && _delegate->getOugisEnable(true) && !_isLock && !_delegate->ougisLayer){
					 _delegate->costCKR(25000,true);	
					 return true;
				 }
				
			 }else{
				 if( _timeCout <= 0 && _delegate->getSkillFinish() && !_isLock  && !_delegate->ougisLayer){
					 return true;
				 }
			 }
		 }

		 return false;
	 }else{
		return true;
	 }
};

void ActionButton::beganAnimation(bool isLock){
	//record the click time
	_clickTime = currentTimeMilliseconds();

	if(markSprite && this->getABType()!=OUGIS1 && this->getABType()!=OUGIS2){
		if(!_freezeAction || _isColdChanged){
			if(_isColdChanged){
				_isColdChanged=false;
			}
			this->createFreezeAnimation();
		};

		
		this->setTimeCout(this->getCD());
		if(cdLabel){
			cdLabel->removeFromParent();
			this->unschedule(CC_SCHEDULE_SELECTOR(ActionButton::updateCDLabel));
		}
		if(this->getABType()!=Item1){
			
			cdLabel=LabelBMFont::create(CCString::createWithFormat("%d",this->getCD()/1000)->getCString(),"Fonts/1.fnt");
			cdLabel->setScale(0.4f);
			cdLabel->setPosition(Vec2(this->getPositionX()+this->getContentSize().width/2,
				this->getPositionY()+this->getContentSize().height/2));
			this->_delegate->addChild(cdLabel,200);
			
		}
		this->schedule(CC_SCHEDULE_SELECTOR(ActionButton::updateCDLabel),1.0f);

		markSprite->stopAllActions();

		markSprite->runAction(_freezeAction);
	
	}
	

};

void ActionButton::setGearType(int tmpGearType){

	Sprite* gearIcon=Sprite::createWithSpriteFrameName(CCString::createWithFormat("gear_%02d.png",gearType(tmpGearType))->getCString());
	gearIcon->setAnchorPoint(Vec2(0.5f,0.5f));
	gearIcon->setScale(0.85f);
	gearIcon->setPosition(Vec2(18,18));
	this->addChild(gearIcon);
	this->_gearType=gearType(tmpGearType);
	
	if(this->gearSign){
		this->gearSign->removeFromParent();
		this->gearSign=NULL;
	}
}

void ActionButton::updateCDLabel(float dt){

	if( !_delegate->ougisLayer){

		if(this->getTimeCout()>1000){
			int tempCout=this->getTimeCout()-1000;
			this->setTimeCout(tempCout);
			if(cdLabel){
				cdLabel->setString(CCString::createWithFormat("%d",tempCout/1000)->getCString());
			}
			
		}else{
			this->unschedule(CC_SCHEDULE_SELECTOR(ActionButton::updateCDLabel));
			this->setTimeCout(0);
			if(cdLabel){
			cdLabel->removeFromParent();
			cdLabel=NULL;
			}
		}
	};
};

void ActionButton::setMarkSprite(const char* mark){
	Sprite* tmpSprite =Sprite::createWithSpriteFrameName(mark);
	markSprite=ProgressTimer::create(tmpSprite);
	markSprite->setType(ProgressTimer::Type::RADIAL);
	
	markSprite->setReverseDirection(true);
	markSprite->setPosition(this->getPosition());
	markSprite->setAnchorPoint(Vec2(0,0));
	this->_delegate->addChild(markSprite,500);

	if(this->getABType()==GearBtn){
		if(_delegate->_delegate->_isHardCoreGame){
			gearSign =Sprite::createWithSpriteFrameName("gearsign.png");
			gearSign->setPosition(Vec2(this->getPositionX()+17,this->getPositionY()+17));
			this->_delegate->addChild(gearSign,500);
		}
		

	}
};

void ActionButton::setOugisMark(){
	ougismarkSprite =Sprite::createWithSpriteFrameName("skill_freeze.png");
	ougismarkSprite->setPosition(this->getPosition());
	ougismarkSprite->setAnchorPoint(Vec2(0,0));
	this->_delegate->addChild(ougismarkSprite,500);
	if (this->getABType()== OUGIS1){
	lockLabel1=LabelBMFont::create("LV2","Fonts/1.fnt");
	}else{
	lockLabel1=LabelBMFont::create("LV4","Fonts/1.fnt");
	}
	lockLabel1->setScale(0.4f);
	lockLabel1->setPosition(Vec2(this->getPositionX()+this->getContentSize().width/2,
		this->getPositionY()+this->getContentSize().height/2));
	this->_delegate->addChild(lockLabel1,200);
	_isLock=true;
}

void ActionButton::setProgressMark(){

	clipper = ClippingNode::create();
	Node *stencil = Sprite::createWithSpriteFrameName("icon_bg1.png");
	stencil->setAnchorPoint(Vec2(0,0));
	clipper->setStencil(stencil);

	proressmarkSprite=Sprite::createWithSpriteFrameName("icon_bg2.png");

	clipper->setPosition(this->getPosition());
	clipper->addChild(proressmarkSprite);

	proressmarkSprite->setAnchorPoint(Vec2(0.5f,0.5f));
	proressmarkSprite->setPosition(Vec2(proressmarkSprite->getContentSize().width/2,proressmarkSprite->getContentSize().height/2));
	//50,120,180
	
	this->_delegate->addChild(clipper,-50);
	

	proressblinkSprite =Sprite::createWithSpriteFrameName("icon_bg3.png");
	proressblinkSprite->setPosition(this->getPosition());
	proressblinkSprite->setAnchorPoint(Vec2(0.5f,0.5f));
	proressblinkSprite->setPosition(Vec2(proressmarkSprite->getContentSize().width/2,proressmarkSprite->getContentSize().height/2));
	

	FadeOut* fd=FadeOut::create(0.2f);
	proressblinkSprite->runAction(RepeatForever::create(fd));
	clipper->addChild(proressblinkSprite,50);
	

	if (this->getABType()== OUGIS1){
	progressPointSprite=Sprite::createWithSpriteFrameName("icon_bg4.png");
	proressmarkSprite->setRotation(-50);
	proressblinkSprite->setRotation(-50);
	progressPointSprite->setPosition(this->getPosition());
	}else{
	progressPointSprite=Sprite::createWithSpriteFrameName("icon_bg5.png");
	proressmarkSprite->setRotation(-85);
	proressblinkSprite->setRotation(-85);
	progressPointSprite->setPosition(Vec2(this->getPositionX()+1,this->getPositionY()));
	}

	
	progressPointSprite->setAnchorPoint(Vec2(0,0));
	this->_delegate->addChild(progressPointSprite,-25);

	FadeOut* fd2=FadeOut::create(0.5f);
	proressblinkMask=Sprite::createWithSpriteFrameName("icon_bg6.png");
	proressblinkMask->setPosition(this->getPosition());
	proressblinkMask->setAnchorPoint(Vec2(0,0));
	proressblinkMask->runAction(RepeatForever::create(fd2));


	this->_delegate->addChild(proressblinkMask,200);

	if(this->getABType()==OUGIS2){

		if(_delegate->skill4Button){
			if(_delegate->skill4Button->proressblinkMask && !_delegate->skill4Button->_isLock){
				_delegate->skill4Button->proressblinkMask->stopAllActions();
				FadeOut* fd2=FadeOut::create(0.5f);
				_delegate->skill4Button->proressblinkMask->runAction(RepeatForever::create(fd2));
			}

		}
	}

}

void ActionButton::updateProgressMark(){
	float ckr=atof(_delegate->_delegate->currentPlayer->getCKR()->getCString());
	float ckr2=atof(_delegate->_delegate->currentPlayer->getCKR2()->getCString());
	if (this->getABType()== OUGIS1){
		if(ckr<15000){
			if(proressblinkSprite){
				proressblinkSprite->setRotation(0);
			}
			if(proressmarkSprite){
			proressmarkSprite->setRotation(-1*((ckr/15000)*50));
			}
			if(proressblinkMask){
				proressblinkMask->setVisible(false);
			}
			
		}else if(ckr<30000){
			if(proressblinkSprite){
			proressblinkSprite->setRotation(-50);
			}
			if(proressmarkSprite){
			proressmarkSprite->setRotation(-1*(((ckr-15000)/15000)*70+50));
			}
			if(proressblinkMask && !_isLock){
			
				proressblinkMask->setVisible(true);
				proressblinkMask->stopAllActions();
				FadeOut* fd2=FadeOut::create(0.5f);
				proressblinkMask->runAction(RepeatForever::create(fd2));

				if(_delegate->skill5Button){
					if(_delegate->skill5Button->proressblinkMask && !_delegate->skill5Button->_isLock){
						_delegate->skill5Button->proressblinkMask->stopAllActions();
						FadeOut* fd2=FadeOut::create(0.5f);
						_delegate->skill5Button->proressblinkMask->runAction(RepeatForever::create(fd2));
					}

				}

			}

			


		}else if(ckr<45000) {
			if(proressblinkSprite){
			proressblinkSprite->setRotation(-120);
			}
			if(proressmarkSprite){
			proressmarkSprite->setRotation(-1*(((ckr-30000)/15000)*60+120));
			}
		}else{
			if(proressblinkSprite){
			proressmarkSprite->setRotation(-180);
			}
			if(proressblinkSprite){
				proressblinkSprite->setRotation(-180);
			}
			
		}
	}else{
		if(ckr2<25000){
			if(proressblinkSprite){
			proressblinkSprite->setRotation(0);
			}
			if(proressmarkSprite){
				proressmarkSprite->setRotation(-1*((ckr2/25000)*85));
			}
			if(proressblinkMask){
				proressblinkMask->setVisible(false);
			}
		}else if(ckr2<50000){
			if(proressblinkSprite){
			proressblinkSprite->setRotation(-85);
			}
			if(proressmarkSprite){
				proressmarkSprite->setRotation(-1*(((ckr2-25000)/25000)*90+90));
			}
			
			if(proressblinkMask && !_isLock){
				
				proressblinkMask->setVisible(true);
				proressblinkMask->stopAllActions();
				FadeOut* fd2=FadeOut::create(0.5f);
				proressblinkMask->runAction(RepeatForever::create(fd2));

				if(_delegate->skill4Button){
					if(_delegate->skill4Button->proressblinkMask && !_delegate->skill4Button->_isLock){
						_delegate->skill4Button->proressblinkMask->stopAllActions();
						FadeOut* fd2=FadeOut::create(0.5f);
						_delegate->skill4Button->proressblinkMask->runAction(RepeatForever::create(fd2));
					}

				}
			}

			

		}else{
			if(proressmarkSprite){
				proressmarkSprite->setRotation(-180);
			}
			
			if(proressblinkSprite){
			proressblinkSprite->setRotation(-180);
			}
		}
	}


}

void ActionButton::setLock(){

	if(markSprite){
		markSprite->stopAllActions();
		markSprite->setPercentage(100);
	}else{
		this->setMarkSprite("skill_freeze.png");
	}

	if(proressblinkMask){
		proressblinkMask->setVisible(false);
	}
	_isLock=true;
}

void ActionButton::unLock(){
	if(markSprite){
		markSprite->stopAllActions();
		markSprite->setPercentage(0);
	}

	float ckr=atof(_delegate->_delegate->currentPlayer->getCKR()->getCString());
	float ckr2=atof(_delegate->_delegate->currentPlayer->getCKR2()->getCString());

	if (this->getABType()== OUGIS1){
		if(ckr>=15000){
			if(proressblinkMask){
				proressblinkMask->setVisible(true);
			}
		}
	}else if(this->getABType()== OUGIS2) {
		if(ckr2>=25000){
			if(proressblinkMask){
			proressblinkMask->setVisible(false);
			}
		}
	}
	
	_isLock=false;
}

void ActionButton::createFreezeAnimation(){

	FiniteTimeAction *to = ProgressTo::create(0, 99.999f);
	
	
	int delay=_coldDown/1000;
	
	FiniteTimeAction *to1 = ProgressTo::create(delay, 0);

	Action* freezeAction;
	if(_isDoubleSkill){
		FiniteTimeAction *callback = CallFunc::create(CC_CALLBACK_0(ActionButton::clearClick, this));
		freezeAction= Sequence::create(to, to1, callback, nullptr);
	} else{
		freezeAction= Sequence::create(to, to1, nullptr);
	}
	
	this->setFreezeAction(freezeAction);

};

void ActionButton::clearClick(){
	_clickNum=0;
}

void ActionButton::clearOugisMark(){
	if(this->ougismarkSprite){
		this->ougismarkSprite->removeAllChildrenWithCleanup(true);
	}
	
	if(this->clipper){
		this->clipper->removeFromParentAndCleanup(true);
	}
	if(this->progressPointSprite){
		this->progressPointSprite->removeFromParent();
	}
	if(this->proressblinkMask){
		this->proressblinkMask->removeFromParent();
	}
	

}


ActionButton*  ActionButton::create( const char *szImage ){
	 ActionButton* ab=new  ActionButton();
	if (ab && ab->init(szImage))
	{
		ab->autorelease();
		return ab;
	}
	else
	{
		delete ab;
		return NULL;
	}
}

