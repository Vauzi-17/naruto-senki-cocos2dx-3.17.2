#include "LoadLayer.h"


#include<time.h>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#endif


USING_NS_CC_EXT;
using namespace std;
using namespace CocosDenshion;

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
	DLCArray=NULL;
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

	Ref* pObject=NULL;
	int i=0;
	const char* path;
	KTools* tool=KTools::create();

	CCARRAY_FOREACH(tempHeros,pObject){

		CCDictionary* tempdict=(CCDictionary *) pObject;

		CCString* player=CCString::create(tempdict->valueForKey("character")->getCString());


		//���ؼ���ͼ��
		Image::setPVRImagesHavePremultipliedAlpha(true);

		if (strcmp(player->getCString(),"Kiba")!=0 &&
			strcmp(player->getCString(),"Kakuzu")!=0
			){

				path=CCString::createWithFormat("Element/Skills/%s_Skill.plist",player->getCString())->getCString();
				SpriteFrameCache::getInstance()->addSpriteFramesWithFile(path);
				applyPixelArtToTexture(Director::getInstance()->getTextureCache()->textureForKey(CCString::createWithFormat("Element/Skills/%s_Skill.pvr.ccz",player->getCString())->getCString()));
		}

		//Ԥ������Ч
		tool->prepareFileOGG(player->getCString());



		/*	if(strcmp(player->getCString(),"Chiyo")==0 ||
		strcmp(player->getCString(),"Shikamaru")==0 ||
		strcmp(player->getCString(),"Kisame")==0
		){

		std::string filePath=FileUtils::getInstance()->getWritablePath()+CCString::createWithFormat("/DLC/%s.xml",player->getCString())->getCString();
		bool isExisted=FileUtils::getInstance()->isFileExist(filePath);

		if(!isExisted){
		if(!DLCArray){
		DLCArray=CCArray::create();
		DLCArray->retain();
		}
		CCDictionary* dic=CCDictionary::create();
		dic->setObject(player,"character");
		DLCArray->addObject(dic);
		}

		}*/

		path=CCString::createWithFormat("Element/%s/%s.plist",player->getCString(),player->getCString())->getCString();
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(path);
		applyPixelArtToTexture(Director::getInstance()->getTextureCache()->textureForKey(CCString::createWithFormat("Element/%s/%s.pvr.ccz",player->getCString(),player->getCString())->getCString()));



		//���ض����ɫ
		if(strcmp(player->getCString(),"Jiraiya")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/SageJiraiya/SageJiraiya.plist");
			tool->prepareFileOGG("SageJiraiya");
		}else if(strcmp(player->getCString(),"Kankuro")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Karasu/Karasu.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Sanshouuo/Sanshouuo.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Saso/Saso.plist");
		}else if(strcmp(player->getCString(),"Chiyo")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Parents/Parents.plist");
		}else if(strcmp(player->getCString(),"Kakuzu")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/MaskRaidon/MaskRaidon.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/MaskFudon/MaskFudon.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/MaskKadon/MaskKadon.plist");
		}else if(strcmp(player->getCString(),"Naruto")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/RikudoNaruto/RikudoNaruto.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/SageNaruto/SageNaruto.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Kurama/Kurama.plist");
			tool->prepareFileOGG("SageNaruto");
			tool->prepareFileOGG("RikudoNaruto");
		}else if(strcmp(player->getCString(),"Lee")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/RockLee/RockLee.plist");
		}else if(strcmp(player->getCString(),"Tsunade")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Slug/Slug.plist");
		}else if(strcmp(player->getCString(),"Shisui")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-1.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-2.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-3.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-4.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-5.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Shisui/Shisui-6.plist");
		}else if(strcmp(player->getCString(),"Kakashi")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/DogWall/DogWall.plist");
		}else if(strcmp(player->getCString(),"Deidara")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Centipede/Centipede.plist");
		}else if(strcmp(player->getCString(),"Pain")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/DevaPath/DevaPath.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/AsuraPath/AsuraPath.plist");
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/AnimalPath/AnimalPath.plist");
		}else if(strcmp(player->getCString(),"Sasuke")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/ImmortalSasuke/ImmortalSasuke.plist");
			tool->prepareFileOGG("ImmortalSasuke");
		}else if(strcmp(player->getCString(),"Kiba")==0){
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Akamaru/Akamaru.plist");
		}


		if(i==0 ||( (i==1 || i==2 ) && _isHardCoreMode) ){

			Sprite* loadingAvator=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s_Walk_01.png",player->getCString())->getCString());
			loadingAvator->setFlipX(true);
			loadingAvator->setPosition(Vec2(winSize.width-100+i*16,30));
			loadingAvator->setAnchorPoint(Vec2(0,0));

			CCArray* animeFrames = CCArray::create();
			CCString* str;

			const char* file=CCString::createWithFormat("%s_Walk_",player->getCString())->getCString();
			int frameCount;
			if(strcmp(player->getCString(),"Konan")==0){
				frameCount=1;
			}else {
				frameCount=7;
			}

			for(int i = 1; i < frameCount; i++){
				str=CCString::createWithFormat("%s%02d.png",file,i);
				SpriteFrame *frame =  SpriteFrameCache::getInstance()->spriteFrameByName(str->getCString());
				applyPixelArtToFrame(frame);
				if(frame){
					animeFrames->addObject(frame);
				}
			}

			Animation* tempAnimation=cocos2d::cocos2dx2_compat::createAnimationWithSpriteFrames(animeFrames, float(1.0 / 10.0));
			Action* tempAction=Animate::create(tempAnimation);
			CCArray* seqArray=CCArray::createWithObject(tempAction);
			Action* seq;
            seq=RepeatForever::create(cocos2d::cocos2dx2_compat::createSequence(seqArray));

			this->addChild(loadingAvator);
			loadingAvator->runAction(seq);



		}

		i++;

	};

	tempHeros->retain();


	//�����ػ���
	if(_isHardCoreMode){
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Roshi/Roshi.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Han/Han.plist");
		tool->prepareFileOGG("Roshi");
		tool->prepareFileOGG("Han");
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/KotetsuFlog.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/FemalePainFlog.plist");


	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/IzumoFlog.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/KakashiFlog.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/PainFlog.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Flog/ObitoFlog.plist");

	tool->prepareFileOGG("Effect");
	tool->prepareFileOGG("Ougis");


	try{
		srand((int)time(0));
		int num=rand()%3+1;
		Sprite* tips=Sprite::createWithSpriteFrameName(CCString::createWithFormat("tip%d.png",num)->getCString());
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



	if(!DLCArray){
		this->scheduleOnce(CC_SCHEDULE_SELECTOR(LoadLayer::onLoadFinish),3.0f);
	}else{
		Ref* tmpObject;
		int i=0;
		CCARRAY_FOREACH(DLCArray,tmpObject){
			CCDictionary* tempdict=(CCDictionary *) tmpObject;
			CCString* heroString=CCString::create(tempdict->valueForKey("character")->getCString());
			isPosting=true;
			cocos2d::extension::CCHttpRequest* request = new  cocos2d::extension::CCHttpRequest();
			std::string codeFinal;
			std::string code1=CCString::createWithFormat("code=300&id=%d",MemberID)->getCString();
			std::string code2=CCString::createWithFormat("&version=%d",CURRENT_VERSION)->getCString();
			std::string code3=CCString::createWithFormat("&char=%s",heroString->getCString())->getCString();
			std::string url=SERVER"nsk/list.php?"+code1+code2+"&pw="+PWord+code3;

			request->setUrl(url.c_str());
			request->setRequestType(CCHttpRequest::kHttpGet);
			request->setResponseCallback(this, callfuncND_selector(LoadLayer::onHttpRequestCompleted));
			request->setTag(heroString->getCString());
			cocos2d::extension::CCHttpClient::getInstance()->send(request);
			request->release();
			i++;
		}
	}



}

void LoadLayer::playBGM(float dt){
   SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/loading_music.mp3",false);
}


void LoadLayer::onHttpRequestCompleted(Node *sender ,void *data) {
	/*AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);*/
	isPosting=false;

	cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;
	if (!response)
	{
		return;
	}
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s completed", response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	char statusString[64] = {};

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer:%s" ,response->getErrorBuffer());
		CCTips *tip=CCTips::create("ServerError");
		this->addChild(tip,5000);
		return;
	}




	std::vector<char> *buffer = response->getResponseData();
	std::string buf(buffer->begin(),buffer->end());

	/*for (unsigned int i = 0; i < buffer->size(); i ++)
	{
	printf("%c" ,(*buffer)[i]);
	}  */

	std::string foldName=FileUtils::getInstance()->getWritablePath()+"DLC";

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	mkdir(foldName.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#else
	_mkdir(foldName.c_str());
#endif


	std::string fileName = CCString::createWithFormat("%s.xml",response->getHttpRequest()->getTag())->getCString();
	//std::string filePath = FileUtils::getInstance()->getWritablePath()+"DLC" + fileName;

	const char* filePath;

	filePath =CCString::createWithFormat("%s/DLC/%s.xml",FileUtils::getInstance()->getWritablePath().c_str(),response->getHttpRequest()->getTag())->getCString();

	std::ofstream fos(filePath);//�������ļ�
	if(!fos)std::cerr<<"error";//У���ļ�
	fos<<buf;//���
	fos.close();
	//std::remove(filePath.c_str());


	Ref* tmpObject;
	int i=0;
	CCARRAY_FOREACH(DLCArray,tmpObject){
		CCDictionary* tempdict=(CCDictionary *) tmpObject;
		CCString* heroString=CCString::create(tempdict->valueForKey("character")->getCString());
		if(strcmp(heroString->getCString(),response->getHttpRequest()->getTag())==0){
			DLCArray->removeObject(tmpObject);
		}

	}

	if(DLCArray->count()<=0){
		this->scheduleOnce(CC_SCHEDULE_SELECTOR(LoadLayer::onLoadFinish),3.0f);
	}

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
	std::string filePath=FileUtils::getInstance()->getWritablePath()+CCString::createWithFormat("%s.xml",str2.c_str())->getCString();
	std::string strFileMD5 =CMD5Checksum::GetMD5(filePath);
	remove(filePath.c_str());
	CCString* code1=CCString::createWithFormat("%s%s",strFileMD5.substr(16,5).c_str(),strFileMD5.substr(6,5).c_str());
	if(strcmp(code1->getCString(),"e0213bffe1")!=0){
	const char* soundpath="Audio";
	//return;
	//tool->dfsFolder(soundpath,0,1);
	}

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
	_gameLayer->setTotalKills(CCString::create("0"));
	_gameLayer->setTotalTM(CCString::create("0"));

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
	CCDictionary* tempdict=(CCDictionary *) pObject;
	CCString* player=CCString::create(tempdict->valueForKey("character")->getCString());
	const char* name=player->getCString();

    const char* path=CCString::createWithFormat("Element/%s/%s.plist",name,name)->getCString();
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
