#include "GameOver.h"
#include <cstdlib>
#include <string>
#include "GameLayer.h"
#include "Characters.h"

USING_NS_CC_EXT;
using namespace CocosDenshion;
#include <fstream>
static const char* gResultHeroList[] = {
	"Konan","Sakura","Naruto","Sai","Deidara",
	"Kakashi","Itachi","Tenten","Jiraiya","Suigetsu",
	"Tsunade","Tobirama","Shisui","Neji","Ino","Asuma","Gaara",
	"Karin","Sasuke","Hidan","Choji","Kankuro",
	"Shino","Minato","Tobi","Kakuzu","Hinata",
	"Shikamaru","Chiyo","Kisame",
	"Hiruzen","Kiba","Jugo","Lee"
};

static void resultAddHero(CCArray* heros, const char* character, const char* role, const char* group)
{
	CCDictionary* dic = CCDictionary::create();
	dic->setObject(CCString::create(character), "character");
	dic->setObject(CCString::create(role), "role");
	dic->setObject(CCString::create(group), "group");
	heros->addObject(dic);
}
GameOver::GameOver(void)
{
	exitLayer=nullptr;
	cheatLayer=nullptr;
	changeCharLayer=nullptr;
	changeTargetEnemy=false;
	isPosting=false;
	refreshBtn=nullptr;
}


GameOver::~GameOver(void)
{
	AnimationCache::destroyInstance();
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();

}

bool GameOver::init(RenderTexture* snapshoot){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());

		SimpleAudioEngine::getInstance()->stopAllEffects();
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();




		Texture2D* bgTexture=snapshoot->getSprite()->getTexture();
		Sprite* bg=Sprite::createWithTexture(bgTexture);
		bg->setAnchorPoint(Vec2(0,0));
		bg->setFlipY(true);
		this->addChild(bg,0);

		Layer* blend=LayerColor::create(Color4B(0, 0, 0, 150),winSize.width,winSize.height);
		this->addChild(blend,1);

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

		Sprite* result_title=Sprite::createWithSpriteFrameName("result_title.png");
		result_title->setAnchorPoint(Vec2(0,0));
		result_title->setPosition(Vec2(2,winSize.height-result_title->getContentSize().height-2));
		this->addChild(result_title,3);

		result_bg=Sprite::createWithSpriteFrameName("gameover_bg.png");
		result_bg->setAnchorPoint(Vec2(0.5f,0.5f));
		result_bg->setScale(0.5f);
		result_bg->setPosition(Vec2(winSize.width/2,winSize.height/2-6));
		this->addChild(result_bg,4);

		ScaleTo* su=ScaleTo::create(0.2f,1.0);
		Action* seq=Sequence::create(su,CallFunc::create(CC_CALLBACK_0(GameOver::listResult, this)),nullptr);
		result_bg->runAction(seq);



		bRet=true;
	}while(0);

	return bRet;

}

void GameOver::listResult(){

	isPlayed=true;
	if(_delegate->_isHardCoreGame){
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/battle_over1.ogg");

	}else{
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/battle_over.ogg");
	}

	std::string path=cocos2d::StringUtils::format("%s_half.png",_delegate->currentPlayer->getCharacter()->getCString());
	Sprite* half=Sprite::createWithSpriteFrameName(path);
	if(!half){
		half=Sprite::createWithSpriteFrameName("Tobirama_half.png");
	}

	if(strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Konan")==0||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Karin")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Suigetsu")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Hidan")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Tobirama")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Tsunade")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Kankuro")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"SageJiraiya")==0||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Minato")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Tobi")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Lee")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"RockLee")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Hinata")==0||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Asuma")==0 ||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Chiyo")==0||
	strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"Kisame")==0
		){
		half->setFlipX(true);
	}

	half->setAnchorPoint(Vec2(0,0));
	half->setPosition(Vec2(winSize.width/2+result_bg->getContentSize().width/2-half->getContentSize().width,result_bg->getPositionY()-result_bg->getContentSize().height/2+9));
	this->addChild(half,5);


	Sprite* list_bg=Sprite::createWithSpriteFrameName("list_bg.png");
	list_bg->setAnchorPoint(Vec2(0,0));
	list_bg->setPosition(Vec2(winSize.width/2-result_bg->getContentSize().width/2+2,result_bg->getPositionY()-result_bg->getContentSize().height/2+26));
	this->addChild(list_bg,5);




	int _hour=_delegate->_minute/60;
	int _minute=_delegate->_minute%60;


	Sprite* timeBG=Sprite::createWithSpriteFrameName("time_bg.png");
	timeBG->setAnchorPoint(Vec2(0,0));
	timeBG->setPosition(Vec2(winSize.width/2+result_bg->getContentSize().width/2-11-timeBG->getContentSize().width,result_bg->getPositionY()-result_bg->getContentSize().height/2+46));
	this->addChild(timeBG,6);


	std::string tempTime=cocos2d::StringUtils::format("%02d:%02d:%02d",_hour,_minute,_delegate->_second);


	LabelBMFont* gameClock=LabelBMFont::create(tempTime.c_str(),"Fonts/1.fnt");
	gameClock->setAnchorPoint(Vec2(0.5f,0));
    gameClock->setPosition(Vec2(timeBG->getPositionX()+timeBG->getContentSize().width/2,timeBG->getPositionY()+3));
	gameClock->setScale(0.48f);
	this->addChild(gameClock,7);

	if(_delegate->_isDuelMode){
		std::string duelScore=cocos2d::StringUtils::format("DUEL %d:%d",_delegate->_duelPlayerScore,_delegate->_duelEnemyScore);
		LabelBMFont* duelScoreLabel=LabelBMFont::create(duelScore.c_str(),"Fonts/1.fnt");
		duelScoreLabel->setAnchorPoint(Vec2(0.5f,0.5f));
		duelScoreLabel->setPosition(Vec2(result_bg->getPositionX()+12,result_bg->getPositionY()+result_bg->getContentSize().height/2-52));
		duelScoreLabel->setScale(0.38f);
		this->addChild(duelScoreLabel,7);
	}



	float _totalSecond=_delegate->_minute*60+_delegate->_second;
	float resultScore=0;
	int killDead=_delegate->currentPlayer->getKillNum()-_delegate->currentPlayer->_deadNum;


	if(!_delegate->_isDuelMode && !_delegate->_isTrainingMode && _totalSecond != _delegate->getTotalTM()){
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Director::getInstance()->end();
		return;

	}

	if(_delegate->_isHardCoreGame){
		if(_totalSecond>900.0){
			resultScore=float((killDead/float(_totalSecond/60))/3)*100;
		}else{
			resultScore=float((killDead-((_totalSecond/60-15)*3))/45)*100;
		}

	}else{
		if(_totalSecond>600.0){
			resultScore=float((killDead/float(_totalSecond/60))/4)*100;
		}else{
			resultScore=float((killDead-((_totalSecond/60-10)*4))/40) *100;
		}
	}

	KTools* tool= KTools::create();

	if(!_delegate->_isDuelMode && !_delegate->_isTrainingMode && _totalSecond <2*30+5 && _isWin ){
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Director::getInstance()->end();
		return;
	}




	Ref* pObject;
	int i=0;
	int konohaKill=0;
	int akatsukiKill=0;
	int konohaIndex=0;
	int akatsukiIndex=0;
	int konohaDisplayCount=0;
	int akatsukiDisplayCount=0;

	for(unsigned int characterIndex=0; characterIndex<_delegate->_CharacterArray->count(); ++characterIndex){
		pObject=_delegate->_CharacterArray->objectAtIndex(characterIndex);
		Hero* countHero=(Hero*)pObject;
		if(strcmp(countHero->getRole()->getCString(),"Clone")==0 ||
			strcmp(countHero->getRole()->getCString(),"Summon")==0||
			strcmp(countHero->getRole()->getCString(),"Kuilei")==0 ||
			strcmp(countHero->getCharacter()->getCString(),"Han")==0 ||
			strcmp(countHero->getCharacter()->getCString(),"Roshi")==0){
			continue;
		}
		if(Cheats>10 && strcmp(countHero->getRole()->getCString(),"Player")!=0){
			continue;
		}
		if(strcmp(countHero->getGroup()->getCString(),"Konoha")==0){
			konohaDisplayCount++;
		}else{
			akatsukiDisplayCount++;
		}
	}

	int maxTeamCount=MAX(konohaDisplayCount,akatsukiDisplayCount);
	bool compactResult=maxTeamCount>3;
	float rowGap=compactResult?16.0f:25.0f;
	float avatarScale=compactResult?0.68f:1.0f;
	float labelScale=compactResult?0.24f:0.3f;
	float gearScale=compactResult?0.40f:0.5f;
	float gearGap=compactResult?15.0f:19.0f;

	for(unsigned int characterIndex=0; characterIndex<_delegate->_CharacterArray->count(); ++characterIndex){
		pObject=_delegate->_CharacterArray->objectAtIndex(characterIndex);


		Hero* hero=(Hero*)pObject;
		if(strcmp(hero->getRole()->getCString(),"Clone")==0 ||
			strcmp(hero->getRole()->getCString(),"Summon")==0||
			strcmp(hero->getRole()->getCString(),"Kuilei")==0 ||
			strcmp(hero->getCharacter()->getCString(),"Han")==0 ||
			strcmp(hero->getCharacter()->getCString(),"Roshi")==0
			){
				continue;
		}

		//4v4
		if(Cheats>10){
			if(strcmp(hero->getRole()->getCString(),"Player")!=0){
				continue;
			}
		}

		if(hero->_isControled){
			hero->_isControled=false;
			hero->changeGroup();
		}

		std::string path=cocos2d::StringUtils::format("%s_small.png",hero->getCharacter()->getCString());
		Sprite* avator_small=Sprite::createWithSpriteFrameName(path);
		if(!avator_small){
			avator_small=Sprite::createWithSpriteFrameName("error_small.png");
		}
		if(!avator_small){
			continue;
		}
		avator_small->setAnchorPoint(Vec2(0,0));
		avator_small->setScale(avatarScale);



		int realKillNum=hero->getKillNum();

		if(strcmp(hero->getGroup()->getCString(),"Konoha")==0){
			konohaKill+=realKillNum;
		}else{
			akatsukiKill+=realKillNum;
		}

		std::string knum=cocos2d::StringUtils::format("%d",realKillNum);
		LabelBMFont* killNum=LabelBMFont::create(knum.c_str(),"Fonts/1.fnt");
		killNum->setScale(labelScale);

		std::string dnum=cocos2d::StringUtils::format("%d",hero->_deadNum);
		LabelBMFont* deadNum=LabelBMFont::create(dnum.c_str(),"Fonts/1.fnt");
		deadNum->setScale(labelScale);

		std::string fnum=cocos2d::StringUtils::format("%d",hero->_flogNum);
		LabelBMFont* flogNum=LabelBMFont::create(fnum.c_str(),"Fonts/1.fnt");
		flogNum->setScale(labelScale);



		float posX=winSize.width/2-result_bg->getContentSize().width/2+2;
		float avatarHeight=avator_small->getContentSize().height*avatarScale;
		float rowY;
		int rowIndex;

		if(strcmp(hero->getGroup()->getCString(),"Konoha")==0){
			rowIndex=konohaIndex++;
			rowY=result_bg->getPositionY()-result_bg->getContentSize().height/2+(compactResult?158:152)-(rowIndex*rowGap);
		}else{
			rowIndex=akatsukiIndex++;
			rowY=result_bg->getPositionY()-result_bg->getContentSize().height/2+(compactResult?74:76)-(rowIndex*rowGap);
		}
		avator_small->setPosition(Vec2(posX,rowY));
		killNum->setPosition(Vec2(posX+44,avator_small->getPositionY()+avatarHeight/2));
		deadNum->setPosition(Vec2(posX+68,avator_small->getPositionY()+avatarHeight/2));
		flogNum->setPosition(Vec2(posX+94,avator_small->getPositionY()+avatarHeight/2));

		if(hero->getGearArray() && hero->getGearArray()->count()>0){

			Ref* pObject;
			int i=0;
			for(unsigned int gearIndex=0; gearIndex<hero->getGearArray()->count(); ++gearIndex){
				pObject=hero->getGearArray()->objectAtIndex(gearIndex);
				CCString* tmpGear=(CCString*) pObject;
				std::string gearPath=cocos2d::StringUtils::format("gear_%02d.png",std::atoi(tmpGear->getCString()));
				Sprite* gearIcon=Sprite::createWithSpriteFrameName(gearPath);
				gearIcon->setPosition(Vec2(flogNum->getPositionX()+22+i*gearGap,flogNum->getPositionY()-1));
				gearIcon->setScale(gearScale);
				this->addChild(gearIcon,7);
				i++;
			}
		}

		this->addChild(avator_small,7);
		this->addChild(killNum,7);
		this->addChild(deadNum,7);
		this->addChild(flogNum,7);

		i++;

	}

	bool isTKO=false;
	if(_totalSecond>900 && _delegate->_isSurrender){
		if(strcmp(_delegate->currentPlayer->getGroup()->getCString(),"Konoha")==0){
			if(konohaKill>akatsukiKill){
			 _isWin=true;
			  isTKO=true;
			}
		}else{
			if(akatsukiKill>konohaKill){
				_isWin=true;
				 isTKO=true;
			}
		}
	}

	if(Cheats<=10 && !_delegate->_isDuelMode && !_delegate->_isTrainingMode){

		if(akatsukiKill+konohaKill !=_delegate->getTotalKills()){
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->end();
			return;
		}

	int realKillNum=_delegate->currentPlayer->getKillNum();

	std::string tempReward="";
	if(_delegate->_isHardCoreGame){
		tempReward="FDDD";
	}else{
		tempReward="ONNN";
	}

	tool->decode(tempReward);

	std::string coinRecord=tool->readFromSQLite();
	int rewardNum;
	if(_isWin){
		rewardNum=realKillNum*75+std::atoi(tempReward.c_str());
	}else{
		rewardNum=realKillNum*50;
	}



	Sprite* coinBG=Sprite::createWithSpriteFrameName("coin_bg.png");
	coinBG->setAnchorPoint(Vec2(0,0));
	coinBG->setPosition(Vec2(winSize.width/2+result_bg->getContentSize().width/2-coinBG->getContentSize().width -11,result_bg->getPositionY()-result_bg->getContentSize().height/2+12));
	this->addChild(coinBG,6);

	Sprite* adExtra=Sprite::createWithSpriteFrameName("adExtra.png");
	adExtra->setAnchorPoint(Vec2(0.5f,0));
	adExtra->setPosition(Vec2(coinBG->getPositionX()+70,coinBG->getPositionY()+22));
	this->addChild(adExtra,7);

	std::string extraCoin;
	int tempCoin;
	if(adResult==1 && _isWin ){
		std::string tempEtra="idd4";
		tool->decode(tempEtra);
		tempCoin=rewardNum+std::atoi(coinRecord.c_str())+std::atoi(tempEtra.c_str());
		extraCoin=cocos2d::StringUtils::format("+%d",500);
	}else{
		tempCoin=rewardNum+std::atoi(coinRecord.c_str());
		extraCoin=cocos2d::StringUtils::format("+%d",0);
	}

	std::string realCoin=cocos2d::StringUtils::format("%d",tempCoin);
	tool->saveToSQLite("GameRecord","coin",realCoin.c_str(),false);


	LabelBMFont* extraLabel=LabelBMFont::create(extraCoin.c_str(),"Fonts/yellow.fnt");
	extraLabel->setScale(0.5f);
	extraLabel->setAnchorPoint(Vec2(0.5f,0));
	extraLabel->setPosition(Vec2(coinBG->getPositionX()+68,coinBG->getPositionY()+3));
	this->addChild(extraLabel,7);


	std::string rewardCoin=cocos2d::StringUtils::format("%d",rewardNum);
	LabelBMFont* rewardLabel=LabelBMFont::create(rewardCoin.c_str(),"Fonts/yellow.fnt");
	rewardLabel->setAnchorPoint(Vec2(0.5f,0));
	rewardLabel->setPosition(Vec2(coinBG->getPositionX()+28,coinBG->getPositionY()+3));
	rewardLabel->setScale(0.55f);
	this->addChild(rewardLabel,7);

	}

	char* imgSrc="";


	if(_isWin && adResult==1){
		if(resultScore>=140 && _delegate->_isHardCoreGame && _delegate->_isRandomChar){
			imgSrc="result_SSSR.png";
		}else if(resultScore>=120 && _delegate->_isHardCoreGame && _delegate->_isRandomChar){
			imgSrc="result_SSR.png";
		}else if(resultScore>=100 && _delegate->_isHardCoreGame && _delegate->_isRandomChar){
			imgSrc="result_SR.png";
		}else if(resultScore>=140){
			imgSrc="result_SSS.png";
		}else if(resultScore>=120){
			imgSrc="result_SS.png";
		}else if(resultScore>=100){
			imgSrc="result_S.png";
		}else if(resultScore>=80){
			imgSrc="result_A.png";
		}else if(resultScore>=60){
			imgSrc="result_B.png";
		}else{
			imgSrc="result_C.png";
		}
	}else if(!_isWin){
		imgSrc="result_Defeat.png";
	}

	if(strcmp(imgSrc,"")!=0){
		Sprite* recordSprite=Sprite::createWithSpriteFrameName(imgSrc);
		recordSprite->setAnchorPoint(Vec2(0,0));
		recordSprite->setPosition(Vec2(winSize.width/2+result_bg->getContentSize().width/2-recordSprite->getContentSize().width-12,result_bg->getPositionY()-result_bg->getContentSize().height/2+88));
		this->addChild(recordSprite,7);
		/*Sprite* koSprite=nullptr;
		if(isTKO && resultScore<120){
			koSprite=Sprite::createWithSpriteFrameName("twin.png");
		}else if(_isWin && resultScore<120){
			koSprite=Sprite::createWithSpriteFrameName("dwin.png");
		}
		if(koSprite){
			koSprite->setAnchorPoint(Vec2(0.5f,0.5f));
			koSprite->setPosition(Vec2(recordSprite->getPositionX()+recordSprite->getContentSize().width/2,recordSprite->getPositionY()+8));
			this->addChild(koSprite,8);
		}*/

		if(_isWin && adResult==1 &&_delegate->_isHardCoreGame){

			finnalScore=resultScore+float(_delegate->currentPlayer->_flogNum)/100;
			std::string recordString=cocos2d::StringUtils::format("%0.2fPts",finnalScore);

			LabelBMFont* recordScore=LabelBMFont::create(recordString.c_str(),"Fonts/1.fnt");
			recordScore->setAnchorPoint(Vec2(1,0.5f));
			recordScore->setPosition(Vec2(recordSprite->getPositionX()+recordSprite->getContentSize().width,recordSprite->getPositionY()-7));
			recordScore->setScale(0.35f);
			this->addChild(recordScore,10);

			upload_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("upload_btn.png"),Sprite::createWithSpriteFrameName("upload_btn.png"),nullptr,CC_CALLBACK_1(GameOver::onUPloadBtn, this));
			Menu* upMenu=Menu::create(upload_btn,nullptr);
			upload_btn->setAnchorPoint(Vec2(1.0,0));
			upMenu->setPosition(Vec2(winSize.width/2+result_bg->getContentSize().width/2-14,winSize.height/2+result_bg->getContentSize().height/2-62));
			this->addChild(upMenu,7);


			detailRecord=cocos2d::StringUtils::format("%02d:%02d,%d,%d,%d",_minute,_delegate->_second,_delegate->currentPlayer->getKillNum(),_delegate->currentPlayer->_deadNum,_delegate->currentPlayer->_flogNum);

		}

		//记录 最速获胜时间
		if(Cheats<=10 && !_delegate->_isDuelMode && !_delegate->_isTrainingMode){

			resultChar=_delegate->currentPlayer->getCharacter()->getCString();

			//觉醒必备
			if(strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"SageNaruto")==0){
				resultChar="Naruto";
			}else if(strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"RikudoNaruto")==0){
				resultChar="Naruto";
			}else if(strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"SageJiraiya")==0){
				resultChar="Jiraiya";
			}else if(strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"ImmortalSasuke")==0){
				resultChar="Sasuke";
			}else if(strcmp(_delegate->currentPlayer->getCharacter()->getCString(),"RockLee")==0){
				resultChar="Lee";
			}


			if(_isWin ){
				std::string winNum=tool->readSQLite("CharRecord","name",resultChar.c_str(),"column1");
				int tempWin=0;

				if(resultScore>=140){
					tempWin=std::atoi(winNum.c_str())+3;
				}else if(resultScore>=120){
					tempWin=std::atoi(winNum.c_str())+2;
				}else{
					tempWin=std::atoi(winNum.c_str())+1;
				}

				std::string realWin=cocos2d::StringUtils::format("%d",tempWin);
				if(CError!=1){
					tool->saveSQLite("CharRecord","name",resultChar.c_str(),"column1",(char*) realWin.c_str(),false);
				}
				//
				if(_delegate->_isRandomChar && resultScore>=120 ){
					Ref* pObject;

					if(_delegate->currentPlayer->_isControled){
						_delegate->currentPlayer->_isControled=false;
						_delegate->currentPlayer->changeGroup();
					}


					for(unsigned int characterIndex=0; characterIndex<_delegate->_CharacterArray->count(); ++characterIndex){
		pObject=_delegate->_CharacterArray->objectAtIndex(characterIndex);

						Hero* hero=(Hero*)pObject;
						if(strcmp(hero->getRole()->getCString(),"Clone")==0 ||
							strcmp(hero->getRole()->getCString(),"Player")==0 ||
							strcmp(hero->getRole()->getCString(),"Summon")==0||
							strcmp(hero->getRole()->getCString(),"Kuilei")==0 ||
							strcmp(hero->getCharacter()->getCString(),"Han")==0 ||
							strcmp(hero->getCharacter()->getCString(),"Roshi")==0
							){
								continue;
						}

						if(hero->_isControled){
							hero->_isControled=false;
							hero->changeGroup();
						}


						if(strcmp(hero->getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())==0){

							std::string winNum=tool->readSQLite("CharRecord","name",hero->getCharacter()->getCString(),"column1");
							int tempWin=0;
							if(resultScore>=140){
								tempWin=std::atoi(winNum.c_str())+2;
							}else{
								tempWin=std::atoi(winNum.c_str())+1;
							}

							std::string realWin=cocos2d::StringUtils::format("%d",tempWin);
							if(CError!=1){
								tool->saveSQLite("CharRecord","name",hero->getCharacter()->getCString(),"column1",(char*) realWin.c_str(),false);
							}

						}

					}


				}

				//最速记录
				std::string recordTime=tool->readSQLite("CharRecord","name",resultChar.c_str(),"column3");
				if(recordTime.empty()){
					if(CError!=1){
						tool->saveSQLite("CharRecord","name",resultChar.c_str(),"column3",(char*) tempTime->getCString(),false);
					}
				}else{
					std::string bestTime=recordTime;
					std::string recordHour=bestTime.substr(0,2);
					std::string recordMinute=bestTime.substr(3,2);
					std::string recordSecond=bestTime.substr(6,2);
					bool isNewRecord=false;
					if(std::atoi(recordHour.c_str())>_hour){
						isNewRecord=true;
					}else if(std::atoi(recordHour.c_str())==_hour){

						if(std::atoi(recordMinute.c_str())>_minute){
							isNewRecord=true;
						}else if(std::atoi(recordMinute.c_str())==_minute){
							if(std::atoi(recordSecond.c_str())>_delegate->_second){
								isNewRecord=true;
							}else{
								isNewRecord=false;
							}
						}else {

							isNewRecord=false;
						}

					}else{
						isNewRecord=false;
					}

					if(isNewRecord){
						if(CError!=1){
							tool->saveSQLite("CharRecord","name",resultChar.c_str(),"column3",(char*) tempTime->getCString(),false);
						}
					}



				}
				}

			}





	}




	if(Cheats<=10 && !_delegate->_isDuelMode && !_delegate->_isTrainingMode){
		LabelBMFont* version=LabelBMFont::create("The Last Fixed","Fonts/1.fnt");
		version->setPosition(Vec2(winSize.width/2+94,result_bg->getPositionY()-result_bg->getContentSize().height/2+6));
		version->setScale(0.3f);
		this->addChild(version,5);

	}else{

		LabelBMFont* version=LabelBMFont::create("The Carnival","Fonts/1.fnt");
		version->setPosition(Vec2(winSize.width/2+94,result_bg->getPositionY()-result_bg->getContentSize().height/2+6));
		version->setScale(0.3f);
		this->addChild(version,5);
	}


	MenuItem* btm_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("close_btn1.png"),Sprite::createWithSpriteFrameName("close_btn2.png"),nullptr,CC_CALLBACK_1(GameOver::onBackToMenu, this));
	Menu* overMenu=Menu::create(btm_btn,nullptr);
	overMenu->setPosition(Vec2(winSize.width/2+result_bg->getContentSize().width/2-12,winSize.height/2+result_bg->getContentSize().height/2-18));
	this->addChild(overMenu,7);

	if(_delegate->_isDuelMode || _delegate->_isTrainingMode){
		Label* changeLabel=Label::createWithSystemFont("CHANGE",FONT_TYPE,11);
		changeLabel->setColor(ccc3(255,224,80));
		MenuItemLabel* changeBtn=MenuItemLabel::create(changeLabel,CC_CALLBACK_1(GameOver::onChangeChar, this));
		Menu* changeMenu=Menu::create(changeBtn,nullptr);
		changeMenu->setPosition(Vec2(result_bg->getPositionX()+result_bg->getContentSize().width/2-105,result_bg->getPositionY()-result_bg->getContentSize().height/2+18));
		this->addChild(changeMenu,7);
	}

	if(_delegate->_isDuelMode){
		Label* rematchLabel=Label::createWithSystemFont("REMATCH",FONT_TYPE,11);
		rematchLabel->setColor(ccc3(255,224,80));
		MenuItemLabel* rematchBtn=MenuItemLabel::create(rematchLabel,CC_CALLBACK_1(GameOver::onRematch, this));
		Menu* rematchMenu=Menu::create(rematchBtn,nullptr);
		rematchMenu->setPosition(Vec2(result_bg->getPositionX()+result_bg->getContentSize().width/2-45,result_bg->getPositionY()-result_bg->getContentSize().height/2+18));
		this->addChild(rematchMenu,7);
	}

	_delegate->_isSurrender=false;

}
void GameOver::onRematch(Ref* sender){

	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
	if(_delegate){
		_delegate->onRematch();
	}

}
void GameOver::onChangeChar(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
	if(changeCharLayer){
		changeCharLayer->removeFromParentAndCleanup(true);
		changeCharLayer=nullptr;
	}
	if(!_delegate || !_delegate->currentPlayer){
		return;
	}
	if(changePlayerPick.empty()){
		changePlayerPick=_delegate->currentPlayer->getCharacter()->getCString();
	}
	if(changeEnemyPick.empty()){
		Ref* pObject=nullptr;
		for(unsigned int characterIndex=0; characterIndex<_delegate->_CharacterArray->count(); ++characterIndex){
		pObject=_delegate->_CharacterArray->objectAtIndex(characterIndex);
			NarutoSenki::ActionManager* hero=(NarutoSenki::ActionManager*)pObject;
			if(strcmp(hero->getRole()->getCString(),"Com")==0 && strcmp(hero->getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0){
				changeEnemyPick=hero->getCharacter()->getCString();
				break;
			}
		}
	}

	changeCharLayer=Layer::create();
	LayerColor* blend=LayerColor::create(Color4B(0,0,0,150),winSize.width,winSize.height);
	changeCharLayer->addChild(blend,0);
	cocos2d::ui::Scale9Sprite* bg=cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("confirm_bg.png");
	if(bg){
		bg->setContentSize(Size(336,236));
		bg->setPosition(Vec2(winSize.width/2,winSize.height/2));
		changeCharLayer->addChild(bg,1);
	}
	float cx=winSize.width/2;
	float cy=winSize.height/2;
	Label* title=Label::createWithSystemFont("CHANGE CHARACTER",FONT_TYPE,14);
	title->setColor(ccc3(126,58,30));
	title->setPosition(Vec2(cx,cy+96));
	changeCharLayer->addChild(title,2);

	Menu* targetMenu=nullptr;
	if(_delegate->_isDuelMode){
		Label* playerLabel=Label::createWithSystemFont("PLAYER",FONT_TYPE,changeTargetEnemy?11:13);
		playerLabel->setColor(changeTargetEnemy?ccc3(86,70,52):ccc3(142,63,28));
		MenuItemLabel* playerItem=MenuItemLabel::create(playerLabel,CC_CALLBACK_1(GameOver::onChangeCharTarget, this));
		playerItem->setTag(0);
		Label* enemyLabel=Label::createWithSystemFont("ENEMY",FONT_TYPE,changeTargetEnemy?13:11);
		enemyLabel->setColor(changeTargetEnemy?ccc3(142,63,28):ccc3(86,70,52));
		MenuItemLabel* enemyItem=MenuItemLabel::create(enemyLabel,CC_CALLBACK_1(GameOver::onChangeCharTarget, this));
		enemyItem->setTag(1);
		targetMenu=Menu::create(playerItem,enemyItem,nullptr);
		targetMenu->alignItemsHorizontallyWithPadding(40);
		targetMenu->setPosition(Vec2(cx,cy+72));
		changeCharLayer->addChild(targetMenu,2);
	}else{
		changeTargetEnemy=false;
		Label* playerOnly=Label::createWithSystemFont("PLAYER",FONT_TYPE,13);
		playerOnly->setColor(ccc3(142,63,28));
		playerOnly->setPosition(Vec2(cx,cy+72));
		changeCharLayer->addChild(playerOnly,2);
	}

	Size viewSize=Size(286,122);
	int heroCount=sizeof(gResultHeroList)/sizeof(char*);
	int columnCount=5;
	int rowCount=(heroCount+columnCount-1)/columnCount;
	float cellW=56;
	float cellH=50;
	float contentH=rowCount*cellH+8;
	Layer* heroContainer=Layer::create();
	heroContainer->setContentSize(Size(viewSize.width,contentH));
	Menu* heroMenu=Menu::create(nullptr);
	heroMenu->setPosition(Vec2::ZERO);
	heroContainer->addChild(heroMenu,1);
	for(int i=0;i<heroCount;i++){
		const char* heroName=gResultHeroList[i];
		std::string rpPath=cocos2d::StringUtils::format("%s_rp.png",heroName);
		std::string smallPath=cocos2d::StringUtils::format("%s_small.png",heroName);
		Sprite* normal=Sprite::createWithSpriteFrameName(rpPath);
		if(!normal){
			normal=Sprite::createWithSpriteFrameName(smallPath);
		}
		Sprite* selected=Sprite::createWithSpriteFrameName(rpPath);
		if(!selected){
			selected=Sprite::createWithSpriteFrameName(smallPath);
		}
		if(!normal || !selected){
			continue;
		}
		normal->setScale(0.72f);
		selected->setScale(0.82f);
		MenuItemSprite* item=MenuItemSprite::create(normal,selected,CC_CALLBACK_1(GameOver::onChangeCharPick, this));
		item->setTag(i);
		int col=i%columnCount;
		int row=i/columnCount;
		item->setPosition(Vec2(30+col*cellW,contentH-30-row*cellH));
		heroMenu->addChild(item);
	}
	ScrollView* heroScroll=ScrollView::create(viewSize,heroContainer);
	heroScroll->setDirection(ScrollView::Direction::VERTICAL);
	heroScroll->setClippingToBounds(true);
	heroScroll->setPosition(Vec2(cx-viewSize.width/2,cy-58));
	heroScroll->setContentOffset(Vec2(0,viewSize.height-contentH));
	changeCharLayer->addChild(heroScroll,2);

	std::string pickText=cocos2d::StringUtils::format("%s: %s",changeTargetEnemy?"ENEMY":"PLAYER",changeTargetEnemy && !changeEnemyPick.empty()?changeEnemyPick.c_str():changePlayerPick.c_str());
	Label* pickLabel=Label::createWithSystemFont(pickText,FONT_TYPE,10);
	pickLabel->setColor(ccc3(72,55,40));
	pickLabel->setPosition(Vec2(cx,cy-76));
	changeCharLayer->addChild(pickLabel,2);

	MenuItem* yesBtn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(GameOver::onChangeCharConfirm, this));
	MenuItem* noBtn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(GameOver::onChangeCharCancel, this));
	yesBtn->setScale(0.82f);
	noBtn->setScale(0.82f);
	Menu* confirmMenu=Menu::create(yesBtn,noBtn,nullptr);
	confirmMenu->alignItemsHorizontallyWithPadding(26);
	confirmMenu->setPosition(Vec2(cx,cy-102));
	changeCharLayer->addChild(confirmMenu,3);
	this->addChild(changeCharLayer,800);
}

void GameOver::onChangeCharTarget(Ref* sender){
	changeTargetEnemy=((Node*)sender)->getTag()==1;
	onChangeChar(sender);
}

void GameOver::onChangeCharPick(Ref* sender){
	int index=((Node*)sender)->getTag();
	int heroCount=sizeof(gResultHeroList)/sizeof(char*);
	if(index<0 || index>=heroCount){
		return;
	}
	if(changeTargetEnemy && _delegate && _delegate->_isDuelMode){
		changeEnemyPick=gResultHeroList[index];
	}else{
		changePlayerPick=gResultHeroList[index];
	}
	onChangeChar(sender);
}

void GameOver::onChangeCharConfirm(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
	if(!_delegate || changePlayerPick.empty()){
		return;
	}
	CCArray* rematchHeros=CCArray::create();
	Ref* pObject=nullptr;
	for(unsigned int characterIndex=0; characterIndex<_delegate->_CharacterArray->count(); ++characterIndex){
		pObject=_delegate->_CharacterArray->objectAtIndex(characterIndex);
		NarutoSenki::ActionManager* hero=(NarutoSenki::ActionManager*)pObject;
		if(strcmp(hero->getRole()->getCString(),"Player")!=0 && strcmp(hero->getRole()->getCString(),"Com")!=0){
			continue;
		}
		if(strcmp(hero->getCharacter()->getCString(),"Roshi")==0 || strcmp(hero->getCharacter()->getCString(),"Han")==0){
			continue;
		}
		const char* character=hero->getCharacter()->getCString();
		if(strcmp(hero->getRole()->getCString(),"Player")==0){
			character=changePlayerPick.c_str();
		}else if(_delegate->_isDuelMode && !changeEnemyPick.empty() && strcmp(hero->getGroup()->getCString(),_delegate->currentPlayer->getGroup()->getCString())!=0){
			character=changeEnemyPick.c_str();
		}
		resultAddHero(rematchHeros,character,hero->getRole()->getCString(),hero->getGroup()->getCString());
	}
	rematchHeros->retain();
	CC_SAFE_RELEASE(_delegate->_rematchHerosOverride);
	_delegate->_rematchHerosOverride=rematchHeros;
	_delegate->onRematch();
}

void GameOver::onChangeCharCancel(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
	if(changeCharLayer){
		changeCharLayer->removeFromParentAndCleanup(true);
		changeCharLayer=nullptr;
	}
}
void GameOver::onUPloadBtn(Ref* sender){

	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);

}

void GameOver::onHttpRequestCompleted(Node *sender ,void *data) {

	// Jalur upload legacy tidak dipakai (game offline); callback ini dipertahankan
	// untuk menjaga kontrak lama, tapi tidak menjalankan HTTP parsing lagi.
	(void)sender;
	(void)data;
	isPosting=false;
}


void GameOver::onBackToMenu(Ref* sender){

	if(!exitLayer){
		SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/select.mp3");
		exitLayer=Layer::create();

		Sprite* exit_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
		exit_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));

		Sprite* comfirm_title=Sprite::createWithSpriteFrameName("confirm_title.png");
		comfirm_title->setPosition(Vec2(winSize.width/2,winSize.height/2+38));

		Sprite* btm_text=Sprite::createWithSpriteFrameName("btm_text.png");
		btm_text->setPosition(Vec2(winSize.width/2,winSize.height/2+8));

		MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(GameOver::onLeft, this));
		MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(GameOver::onCancel, this));

		Menu* confirm_menu = Menu::create(yes_btn,no_btn,nullptr);
		confirm_menu->alignItemsHorizontallyWithPadding(24);
		confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-30));

		exitLayer->addChild(exit_bg,1);
		exitLayer->addChild(confirm_menu,2);
		exitLayer->addChild(comfirm_title,2);
		exitLayer->addChild(btm_text,2);
		this->addChild(exitLayer,500);
	}


};

void GameOver::onLeft(Ref* sender){

	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");



	_delegate->_isExiting=true;
	Director::getInstance()->popScene();

}

void GameOver::onCancel(Ref* sender){

	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
	exitLayer->removeFromParent();
	exitLayer=nullptr;
}



GameOver*  GameOver::create(RenderTexture* snapshoot){
	GameOver* pl=new  GameOver();
	if (pl && pl->init(snapshoot))
	{
		pl->autorelease();
		return pl;
	}
	else
	{
		delete pl;
		return nullptr;
	}
}








