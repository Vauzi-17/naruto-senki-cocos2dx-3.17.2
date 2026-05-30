#include "NetworkLayer.h"
#include "Defines.h"
#include "StartMenu.h"
USING_NS_CC_EXT;
using namespace CocosDenshion;

#include<time.h>

static bool networkContainsHero(CCArray* used, const char* heroName)
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

static const char* networkPickHero(CCArray* used, const char** allHeroes, int heroCount, const char** presetList, int presetCount, int presetIndex, bool randomPick)
{
	if(!randomPick && presetIndex < presetCount && !networkContainsHero(used, presetList[presetIndex]))
	{
		used->addObject(CCString::create(presetList[presetIndex]));
		return presetList[presetIndex];
	}

	for(int attempt=0; attempt<heroCount*2; attempt++)
	{
		int index=random(heroCount);
		const char* heroName=allHeroes[index];
		if(!networkContainsHero(used, heroName))
		{
			used->addObject(CCString::create(heroName));
			return heroName;
		}
	}

	for(int i=0;i<heroCount;i++)
	{
		if(!networkContainsHero(used, allHeroes[i]))
		{
			used->addObject(CCString::create(allHeroes[i]));
			return allHeroes[i];
		}
	}

	return allHeroes[0];
}


static bool networkArrayHasHero(CCArray* list, const char* heroName)
{
	if(!list || !heroName){
		return false;
	}
	CCObject* pObject=NULL;
	CCARRAY_FOREACH(list,pObject)
	{
		CCString* value=(CCString*)pObject;
		if(strcmp(value->getCString(),heroName)==0){
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
	_trainingPlayerPick=NULL;
	_trainingTeamPicks=NULL;
	_trainingEnemyPicks=NULL;
	selectArray=NULL;

	_pageLayer1=NULL;
	_pageLayer2=NULL;
	_pageLayer3=NULL;

	

	isPosting=false;
	refreshBtn=NULL;
	
	_teamSelector=NULL;
	_trainingTeamPicks=CCArray::create();
	_trainingTeamPicks->retain();
	_trainingEnemyPicks=CCArray::create();
	_trainingEnemyPicks->retain();
	for(int i=0;i<5;i++){
		_trainingTeamSlots[i]=NULL;
		_trainingEnemySlots[i]=NULL;
	}
}


NetworkLayer::~NetworkLayer(void)
{
	CC_SAFE_RELEASE(selectArray);
	CC_SAFE_RELEASE(_trainingPlayerPick);
	CC_SAFE_RELEASE(_trainingTeamPicks);
	CC_SAFE_RELEASE(_trainingEnemyPicks);
	
}

bool NetworkLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!CCLayer::init());

			//�������м�	
		    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Menu.plist");
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
			


//			if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
//				if(tool->checkMD5()==0){
//					SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
//					CCDirector::sharedDirector()->end();
//					return false;
//				}
//			}
			
			
		char* bg_src="blue_bg.png";

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

		CCSprite* select_title=CCSprite::createWithSpriteFrameName("hcmode_title.png");
		
		select_title->setAnchorPoint(ccp(0,0));
		select_title->setPosition(ccp(2,winSize.height-select_title->getContentSize().height-2));
		this->addChild(select_title,3);

		

		if(!SaveError){

//			if(!isPosting){
//
//				isPosting=true;
//				if(!refreshBtn){
//					refreshBtn=CCSprite::createWithSpriteFrameName("refresh_btn.png");
//					refreshBtn->setPosition(ccp(winSize.width/2,winSize.height/2));
//					CCActionInterval* rt=CCRotateBy::create(0.3f,180,180);
//					refreshBtn->runAction(CCRepeatForever::create(rt));
//					this->addChild(refreshBtn,1000);
//
//				}
//
//				cocos2d::extension::CCHttpRequest* request = new  cocos2d::extension::CCHttpRequest(); 
//				std::string codeFinal;
//
//				std::string Andriodstr="";
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
//				JniMethodInfo minfo;
//				bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/NarutoSenki","getMacAdress", "()Ljava/lang/String;");  
//
//				if (isHave)   
//				{  
//					jstring jstr=(jstring) minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID); 
//					Andriodstr = Andriodstr + JniHelper::jstring2string(jstr);
//
//				}  
//#endif		
//				std::string Imeistr="";
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
//				JniMethodInfo minfo2;
//				bool isHave2 = JniHelper::getStaticMethodInfo(minfo2,"net/zakume/game/NarutoSenki","getImei", "()Ljava/lang/String;");  
//
//				if (isHave2)   
//				{  
//					jstring jstr2=(jstring) minfo2.env->CallStaticObjectMethod(minfo2.classID, minfo2.methodID); 
//					Imeistr = Imeistr + JniHelper::jstring2string(jstr2);
//
//				}  
//#endif	
//
//				std::string code1=CCString::createWithFormat("code=100&id=%d",MemberID)->getCString();
//				std::string code2=CCString::createWithFormat("&version=%d",CURRENT_VERSION)->getCString();
//				std::string url=SERVER"nsk/list.php?"+code1+code2+"&pw="+PWord+"&androidID="+Andriodstr+"&Imei="+Imeistr;
//				request->setUrl(url.c_str());  
//				request->setRequestType( cocos2d::extension::CCHttpRequest::kHttpGet);  
//
//				request->setResponseCallback(this, callfuncND_selector(NetworkLayer::onListRequestCompleted));  
//
//				request->setTag("Post_My_Data");  
//				cocos2d::extension::CCHttpClient::getInstance()->send(request);  
//				request->release();  
//
//			}
			_selectList=CCArray::create();
			selectArray=CCArray::create();
			_pageLayer1=CCLayer::create();
			_pageLayer2=CCLayer::create();
			_pageLayer3=CCLayer::create();

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

				_selectList->addObject(CCString::createWithFormat("%s",charName));
				
				CCString* path;

				path=CCString::createWithFormat("%s_select.png",charName);
				Column=i%7;
				Row=i/7;
				Page=int(i/21);
				SelectButton* select_btn=SelectButton::create(path->getCString());
				select_btn->setDelegate2(this);
				select_btn->setCharName(CCString::createWithFormat("%s",charName));
				if(Page==0){	
					select_btn->setPosition(ccp(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*Row)));
					_pageLayer1->addChild(select_btn,-Column);
				}else if(Page==1){
					select_btn->setPosition(ccp(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*(Row-3))));
					_pageLayer2->addChild(select_btn,-Column);

				}else if(Page==2){
					select_btn->setPosition(ccp(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*(Row-6))));
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

			page1_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("page1_off.png"),CCSprite::createWithSpriteFrameName("page1_on.png"),this,menu_selector(NetworkLayer::onPage1Btn));
			page2_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("page2_off.png"),CCSprite::createWithSpriteFrameName("page2_on.png"),this,menu_selector(NetworkLayer::onPage2Btn));
			page3_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("page3_off.png"),CCSprite::createWithSpriteFrameName("page3_on.png"),this,menu_selector(NetworkLayer::onPage3Btn));

			page_menu=CCMenu::create(page1_btn,page2_btn,page3_btn,NULL);

			page_menu->alignItemsHorizontallyWithPadding(20);
			page_menu->setPosition(ccp(winSize.width/2+48,42));
			this->addChild(page_menu,10);
			page1_btn->selected();



			selectArray->retain();
			_selectList->retain();

			CCString* hero=(CCString *)_selectList->objectAtIndex(0);

			const char* path=CCString::createWithFormat("%s_half.png",hero->getCString())->getCString();
			_heroHalf=CCSprite::createWithSpriteFrameName(path);
			_heroHalf->setAnchorPoint(ccp(0,0));
			_heroHalf->setPosition(ccp(10,10));
			this->addChild(_heroHalf,1);

			const char* namePath=CCString::createWithFormat("%s_font.png",hero->getCString())->getCString();
			_heroName=CCSprite::createWithSpriteFrameName(namePath);
			_heroName->setAnchorPoint(ccp(0.5f,0));
			_heroName->setPosition(ccp(100,20));
			this->addChild(_heroName,5);


			_selectImg=CCSprite::createWithSpriteFrameName("1p.png");
			_selectImg->setAnchorPoint(ccp(0,0));
			SelectButton* pObject=(SelectButton*) selectArray->objectAtIndex(0);
			_selectImg->setPosition(ccp(pObject->getPositionX()-2,pObject->getPositionY()-2));
			this->addChild(_selectImg,500);

			_selectHero=pObject->getCharName()->getCString();

			CCActionInterval* blink=CCBlink::create(0.6f,1);
			_selectImg->runAction(CCRepeatForever::create(blink));



			if(adResult==1 || _isTrainingSelect){
				_teamSelector=CCLayer::create();

				CCSprite* _teamBg=CCSprite::createWithSpriteFrameName("team_bg.png");
				_teamBg->setAnchorPoint(ccp(0,0));
				_teamBg->setPosition(ccp(0,185));
				_teamSelector->addChild(_teamBg);



				_comSelector1=CCSprite::createWithSpriteFrameName("unknow_select.png");
				_comSelector1->setAnchorPoint(ccp(0,0));
				_comSelector1->setPosition(ccp(2,194));
				_teamSelector->addChild(_comSelector1);

				_comLabel1=CCSprite::createWithSpriteFrameName("com_label.png");
				_comLabel1->setAnchorPoint(ccp(0.5,0.5f));
				_comLabel1->setPosition(ccp(_comSelector1->getPositionX()+_comSelector1->getContentSize().width+2+18,_comSelector1->getPositionY()+_comSelector1->getContentSize().height/2));
				_teamSelector->addChild(_comLabel1);


				_comSelector2=CCSprite::createWithSpriteFrameName("unknow_select.png");
				_comSelector2->setAnchorPoint(ccp(0,0));
				_comSelector2->setPosition(ccp(_comSelector1->getPositionX()+_comSelector1->getContentSize().width+40,_comSelector1->getPositionY()));
				_teamSelector->addChild(_comSelector2);


				_comLabel2=CCSprite::createWithSpriteFrameName("com_label.png");
				_comLabel2->setAnchorPoint(ccp(0.5,0.5f));
				_comLabel2->setPosition(ccp(_comSelector2->getPositionX()+_comSelector2->getContentSize().width+2+18,_comSelector2->getPositionY()+_comSelector2->getContentSize().height/2));
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
					_comSelector1->setPosition(ccp(teamStartX,slotY));
					_comSelector2->setPosition(ccp(teamStartX+slotGap,slotY));
					_comSelector2->setVisible(_trainingTeamSize>1);

					_trainingTeamSlots[0]=_comSelector1;
					_trainingTeamSlots[1]=_comSelector2;
					for(int i=2;i<_trainingTeamSize && i<5;i++){
						_trainingTeamSlots[i]=CCSprite::createWithSpriteFrameName("unknow_select.png");
						_trainingTeamSlots[i]->setAnchorPoint(ccp(0,0));
						_trainingTeamSlots[i]->setScale(slotScale);
						_trainingTeamSlots[i]->setPosition(ccp(teamStartX+slotGap*i,slotY));
						_teamSelector->addChild(_trainingTeamSlots[i]);
					}
					for(int i=0;i<_trainingTeamSize && i<5;i++){
						_trainingEnemySlots[i]=CCSprite::createWithSpriteFrameName("unknow_select.png");
						_trainingEnemySlots[i]->setAnchorPoint(ccp(0,0));
						_trainingEnemySlots[i]->setScale(slotScale);
						_trainingEnemySlots[i]->setPosition(ccp(enemyStartX+slotGap*i,slotY));
						_teamSelector->addChild(_trainingEnemySlots[i]);
					}
				}

				this->addChild(_teamSelector,_isTrainingSelect ? 4 : 50);
			}


			//���а�
			this->updateMapPreview();

			CCMenuItem* ranking_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ranking_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onRankingBtn));
			CCMenu* menu3 =CCMenu::create(ranking_btn,NULL);
			ranking_btn->setAnchorPoint(ccp(1,0.5f));
			menu3->setPosition(winSize.width-15,winSize.height-34);
			this->addChild(menu3,5);



		

			CCMenuItem* start_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("start_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onGameStart));
			CCMenu* menu =CCMenu::create(start_bt,NULL);
			menu->setAnchorPoint(ccp(0,0));
			menu->setPosition(winSize.width-40,36);
			this->addChild(menu,80);



			CCMenuItem* skill_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("skill_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onSkillMenu));
			CCMenu* menu2 =CCMenu::create(skill_bt,NULL);
			menu2->setAnchorPoint(ccp(0,0));
			menu2->setPosition(winSize.width-35,96);
			this->addChild(menu2,80);

			CCMenuItem* unselect_all_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("change_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onUnselectAllBtn));
			CCMenu* unselectMenu=CCMenu::create(unselect_all_bt,NULL);
			unselectMenu->setAnchorPoint(ccp(0,0));
			unselectMenu->setPosition(38,156);
			this->addChild(unselectMenu,80);









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
void NetworkLayer::onListRequestCompleted(CCNode *sender ,void *data){


	isPosting=false;
	//ˢ�°�ť
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}

	cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;    
	if (!response)    
	{  
		return;    
	}   
	if (0 != strlen(response->getHttpRequest()->getTag()))   
	{  
		//CCLOG("%s completed", response->getHttpRequest()->getTag());  
	}  
	int statusCode = response->getResponseCode();  
	char statusString[64] = {};  
	sprintf(statusString ,"Http status code:%d ,tag = %s" ,statusCode ,response->getHttpRequest()->getTag());  
	CCLOG("response code:%d" ,statusCode);  
	if (!response->isSucceed())  
	{  
		CCLOG("response failed");  
		CCLOG("error buffer:%s" ,response->getErrorBuffer());
		CCTips *tip=CCTips::create("ServerError");
		this->addChild(tip,5000);
		return;
	}  
	std::vector<char> *buffer = response->getResponseData();  
	printf("Http response,dump data:");  


	Document doc;
	std::string buf(buffer->begin(),buffer->end()); 	
	const char* json=(char*) buf.c_str();
	doc.Parse<kParseDefaultFlags>(json);
	rapidjson::Value &arr=doc;
	int Column=0;
	int Row=0;
	int Page=0;

	
	if(arr.IsArray()){

		_selectList=CCArray::create();
		selectArray=CCArray::create();
		_pageLayer1=CCLayer::create();
		_pageLayer2=CCLayer::create();
		_pageLayer3=CCLayer::create();

		for (SizeType i = 0; i < arr.Size(); i++)  {
			rapidjson::Value & node=arr[i];
			rapidjson::Value & nameValue= node;
			std::string nameString=nameValue.GetString();
			if(strcmp(nameString.c_str(),"None")!=0 &&
				strcmp(nameString.c_str(),"Limited1")!=0 &&
				strcmp(nameString.c_str(),"Limited2")!=0 &&
				strcmp(nameString.c_str(),"Limited3")!=0 &&
				strcmp(nameString.c_str(),"Limited4")!=0 &&
				strcmp(nameString.c_str(),"Limited5")!=0
				){
				_selectList->addObject(CCString::createWithFormat("%s",nameString.c_str()));
			}
				
			CCString* path;
			
			path=CCString::createWithFormat("%s_select.png",nameString.c_str());
			Column=i%7;
			Row=i/7;
			Page=int(i/21);
			SelectButton* select_btn=SelectButton::create(path->getCString());
			select_btn->setDelegate2(this);
			select_btn->setCharName(CCString::createWithFormat("%s",nameString.c_str()));
			if(Page==0){	
				select_btn->setPosition(ccp(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*Row)));
				_pageLayer1->addChild(select_btn,-Column);
			}else if(Page==1){
				select_btn->setPosition(ccp(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*(Row-3))));
				_pageLayer2->addChild(select_btn,-Column);
				
			}else if(Page==2){
				select_btn->setPosition(ccp(winSize.width/2-36+(Column-1)*27+Column/4*10,winSize.height-112-(72*(Row-6))));
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

		page1_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("page1_off.png"),CCSprite::createWithSpriteFrameName("page1_on.png"),this,menu_selector(NetworkLayer::onPage1Btn));
		page2_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("page2_off.png"),CCSprite::createWithSpriteFrameName("page2_on.png"),this,menu_selector(NetworkLayer::onPage2Btn));
		page3_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("page3_off.png"),CCSprite::createWithSpriteFrameName("page3_on.png"),this,menu_selector(NetworkLayer::onPage3Btn));

		page_menu=CCMenu::create(page1_btn,page2_btn,page3_btn,NULL);
		
		page_menu->alignItemsHorizontallyWithPadding(20);
		page_menu->setPosition(ccp(winSize.width/2+48,42));
		this->addChild(page_menu,10);
		page1_btn->selected();



		selectArray->retain();
		_selectList->retain();
		
		CCString* hero=(CCString *)_selectList->objectAtIndex(0);

		const char* path=CCString::createWithFormat("%s_half.png",hero->getCString())->getCString();
		_heroHalf=CCSprite::createWithSpriteFrameName(path);
		_heroHalf->setAnchorPoint(ccp(0,0));
		_heroHalf->setPosition(ccp(10,10));
		this->addChild(_heroHalf,1);

		const char* namePath=CCString::createWithFormat("%s_font.png",hero->getCString())->getCString();
		_heroName=CCSprite::createWithSpriteFrameName(namePath);
		_heroName->setAnchorPoint(ccp(0.5f,0));
		_heroName->setPosition(ccp(100,20));
		this->addChild(_heroName,5);


		_selectImg=CCSprite::createWithSpriteFrameName("1p.png");
		_selectImg->setAnchorPoint(ccp(0,0));
		SelectButton* pObject=(SelectButton*) selectArray->objectAtIndex(0);
		_selectImg->setPosition(ccp(pObject->getPositionX()-2,pObject->getPositionY()-2));
		this->addChild(_selectImg,500);

		_selectHero=pObject->getCharName()->getCString();

		CCActionInterval* blink=CCBlink::create(0.6f,1);
		_selectImg->runAction(CCRepeatForever::create(blink));



		 if(adResult==1 || _isTrainingSelect){
			 _teamSelector=CCLayer::create();

			 CCSprite* _teamBg=CCSprite::createWithSpriteFrameName("team_bg.png");
			 _teamBg->setAnchorPoint(ccp(0,0));
			 _teamBg->setPosition(ccp(0,185));
			 _teamSelector->addChild(_teamBg);



			 _comSelector1=CCSprite::createWithSpriteFrameName("unknow_select.png");
			 _comSelector1->setAnchorPoint(ccp(0,0));
			 _comSelector1->setPosition(ccp(2,194));
			 _teamSelector->addChild(_comSelector1);

			 _comLabel1=CCSprite::createWithSpriteFrameName("com_label.png");
			 _comLabel1->setAnchorPoint(ccp(0.5,0.5f));
			 _comLabel1->setPosition(ccp(_comSelector1->getPositionX()+_comSelector1->getContentSize().width+2+18,_comSelector1->getPositionY()+_comSelector1->getContentSize().height/2));
			 _teamSelector->addChild(_comLabel1);


			 _comSelector2=CCSprite::createWithSpriteFrameName("unknow_select.png");
			 _comSelector2->setAnchorPoint(ccp(0,0));
			 _comSelector2->setPosition(ccp(_comSelector1->getPositionX()+_comSelector1->getContentSize().width+40,_comSelector1->getPositionY()));
			 _teamSelector->addChild(_comSelector2);


			 _comLabel2=CCSprite::createWithSpriteFrameName("com_label.png");
			 _comLabel2->setAnchorPoint(ccp(0.5,0.5f));
			 _comLabel2->setPosition(ccp(_comSelector2->getPositionX()+_comSelector2->getContentSize().width+2+18,_comSelector2->getPositionY()+_comSelector2->getContentSize().height/2));
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
					_comSelector1->setPosition(ccp(teamStartX,slotY));
					_comSelector2->setPosition(ccp(teamStartX+slotGap,slotY));
					_comSelector2->setVisible(_trainingTeamSize>1);

					_trainingTeamSlots[0]=_comSelector1;
					_trainingTeamSlots[1]=_comSelector2;
					for(int i=2;i<_trainingTeamSize && i<5;i++){
						_trainingTeamSlots[i]=CCSprite::createWithSpriteFrameName("unknow_select.png");
						_trainingTeamSlots[i]->setAnchorPoint(ccp(0,0));
						_trainingTeamSlots[i]->setScale(slotScale);
						_trainingTeamSlots[i]->setPosition(ccp(teamStartX+slotGap*i,slotY));
						_teamSelector->addChild(_trainingTeamSlots[i]);
					}
					for(int i=0;i<_trainingTeamSize && i<5;i++){
						_trainingEnemySlots[i]=CCSprite::createWithSpriteFrameName("unknow_select.png");
						_trainingEnemySlots[i]->setAnchorPoint(ccp(0,0));
						_trainingEnemySlots[i]->setScale(slotScale);
						_trainingEnemySlots[i]->setPosition(ccp(enemyStartX+slotGap*i,slotY));
						_teamSelector->addChild(_trainingEnemySlots[i]);
					}
				}
			 this->addChild(_teamSelector,_isTrainingSelect ? 4 : 50);
		 }
		
		
		//���а�
		this->updateMapPreview();

		CCMenuItem* ranking_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("ranking_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onRankingBtn));
		CCMenu* menu3 =CCMenu::create(ranking_btn,NULL);
		ranking_btn->setAnchorPoint(ccp(1,0.5f));
		menu3->setPosition(winSize.width-15,winSize.height-34);
		this->addChild(menu3,5);


		
		CCMenuItem* quest_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("quest_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onQuestBtn));
		CCMenu* menu4 =CCMenu::create(quest_btn,NULL);
		quest_btn->setAnchorPoint(ccp(0,0.5f));
		menu4->setPosition(4,winSize.height-56);
		this->addChild(menu4,5);


		CCMenuItem* start_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("start_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onGameStart));
		CCMenu* menu =CCMenu::create(start_bt,NULL);
		menu->setAnchorPoint(ccp(0,0));
		menu->setPosition(winSize.width-40,36);
		this->addChild(menu,80);



		CCMenuItem* skill_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("skill_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onSkillMenu));
		CCMenu* menu2 =CCMenu::create(skill_bt,NULL);
		menu2->setAnchorPoint(ccp(0,0));
		menu2->setPosition(winSize.width-35,96);
		this->addChild(menu2,80);

		CCMenuItem* unselect_all_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("change_btn.png"),NULL,NULL,this,menu_selector(NetworkLayer::onUnselectAllBtn));
		CCMenu* unselectMenu=CCMenu::create(unselect_all_bt,NULL);
		unselectMenu->setAnchorPoint(ccp(0,0));
		unselectMenu->setPosition(38,156);
		this->addChild(unselectMenu,80);
	}else{
		CCTips *tip=CCTips::create(buf.c_str());
		this->addChild(tip,5000);
	}




	

	

}




void  NetworkLayer::onPage1Btn(CCObject* sender){
	SimpleAudioEngine::sharedEngine()->playEffect(SELECT_SOUND);
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


void  NetworkLayer::onPage2Btn(CCObject* sender){
	SimpleAudioEngine::sharedEngine()->playEffect(SELECT_SOUND);
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


void  NetworkLayer::onPage3Btn(CCObject* sender){
	SimpleAudioEngine::sharedEngine()->playEffect(SELECT_SOUND);
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

void NetworkLayer::onMapBtn(CCObject* sender){
	SimpleAudioEngine::sharedEngine()->playEffect(SELECT_SOUND);
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
    CCPoint mapPos=ccp(65, winSize.height-70);
//    CCPoint mapPos=ccp(70,250);

	CCNode* oldPreview=this->getChildByTag(previewTag);
	if(oldPreview){
		oldPreview->removeFromParent();
	}
	CCNode* oldLabel=this->getChildByTag(labelTag);
	if(oldLabel){
		oldLabel->removeFromParent();
	}

	_mapPreview=CCSprite::create(CCString::createWithFormat("Tiles/map_bg%d.png",_selectedMap+1)->getCString());
	if(_mapPreview){
		_mapPreview->setScale(0.205f);
		_mapPreview->setPosition(ccp(mapPos.x,mapPos.y+6));
		this->addChild(_mapPreview,81,previewTag);
	}

	_mapLabel=CCLabelBMFont::create(CCString::createWithFormat("MAP %d",_selectedMap+1)->getCString(),"Fonts/1.fnt");
	if(_mapLabel){
		_mapLabel->setScale(0.25f);
		_mapLabel->setAnchorPoint(ccp(0.5f,0.5f));
		_mapLabel->setPosition(ccp(mapPos.x,mapPos.y-18));
		this->addChild(_mapLabel,83,labelTag);
	}

	if(!this->getChildByTag(buttonTag)){
		CCSprite* mapNormal=CCSprite::createWithSpriteFrameName("minimap_bg.png");
		CCSprite* mapSelected=CCSprite::createWithSpriteFrameName("minimap_bg.png");
		mapNormal->setOpacity(120);
		mapSelected->setOpacity(180);
		CCMenuItemSprite* mapBtn=CCMenuItemSprite::create(
			mapNormal,
			mapSelected,
			this,
			menu_selector(NetworkLayer::onMapBtn));
		CCMenu* mapMenu=CCMenu::create(mapBtn,NULL);
		mapMenu->setPosition(mapPos);
		this->addChild(mapMenu,80,buttonTag);
	}
}


SelectButton* NetworkLayer::findSelectButton(const char* heroName){
	if(!heroName || !selectArray){
		return NULL;
	}
	CCObject* pObject=NULL;
	CCARRAY_FOREACH(selectArray,pObject){
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
	CCSpriteFrame* emptyFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("unknow_select.png");
	CCSpriteFrame* labelFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("com_label.png");
	CCActionInterval* fd=CCFadeOut::create(1.0f);
	CCAction* seq=CCRepeatForever::create(cocos2d::cocos2dx2_compat::createSequence(fd,fd->reverse(),NULL));

	if(_comSelector1 && emptyFrame){
		_comSelector1->setDisplayFrame(emptyFrame);
	}
	if(_comSelector2 && emptyFrame){
		_comSelector2->setDisplayFrame(emptyFrame);
	}
	if(_comLabel1 && labelFrame){
		_comLabel1->stopAllActions();
		_comLabel1->setOpacity(255);
		_comLabel1->setDisplayFrame(labelFrame);
	}
	if(_comLabel2 && labelFrame){
		_comLabel2->stopAllActions();
		_comLabel2->setOpacity(255);
		_comLabel2->setDisplayFrame(labelFrame);
	}

	if(_playerSelect && !_com1Select && _comLabel1){
		_comLabel1->runAction(seq);
	}

	if(_com1Select){
		CCSpriteFrame* com1Frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",_com1Select)->getCString());
		if(com1Frame && _comSelector1){
			_comSelector1->setDisplayFrame(com1Frame);
		}
		if(_comLabel1){
			CCSpriteFrame* confirmedLabel=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("com_label2.png");
			if(confirmedLabel){
				_comLabel1->stopAllActions();
				_comLabel1->setOpacity(255);
				_comLabel1->setDisplayFrame(confirmedLabel);
			}
		}
		if(!_com2Select && _comLabel2){
			_comLabel2->runAction(seq);
		}
	}

	if(_com2Select){
		CCSpriteFrame* com2Frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",_com2Select)->getCString());
		if(com2Frame && _comSelector2){
			_comSelector2->setDisplayFrame(com2Frame);
		}
		if(_comLabel2){
			CCSpriteFrame* confirmedLabel=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("com_label2.png");
			if(confirmedLabel){
				_comLabel2->stopAllActions();
				_comLabel2->setOpacity(255);
				_comLabel2->setDisplayFrame(confirmedLabel);
			}
		}
	}
}

void NetworkLayer::refreshTrainingTeamSlots(){
	CCSpriteFrame* emptyFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("unknow_select.png");
	for(int i=0;i<_trainingTeamSize && i<5;i++){
		if(!_trainingTeamSlots[i]){
			continue;
		}
		if(i==0){
			if(_playerSelect){
				CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",_playerSelect)->getCString());
				if(frame){
					_trainingTeamSlots[i]->setDisplayFrame(frame);
				}
			}else if(emptyFrame){
				_trainingTeamSlots[i]->setDisplayFrame(emptyFrame);
			}
		}else if(_trainingTeamPicks && (int)_trainingTeamPicks->count()>=i){
			CCString* picked=(CCString*)_trainingTeamPicks->objectAtIndex(i-1);
			CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",picked->getCString())->getCString());
			if(frame){
				_trainingTeamSlots[i]->setDisplayFrame(frame);
			}
		}else if(emptyFrame){
			_trainingTeamSlots[i]->setDisplayFrame(emptyFrame);
		}
	}
}

void NetworkLayer::refreshTrainingEnemySlots(){
	CCSpriteFrame* emptyFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("unknow_select.png");
	for(int i=0;i<_trainingTeamSize && i<5;i++){
		if(!_trainingEnemySlots[i]){
			continue;
		}
		if(_trainingEnemyPicks && (int)_trainingEnemyPicks->count()>i){
			CCString* picked=(CCString*)_trainingEnemyPicks->objectAtIndex(i);
			CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",picked->getCString())->getCString());
			if(frame){
				_trainingEnemySlots[i]->setDisplayFrame(frame);
			}
		}else if(emptyFrame){
			_trainingEnemySlots[i]->setDisplayFrame(emptyFrame);
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
		CC_SAFE_RELEASE(_trainingPlayerPick);
		_trainingPlayerPick=NULL;
		if(!_selectImg && selectArray && selectArray->count()>0){
			SelectButton* firstBtn=(SelectButton*)selectArray->objectAtIndex(0);
			_selectImg=CCSprite::createWithSpriteFrameName("1p.png");
			_selectImg->setAnchorPoint(ccp(0,0));
			_selectImg->setPosition(ccp(firstBtn->getPositionX()-2,firstBtn->getPositionY()-2));
			this->addChild(_selectImg,500);
			CCActionInterval* blink=CCBlink::create(0.6f,1);
			_selectImg->runAction(CCRepeatForever::create(blink));
		}
	}else if(_com1Select && strcmp(_com1Select,heroName)==0){
		_com1Select=NULL;
	}else if(_com2Select && strcmp(_com2Select,heroName)==0){
		_com2Select=NULL;
	}else if(_isTrainingSelect && _trainingTeamPicks){
		CCObject* pObject=NULL;
		CCARRAY_FOREACH(_trainingTeamPicks,pObject){
			CCString* value=(CCString*)pObject;
			if(strcmp(value->getCString(),heroName)==0){
				_trainingTeamPicks->removeObject(value);
				break;
			}
		}
	}else if(_isTrainingSelect && _trainingEnemyPicks){
		CCObject* pObject=NULL;
		CCARRAY_FOREACH(_trainingEnemyPicks,pObject){
			CCString* value=(CCString*)pObject;
			if(strcmp(value->getCString(),heroName)==0){
				_trainingEnemyPicks->removeObject(value);
				break;
			}
		}
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
		&& (!_trainingPlayerPick)
		&& (!_trainingTeamPicks || _trainingTeamPicks->count()==0)
		&& (!_trainingEnemyPicks || _trainingEnemyPicks->count()==0)){
		return;
	}

	if(selectArray){
		CCObject* pObject=NULL;
		CCARRAY_FOREACH(selectArray,pObject){
			SelectButton* btn=(SelectButton*)pObject;
			btn->spriteResetGrey();
		}
	}

	_playerSelect=NULL;
	_com1Select=NULL;
	_com2Select=NULL;
	CC_SAFE_RELEASE(_trainingPlayerPick);
	_trainingPlayerPick=NULL;
	if(_trainingTeamPicks){
		_trainingTeamPicks->removeAllObjects();
	}
	if(_trainingEnemyPicks){
		_trainingEnemyPicks->removeAllObjects();
	}

	if(_isTrainingSelect){
		refreshTrainingTeamSlots();
		refreshTrainingEnemySlots();
	}else if(adResult==1){
		refreshTeamSelectorUI();
	}

	if(!_selectImg && selectArray && selectArray->count()>0){
		SelectButton* firstBtn=(SelectButton*)selectArray->objectAtIndex(0);
		_selectImg=CCSprite::createWithSpriteFrameName("1p.png");
		_selectImg->setAnchorPoint(ccp(0,0));
		_selectImg->setPosition(ccp(firstBtn->getPositionX()-2,firstBtn->getPositionY()-2));
		this->addChild(_selectImg,500);
		CCActionInterval* blink=CCBlink::create(0.6f,1);
		_selectImg->runAction(CCRepeatForever::create(blink));
	}

	isStart=false;
}

void NetworkLayer::onUnselectAllBtn(CCObject* sender){
	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/cancel.ogg");
	unselectAll();
}

void NetworkLayer::setSelected(CCObject* sender){
	 
	 SelectButton* btn=(SelectButton*) sender;
	 if(_selectImg){
		_selectImg->setPosition(ccp(btn->getPositionX()-2,btn->getPositionY()-2));
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
		bool selectedTeamFull=_trainingRandomTeam || (_trainingTeamPicks && (int)_trainingTeamPicks->count()>=selectedTeamNeed);
		bool selectedEnemyFull=_trainingRandomEnemy || (_trainingEnemyPicks && (int)_trainingEnemyPicks->count()>=_trainingTeamSize);
		if(_playerSelect && selectedTeamFull && selectedEnemyFull && !isHeroConfirmedSelected(_selectHero)){
			return;
		}

		if(_heroHalf){
			_heroHalf->removeFromParent();
		}
		const char* path=CCString::createWithFormat("%s_half.png",_selectHero)->getCString();
		_heroHalf=CCSprite::createWithSpriteFrameName(path);
		if(_heroHalf){
			_heroHalf->setAnchorPoint(ccp(0,0));
			_heroHalf->setPosition(ccp(10,10));
			this->addChild(_heroHalf,1);
		}

		if(_heroName){
			_heroName->removeFromParent();
		}
		const char* namePath=CCString::createWithFormat("%s_font.png",_selectHero)->getCString();
		_heroName=CCSprite::createWithSpriteFrameName(namePath);
		if(_heroName){
			_heroName->setAnchorPoint(ccp(0.5f,0));
			_heroName->setPosition(ccp(100,20));
			this->addChild(_heroName,5);
		}

		if(btn->_clickTime<2){
			return;
		}

		if(!_playerSelect){
			CC_SAFE_RELEASE(_trainingPlayerPick);
			_trainingPlayerPick=CCString::create(_selectHero);
			_trainingPlayerPick->retain();
			_playerSelect=_trainingPlayerPick->getCString();
			CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",_selectHero)->getCString());
			if(frame && _trainingTeamSlots[0]){
				_trainingTeamSlots[0]->setDisplayFrame(frame);
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
		if(!_trainingRandomTeam && (int)_trainingTeamPicks->count()<teamNeed){
			int slotIndex=_trainingTeamPicks->count()+1;
			_trainingTeamPicks->addObject(CCString::create(_selectHero));
			CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",_selectHero)->getCString());
			if(frame && slotIndex<5 && _trainingTeamSlots[slotIndex]){
				_trainingTeamSlots[slotIndex]->setDisplayFrame(frame);
			}
			return;
		}

		if(!_trainingRandomEnemy && (int)_trainingEnemyPicks->count()<_trainingTeamSize){
			int slotIndex=_trainingEnemyPicks->count();
			_trainingEnemyPicks->addObject(CCString::create(_selectHero));
			CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",_selectHero)->getCString());
			if(frame && slotIndex<5 && _trainingEnemySlots[slotIndex]){
				_trainingEnemySlots[slotIndex]->setDisplayFrame(frame);
			}
			return;
		}

		return;
	}

	if(_isHardCoreMode){

		CCActionInterval*  fd=CCFadeOut::create(1.0f);
		CCAction* seq=CCRepeatForever::create(cocos2d::cocos2dx2_compat::createSequence(fd,fd->reverse(),NULL));

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


			const char* path=CCString::createWithFormat("%s_half.png", charName)->getCString();
			_heroHalf=CCSprite::createWithSpriteFrameName(path);

			_heroHalf->setAnchorPoint(ccp(0,0));
			_heroHalf->setPosition(ccp(10,10));
			this->addChild(_heroHalf,1);

			_heroName->removeFromParent();
			const char* namePath=CCString::createWithFormat("%s_font.png",charName)->getCString();
			_heroName=CCSprite::createWithSpriteFrameName(namePath);
			_heroName->setAnchorPoint(ccp(0.5f,0));
			_heroName->setPosition(ccp(100,20));
			this->addChild(_heroName,5);

			

		}else if(!_com1Select){
			CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",_selectHero)->getCString()); 
			this->getComSelector1()->setDisplayFrame(frame);

			if(btn->_clickTime>=2){
				_com1Select=_selectHero;

				this->getComLabel1()->stopAllActions();
				this->getComLabel1()->setOpacity(255);
				CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("com_label2.png"); 
				this->getComLabel1()->setDisplayFrame(frame);
				this->getComLabel2()->runAction(seq);
			}

			

		}else if(!_com2Select){


			CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_small.png",_selectHero)->getCString()); 
			this->getComSelector2()->setDisplayFrame(frame);

			if(btn->_clickTime>=2){
				_com2Select=_selectHero;

				this->getComLabel2()->stopAllActions();
				this->getComLabel2()->setOpacity(255);
				CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("com_label2.png"); 
				this->getComLabel2()->setDisplayFrame(frame);

				_selectImg->removeFromParent();
				_selectImg=NULL;
			}

			
		}

	}


}



void NetworkLayer::onError(CCObject* sender){

	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CCDirector::sharedDirector()->end();
	return;

}


void NetworkLayer::onSkillMenu(CCObject* sender){
	if(!_selectHero){
		return;
	}
	if(strcmp(_selectHero,"Pain")==0 || strcmp(_selectHero,"Orochimaru")==0 ){
		return;

	}

	CCScene* pscene =CCScene::create();
	SkillLayer* skillLayer=SkillLayer::create();
	skillLayer->setDelegate2(this);
	skillLayer->initInterface();
	pscene->addChild(skillLayer);

	CCDirector::sharedDirector()->pushScene(pscene);

}

void  NetworkLayer::onRankingBtn(CCObject* sender){
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CCScene* pscene =CCScene::create();
	RakingLayer* rakingLayer=RakingLayer::create();
	rakingLayer->setDelegate(this);
	pscene->addChild(rakingLayer);

	CCDirector::sharedDirector()->pushScene(pscene);
}

void  NetworkLayer::onQuestBtn(CCObject* sender){

	CCTips *tip=CCTips::create("ComingSoon");
	this->addChild(tip,5000);
	return;
	
}



void NetworkLayer::onGameStart(CCObject* sender){

	

	if (!isStart){

		KTools* tool= KTools::create();

		


		isStart=true;
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");


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
			CCArray* used=CCArray::create();
			const char* playerName=_trainingPlayerPick ? _trainingPlayerPick->getCString() : (_playerSelect ? _playerSelect : _selectHero);
			if(!playerName || strcmp(playerName,"None")==0){
				playerName="Naruto";
			}
			used->addObject(CCString::create(playerName));
			networkAddHero(tempHeros,playerName,"Player","Konoha");

			for(int i=1;i<teamSize;i++){
				const char* heroName=NULL;
				if(!_trainingRandomTeam && _trainingTeamPicks && (int)_trainingTeamPicks->count()>=i){
					CCString* picked=(CCString*)_trainingTeamPicks->objectAtIndex(i-1);
					heroName=picked->getCString();
				}else{
					heroName=networkPickHero(used,(const char**)heroList,num,teamPreset,sizeof(teamPreset)/sizeof(char*),i-1,_trainingRandomTeam);
				}
				if(!heroName || networkContainsHero(used,heroName)){
					heroName=networkPickHero(used,(const char**)heroList,num,teamPreset,sizeof(teamPreset)/sizeof(char*),i-1,true);
				}else{
					used->addObject(CCString::create(heroName));
				}
				networkAddHero(tempHeros,heroName,"Com","Konoha");
			}

			for(int i=0;i<teamSize;i++){
				const char* heroName=NULL;
				if(!_trainingRandomEnemy && _trainingEnemyPicks && (int)_trainingEnemyPicks->count()>i){
					CCString* picked=(CCString*)_trainingEnemyPicks->objectAtIndex(i);
					heroName=picked->getCString();
				}else{
					heroName=networkPickHero(used,(const char**)heroList,num,enemyPreset,sizeof(enemyPreset)/sizeof(char*),i,_trainingRandomEnemy);
				}
				if(!heroName || networkContainsHero(used,heroName)){
					heroName=networkPickHero(used,(const char**)heroList,num,enemyPreset,sizeof(enemyPreset)/sizeof(char*),i,true);
				}else{
					used->addObject(CCString::create(heroName));
				}
				networkAddHero(tempHeros,heroName,"Com","Akatsuki");
			}

			CCScene* loadScene=CCScene::create();
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
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f,loadScene));
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

					CCObject* tempObject=realHero->objectAtIndex(index);
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

				CCObject* tempObject=realHero->objectAtIndex(index);
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



		CCScene* loadScene=CCScene::create();
		LoadLayer* loadLayer=LoadLayer::create();
		loadLayer->tempHeros=tempHeros;
		loadScene->addChild(loadLayer);
		
		loadLayer->_isHardCoreMode=_isHardCoreMode;
		loadLayer->_isRandomChar=_isRandomChar;
		loadLayer->_selectedMap=_selectedMap;
		loadLayer->preloadAudio();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f,loadScene));
	

	}
}


void NetworkLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		onBackKeyClicked();
	}
}

void NetworkLayer::onBackKeyClicked(){
	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/cancel.ogg");	
	CCScene* menuScene =CCScene::create();
	CCLayer* menuLayer=StartMenu::create();
	menuScene->addChild(menuLayer);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f,menuScene));

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






