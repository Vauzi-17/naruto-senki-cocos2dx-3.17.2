#include "SkillLayer.h"
#include <cstdlib>
#include "Defines.h"
#include "SelectLayer.h"
#include "NetworkLayer.h"
USING_NS_CC_EXT;
using namespace CocosDenshion;



/*----------------------
init MenuButton ;
----------------------*/
SelectButton::SelectButton(void)
{

	_btnType=SkillMenu;
	_isCanBuy=false;
	_isCanBuy2=false;
	_isUnlock=false;
	_clickTime=0;
	_isAviable=true;
	_delegate1=NULL;
	_delegate2=NULL;
	_delegate3=NULL;
	_charName=NULL;
}


SelectButton::~SelectButton(void)
{

}

bool SelectButton::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());
		const char* path;

		path=CCString::createWithFormat("%s",szImage)->getCString();
		this->initWithSpriteFrameName(path);
		this->setAnchorPoint(Vec2(0,0));

		bRet=true;

	}while(0);

	return bRet;

}

void SelectButton::onEnter(){
	Sprite::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SelectButton::ccTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(SelectButton::ccTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void SelectButton::onExit(){
	Sprite::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

Rect SelectButton::getRect(){
	Size size=this->getContentSize();
	return Rect(0,0, size.width, size.height);

}

bool SelectButton::containsTouchLocation(Touch *touch){

	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}

bool SelectButton::ccTouchBegan(Touch* touch,Event* event){
	// touch area
	if (!this->containsTouchLocation(touch))
		return false;
	click();
	return true;

}
void SelectButton::ccTouchEnded(Touch* touch,Event* event){

};

void SelectButton::click(){

	if(this->getBtnType()==SkillMenu){
			if(strcmp(_charName->getCString(),"None2")==0 ){
				CCTips *tip=CCTips::create("LimitedChar");
				this->_delegate2->addChild(tip,5000);
			}else if( strcmp(_charName->getCString(),"None")!=0){
			if(!_isAviable && _delegate2 && _delegate2->isHeroConfirmedSelected(_charName->getCString())){
				SimpleAudioEngine::getInstance()->stopAllEffects();
				Ref* pObject;
				CCARRAY_FOREACH(_delegate2->selectArray,pObject){
					SelectButton* selectBtn=(SelectButton*) pObject;
					if(strcmp(selectBtn->getCharName()->getCString(),this->getCharName()->getCString())!=0){
						selectBtn->_clickTime=0;
					}
				}
				if(this->_clickTime!=20){
					SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
					this->_clickTime=20;
					_delegate2->setSelected(this);
				}else if(this->_clickTime==20){
					SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
					_delegate2->unselectHero(_charName->getCString());
				}
				return;
			}
			if(!_isAviable){
				return;
			}
			SimpleAudioEngine::getInstance()->stopAllEffects();

			if(_delegate2){
				if(_delegate2->_isTrainingSelect){
					int teamNeed=_delegate2->_trainingTeamSize-1;
					if(teamNeed<0){
						teamNeed=0;
					}
					bool teamFull=_delegate2->_trainingRandomTeam || (_delegate2->_trainingTeamPicks && (int)_delegate2->_trainingTeamPicks->count()>=teamNeed);
					bool enemyFull=_delegate2->_trainingRandomEnemy || (_delegate2->_trainingEnemyPicks && (int)_delegate2->_trainingEnemyPicks->count()>=_delegate2->_trainingTeamSize);
					if(_delegate2->_playerSelect && teamFull && enemyFull){
						if(!_delegate2->isHeroConfirmedSelected(this->getCharName()->getCString())){
							return;
						}
					}

					Ref* pObject;
					CCARRAY_FOREACH(_delegate2->selectArray,pObject){
						SelectButton* selectBtn=(SelectButton*) pObject;
						if (strcmp(selectBtn->getCharName()->getCString(),this->getCharName()->getCString())!=0){
							selectBtn->_clickTime=0;
						}
					}
					if(this->_clickTime==0){
						if(UserDefault::getInstance()->getBoolForKey("isVoice")!=false){
							SimpleAudioEngine::getInstance()->stopAllEffects();
							CCString* introSRC=CCString::createWithFormat("Audio/Intro/%s.mp3",this->getCharName()->getCString());
							SimpleAudioEngine::getInstance()->playEffect(introSRC->getCString());
						}
						SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
					}else if(this->_clickTime==1){
						SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm2.ogg");
						_isAviable=false;
						this->spriteToGrey();
						this->getParent()->reorderChild(this,500);
					}
					this->_clickTime++;
					_delegate2->setSelected(this);
					return;
				}
				if(_delegate2->_playerSelect && adResult!=1){
					if(!_delegate2->isHeroConfirmedSelected(this->getCharName()->getCString())){
						return;
					}
				}
				if(!_delegate2->_com2Select || _delegate2->isHeroConfirmedSelected(this->getCharName()->getCString())){

					Ref* pObject;
					CCARRAY_FOREACH(_delegate2->selectArray,pObject){
						SelectButton* selectBtn=(SelectButton*) pObject;
						if (strcmp(selectBtn->getCharName()->getCString(),this->getCharName()->getCString())!=0){
							selectBtn->_clickTime=0;
						}
					}
					if(this->_clickTime==0){
						if(UserDefault::getInstance()->getBoolForKey("isVoice")!=false){
							SimpleAudioEngine::getInstance()->stopAllEffects();
							CCString* introSRC=CCString::createWithFormat("Audio/Intro/%s.mp3",this->getCharName()->getCString());
							SimpleAudioEngine::getInstance()->playEffect(introSRC->getCString());
						}
						SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
					}else if(this->_clickTime==1){
						SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm2.ogg");
						_isAviable=false;
						this->spriteToGrey();
						this->getParent()->reorderChild(this,500);
					}

					if(strcmp(_charName->getCString(),"Pain")==0 ||strcmp(_charName->getCString(),"Orochimaru")==0
						){
							CCTips *tip=CCTips::create("LimitedChar");
							this->_delegate2->addChild(tip,5000);

					}else{
						this->_clickTime++;

					}

					_delegate2->setSelected(this);
				}


			}else if(_delegate1){
				SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
				_delegate1->setSelected(this);
			}


		}

	}else{


		_delegate3->setSkillExplain(_btnType);

	}

}


void SelectButton::spriteToGrey(){
	Sprite* black=Sprite::createWithSpriteFrameName("Black_select.png");
	black->setAnchorPoint(Vec2(0.5,0.5));
	black->setPosition(Vec2(17,32));
	this->addChild(black);
	//	"#ifdef GL_ES \n \
	//	precision mediump float; \n \
	//	#endif \n \
	//	uniform sampler2D u_texture; \n \
	//	varying vec2 v_texCoord; \n \
	//	varying vec4 v_fragmentColor; \n \
	//	void main(void) \n \
	//	{ \n \
	//	// Convert to greyscale using NTSC weightings \n \
	//	vec4 col = texture2D(u_texture, v_texCoord);  float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114));  gl_FragColor = vec4(grey, grey, grey, col.a);   \n \
	//	}";

	//CCGLProgram* pProgram = new CCGLProgram();
	//pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
	//this->setShaderProgram(pProgram);
	//pProgram->release();
	//CHECK_GL_ERROR_DEBUG();

	//this->getShaderProgram()->addAttribute(kCCAttributeNamePosition,kCCVertexAttrib_Position);
	//this->getShaderProgram()->addAttribute(kCCAttributeNameColor,kCCVertexAttrib_Color);
	//this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord,kCCVertexAttrib_TexCoords);
	//CHECK_GL_ERROR_DEBUG();

	//this->getShaderProgram()->link();
	//CHECK_GL_ERROR_DEBUG();

	//this->getShaderProgram()->updateUniforms();
	//CHECK_GL_ERROR_DEBUG();

}

void SelectButton::spriteResetGrey(){
	this->removeAllChildren();
	_isAviable=true;
	_clickTime=0;
}

SelectButton* SelectButton::create( const char *szImage ){

	SelectButton* mb=new SelectButton();
	if (mb && mb->init(szImage))
	{
		mb->autorelease();
		return mb;
	}
	else
	{
		delete mb;
		return NULL;
	}
}


//SkillLayer


SkillLayer::SkillLayer(void)
{
	_skillExplain=NULL;
	buyLayer=NULL;
	_buyType=SkillMenu;
	fengSprite=NULL;
	fengSprite2=NULL;
	_autoMove=true;

	_delegate=NULL;
	_delegate2=NULL;

	refreshBtn=NULL;
	isPosting=false;
}


SkillLayer::~SkillLayer(void)
{

}

bool SkillLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());



		//produce groud

		Sprite* gold_left=Sprite::createWithSpriteFrameName("gold_left.png");
		gold_left->setAnchorPoint(Vec2(0,0));
		gold_left->setPosition(Vec2(0,20));
		this->addChild(gold_left,1);


		Sprite* gold_right=Sprite::createWithSpriteFrameName("gold_right.png");
		gold_right->setAnchorPoint(Vec2(0,1));
		gold_right->setPosition(Vec2(winSize.width-gold_right->getContentSize().width-20,winSize.height-20));
		this->addChild(gold_right,1);


		//produce the cloud
		Sprite* cloud_left=Sprite::createWithSpriteFrameName("cloud.png");
		cloud_left->setPosition(Vec2(0,15));
		cloud_left->setFlipX(true);
		cloud_left->setFlipY(true);
		cloud_left->setAnchorPoint(Vec2(0,0));
		this->addChild(cloud_left,2);

		ActionInterval* cmv1=MoveBy::create(1,Vec2(-15,0));
		Action* cseq1=RepeatForever::create(Sequence::create(cmv1, cmv1->reverse(), nullptr));
		cloud_left->runAction(cseq1);

		Sprite* cloud_right=Sprite::createWithSpriteFrameName("cloud.png");
		cloud_right->setPosition(Vec2(winSize.width-cloud_right->getContentSize().width,15));
		cloud_right->setFlipY(true);
		cloud_right->setAnchorPoint(Vec2(0,0));
		this->addChild(cloud_right,2);

		ActionInterval* cmv2=MoveBy::create(1,Vec2(15,0));
		Action* cseq2=RepeatForever::create(Sequence::create(cmv2, cmv2->reverse(), nullptr));
		cloud_right->runAction(cseq2);

		Sprite* cloud_top=Sprite::createWithSpriteFrameName("cloud_top.png");
		cloud_top->setPosition(Vec2(winSize.width/2,winSize.height-(cloud_top->getContentSize().height+12)));
		cloud_top->setAnchorPoint(Vec2(0.5f,0));
		this->addChild(cloud_top,1);




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

		Sprite* record_title=Sprite::createWithSpriteFrameName("record_title.png");
		record_title->setAnchorPoint(Vec2(0,0));
		record_title->setPosition(Vec2(2,winSize.height-record_title->getContentSize().height-2));
		this->addChild(record_title,3);



		MenuItem* start_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("return_btn.png"),NULL,NULL,CC_CALLBACK_1(SkillLayer::onCancel, this));
		Menu* SkillMenu =Menu::create(start_bt,NULL);
		SkillMenu->setPosition(winSize.width-38,86);
		//SkillMenu->setPosition(winSize.width/2+skillSprite->getContentSize().width/2-10,winSize.height/2+skillSprite->getContentSize().height/2-12);
		this->addChild(SkillMenu,5);

		bRet=true;

	}while(0);

	return bRet;

}

void SkillLayer::initInterface(){

	char* bg_src;
	if(_delegate){
		bg_src="red_bg.png";
		selectHero=_delegate->_selectHero;
	}else if(_delegate2){
		bg_src="blue_bg.png";
		selectHero=_delegate2->_selectHero;
	}

	Sprite* bgSprite = Sprite::create(bg_src);
	bgSprite->setAnchorPoint(Vec2(0,0));
	scaleSpriteToScreen(bgSprite);
	bgSprite->setPosition(Vec2(0,0));
	this->addChild(bgSprite, -5);

	KTools* tool= KTools::create();

	if(!tool->checkData()){
		return;
	}



		char* skillbg_src="skill_bg1.png";


		skillSprite = Sprite::createWithSpriteFrameName(skillbg_src);
		skillSprite->setPosition(Vec2(winSize.width/2,winSize.height/2));
		this->addChild(skillSprite,2);





		MenuItem* change_bt=NULL;
		if(strcmp(selectHero,"Naruto")==0 ||
			strcmp(selectHero,"Jiraiya")==0  ||
			strcmp(selectHero,"Sasuke")==0
			){

				change_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("change_btn.png"),NULL,NULL,CC_CALLBACK_1(SkillLayer::onChangeBtn, this));

		}

		if(change_bt){
			changemenu =Menu::create(change_bt,NULL);
			changemenu->setPosition(Vec2(winSize.width/2,winSize.height/2-skillSprite->getContentSize().height/2+120));
			ActionInterval*  fd=FadeOut::create(1.0f);
			Action* seq=RepeatForever::create(Sequence::create(fd, fd->reverse(), nullptr));
			change_bt->runAction(seq);
			this->addChild(changemenu,15);
		}

		Sprite* goldBG=Sprite::createWithSpriteFrameName("gold_bg.png");
		goldBG->setAnchorPoint(Vec2(0.5,0));
		goldBG->setPosition(Vec2(winSize.width/2+skillSprite->getContentSize().width/4+25,winSize.height/2+skillSprite->getContentSize().width/2-74));
		this->addChild(goldBG,9);

		CCString* cl=tool->readFromSQLite();
		coinLabel=LabelBMFont::create(cl->getCString(),"Fonts/1.fnt");
		coinLabel->setScale(0.3f);
		coinLabel->setAnchorPoint(Vec2(0.5,0));
		coinLabel->setPosition(Vec2(goldBG->getPositionX()-goldBG->getContentSize().width/2+40,goldBG->getPositionY()+4));
		this->addChild(coinLabel,15);





		Sprite* bondBG=Sprite::createWithSpriteFrameName("golds_1.png");
		bondBG->setAnchorPoint(Vec2(0.5,0));
		bondBG->setPosition(Vec2(goldBG->getPositionX()-goldBG->getContentSize().width/2+15,goldBG->getPositionY()+4));
		this->addChild(bondBG,15);

		Sprite* bondBG2=Sprite::createWithSpriteFrameName("golds_2.png");
		bonds=ProgressTimer::create(bondBG2);
		bonds->setType(ProgressTimer::Type::BAR);
		bonds->setAnchorPoint(Vec2(0.5,0));
		bonds->setMidpoint(Vec2(0,0));
		bonds->setBarChangeRate(Vec2(0, 1));



		if(std::atoi(cl->getCString())>=99999){
		bonds->setPercentage(100);
		}else if(std::atoi(cl->getCString())>=80000){
		bonds->setPercentage(80);
		}else if(std::atoi(cl->getCString())>=60000){
		bonds->setPercentage(60);
		}else if(std::atoi(cl->getCString())>=40000){
		bonds->setPercentage(40);
		}else if(std::atoi(cl->getCString())>=20000){
		bonds->setPercentage(20);
		}

		bonds->setPosition(Vec2(goldBG->getPositionX()-goldBG->getContentSize().width/2+15,goldBG->getPositionY()+4));
		this->addChild(bonds,20);






		CCString* winNum=tool->readSQLite("CharRecord","name",selectHero,"column1");
		std::string bondString=winNum->getCString();
		CCString* totalNum=tool->readSQLite("CharRecord","name",selectHero,"column2");



		char* rank_src;
		bool isBlink=false;
		if(std::atoi(bondString.c_str())>=300){
			skillbg_src="skill_bg6.png";
			rank_src="rank_sss.png";
			isBlink=true;
		}else if(std::atoi(bondString.c_str())>=200){
			skillbg_src="skill_bg5.png";
			rank_src="rank_ss.png";
			isBlink=true;
		}else if(std::atoi(bondString.c_str())>=100){
			skillbg_src="skill_bg4.png";
			rank_src="rank_s.png";
			isBlink=true;
		}else if(std::atoi(bondString.c_str())>=50){
			skillbg_src="skill_bg3.png";
			rank_src="rank_a.png";
		}else if(std::atoi(bondString.c_str())>=25){
			skillbg_src="skill_bg2.png";
			rank_src="rank_b.png";
		}else{
			skillbg_src="skill_bg1.png";
			rank_src="rank_c.png";
		}

		skillSprite = Sprite::createWithSpriteFrameName(skillbg_src);
		skillSprite->setPosition(Vec2(winSize.width/2,winSize.height/2));
		this->addChild(skillSprite,2);

		CCString* imgPath=NULL;


		if((strcmp(selectHero,"Hinata")==0 ||
			strcmp(selectHero,"Sakura")==0 ||
			strcmp(selectHero,"Ino")==0 ||
			strcmp(selectHero,"Tsunade")==0 ||
			strcmp(selectHero,"Tenten")==0 ||
			strcmp(selectHero,"Konan")==0 ||
			strcmp(selectHero,"Karin")==0
			) && std::atoi(bondString.c_str())>=100){
				imgPath=CCString::createWithFormat("%s_full2.png",selectHero);
		}else{
			imgPath=CCString::createWithFormat("%s_full.png",selectHero);
		}
		_heroFull=Sprite::createWithSpriteFrameName(imgPath->getCString());
		_heroFull->setAnchorPoint(Vec2(0.5,0));
		_heroFull->setPosition(Vec2(winSize.width/2,
			winSize.height/2-skillSprite->getContentSize().height/2+23));
		this->addChild(_heroFull,5);



		if(isBlink){

			Sprite* blink=Sprite::createWithSpriteFrameName("skill_blink6.png");
			blink->setPosition(Vec2(winSize.width/2,winSize.height/2));

			this->addChild(blink,9);


			Vector<SpriteFrame*> tempArray;

			int i=1;

			while (i<7)
			{
				CCString* path=CCString::createWithFormat("skill_blink%d.png",i);
				SpriteFrame* frame = SpriteFrameCache::getInstance()->spriteFrameByName(path->getCString());
				tempArray.pushBack(frame);


				i+=1;
			}

			Animation* tempAnimation= Animation::createWithSpriteFrames(tempArray, 0.1f);
			FiniteTimeAction* tempAction=Animate::create(tempAnimation);
			DelayTime* delay=DelayTime::create(2.0f);

			blink->runAction(RepeatForever::create(Sequence::create(tempAction,delay,nullptr)));
		}


		Sprite* frameSprite=Sprite::createWithSpriteFrameName("skill_frame.png");
		frameSprite->setPosition(Vec2(winSize.width/2,winSize.height/2));
		this->addChild( frameSprite,10);

		Sprite* rankSprite=Sprite::createWithSpriteFrameName(rank_src);
		rankSprite->setAnchorPoint(Vec2(0,0));
		rankSprite->setPosition(Vec2(skillSprite->getPositionX()-skillSprite->getContentSize().width/2+5,skillSprite->getPositionY()+skillSprite->getContentSize().height/2-20));
		this->addChild(rankSprite,11);


		Sprite* detailBG1=Sprite::createWithSpriteFrameName("detail_bg.png");
		detailBG1->setAnchorPoint(Vec2(0,0));
		detailBG1->setPosition(Vec2(skillSprite->getPositionX()-skillSprite->getContentSize().width/2+8,skillSprite->getPositionY()-skillSprite->getContentSize().height/2+74));
		this->addChild(detailBG1,5);

		CCString* recordTime=tool->readSQLite("CharRecord","name",selectHero,"column3");
		LabelBMFont* bestLabel;
		if(strcmp(recordTime->getCString(),"")==0){
			bestLabel=LabelBMFont::create("00:00:00","Fonts/1.fnt");
		}else{
			bestLabel=LabelBMFont::create(recordTime->getCString(),"Fonts/1.fnt");
		}

		bestLabel->setScale(0.3f);
		bestLabel->setAnchorPoint(Vec2(0,0));
		bestLabel->setPosition(Vec2(detailBG1->getPositionX()+8,detailBG1->getPositionY()+7));
		this->addChild(bestLabel,6);



		Sprite* detailBG2=Sprite::createWithSpriteFrameName("detail_bg.png");
		detailBG2->setAnchorPoint(Vec2(0,0));
		detailBG2->setPosition(Vec2(skillSprite->getPositionX()+skillSprite->getContentSize().width/2-detailBG2->getContentSize().width-8,skillSprite->getPositionY()-skillSprite->getContentSize().height/2+74));
		this->addChild(detailBG2,5);



		LabelBMFont* recordLabel=LabelBMFont::create(CCString::createWithFormat("%s bonds",bondString.c_str())->getCString(),"Fonts/1.fnt");
		recordLabel->setScale(0.3f);
		recordLabel->setAnchorPoint(Vec2(0,0));

		recordLabel->setPosition(Vec2(detailBG2->getPositionX()+12,detailBG2->getPositionY()+7));
		this->addChild(recordLabel,6);




		//????????
		this->updateSkillGroup();

	this->scheduleUpdate();

}


void SkillLayer::onSkillRequestCompleted(Node *sender ,void *data){
	// Jalur HTTP untuk refresh skill dimatikan pada mode offline.
	(void)sender;
	(void)data;
	isPosting=false;
}


void SkillLayer::onChangeBtn(Ref* sender){


	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/chang_btn.ogg");
	skillGroup->removeFromParent();
	changemenu->removeFromParent();





	//??????
	if(strcmp(selectHero,"Naruto")==0){
		selectHero="SageNaruto";
	}else if(strcmp(selectHero,"SageNaruto")==0){
		selectHero="RikudoNaruto";
	}else if(strcmp(selectHero,"Jiraiya")==0){
		selectHero="SageJiraiya";
	}else if(strcmp(selectHero,"Sasuke")==0){
		selectHero="ImmortalSasuke";
	}

	SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName(CCString::createWithFormat("%s_full.png",selectHero)->getCString());
	_heroFull->setSpriteFrame(frame);


	//????????
	this->updateSkillGroup();

	if(strcmp(selectHero,"SageNaruto")==0 ){

		MenuItem* change_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("change_btn2.png"),NULL,NULL,CC_CALLBACK_1(SkillLayer::onChangeBtn, this));

		if(change_bt){

			changemenu =Menu::create(change_bt,NULL);
			changemenu->setPosition(Vec2(winSize.width/2,winSize.height/2-skillSprite->getContentSize().height/2+120));
			ActionInterval*  fd=FadeOut::create(1.0f);
			Action* seq=RepeatForever::create(Sequence::create(fd, fd->reverse(), nullptr));
			change_bt->runAction(seq);
			this->addChild(changemenu,15);
		}

	};


}

void SkillLayer::updateSkillGroup(){

	skillGroup=Sprite::create();

	for(int i=1;i<=5;i++){

		CCString* path=CCString::createWithFormat("%s_skill%d.png",selectHero,i);
		SelectButton* skill_btn=SelectButton::create(path->getCString());

		if(i==4){
			skill_btn->_isUnlock=true;
		}

		if(i==5){
			skill_btn->_isCanBuy2=false;
			skill_btn->_isUnlock=true;
		}


		skill_btn->setPosition(Vec2((i-1)*46,0));


		skill_btn->setBtnType((sbtnType)(i+2));
		if(this->_delegate){
			skill_btn->setDelegate1(this->_delegate);
		}else{
			skill_btn->setDelegate2(this->_delegate2);
		}


		skill_btn->setDelegate3(this);
		skillGroup->addChild(skill_btn);

	}


	skillGroup->setPosition(Vec2(skillSprite->getPositionX()-skillSprite->getContentSize().width/2+2,60));
	this->addChild(skillGroup,8);
}

void SkillLayer::setSkillExplain(sbtnType type){


	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/select.mp3");
	if(_skillExplain) _skillExplain->removeFromParent();
	CCString* imgPath=CCString::create("");
	if(type==Unlock1){
		imgPath=CCString::createWithFormat("fenglabel.png");

	}else if(type==Unlock2){
		imgPath=CCString::createWithFormat("fenglabel2.png");

	}else{
		imgPath=CCString::createWithFormat("%s_label%d.png",selectHero,(int)type-2);

	}

	ClippingNode* clipper = ClippingNode::create();
	Node *stencil = Sprite::createWithSpriteFrameName("clipperMask.png");
	stencil->setAnchorPoint(Vec2(0,0));
	clipper->setStencil(stencil);

	_skillExplain=Sprite::createWithSpriteFrameName(imgPath->getCString());
	_skillExplain->setAnchorPoint(Vec2(0,0));
	_skillExplain->setPositionX(10);

	clipper->setPosition(Vec2(winSize.width/2-skillSprite->getContentSize().width/2+2,
		winSize.height/2-skillSprite->getContentSize().height/2+4));

	clipper->addChild(_skillExplain);

	this->addChild(clipper,600);


};


void SkillLayer::update(float dt){

	if(!_skillExplain || !_autoMove){
		return;
	}

	float currentX = _skillExplain->getPositionX();
	float contentX = getContentSize().width;
	float lableX = _skillExplain->getContentSize().width;

	if(_skillExplain->getPositionX()>=-lableX){
		_skillExplain->setPositionX(_skillExplain->getPositionX()-0.6f);
	}else{
		_skillExplain->setPositionX(320);
	}

}



void SkillLayer::onCancel(Ref* sender){

	_skillExplain=NULL;
	buyLayer=NULL;
	fengSprite=NULL;
	fengSprite2=NULL;


	this->removeFromParent();
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");

	Director::getInstance()->popScene();
}
