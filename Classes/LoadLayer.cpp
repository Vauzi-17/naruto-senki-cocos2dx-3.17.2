#include "LoadLayer.h"
#include "HeroPayload.h"


#include<time.h>
#include <fstream>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <direct.h>
#endif


USING_NS_CC_EXT;
using namespace std;
using namespace CocosDenshion;

namespace {

void preloadCharacterMatchAudio(const char* character)
{
	if (!character || !character[0])
		return;

	const std::string xmlPath=cocos2d::StringUtils::format("Element/%s/%s.xml",character,character);
	KTools::preloadMatchEffectsFromXML(xmlPath.c_str());

	const std::string ougisPath=cocos2d::StringUtils::format("Audio/Ougis/%s_ougis.mp3",character);
	KTools::preloadMatchEffect(ougisPath.c_str());
}

}

LoadLayer::LoadLayer(void)
{
	tempHeros=NULL;
	loadNum=0;
	_hudLayer=NULL;
	_bgLayer=NULL;
	_hudLayer=NULL;
	_isHardCoreMode=false;
	_isRandomChar=false;
	_isTrainingMode=false;
	_selectedMap=-1;
	_isDuelMode=false;
	_duelKillLimit=3;
	isPosting=false;
}


LoadLayer::~LoadLayer(void)
{

	CC_SAFE_RELEASE(tempHeros);

}

bool LoadLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());



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

		Sprite* loading_title=Sprite::createWithSpriteFrameName("loading_title.png");
		loading_title->setAnchorPoint(Vec2(0,0));
		loading_title->setPosition(Vec2(2,winSize.height-loading_title->getContentSize().height-2));
		this->addChild(loading_title,3);


		//produce the cloud
		Sprite* cloud_left=Sprite::createWithSpriteFrameName("cloud.png");
		cloud_left->setPosition(Vec2(0,15));
		cloud_left->setFlipX(true);
		cloud_left->setFlipY(true);
		cloud_left->setAnchorPoint(Vec2(0,0));
		this->addChild(cloud_left,1);

		ActionInterval* cmv1=MoveBy::create(1,Vec2(-15,0));
		Action* cseq1=RepeatForever::create(Sequence::create(cmv1, cmv1->reverse(), nullptr));
		cloud_left->runAction(cseq1);

		Sprite* cloud_right=Sprite::createWithSpriteFrameName("cloud.png");
		cloud_right->setPosition(Vec2(winSize.width-cloud_right->getContentSize().width,
			winSize.height-(cloud_right->getContentSize().height+15)));
		cloud_right->setAnchorPoint(Vec2(0,0));
		this->addChild(cloud_right,1);

		ActionInterval* cmv2=MoveBy::create(1,Vec2(15,0));
		Action* cseq2=RepeatForever::create(Sequence::create(cmv2, cmv2->reverse(), nullptr));
		cloud_right->runAction(cseq2);


		bRet=true;
	}while(0);

	return bRet;

};


void LoadLayer::preloadIMG(){

	unsigned int teamAvatarIndex=0;
	unsigned int teamAvatarCount=0;
	std::string playerGroup;
	std::string path;

	for(unsigned int heroIndex=0; heroIndex<tempHeros->count(); ++heroIndex){
		Ref* heroPayload=tempHeros->objectAtIndex(heroIndex);
		if(strcmp(NarutoSenkiPayload::role(heroPayload),"Player")==0){
			playerGroup=NarutoSenkiPayload::group(heroPayload);
			break;
		}
	}
	for(unsigned int heroIndex=0; heroIndex<tempHeros->count(); ++heroIndex){
		Ref* heroPayload=tempHeros->objectAtIndex(heroIndex);
		const char* role=NarutoSenkiPayload::role(heroPayload);
		if(strcmp(NarutoSenkiPayload::group(heroPayload),playerGroup.c_str())==0 &&
			(strcmp(role,"Player")==0 || strcmp(role,"Com")==0)){
			teamAvatarCount++;
		}
	}
	KTools::preloadMatchEffect("Audio/Effect/poof.mp3");
	KTools::preloadMatchEffect("Audio/Effect/poof2.mp3");
	KTools::preloadMatchEffect("Audio/Effect/suzou_effect.mp3");

	for(unsigned int heroIndex=0; heroIndex<tempHeros->count(); ++heroIndex){
		Ref* pObject=tempHeros->objectAtIndex(heroIndex);

		const char* player=NarutoSenkiPayload::character(pObject);


		//���ؼ���ͼ��
		Image::setPVRImagesHavePremultipliedAlpha(true);

		if (strcmp(player,"Kiba")!=0 &&
			strcmp(player,"Kakuzu")!=0
			){

				path=cocos2d::StringUtils::format("Element/Skills/%s_Skill.plist",player);
				SpriteFrameCache::getInstance()->addSpriteFramesWithFile(path);
				std::string texturePath=cocos2d::StringUtils::format("Element/Skills/%s_Skill.pvr.ccz",player);
				applyPixelArtToTexture(Director::getInstance()->getTextureCache()->textureForKey(texturePath));
		}

		//Ԥ������Ч
		preloadCharacterMatchAudio(player);



		path=cocos2d::StringUtils::format("Element/%s/%s.plist",player,player);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(path);
		std::string texturePath=cocos2d::StringUtils::format("Element/%s/%s.pvr.ccz",player,player);
		applyPixelArtToTexture(Director::getInstance()->getTextureCache()->textureForKey(texturePath));



		//���ض����ɫ
		if(strcmp(player,"Jiraiya")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/SageJiraiya/SageJiraiya.plist");
			preloadCharacterMatchAudio("SageJiraiya");
		}else if(strcmp(player,"Kankuro")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Karasu/Karasu.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Sanshouuo/Sanshouuo.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Saso/Saso.plist");
			preloadCharacterMatchAudio("Karasu");
			preloadCharacterMatchAudio("Sanshouuo");
			preloadCharacterMatchAudio("Saso");
		}else if(strcmp(player,"Chiyo")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Parents/Parents.plist");
			preloadCharacterMatchAudio("Parents");
		}else if(strcmp(player,"Kakuzu")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/MaskRaidon/MaskRaidon.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/MaskFudon/MaskFudon.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/MaskKadon/MaskKadon.plist");
			preloadCharacterMatchAudio("MaskRaidon");
			preloadCharacterMatchAudio("MaskFudon");
			preloadCharacterMatchAudio("MaskKadon");
		}else if(strcmp(player,"Naruto")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/RikudoNaruto/RikudoNaruto.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/SageNaruto/SageNaruto.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Kurama/Kurama.plist");
			preloadCharacterMatchAudio("Kurama");
			preloadCharacterMatchAudio("SageNaruto");
			preloadCharacterMatchAudio("RikudoNaruto");
		}else if(strcmp(player,"Lee")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/RockLee/RockLee.plist");
			preloadCharacterMatchAudio("RockLee");
		}else if(strcmp(player,"Tsunade")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Slug/Slug.plist");
			preloadCharacterMatchAudio("Slug");
		}else if(strcmp(player,"Shisui")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-1.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-2.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-3.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-4.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-5.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-6.plist");
		}else if(strcmp(player,"Kakashi")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/DogWall/DogWall.plist");
			preloadCharacterMatchAudio("DogWall");
		}else if(strcmp(player,"Deidara")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Centipede/Centipede.plist");
			preloadCharacterMatchAudio("Centipede");
		}else if(strcmp(player,"Pain")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/DevaPath/DevaPath.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/AsuraPath/AsuraPath.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/AnimalPath/AnimalPath.plist");
			preloadCharacterMatchAudio("DevaPath");
			preloadCharacterMatchAudio("AsuraPath");
			preloadCharacterMatchAudio("AnimalPath");
		}else if(strcmp(player,"Sasuke")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/ImmortalSasuke/ImmortalSasuke.plist");
			preloadCharacterMatchAudio("ImmortalSasuke");
		}else if(strcmp(player,"Kiba")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Akamaru/Akamaru.plist");
			preloadCharacterMatchAudio("Akamaru");
		}


		const char* playerRole=NarutoSenkiPayload::role(pObject);
		const char* playerTeam=NarutoSenkiPayload::group(pObject);
		const bool showTeamAvatar=!playerGroup.empty() &&
			strcmp(playerTeam,playerGroup.c_str())==0 &&
			(strcmp(playerRole,"Player")==0 || strcmp(playerRole,"Com")==0);

		if(showTeamAvatar){

			std::string loadingAvatarPath=cocos2d::StringUtils::format("%s_Walk_01.png",player);
			Sprite* loadingAvator=Sprite::createWithSpriteFrameName(loadingAvatarPath);
			loadingAvator->setFlipX(true);
			float avatarStartX=winSize.width-100-static_cast<float>(teamAvatarCount>0 ? teamAvatarCount-1 : 0)*16.0f;
			loadingAvator->setPosition(Vec2(avatarStartX+teamAvatarIndex*16.0f,30));
			loadingAvator->setAnchorPoint(Vec2(0,0));

			Vector<SpriteFrame*> animeFrames;

			std::string filePrefix=cocos2d::StringUtils::format("%s_Walk_",player);
			int frameCount;
			if(strcmp(player,"Konan")==0){
				frameCount=1;
			}else {
				frameCount=7;
			}

			for(int i = 1; i < frameCount; i++){
				std::string framePath=cocos2d::StringUtils::format("%s%02d.png",filePrefix.c_str(),i);
				SpriteFrame *frame =  SpriteFrameCache::getInstance()->spriteFrameByName(framePath);
				applyPixelArtToFrame(frame);
				if(frame){
					animeFrames.pushBack(frame);
				}
			}

			Animation* tempAnimation=Animation::createWithSpriteFrames(animeFrames, float(1.0 / 10.0));
			FiniteTimeAction* tempAction=Animate::create(tempAnimation);
			Action* seq=RepeatForever::create(Sequence::create(tempAction,nullptr));

			this->addChild(loadingAvator);
			loadingAvator->runAction(seq);
			teamAvatarIndex++;



		}


	};

	tempHeros->retain();


	//�����ػ���
	if(_isHardCoreMode){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Roshi/Roshi.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Han/Han.plist");
		preloadCharacterMatchAudio("Roshi");
		preloadCharacterMatchAudio("Han");
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/KotetsuFlog.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/FemalePainFlog.plist");


	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/IzumoFlog.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/KakashiFlog.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/PainFlog.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/ObitoFlog.plist");



	try{
		srand((int)time(0));
		int num=rand()%3+1;
		std::string tipsPath=cocos2d::StringUtils::format("tip%d.png",num);
		Sprite* tips=Sprite::createWithSpriteFrameName(tipsPath);
		tips->setPosition(Vec2(winSize.width/2,winSize.height/2));
		this->addChild(tips);

	}catch(...){
		CCLOG("error");
		return;
	}


	Sprite* loading=Sprite::createWithSpriteFrameName("loading_font.png");

	loading->setPosition(Vec2(winSize.width-120,45));
	ActionInterval* fade=FadeOut::create(1.0f);
	Action* fadeseq=RepeatForever::create(Sequence::create(fade, fade->reverse(), nullptr));
	this->addChild(loading);
	loading->runAction(fadeseq);

	this->scheduleOnce(CC_SCHEDULE_SELECTOR(LoadLayer::playBGM),1.0f);



	this->scheduleOnce(CC_SCHEDULE_SELECTOR(LoadLayer::onLoadFinish),3.0f);



}

void LoadLayer::playBGM(float dt){
   SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/loading_music.mp3",false);
}


void LoadLayer::onHttpRequestCompleted(Node *sender ,void *data) {
	// Jalur download DLC lama tidak dipakai pada mode offline.
	(void)sender;
	(void)data;
	isPosting=false;
}

void LoadLayer::preloadAudio(){

	char* bg_src;
	if(_isHardCoreMode){
		bg_src="blue_bg.png";
	}else{
		bg_src="red_bg.png";
	}
	Sprite* bgSprite = Sprite::create(bg_src);

	bgSprite->setAnchorPoint(Vec2(0,0));
	scaleSpriteToScreen(bgSprite);
	bgSprite->setPosition(Vec2(0,0));
	this->addChild(bgSprite, -5);

	KTools* tool=KTools::create();
	tool->prepareFileMD5();
	std::string str2="fkhfnPG8";
	tool->decode(str2);
	std::string filePath=FileUtils::getInstance()->getWritablePath()+cocos2d::StringUtils::format("%s.xml",str2.c_str());
	std::string strFileMD5 =CMD5Checksum::GetMD5(filePath);
	remove(filePath.c_str());
	std::string code1=cocos2d::StringUtils::format("%s%s",strFileMD5.substr(16,5).c_str(),strFileMD5.substr(6,5).c_str());
	if(code1!="e0213bffe1"){
	const char* soundpath="Audio";
	//return;
	//tool->dfsFolder(soundpath,0,1);
	}

	KTools::releaseMatchEffects();
	this->preloadIMG();
}

void LoadLayer::onLoadFinish(float dt){



	Scene* gameScene=Scene::create();

	_gameLayer=GameLayer::create();
	_gameLayer->Heros=tempHeros;
	_gameLayer->_isHardCoreGame=_isHardCoreMode;
	_gameLayer->_isRandomChar=_isRandomChar;
	_gameLayer->_isTrainingMode=_isTrainingMode;
	_gameLayer->_isDuelMode=_isDuelMode;
	_gameLayer->_duelKillLimit=_duelKillLimit;
	_gameLayer->selectedMap=_selectedMap;
	_gameLayer->initHeros();

	_bgLayer=BGLayer::create();
	_bgLayer->setDelegate(_gameLayer);

	_hudLayer=HudLayer::create();
	_hudLayer->setDelegate(_gameLayer);
	//_hudLayer->initHeroInterface();

	_gameLayer->setHudLayer(_hudLayer);
	_gameLayer->setTotalKills(0);
	_gameLayer->setTotalTM(0);

	gameScene->addChild(_gameLayer,GlTag);
	gameScene->addChild(_bgLayer,BgTag);
	gameScene->addChild(_hudLayer,HudTag);

	KTools* tool= KTools::create();

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f,gameScene));

}

void LoadLayer::preloadPlist(Ref* obj){

	Texture2D* texture=(Texture2D*)obj; //�����obj�����첽���ɵ�����

	int totalCount=tempHeros ? tempHeros->count() : 0;
	if(loadNum>=totalCount){
		this->onLoadFinish(0.2f);
		return;
	}

	Ref* pObject=tempHeros->objectAtIndex(loadNum);
	const char* name=NarutoSenkiPayload::character(pObject);

    std::string path=cocos2d::StringUtils::format("Element/%s/%s.plist",name,name);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(path,texture);//ͨ��������.plist�ļ�����SpriteFrameCache

	loadNum++;

	if(loadNum<totalCount){
		this->preloadIMG();
	}else{
		this->onLoadFinish(0.2f);
	}


}

void LoadLayer::onEnter(){
	Layer::onEnter();

}

void LoadLayer::onExit(){
	Layer::onExit();
}
