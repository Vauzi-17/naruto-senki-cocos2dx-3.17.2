#include "NetworkLayer.h"
#include "Defines.h"
#include "StartMenu.h"
#include <algorithm>
#include <string>
USING_NS_CC_EXT;
using namespace CocosDenshion;

#include<time.h>

static bool networkContainsHero(const std::vector<std::string>& used, const char* heroName)
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

static const char* networkPickHero(std::vector<std::string>& used, const char** allHeroes, int heroCount, const char** presetList, int presetCount, int presetIndex, bool randomPick)
{
	if(!randomPick && presetIndex < presetCount && !networkContainsHero(used, presetList[presetIndex]))
	{
		used.push_back(presetList[presetIndex]);
		return presetList[presetIndex];
	}

	for(int attempt=0; attempt<heroCount*2; attempt++)
	{
		int index=random(heroCount);
		const char* heroName=allHeroes[index];
		if(!networkContainsHero(used, heroName))
		{
			used.push_back(heroName);
			return heroName;
		}
	}

	for(int i=0;i<heroCount;i++)
	{
		if(!networkContainsHero(used, allHeroes[i]))
		{
			used.push_back(allHeroes[i]);
			return allHeroes[i];
		}
	}

	return allHeroes[0];
}


static bool networkArrayHasHero(const std::vector<std::string>& list, const char* heroName)
{
	if(!heroName){
		return false;
	}
	for(const std::string& value : list){
		if(value == heroName){
			return true;
		}
	}
	return false;
}

static void networkAddHero(CCArray* heros, const char* character, const char* role, const char* group)
{
	CCDictionary* dic=CCDictionary::create();
	dic->setObject(CCString::create(character),"character");
	dic->setObject(CCString::create(role),"role");
	dic->setObject(CCString::create(group),"group");
	heros->addObject(dic);
}
/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#endif  */


NetworkLayer::NetworkLayer(void)
{
	_heroHalf=NULL;
	_heroName=NULL;
	_selectImg=NULL;


	_selectList=NULL;

	_comSelector1=NULL;
	_comLabel1=NULL;

	_comSelector2=NULL;
	_comLabel2=NULL;

	isStart=false;
	_isHardCoreMode=true;
	_isRandomChar=false;
	_isTrainingSelect=false;
	_isTrainingMode=false;
	_trainingTeamSize=3;
	_trainingRandomTeam=false;
	_trainingRandomEnemy=true;
	_trainingKillLimit=3;
	_selectedMap=0;
	_mapPreview=NULL;
	_mapLabel=NULL;

	SaveError=false;
	_playerSelect=NULL;
	_com1Select=NULL;
	_com2Select=NULL;
	selectArray=NULL;

	_pageLayer1=NULL;
	_pageLayer2=NULL;
	_pageLayer3=NULL;



	isPosting=false;
	refreshBtn=NULL;

	_teamSelector=NULL;
	for(int i=0;i<5;i++){
		_trainingTeamSlots[i]=NULL;
		_trainingEnemySlots[i]=NULL;
	}
}


NetworkLayer::~NetworkLayer(void)
{
	CC_SAFE_RELEASE(selectArray);

}

bool NetworkLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());

			//�������м�
		    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
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



//			if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
//				if(tool->checkMD5()==0){
//					SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//					Director::getInstance()->end();
//					return false;
//				}
//			}


		char* bg_src="blue_bg.png";

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

		Sprite* select_title=Sprite::createWithSpriteFrameName("hcmode_title.png");

		select_title->setAnchorPoint(Vec2(0,0));
		select_title->setPosition(Vec2(2,winSize.height-select_title->getContentSize().height-2));
		this->addChild(select_title,3);



		if(!SaveError){

			_selectList=CCArray::create();
			selectArray=CCArray::create();
			_pageLayer1=Layer::create();
			_pageLayer2=Layer::create();
			_pageLayer3=Layer::create();

			char* selectList[]={"Naruto","Sakura","Sai","Kakashi",
			"None","None","None",
			"Shikamaru","Ino","Choji","Asuma",
			"Kiba","Hinata","Shino",
			"Neji","Tenten","Lee","None",
			"None","None","None",

			"Shisui","Tobirama","Hiruzen","Minato",
			"Jiraiya","Tsunade","Orochimaru",
			"None","Deidara","Kakuzu","Hidan",
			"Tobi","Konan","Pain",
			"Itachi","Kisame","None","None",
			"None","None","None",

			"Sasuke","Karin","Suigetsu","Jugo",
			"None","None","None",
			"Gaara","None","Kankuro","Chiyo",
			"None","None","None",
			"None","None","None","None",
			"None","None","None"
			};




			int num= sizeof(selectList)/sizeof(char*);

			int Column=0;
			int Row=0;
			int Page=0;

			for (int i=0; i < num; i++)  {

				const char* charName;
				charName=selectList[i];

				_selectList->addObject(CCString::create(charName));

				std::string path=cocos2d::StringUtils::format("%s_select.png",charName);
				Column=i%7;
				Row=i/7;
				Page=int(i/21);
				SelectButton* select_btn=SelectButton::create(path.c_str());
				select_btn->setDelegate2(this);
				select_btn->setCharName(CCString::create(charName));
				if(Page==0){
					select_btn->setPosition(Vec2(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*Row)));
					_pageLayer1->addChild(select_btn,-Column);
				}else if(Page==1){
					select_btn->setPosition(Vec2(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*(Row-3))));
					_pageLayer2->addChild(select_btn,-Column);

				}else if(Page==2){
					select_btn->setPosition(Vec2(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*(Row-6))));
					_pageLayer3->addChild(select_btn,-Column);

				}


				selectArray->addObject(select_btn);
			};


			this->addChild(_pageLayer1,5);
			this->addChild(_pageLayer2,5);

			this->addChild(_pageLayer3,5);

			_pageLayer2->setVisible(false);
			_pageLayer3->setVisible(false);
			_pageLayer2->setPositionY(2000);
			_pageLayer3->setPositionY(2000);

			page1_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("page1_off.png"),Sprite::createWithSpriteFrameName("page1_on.png"),CC_CALLBACK_1(NetworkLayer::onPage1Btn, this));
			page2_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("page2_off.png"),Sprite::createWithSpriteFrameName("page2_on.png"),CC_CALLBACK_1(NetworkLayer::onPage2Btn, this));
			page3_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("page3_off.png"),Sprite::createWithSpriteFrameName("page3_on.png"),CC_CALLBACK_1(NetworkLayer::onPage3Btn, this));

			page_menu=Menu::create(page1_btn,page2_btn,page3_btn,NULL);

			page_menu->alignItemsHorizontallyWithPadding(20);
			page_menu->setPosition(Vec2(winSize.width/2+48,42));
			this->addChild(page_menu,10);
			page1_btn->selected();



			selectArray->retain();
			_selectList->retain();

			CCString* hero=(CCString *)_selectList->objectAtIndex(0);

			std::string path=cocos2d::StringUtils::format("%s_half.png",hero->getCString());
			_heroHalf=Sprite::createWithSpriteFrameName(path);
			_heroHalf->setAnchorPoint(Vec2(0,0));
			_heroHalf->setPosition(Vec2(10,10));
			this->addChild(_heroHalf,1);

			std::string namePath=cocos2d::StringUtils::format("%s_font.png",hero->getCString());
			_heroName=Sprite::createWithSpriteFrameName(namePath);
			_heroName->setAnchorPoint(Vec2(0.5f,0));
			_heroName->setPosition(Vec2(100,20));
			this->addChild(_heroName,5);


			_selectImg=Sprite::createWithSpriteFrameName("1p.png");
			_selectImg->setAnchorPoint(Vec2(0,0));
			SelectButton* pObject=(SelectButton*) selectArray->objectAtIndex(0);
			_selectImg->setPosition(Vec2(pObject->getPositionX()-2,pObject->getPositionY()-2));
			this->addChild(_selectImg,500);

			_selectHero=pObject->getCharName()->getCString();

			ActionInterval* blink=Blink::create(0.6f,1);
			_selectImg->runAction(RepeatForever::create(blink));



			if(adResult==1 || _isTrainingSelect){
				_teamSelector=Layer::create();

				Sprite* _teamBg=Sprite::createWithSpriteFrameName("team_bg.png");
				_teamBg->setAnchorPoint(Vec2(0,0));
				_teamBg->setPosition(Vec2(0,185));
				_teamSelector->addChild(_teamBg);



				_comSelector1=Sprite::createWithSpriteFrameName("unknow_select.png");
				_comSelector1->setAnchorPoint(Vec2(0,0));
				_comSelector1->setPosition(Vec2(2,194));
				_teamSelector->addChild(_comSelector1);

				_comLabel1=Sprite::createWithSpriteFrameName("com_label.png");
				_comLabel1->setAnchorPoint(Vec2(0.5,0.5f));
				_comLabel1->setPosition(Vec2(_comSelector1->getPositionX()+_comSelector1->getContentSize().width+2+18,_comSelector1->getPositionY()+_comSelector1->getContentSize().height/2));
				_teamSelector->addChild(_comLabel1);


				_comSelector2=Sprite::createWithSpriteFrameName("unknow_select.png");
				_comSelector2->setAnchorPoint(Vec2(0,0));
				_comSelector2->setPosition(Vec2(_comSelector1->getPositionX()+_comSelector1->getContentSize().width+40,_comSelector1->getPositionY()));
				_teamSelector->addChild(_comSelector2);


				_comLabel2=Sprite::createWithSpriteFrameName("com_label.png");
				_comLabel2->setAnchorPoint(Vec2(0.5,0.5f));
				_comLabel2->setPosition(Vec2(_comSelector2->getPositionX()+_comSelector2->getContentSize().width+2+18,_comSelector2->getPositionY()+_comSelector2->getContentSize().height/2));
				_teamSelector->addChild(_comLabel2);
				if(_isTrainingSelect){
					float slotScale=0.82f;
					float slotW=_comSelector1->getContentSize().width*slotScale;
					float slotGap=slotW+4;
					float slotY=_comSelector1->getPositionY();
					float teamStartX=4;
					float enemyStartX=winSize.width-4-slotGap*_trainingTeamSize;

					_comLabel1->setVisible(false);
					_comLabel2->setVisible(false);
					_comSelector1->setScale(slotScale);
					_comSelector2->setScale(slotScale);
					_comSelector1->setPosition(Vec2(teamStartX,slotY));
					_comSelector2->setPosition(Vec2(teamStartX+slotGap,slotY));
					_comSelector2->setVisible(_trainingTeamSize>1);

					_trainingTeamSlots[0]=_comSelector1;
					_trainingTeamSlots[1]=_comSelector2;
					for(int i=2;i<_trainingTeamSize && i<5;i++){
						_trainingTeamSlots[i]=Sprite::createWithSpriteFrameName("unknow_select.png");
						_trainingTeamSlots[i]->setAnchorPoint(Vec2(0,0));
						_trainingTeamSlots[i]->setScale(slotScale);
						_trainingTeamSlots[i]->setPosition(Vec2(teamStartX+slotGap*i,slotY));
						_teamSelector->addChild(_trainingTeamSlots[i]);
					}
					for(int i=0;i<_trainingTeamSize && i<5;i++){
						_trainingEnemySlots[i]=Sprite::createWithSpriteFrameName("unknow_select.png");
						_trainingEnemySlots[i]->setAnchorPoint(Vec2(0,0));
						_trainingEnemySlots[i]->setScale(slotScale);
						_trainingEnemySlots[i]->setPosition(Vec2(enemyStartX+slotGap*i,slotY));
						_teamSelector->addChild(_trainingEnemySlots[i]);
					}
				}

				this->addChild(_teamSelector,_isTrainingSelect ? 4 : 50);
			}


			//���а�
			this->updateMapPreview();

			MenuItem* ranking_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("ranking_btn.png"),NULL,NULL,CC_CALLBACK_1(NetworkLayer::onRankingBtn, this));
			Menu* menu3 =Menu::create(ranking_btn,NULL);
			ranking_btn->setAnchorPoint(Vec2(1,0.5f));
			menu3->setPosition(winSize.width-15,winSize.height-34);
			this->addChild(menu3,5);





			MenuItem* start_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("start_btn.png"),NULL,NULL,CC_CALLBACK_1(NetworkLayer::onGameStart, this));
			Menu* menu =Menu::create(start_bt,NULL);
			menu->setAnchorPoint(Vec2(0,0));
			menu->setPosition(winSize.width-40,36);
			this->addChild(menu,80);



			MenuItem* skill_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("skill_btn.png"),NULL,NULL,CC_CALLBACK_1(NetworkLayer::onSkillMenu, this));
			Menu* menu2 =Menu::create(skill_bt,NULL);
			menu2->setAnchorPoint(Vec2(0,0));
			menu2->setPosition(winSize.width-35,96);
			this->addChild(menu2,80);

			MenuItem* unselect_all_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("change_btn.png"),NULL,NULL,CC_CALLBACK_1(NetworkLayer::onUnselectAllBtn, this));
			Menu* unselectMenu=Menu::create(unselect_all_bt,NULL);
			unselectMenu->setAnchorPoint(Vec2(0,0));
			unselectMenu->setPosition(38,156);
			this->addChild(unselectMenu,80);









			if(UserDefault::getInstance()->getBoolForKey("isBGM")!=false){
				SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/select_music.mp3",true);
			}


		}




		bRet=true;
	}while(0);

	return bRet;

}

void NetworkLayer::onEnter(){
	Layer::onEnter();
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(NetworkLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void NetworkLayer::onExit(){
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();
}

void NetworkLayer::onListRequestCompleted(Node *sender ,void *data){

	// Jalur request list hero lama tidak dipakai pada mode offline.
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}

void NetworkLayer::onPage1Btn(cocos2d::Ref* sender)
{
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);

	page2_btn->unselected();
	page3_btn->unselected();
	page1_btn->selected();

	_pageLayer1->setVisible(true);
	_pageLayer2->setVisible(false);
	_pageLayer3->setVisible(false);

	_pageLayer1->setPositionY(0);
	_pageLayer2->setPositionY(2000);
	_pageLayer3->setPositionY(2000);
}

void NetworkLayer::onPage2Btn(cocos2d::Ref* sender)
{
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);

	page1_btn->unselected();
	page3_btn->unselected();
	page2_btn->selected();

	_pageLayer2->setVisible(true);
	_pageLayer1->setVisible(false);
	_pageLayer3->setVisible(false);

	_pageLayer2->setPositionY(0);
	_pageLayer1->setPositionY(2000);
	_pageLayer3->setPositionY(2000);
}

void NetworkLayer::onPage3Btn(cocos2d::Ref* sender)
{
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);

	page1_btn->unselected();
	page2_btn->unselected();
	page3_btn->selected();

	_pageLayer3->setVisible(true);
	_pageLayer1->setVisible(false);
	_pageLayer2->setVisible(false);

	_pageLayer3->setPositionY(0);
	_pageLayer1->setPositionY(2000);
	_pageLayer2->setPositionY(2000);
}

void NetworkLayer::onMapBtn(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
	_selectedMap++;
	if(_selectedMap>4){
		_selectedMap=0;
	}
	this->updateMapPreview();
}

void NetworkLayer::updateMapPreview(){
	const int previewTag=9101;
	const int labelTag=9102;
	const int buttonTag=9103;
    Vec2 mapPos=Vec2(65, winSize.height-70);
//    Vec2 mapPos=Vec2(70,250);

	Node* oldPreview=this->getChildByTag(previewTag);
	if(oldPreview){
		oldPreview->removeFromParent();
	}
	Node* oldLabel=this->getChildByTag(labelTag);
	if(oldLabel){
		oldLabel->removeFromParent();
	}

	std::string mapPath=cocos2d::StringUtils::format("Tiles/map_bg%d.png",_selectedMap+1);
	_mapPreview=Sprite::create(mapPath);
	if(_mapPreview){
		_mapPreview->setScale(0.205f);
		_mapPreview->setPosition(Vec2(mapPos.x,mapPos.y+6));
		this->addChild(_mapPreview,81,previewTag);
	}

	std::string mapLabel=cocos2d::StringUtils::format("MAP %d",_selectedMap+1);
	_mapLabel=LabelBMFont::create(mapLabel,"Fonts/1.fnt");
	if(_mapLabel){
		_mapLabel->setScale(0.25f);
		_mapLabel->setAnchorPoint(Vec2(0.5f,0.5f));
		_mapLabel->setPosition(Vec2(mapPos.x,mapPos.y-18));
		this->addChild(_mapLabel,83,labelTag);
	}

	if(!this->getChildByTag(buttonTag)){
		Sprite* mapNormal=Sprite::createWithSpriteFrameName("minimap_bg.png");
		Sprite* mapSelected=Sprite::createWithSpriteFrameName("minimap_bg.png");
		mapNormal->setOpacity(120);
		mapSelected->setOpacity(180);
		MenuItemSprite* mapBtn=MenuItemSprite::create(
			mapNormal,
			mapSelected,
			CC_CALLBACK_1(NetworkLayer::onMapBtn, this));
		Menu* mapMenu=Menu::create(mapBtn,NULL);
		mapMenu->setPosition(mapPos);
		this->addChild(mapMenu,80,buttonTag);
	}
}


SelectButton* NetworkLayer::findSelectButton(const char* heroName){
	if(!heroName || !selectArray){
		return NULL;
	}
	for(unsigned int selectIndex=0; selectIndex<selectArray->count(); ++selectIndex){
		Ref* pObject=selectArray->objectAtIndex(selectIndex);
		SelectButton* btn=(SelectButton*)pObject;
		if(strcmp(btn->getCharName()->getCString(),heroName)==0){
			return btn;
		}
	}
	return NULL;
}

bool NetworkLayer::isHeroConfirmedSelected(const char* heroName){
	if(!heroName || strcmp(heroName,"None")==0){
		return false;
	}
	if(_playerSelect && strcmp(_playerSelect,heroName)==0){
		return true;
	}
	if(_com1Select && strcmp(_com1Select,heroName)==0){
		return true;
	}
	if(_com2Select && strcmp(_com2Select,heroName)==0){
		return true;
	}
	if(_isTrainingSelect){
		if(networkArrayHasHero(_trainingTeamPicks,heroName)){
			return true;
		}
		if(networkArrayHasHero(_trainingEnemyPicks,heroName)){
			return true;
		}
	}
	return false;
}

void NetworkLayer::refreshTeamSelectorUI(){
	SpriteFrame* emptyFrame=SpriteFrameCache::getInstance()->spriteFrameByName("unknow_select.png");
	SpriteFrame* labelFrame=SpriteFrameCache::getInstance()->spriteFrameByName("com_label.png");
	ActionInterval* fd=FadeOut::create(1.0f);
	Action* seq=RepeatForever::create(Sequence::create(fd, fd->reverse(), nullptr));

	if(_comSelector1 && emptyFrame){
		_comSelector1->setSpriteFrame(emptyFrame);
	}
	if(_comSelector2 && emptyFrame){
		_comSelector2->setSpriteFrame(emptyFrame);
	}
	if(_comLabel1 && labelFrame){
		_comLabel1->stopAllActions();
		_comLabel1->setOpacity(255);
		_comLabel1->setSpriteFrame(labelFrame);
	}
	if(_comLabel2 && labelFrame){
		_comLabel2->stopAllActions();
		_comLabel2->setOpacity(255);
		_comLabel2->setSpriteFrame(labelFrame);
	}

	if(_playerSelect && !_com1Select && _comLabel1){
		_comLabel1->runAction(seq);
	}

	if(_com1Select){
		std::string com1Path=cocos2d::StringUtils::format("%s_small.png",_com1Select);
		SpriteFrame* com1Frame=SpriteFrameCache::getInstance()->spriteFrameByName(com1Path);
		if(com1Frame && _comSelector1){
			_comSelector1->setSpriteFrame(com1Frame);
		}
		if(_comLabel1){
			SpriteFrame* confirmedLabel=SpriteFrameCache::getInstance()->spriteFrameByName("com_label2.png");
			if(confirmedLabel){
				_comLabel1->stopAllActions();
				_comLabel1->setOpacity(255);
				_comLabel1->setSpriteFrame(confirmedLabel);
			}
		}
		if(!_com2Select && _comLabel2){
			_comLabel2->runAction(seq);
		}
	}

	if(_com2Select){
		std::string com2Path=cocos2d::StringUtils::format("%s_small.png",_com2Select);
		SpriteFrame* com2Frame=SpriteFrameCache::getInstance()->spriteFrameByName(com2Path);
		if(com2Frame && _comSelector2){
			_comSelector2->setSpriteFrame(com2Frame);
		}
		if(_comLabel2){
			SpriteFrame* confirmedLabel=SpriteFrameCache::getInstance()->spriteFrameByName("com_label2.png");
			if(confirmedLabel){
				_comLabel2->stopAllActions();
				_comLabel2->setOpacity(255);
				_comLabel2->setSpriteFrame(confirmedLabel);
			}
		}
	}
}

void NetworkLayer::refreshTrainingTeamSlots(){
	SpriteFrame* emptyFrame=SpriteFrameCache::getInstance()->spriteFrameByName("unknow_select.png");
	for(int i=0;i<_trainingTeamSize && i<5;i++){
		if(!_trainingTeamSlots[i]){
			continue;
		}
		if(i==0){
			if(_playerSelect){
				std::string framePath=cocos2d::StringUtils::format("%s_small.png",_playerSelect);
				SpriteFrame* frame=SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
				if(frame){
					_trainingTeamSlots[i]->setSpriteFrame(frame);
				}
			}else if(emptyFrame){
				_trainingTeamSlots[i]->setSpriteFrame(emptyFrame);
			}
		}else if((int)_trainingTeamPicks.size()>=i){
			const std::string& picked=_trainingTeamPicks[i-1];
			std::string framePath=cocos2d::StringUtils::format("%s_small.png",picked.c_str());
			SpriteFrame* frame=SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
			if(frame){
				_trainingTeamSlots[i]->setSpriteFrame(frame);
			}
		}else if(emptyFrame){
			_trainingTeamSlots[i]->setSpriteFrame(emptyFrame);
		}
	}
}

void NetworkLayer::refreshTrainingEnemySlots(){
	SpriteFrame* emptyFrame=SpriteFrameCache::getInstance()->spriteFrameByName("unknow_select.png");
	for(int i=0;i<_trainingTeamSize && i<5;i++){
		if(!_trainingEnemySlots[i]){
			continue;
		}
		if((int)_trainingEnemyPicks.size()>i){
			const std::string& picked=_trainingEnemyPicks[i];
			std::string framePath=cocos2d::StringUtils::format("%s_small.png",picked.c_str());
			SpriteFrame* frame=SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
			if(frame){
				_trainingEnemySlots[i]->setSpriteFrame(frame);
			}
		}else if(emptyFrame){
			_trainingEnemySlots[i]->setSpriteFrame(emptyFrame);
		}
	}
}

void NetworkLayer::unselectHero(const char* heroName){
	if(!heroName || !isHeroConfirmedSelected(heroName)){
		return;
	}

	SelectButton* btn=findSelectButton(heroName);
	if(btn){
		btn->spriteResetGrey();
	}

	if(_playerSelect && strcmp(_playerSelect,heroName)==0){
		_playerSelect=NULL;
		_trainingPlayerPick.clear();
		if(!_selectImg && selectArray && selectArray->count()>0){
			SelectButton* firstBtn=(SelectButton*)selectArray->objectAtIndex(0);
			_selectImg=Sprite::createWithSpriteFrameName("1p.png");
			_selectImg->setAnchorPoint(Vec2(0,0));
			_selectImg->setPosition(Vec2(firstBtn->getPositionX()-2,firstBtn->getPositionY()-2));
			this->addChild(_selectImg,500);
			ActionInterval* blink=Blink::create(0.6f,1);
			_selectImg->runAction(RepeatForever::create(blink));
		}
	}else if(_com1Select && strcmp(_com1Select,heroName)==0){
		_com1Select=NULL;
	}else if(_com2Select && strcmp(_com2Select,heroName)==0){
		_com2Select=NULL;
	}else if(_isTrainingSelect){
		_trainingTeamPicks.erase(std::remove(_trainingTeamPicks.begin(), _trainingTeamPicks.end(), heroName), _trainingTeamPicks.end());
		_trainingEnemyPicks.erase(std::remove(_trainingEnemyPicks.begin(), _trainingEnemyPicks.end(), heroName), _trainingEnemyPicks.end());
	}

	if(_isTrainingSelect){
		refreshTrainingTeamSlots();
		refreshTrainingEnemySlots();
	}else if(adResult==1){
		refreshTeamSelectorUI();
	}

	isStart=false;
}

void NetworkLayer::unselectAll(){
	if(!_playerSelect && !_com1Select && !_com2Select
		&& _trainingPlayerPick.empty()
		&& _trainingTeamPicks.empty()
		&& _trainingEnemyPicks.empty()){
		return;
	}

	if(selectArray){
		for(unsigned int selectIndex=0; selectIndex<selectArray->count(); ++selectIndex){
			Ref* pObject=selectArray->objectAtIndex(selectIndex);
			SelectButton* btn=(SelectButton*)pObject;
			btn->spriteResetGrey();
		}
	}

	_playerSelect=NULL;
	_com1Select=NULL;
	_com2Select=NULL;
	_trainingPlayerPick.clear();
	_trainingTeamPicks.clear();
	_trainingEnemyPicks.clear();

	if(_isTrainingSelect){
		refreshTrainingTeamSlots();
		refreshTrainingEnemySlots();
	}else if(adResult==1){
		refreshTeamSelectorUI();
	}

	if(!_selectImg && selectArray && selectArray->count()>0){
		SelectButton* firstBtn=(SelectButton*)selectArray->objectAtIndex(0);
		_selectImg=Sprite::createWithSpriteFrameName("1p.png");
		_selectImg->setAnchorPoint(Vec2(0,0));
		_selectImg->setPosition(Vec2(firstBtn->getPositionX()-2,firstBtn->getPositionY()-2));
		this->addChild(_selectImg,500);
		ActionInterval* blink=Blink::create(0.6f,1);
		_selectImg->runAction(RepeatForever::create(blink));
	}

	isStart=false;
}

void NetworkLayer::onUnselectAllBtn(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
	unselectAll();
}

void NetworkLayer::setSelected(Ref* sender){

	 SelectButton* btn=(SelectButton*) sender;
	 if(_selectImg){
		_selectImg->setPosition(Vec2(btn->getPositionX()-2,btn->getPositionY()-2));
	 }

	if(adResult!=1 && !_isTrainingSelect && _playerSelect && !isHeroConfirmedSelected(btn->getCharName()->getCString())){
		return;
	}


	_selectHero=btn->getCharName()->getCString();

	if(_isTrainingSelect){
		if(!_selectHero || strcmp(_selectHero,"None")==0){
			return;
		}

		int selectedTeamNeed=_trainingTeamSize-1;
		if(selectedTeamNeed<0){
			selectedTeamNeed=0;
		}
		bool selectedTeamFull=_trainingRandomTeam || (int)_trainingTeamPicks.size()>=selectedTeamNeed;
		bool selectedEnemyFull=_trainingRandomEnemy || (int)_trainingEnemyPicks.size()>=_trainingTeamSize;
		if(_playerSelect && selectedTeamFull && selectedEnemyFull && !isHeroConfirmedSelected(_selectHero)){
			return;
		}

		if(_heroHalf){
			_heroHalf->removeFromParent();
		}
		std::string path=cocos2d::StringUtils::format("%s_half.png",_selectHero);
		_heroHalf=Sprite::createWithSpriteFrameName(path);
		if(_heroHalf){
			_heroHalf->setAnchorPoint(Vec2(0,0));
			_heroHalf->setPosition(Vec2(10,10));
			this->addChild(_heroHalf,1);
		}

		if(_heroName){
			_heroName->removeFromParent();
		}
		std::string namePath=cocos2d::StringUtils::format("%s_font.png",_selectHero);
		_heroName=Sprite::createWithSpriteFrameName(namePath);
		if(_heroName){
			_heroName->setAnchorPoint(Vec2(0.5f,0));
			_heroName->setPosition(Vec2(100,20));
			this->addChild(_heroName,5);
		}

		if(isHeroConfirmedSelected(_selectHero)){
			return;
		}

		if(btn->_clickTime<2){
			return;
		}

		if(!_playerSelect){
			_trainingPlayerPick=_selectHero;
			_playerSelect=_trainingPlayerPick.c_str();
			std::string framePath=cocos2d::StringUtils::format("%s_small.png",_selectHero);
			SpriteFrame* frame=SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
			if(frame && _trainingTeamSlots[0]){
				_trainingTeamSlots[0]->setSpriteFrame(frame);
			}
			return;
		}

		if(strcmp(_selectHero,_playerSelect)==0 || networkArrayHasHero(_trainingTeamPicks,_selectHero) || networkArrayHasHero(_trainingEnemyPicks,_selectHero)){
			return;
		}

		int teamNeed=_trainingTeamSize-1;
		if(teamNeed<0){
			teamNeed=0;
		}
		if(!_trainingRandomTeam && (int)_trainingTeamPicks.size()<teamNeed){
			int slotIndex=(int)_trainingTeamPicks.size()+1;
			_trainingTeamPicks.push_back(_selectHero);
			std::string framePath=cocos2d::StringUtils::format("%s_small.png",_selectHero);
			SpriteFrame* frame=SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
			if(frame && slotIndex<5 && _trainingTeamSlots[slotIndex]){
				_trainingTeamSlots[slotIndex]->setSpriteFrame(frame);
			}
			return;
		}

		if(!_trainingRandomEnemy && (int)_trainingEnemyPicks.size()<_trainingTeamSize){
			int slotIndex=(int)_trainingEnemyPicks.size();
			_trainingEnemyPicks.push_back(_selectHero);
			std::string framePath=cocos2d::StringUtils::format("%s_small.png",_selectHero);
			SpriteFrame* frame=SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
			if(frame && slotIndex<5 && _trainingEnemySlots[slotIndex]){
				_trainingEnemySlots[slotIndex]->setSpriteFrame(frame);
			}
			return;
		}

		return;
	}

	if(_isHardCoreMode){

		ActionInterval*  fd=FadeOut::create(1.0f);
		Action* seq=RepeatForever::create(Sequence::create(fd, fd->reverse(), nullptr));

		if(isHeroConfirmedSelected(_selectHero)){
			_heroHalf->removeFromParent();
			const char* charName=btn->getCharName()->getCString();
			std::string path=cocos2d::StringUtils::format("%s_half.png", charName);
			_heroHalf=Sprite::createWithSpriteFrameName(path);
			_heroHalf->setAnchorPoint(Vec2(0,0));
			_heroHalf->setPosition(Vec2(10,10));
			this->addChild(_heroHalf,1);

			_heroName->removeFromParent();
			std::string namePath=cocos2d::StringUtils::format("%s_font.png",charName);
			_heroName=Sprite::createWithSpriteFrameName(namePath);
			_heroName->setAnchorPoint(Vec2(0.5f,0));
			_heroName->setPosition(Vec2(100,20));
			this->addChild(_heroName,5);
			return;
		}

		if(!_playerSelect){

			if(btn->_clickTime>=2){
					_playerSelect=_selectHero;

					if(adResult!=1 && !_isTrainingSelect){
						_selectImg->removeFromParent();
						_selectImg=NULL;
					}else{
						this->getComLabel1()->runAction(seq);
					}

			}

			_heroHalf->removeFromParent();
			const char* charName=btn->getCharName()->getCString();


			std::string path=cocos2d::StringUtils::format("%s_half.png", charName);
			_heroHalf=Sprite::createWithSpriteFrameName(path);

			_heroHalf->setAnchorPoint(Vec2(0,0));
			_heroHalf->setPosition(Vec2(10,10));
			this->addChild(_heroHalf,1);

			_heroName->removeFromParent();
			std::string namePath=cocos2d::StringUtils::format("%s_font.png",charName);
			_heroName=Sprite::createWithSpriteFrameName(namePath);
			_heroName->setAnchorPoint(Vec2(0.5f,0));
			_heroName->setPosition(Vec2(100,20));
			this->addChild(_heroName,5);



		}else if(!_com1Select){
			std::string framePath=cocos2d::StringUtils::format("%s_small.png",_selectHero);
			SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
			this->getComSelector1()->setSpriteFrame(frame);

			if(btn->_clickTime>=2){
				_com1Select=_selectHero;

				this->getComLabel1()->stopAllActions();
				this->getComLabel1()->setOpacity(255);
				SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName("com_label2.png");
				this->getComLabel1()->setSpriteFrame(frame);
				this->getComLabel2()->runAction(seq);
			}



		}else if(!_com2Select){


			std::string framePath=cocos2d::StringUtils::format("%s_small.png",_selectHero);
			SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
			this->getComSelector2()->setSpriteFrame(frame);

			if(btn->_clickTime>=2){
				_com2Select=_selectHero;

				this->getComLabel2()->stopAllActions();
				this->getComLabel2()->setOpacity(255);
				SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName("com_label2.png");
				this->getComLabel2()->setSpriteFrame(frame);

				_selectImg->removeFromParent();
				_selectImg=NULL;
			}


		}

	}


}



void NetworkLayer::onError(Ref* sender){

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->end();
	return;

}


void NetworkLayer::onSkillMenu(Ref* sender){
	if(!_selectHero){
		return;
	}
	if(strcmp(_selectHero,"Pain")==0 || strcmp(_selectHero,"Orochimaru")==0 ){
		return;

	}

	Scene* pscene =Scene::create();
	SkillLayer* skillLayer=SkillLayer::create();
	skillLayer->setDelegate2(this);
	skillLayer->initInterface();
	pscene->addChild(skillLayer);

	Director::getInstance()->pushScene(pscene);

}

void  NetworkLayer::onRankingBtn(Ref* sender){
	SimpleAudioEngine::getInstance()->stopAllEffects();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Scene* pscene =Scene::create();
	RakingLayer* rakingLayer=RakingLayer::create();
	rakingLayer->setDelegate(this);
	pscene->addChild(rakingLayer);

	Director::getInstance()->pushScene(pscene);
}

void  NetworkLayer::onQuestBtn(Ref* sender){

	CCTips *tip=CCTips::create("ComingSoon");
	this->addChild(tip,5000);
	return;

}



void NetworkLayer::onGameStart(Ref* sender){



	if (!isStart){

		KTools* tool= KTools::create();




		isStart=true;
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");


		if(_isTrainingSelect){
			char* heroList[]={"Konan","Sakura","Naruto","Sai","Deidara",
				"Kakashi","Itachi","Tenten","Jiraiya","Suigetsu",
				"Tsunade","Tobirama","Shisui","Neji","Ino","Asuma","Gaara",
				"Karin","Sasuke","Hidan","Choji","Kankuro",
				"Shino","Minato","Tobi","Kakuzu","Hinata",
				"Shikamaru","Chiyo","Kisame",
				"Hiruzen","Kiba","Jugo","Lee"
			};
			const char* teamPreset[]={"Sakura","Kakashi","Lee","Tsunade"};
			const char* enemyPreset[]={"Deidara","Itachi","Kisame","Hidan","Kakuzu"};
			int num=sizeof(heroList)/sizeof(char*);
			int teamSize=_trainingTeamSize;
			if(teamSize<1){
				teamSize=1;
			}else if(teamSize>5){
				teamSize=5;
			}

			CCArray* tempHeros=CCArray::create();
			std::vector<std::string> used;
			const char* playerName=!_trainingPlayerPick.empty() ? _trainingPlayerPick.c_str() : (_playerSelect ? _playerSelect : _selectHero);
			if(!playerName || strcmp(playerName,"None")==0){
				playerName="Naruto";
			}
			used.push_back(playerName);
			networkAddHero(tempHeros,playerName,"Player","Konoha");

			for(int i=1;i<teamSize;i++){
				const char* heroName=NULL;
				if(!_trainingRandomTeam && (int)_trainingTeamPicks.size()>=i){
					heroName=_trainingTeamPicks[i-1].c_str();
				}else{
					heroName=networkPickHero(used,(const char**)heroList,num,teamPreset,sizeof(teamPreset)/sizeof(char*),i-1,_trainingRandomTeam);
				}
				if(!heroName || networkContainsHero(used,heroName)){
					heroName=networkPickHero(used,(const char**)heroList,num,teamPreset,sizeof(teamPreset)/sizeof(char*),i-1,true);
				}else{
					used.push_back(heroName);
				}
				networkAddHero(tempHeros,heroName,"Com","Konoha");
			}

			for(int i=0;i<teamSize;i++){
				const char* heroName=NULL;
				if(!_trainingRandomEnemy && (int)_trainingEnemyPicks.size()>i){
					heroName=_trainingEnemyPicks[i].c_str();
				}else{
					heroName=networkPickHero(used,(const char**)heroList,num,enemyPreset,sizeof(enemyPreset)/sizeof(char*),i,_trainingRandomEnemy);
				}
				if(!heroName || networkContainsHero(used,heroName)){
					heroName=networkPickHero(used,(const char**)heroList,num,enemyPreset,sizeof(enemyPreset)/sizeof(char*),i,true);
				}else{
					used.push_back(heroName);
				}
				networkAddHero(tempHeros,heroName,"Com","Akatsuki");
			}

			Scene* loadScene=Scene::create();
			LoadLayer* loadLayer=LoadLayer::create();
			loadLayer->tempHeros=tempHeros;
			loadLayer->_isHardCoreMode=true;
			loadLayer->_isRandomChar=_trainingRandomTeam || _trainingRandomEnemy;
			loadLayer->_isTrainingMode=_isTrainingMode;
			loadLayer->_selectedMap=_selectedMap;
			loadLayer->_isDuelMode=(teamSize==1 && !_isTrainingMode);
			loadLayer->_duelKillLimit=_trainingKillLimit;
			loadScene->addChild(loadLayer);
			loadLayer->preloadAudio();
			Director::getInstance()->replaceScene(TransitionFade::create(1.5f,loadScene));
			return;
		}
		CCDictionary* dic=CCDictionary::create();

		CCString* tmpChar;



		char* heroList[]={"Konan","Sakura","Naruto","Sai","Deidara",
			"Kakashi","Itachi","Tenten","Jiraiya","Suigetsu",
			"Tsunade","Tobirama","Shisui","Neji","Ino","Asuma","Gaara",
			"Karin","Sasuke","Hidan","Choji","Kankuro",
			"Shino","Minato","Tobi","Kakuzu","Hinata",
			"Shikamaru","Chiyo","Kisame",
			"Hiruzen","Kiba","Jugo","Lee"
		};



		int num= sizeof(heroList)/sizeof(char*);



		if(_playerSelect){
			tmpChar=CCString::create(_playerSelect);
		}else{
			srand((int)time(0));
			int num2=this->getSelectList()->count();
			int index=random(num2);
			tmpChar=(CCString*)_selectList->objectAtIndex(index);
			do{
				srand((int)time(0));
				int num=this->getSelectList()->count();
				int index=random(num);
				tmpChar=(CCString*)_selectList->objectAtIndex(index);
			}
			while(strcmp(tmpChar->getCString(),"Pain")==0 ||
				strcmp(tmpChar->getCString(),"Orochimaru")==0 ||
				strcmp(tmpChar->getCString(),"None")==0
				);


			_playerSelect=tmpChar->getCString();
			_isRandomChar=true;
		}


		srand((int)time(0));
		int zhenying=1;
		zhenying=random(2);

		const char* groupName;
		if(zhenying>0){
			 groupName="Konoha";
		}else{
			groupName="Akatsuki";
		}


		CCString* tmpRole=CCString::create("Player");
		CCString* tmpGroup=CCString::create(groupName);

		dic->setObject(tmpChar,"character");
		dic->setObject(tmpRole,"role");
		dic->setObject(tmpGroup,"group");

		CCArray* tempHeros=CCArray::createWithObject(dic);



		CCArray* realHero=CCArray::create();

		for(int i=0;i<num;i++){


			if(strcmp(_playerSelect,heroList[i])==0){
				continue;
			}

			if(_com1Select){
				if(strcmp(_com1Select,heroList[i])==0){
					continue;
				}
			}
			if(_com2Select){
				if(strcmp(_com2Select,heroList[i])==0){
					continue;
				}
			}


			CCString* hero=CCString::create(heroList[i]);
			realHero->addObject(hero);
		}


		for(int i=0;i<5;i++){
			CCString* hero;
			if(i<2){

				if(i==0 && _isHardCoreMode && _com1Select){
					hero=CCString::create(_com1Select);
				}else if(i==1 && _isHardCoreMode && _com2Select){
					hero=CCString::create(_com2Select);
				}else{
					int length=realHero->count();
					srand((int)time(0));
					int index=random(length);
					if(index==(int)realHero->count()){
						index=realHero->count()-1;
					}

					Ref* tempObject=realHero->objectAtIndex(index);
					hero=(CCString*) tempObject;
					realHero->removeObjectAtIndex(index);
				}

				dic=CCDictionary::create();
				tmpChar=CCString::create(hero->getCString());
				tmpRole=CCString::create("Com");
				if(zhenying>0){
				tmpGroup=CCString::create("Konoha");
				}else{
					tmpGroup=CCString::create("Akatsuki");
				}
				dic->setObject(tmpChar,"character");
				dic->setObject(tmpRole,"role");
				dic->setObject(tmpGroup,"group");

				tempHeros->addObject(dic);


			}else{
				int length=realHero->count();
				srand((int)time(0));
				int index=random(length);
				if(index==(int)realHero->count()){
					index=realHero->count()-1;
				}

				Ref* tempObject=realHero->objectAtIndex(index);
				CCString* hero=(CCString*) tempObject;

				dic=CCDictionary::create();
				tmpChar=CCString::create(hero->getCString());
				tmpRole=CCString::create("Com");
				if(zhenying>0){
					tmpGroup=CCString::create("Akatsuki");
				}else{
					tmpGroup=CCString::create("Konoha");

				}
				dic->setObject(tmpChar,"character");
				dic->setObject(tmpRole,"role");
				dic->setObject(tmpGroup,"group");

				tempHeros->addObject(dic);
				realHero->removeObjectAtIndex(index);

			}

		}



		Scene* loadScene=Scene::create();
		LoadLayer* loadLayer=LoadLayer::create();
		loadLayer->tempHeros=tempHeros;
		loadScene->addChild(loadLayer);

		loadLayer->_isHardCoreMode=_isHardCoreMode;
		loadLayer->_isRandomChar=_isRandomChar;
		loadLayer->_selectedMap=_selectedMap;
		loadLayer->preloadAudio();
		Director::getInstance()->replaceScene(TransitionFade::create(1.5f,loadScene));


	}
}


void NetworkLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		onBackKeyClicked();
	}
}

void NetworkLayer::onBackKeyClicked(){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
	Scene* menuScene =Scene::create();
	Layer* menuLayer=StartMenu::create();
	menuScene->addChild(menuLayer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,menuScene));

}



NetworkLayer* NetworkLayer::createTraining(int teamSize, bool randomTeam, bool randomEnemy, bool trainingMode, int killLimit){
	NetworkLayer* layer=new NetworkLayer();
	layer->_isTrainingSelect=true;
	layer->_isTrainingMode=trainingMode;
	layer->_trainingTeamSize=teamSize;
	layer->_trainingRandomTeam=randomTeam;
	layer->_trainingRandomEnemy=randomEnemy;
	layer->_trainingKillLimit=killLimit;
	layer->_isHardCoreMode=true;
	if(layer && layer->init()){
		layer->autorelease();
		return layer;
	}
	delete layer;
	return NULL;
}

NetworkLayer* NetworkLayer::create(){

	NetworkLayer* sl=new NetworkLayer();
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
