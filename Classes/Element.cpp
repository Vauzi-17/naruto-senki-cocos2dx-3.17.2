#include "Characters.h"
#include <cstdlib>
#include <string>
#include "HudLayer.h"
USING_NS_CC;

namespace
{
CCString* createElementIntString(int value)
{
	return CCString::create(cocos2d::StringUtils::format("%d", value));
}

CCString* createElementFloatString(float value)
{
	return CCString::create(cocos2d::StringUtils::format("%f", value));
}
}

HeroElement::HeroElement(void)
{
	rebornSprite=NULL;
}


HeroElement::~HeroElement(void)
{
	CC_SAFE_RELEASE(callValue);
	CC_SAFE_RELEASE(_nattackType);
	CC_SAFE_RELEASE(_sattackType1);
	CC_SAFE_RELEASE(_sattackType2);
	CC_SAFE_RELEASE(_sattackType3);
	CC_SAFE_RELEASE(_sattackType4);
	CC_SAFE_RELEASE(_sattackType5);
	CC_SAFE_RELEASE(_spcattackType1);
	CC_SAFE_RELEASE(_spcattackType2);
	CC_SAFE_RELEASE(_spcattackType3);
	CC_SAFE_RELEASE(_damageArray);
	CC_SAFE_RELEASE(_monsterArray);
	CC_SAFE_DELETE(skillSPC1Array);
	CC_SAFE_RELEASE(skillSPC2Array);
	CC_SAFE_RELEASE(skillSPC3Array);
	CC_SAFE_RELEASE(skillSPC4Array);
	CC_SAFE_RELEASE(skillSPC5Array);
	CC_SAFE_RELEASE(nattackArray);
	CC_SAFE_RELEASE(walkArray);
	CC_SAFE_RELEASE(knockDownArray);
	CC_SAFE_RELEASE(skill1Array);
	CC_SAFE_RELEASE(skill2Array);
	CC_SAFE_RELEASE(skill3Array);
	CC_SAFE_RELEASE(skill4Array);
	CC_SAFE_RELEASE(skill5Array);
	CC_SAFE_RELEASE(idleArray);

}

bool HeroElement::init(){
	bool bRet = false;
	do {

		CC_BREAK_IF(!NarutoSenki::ActionManager::init());
		this->setAnchorPoint(Vec2(0.5,0));
		this->scheduleUpdate();
		//this->schedule(CC_SCHEDULE_SELECTOR(HeroElement::neicun),0.5f);
		bRet=true;
	}while(0);

	return bRet;

};

void HeroElement::setID(CCString* character,CCString* role,CCString* group){

	this->setCharacter(character);
	this->setRole(role);
	this->setGroup(group);

	CCArray* animationArray=CCArray::create();
	std::string filePath;

	KTools* tool= KTools::create();

	filePath=cocos2d::StringUtils::format("Element/%s/%s.xml",this->getCharacter()->getCString(),this->getCharacter()->getCString());

	std::string key =tool->getKeycode(filePath.c_str());
	int id=tool->checkMD5(filePath.c_str());

	if(strcmp(key.c_str(),_delegate->KeyList[id])!=0){
		//return;
	}







	tool->readXMLToArray(filePath.c_str(),animationArray);

	SpriteFrame* firstFrame=NULL;
	if(animationArray->count()>0){
		CCArray* firstAction=(CCArray*)animationArray->objectAtIndex(0);
		if(firstAction && firstAction->count()>1){
			CCArray* firstFrames=(CCArray*)firstAction->objectAtIndex(1);
			if(firstFrames && firstFrames->count()>0){
				CCDictionary* firstFrameDict=(CCDictionary*)firstFrames->objectAtIndex(0);
				const char* firstFrameName=firstFrameDict->valueForKey("frameName")->getCString();
				firstFrame=SpriteFrameCache::getInstance()->spriteFrameByName(firstFrameName);
			}
		}
	}
	if(firstFrame){
		this->setSpriteFrame(firstFrame);
		applyPixelArtToSprite(this);
	}

	//init Attribute; & indleFrame

	CCArray* tmpAction=(CCArray*)(animationArray->objectAtIndex(0));
	CCArray* tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	idleArray=(CCArray*)(tmpAction->objectAtIndex(1));

	if(strcmp(this->getCharacter()->getCString(),"Kakashi")==0||
		strcmp(this->getCharacter()->getCString(),"Deidara")==0 ||
		strcmp(this->getCharacter()->getCString(),"Suigetsu")==0 ||
		strcmp(this->getCharacter()->getCString(),"Jugo")==0 ||
		strcmp(this->getCharacter()->getCString(),"Gaara")==0  ||
		strcmp(this->getCharacter()->getCString(),"Itachi")==0 ||
		strcmp(this->getCharacter()->getCString(),"ImmortalSasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"Choji")==0 ||
		strcmp(this->getCharacter()->getCString(),"Tsunade")==0 ||
		strcmp(this->getCharacter()->getCString(),"Hidan")==0 ||
		strcmp(this->getCharacter()->getCString(),"Roshi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Han")==0 ||
		strcmp(this->getCharacter()->getCString(),"Hiruzen")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sai")==0||
		strcmp(this->getCharacter()->getCString(),"Kiba")==0 ||
		strcmp(this->getCharacter()->getCString(),"Akamaru")==0 ||
		strcmp(this->getCharacter()->getCString(),"Konan")==0 ||
		strcmp(this->getCharacter()->getCString(),"Naruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"SageNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"RikudoNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"Kisame")==0
		){
		idleArray->retain();
	}


	CCString* tmpName;
	CCString* tmpHpMax;
	int tmpWidth;
	int tmpHeight;
	unsigned int tmpSpeed;
	int tmpCombatPoint;
	this->readDate(tmpDate,tmpName,tmpHpMax,tmpWidth,tmpHeight,tmpSpeed,tmpCombatPoint);
	this->setMaxHP(createElementIntString(std::atoi(tmpHpMax->getCString())));
	this->setHP(CCString::create(this->getMaxHP()->getCString()));

	this->setHeight(tmpHeight);
	this->setWalkSpeed(tmpSpeed);
	_originSpeed=tmpSpeed;

	if(!this->getCKR() && !this->getCKR2()){
		this->setCKR(CCString::create("0"));
		this->setCKR2(CCString::create("0"));
	}


	//init WalkFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(1));
	walkArray=(CCArray*)(tmpAction->objectAtIndex(1));

	if(strcmp(this->getCharacter()->getCString(),"Konan")==0 ||
		strcmp(this->getCharacter()->getCString(),"Deidara")==0 ||
		strcmp(this->getCharacter()->getCString(),"Suigetsu")==0 ||
		strcmp(this->getCharacter()->getCString(),"Jugo")==0 ||
		strcmp(this->getCharacter()->getCString(),"Itachi")==0 ||
		strcmp(this->getCharacter()->getCString(),"ImmortalSasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"Lee")==0 ||
		strcmp(this->getCharacter()->getCString(),"RockLee")==0 ||
		strcmp(this->getCharacter()->getCString(),"Choji")==0 ||
		strcmp(this->getCharacter()->getCString(),"Tsunade")==0 ||
		strcmp(this->getCharacter()->getCString(),"Hidan")==0 ||
		strcmp(this->getCharacter()->getCString(),"Shino")==0 ||
		strcmp(this->getCharacter()->getCString(),"Roshi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Han")==0||
		strcmp(this->getCharacter()->getCString(),"Hiruzen")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sai")==0||
		strcmp(this->getCharacter()->getCString(),"Kiba")==0 ||
		strcmp(this->getCharacter()->getCString(),"Akamaru")==0 ||
		strcmp(this->getCharacter()->getCString(),"Gaara")==0  ||
		strcmp(this->getCharacter()->getCString(),"Naruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"SageNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"RikudoNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"Tobi")==0||
		strcmp(this->getCharacter()->getCString(),"Parents")==0||
		strcmp(this->getCharacter()->getCString(),"Kisame")==0
		){
			walkArray->retain();
	}


	//init HurtFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(2));
	hurtArray=(CCArray*)(tmpAction->objectAtIndex(1));

	if(strcmp(this->getCharacter()->getCString(),"Sasuke")==0||
		strcmp(this->getCharacter()->getCString(),"ImmortalSasuke")==0
		){
		hurtArray->retain();
	}


	//init AirHurtFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(3));
	airHurtArray=(CCArray*)(tmpAction->objectAtIndex(1));

	//init KnockDownFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(4));
	knockDownArray=(CCArray*)(tmpAction->objectAtIndex(1));

	if(strcmp(this->getCharacter()->getCString(),"Itachi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"Naruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"SageNaruto")==0
		){
		knockDownArray->retain();
	}

	//init FloatFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(5));
	floatArray=(CCArray*)(tmpAction->objectAtIndex(1));

	//init DeadFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(6));
	deadArray=(CCArray*)(tmpAction->objectAtIndex(1));


	//init nAttack date & Frame Array
	tmpAction=(CCArray*)(animationArray->objectAtIndex(7));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	CCString* tmpValue;
	unsigned int tmpCD;
	this->readDate(tmpDate,_nattackType,tmpValue,_nattackRangeX,_nattackRangeY,tmpCD,tmpCombatPoint);
	this->setnAttackValue(tmpValue);
	_nattackType->retain();

	nattackArray=(CCArray *)(tmpAction->objectAtIndex(1));

	if(strcmp(this->getCharacter()->getCString(),"Sai")==0||
		strcmp(this->getCharacter()->getCString(),"Tenten")==0||
		strcmp(this->getCharacter()->getCString(),"Suigetsu")==0 ||
		strcmp(this->getCharacter()->getCString(),"Jugo")==0 ||
		strcmp(this->getCharacter()->getCString(),"Choji")==0 ||
		strcmp(this->getCharacter()->getCString(),"Itachi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Minato")==0 ||
		strcmp(this->getCharacter()->getCString(),"ImmortalSasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"Choji")==0  ||
		strcmp(this->getCharacter()->getCString(),"Tsunade")==0 ||
		strcmp(this->getCharacter()->getCString(),"Hidan")==0 ||
		strcmp(this->getCharacter()->getCString(),"Shino")==0 ||
		strcmp(this->getCharacter()->getCString(),"Roshi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Han")==0||
		strcmp(this->getCharacter()->getCString(),"Hiruzen")==0 ||
		strcmp(this->getCharacter()->getCString(),"Kiba")==0 ||
		strcmp(this->getCharacter()->getCString(),"Akamaru")==0 ||
		strcmp(this->getCharacter()->getCString(),"Naruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"SageNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"RikudoNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"Deidara")==0 ||
		strcmp(this->getCharacter()->getCString(),"Gaara")==0  ||
		strcmp(this->getCharacter()->getCString(),"Sakura")==0 ||
		strcmp(this->getCharacter()->getCString(),"Parents")==0||
		strcmp(this->getCharacter()->getCString(),"Kisame")==0
		){

		nattackArray->retain();
	}


	//init skill1 date & Frame Array
	tmpAction=(CCArray*)(animationArray->objectAtIndex(8));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));

	this->readDate(tmpDate,_sattackType1,tmpValue,_sattackRangeX1,_sattackRangeY1,_sattackcoldDown1,_sattackCombatPoint1);
	this->setsAttackValue1(tmpValue);
	_sattackType1->retain();

	skill1Array=(CCArray *)(tmpAction->objectAtIndex(1));



	//init skill2 date & Frame Array
	tmpAction=(CCArray*)(animationArray->objectAtIndex(9));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	this->readDate(tmpDate,_sattackType2,tmpValue,_sattackRangeX2,_sattackRangeY2,_sattackcoldDown2,_sattackCombatPoint2);
	_sattackType2->retain();
	this->setsAttackValue2(tmpValue);
	skill2Array=(CCArray *)(tmpAction->objectAtIndex(1));

	if(strcmp(this->getCharacter()->getCString(),"Kakashi")==0 ){
		skill1Array->retain();
		skill2Array->retain();
	}else if(strcmp(this->getCharacter()->getCString(),"Minato")==0 ){
		skill1Array->retain();
		skill2Array->retain();
		_sattack1isDouble=true;
	}



	//init skill3 date & Frame Array
	tmpAction=(CCArray*)(animationArray->objectAtIndex(10));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	this->readDate(tmpDate,_sattackType3,tmpValue,_sattackRangeX3,_sattackRangeY3,_sattackcoldDown3,_sattackCombatPoint3);
	_sattackType3->retain();
	this->setsAttackValue3(tmpValue);
	skill3Array=(CCArray *)(tmpAction->objectAtIndex(1));


	//init skill4 date & Frame Array
	tmpAction=(CCArray*)(animationArray->objectAtIndex(11));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	this->readDate(tmpDate,_sattackType4,tmpValue,_sattackRangeX4,_sattackRangeY4,_sattackcoldDown4,_sattackCombatPoint4);
	this->setsAttackValue4(tmpValue);
	_sattackType4->retain();
	skill4Array=(CCArray *)(tmpAction->objectAtIndex(1));

	//init skill5 date & Frame Array

	tmpAction=(CCArray*)(animationArray->objectAtIndex(12));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	this->readDate(tmpDate,_sattackType5,tmpValue,_sattackRangeX5,_sattackRangeY5,_sattackcoldDown5,_sattackCombatPoint5);
	this->setsAttackValue5(tmpValue);
	_sattackType5->retain();
	skill5Array=(CCArray *)(tmpAction->objectAtIndex(1));


	//init specal Frame

	if(
		strcmp(this->getCharacter()->getCString(),"Tenten")==0 ||
		strcmp(this->getCharacter()->getCString(),"DevaPath")==0 ||
		strcmp(this->getCharacter()->getCString(),"Ino")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sakura")==0 ||
		strcmp(this->getCharacter()->getCString(),"Tobi")==0 ||
		strcmp(this->getCharacter()->getCString(),"RockLee")==0
		){
			tmpAction=(CCArray*)(animationArray->objectAtIndex(17));
			tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
			this->readDate(tmpDate,_spcattackType1,tmpValue,_spcattackRangeX1,_spcattackRangeY1,_spcattackcoldDown1,tmpCombatPoint);
			this->setspcAttackValue1(tmpValue);
			_spcattackType1->retain();
			skillSPC1Array=(CCArray *)(tmpAction->objectAtIndex(1));
			skillSPC1Array->retain();

	}else if(strcmp(this->getCharacter()->getCString(),"Sai")==0 ||
		strcmp(this->getCharacter()->getCString(),"Deidara")==0 ||
		strcmp(this->getCharacter()->getCString(),"SageNaruto")==0 ||
			strcmp(this->getCharacter()->getCString(),"RikudoNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"Suigetsu")==0 ||
		strcmp(this->getCharacter()->getCString(),"Jugo")==0 ||
		strcmp(this->getCharacter()->getCString(),"Kakashi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Itachi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Minato")==0 ||
		strcmp(this->getCharacter()->getCString(),"ImmortalSasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sasuke")==0		||
		strcmp(this->getCharacter()->getCString(),"Choji")==0 ||
		strcmp(this->getCharacter()->getCString(),"Tsunade")==0 ||
		strcmp(this->getCharacter()->getCString(),"Gaara")==0 ||
		strcmp(this->getCharacter()->getCString(),"Hidan")==0 ||
		strcmp(this->getCharacter()->getCString(),"Shino")==0 ||
		strcmp(this->getCharacter()->getCString(),"Roshi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Han")==0||
		strcmp(this->getCharacter()->getCString(),"Hiruzen")==0 ||
		strcmp(this->getCharacter()->getCString(),"Kiba")==0 ||
		strcmp(this->getCharacter()->getCString(),"Akamaru")==0 ||
		strcmp(this->getCharacter()->getCString(),"Konan")==0 ||
		strcmp(this->getCharacter()->getCString(),"Naruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"Parents")==0||
		strcmp(this->getCharacter()->getCString(),"Kisame")==0
		){

		tmpAction=(CCArray*)(animationArray->objectAtIndex(17));
		tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
		this->readDate(tmpDate,_spcattackType1,tmpValue,_spcattackRangeX1,_spcattackRangeY1,_spcattackcoldDown1,tmpCombatPoint);
		this->setspcAttackValue1(tmpValue);
		_spcattackType1->retain();
		skillSPC1Array=(CCArray *)(tmpAction->objectAtIndex(1));
		skillSPC1Array->retain();

		tmpAction=(CCArray*)(animationArray->objectAtIndex(18));
		tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
		this->readDate(tmpDate,_spcattackType2,tmpValue,_spcattackRangeX2,_spcattackRangeY2,_spcattackcoldDown2,tmpCombatPoint);
		skillSPC2Array=(CCArray *)(tmpAction->objectAtIndex(1));
		this->setspcAttackValue2(tmpValue);
		_spcattackType2->retain();
		skillSPC2Array->retain();

		tmpAction=(CCArray*)(animationArray->objectAtIndex(19));
		tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
		this->readDate(tmpDate,_spcattackType3,tmpValue,_spcattackRangeX3,_spcattackRangeY3,_spcattackcoldDown3,tmpCombatPoint);
		skillSPC3Array=(CCArray *)(tmpAction->objectAtIndex(1));
		this->setspcAttackValue3(tmpValue);
		_spcattackType3->retain();
		skillSPC3Array->retain();

	}

	if(strcmp(this->getCharacter()->getCString(),"Itachi")==0 ||
		strcmp(this->getCharacter()->getCString(),"Choji")==0 ||
		strcmp(this->getCharacter()->getCString(),"Kiba")==0 ||
		strcmp(this->getCharacter()->getCString(),"Naruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"SageNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"RikudoNaruto")==0 ||
		strcmp(this->getCharacter()->getCString(),"Sasuke")==0||
		strcmp(this->getCharacter()->getCString(),"ImmortalSasuke")==0
		){
		tmpAction=(CCArray*)(animationArray->objectAtIndex(16));
		skillSPC4Array=(CCArray *)(tmpAction->objectAtIndex(1));
		skillSPC4Array->retain();
	}

	if(strcmp(this->getCharacter()->getCString(),"Sasuke")==0 ||
		strcmp(this->getCharacter()->getCString(),"SageNaruto")==0
		){
		tmpAction=(CCArray*)(animationArray->objectAtIndex(15));
		skillSPC5Array=(CCArray *)(tmpAction->objectAtIndex(1));
		skillSPC5Array->retain();
	}


	if(strcmp(this->getCharacter()->getCString(),"Kiba")==0){
		_isBati=true;
	}
	if(strcmp(this->getCharacter()->getCString(),"Kakuzu")==0){
		_heartEffect=Sprite::createWithSpriteFrameName("Heart_Effect_00.png");
		_heartEffect->setPosition(Vec2(this->getContentSize().width+40,70));
		this->addChild(_heartEffect);
	}


	_damageArray=CCArray::create();
	_damageArray->retain();

	if(this->getLV()==1 && !this->getCoin()){
		this->setGearArray(CCArray::create());
		this->setCoin(CCString::create("50"));
	}




	this->initAction();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(NarutoSenki::ActionManager::acceptAttack), "acceptAttack", NULL);

};


void HeroElement::initAction(){

	this->setIdleAction(createAnimation(idleArray,5.0f,true,false));
	this->setWalkAction(createAnimation(walkArray,10.0f,true,false));
	this->setHurtAction(createAnimation(hurtArray,10.0f,false,false));

	this->setAirHurtAction(createAnimation(airHurtArray,10.0f,false,false));
	this->setKnockDownAction(createAnimation(knockDownArray,10.0f,false,true));
	this->setDeadAction(createAnimation(deadArray,10.0f,false,false));
	this->setFloatAction(createAnimation(floatArray,10.0f,false,false));


	this->setNAttackAction(createAnimation(nattackArray,10.0f,false,true));
	this->setSkill1Action(createAnimation(skill1Array,10.0f,false,true));
	this->setSkill2Action(createAnimation(skill2Array,10.0f,false,true));
	this->setSkill3Action(createAnimation(skill3Array,10.0f,false,true));
	this->setSkill4Action(createAnimation(skill4Array,10.0f,false,true));
	this->setSkill5Action(createAnimation(skill5Array,10.0f,false,true));


};

void HeroElement::setShadows(){

	if(!_shadow){
		_shadow=Sprite::createWithSpriteFrameName("shadows.png");
		_shadow->setAnchorPoint(Vec2(0.5,0.5));
		_shadow->setPosition(this->getPosition());
		this->getDelegate()->shadowBatch->addChild(_shadow);

	}
}

void HeroElement::setHPbar(){



	if(strcmp(getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0){
		_hpBar=HPBar::create("hp_bar_r.png");
	} else if((strcmp(getRole()->getCString(),"Com")==0 ||
		strcmp(getRole()->getCString(),"Clone")==0 ||
		strcmp(this->getRole()->getCString(),"Kuilei")==0 ||
		strcmp(this->getRole()->getCString(),"Summon")==0)
		){
		_hpBar=HPBar::create("hp_bar_b.png");
	} else if(strcmp(getRole()->getCString(),"Player")==0){
		_hpBar=HPBar::create("hp_bar.png");
	}
	_hpBar->setPositionY(this->getHeight());
	_hpBar->setDelegate(this);
	this->addChild(_hpBar);
	this->changeHPbar();


};

void HeroElement::applyStartLevel6(){
	static const float expThresholds[]={500.f,1000.f,1500.f,2000.f,2500.f};
	_level=1;
	_exp=0;
	for(unsigned int i=0;i<sizeof(expThresholds)/sizeof(expThresholds[0]);i++){
		_exp=expThresholds[i];
		this->changeHPbar();
	}
	this->setHP(CCString::create(this->getMaxHP()->getCString()));
	if(_hpBar){
		_hpBar->loseHP(this->getHpPercent());
	}
}

void HeroElement::changeHPbar(){

		if(_exp>=500 && _level==1){
			_level=2;
			float newValue=atof(this->getCKR()->getCString())+15001;
			this->setCKR(createElementFloatString(newValue));
			_isCanOugis1=true;
			if(strcmp(this->getRole()->getCString(),"Player")==0){

				this->getDelegate()->setCKRLose(false);

				if(strcmp(this->getCharacter()->getCString(),"Kakuzu")==0 && this->hearts<1){
					if(_delegate->getHudLayer()->skill4Button){
						_delegate->getHudLayer()->skill4Button->setLock();
					}
				}
				if(strcmp(this->getCharacter()->getCString(),"Lee")==0 ){
						if(this->bamen<1){
							if(_delegate->getHudLayer()->skill4Button){
								_delegate->getHudLayer()->skill4Button->setLock();
							}
						}

				}

			}
			int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
			tempMaxHP+=500;
			this->setMaxHP(createElementIntString(tempMaxHP));
			this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+9));
			_rebornTime+=1;
		}else if(_exp>=1000 && _level==2){
			_level=3;
			int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
			tempMaxHP+=1000;
			this->setMaxHP(createElementIntString(tempMaxHP));
				this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+18));
			_rebornTime+=2;
		}else if(_exp>=1500 && _level==3){
			_level=4;
			float newValue=atof(this->getCKR2()->getCString())+25001;
			this->setCKR2(createElementFloatString(newValue));
			_isCanOugis2=true;
			if(strcmp(this->getRole()->getCString(),"Player")==0){

				this->getDelegate()->setCKRLose(true);

				if(strcmp(this->getCharacter()->getCString(),"Kakuzu")==0){
					if(_delegate->getHudLayer()->skill5Button){
						_delegate->getHudLayer()->skill5Button->setLock();
					}
				}

				if(strcmp(this->getCharacter()->getCString(),"Lee")==0 ){
					if(this->bamen<3){
						if(_delegate->getHudLayer()->skill5Button){
							_delegate->getHudLayer()->skill5Button->setLock();
						}
					}
				}
			}
			int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
			tempMaxHP+=2000;
			this->setMaxHP(createElementIntString(tempMaxHP));
				this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+27));
			_rebornTime+=3;

		}else if(_exp>=2000 && _level==4){
			_level=5;
			int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
			tempMaxHP+=2500;
			this->setMaxHP(createElementIntString(tempMaxHP));
				this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+36));;
			_rebornTime+=4;
		}else if(_exp>=2500 && _level==5){
			_level=6;
			int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
			tempMaxHP+=3000;
			this->setMaxHP(createElementIntString(tempMaxHP));
				this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+45));
			_rebornTime+=5;
		}

		if(_hpBar){
			std::string hpBottomPath=cocos2d::StringUtils::format("hp_bottom%d.png",int(_level));
		SpriteFrame* frame = SpriteFrameCache::getInstance()->spriteFrameByName(hpBottomPath);
			_hpBar->getHPBottom()->setSpriteFrame(frame);
		}

	if(_delegate){
		_delegate->refreshOugisSkillLocks();
	}

}
void HeroElement::setAI(float dt){

}


void HeroElement::neicun(float dt){
	//CCLOG("%d",callValue->retainCount());
}


void HeroElement::dealloc(){

	this->stopAllActions();
	_actionState=ACTION_STATE_DEAD;


	//��ɾ�� ���
	if(strcmp(this->getCharacter()->getCString(),"Minato")!=0){

		if(this->getMonsterArray() && getMonsterArray()->count()>0){
			Ref* pObject;
			for (unsigned int ccIndex = 0; this->getMonsterArray() && ccIndex < this->getMonsterArray()->count(); ++ccIndex){
				pObject = this->getMonsterArray()->objectAtIndex(ccIndex);
				NarutoSenki::ActionManager* mo=(NarutoSenki::ActionManager*) pObject;

				//�����ɫ����monter
				int index=_delegate->_CharacterArray->indexOfObject(mo);
				if(index>=0){
					_delegate->_CharacterArray->removeObjectAtIndex(index);
				}
				NotificationCenter::getInstance()->removeObserver(mo,"acceptAttack");
				mo->stopAllActions();
				mo->unscheduleAllSelectors();
				mo->setActionState(ACTION_STATE_DEAD);
				mo->removeFromParent();
				mo=NULL;

			};
			this->getMonsterArray()->removeAllObjects();
			_monsterArray=NULL;

		}

	}

	if(strcmp(this->getRole()->getCString(),"Clone")==0 ||
			strcmp(this->getRole()->getCString(),"Kuilei")==0 ||
			strcmp(this->getRole()->getCString(),"Summon")==0
			){
			this->unschedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setAI));
			NotificationCenter::getInstance()->removeObserver(this,"acceptAttack");

			//�Ƴ�����ɫ��
			int index=_delegate->_CharacterArray->indexOfObject(this);
			if(index>=0){
				_delegate->_CharacterArray->removeObjectAtIndex(index);
			}

			if(_master &&  this->_master->getMonsterArray()){
				int index=_master->getMonsterArray()->indexOfObject(this);
				if(index>=0){
					_master->getMonsterArray()->removeObjectAtIndex(index);
				}
			}


			if(strcmp(this->getCharacter()->getCString(),"DevaPath")==0){

				this->getMaster()->_skillChangeBuffValue=NULL;

				if(strcmp(this->getMaster()->getRole()->getCString(),"Player")==0){
					if(this->_delegate->getHudLayer()->skill5Button){
						this->_delegate->getHudLayer()->skill5Button->unLock();
					}

				}

			}else if(strcmp(this->getCharacter()->getCString(),"Akamaru")==0 ||
				strcmp(this->getCharacter()->getCString(),"Karasu")==0||
				strcmp(this->getCharacter()->getCString(),"Parents")==0
				){
				_master->setActionResume();

			}else if(strcmp(this->getCharacter()->getCString(),"Sanshouuo")==0){

				if(strcmp(this->getMaster()->getRole()->getCString(),"Player")==0){

					if(this->_delegate->getHudLayer()->skill4Button){
					this->_delegate->getHudLayer()->skill4Button->unLock();
					}

				}

			}else if(strcmp(this->getCharacter()->getCString(),"MaskFudon")==0 ||
				strcmp(this->getCharacter()->getCString(),"MaskRaidon")==0 ||
				strcmp(this->getCharacter()->getCString(),"MaskKadon")==0
				){

				if(_master->hearts>0){
					if(strcmp(_master->getRole()->getCString(),"Player")==0){
						if(_delegate->getHudLayer()->skill4Button){
							_delegate->getHudLayer()->skill4Button->unLock();
						}
					}
				}
			}else if(strcmp(this->getCharacter()->getCString(),"Saso")==0){

				if(strcmp(this->getMaster()->getRole()->getCString(),"Player")==0){

					if(this->_delegate->getHudLayer()->skill5Button){
						this->_delegate->getHudLayer()->skill5Button->unLock();
					}

				}
			}

			this->removeFromParentAndCleanup(true);

		}else{


			if(strcmp(this->getCharacter()->getCString(),"Kankuro")==0){


				if(strcmp(this->getRole()->getCString(),"Player")==0){

					if(this->_delegate->getHudLayer()->skill4Button){
						this->_delegate->getHudLayer()->skill4Button->unLock();
					}
					if(this->_delegate->getHudLayer()->skill5Button){
					this->_delegate->getHudLayer()->skill5Button->unLock();
					}
				}

			}else if(strcmp(this->getCharacter()->getCString(),"Shikamaru")==0 ||
				strcmp(this->getCharacter()->getCString(),"Choji")==0
				){

				Ref* pObject;
				for (unsigned int ccIndex = 0; _delegate->_CharacterArray && ccIndex < _delegate->_CharacterArray->count(); ++ccIndex){
					pObject = _delegate->_CharacterArray->objectAtIndex(ccIndex);
					Hero* tempHero=(Hero*) pObject;
					if(tempHero->_isSticking){
						if(tempHero->getActionState()!=ACTION_STATE_DEAD){
							tempHero->removeLostBlood(0.1f);
							tempHero->idle();
						}

					}

				};
			}else if(strcmp(this->getCharacter()->getCString(),"Hidan")==0){

				if(strcmp(this->getRole()->getCString(),"Player")==0 ){
					this->_delegate->getHudLayer()->skill1Button->unLock();
				}
			}


			//��ɾ�� ���
			if(strcmp(this->getCharacter()->getCString(),"Minato")!=0 ){

				if(this->getMonsterArray() && getMonsterArray()->count()>0){
					Ref* pObject;
					for (unsigned int ccIndex = 0; this->getMonsterArray() && ccIndex < this->getMonsterArray()->count(); ++ccIndex){
						pObject = this->getMonsterArray()->objectAtIndex(ccIndex);
						NarutoSenki::ActionManager* mo=(NarutoSenki::ActionManager*) pObject;

						//�����ɫ����monter
						int index=_delegate->_CharacterArray->indexOfObject(mo);
						if(index>=0){
							_delegate->_CharacterArray->removeObjectAtIndex(index);
						}
						NotificationCenter::getInstance()->removeObserver(mo,"acceptAttack");
						mo->stopAllActions();
						mo->unscheduleAllSelectors();
						mo->setActionState(ACTION_STATE_DEAD);
						mo->removeFromParent();

					};
					this->getMonsterArray()->removeAllObjects();
					_monsterArray->release();
					_monsterArray=NULL;


				}



			}

			if(strcmp(this->getCharacter()->getCString(),"Roshi")!=0 &&
				strcmp(this->getCharacter()->getCString(),"Han")!=0
				){
					if(rebornLabelTime==3){
						this->scheduleOnce(CC_SCHEDULE_SELECTOR(HeroElement::reborn),3.0f);
					}else{
						rebornLabelTime=this->getRebornTime();
						this->scheduleOnce(CC_SCHEDULE_SELECTOR(HeroElement::reborn),this->getRebornTime());
					}



					//�����ʱ
					if(!this->rebornSprite){

						rebornSprite=Sprite::create();
						Sprite* skullSpirte=Sprite::createWithSpriteFrameName("skull.png");
						skullSpirte->setPosition(Vec2(0,0));
						rebornSprite->addChild(skullSpirte);

						std::string rebornText=cocos2d::StringUtils::format("%d",rebornLabelTime);
						rebornLabel=LabelBMFont::create(rebornText,"Fonts/1.fnt");
						rebornLabel->setScale(0.3f);
						rebornLabel->setPosition(Vec2(skullSpirte->getContentSize().width,0));
						rebornSprite->addChild(rebornLabel);

						rebornSprite->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
						this->addChild(rebornSprite);

					}
					this->schedule(CC_SCHEDULE_SELECTOR(HeroElement::countDown),1);

			}



		}

}
void HeroElement::countDown(float dt){

		rebornLabelTime-=1;
		std::string rebornText=cocos2d::StringUtils::format("%d",rebornLabelTime);
		rebornLabel->setString(rebornText);


}
void HeroElement::reborn(float dt){


	this->setPosition(this->getSpawnPoint());


	if(this->getPosition().equals(this->getSpawnPoint()))
	{
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(NarutoSenki::ActionManager::acceptAttack), "acceptAttack", NULL);
		this->setOpacity(255);
		this->setHP(CCString::create(this->getMaxHP()->getCString()));
		if(_delegate && _delegate->_isDuelMode && _delegate->currentPlayer){
			if(strcmp(this->getRole()->getCString(),"Player")==0){
				_delegate->setHPLose(this->getHpPercent());
			}else if(strcmp(this->getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0){
				_delegate->getHudLayer()->setEnemyHPLose(this->getHpPercent());
			}
		}
		this->setHPbar();
		this->_actionState=ACTION_STATE_HURT;




		if(this->getLV()<4){
			if(strcmp(this->getGroup()->getCString(),"Konoha")==0){
				this->setEXP(this->getEXP()+this->getDelegate()->kEXPBound);
			}else{
				this->setEXP(this->getEXP()+this->getDelegate()->aEXPBound);
			}

			this->changeHPbar();

			if(strcmp(this->getRole()->getCString(),"Player")==0){
				_delegate->getHudLayer()->setEXPLose(NULL);
			}

		}

		this->idle();
		if(rebornSprite){
			this->unschedule(CC_SCHEDULE_SELECTOR(HeroElement::countDown));
			rebornSprite->removeFromParent();
			rebornSprite=NULL;
		}
		if(strcmp(this->getRole()->getCString(),"Player")!=0){
			if(!_delegate || !_delegate->_isTrainingMode){
				bool ksan=true;
				bool asan=true;
				if(CError==1){
					if(_delegate->zhenying>0) {
						ksan=false;
					}else{
						asan=false;
					}
				}
				if(strcmp(this->getGroup()->getCString(),"Konoha")==0 && ksan){
					this->doAI();
				}else if(strcmp(this->getGroup()->getCString(),"Akatsuki")==0  && asan){
					this->doAI();
				}
			}

		}else{
			if(this->_isAI){
				this->doAI();
			}
			_delegate->getHudLayer()->status_hpbar->setOpacity(255);
			_delegate->setHPLose(this->getHpPercent());
		}
		this->scheduleUpdate();
	}
	_delegate->reorderChild(this,-this->getPositionY());

}




/*---------------
init flog
---------------*/

Flog::Flog(void)
{
	_randomPosX=NULL;
	_randomPosY=NULL;
}


Flog::~Flog(void)
{
	CC_SAFE_RELEASE(callValue);
	CC_SAFE_RELEASE(_nattackType);
	CC_SAFE_RELEASE(_damageArray);
}


bool Flog::init(){
	bool bRet = false;
	do {

		CC_BREAK_IF(!NarutoSenki::ActionManager::init());
		this->setAnchorPoint(Vec2(0.5,0));
		this->scheduleUpdate();
		bRet=true;
	}while(0);

	return bRet;

};

void Flog::setID(CCString* character,CCString* role,CCString* group){

	this->setCharacter(character);
	this->setRole(role);
	this->setGroup(group);

	CCArray* animationArray=CCArray::create();
	std::string filePath;


	filePath=cocos2d::StringUtils::format("Element/Flog/%s.xml",this->getCharacter()->getCString());


	KTools* tool= KTools::create();


	tool->readXMLToArray(filePath.c_str(),animationArray);

	//init Attribute; & indleFrame

	CCArray* tmpAction=(CCArray*)(animationArray->objectAtIndex(0));
	CCArray* tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	idleArray=(CCArray*)(tmpAction->objectAtIndex(1));

	CCString* tmpName;
	CCString* tmpHpMax;
	int tmpWidth;
	int tmpHeight;
	unsigned int tmpSpeed;
	int tmpCombatPoint;

	this->readDate(tmpDate,tmpName,tmpHpMax,tmpWidth,tmpHeight,tmpSpeed,tmpCombatPoint);
	this->setMaxHP(createElementIntString(std::atoi(tmpHpMax->getCString())));
	this->setHP(CCString::create(this->getMaxHP()->getCString()));
	this->setHeight(tmpHeight);
	this->setWalkSpeed(tmpSpeed);

	this->setKillNum(0);



	if(!this->getCKR() && !this->getCKR2()){
		this->setCKR(CCString::create("0"));
		this->setCKR2(CCString::create("0"));
	}

	//init WalkFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(1));
	walkArray=(CCArray*)(tmpAction->objectAtIndex(1));


	//init HurtFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(2));
	hurtArray=(CCArray*)(tmpAction->objectAtIndex(1));

	//init AirHurtFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(3));
	airHurtArray=(CCArray*)(tmpAction->objectAtIndex(1));

	//init KnockDownFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(4));
	knockDownArray=(CCArray*)(tmpAction->objectAtIndex(1));

	//init FloatFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(5));
	floatArray=(CCArray*)(tmpAction->objectAtIndex(1));

	//init DeadFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(6));
	deadArray=(CCArray*)(tmpAction->objectAtIndex(1));


	//init nAttack date & Frame Array
	tmpAction=(CCArray*)(animationArray->objectAtIndex(7));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	unsigned int tmpCD;
	CCString* tmpValue;
	this->readDate(tmpDate,_nattackType,tmpValue,_nattackRangeX,_nattackRangeY,tmpCD,tmpCombatPoint);
	this->setnAttackValue(tmpValue);
	_nattackType->retain();
	nattackArray=(CCArray *)(tmpAction->objectAtIndex(1));


	_damageArray=CCArray::create();
	_damageArray->retain();

	this->setCoin(CCString::create("50"));


	this->initAction();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(NarutoSenki::ActionManager::acceptAttack), "acceptAttack", NULL);

};



void Flog::initAction(){

	this->setIdleAction(createAnimation(idleArray,5.0,true,false));
	this->setWalkAction(createAnimation(walkArray,10.0,true,false));
	this->setHurtAction(createAnimation(hurtArray,10.0,false,true));

	this->setAirHurtAction(createAnimation(airHurtArray,10.0f,false,false));
	this->setKnockDownAction(createAnimation(knockDownArray,10.0f,false,true));
	this->setDeadAction(createAnimation(deadArray,10.0f,false,false));

	this->setFloatAction(createAnimation(floatArray,10.0f,false,false));

	this->setNAttackAction(createAnimation(nattackArray,10.0,false,true));


};


void Flog::setHPbar(){
	if(strcmp(getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0){
		_hpBar=HPBar::create("flog_bar_r.png");
	} else{
		_hpBar=HPBar::create("flog_bar.png");
	}
	_hpBar->setPositionY(this->getHeight());
	_hpBar->setDelegate(this);
	this->addChild(_hpBar);
};

void Flog::setAI(float dt){
	if(this->getActionState()==ACTION_STATE_IDLE || this->getActionState()==ACTION_STATE_WALK ||
		this->getActionState()==ACTION_STATE_ATTACK
		){


	if(!_randomPosY){
		_randomPosY=rand()%8+4;
		_randomPosX=(rand()%32+20);
	}


	bool hasTarget=false;

	if(_mainTarget){
		if(_mainTarget->getActionState()!=ACTION_STATE_DEAD
			&& !_mainTarget->_isWudi && _mainTarget->_isVisable &&
			strcmp(_mainTarget->getGroup()->getCString(),this->getGroup()->getCString())!=0
			){

		hasTarget=true;
		}
	}


	if(!hasTarget){

			if(!this->findEnemy("Flog",NULL)){

				if(!this->findEnemy("Hero",64)){

					if(!this->findEnemy("Tower",NULL)){

						_mainTarget=NULL;
					};
				}
			}
	}


	Vec2 moveDirection;

	if(_mainTarget){
		Vec2 sp = _mainTarget->getPosition() - this->getPosition();
			if(abs(sp.x)> _randomPosX || abs(sp.y)>_randomPosY){

				if(abs(sp.x)>64 && strcmp(_mainTarget->getRole()->getCString(),"Flog")!=0 && strcmp(_mainTarget->getRole()->getCString(),"Tower")!=0 ){
					_mainTarget=NULL;
					return;
				}

				moveDirection = sp.getNormalized();
				this->walk(moveDirection);
			}else{
				if( this->getActionState()!=ACTION_STATE_ATTACK){
					this->changeSide(sp);
				    this->attack(NAttack);
				}
			}
			return;
	}

	// no target then step on


	if(abs(this->getPositionY()-this->_mainPosY)>8){
		if(strcmp(this->getGroup()->getCString(),"Konoha")==0){
			moveDirection = Vec2(1, this->getPositionY() > this->_mainPosY ? -1 : 1).getNormalized();
		}else{
			moveDirection = Vec2(-1, this->getPositionY() > this->_mainPosY ? -1 : 1).getNormalized();
		}
	}else{
		if(strcmp(this->getGroup()->getCString(),"Konoha")==0){
			 moveDirection = Vec2(1,0).getNormalized();
		}else{
			moveDirection = Vec2(-1,0).getNormalized();
		}
	}
	this->walk(moveDirection);

	}
}


void Flog::dealloc(){
	this->unschedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setAI));
	this->setActionState(ACTION_STATE_DEAD);
	this->stopAllActions();

	if(strcmp(this->getGroup()->getCString(),"Konoha")==0){
		int index=_delegate->_KonohaFlogArray->indexOfObject(this);
		_delegate->_KonohaFlogArray->removeObjectAtIndex(index);
	}else{
		int index=_delegate->_AkatsukiFlogArray->indexOfObject(this);
		_delegate->_AkatsukiFlogArray->removeObjectAtIndex(index);
	};
	this->removeFromParent();

}


/*---------------
init Tower
---------------*/


Tower::Tower(void)
{

}


Tower::~Tower(void)
{



}


bool Tower::init(){
	bool bRet = false;
	do {

		CC_BREAK_IF(!NarutoSenki::ActionManager::init());
		this->setAnchorPoint(Vec2(0.5,0.5));
		this->scheduleUpdate();
		bRet=true;
	}while(0);

	return bRet;

};

void Tower::setID(CCString* character,CCString* role,CCString* group){

	this->setCharacter(character);
	this->setRole(role);
	this->setGroup(group);

	CCArray* animationArray=CCArray::create();
	std::string filePath;


	if(strcmp(this->getCharacter()->getCString(),"KonohaCenter")==0 || strcmp(this->getCharacter()->getCString(),"AkatsukiCenter")==0 ){
		filePath="Element/Tower/CenterDate.xml";

	}else{
		filePath="Element/Tower/TowerDate.xml";
	}

	KTools* tool= KTools::create();
	std::string key =tool->getKeycode(filePath.c_str());
	int id=tool->checkMD5(filePath.c_str());

	if(strcmp(key.c_str(),_delegate->KeyList[id])!=0){
		//return;
	}


	tool->readXMLToArray(filePath.c_str(),animationArray);


	//init Attribute; & indleFrame

	CCArray* tmpAction=(CCArray*)(animationArray->objectAtIndex(0));
	CCArray* tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	idleArray=(CCArray*)(tmpAction->objectAtIndex(1));

	CCString* tmpName;
	CCString* tmpHpMax;
	int tmpWidth;
	int tmpHeight;
	unsigned int tmpSpeed;
	int tmpCombatPoint;

	this->readDate(tmpDate,tmpName,tmpHpMax,tmpWidth,tmpHeight,tmpSpeed, tmpCombatPoint);
	this->setMaxHP(createElementIntString(std::atoi(tmpHpMax->getCString())));
	this->setHP(CCString::create(this->getMaxHP()->getCString()));

	this->setHeight(tmpHeight);
	this->setWalkSpeed(tmpSpeed);


	if(!this->getCKR() && !this->getCKR2()){
		this->setCKR(CCString::create("0"));
		this->setCKR2(CCString::create("0"));
	}

	//init DeadFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(6));
	deadArray=(CCArray*)(tmpAction->objectAtIndex(1));

	this->setCoin(CCString::create("50"));

	this->initAction();

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(NarutoSenki::ActionManager::acceptAttack), "acceptAttack", NULL);

}

void Tower::initAction(){
	this->setDeadAction(createAnimation(deadArray,10.0f,false,false));
	this->setIdleAction(createAnimation(idleArray,5.0,true,false));

};


void Tower::setHPbar(){

	if(strcmp(getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0){
		_hpBar=HPBar::create("hp_bar_r.png");
	}else{
		_hpBar=HPBar::create("hp_bar.png");
	}
	_hpBar->getHPBAR()->setPosition(Vec2(1,1));
	_hpBar->setPositionY(this->getHeight());
	_hpBar->setDelegate(this);
	this->addChild(_hpBar);
};


void Tower::dealloc(){
	this->unschedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setAI));
	this->setActionState(ACTION_STATE_DEAD);
	this->stopAllActions();

	Ref* pObject;
	CCArray* list;
	for(int i=0;i<2;i++){
		if(i==0){
			list=_delegate->_KonohaFlogArray;
		}else if(i==1){
			list=_delegate->_AkatsukiFlogArray;
		}

		for (unsigned int ccIndex = 0; list && ccIndex < list->count(); ++ccIndex){

			pObject = list->objectAtIndex(ccIndex);
			NarutoSenki::ActionManager* tempFlog=(NarutoSenki::ActionManager*) pObject;
			if(tempFlog->_mainTarget){
				if(tempFlog->_mainTarget==(NarutoSenki::ActionManager*)this){
					tempFlog->_mainTarget=NULL;
				}
			}
		}

	}




	int index=_delegate->_TowerArray->indexOfObject(this);
	_delegate->_TowerArray->removeObjectAtIndex(index);
	_delegate->getHudLayer()->setTowerState(this->getCharNO());
	_delegate->checkTower();
	this->removeFromParentAndCleanup(true);
};

/*---------------
init Monster
---------------*/


Monster::Monster(void)
{

}


Monster::~Monster(void)
{
	CC_SAFE_RELEASE(callValue);
	CC_SAFE_RELEASE(_nattackType);
	CC_SAFE_RELEASE(_damageArray);

}

bool Monster::init(){
	bool bRet = false;
	do {

		CC_BREAK_IF(!NarutoSenki::ActionManager::init());
		this->setAnchorPoint(Vec2(0.5,0));
		this->scheduleUpdate();
		bRet=true;
	}while(0);

	return bRet;

};

void Monster::setID(CCString* character,CCString* role,CCString* group){

	this->setCharacter(character);
	this->setRole(role);
	this->setGroup(group);

	CCArray* animationArray=CCArray::create();
	std::string filePath;


	filePath=cocos2d::StringUtils::format("Element/Monster/%s.xml",this->getCharacter()->getCString());


	KTools* tool= KTools::create();
	std::string key =tool->getKeycode(filePath.c_str());

	tool->readXMLToArray(filePath.c_str(),animationArray);

	//init Attribute; & indleFrame

	CCArray* tmpAction=(CCArray*)(animationArray->objectAtIndex(0));
	CCArray* tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	idleArray=(CCArray*)(tmpAction->objectAtIndex(1));

	CCString* tmpName;
	CCString* tmpHpMax;
	int tmpWidth;
	int tmpHeight;
	unsigned int tmpSpeed;
	int  tmpCombatPoint;

	this->readDate(tmpDate,tmpName,tmpHpMax,tmpWidth,tmpHeight,tmpSpeed, tmpCombatPoint);
	this->setMaxHP(createElementIntString(std::atoi(tmpHpMax->getCString())));
	this->setHP(CCString::create(this->getMaxHP()->getCString()));

	this->setHeight(tmpHeight);
	this->setWalkSpeed(tmpSpeed);



	if(!this->getCKR() && !this->getCKR2()){
		this->setCKR(CCString::create("0"));
		this->setCKR2(CCString::create("0"));
	}

	//init WalkFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(1));
	walkArray=(CCArray*)(tmpAction->objectAtIndex(1));


	//init DeadFrame
	tmpAction=(CCArray*)(animationArray->objectAtIndex(6));
	deadArray=(CCArray*)(tmpAction->objectAtIndex(1));


	//init nAttack date & Frame Array
	tmpAction=(CCArray*)(animationArray->objectAtIndex(7));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	unsigned int tmpCD;
	CCString* tmpValue;
	this->readDate(tmpDate,_nattackType,tmpValue,_nattackRangeX,_nattackRangeY,tmpCD,tmpCombatPoint);
	this->setnAttackValue(tmpValue);
	_nattackType->retain();
	nattackArray=(CCArray *)(tmpAction->objectAtIndex(1));

	this->setCoin(CCString::create("50"));

	this->initAction();

};



void Monster::initAction(){

	this->setIdleAction(createAnimation(idleArray,5.0,true,false));
	this->setWalkAction(createAnimation(walkArray,10.0,true,false));
	this->setDeadAction(createAnimation(deadArray,10.0f,false,false));
	if(strcmp(this->getCharacter()->getCString(),"Kage")==0||
		strcmp(this->getCharacter()->getCString(),"KageHand")==0 ||
		strcmp(this->getCharacter()->getCString(),"FudonSRK")==0 ||
		strcmp(this->getCharacter()->getCString(),"FudonSRK2")==0 ||
		strcmp(this->getCharacter()->getCString(),"Kubi")==0
		){
	this->setNAttackAction(createAnimation(nattackArray,10.0,false,false));
	}else{
	this->setNAttackAction(createAnimation(nattackArray,10.0,false,true));
	}


};

void Monster::setHPbar(){

	if(strcmp(getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0){
		_hpBar=HPBar::create("hp_bar_r.png");
	} else {
		_hpBar=HPBar::create("hp_bar_b.png");
	}

	_hpBar->setPositionY(this->getHeight());
	_hpBar->setDelegate(this);
	this->addChild(_hpBar);
	this->changeHPbar();
};

void Monster::changeHPbar(){


	if(_exp>=500 && _level==1){
		_level=2;
		int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
		tempMaxHP+=1000;
		this->setMaxHP(createElementIntString(tempMaxHP));
			this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+9));
	}else if(_exp>=1000 && _level==2){
		_level=3;
		int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
		tempMaxHP+=1500;
		this->setMaxHP(createElementIntString(tempMaxHP));
		this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+18));

	}else if(_exp>=1500 && _level==3){
		_level=4;
		int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
		tempMaxHP+=2000;
		this->setMaxHP(createElementIntString(tempMaxHP));
		this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+27));

	}else if(_exp>=2000 && _level==4){
		_level=5;
		int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
		tempMaxHP+=2500;
		this->setMaxHP(createElementIntString(tempMaxHP));
		this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+36));

	}else if(_exp>=2500 && _level==5){
		_level=6;
		int tempMaxHP=std::atoi(this->getMaxHP()->getCString());
		tempMaxHP+=3000;
		this->setMaxHP(createElementIntString(tempMaxHP));
		this->setnAttackValue(createElementIntString(std::atoi(this->getnAttackValue()->getCString())+48));
	}

	if(_hpBar){
		std::string hpBottomPath=cocos2d::StringUtils::format("hp_bottom%d.png",int(_level));
		SpriteFrame* frame = SpriteFrameCache::getInstance()->spriteFrameByName(hpBottomPath);
		_hpBar->getHPBottom()->setSpriteFrame(frame);
	}


}


void Monster::setAI(float dt){

	if(strcmp(getCharacter()->getCString(),"Kage")==0){

		Ref* pObject;
		for (unsigned int ccIndex = 0; _delegate->_CharacterArray && ccIndex < _delegate->_CharacterArray->count(); ++ccIndex){
			pObject = _delegate->_CharacterArray->objectAtIndex(ccIndex);
			Hero* tempHero=(Hero*) pObject;
			if(strcmp(this->getGroup()->getCString(),tempHero->getGroup()->getCString())!=0 &&
				tempHero->getActionState()!=ACTION_STATE_DEAD &&
				tempHero->getActionState()!=ACTION_STATE_O2ATTACK &&
				!tempHero->_isWudi &&
				!tempHero->_isBati &&
				tempHero->_isVisable
				){

				Vec2 sp = tempHero->getPosition() - this->getPosition();
				float distanceY=tempHero->_originY?abs(this->getPositionY()-tempHero->_originY):abs(sp.y);
				float distanceX=_isFlipped?tempHero->getPositionX() - this->getPositionX() + this->getContentSize().width:tempHero->getPositionX() - this->getPositionX()-this->getContentSize().width;
				if( abs(distanceX)< 32 &&  distanceY< 48){
					if(!this->_monsterArray){
						CCDictionary* dic=CCDictionary::create();
						CCString* monterName=CCString::create("KageHand");
						dic->setObject(monterName,1);
						this->setMon(this,(void*) dic);
						this->unschedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setAI));
					}
				}

			}

		};
		return;
	}

	if(strcmp(getCharacter()->getCString(),"Mouse")==0
		){

		if(!this->findEnemy("Hero",NULL)){
			_mainTarget=NULL;
		}
	}else if(strcmp(getCharacter()->getCString(),"Spider")==0 ||
		strcmp(getCharacter()->getCString(),"ClayBird")==0
		){


			if(!this->findEnemy("Hero",NULL)){
				if(!this->findEnemy("Tower",NULL)){
					_mainTarget=NULL;
				}
			}

	}else if(strcmp(getCharacter()->getCString(),"FudonSRK2")==0||
		strcmp(getCharacter()->getCString(),"FudonSRK")==0){

	}else{

		if(!this->findEnemy("Hero",NULL)){
			if(!this->findEnemy("Flog",NULL)){
				_mainTarget=NULL;
			}
		}
	}


	Vec2 moveDirection;

	if(_mainTarget){

		Vec2 sp;

		sp = Vec2(_mainTarget->getPositionX(), _mainTarget->_originY ? _mainTarget->_originY : _mainTarget->getPositionY())
			- Vec2(this->getPositionX(), _originY ? _originY : this->getPositionY());


		if( strcmp(this->getCharacter()->getCString(),"FudonSRK2")==0 ||
			strcmp(this->getCharacter()->getCString(),"FudonSRK")==0 ){
				if(abs(sp.x)> 48 || abs(sp.y)> 32){
					this->setActionState(ACTION_STATE_WALK);
					moveDirection = sp.getNormalized();
					this->walk(moveDirection);
					return;
				}

				if(strcmp(_mainTarget->getRole()->getCString(),"Player")==0 ||
					strcmp(_mainTarget->getRole()->getCString(),"Com")==0 ){
						this->stopAllActions();
						this->dealloc();
				}

		}else if (strcmp(this->getCharacter()->getCString(),"Traps")==0 ){
			if(abs(sp.x)>32 || abs(sp.y)> 32 ){

			}else{
				this->attack(NAttack);
			}

		}else if(abs(sp.x)> 32 || abs(sp.y)> 16){

			if(strcmp(this->getCharacter()->getCString(),"Dogs")!=0 &&
				strcmp(this->getCharacter()->getCString(),"Mine")!=0 &&
				strcmp(this->getCharacter()->getCString(),"Traps")!=0 &&
				strcmp(this->getCharacter()->getCString(),"Yominuma")!=0 &&
				strcmp(this->getCharacter()->getCString(),"Tsukuyomi")!=0
				){
					moveDirection = sp.getNormalized();

					this->walk(moveDirection);
			}

		}else{

			if(strcmp(this->getCharacter()->getCString(),"Mine") ==0){

				if(strcmp(_mainTarget->getRole()->getCString(),"Player")==0 ||
					strcmp(_mainTarget->getRole()->getCString(),"Com")==0 ){
					this->attack(NAttack);
					this->unschedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setAI));
				}

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

				if(strcmp(getCharacter()->getCString(),"SmallSlug")!=0){
						this->unschedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setAI));
				}

			}


		}



	}else{
		if(strcmp(getCharacter()->getCString(),"Rocket")==0 ||
			strcmp(getCharacter()->getCString(),"Spider")==0 ||
			strcmp(getCharacter()->getCString(),"ClayBird")==0
			){
			this->stepOn();
		}else{
			this->idle();
		}
	}

}


void Monster::dealloc(){

	this->unschedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setAI));
	this->setActionState(ACTION_STATE_DEAD);
	this->stopAllActions();

	if(strcmp(this->getCharacter()->getCString(),"FudonSRK")==0||
		strcmp(this->getCharacter()->getCString(),"FudonSRK2")==0
		){

			FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(Monster::dealloc2, this));
			auto deadAction=dynamic_cast<FiniteTimeAction*>(this->getDeadAction());
			Action* seq=deadAction ? Sequence::create(deadAction,call,nullptr) : Sequence::create(call,nullptr);
			this->runAction(seq);
			return;

	}

	//��������
	if(strcmp(this->getCharacter()->getCString(),"HiraishinMark")==0){

		_master->_isCanSkill1=false;
		_master->setActionResume();
		_master->scheduleOnce(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::enableSkill1),_sattackcoldDown1);

		if(strcmp(_master->getRole()->getCString(),"Player")==0){
			SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName("Minato_skill1.png");
			_delegate->getHudLayer()->skill1Button->setSpriteFrame(frame);
			_delegate->getHudLayer()->skill1Button->_clickNum++;
			_delegate->clearDoubleClick();
		}

		Ref* pObject;
		for (unsigned int ccIndex = 0; _master->getMonsterArray() && ccIndex < _master->getMonsterArray()->count(); ++ccIndex){
			pObject = _master->getMonsterArray()->objectAtIndex(ccIndex);
			Monster* mo=(Monster*) pObject;

			if(strcmp(mo->getCharacter()->getCString(),"HiraishinMark")==0){
				int index=_master->getMonsterArray()->indexOfObject(mo);
				_master->getMonsterArray()->removeObjectAtIndex(index);
				mo->removeFromParentAndCleanup(true);
			}

		};

		return;

	}

	if(strcmp(this->getCharacter()->getCString(),"SmallSlug")==0){
		if(_secmaster && _secmaster->getMonsterArray()){
			int index=_secmaster->getMonsterArray()->indexOfObject(this);
			if(index>=0){
				_secmaster->getMonsterArray()->removeObjectAtIndex(index);
			}

		}

	}else {

		if(_master &&  _master->getMonsterArray()){
			int index=_master->getMonsterArray()->indexOfObject(this);
			if(index>=0){
				_master->getMonsterArray()->removeObjectAtIndex(index);
			}

		}

	}

	if(strcmp(this->getCharacter()->getCString(),"KageHand")==0 ||
		strcmp(this->getCharacter()->getCString(),"Kage")==0
		){

		FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(Monster::dealloc2, this));


		auto deadAction=dynamic_cast<FiniteTimeAction*>(this->getDeadAction());

		FiniteTimeAction* call2 = CallFunc::create(CC_CALLBACK_0(Monster::setResume, this));

		Action* seq=deadAction ? Sequence::create(deadAction,call,call2,nullptr) : Sequence::create(call,call2,nullptr);
		this->runAction(seq);


	}else{

		this->removeFromParentAndCleanup(true);

	}



}


void Monster::setDirectMove(unsigned int length,float delay,bool isReverse){

	Vec2 direction=Vec2(this->_isFlipped?this->getPosition().x-length:this->getPosition().x+length,
		this->getPositionY());
	Vec2 direction2=this->getPosition();
	ActionInterval* mv=MoveTo::create(delay,direction);
	FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(Monster::dealloc, this));
	Action* seq;
	if (!isReverse) {
		seq=Sequence::create(mv,call,nullptr);
	}else{
		ActionInterval* mv2=MoveTo::create(delay,direction2);
		seq=Sequence::create(mv,mv2,call,nullptr);
	}

	this->runAction(seq);


}
void Monster::setEaseIn(unsigned int length,float delay){

	Vec2 direction=Vec2(this->_isFlipped?this->getPosition().x-length:this->getPosition().x+length,
		this->getPositionY());
	ActionInterval* mv=MoveTo::create(1.0f,direction);
	ActionInterval* eo=EaseIn::create(mv,delay);

	FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(Monster::dealloc, this));
	Action* seq=Sequence::create(eo,call,nullptr);
	this->runAction(seq) ;
}

void Monster::setDirectMoveBy(unsigned int length,float delay){

	Vec2 direction=Vec2(this->_isFlipped?this->getPosition().x-length:this->getPosition().x+length,
		this->getPositionY());

	if(_mainTarget ){

		ActionInterval* mv=MoveBy::create(0.1f,Vec2(_mainTarget->getPositionX()>this->getPositionX()?16:-16,_mainTarget->getPositionY()>this->getPositionY()?16:-16));

		this->runAction(RepeatForever::create(mv));
	_mainTarget =NULL;
	_master->_mainTarget=NULL;
	}else{
		ActionInterval* mv=MoveBy::create(0.1f,Vec2(this->_isFlipped?-16:16,0));

		this->runAction(RepeatForever::create(mv));
	}

	DelayTime* dl=DelayTime::create(delay);
	FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(Monster::dealloc, this));
	Action* seq=Sequence::create(dl,call,nullptr);
	this->runAction(seq) ;

}



void Monster::setResume(){

	if(this->getSecMaster()){
		this->getSecMaster()->dealloc();
	}else{

		if(this->getMaster() && strcmp(this->getMaster()->getCharacter()->getCString(),"Shikamaru")==0){
			this->getMaster()->idle();
		}

	}
}
void Monster::dealloc2(){
	this->removeFromParentAndCleanup(true);
}

/*---------------
init Bullet
---------------*/


Bullet::Bullet(void)
{

}


Bullet::~Bullet(void)
{


}

bool Bullet::init(){
	bool bRet = false;
	do {

		//CC_BREAK_IF(!::ActionManager::initWithSpriteFrameName("coin.png"));
		CC_BREAK_IF(!NarutoSenki::ActionManager::init());
		this->scheduleUpdate();
		bRet=true;
	}while(0);

	return bRet;

};

void Bullet::setID(CCString* character,CCString* role,CCString* group){



	this->setCharacter(character);
	this->setRole(role);
	this->setGroup(group);

	CCArray* animationArray=CCArray::create();
	std::string filePath;


	filePath=cocos2d::StringUtils::format("Element/Bullet/%s.xml",this->getCharacter()->getCString());


	KTools* tool= KTools::create();
	std::string key =tool->getKeycode(filePath.c_str());
	int id=tool->checkMD5(filePath.c_str());

	if(strcmp(key.c_str(),_delegate->KeyList[id])!=0){
		//return;
	}


	tool->readXMLToArray(filePath.c_str(),animationArray);

	//init Attribute; & idleFrame

	CCArray* tmpAction=(CCArray*)(animationArray->objectAtIndex(0));
	CCArray* tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	idleArray=(CCArray*)(tmpAction->objectAtIndex(1));

	//init nAttack date & Frame Array
	tmpAction=(CCArray*)(animationArray->objectAtIndex(7));
	tmpDate=(CCArray *)(tmpAction->objectAtIndex(0));
	unsigned int tmpCD;
	int tmpCombatPoint;
	CCString* tmpValue;

	this->readDate(tmpDate,_nattackType,tmpValue,_nattackRangeX,_nattackRangeY,tmpCD,tmpCombatPoint);
	this->setnAttackValue(tmpValue);
	_nattackType->retain();
	nattackArray=(CCArray *)(tmpAction->objectAtIndex(1));

	if(strcmp(this->getCharacter()->getCString(),"Amaterasu")==0 ){
		//init DeadFrame
		tmpAction=(CCArray*)(animationArray->objectAtIndex(6));
		deadArray=(CCArray*)(tmpAction->objectAtIndex(1));
	}

	this->setCoin(CCString::create("50"));

	this->initAction();

};



void Bullet::initAction(){

	this->setIdleAction(createAnimation(idleArray,5.0,true,false));
	this->setNAttackAction(createAnimation(nattackArray,10.0,true,false));
	if(strcmp(this->getCharacter()->getCString(),"Amaterasu")==0 ||
		strcmp(this->getCharacter()->getCString(),"SusanoTama")==0
		){
		this->setDeadAction(createAnimation(deadArray,10.0f,false,false));
	}

};


void Bullet::dealloc(){

	this->stopAllActions();


	if(strcmp(this->getCharacter()->getCString(),"Amaterasu")==0
		){

		FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(Bullet::dealloc2, this));
		auto deadAction=dynamic_cast<FiniteTimeAction*>(this->getDeadAction());
		Action* seq=deadAction ? Sequence::create(deadAction,call,nullptr) : Sequence::create(call,nullptr);
		this->runAction(seq);


	}else{

		if(strcmp(this->getCharacter()->getCString(),"HiraishinKunai")==0){

			if(_master->getActionState()==ACTION_STATE_SATTACK){
					_master->setActionState(ACTION_STATE_ATTACK);
					_master->_markPoint=Vec2(this->getPositionX(),this->_originY);
					_master->changeAction2();
					_master->_isCanSkill2=true;
					_master->sAttack(SKILL2);

			}
		}

		if(_master &&  _master->getMonsterArray()){
			int index=_master->getMonsterArray()->indexOfObject(this);
			if(index>=0){
				_master->getMonsterArray()->removeObjectAtIndex(index);
			}
		}
		this->removeFromParentAndCleanup(true);
	}

}

void Bullet::dealloc2(){

	this->removeFromParentAndCleanup(true);

}

void Bullet::setMove(unsigned int length,float delay,bool isReverse){
	Vec2 direction=Vec2(this->_isFlipped?this->getPosition().x-length:this->getPosition().x+length,
		this->getPositionY());
	Vec2 direction2=this->getPosition();
	ActionInterval* mv=MoveTo::create(delay,direction);
	FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(Bullet::dealloc, this));
	Action* seq;
	if (!isReverse) {

		seq=Sequence::create(mv,call,nullptr);
	}else{
		ActionInterval* mv2=MoveTo::create(delay,direction2);
		seq=Sequence::create(mv,mv2,call,nullptr);
	}

	this->runAction(seq);
}

void Bullet::setEaseIn(unsigned int length,float delay){

	Vec2 direction=Vec2(this->_isFlipped?this->getPosition().x-length:this->getPosition().x+length,
		this->getPositionY());
	ActionInterval* mv=MoveTo::create(1.0f,direction);
	ActionInterval* eo=EaseIn::create(mv,delay);

    FiniteTimeAction* call = CallFunc::create(CC_CALLBACK_0(Bullet::dealloc, this));
	Action* seq=Sequence::create(eo,call,nullptr);
	this->runAction(seq) ;
}

void Bullet::setAttack(float dt){
	this->attack(NAttack);
}


