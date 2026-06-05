#include "GameLayer.h"
#include <cstdlib>
#include <string>
#include "Characters.h"
#include "BGLayer.h"
#include "HudLayer.h"
#include "HeroPayload.h"
#include "StartMenu.h"
#include "LoadLayer.h"
#include "MyUtils/KTools.h"

int CError=0;

using namespace CocosDenshion;

namespace {

__Array* createGameplayArray()
{
	return __Array::create();
}

}

GameLayer::GameLayer(void)
{
	_isAttackButtonRelease=true;
	_isSkillFinish=true;

	_KonohaFlogArray=NULL;
	_AkatsukiFlogArray=NULL;
	_CharacterArray=NULL;

	_second=0;
	_minute=0;
	_playNum=2;

	kEXPBound=25;
	aEXPBound=25;

	totalKills=0;
	totalTM=0;
	_isShacking=false;
	_isSurrender=false;
	_isGuardian=false;
	_isExiting=false;
	ougisChar=NULL;
	controlChar=NULL;
	_isOugis2Game=false;
	_isHardCoreGame=false;
	_isRandomChar=false;
	_isDuelMode=false;
	_isUnlimitedCKR=false;
	_isDuelFinished=false;
	_isRematching=false;
	_rematchHerosOverride=NULL;
	_duelKillLimit=3;
	_duelPlayerScore=0;
	_duelEnemyScore=0;
	selectedMap=-1;
	zhenying=1;
	currentPlayer=NULL;
	isPosting=false;
	postTime=0;
	_hudLayer = NULL;

	_TowerArray = NULL;
	bulletBatch = NULL;
	blend = NULL;

	_isAttackButtonRelease = true;
	_isSkillFinish = true;
}


GameLayer::~GameLayer(void)
{
	CC_SAFE_RELEASE(_rematchHerosOverride);

}

bool GameLayer::init(){
	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());
		//Texture2D::setDefaultAlphaPixelFormat(kTexture2DPixelFormat_RGBA4444);
		KeyList.push_back("8396c,4a035");
		KeyList.push_back("8384b,880d8");
		KeyList.push_back("f2e4e,4203d");
		KeyList.push_back("452eb,8a5ca");
		KeyList.push_back("ef77f,226b2");
		KeyList.push_back("3c67e,65275");
		KeyList.push_back("194fa,ed62c");
		KeyList.push_back("738e3,3dcdd");
		KeyList.push_back("1004b,fab1a");
		KeyList.push_back("6eee4,724c5");
		KeyList.push_back("c7475,b59d9");
		KeyList.push_back("54339,e85f7");
		KeyList.push_back("a0959,7364e");
		KeyList.push_back("57dca,0f3bd");
		KeyList.push_back("4cc9b,11374");
		KeyList.push_back("d0705,f5120");
		KeyList.push_back("59c42,32637");
		KeyList.push_back("ba442,6aa6d");
		KeyList.push_back("a80ce,75ddf");
		KeyList.push_back("5c458,87e85");
		KeyList.push_back("2bd55,60e5f");
		KeyList.push_back("10d9b,ed7b0");
		KeyList.push_back("893f9,1bd0a");
		KeyList.push_back("0723f,ee621");
		KeyList.push_back("cd67d,20fa5");
		KeyList.push_back("e1b9e,0e13b");
		KeyList.push_back("898df,7a17b");
		KeyList.push_back("8f941,db312");
		KeyList.push_back("6ef58,5e73b");
		KeyList.push_back("8b4aa,da21a");
		KeyList.push_back("5b3b4,f26c1");
		KeyList.push_back("db7d1,25d60");
		KeyList.push_back("37a08,c9b59");
		KeyList.push_back("525c0,f4cad");
		KeyList.push_back("58519,d99f8");
		KeyList.push_back("af689,34cb2");
		KeyList.push_back("7a8b4,efa9a");
		KeyList.push_back("68e38,c9a65");
		KeyList.push_back("c04d9,0caa7");
		KeyList.push_back("10f02,d5e1c");
		KeyList.push_back("24b7c,8a25a");
		KeyList.push_back("2029e,01629");
		KeyList.push_back("8d7d6,3ff38");
		KeyList.push_back("80a78,cac62");
		KeyList.push_back("2105b,32445");
		KeyList.push_back("ea981,0ea5f");
		KeyList.push_back("33ca1,ae63e");
		KeyList.push_back("c8687,fde6d");
		KeyList.push_back("fd008,faf10");
		KeyList.push_back("38811,e1370");
		KeyList.push_back("65459,2d764");
		KeyList.push_back("a7bb2,5db37");
		KeyList.push_back("4267f,f0025");
		KeyList.push_back("ef094,5d08d");
		KeyList.push_back("36a34,be5fc");
		KeyList.push_back("d38d6,eccc8");
		KeyList.push_back("923b1,4cc85");
		KeyList.push_back("21a58,4d524");
		KeyList.push_back("81cbc,7505c");
		KeyList.push_back("b1b3d,0f5e4");
		KeyList.push_back("222d6,fc46c");
		KeyList.push_back("b763e,d5996");
		KeyList.push_back("6635b,83b88");
		KeyList.push_back("a207f,80adc");
		KeyList.push_back("c9926,a34ca");
		KeyList.push_back("ea652,e776e");
		KeyList.push_back("61196,54b94");
		KeyList.push_back("f528a,1bd6c");
		KeyList.push_back("29f69,4b537");
		KeyList.push_back("fb6d8,bf1ee");
		KeyList.push_back("e88e3,44dbd");
		KeyList.push_back("ef895,53aae");
		KeyList.push_back("0a27e,3030e");
		KeyList.push_back("b77aa,002f0");
		KeyList.push_back("db99f,9e4f6");
		KeyList.push_back("53e6f,fdeaa");
		KeyList.push_back("6252d,b3c23");
		KeyList.push_back("35807,0b2e8");
		KeyList.push_back("bf35c,a2e06");
		KeyList.push_back("da1bd,2927e");
		KeyList.push_back("575d5,cd186");
		KeyList.push_back("75978,df505");
		KeyList.push_back("ebf33,01ba2");
		KeyList.push_back("2fd64,20d11");
		KeyList.push_back("f23e5,c7c03");
		KeyList.push_back("6979f,fe257");
		KeyList.push_back("15c08,f4ec9");
		KeyList.push_back("10d49,580e7");
		KeyList.push_back("7367c,111e7");
		KeyList.push_back("72a0f,d06fb");
		KeyList.push_back("e0ef6,20d2f");
		KeyList.push_back("bb21b,54211");
		KeyList.push_back("b1871,f58bc");
		KeyList.push_back("c47a7,05b13");
		KeyList.push_back("ebb56,cacca");
		KeyList.push_back("bdf5e,46ab8");
		KeyList.push_back("c8268,a3f47");
		KeyList.push_back("7528e,7ef23");
		KeyList.push_back("30779,4166b");
		KeyList.push_back("b6e64,bb46b");
		KeyList.push_back("51de9,7c3dc");
		KeyList.push_back("a5c50,10757");
		KeyList.push_back("8240d,edd82");
		KeyList.push_back("22608,7cdd1");
		KeyList.push_back("0e676,8361b");
		KeyList.push_back("e3d38,ec408");
		KeyList.push_back("f7e12,7c28b");
		KeyList.push_back("bb902,0ddf9");
		KeyList.push_back("b0a43,5dbc7");
		KeyList.push_back("e9388,6eca3");
		KeyList.push_back("2e91a,cca0b");
		KeyList.push_back("f48a5,337e3");
		KeyList.push_back("c10e3,d4d06");
		KeyList.push_back("4bc9d,6975b");
		KeyList.push_back("7a0a1,8bc38");
		KeyList.push_back("377ec,f2a3f");
		KeyList.push_back("5f3ff,d5a32");
		KeyList.push_back("a060f,67454");
		KeyList.push_back("2678e,ff54c");
		KeyList.push_back("2c9b9,c8ab4");
		KeyList.push_back("610b8,1e7e1");
		KeyList.push_back("db1b5,b54cc");
		KeyList.push_back("6d4d7,78d7c");
		KeyList.push_back("7b1de,d49a7");
		KeyList.push_back("69ba7,7fd27");
		KeyList.push_back("cdab3,ec968");
		KeyList.push_back("0fada,93b09");
		KeyList.push_back("e5f1c,15d01");
		KeyList.push_back("5c27d,b2fbb");
		KeyList.push_back("54fdd,38ad7");
		KeyList.push_back("4ab00,d766f");
		KeyList.push_back("009eb,8eddc");
		KeyList.push_back("de13f,8d841");
		KeyList.push_back("78f66,170f0");
		KeyList.push_back("98283,b7567");
		KeyList.push_back("e467c,37ae8");
		KeyList.push_back("53ce3,99f0f");
		KeyList.push_back("89707,7ed31");
		KeyList.push_back("61ffa,be0bd");
		KeyList.push_back("67eb9,38789");
		KeyList.push_back("51664,7bd60");
		KeyList.push_back("2fdd9,d5807");
		KeyList.push_back("3bbe7,ae702");
		KeyList.push_back("407e9,c2b00");
		KeyList.push_back("e815f,22c2e");
		KeyList.push_back("5280f,69111");
		KeyList.push_back("13b5b,74c91");
		KeyList.push_back("49f61,5c2f9");

		this->setTouchEnabled(true);

		bRet=true;

	}while(0);


	return bRet;



}
void GameLayer::onEnter(){
	if(_isExiting){
		this->onLeft();
		return;
	}

	if(currentPlayer && !ougisChar){
		if(currentPlayer->getActionState()==ACTION_STATE_WALK){
			currentPlayer->idle();
		}
	}

	Layer::onEnter();

	if(_isSurrender){
		this->onGameOver(false);
	}

};
void GameLayer::onExit(){
	Layer::onExit();
	if(_isExiting){
		_isExiting=false;
		CC_SAFE_RELEASE(_TowerArray);
		CC_SAFE_RELEASE(_KonohaFlogArray);
		CC_SAFE_RELEASE(_AkatsukiFlogArray);
		CC_SAFE_RELEASE(_CharacterArray);

	}


};



void GameLayer::initTileMap(){
	//�����ͼ


	if(selectedMap>=0 && selectedMap<=4){
		randomMap=selectedMap;
	}else if(_isHardCoreGame){
		srand((int)time(0));
		randomMap=random(4)+1;
	}else {
		randomMap=0;
	}

	const char* filePath;
	if (randomMap==0){
		filePath=S_MAP01;
	}else if(randomMap==1){
		filePath=S_MAP02;
	}else if(randomMap==2){
		filePath=S_MAP03;
	}else if(randomMap==3){
		filePath=S_MAP04;
	}else if(randomMap==4){
		filePath=S_MAP05;
	}

	KTools* tool= KTools::create();

	std::string key =tool->getKeycode(filePath);
	int id=tool->checkMD5(filePath);
//	if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
//		if(strcmp(key.c_str(),KeyList[id])!=0){
//			return;
//		}
//	}

	currentMap=TMXTiledMap::create(filePath);

	this->addChild(currentMap,currentMapTag);

}

void GameLayer::initGard(){



	int index=0;
	srand((int)time(0));
	index=random(2);
	Hero* Guardian=Hero::create();
	Guardian->setDelegate(this);

	const char* groupName;

	if(zhenying>0){
		groupName="Akatsuki";
	}else{
		groupName="Konoha";
	}
	if(index==0){
		NarutoSenkiPayload::setId(Guardian,"Roshi","Com",groupName);
	}else if(index==1){
		NarutoSenkiPayload::setId(Guardian,"Han","Com",groupName);
	}


	if(zhenying>0){
	Guardian->setPosition(Vec2(2800,80));
	Guardian->setSpawnPoint(Vec2(2800,80));
	}else{
		Guardian->setPosition(Vec2(272,80));
		Guardian->setSpawnPoint(Vec2(272,80));
	}



	this->addChild(Guardian,-Guardian->getPositionY());
	Guardian->setLV(6);
	Guardian->setHPbar();
	Guardian->setShadows();
	Guardian->setCharNO(7);

	Guardian->idle();
	auto callValue=NarutoSenkiPayload::createStringPayload("smk");
	Guardian->setSkillEffect(Guardian,callValue);

	Guardian->doAI();




	_CharacterArray->addObject(Guardian);
	_hudLayer->addMapIcon();
	_isGuardian=true;

}

void GameLayer::initHeros(){

	this->initTileMap();
	this->initEffects();

	_CharacterArray=createGameplayArray();
	if(_isDuelMode){
		_KonohaFlogArray=createGameplayArray();
		_KonohaFlogArray->retain();
		_AkatsukiFlogArray=createGameplayArray();
		_AkatsukiFlogArray->retain();
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/hpBar/hpBar.plist");
	Ref* pObject=NULL;
	int i=0;


	TMXObjectGroup* group = currentMap->objectGroupNamed("object");
	const auto& objectArray = group->getObjects();

	_isOugis2Game=true;
	//4v4

	if(Heros->count()>6 || Cheats>10){

		const int konohaSpawnMap[]={0,1,2,6,7};
		const int akatsukiSpawnMap[]={3,5,4,8,9};
		int konohaSpawnIndex=0;
		int akatsukiSpawnIndex=0;

		for(unsigned int heroIndex=0; heroIndex<Heros->count(); ++heroIndex){
			pObject=Heros->objectAtIndex(heroIndex);

			const char* group=NarutoSenkiPayload::group(pObject);

			int mapPos;
			if(strcmp(group,"Akatsuki")==0){
				int spawnIndex=akatsukiSpawnIndex++;
				if(spawnIndex>4){
					spawnIndex=4;
				}
				mapPos=akatsukiSpawnMap[spawnIndex];
			}else{
				int spawnIndex=konohaSpawnIndex++;
				if(spawnIndex>4){
					spawnIndex=4;
				}
				mapPos=konohaSpawnMap[spawnIndex];
			}

				const auto& mapdict = objectArray.at(mapPos).asValueMap();
				int x=mapdict.at("x").asInt();
				int y=mapdict.at("y").asInt();
			spawnPoint=Vec2(x,y);

			if(i==0){

				if(strcmp(group,"Akatsuki")==0){
					zhenying=0;
				}

				currentPlayer=Hero::create();
				currentPlayer->setDelegate(this);
				NarutoSenkiPayload::setHeroId(currentPlayer,pObject);
				currentPlayer->setPosition(spawnPoint);
				currentPlayer->setSpawnPoint(spawnPoint);
				this->addChild(currentPlayer,-currentPlayer->getPositionY());
				currentPlayer->setHPbar();
				currentPlayer->setShadows();
				currentPlayer->idle();
				if(zhenying<=0){
					currentPlayer->_isFlipped=true;
					currentPlayer->setFlipX(true);
				}
				currentPlayer->setCharNO(i+1);
				currentPlayer->schedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setRestore2),1.0f);
				_CharacterArray->addObject(currentPlayer);

			}else{

				Hero* Com=Hero::create();
				Com->setDelegate(this);
				NarutoSenkiPayload::setHeroId(Com,pObject);
				Com->setPosition(spawnPoint);
				Com->setSpawnPoint(spawnPoint);
				this->addChild(Com,-Com->getPositionY());
				Com->setHPbar();
				Com->setShadows();
				Com->idle();
				if(strcmp(Com->getGroup()->getCString(),"Akatsuki")==0){
					Com->_isFlipped=true;
					Com->setFlipX(true);
				}
				Com->setCharNO(i+1);
				Com->schedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setRestore2),1.0f);
				_CharacterArray->addObject(Com);
			}
			i++;
		};

	}else{
		bool useCenterSpawn=(_isDuelMode || _isTrainingMode) && Heros->count()<=2;
		Size mapSize=currentMap->getMapSize();
		Size tileSize=currentMap->getTileSize();
		float mapCenterX=mapSize.width*tileSize.width*0.5f;
		const float centerSpawnY=108.0f;
		const float centerSpawnOffsetX=160.0f;

		for(unsigned int heroIndex=0; heroIndex<Heros->count(); ++heroIndex){
			pObject=Heros->objectAtIndex(heroIndex);

			const char* group=NarutoSenkiPayload::group(pObject);

			if(useCenterSpawn){
				if(strcmp(group,"Akatsuki")==0){
					spawnPoint=Vec2(mapCenterX+centerSpawnOffsetX,centerSpawnY);
				}else{
					spawnPoint=Vec2(mapCenterX-centerSpawnOffsetX,centerSpawnY);
				}
			}else{
				int mapPos=i;
				if(strcmp(group,"Akatsuki")==0){
					if(mapPos<=2){
						mapPos+=3;
					}

				}else{
					if(mapPos>2){
						mapPos-=3;
					}
				}

				const auto& mapdict = objectArray.at(mapPos).asValueMap();
				int x=mapdict.at("x").asInt();
				int y=mapdict.at("y").asInt();
				spawnPoint=Vec2(x,y);
			}



			if(i==0){

				if(strcmp(group,"Akatsuki")==0){
					zhenying=0;
				}

				currentPlayer=Hero::create();
				currentPlayer->setDelegate(this);
				NarutoSenkiPayload::setHeroId(currentPlayer,pObject);
				currentPlayer->setPosition(spawnPoint);
				currentPlayer->setSpawnPoint(spawnPoint);
				this->addChild(currentPlayer,-currentPlayer->getPositionY());
				currentPlayer->setHPbar();
				currentPlayer->setShadows();
				currentPlayer->idle();
				if(zhenying<=0){
					currentPlayer->_isFlipped=true;
					currentPlayer->setFlipX(true);
				}
				currentPlayer->setCharNO(i+1);
				currentPlayer->schedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setRestore2),1.0f);
				_CharacterArray->addObject(currentPlayer);

			}else{

				Hero* Com=Hero::create();
				Com->setDelegate(this);
				NarutoSenkiPayload::setHeroId(Com,pObject);
				Com->setPosition(spawnPoint);
				Com->setSpawnPoint(spawnPoint);
				this->addChild(Com,-Com->getPositionY());
				Com->setHPbar();
				Com->setShadows();
				Com->idle();
				if(strcmp(Com->getGroup()->getCString(),"Akatsuki")==0){
					Com->_isFlipped=true;
					Com->setFlipX(true);
				}

				Com->setCharNO(i+1);
				Com->schedule(CC_SCHEDULE_SELECTOR(NarutoSenki::ActionManager::setRestore2),1.0f);
				_CharacterArray->addObject(Com);
			}
			i++;
		};


	}


	_CharacterArray->retain();

	this->initTower();



	this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::updateViewPoint),0.0f);




	/*if(strcmp(currentPlayer->getCharacter()->getCString(),"Asuma")==0){
		this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::checkPost),10.0f);
	}*/




	this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameLayer::onKaichang),0.5f);


};

void GameLayer::onKaichang(float dt){
	this->getHudLayer()->onKaichang();

	if(_isHardCoreGame){
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/battle_start1.ogg");
	}else{
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/battle_start.ogg");
	}


	this->scheduleOnce(CC_SCHEDULE_SELECTOR(GameLayer::onGameStart),0.75f);

}

void GameLayer::onGameStart(float dt){
	this->getHudLayer()->Kaichang->removeFromParent();
	this->getHudLayer()->Kaichang=NULL;
	this->getHudLayer()->initHeroInterface();
	this->applyDuelTrainStartSetup();
	if((_isDuelMode || _isTrainingMode) && currentPlayer){
		currentPlayer->setCoin(10000);
		if(_hudLayer && _hudLayer->coinLabel){
			_hudLayer->coinLabel->setString("10000");
		}
	}
	this->refreshOugisSkillLocks();
	this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::updateGameTime),1.0f);
	this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::checkBackgroundMusic),2.0f);
	if(!_isTrainingMode && !_isDuelMode){
		this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::addFlog),15.0f);
		this->initFlogs();
	}

	Ref* pObject;
	for(unsigned int characterIndex=0; characterIndex<this->_CharacterArray->count(); ++characterIndex){
		pObject=this->_CharacterArray->objectAtIndex(characterIndex);
		NarutoSenki::ActionManager* tempChar=(NarutoSenki::ActionManager*) pObject;
		if(strcmp(tempChar->getRole()->getCString(),"Com")==0 && !_isTrainingMode){
			tempChar->doAI();
		}

	}

}

void GameLayer::initFlogs(){

	const char* KonohaFlogName="KotetsuFlog";
	const char* AkatsukiFlogName="FemalePainFlog";

	kName="KotetsuFlog";
	aName="FemalePainFlog";

	_KonohaFlogArray=createGameplayArray();
	_AkatsukiFlogArray=createGameplayArray();


	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/hpBar/flogBar.plist");

	int i;
	Flog* aiFlog;
	float mainPosY;
	for(i= 0;i<6;i++){
		aiFlog=Flog::create();
		aiFlog->setDelegate(this);
		NarutoSenkiPayload::setId(aiFlog,KonohaFlogName,"Flog","Konoha");
		if(i<3){
			mainPosY=(5.5-i/1.5)*32;

		}else{
			mainPosY=(3.5-i/1.5)*32;
		}
		aiFlog->_mainPosY= mainPosY;
		aiFlog->setPosition(Vec2(13*32,aiFlog->_mainPosY));
		aiFlog->setHPbar();
		this->addChild(aiFlog,-int(aiFlog->getPositionY()));
		aiFlog->idle();
		aiFlog->doAI();
		_KonohaFlogArray->addObject(aiFlog);

	}

	_KonohaFlogArray->retain();

	for(i= 0;i<6;i++){
		aiFlog=Flog::create();
		aiFlog->setDelegate(this);
		NarutoSenkiPayload::setId(aiFlog,AkatsukiFlogName,"Flog","Akatsuki");
		if(i<3){
			mainPosY=(5.5-i/1.5)*32 ;

		}else{
			mainPosY=(3.5-i/1.5)*32;
		}
		aiFlog->_mainPosY= mainPosY;
		aiFlog->setPosition(Vec2(83*32,aiFlog->_mainPosY));
		aiFlog->setHPbar();
		aiFlog->idle();
		aiFlog->doAI();
		_AkatsukiFlogArray->addObject(aiFlog);
		this->addChild(aiFlog,-aiFlog->getPositionY());

	}

	_AkatsukiFlogArray->retain();

};
void GameLayer::addFlog(float dt){

	const char* KonohaFlogName=kName;
	const char* AkatsukiFlogName=aName;


	int i;
	Flog* aiFlog;
	float mainPosY;
	for(i= 0;i<6;i++){
		aiFlog=Flog::create();
		aiFlog->setDelegate(this);
		NarutoSenkiPayload::setId(aiFlog,KonohaFlogName,"Flog","Konoha");
		if(i<3){
			mainPosY=(5.5-i/1.5)*32;

		}else{
			mainPosY=(3.5-i/1.5)*32;
		}
		aiFlog->_mainPosY= mainPosY;
		aiFlog->setPosition(Vec2(13*32,aiFlog->_mainPosY));
		aiFlog->setHPbar();
		aiFlog->idle();
		aiFlog->doAI();
		_KonohaFlogArray->addObject(aiFlog);
		this->addChild(aiFlog,-int(aiFlog->getPositionY()));
	}

	for(i= 0;i<6;i++){
		aiFlog=Flog::create();
		aiFlog->setDelegate(this);
		NarutoSenkiPayload::setId(aiFlog,AkatsukiFlogName,"Flog","Akatsuki");
		if(i<3){
			mainPosY=(5.5-i/1.5)*32 ;

		}else{
			mainPosY=(3.5-i/1.5)*32;
		}
		aiFlog->_mainPosY= mainPosY;
		aiFlog->setPosition(Vec2(83*32,aiFlog->_mainPosY));
		aiFlog->setHPbar();
		aiFlog->idle();
		aiFlog->doAI();
		_AkatsukiFlogArray->addObject(aiFlog);
		this->addChild(aiFlog,-aiFlog->getPositionY());

	}

}



void GameLayer::initTower(){

	if(_isDuelMode){
		_TowerArray=createGameplayArray();
		_TowerArray->retain();
		return;
	}

	if (randomMap==0) {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Tower/Tower.plist");
	}else if(randomMap==1){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Tower/Tower2.plist");
	}else if(randomMap==2){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Tower/Tower3.plist");
	}else if(randomMap==3){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Tower/Tower4.plist");
	}else if(randomMap==4){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Tower/Tower5.plist");
	}



	TMXObjectGroup* metaGroup = currentMap->objectGroupNamed("meta");
	const auto& metaArray = metaGroup->getObjects();

	_TowerArray=createGameplayArray();

	int i=0;

	for (const auto& metaValue : metaArray){

		const auto& dict = metaValue.asValueMap();
		int metaX=dict.at("x").asInt();
		int metaY=dict.at("y").asInt();
		int metaWidth=dict.at("width").asInt();
		int metaHeight=dict.at("height").asInt();
		std::string name=dict.at("name").asString();


		Tower* tower=Tower::create();
		tower->setDelegate(this);
		char towerName[7]="abcdef";
		strncpy(towerName,name.c_str(),6);
		if(strcmp(towerName,"Konoha")==0){
			NarutoSenkiPayload::setId(tower,name.c_str(),"Tower","Konoha");
		}else{
			NarutoSenkiPayload::setId(tower,name.c_str(),"Tower","Akatsuki");
			tower->setFlipX(true);
			tower->_isFlipped=true;
		}
		float posX=metaX+metaWidth/2;
		float posY=metaY+metaHeight/2;
		tower->setPosition(Vec2(posX,posY));
		tower->setSpawnPoint(Vec2(posX,posY));
		tower->setCharNO(i+1);

		if(i==1 || i==4 ){
			//4v4
			if(Cheats>10){
				NarutoSenkiPayload::setMaxHP(tower,"80000");
			}else{
				NarutoSenkiPayload::setMaxHP(tower,"50000");
			}
			NarutoSenkiPayload::setHP(tower,tower->getMaxHP()->getCString());
		}
		tower->setHPbar();
		tower->_hpBar->setVisible(false);
		tower->idle();
		this->addChild(tower,-tower->getPositionY());

		_TowerArray->addObject(tower);
		_TowerArray->retain();

		i++;
	}

};

void GameLayer::initEffects(){


	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effects/SkillEffect.plist");
	skillEffectBatch=SpriteBatchNode::create("Effects/SkillEffect.png");
	this->addChild(skillEffectBatch,currentSkillTag);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effects/DamageEffect.plist");
	damageEffectBatch=SpriteBatchNode::create("Effects/DamageEffect.png");
	this->addChild(damageEffectBatch,currentDamageTag);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effects/Shadows.plist");
	shadowBatch=SpriteBatchNode::create("Effects/Shadows.png");
	this->addChild(shadowBatch,currentShadowTag);

};

void GameLayer::updateGameTime(float dt)
{
	_second+=1;
	if(_second==60){
		_minute+=1;
		_second=0;
	}
	std::string tempTime=cocos2d::StringUtils::format("%02d:%02d",_minute,_second);
	this->_hudLayer->gameClock->setString(tempTime);

	this->setTotalTM(this->getTotalTM()+1);
}


void GameLayer::updateViewPoint(float dt){



	if(!currentPlayer) return;
	Vec2 playerPoint;
	if(ougisChar){
		playerPoint=ougisChar->getPosition();
	}else if(controlChar){
		playerPoint=controlChar->getPosition();
	}else{
		playerPoint=currentPlayer->getPosition();
	}

	int x=MAX(playerPoint.x,winSize.width/2);
	int y=MAX(playerPoint.y,winSize.width/2);
	x=MIN(x,(currentMap->getMapSize().width*currentMap->getTileSize().width)-winSize.width/2);
	y=MIN(y,(currentMap->getMapSize().height*currentMap->getTileSize().height)-winSize.height/2);

	Vec2 actualPoint=Vec2(x,y);
	Vec2 centerPoint=Vec2(winSize.width/2,y);
	Vec2 viewPoint=(centerPoint - actualPoint);

	this->setPosition(viewPoint);
	//Director::getInstance()->getScheduler()->setTimeScale(1.0f);

};

void GameLayer::setHPLose(float percent){
	this->_hudLayer->setHPLose(percent);
}
void GameLayer::setCKRLose(bool isCRK2){
	this->_hudLayer->setCKRLose(isCRK2);
}



void GameLayer::setReport(const char* name1, const char* name2,const char* killNum){
	this->_hudLayer->setReport(name1,name2,killNum);
}

bool GameLayer::onDuelKill(NarutoSenki::ActionManager* killer, NarutoSenki::ActionManager* victim){
	if(!_isDuelMode || _isDuelFinished || !killer || !victim || !currentPlayer){
		return false;
	}
	if(strcmp(killer->getGroup()->getCString(),currentPlayer->getGroup()->getCString())==0){
		_duelPlayerScore++;
	}else{
		_duelEnemyScore++;
	}
	if(_duelKillLimit<1){
		_duelKillLimit=3;
	}
	if(_duelPlayerScore>=_duelKillLimit || _duelEnemyScore>=_duelKillLimit){
		_isDuelFinished=true;
		this->onGameOver(_duelPlayerScore>=_duelKillLimit);
	}
	return true;
}


void GameLayer::resetStatusBar(){
	this->_hudLayer->status_hpbar->setRotation(0);

}

void GameLayer::setCoin(const char* value){
	this->_hudLayer->setCoin(value);

}
void GameLayer::removeOugisMark(int type){
	CCLOG("removeOugisMark()");
	CCLOG("_hudLayer=%p", _hudLayer);

	if (!_hudLayer)
	{
		CCLOG("HUD NULL");
		return;
	}
	if(type==1){
		if(this->_hudLayer->skill4Button){
			if(this->_hudLayer->skill4Button->lockLabel1){
				this->_hudLayer->skill4Button->lockLabel1->removeFromParent();
				this->_hudLayer->skill4Button->lockLabel1=NULL;
			}
		}

	}else{

		if(this->_hudLayer->skill5Button){
			if(this->_hudLayer->skill5Button->lockLabel1){
				this->_hudLayer->skill5Button->lockLabel1->removeFromParent();
				this->_hudLayer->skill5Button->lockLabel1=NULL;
			}
		}
	}
}

bool GameLayer::shouldUnlockAllOugis() const{
	return _isDuelMode || _isTrainingMode;
}

void GameLayer::fillCKRToMax(NarutoSenki::ActionManager* hero){
	if(!hero){
		return;
	}
	NarutoSenkiPayload::setCKR(hero,"45000");
	NarutoSenkiPayload::setCKR2(hero,"50000");
	hero->_isCanOugis1=true;
	hero->_isCanOugis2=true;
}

void GameLayer::applyDuelTrainStartSetup(){
	if(!_isDuelMode && !_isTrainingMode){
		_isUnlimitedCKR=false;
		return;
	}
	_isUnlimitedCKR=_isTrainingMode;

	if(!_CharacterArray){
		return;
	}

	Ref* pObject=NULL;
	for(unsigned int characterIndex=0; characterIndex<_CharacterArray->count(); ++characterIndex){
	pObject=_CharacterArray->objectAtIndex(characterIndex);
		Hero* hero=(Hero*)pObject;
		if(!hero){
			continue;
		}
		if(strcmp(hero->getRole()->getCString(),"Player")!=0 &&
			strcmp(hero->getRole()->getCString(),"Com")!=0){
			continue;
		}
		hero->applyStartLevel6();
		this->fillCKRToMax(hero);
	}

	if(currentPlayer && _hudLayer){
		_hudLayer->setHPLose(currentPlayer->getHpPercent());
		_hudLayer->setEXPLose(NULL);
		_hudLayer->setCKRLose(false);
		_hudLayer->setCKRLose(true);
	}

	if(_isDuelMode && _hudLayer && currentPlayer){
		for(unsigned int characterIndex=0; characterIndex<_CharacterArray->count(); ++characterIndex){
	pObject=_CharacterArray->objectAtIndex(characterIndex);
			NarutoSenki::ActionManager* hero=(NarutoSenki::ActionManager*)pObject;
			if(strcmp(hero->getGroup()->getCString(),currentPlayer->getGroup()->getCString())!=0 &&
				(strcmp(hero->getRole()->getCString(),"Player")==0 || strcmp(hero->getRole()->getCString(),"Com")==0)){
				_hudLayer->setEnemyHPLose(hero->getHpPercent());
				break;
			}
		}
	}
}

void GameLayer::refreshOugisSkillLocks(){
	if(!_hudLayer || !_CharacterArray){
		return;
	}

	if(shouldUnlockAllOugis()){
		this->removeOugisMark(1);
		this->removeOugisMark(2);
		if(_hudLayer->skill4Button){
			if(_hudLayer->skill4Button->ougismarkSprite){
				_hudLayer->skill4Button->ougismarkSprite->setVisible(false);
			}
			_hudLayer->skill4Button->unLock();
			if(!_hudLayer->skill4Button->clipper){
				_hudLayer->skill4Button->setProgressMark();
			}
		}
		if(_hudLayer->skill5Button){
			if(_hudLayer->skill5Button->ougismarkSprite){
				_hudLayer->skill5Button->ougismarkSprite->setVisible(false);
			}
			_hudLayer->skill5Button->unLock();
			if(!_hudLayer->skill5Button->clipper){
				_hudLayer->skill5Button->setProgressMark();
			}
		}
		return;
	}

	unsigned int maxLevel=1;
	Ref* pObject=NULL;
	for(unsigned int characterIndex=0; characterIndex<_CharacterArray->count(); ++characterIndex){
	pObject=_CharacterArray->objectAtIndex(characterIndex);
		NarutoSenki::ActionManager* hero=(NarutoSenki::ActionManager*)pObject;
		if(strcmp(hero->getRole()->getCString(),"Player")==0 ||
			strcmp(hero->getRole()->getCString(),"Com")==0){
			if(hero->getLV()>maxLevel){
				maxLevel=hero->getLV();
			}
		}
	}

	if(maxLevel>=2){
		this->removeOugisMark(1);
		if(_hudLayer->skill4Button){
			_hudLayer->skill4Button->unLock();
		}
	}
	if(maxLevel>=4){
		this->removeOugisMark(2);
		if(_hudLayer->skill5Button){
			_hudLayer->skill5Button->unLock();
		}
	}
}

void GameLayer::checkTower(){

	int konohaTowerCount=0;
	int akatsukiTowerCount=0;
	Ref* pObject;

	for(unsigned int towerIndex=0; towerIndex<_TowerArray->count(); ++towerIndex){
	pObject=_TowerArray->objectAtIndex(towerIndex);
		Tower* tower=(Tower*) pObject;
		if(strcmp(tower->getGroup()->getCString(),"Konoha")==0){
			konohaTowerCount++;
		}else{
			akatsukiTowerCount++;
		}
	}

	if(konohaTowerCount==2){
		aName="PainFlog";
		kEXPBound=50;
	}else if(konohaTowerCount==1){
		aName="ObitoFlog";
		kEXPBound=100;
	}

	if(akatsukiTowerCount==2){
		kName="IzumoFlog";
		aEXPBound=50;
	}else if(akatsukiTowerCount==1){
		kName="KakashiFlog";
		aEXPBound=100;
	}

	if(postTime>=9){
		currentPlayer=NULL;
	}



	//֪ͨս������
	for(unsigned int characterIndex=0; characterIndex<_CharacterArray->count(); ++characterIndex){
	pObject=_CharacterArray->objectAtIndex(characterIndex);
		NarutoSenki::ActionManager* tmpHero=(NarutoSenki::ActionManager*) pObject;

		if(strcmp(tmpHero->getRole()->getCString(),"Com")!=0){
			continue;
		}

		if(strcmp(tmpHero->getGroup()->getCString(),"Konoha")==0){
			tmpHero->battleCondiction=konohaTowerCount-akatsukiTowerCount;
			if(konohaTowerCount==1){
				tmpHero->isBaseDanger=true;
			}

		}else{

			tmpHero->battleCondiction=akatsukiTowerCount-konohaTowerCount;
			if(_isHardCoreGame){
				if(akatsukiTowerCount==1){
					tmpHero->isBaseDanger=true;
				}
			}
		}


	}

	std::string bag="";
	KTools* tool=KTools::create();
	std::string str="rix2~eoyqi2keqi";
	tool->decode(str);

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	JniMethodInfo minfo2;
//	bool isHave2 = JniHelper::getStaticMethodInfo(minfo2,"net/zakume/game/NarutoSenki","getCurrentName", "()Ljava/lang/String;");
//
//	if (isHave2)
//	{
//		jstring jstr2=(jstring) minfo2.env->CallStaticObjectMethod(minfo2.classID, minfo2.methodID);
//		bag = bag + JniHelper::jstring2string(jstr2);
//	}else{
//		_CharacterArray=NULL;
//
//	}
//
//	if(strcmp(bag.c_str(),"")!=0 && strcmp(bag.c_str(),str.c_str())!=0 ){
//		_CharacterArray=NULL;
//	}
//
//#endif


	if(konohaTowerCount==0 || akatsukiTowerCount==0){
		if(zhenying>0){
			if(konohaTowerCount==0){
				this->onGameOver(false);
			}else{
				this->onGameOver(true);
			}
		}else{
			if(akatsukiTowerCount==0){
				this->onGameOver(false);
			}else{
				this->onGameOver(true);
			}
		}


	}
}


void GameLayer::clearDoubleClick(){
	if(_hudLayer->skill1Button->getDoubleSkill() &&
		_hudLayer->skill1Button->_clickNum>=1
		){
			_hudLayer->skill1Button->setFreezeAction(NULL);
			_hudLayer->skill1Button->beganAnimation();
	}

}

void GameLayer::JoyStickRelease(){

	if (currentPlayer->getActionState()==ACTION_STATE_WALK){
		currentPlayer->idle();
	}
}

void GameLayer::JoyStickUpdate(Vec2 direction){
	if(!ougisChar){
		//CCLOG("x:%f,y:%f",direction.x,direction.y);
		currentPlayer->walk(direction);
	}
}

void GameLayer::attackButtonClick(abType type){
	if(type==NAttack){
		_isAttackButtonRelease=false;
	}

	if(type==Item1){
		currentPlayer->setItem(type);
	}else{
		currentPlayer->attack(type);
	};
}
void GameLayer::gearButtonClick(gearType type){

	currentPlayer->useGear(type);

}


void GameLayer::attackButtonRelease(){
	_isAttackButtonRelease=true;
}

void GameLayer::onPause(){

	RenderTexture* snapshoot=RenderTexture::create(winSize.width,winSize.height);
	Scene* f = Director::getInstance()->getRunningScene();
	Ref* pObject= f->getChildren().at(0);
	BGLayer* bg=(BGLayer*) pObject;
	snapshoot->begin();
	bg->visit(Director::getInstance()->getRenderer(), Mat4::IDENTITY, 0);

	this->visit(Director::getInstance()->getRenderer(), Mat4::IDENTITY, 0);
	snapshoot->end();


	Scene* pscene =Scene::create();
	PauseLayer* layer=PauseLayer::create(snapshoot);

	layer->setDelegate(this);
	pscene->addChild(layer);
	Director::getInstance()->pushScene(pscene);

}

void GameLayer::onGear(){

	if(this->_isHardCoreGame){

		RenderTexture* snapshoot=RenderTexture::create(winSize.width,winSize.height);
		Scene* f = Director::getInstance()->getRunningScene();
		Ref* pObject= f->getChildren().at(0);
		BGLayer* bg=(BGLayer*) pObject;
		snapshoot->begin();
		bg->visit(Director::getInstance()->getRenderer(), Mat4::IDENTITY, 0);

		this->visit(Director::getInstance()->getRenderer(), Mat4::IDENTITY, 0);
		snapshoot->end();

		Scene* pscene =Scene::create();
		GearLayer* layer=GearLayer::create(snapshoot);

		layer->setDelegate(this);
		layer->updatePlayerGear();
		pscene->addChild(layer);
		Director::getInstance()->pushScene(pscene);

	}


}

void GameLayer::onGameOver(bool isWin){


	if(CError!=1){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Result.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Select.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Report.plist");
		RenderTexture* snapshoot=RenderTexture::create(winSize.width,winSize.height);
		Scene* f = Director::getInstance()->getRunningScene();
		Ref* pObject= f->getChildren().at(0);
		BGLayer* bg=(BGLayer*) pObject;
		snapshoot->begin();
		bg->visit(Director::getInstance()->getRenderer(), Mat4::IDENTITY, 0);
		this->visit(Director::getInstance()->getRenderer(), Mat4::IDENTITY, 0);
		snapshoot->end();

		Scene* pscene =Scene::create();
		GameOver* layer=GameOver::create(snapshoot);
		layer->setWin(isWin);
		layer->setDelegate(this);
		pscene->addChild(layer);
		Director::getInstance()->pushScene(pscene);
	}


};

void GameLayer::onLeft(){

	const auto& childArray = this->getChildren();
	for (auto pObject : childArray){
		//�������
		NarutoSenki::ActionManager* ac=(NarutoSenki::ActionManager*) pObject;
		ac->unregisterAcceptAttackListener();
	}

	//�����ͼ����

	if(randomMap==0) {
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Tower/Tower.plist");
	}else if(randomMap==1){
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Tower/Tower2.plist");
	}else if (randomMap==2){
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Tower/Tower3.plist");
	}else if (randomMap==3){
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Tower/Tower4.plist");
	}else if (randomMap==4){
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Tower/Tower5.plist");
	}

	KTools* tool=KTools::create();

	auto rematchHeros=_isRematching ? _rematchHerosOverride : nullptr;
	if(_isRematching && !rematchHeros && _CharacterArray){
		rematchHeros=NarutoSenkiPayload::createHeroList();
		Ref* heroObject;
		for(unsigned int heroIndex=0; heroIndex<_CharacterArray->count(); ++heroIndex){
			heroObject=_CharacterArray->objectAtIndex(heroIndex);
			NarutoSenki::ActionManager* hero=(NarutoSenki::ActionManager*)heroObject;
			if(strcmp(hero->getRole()->getCString(),"Player")!=0 && strcmp(hero->getRole()->getCString(),"Com")!=0){
				continue;
			}
			if(strcmp(hero->getCharacter()->getCString(),"Roshi")==0 || strcmp(hero->getCharacter()->getCString(),"Han")==0){
				continue;
			}
			NarutoSenkiPayload::addHero(rematchHeros,hero->getCharacter()->getCString(),hero->getRole()->getCString(),hero->getGroup()->getCString());
		}
	}

	std::string path;
	Ref* pObject;
	for(unsigned int characterIndex=0; characterIndex<_CharacterArray->count(); ++characterIndex){
	pObject=_CharacterArray->objectAtIndex(characterIndex);
		NarutoSenki::ActionManager* player=(NarutoSenki::ActionManager*) pObject;

		if(strcmp(player->getRole()->getCString(),"Clone")==0 ||
			strcmp(player->getRole()->getCString(),"Summon")==0){
				continue;
		}

		//ɾ�����ﻺ��
		path=cocos2d::StringUtils::format("Element/%s/%s.plist",player->getCharacter()->getCString(),player->getCharacter()->getCString());
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(path);

		//ɾ��Ԥ������Ч
		if(strcmp(player->getRole()->getCString(),"Com")==0 ||
			strcmp(player->getRole()->getCString(),"Player")==0
			){
			tool->prepareFileOGG(player->getCharacter()->getCString(),1);
		}


		//ɾ�������ɫ
		if(strcmp(player->getCharacter()->getCString(),"Jiraiya")==0){
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/SageJiraiya/SageJiraiya.plist");
			tool->prepareFileOGG("SageJiraiya",1);
		}else if(strcmp(player->getCharacter()->getCString(),"Kankuro")==0){
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Karasu/Karasu.plist");
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Sanshouuo/Sanshouuo.plist");
		}else if(strcmp(player->getCharacter()->getCString(),"Kakuzu")==0){
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/MaskFudon/MaskFudon.plist");
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/MaskRaidon/MaskRaidon.plist");
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/MaskKadon/MaskKadon.plist");
		}else if(strcmp(player->getCharacter()->getCString(),"Naruto")==0){
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/SageNaruto/SageNaruto.plist");
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/RikudoNaruto/RikudoNaruto.plist");
			tool->prepareFileOGG("SageNaruto",1);
			tool->prepareFileOGG("RikudoNaruto",1);
		}else if(strcmp(player->getCharacter()->getCString(),"RockLee")==0){
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Lee/Lee.plist");
		}else if(strcmp(player->getCharacter()->getCString(),"Lee")==0){
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/RockLee/RockLee.plist");
		}else if(strcmp(player->getCharacter()->getCString(),"Sasuke")==0){
			tool->prepareFileOGG("ImmortalSasuke",1);
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/ImmortalSasuke/ImmortalSasuke.plist");
		}else if(strcmp(player->getCharacter()->getCString(),"Shisui")==0){
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Shisui/Shisui-1.plist");
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Shisui/Shisui-2.plist");
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Shisui/Shisui-3.plist");
		}
		player->removeFromParentAndCleanup(true);

	}

	//�����ػ���
	if(this->_isHardCoreGame){
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Roshi/Roshi.plist");
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Element/Han/Han.plist");
		tool->prepareFileOGG("Roshi",1);
		tool->prepareFileOGG("Han",1);
	}

	tool->prepareFileOGG("Effect",1);
	tool->prepareFileOGG("Ougis",1);


    if(_CharacterArray){
        _CharacterArray->removeAllObjects();
        _CharacterArray=NULL;
    }

    if(_TowerArray){
        _TowerArray->removeAllObjects();
        _TowerArray=NULL;
    }

    if(_KonohaFlogArray){
        _KonohaFlogArray->removeAllObjects();
        _KonohaFlogArray=NULL;
    }

    if(_AkatsukiFlogArray){
        _AkatsukiFlogArray->removeAllObjects();
        _AkatsukiFlogArray=NULL;
    }

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("UI.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Map.plist");

	Scene* nextScene =Scene::create();
	if(_isRematching && rematchHeros && rematchHeros->count()>0){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Result.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Select.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Report.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis2.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map.plist");
		LoadLayer* loadLayer=LoadLayer::create();
		loadLayer->tempHeros=rematchHeros;
		loadLayer->_isHardCoreMode=_isHardCoreGame;
		loadLayer->_isRandomChar=_isRandomChar;
		loadLayer->_isTrainingMode=_isTrainingMode;
		loadLayer->_selectedMap=selectedMap;
		loadLayer->_isDuelMode=_isDuelMode;
		loadLayer->_duelKillLimit=_duelKillLimit;
		nextScene->addChild(loadLayer);
		loadLayer->preloadAudio();
	}else{
		StartMenu* menuLayer=StartMenu::create();
		nextScene->addChild(menuLayer);
	}
	Director::getInstance()->replaceScene(TransitionFade::create(2.0f,nextScene));



}

void GameLayer::onRematch(){

	_isRematching=true;
	_isExiting=true;
	Director::getInstance()->popScene();

}

void GameLayer::checkPost(float dt){

	// Jalur heartbeat online lama tidak dipakai pada mode offline.
	(void)dt;
	isPosting=false;
}

void GameLayer::onHttpRequestCompleted(Node *sender ,void *data) {

	// Callback lama dipertahankan sebagai stub untuk kontrak internal.
	(void)sender;
	(void)data;
	isPosting=false;
}


void GameLayer::checkBackgroundMusic(float dt){
	if(UserDefault::getInstance()->getBoolForKey("isBGM")!=false){

		if(!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
			if(!_isHardCoreGame){
				SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/Battle1.mp3");
			}else{
				if(_playNum==0){
					std::string musicPath=cocos2d::StringUtils::format("Audio/Music/Battle%d.mp3",int(2+randomMap*3));
					SimpleAudioEngine::getInstance()->playBackgroundMusic(musicPath.c_str(),false);
					_playNum++;
				}else if(_playNum==1){
					std::string musicPath=cocos2d::StringUtils::format("Audio/Music/Battle%d.mp3",int(3+randomMap*3));
					SimpleAudioEngine::getInstance()->playBackgroundMusic(musicPath.c_str(),false);
					_playNum++;
				}else if(_playNum==2){
					std::string musicPath=cocos2d::StringUtils::format("Audio/Music/Battle%d.mp3",int(1+randomMap*3));
					SimpleAudioEngine::getInstance()->playBackgroundMusic(musicPath.c_str(),false);
					_playNum=0;
				}
			}


		}
	}
}


void GameLayer::setOugis(Node* sender){

	if(!this->_hudLayer->ougisLayer){
		const auto& childArray = this->getChildren();
		ougisChar=sender;
		NarutoSenki::ActionManager* Sender=(NarutoSenki::ActionManager*) sender;
		for (auto pObject : childArray){

			Node* object=(Node*) pObject;

			object->pauseSchedulerAndActions();

		}
		Director::getInstance()->getScheduler()->pauseTarget(this);

		this->updateViewPoint(0.0);
		/*
		Vec2 normalPos=this->getPosition();
		Rect ougisBox=sender->boundingBox();
		Vec2 ougisPoint=Vec2(ougisBox.getMidX(),ougisBox.getMidY());
		float ougisZoom=1.50f;
		Vec2 zoomPos=Vec2(winSize.width/2-ougisPoint.x*ougisZoom,winSize.height/2-ougisPoint.y*ougisZoom);

		this->stopAllActions();
		this->runAction(Sequence::create(
			EaseSineInOut::create(Spawn::create(
				ScaleTo::create(0.28f,ougisZoom),
				MoveTo::create(0.28f,zoomPos),
				NULL
			)),
			DelayTime::create(0.28f),
			EaseSineInOut::create(Spawn::create(
				ScaleTo::create(0.50f,1.0f),
				MoveTo::create(0.50f,normalPos),
				NULL
			)),
			NULL
		));
		*/

		blend=LayerColor::create(Color4B(0, 0, 0, 200),winSize.width,winSize.height);
		blend->setPosition(Vec2(-this->getPositionX(),0));
		this->addChild(blend,1000);
		sender->setZOrder(2000);

		if(UserDefault::getInstance()->getBoolForKey("isVoice")!=false){
			std::string ougisPath=cocos2d::StringUtils::format("Audio/Ougis/%s_ougis.mp3",Sender->getCharacter()->getCString());
			KTools::playEffectOptimized(ougisPath.c_str(), 15, 3);
		}

		_hudLayer->setOugis(Sender->getCharacter()->getCString(),Sender->getGroup()->getCString());
	}



}

void GameLayer::removeOugis(){
	ougisChar->setZOrder(-ougisChar->getPositionY());
	/*
	this->stopAllActions();
	this->setScale(1.0f);
	*/
	const auto& childArray = this->getChildren();
	for (auto pObject : childArray){
		Node* object=(Node*) pObject;
		object->resumeSchedulerAndActions();
	}
	Director::getInstance()->getScheduler()->resumeTarget(this);
	this->updateViewPoint(0.0);

	blend->removeFromParent();
	ougisChar=NULL;
}






