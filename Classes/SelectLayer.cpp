#include "SelectLayer.h"
#include "Defines.h"
#include "StartMenu.h"
#include <string>
#include <vector>
USING_NS_CC_EXT;
using namespace CocosDenshion;
#include<time.h>

static bool selectContainsHero(const std::vector<std::string>& used, const char* heroName)
{
	if(!heroName){
		return false;
	}
	for(const std::string& value : used)
	{
		if(value == heroName)
		{
			return true;
		}
	}
	return false;
}

static const char* selectPickHero(std::vector<std::string>& used, const char** allHeroes, int heroCount, const char** presetList, int presetCount, int presetIndex, bool randomPick)
{
	if(!randomPick && presetIndex < presetCount && !selectContainsHero(used, presetList[presetIndex]))
	{
		used.push_back(presetList[presetIndex]);
		return presetList[presetIndex];
	}

	for(int attempt = 0; attempt < heroCount * 2; attempt++)
	{
		int index=random(heroCount);
		const char* heroName=allHeroes[index];
		if(!selectContainsHero(used, heroName))
		{
			used.push_back(heroName);
			return heroName;
		}
	}

	for(int i=0;i<heroCount;i++)
	{
		if(!selectContainsHero(used, allHeroes[i]))
		{
			used.push_back(allHeroes[i]);
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


}


SelectLayer::~SelectLayer(void)
{

}

bool SelectLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());

			//¼ÓÔØÐòÁÐ¼¯
		    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Select.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Record.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Record2.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Report.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis2.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Gears.plist");

			KTools* tool= KTools::create();

			if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
				if(tool->checkMD5()==0){
					SimpleAudioEngine::getInstance()->stopBackgroundMusic();
					Director::getInstance()->end();
					return false;
				}
			}


		char* bg_src="red_bg.png";

		Sprite* bgSprite = Sprite::create(bg_src);
		bgSprite->setAnchorPoint(Vec2(0,0));

		scaleSpriteToScreen(bgSprite);
		bgSprite->setPosition(Vec2(0,0));
		this->addChild(bgSprite, -5);

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


		Sprite* select_title=Sprite::createWithSpriteFrameName("select_title.png");
		if(select_title){
			select_title->setAnchorPoint(Vec2(0,0));
			select_title->setPosition(Vec2(2,winSize.height-select_title->getContentSize().height-2));
			this->addChild(select_title,3);
		}



		if(!SaveError){
			int first_player=NULL;
			selectArray.clear();

			char* selectList[]={"Naruto","Sakura","Sai",
				"Itachi","Konan","Deidara",
				"Sasuke","Karin","Jugo"
			};

			int num= sizeof(selectList)/sizeof(char*);
			for(int i=0;i<num;i++){
				std::string path;
				const char* charName;

				path=cocos2d::StringUtils::format("%s_select.png",selectList[i]);
				charName=selectList[i];

				SelectButton* select_btn=SelectButton::create(path.c_str());
				select_btn->setDelegate1(this);

				if(i<=2){
					select_btn->setPosition(Vec2(winSize.width/2-64+i*27+(i%3)*10,winSize.height-112));
				}else if (i<=5){
					select_btn->setPosition(Vec2(winSize.width/2-64+(i%3)*27+i%3*10,winSize.height-184));
				}else if (i<=8){
					select_btn->setPosition(Vec2(winSize.width/2-64+(i%3)*27+i%3*10,winSize.height-256));

				}
				select_btn->setCharName(CCString::create(charName));
				this->addChild(select_btn,2);
				selectArray.push_back(select_btn);

			};


			first_player=0;


			std::string path=cocos2d::StringUtils::format("%s_half.png",selectList[first_player]);
			_heroHalf=Sprite::createWithSpriteFrameName(path);
			_heroHalf->setAnchorPoint(Vec2(0,0));
			_heroHalf->setPosition(Vec2(10,10));
			this->addChild(_heroHalf,1);
			num2=num;
			std::string namePath=cocos2d::StringUtils::format("%s_font.png",selectList[first_player]);
			_heroName=Sprite::createWithSpriteFrameName(namePath);
			_heroName->setAnchorPoint(Vec2(0.5f,0));
			_heroName->setPosition(Vec2(100,20));
			this->addChild(_heroName,5);


			_selectImg=Sprite::createWithSpriteFrameName("1p.png");
			_selectImg->setAnchorPoint(Vec2(0,0));
			SelectButton* pObject=selectArray[first_player];
			_selectImg->setPosition(Vec2(pObject->getPositionX()-2,pObject->getPositionY()-2));
			this->addChild(_selectImg,500);

			_selectHero=pObject->getCharName()->getCString();

			ActionInterval* blink=Blink::create(0.6f,1);
			_selectImg->runAction(RepeatForever::create(blink));


			MenuItem* start_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("start_btn.png"),NULL,NULL,CC_CALLBACK_1(SelectLayer::onGameStart, this));
			Menu* menu =Menu::create(start_bt,NULL);
			menu->setAnchorPoint(Vec2(0,0));
			menu->setPosition(winSize.width-40,36);
			this->addChild(menu,500);



			MenuItem* skill_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("skill_btn.png"),NULL,NULL,CC_CALLBACK_1(SelectLayer::onSkillMenu, this));
			Menu* menu2 =Menu::create(skill_bt,NULL);
			menu2->setAnchorPoint(Vec2(0,0));
			menu2->setPosition(winSize.width-35,96);
			this->addChild(menu2,5);



			if(UserDefault::getInstance()->getBoolForKey("isBGM")!=false){
				SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/select_music.mp3",true);
			}


		}


		bRet=true;
	}while(0);

	return bRet;

}

void SelectLayer::onEnter(){
	Layer::onEnter();
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(SelectLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void SelectLayer::onExit(){
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();
}

void SelectLayer::setSelected(Ref* sender){
	SelectButton* btn=(SelectButton*) sender;

	 _selectImg->setPosition(Vec2(btn->getPositionX()-2,btn->getPositionY()-2));

	 _heroHalf->removeFromParent();
	 std::string path=cocos2d::StringUtils::format("%s_half.png",btn->getCharName()->getCString());
	 _heroHalf=Sprite::createWithSpriteFrameName(path);

	 _heroHalf->setAnchorPoint(Vec2(0,0));
	 _heroHalf->setPosition(Vec2(10,10));
	 this->addChild(_heroHalf,1);

	 _heroName->removeFromParent();
	 std::string namePath=cocos2d::StringUtils::format("%s_font.png",btn->getCharName()->getCString());
	 _heroName=Sprite::createWithSpriteFrameName(namePath);
	 _heroName->setAnchorPoint(Vec2(0.5f,0));
	 _heroName->setPosition(Vec2(100,20));
	 this->addChild(_heroName,5);

	_selectHero=btn->getCharName()->getCString();

}


void SelectLayer::onError(Ref* sender){

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->end();
	return;

}


void SelectLayer::onSkillMenu(Ref* sender){

	Scene* pscene =Scene::create();
	SkillLayer* skillLayer=SkillLayer::create();
	skillLayer->setDelegate(this);
	skillLayer->initInterface();
	pscene->addChild(skillLayer);

	Director::getInstance()->pushScene(pscene);

}



void SelectLayer::onGameStart(Ref* sender){



	if (!isStart){

		KTools* tool= KTools::create();



		isStart=true;
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");


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

		std::vector<std::string> used;
		used.push_back(_selectHero);

		for(int i=1;i<teamSize;i++){
			const char* heroName=selectPickHero(used,(const char**)heroList,num,teamPreset,sizeof(teamPreset)/sizeof(char*),i-1,_trainingRandomTeam);
			selectAddHero(tempHeros,heroName,"Com","Konoha");
		}

		for(int i=0;i<teamSize;i++){
			const char* heroName=selectPickHero(used,(const char**)heroList,num,enemyPreset,sizeof(enemyPreset)/sizeof(char*),i,_trainingRandomEnemy);
			selectAddHero(tempHeros,heroName,"Com","Akatsuki");
		}



		Scene* loadScene=Scene::create();
		LoadLayer* loadLayer=LoadLayer::create();
		loadLayer->tempHeros=tempHeros;
		loadScene->addChild(loadLayer);

		loadLayer->_isHardCoreMode=_isHardCoreMode || _trainingTeamSize>3;
		loadLayer->_isTrainingMode=_isTrainingMode;
		loadLayer->preloadAudio();
		Director::getInstance()->replaceScene(TransitionFade::create(1.5f,loadScene));


	}
}


void SelectLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		onBackKeyClicked();
	}
}

void SelectLayer::onBackKeyClicked(){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
	Scene* menuScene =Scene::create();
	Layer* menuLayer=StartMenu::create();
	menuScene->addChild(menuLayer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,menuScene));

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
