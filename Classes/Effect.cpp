#include "Effect.h"
#include "MyUtils/KTools.h"
#include "ActionManager.h"
#include <string>
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
			std::string framePath=cocos2d::StringUtils::format("%s_01.png",szImage);
			std::string effectPrefix=cocos2d::StringUtils::format("%s_",szImage);
			this->initWithSpriteFrameName(framePath);
			FiniteTimeAction* effectAction=static_cast<FiniteTimeAction*>(this->createEffectAnimation(effectPrefix.c_str(),5,5,false));
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(NarutoSenki::ActionManager::disableEffect, at));
			Action* seq=Sequence::create(effectAction,call,nullptr);
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
			Action* seq=Sequence::create(delay,call,nullptr);
			this->runAction(seq);

		}
		else if(strcmp(szImage,"Bagua")==0){
			this->initWithSpriteFrameName("Bagua_01.png");
			Action* effectAction=this->createEffectAnimation("Bagua_",24,10,false);
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX()+2,at->getPositionY()-52));
			this->runAction(effectAction);

		}else if(strcmp(szImage,"Kujiyose")==0){
			std::string framePath=cocos2d::StringUtils::format("%s.png",szImage);
			this->initWithSpriteFrameName(framePath);
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX(),at->getPositionY()-this->getContentSize().height/2));
			DelayTime* delay=DelayTime::create(0.3f);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
			Action* seq=Sequence::create(delay,call,nullptr);
			this->runAction(seq);

		}else if(strcmp(szImage,"kazi")==0){
			this->initWithSpriteFrameName("kazi.png");
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(Vec2(at->getPositionX()+(at->_isFlipped?-32:32),
				at->getPositionY()+at->getContentSize().height/2));
			ActionInterval* su=ScaleBy::create(0.1f,1.2f);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
			Action* seq=Sequence::create(su,su->reverse(),call,nullptr);
			this->runAction(seq);
		}else if(strcmp(szImage,"sharingan")==0 ||
			strcmp(szImage,"sharingan2")==0 ||
			strcmp(szImage,"sharingan3")==0 ||
			strcmp(szImage,"sharingan4")==0
			){
			std::string framePath=cocos2d::StringUtils::format("%s.png",szImage);
			this->initWithSpriteFrameName(framePath);
			this->setPosition(Vec2(at->getPositionX()+(at->_isFlipped?-32:32),
				at->getPositionY()+at->getContentSize().height));
			ActionInterval* rt=RotateBy::create(0.3f,180,180);
			ActionInterval* su=ScaleBy::create(0.2f,1.6f);
			FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
			Action* seq=Sequence::create(rt,su,call,nullptr);
			this->runAction(seq);
		}else if(strcmp(szImage,"Hiraishin")==0||
			strcmp(szImage,"Hiraishin2")==0){
			std::string framePath=cocos2d::StringUtils::format("%sEffect_01.png",szImage);
			std::string effectPrefix=cocos2d::StringUtils::format("%sEffect_",szImage);
			this->initWithSpriteFrameName(framePath);
			this->setAnchorPoint(Vec2(0.5f,0));
			this->setPosition(at->getPosition());
			Action* effectAction=this->createEffectAnimation(effectPrefix.c_str(),4,10,false);
			this->runAction(effectAction);
		}
		//buff特效
		else if(strcmp(szImage,"hBuff")==0 ){
			std::string framePath=cocos2d::StringUtils::format("%s_Effect_01.png",szImage);
			std::string effectPrefix=cocos2d::StringUtils::format("%s_Effect_",szImage);
			this->initWithSpriteFrameName(framePath);
			this->setAnchorPoint(Vec2(0.5f,0));
			Action* effectAction=this->createEffectAnimation(effectPrefix.c_str(),4,5,true);
			this->runAction(effectAction);
		}
		else if(strcmp(szImage,"hsBuff")==0||
			strcmp(szImage,"tBuff")==0
			){
			std::string framePath=cocos2d::StringUtils::format("%s_Effect_01.png",szImage);
			std::string effectPrefix=cocos2d::StringUtils::format("%s_Effect_",szImage);
			this->initWithSpriteFrameName(framePath);
			this->setAnchorPoint(Vec2(0.5f,0));
			Action* effectAction=this->createEffectAnimation(effectPrefix.c_str(),13,10,true);
			this->runAction(effectAction);
		}else if(strcmp(szImage,"dcBuff")==0
			){
				std::string framePath=cocos2d::StringUtils::format("%s_Effect_01.png",szImage);
				std::string effectPrefix=cocos2d::StringUtils::format("%s_Effect_",szImage);
				this->initWithSpriteFrameName(framePath);
				this->setAnchorPoint(Vec2(0.5f,0));
				Action* effectAction=this->createEffectAnimation(effectPrefix.c_str(),11,10,true);
				this->runAction(effectAction);
		}else if(strcmp(szImage,"jdBuff")==0 ||
			strcmp(szImage,"bmBuff")==0
			){
				std::string framePath=cocos2d::StringUtils::format("%s_Effect_01.png",szImage);
				std::string effectPrefix=cocos2d::StringUtils::format("%s_Effect_",szImage);
				this->initWithSpriteFrameName(framePath);
				this->setAnchorPoint(Vec2(0.5f,0));
				Action* effectAction=this->createEffectAnimation(effectPrefix.c_str(),9,10,true);
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
	Vector<SpriteFrame*> animeFrames;
	for(int i = 1; i < frameCount; i++){
		std::string framePath=cocos2d::StringUtils::format("%s%02d.png",file,i);
		SpriteFrame *frame =  SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
		animeFrames.pushBack(frame);
	}

	Animation* tempAnimation=Animation::createWithSpriteFrames(animeFrames, float(1.0 / fps));
	FiniteTimeAction* tempAction=Animate::create(tempAnimation);
	Action* seq;
	if(isRepeat){
		seq=RepeatForever::create(Sequence::create(tempAction,nullptr));
	}else{

		FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeEffect, this));
		seq=Sequence::create(tempAction,call,nullptr);

	}

	return seq;


};
Action* Effect::createFontAnimation(){

	DelayTime* delay=DelayTime::create(0.3f);
	FiniteTimeAction* call=CallFunc::create(CC_CALLBACK_0(Effect::removeFontEffect, this));
	Action* seq=Sequence::create(delay,call,nullptr);

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


