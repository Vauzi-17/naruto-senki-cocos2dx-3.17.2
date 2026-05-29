#include "SelectLayer.h"
#include "Defines.h"
#include "StartMenu.h"
USING_NS_CC_EXT;
using namespace CocosDenshion;
#include<time.h>

static bool selectContainsHero(CCArray* used, const char* heroName)
{
	CCObject* pObject = NULL;
	CCARRAY_FOREACH(used, pObject)
	{
		CCString* value = (CCString*)pObject;
		if(strcmp(value->getCString(), heroName) == 0)
		{
			return true;
		}
	}
	return false;
}

static const char* selectPickHero(CCArray* used, const char** allHeroes, int heroCount, const char** presetList, int presetCount, int presetIndex, bool randomPick)
{
	if(!randomPick && presetIndex < presetCount && !selectContainsHero(used, presetList[presetIndex]))
	{
		used->addObject(CCString::create(presetList[presetIndex]));
		return presetList[presetIndex];
	}

	for(int attempt = 0; attempt < heroCount * 2; attempt++)
	{
		int index=random(heroCount);
		const char* heroName=allHeroes[index];
		if(!selectContainsHero(used, heroName))
		{
			used->addObject(CCString::create(heroName));
			return heroName;
		}
	}

	for(int i=0;i<heroCount;i++)
	{
		if(!selectContainsHero(used, allHeroes[i]))
		{
			used->addObject(CCString::create(allHeroes[i]));
			return allHeroes[i];
		}
	}

	return allHeroes[0];
}

static void selectAddHero(CCArray* heros, const char* character, const char* role, const char* group)
{
	CCDictionary* dic=CCDictionary::create();
	dic->setObject(CCString::create(character),"character");
	dic->setObject(CCString::create(role),"role");
	dic->setObject(CCString::create(group),"group");
	heros->addObject(dic);
}

SelectLayer::SelectLayer(void)
{
	_heroHalf=NULL;
	_heroName=NULL;
	_selectImg=NULL;
	
	isStart=false;
	_isHardCoreMode=false;
	_isTrainingMode=false;
	_trainingTeamSize=3;
	_trainingRandomTeam=false;
	_trainingRandomEnemy=true;

	SaveError=false;
	_playerSelect=NULL;
	selectArray=NULL;


}


SelectLayer::~SelectLayer(void)
{
	CC_SAFE_RELEASE(selectArray);
	
}

bool SelectLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!CCLayer::init());

			//¼ÓÔØÐòÁÐ¼¯	
		    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Select.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Record.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Record2.plist");		
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("UI.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Report.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Ougis.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Ougis2.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Map.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Gears.plist");
			
			KTools* tool= KTools::create();

			if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
				if(tool->checkMD5()==0){
					SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
					CCDirector::sharedDirector()->end();
					return false;
				}
			}
			
			
		char* bg_src="red_bg.png";
		
		CCSprite* bgSprite = CCSprite::create(bg_src);
		bgSprite->setAnchorPoint(ccp(0,0));
		
		scaleSpriteToScreen(bgSprite);
		bgSprite->setPosition(ccp(0,0));
		this->addChild(bgSprite, -5);

		//produce groud 


		CCSprite* gold_left=CCSprite::createWithSpriteFrameName("gold_left.png");
		gold_left->setAnchorPoint(ccp(0,0));
		gold_left->setPosition(ccp(0,20));
		this->addChild(gold_left,1);


		CCSprite* gold_right=CCSprite::createWithSpriteFrameName("gold_right.png");
		gold_right->setAnchorPoint(ccp(0,1));
		gold_right->setPosition(ccp(winSize.width-gold_right->getContentSize().width-20,winSize.height-20));
		this->addChild(gold_right,1);


		//produce the cloud
		CCSprite* cloud_left=CCSprite::createWithSpriteFrameName("cloud.png");	
		cloud_left->setPosition(ccp(0,15));
		cloud_left->setFlipX(true);
		cloud_left->setFlipY(true);
		cloud_left->setAnchorPoint(ccp(0,0));
		this->addChild(cloud_left,2);

		CCActionInterval* cmv1=CCMoveBy::create(1,ccp(-15,0));
		CCAction* cseq1=CCRepeatForever::create(cocos2d::cocos2dx2_compat::createSequence(cmv1,cmv1->reverse(),NULL));
		cloud_left->runAction(cseq1);

		CCSprite* cloud_right=CCSprite::createWithSpriteFrameName("cloud.png");	
		cloud_right->setPosition(ccp(winSize.width-cloud_right->getContentSize().width,15));
		cloud_right->setFlipY(true);
		cloud_right->setAnchorPoint(ccp(0,0));
		this->addChild(cloud_right,2);

		CCActionInterval* cmv2=CCMoveBy::create(1,ccp(15,0));
		CCAction* cseq2=CCRepeatForever::create(cocos2d::cocos2dx2_compat::createSequence(cmv2,cmv2->reverse(),NULL));
		cloud_right->runAction(cseq2);

		CCSprite* cloud_top=CCSprite::createWithSpriteFrameName("cloud_top.png");
		cloud_top->setPosition(ccp(winSize.width/2,winSize.height-(cloud_top->getContentSize().height+12)));
		cloud_top->setAnchorPoint(ccp(0.5f,0));
		this->addChild(cloud_top,1);

		
		//produce the menu_bar
		CCSprite* menu_bar_b = CCSprite::create("menu_bar2.png");
		menu_bar_b->setAnchorPoint(ccp(0,0));
		
		scaleSpriteToScreenWidth(menu_bar_b);
		this->addChild(menu_bar_b,2);

		


		CCSprite* menu_bar_t = CCSprite::create("menu_bar3.png");
		menu_bar_t->setAnchorPoint(ccp(0,0));
		
		scaleSpriteToScreenWidth(menu_bar_t);
		menu_bar_t->setPosition(ccp(0,winSize.height-menu_bar_t->getContentSize().height));
		this->addChild(menu_bar_t,2);

		
		CCSprite* select_title=CCSprite::createWithSpriteFrameName("select_title.png");
		if(select_title){
			select_title->setAnchorPoint(ccp(0,0));
			select_title->setPosition(ccp(2,winSize.height-select_title->getContentSize().height-2));
			this->addChild(select_title,3);
		}

		

		if(!SaveError){
			int first_player=NULL;
			selectArray=CCArray::create();
			
			char* selectList[]={"Naruto","Sakura","Sai",
				"Itachi","Konan","Deidara",
				"Sasuke","Karin","Jugo"
			};

			int num= sizeof(selectList)/sizeof(char*);
			for(int i=0;i<num;i++){
				CCString* path;
				const char* charName;
				
				path=CCString::createWithFormat("%s_select.png",selectList[i]);
				charName=selectList[i];
				
				SelectButton* select_btn=SelectButton::create(path->getCString());
				select_btn->setDelegate1(this);

				if(i<=2){
					select_btn->setPosition(ccp(winSize.width/2-64+i*27+(i%3)*10,winSize.height-112));
				}else if (i<=5){
					select_btn->setPosition(ccp(winSize.width/2-64+(i%3)*27+i%3*10,winSize.height-184));
				}else if (i<=8){
					select_btn->setPosition(ccp(winSize.width/2-64+(i%3)*27+i%3*10,winSize.height-256));
				
				} 
				select_btn->setCharName(CCString::createWithFormat("%s",charName));
				this->addChild(select_btn,2);
				selectArray->addObject(select_btn);

			};
			selectArray->retain();
		
			
			first_player=0;
			

			const char* path=CCString::createWithFormat("%s_half.png",selectList[first_player])->getCString();
			_heroHalf=CCSprite::createWithSpriteFrameName(path);
			_heroHalf->setAnchorPoint(ccp(0,0));
			_heroHalf->setPosition(ccp(10,10));
			this->addChild(_heroHalf,1);
			num2=num;
			const char* namePath=CCString::createWithFormat("%s_font.png",selectList[first_player])->getCString();
			_heroName=CCSprite::createWithSpriteFrameName(namePath);
			_heroName->setAnchorPoint(ccp(0.5f,0));
			_heroName->setPosition(ccp(100,20));
			this->addChild(_heroName,5);


			_selectImg=CCSprite::createWithSpriteFrameName("1p.png");
			_selectImg->setAnchorPoint(ccp(0,0));
			SelectButton* pObject=(SelectButton*) selectArray->objectAtIndex(first_player);
			_selectImg->setPosition(ccp(pObject->getPositionX()-2,pObject->getPositionY()-2));
			this->addChild(_selectImg,500);

			_selectHero=pObject->getCharName()->getCString();

			CCActionInterval* blink=CCBlink::create(0.6f,1);
			_selectImg->runAction(CCRepeatForever::create(blink));


			CCMenuItem* start_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("start_btn.png"),NULL,NULL,this,menu_selector(SelectLayer::onGameStart));
			CCMenu* menu =CCMenu::create(start_bt,NULL);
			menu->setAnchorPoint(ccp(0,0));
			menu->setPosition(winSize.width-40,36);
			this->addChild(menu,500);



			CCMenuItem* skill_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("skill_btn.png"),NULL,NULL,this,menu_selector(SelectLayer::onSkillMenu));
			CCMenu* menu2 =CCMenu::create(skill_bt,NULL);
			menu2->setAnchorPoint(ccp(0,0));
			menu2->setPosition(winSize.width-35,96);
			this->addChild(menu2,5);

			

			if(CCUserDefault::sharedUserDefault()->getBoolForKey("isBGM")!=false){
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/Music/select_music.mp3",true);
			}

			if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
				this->setKeypadEnabled(true);
			}
			
			
		}


		bRet=true;
	}while(0);

	return bRet;

}

void SelectLayer::setSelected(CCObject* sender){
	SelectButton* btn=(SelectButton*) sender;

	 _selectImg->setPosition(ccp(btn->getPositionX()-2,btn->getPositionY()-2));
	
	 _heroHalf->removeFromParent();
	 const char* path=CCString::createWithFormat("%s_half.png",btn->getCharName()->getCString())->getCString();
	 _heroHalf=CCSprite::createWithSpriteFrameName(path);

	 _heroHalf->setAnchorPoint(ccp(0,0));
	 _heroHalf->setPosition(ccp(10,10));
	 this->addChild(_heroHalf,1);

	 _heroName->removeFromParent();
	 const char* namePath=CCString::createWithFormat("%s_font.png",btn->getCharName()->getCString())->getCString();
	 _heroName=CCSprite::createWithSpriteFrameName(namePath);
	 _heroName->setAnchorPoint(ccp(0.5f,0));
	 _heroName->setPosition(ccp(100,20));
	 this->addChild(_heroName,5);

	_selectHero=btn->getCharName()->getCString();

}


void SelectLayer::onError(CCObject* sender){

	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CCDirector::sharedDirector()->end();
	return;

}


void SelectLayer::onSkillMenu(CCObject* sender){

	CCScene* pscene =CCScene::create();
	SkillLayer* skillLayer=SkillLayer::create();
	skillLayer->setDelegate(this);
	skillLayer->initInterface();
	pscene->addChild(skillLayer);

	CCDirector::sharedDirector()->pushScene(pscene);

}



void SelectLayer::onGameStart(CCObject* sender){

	

	if (!isStart){

		KTools* tool= KTools::create();

		

		isStart=true;
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");


		CCDictionary* dic=CCDictionary::create();

		CCString* tmpChar;


		char* heroList[]={"Konan","Sakura","Naruto","Sai","Deidara",
			"Kakashi","Itachi","Tenten","Jiraiya","Suigetsu",
			"Tsunade","Tobirama","Neji","Ino","Asuma","Gaara",
			"Karin","Sasuke","Hidan","Choji","Kankuro",
			"Shino","Minato","Tobi","Kakuzu","Hinata",
			"Hiruzen","Kiba","Jugo","Lee"
		};


		int num= sizeof(heroList)/sizeof(char*);

		
		tmpChar=CCString::create(_selectHero);
		
		srand((int)time(0));
		int zhenying=1;
		



		CCString* tmpRole=CCString::create("Player");
		CCString* tmpGroup=CCString::create("Konoha");

		dic->setObject(tmpChar,"character");
		dic->setObject(tmpRole,"role");
		dic->setObject(tmpGroup,"group");

		CCArray* tempHeros=CCArray::createWithObject(dic);

		

		const char* teamPreset[]={"Sakura","Kakashi","Lee","Tsunade"};
		const char* enemyPreset[]={"Deidara","Itachi","Kisame","Hidan","Kakuzu"};
		int teamSize=_trainingTeamSize;
		if(teamSize<1){
			teamSize=1;
		}else if(teamSize>5){
			teamSize=5;
		}

		CCArray* used=CCArray::create();
		used->addObject(CCString::create(_selectHero));

		for(int i=1;i<teamSize;i++){
			const char* heroName=selectPickHero(used,(const char**)heroList,num,teamPreset,sizeof(teamPreset)/sizeof(char*),i-1,_trainingRandomTeam);
			selectAddHero(tempHeros,heroName,"Com","Konoha");
		}

		for(int i=0;i<teamSize;i++){
			const char* heroName=selectPickHero(used,(const char**)heroList,num,enemyPreset,sizeof(enemyPreset)/sizeof(char*),i,_trainingRandomEnemy);
			selectAddHero(tempHeros,heroName,"Com","Akatsuki");
		}



		CCScene* loadScene=CCScene::create();
		LoadLayer* loadLayer=LoadLayer::create();
		loadLayer->tempHeros=tempHeros;
		loadScene->addChild(loadLayer);
		
		loadLayer->_isHardCoreMode=_isHardCoreMode || _trainingTeamSize>3;
		loadLayer->_isTrainingMode=_isTrainingMode;
		loadLayer->preloadAudio();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f,loadScene));
	

	}
}


void SelectLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		onBackKeyClicked();
	}
}

void SelectLayer::onBackKeyClicked(){
	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/cancel.ogg");	
	CCScene* menuScene =CCScene::create();
	CCLayer* menuLayer=StartMenu::create();
	menuScene->addChild(menuLayer);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f,menuScene));

}


SelectLayer* SelectLayer::create(){

	SelectLayer* sl=new SelectLayer();
	if (sl && sl->init())
	{
		sl->autorelease();
		return sl;
	}
	else
	{
		delete sl;
		return NULL;
	}
}


