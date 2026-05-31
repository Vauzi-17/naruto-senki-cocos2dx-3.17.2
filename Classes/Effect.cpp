#include "Effect.h"
#include "MyUtils/KTools.h"
#include "ActionManager.h"
using namespace CocosDenshion;

Effect::Effect(void)
{
}


Effect::~Effect(void)
{
}


bool Effect::init(const char *szImage,Ref* Attacker){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());

		at=(NarutoSenki::ActionManager*) Attacker;

		if(strcmp(szImage,"n_hit")==0 ||
			strcmp(szImage,"b_hit")==0 ){
				this->initWithSpriteFrameName("red_damge_01.png");
				this->setAnchorPoint(Vec2(0.5f,0));
				this->setScale(0.6f);
				float randomY=rand()%16;
				float randomX=rand()%4;
				this->setPosition(Vec2(at->getPositionX()+randomX,at->getPositionY()+randomY));
				Action* effectAction=this->createEffectAnimation("red_damge_",4,14,false);
				this->runAction(effectAction);
		}else if(strcmp(szImage,"l_hit")==0 ||
			strcmp(szImage,"f_hit")==0 ||
			strcmp(szImage,"f2_hit")==0 ||
			strcmp(szImage,"c_hit")==0 ||
			strcmp(szImage,"o_hit")==0 ||
			strcmp(szImage,"o2_hit")==0 ||
			strcmp(szImage,"ob_hit")==0 ||
			strcmp(szImage,"ab_hit")==0 ||
			strcmp(szImage,"ac_hit")==0 ||
			strcmp(szImage,"ct_hit")==0 ||
			strcmp(szImage,"ts_hit")==0 ||
			strcmp(szImage,"s_hit")==0
			){
				this->setScale(0.8f);
				this->initWithSpriteFrameName("red_damge_01.png");
				this->setAnchorPoint(Vec2(0.5f,0));
				this->setPosition(Vec2(at->getPositionX(),at->getPositionY()));
				Action* effectAction=this->createEffectAnimation("red_damge_",4,14,false);
				this->runAction(effectAction);
		}else if(strcmp(szImage,"bl_hit")==0 ||
			strcmp(szImage,"bc_hit")==0 ||
			strcmp(szImage,"bf_hit")==0 ||
			strcmp(szImage,"sl_hit")==0
			){
				this->initWithSpriteFrameName("blue_damge_01.png");
				//this->setAnchorPoint(Vec2(0.5f,0));
				this->setScale(0.6f);
				int randomInt=rand()%30+10;
				this->setRotation(randomInt);
				this->setPosition(Vec2(at->getPositionX(),at->getPositionY()+at->getContentSize().height/2));
				Action* effectAction=this->createEffectAnimation("blue_damge_",4,14,false);
				this->runAction(effectAction);
		}else if(strcmp(szImage,"a_hit")==0){
			this->initWithSpriteFrameName("bottom_damage_01.png");
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX()+16,at->getPositionY()));
			Action* effectAction=this->createEffectAnimation("bottom_damage_",6,20,false);
			this->runAction(effectAction);
		}
		//技能特效
		else if(strcmp(szImage,"Kagura")==0){
			this->initWithSpriteFrameName("Kagura_01.png");
			Action* effectAction=this->createEffectAnimation("Kagura_",9,10,false);
			this->setAnchorPoint(Vec2(0.5f,0.5f));
			this->runAction(effectAction);

		}else if(strcmp(szImage,"Hupo")==0){
			this->initWithSpriteFrameName("Hupo_01.png");
			Action* effectAction=this->createEffectAnimation("Hupo_",7,10,false);
			this->setAnchorPoint(Vec2(0.5f,0.5f));
			this->runAction(effectAction);

		}else if(strcmp(szImage,"hp_restore")==0 ||
			strcmp(szImage,"speedUp")==0
			){
			this->initWithSpriteFrameName(CCString::createWithFormat("%s_01.png",szImage)->getCString());
			Action* effectAction=this->createEffectAnimation(CCString::createWithFormat("%s_",szImage)->getCString(),5,5,false);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(NarutoSenki::ActionManager::disableEffect, at));
			CCArray* seqArray=CCArray::create();
			seqArray->addObject(effectAction);
			seqArray->addObject(call);
			Action* seq=cocos2d::cocos2dx2_compat::createSequence(seqArray);
			this->runAction(seq);
		}else if(strcmp(szImage,"smk")==0){
			this->initWithSpriteFrameName("smk_01.png");
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(at->getPosition());
			Action* effectAction=this->createEffectAnimation("smk_",5,10,false);
			this->runAction(effectAction);
			if(UserDefault::getInstance()->getBoolForKey("isVoice")!=false){
				KTools::playEffectOptimized("Audio/Effect/poof.mp3");
			}
		}else if(strcmp(szImage,"tishen")==0){
			this->initWithSpriteFrameName("tishen_01.png");
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(at->getPosition());
			Action* effectAction=this->createEffectAnimation("tishen_",6,10,false);
			this->runAction(effectAction);
		}
		else if(strcmp(szImage,"stun")==0){

			this->initWithSpriteFrameName("smk_01.png");
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX(),at->getPositionY()+at->getContentSize().height-2));
			Action* effectAction=this->createEffectAnimation("stun_",5,5,false);
			this->runAction(effectAction);

		}else if(strcmp(szImage,"DarkFlame")==0 ){

			this->initWithSpriteFrameName("DarkFlame_Effect_01.png");
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX(),at->getPositionY()));
			Action* effectAction=this->createEffectAnimation("DarkFlame_Effect_",5,10,true);
			this->runAction(effectAction);

			DelayTime* delay=DelayTime::create(2.8f);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
			Action* seq=cocos2d::cocos2dx2_compat::createSequence(delay,call,nullptr);
			this->runAction(seq);

		}
		else if(strcmp(szImage,"Bagua")==0){
			this->initWithSpriteFrameName(CCString::createWithFormat("Bagua_01.png",szImage)->getCString());
			Action* effectAction=this->createEffectAnimation(CCString::createWithFormat("Bagua_",szImage)->getCString(),24,10,false);
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX()+2,at->getPositionY()-52));
			this->runAction(effectAction);

		}else if(strcmp(szImage,"Kujiyose")==0){
			this->initWithSpriteFrameName(CCString::createWithFormat(("%s.png"),szImage)->getCString());
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX(),at->getPositionY()-this->getContentSize().height/2));
			DelayTime* delay=DelayTime::create(0.3f);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
			Action* seq=cocos2d::cocos2dx2_compat::createSequence(delay,call,nullptr);
			this->runAction(seq);

		}else if(strcmp(szImage,"kazi")==0){
			this->initWithSpriteFrameName("kazi.png");
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX()+(at->_isFlipped?-32:32),
				at->getPositionY()+at->getContentSize().height/2));
			ActionInterval* su=ScaleBy::create(0.1f,1.2f);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
			Action* seq=cocos2d::cocos2dx2_compat::createSequence(su,su->reverse(),call,nullptr);
			this->runAction(seq);
		}else if(strcmp(szImage,"sharingan")==0 ||
			strcmp(szImage,"sharingan2")==0 ||
			strcmp(szImage,"sharingan3")==0 ||
			strcmp(szImage,"sharingan4")==0
			){
			this->initWithSpriteFrameName(CCString::createWithFormat(("%s.png"),szImage)->getCString());
			this->setPosition(Vec2(at->getPositionX()+(at->_isFlipped?-32:32),
				at->getPositionY()+at->getContentSize().height));
			ActionInterval* rt=RotateBy::create(0.3f,180,180);
			ActionInterval* su=ScaleBy::create(0.2f,1.6f);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
			Action* seq=cocos2d::cocos2dx2_compat::createSequence(rt,su,call,nullptr);
			this->runAction(seq);
		}else if(strcmp(szImage,"Hiraishin")==0||
			strcmp(szImage,"Hiraishin2")==0){
			this->initWithSpriteFrameName(CCString::createWithFormat("%sEffect_01.png",szImage)->getCString());
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(at->getPosition());
			Action* effectAction=this->createEffectAnimation(CCString::createWithFormat("%sEffect_",szImage)->getCString(),4,10,false);
			this->runAction(effectAction);
		}
		//buff特效
		else if(strcmp(szImage,"hBuff")==0 ){
			this->initWithSpriteFrameName(CCString::createWithFormat("%s_Effect_01.png",szImage)->getCString());
			this->setAnchorPoint(Vec2(0.5f,0));
			Action* effectAction=this->createEffectAnimation(CCString::createWithFormat("%s_Effect_",szImage)->getCString(),4,5,true);
			this->runAction(effectAction);
		}
		else if(strcmp(szImage,"hsBuff")==0||
			strcmp(szImage,"tBuff")==0
			){
			this->initWithSpriteFrameName(CCString::createWithFormat("%s_Effect_01.png",szImage)->getCString());
			this->setAnchorPoint(Vec2(0.5f,0));
			Action* effectAction=this->createEffectAnimation(CCString::createWithFormat("%s_Effect_",szImage)->getCString(),13,10,true);
			this->runAction(effectAction);
		}else if(strcmp(szImage,"dcBuff")==0
			){
				this->initWithSpriteFrameName(CCString::createWithFormat("%s_Effect_01.png",szImage)->getCString());
				this->setAnchorPoint(Vec2(0.5f,0));
				Action* effectAction=this->createEffectAnimation(CCString::createWithFormat("%s_Effect_",szImage)->getCString(),11,10,true);
				this->runAction(effectAction);
		}else if(strcmp(szImage,"jdBuff")==0 ||
			strcmp(szImage,"bmBuff")==0
			){
				this->initWithSpriteFrameName(CCString::createWithFormat("%s_Effect_01.png",szImage)->getCString());
				this->setAnchorPoint(Vec2(0.5f,0));
				Action* effectAction=this->createEffectAnimation(CCString::createWithFormat("%s_Effect_",szImage)->getCString(),9,10,true);
				this->runAction(effectAction);
		}
		else if(strcmp(szImage,"dhBuff")==0){

			for(int i=0;i<3;i++){
				Sprite* ef=Sprite::createWithSpriteFrameName("FireEffect_01.png");
				ef->setAnchorPoint(Vec2(0.5f,0));
				if(i==0){
					ef->setPosition(Vec2(10,0));
				}else if(i==1){
					ef->setPosition(Vec2(-10,18));
				}else{
					ef->setPosition(Vec2(10,34));
				}
				Action* effectAction=this->createEffectAnimation("FireEffect_",5,10,true);
				ef->runAction(effectAction);
				this->addChild(ef);
			}
		}

		bRet=true;

	}while(0);

	return bRet;

}

Action* Effect::createEffectAnimation(const char* file,int frameCount,float fps,bool isRepeat){
	CCArray* animeFrames = CCArray::create();
	CCString* str;

	for(int i = 1; i < frameCount; i++){
		str=CCString::createWithFormat("%s%02d.png",file,i);
		SpriteFrame *frame =  SpriteFrameCache::getInstance()->spriteFrameByName(str->getCString());
		animeFrames->addObject(frame);
	}

	Animation* tempAnimation=cocos2d::cocos2dx2_compat::createAnimationWithSpriteFrames(animeFrames, float(1.0 / fps));
	Action* tempAction=Animate::create(tempAnimation);
	CCArray* seqArray=CCArray::createWithObject(tempAction);
	Action* seq;
	if(isRepeat){
		seq=RepeatForever::create(cocos2d::cocos2dx2_compat::createSequence(seqArray));
	}else{

		FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
		seqArray->addObject(call);
		seq=cocos2d::cocos2dx2_compat::createSequence(seqArray);

	}

	return seq;


};
Action* Effect::createFontAnimation(){

	DelayTime* delay=DelayTime::create(0.3f);
	FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeFontEffect, this));
	Action* seq=cocos2d::cocos2dx2_compat::createSequence(delay,call,nullptr);

	return seq;
}

void Effect::removeEffect(){
	if(at->damageEffectCount>0){
		at->damageEffectCount--;
	}
	this->stopAllActions();
	this->removeFromParentAndCleanup(true);

}

void Effect::removeFontEffect(){
	this->stopAllActions();
	this->removeFromParentAndCleanup(true);

}

Effect*  Effect::create(const char *szImage,Ref* Attacker){
	Effect* ef=new  Effect();
	if (ef && ef->init(szImage,Attacker))
	{
		ef->autorelease();
		return ef;
	}
	else
	{
		delete ef;
		return nullptr;
	}
}


