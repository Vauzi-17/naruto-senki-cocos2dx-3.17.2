#include "RankingLayer.h"
#include <cstdlib>
#include <string>
#include "NetworkLayer.h"
USING_NS_CC_EXT;
using namespace CocosDenshion;


/*----------------------
init RankCharButton
----------------------*/
RankCharButton::RankCharButton(void)
{
}


RankCharButton::~RankCharButton(void)
{

}


bool RankCharButton::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());
		if(strcmp(szImage,"")!=0)
			this->initWithSpriteFrameName(szImage);


		this->setAnchorPoint(Vec2(0,0));

		bRet=true;

	}while(0);

	return bRet;

}

void RankCharButton::onEnter(){
	Sprite::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(RankCharButton::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(RankCharButton::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(RankCharButton::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
void RankCharButton::onExit(){
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	Sprite::onExit();
}

Rect RankCharButton::getRect(){
	Size size=this->getContentSize();
	return Rect(0,0, size.width, size.height);

}
bool  RankCharButton::containsTouchLocation(Touch *touch){

	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}


void RankCharButton::setBtnType(const char* buttonChar){
	_btnType=buttonChar;
}





bool RankCharButton::onTouchBegan(Touch* touch, Event* event){
	// touch area
	if (!this->containsTouchLocation(touch)){
		return false;
	}else{
		return true;
	}


}

void RankCharButton::onTouchMoved(Touch* touch, Event* event){
	// touch area

}

void RankCharButton::onTouchEnded(Touch* touch, Event* event){
	if(_delegate->_screwLayer1){
		_delegate->_screwLayer1->removeAllChildren();
		_delegate->_screwLayer1->setPositionY(154);
	}
	if(_delegate->day_btn){
		_delegate->day_btn->unselected();
	}
	if(_delegate->week_btn){
		_delegate->week_btn->unselected();
	}
	if(_delegate->month_btn){
		_delegate->month_btn->selected();
	}


	_delegate->postRequest(300,_btnType.c_str());
};



RankCharButton* RankCharButton::create( const char *szImage ){
	RankCharButton* mb=new RankCharButton();
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













RakingLayer::RakingLayer(void)
{
	_screwLayer1=NULL;
	isPosting=false;
	refreshBtn=NULL;
	ranking_layer=NULL;
	order_menu=NULL;
	postingCode=0;
	rankingDetail=NULL;
	side0_btn=NULL;
	isMemberSelect=false;
	isGroupSelect=false;
}


RakingLayer::~RakingLayer(void)
{

}

bool RakingLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!Layer::init());

		//?????
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ranking.plist");

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
		Action* cseq1=RepeatForever::create(Sequence::create(cmv1,cmv1->reverse(),nullptr));
		cloud_left->runAction(cseq1);

		Sprite* cloud_right=Sprite::createWithSpriteFrameName("cloud.png");
		cloud_right->setPosition(Vec2(winSize.width-cloud_right->getContentSize().width,15));
		cloud_right->setFlipY(true);
		cloud_right->setAnchorPoint(Vec2(0,0));
		this->addChild(cloud_right,2);

		ActionInterval* cmv2=MoveBy::create(1,Vec2(15,0));
		Action* cseq2=RepeatForever::create(Sequence::create(cmv2,cmv2->reverse(),nullptr));
		cloud_right->runAction(cseq2);

		Sprite* cloud_top=Sprite::createWithSpriteFrameName("cloud_top.png");
		cloud_top->setPosition(Vec2(winSize.width/2,winSize.height-(cloud_top->getContentSize().height+12)));
		cloud_top->setAnchorPoint(Vec2(0.5f,0));
		this->addChild(cloud_top,1);



		if(UserDefault::getInstance()->getBoolForKey("isBGM")!=false){
			SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/ranking_music.mp3",true);
		}


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



		MenuItem* start_bt=MenuItemSprite::create(Sprite::createWithSpriteFrameName("return_btn.png"),NULL,CC_CALLBACK_1(RakingLayer::onCancel, this));
		Menu* menu =Menu::create(start_bt,NULL);
		menu->setPosition(winSize.width-38,86);
		//menu->setPosition(winSize.width/2+skillSprite->getContentSize().width/2-10,winSize.height/2+skillSprite->getContentSize().height/2-12);
		this->addChild(menu,5);





		//???????


		ClippingNode* clipper = ClippingNode::create();
		Node *stencil = Sprite::createWithSpriteFrameName("screwMask2.png");
		stencil->setAnchorPoint(Vec2(0,0));
		clipper->setStencil(stencil);

		ranking_layer=Layer::create();

		ranking_bg=Sprite::createWithSpriteFrameName("ranking_bg.png");
		ranking_bg->setAnchorPoint(Vec2(0.5f,0.5f));
		ranking_bg->setPosition(Vec2(winSize.width/2,winSize.height/2-15));
		ranking_layer->addChild(ranking_bg,10);


		Sprite* ranking_logo=Sprite::createWithSpriteFrameName("ranking_logo.png");
		ranking_logo->setAnchorPoint(Vec2(0.5f,0.5f));
		ranking_logo->setPosition(Vec2(ranking_bg->getPositionX(),ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2+16));
		ranking_layer->addChild(ranking_logo,11);


		ranking_title=Sprite::createWithSpriteFrameName("ranking_title1.png");
		ranking_title->setAnchorPoint(Vec2(0,0));
		ranking_title->setPosition(Vec2(ranking_bg->getPositionX()-62,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-16));
		ranking_layer->addChild(ranking_title,15);

		_screwLayer1=CCScrewLayer::create();
		_screwLayer1->setAnchorPoint(Vec2(0,0));
		_screwLayer1->setPositionY(154);
		_screwLayer1->line_height=26;
		_screwLayer1->minY=154;

		clipper->setPosition(Vec2(ranking_bg->getPositionX()-ranking_bg->getContentSize().width/2,58));
		clipper->addChild(_screwLayer1,5);

		ranking_layer->addChild(clipper,20);




		side2_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("sidebar2_off.png"),Sprite::createWithSpriteFrameName("sidebar2_on.png"),CC_CALLBACK_1(RakingLayer::onSideBar2, this));
		side2_btn->setAnchorPoint(Vec2(0,0));
		Menu* side_menu2 = Menu::create(side2_btn,NULL);
		side_menu2->setPosition(Vec2(ranking_bg->getPositionX()-ranking_bg->getContentSize().width/2-40,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-35));
		ranking_layer->addChild(side_menu2,5);

		this->addChild(ranking_layer,100);
		if(GroupID){
				this->produceRanking(0);
		}else{
			this->produceRanking(2);
		}



		bRet=true;

	}while(0);

	return bRet;

}
void RakingLayer::onSideBar0(Ref* sender){
	this->produceRanking(0);
}
void RakingLayer::onSideBar1(Ref* sender){
	this->produceRanking(1);
}
void RakingLayer::onSideBar2(Ref* sender){
	this->produceRanking(2);
}
void RakingLayer::onSideBar3(Ref* sender){
	this->produceRanking(3);
}



void RakingLayer::produceRanking(int type){
	if(isPosting){
		return;
	}

	if(_screwLayer1){
		_screwLayer1->removeAllChildren();
		_screwLayer1->setPositionY(154);
	}



	if(order_menu){
		order_menu->removeAllChildren();
		order_menu=NULL;
	}

	if(rankingDetail){
		rankingDetail->removeFromParent();
		rankingDetail=NULL;
	}


	switch (type){

	case 0:
		if(ranking_title){
			SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("ranking_title1.png");
			ranking_title->setSpriteFrame(frame);
			ranking_title->setPosition(Vec2(ranking_bg->getPositionX()-62,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-16));
		}
		if(side0_btn){
			side0_btn->selected();
		}
		side1_btn->unselected();
		side2_btn->unselected();
		side3_btn->unselected();

		isMemberSelect=true;
		isGroupSelect=false;

		day_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_day_off.png"),Sprite::createWithSpriteFrameName("order_day_on.png"),CC_CALLBACK_1(RakingLayer::onDayBtn, this));
		week_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_week_off.png"),Sprite::createWithSpriteFrameName("order_week_on.png"),CC_CALLBACK_1(RakingLayer::onWeekBtn, this));
		month_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_month_off.png"),Sprite::createWithSpriteFrameName("order_month_on.png"),CC_CALLBACK_1(RakingLayer::onMonthBtn, this));
		order_menu = Menu::create(day_btn,week_btn,month_btn,NULL);
		order_menu->alignItemsHorizontallyWithPadding(20);
		order_menu->setPosition(Vec2(ranking_bg->getPositionX()+100,ranking_bg->getPositionY()-ranking_bg->getContentSize().height/2+14));
		ranking_layer->addChild(order_menu,10);
		day_btn->selected();

		this->postRequest(110);
		break;
	case 1:

		if(ranking_title){
			SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("ranking_title1.png");
			ranking_title->setSpriteFrame(frame);
			ranking_title->setPosition(Vec2(ranking_bg->getPositionX()-62,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-16));
		}
		if(side0_btn){
			side0_btn->unselected();
		}
		side1_btn->selected();
		side2_btn->unselected();
		side3_btn->unselected();

		isMemberSelect=false;
		isGroupSelect=false;

		day_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_day_off.png"),Sprite::createWithSpriteFrameName("order_day_on.png"),CC_CALLBACK_1(RakingLayer::onDayBtn, this));
		week_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_week_off.png"),Sprite::createWithSpriteFrameName("order_week_on.png"),CC_CALLBACK_1(RakingLayer::onWeekBtn, this));
		month_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_month_off.png"),Sprite::createWithSpriteFrameName("order_month_on.png"),CC_CALLBACK_1(RakingLayer::onMonthBtn, this));
		order_menu = Menu::create(day_btn,week_btn,month_btn,NULL);
		order_menu->alignItemsHorizontallyWithPadding(20);
		order_menu->setPosition(Vec2(ranking_bg->getPositionX()+100,ranking_bg->getPositionY()-ranking_bg->getContentSize().height/2+14));
		ranking_layer->addChild(order_menu,10);
		day_btn->selected();

		this->postRequest(100);

		break;


	case 2:
		if(side0_btn){
			side0_btn->unselected();
		}
		side2_btn->selected();
		//side1_btn->unselected();
		//side3_btn->unselected();

		if(ranking_title){
			SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("ranking_title2.png");
			ranking_title->setSpriteFrame(frame);
			ranking_title->setPosition(Vec2(ranking_bg->getPositionX()-96,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-16));
		}

		isMemberSelect=false;
		isGroupSelect=false;

		JifenPoint_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_jifen_off.png"),Sprite::createWithSpriteFrameName("order_jifen_on.png"),CC_CALLBACK_1(RakingLayer::onJifenBtn, this));
		battlePoint_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_battle_off.png"),Sprite::createWithSpriteFrameName("order_battle_on.png"),CC_CALLBACK_1(RakingLayer::onBattleBtn, this));
		order_menu = Menu::create(JifenPoint_btn,battlePoint_btn,NULL);
		order_menu->alignItemsHorizontallyWithPadding(20);
		order_menu->setPosition(Vec2(ranking_bg->getPositionX()+100,ranking_bg->getPositionY()-ranking_bg->getContentSize().height/2+14));
		ranking_layer->addChild(order_menu,10);
		JifenPoint_btn->selected();

		this->postRequest(400);

		break;

	case 3:
		if(side0_btn){
			side0_btn->unselected();
		}
		side3_btn->selected();
		side1_btn->unselected();
		side2_btn->unselected();
		if(ranking_title){
			SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("ranking_title3.png");
			ranking_title->setSpriteFrame(frame);
			ranking_title->setPosition(Vec2(ranking_bg->getPositionX()-106,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-16));
		}
		isMemberSelect=false;
		isGroupSelect=true;

		JifenPoint_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_jifen_off.png"),Sprite::createWithSpriteFrameName("order_jifen_on.png"),CC_CALLBACK_1(RakingLayer::onJifenBtn, this));
		battlePoint_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("order_battle_off.png"),Sprite::createWithSpriteFrameName("order_battle_on.png"),CC_CALLBACK_1(RakingLayer::onBattleBtn, this));
		order_menu = Menu::create(JifenPoint_btn,battlePoint_btn,NULL);
		order_menu->alignItemsHorizontallyWithPadding(20);
		order_menu->setPosition(Vec2(ranking_bg->getPositionX()+100,ranking_bg->getPositionY()-ranking_bg->getContentSize().height/2+14));
		ranking_layer->addChild(order_menu,10);
		JifenPoint_btn->selected();

		this->postRequest(600);


		break;


	}
}

void RakingLayer::postRequest(int code,const char* useChar/* =NULL */){

	if(rankingDetail){
		rankingDetail->removeFromParent();
		rankingDetail=NULL;
	}
	postingCode= code;
	rapidjson::Document doc;
	ssize_t size;
	const char* data;
	if(postingCode==400){
		data=(const char*)FileUtils::getInstance()->getFileData("rank.json", "r",&size);
	}else {
		data=(const char*)FileUtils::getInstance()->getFileData("rank2.json", "r",&size);
	}

	std::string clearData(data);
	size_t pos = clearData.rfind("]");
	clearData = clearData.substr(0, pos+1);

	doc.Parse<kParseDefaultFlags>(clearData.c_str());


	rapidjson::Value &arr=doc;
	int countRow=0;

	if(arr.IsArray()){

		for (SizeType i = 0; i < arr.Size(); i++)  {


			rapidjson::Value & node=arr[i];

			if(i==0){
				//??
				rapidjson::Value & sortValue= node["newid"];
				int sortNO=sortValue.GetInt();


				if(sortNO==0){

					CCDictionary *strings = CCDictionary::createWithContentsOfFile("Element/strings.xml");
					const char *reply;
					if(postingCode==400){
						reply= ((CCString*)strings->objectForKey("RankingNull"))->getCString();
					}else{
						reply= ((CCString*)strings->objectForKey("RankingNull2"))->getCString();
					}

					rankingDetail=Label::createWithSystemFont(reply,FONT_TYPE,12);
					rankingDetail->setAnchorPoint(Vec2(0,0.5f));
					rankingDetail->setPosition(Vec2(ranking_bg->getPositionX()-ranking_bg->getContentSize().width/2 +20,ranking_bg->getPositionY()-ranking_bg->getContentSize().height/2+14));
					ranking_layer->addChild(rankingDetail,500);

				}else{
					CCDictionary *strings = CCDictionary::createWithContentsOfFile("Element/strings.xml");
					const char *reply = ((CCString*)strings->objectForKey("RankingDetail"))->getCString();
					const char *reply2 = ((CCString*)strings->objectForKey("RankingDetail2"))->getCString();
					std::string detailText=cocos2d::StringUtils::format("%s %d %s",reply,sortNO,reply2);
					rankingDetail=Label::createWithSystemFont(detailText,FONT_TYPE,12);
					rankingDetail->setAnchorPoint(Vec2(0,0.5f));
					rankingDetail->setPosition(Vec2(ranking_bg->getPositionX()-ranking_bg->getContentSize().width/2 +20,ranking_bg->getPositionY()-ranking_bg->getContentSize().height/2+14));
					ranking_layer->addChild(rankingDetail,500);
				}

			}else{


				rapidjson::Value & idValue= node["id"];
				std::string idString=idValue.GetString();
				int memberID=std::atoi(idString.c_str());


				rapidjson::Value & nameValue= node["name"];
				std::string name=nameValue.GetString();

				rapidjson::Value & npValue= node["np_id"];
				std::string npString=npValue.GetString();

				rapidjson::Value & groupNameValue= node["group_name"];
				std::string groupName;
				int groupID=0;
				if(groupNameValue.IsNull()){
					groupName="";
				}else{
					groupName=groupNameValue.GetString();

					if(postingCode==400 || postingCode==500){
						rapidjson::Value & gidValue= node["group_id"];
						std::string groupString=gidValue.GetString();
						groupID=std::atoi(groupString.c_str());
					}


				}




				int score;
				rapidjson::Value & bpointValue= node["battle_point"];
				std::string bpointString=bpointValue.GetString();
				int bpoint=atof(bpointString.c_str())*100;

				rapidjson::Value & pointValue= node["point"];
				std::string pointString=pointValue.GetString();
				int point=atof(pointString.c_str());

				if(postingCode==500){
					score=bpoint;
				}else if(postingCode==400) {
					score=point;
				}else if(postingCode==600){
					score=point;
				}else if(postingCode==700){
					score=bpoint;
				}else if(postingCode==800){
					score=point;
				}else if(postingCode==900){
					score=bpoint;
				}


				CCScrewItem* screwItem=CCScrewItem::create();
				if(i%2!=0){
					Sprite* line_bg=Sprite::createWithSpriteFrameName("ranking_line.png");
					line_bg->setAnchorPoint(Vec2(0,0));
					screwItem->addChild(line_bg);
				}

				if(i==1){
					Sprite* num_icon;
					if(postingCode==500){
						num_icon=Sprite::createWithSpriteFrameName("num_top3.png");
					}else if(postingCode==400) {
						num_icon=Sprite::createWithSpriteFrameName("num_top2.png");
					}else{
						num_icon=Sprite::createWithSpriteFrameName("num_top4.png");
					}
					num_icon->setAnchorPoint(Vec2(0,0));
					num_icon->setPosition(Vec2(1,1));
					screwItem->addChild(num_icon);
				}else if(i==2){
					Sprite* num_icon=Sprite::createWithSpriteFrameName("num_2.png");
					num_icon->setAnchorPoint(Vec2(0,0));
					num_icon->setPosition(Vec2(1,1));
					screwItem->addChild(num_icon);
				}else if(i==3){
					Sprite* num_icon=Sprite::createWithSpriteFrameName("num_3.png");
					num_icon->setAnchorPoint(Vec2(0,0));
					num_icon->setPosition(Vec2(1,1));
					screwItem->addChild(num_icon);
				}else{
					std::string rankText=cocos2d::StringUtils::format("%d",i);
					Label* numLabel=Label::createWithSystemFont(rankText,"????",12);
					numLabel->setPosition(Vec2(13,13));
					screwItem->addChild(numLabel);
				}

				if(strcmp(npString.c_str(),"")!=0){

					std::string nameplatePath=cocos2d::StringUtils::format("%s.png",npString.c_str());
					Sprite* nameplate=Sprite::createWithSpriteFrameName(nameplatePath);
					nameplate->setAnchorPoint(Vec2(0.5f,0));
					nameplate->setPosition(Vec2(106,0));
					screwItem->addChild(nameplate);

					CCStrokeLabel* user_nameStr;
					Label* user_name=Label::createWithSystemFont(name.c_str(),FONT_TYPE,12);
					if(MemberID){
						if(MemberID==memberID){
							user_nameStr = CCStrokeLabel::create(user_name,ccc3(0,0,0),ccc3(236,162,8),1.0f);
						}else{
							user_nameStr = CCStrokeLabel::create(user_name,ccc3(0,0,0),ccc3(255,255,255),1.0f);
						}
					}else{
						user_nameStr = CCStrokeLabel::create(user_name,ccc3(0,0,0),ccc3(255,255,255),1.0f);
					}

					user_nameStr->setPosition(Vec2(106,13));
					screwItem->addChild(user_nameStr);

					bool isBlink=false;
					const char* bannerPath;
					if(strcmp(npString.c_str(),"np000")==0 ||
						strcmp(npString.c_str(),"np001")==0 ||
						point>=99999){
							bannerPath="banner_black";
							isBlink=true;
					}else if(point>=48000){
						bannerPath="banner_blue";
						isBlink=true;
					}else if(point>=24000){
						bannerPath="banner_red";
						isBlink=true;
					}else if(point>=16000){
						bannerPath="banner_gold";
						isBlink=true;
					}else if(point>=12000){
						bannerPath="banner_gold";
					}else if(point>=6000){
						bannerPath="banner_silver";
					}else{
						bannerPath="banner_green";
					}

					std::string bannerFramePath=cocos2d::StringUtils::format("%s.png",bannerPath);
					Sprite* nameBaner=Sprite::createWithSpriteFrameName(bannerFramePath);
					nameBaner->setAnchorPoint(Vec2(0.5f,0));
					nameBaner->setPosition(Vec2(106,0));
					screwItem->addChild(nameBaner);

					if(isBlink){

						Sprite* blink=Sprite::createWithSpriteFrameName("banner_blink5.png");
						blink->setAnchorPoint(Vec2(0.5f,0));
						blink->setPosition(Vec2(106,0));

						screwItem->addChild(blink);


						Vector<SpriteFrame*> tempArray;

						int i=1;

						while (i<7)
						{
							std::string path=cocos2d::StringUtils::format("banner_blink%d.png",i);
							SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(path);
							tempArray.pushBack(frame);


							i+=1;
						}

						Animation* tempAnimation= Animation::createWithSpriteFrames(tempArray, 0.1f);
						FiniteTimeAction* tempAction=Animate::create(tempAnimation);
						DelayTime* delay=DelayTime::create(1.0f);

						blink->runAction(RepeatForever::create(Sequence::create(tempAction,delay,nullptr)));
					}

				}else{
					Label* user_name=Label::createWithSystemFont(name.c_str(),FONT_TYPE,12);

					if(MemberID){
						if(MemberID==memberID){
							user_name->setColor(ccc3(236,162,8));
						}
					}

					user_name->setAnchorPoint(Vec2(0.5f,0.5));
					user_name->setPosition(Vec2(106,13));
					screwItem->addChild(user_name);

				}



				std::string scoreText=cocos2d::StringUtils::format("%d",score);
				Label* user_score=Label::createWithSystemFont(scoreText,FONT_TYPE,12);

				user_score->setAnchorPoint(Vec2(0.5f,0));
				user_score->setPosition(Vec2(214,5));
				screwItem->addChild(user_score);


				if(groupID!=0){
					Label* group_Name=Label::createWithSystemFont(groupName,FONT_TYPE,12);

					MenuItemLabel* item=MenuItemLabel::create(group_Name, CC_CALLBACK_1(RakingLayer::onSelectGroupBtn, this));

					Menu* pMenu = Menu::create(item, NULL);
					pMenu ->setPosition(Vec2(304,5));
					item ->setAnchorPoint(Vec2(0.5f,0));
					item->setTag(groupID);
					screwItem->addChild(pMenu);
					screwItem->setTag(groupID);

				}else if(strcmp(groupName.c_str(),"")!=0 ){
					Label* group_Name=Label::createWithSystemFont(groupName,FONT_TYPE,12);

					group_Name->setAnchorPoint(Vec2(0.5f,0));
					group_Name->setPosition(Vec2(304,5));
					screwItem->addChild(group_Name);


				}

				int row=i;
				screwItem->setPosition(Vec2(0,-row*26+26));
				screwItem->setAnchorPoint(Vec2(0,0));

				_screwLayer1->addChild(screwItem,10);

				countRow++;

			}
		}
		_screwLayer1->totalRow=countRow;

	}

}



void RakingLayer::onCancel(Ref* sender){

	this->removeFromParent();
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");

	Director::getInstance()->popScene();
}

void  RakingLayer::onDayBtn(Ref* sender){

	if(_screwLayer1){
		_screwLayer1->removeAllChildren();
		_screwLayer1->setPositionY(154);
	}

	if(isMemberSelect){
		this->postRequest(110);
	}else{
		this->postRequest(100);
	}



	day_btn->selected();
	week_btn->unselected();
	month_btn->unselected();
}

void  RakingLayer::onWeekBtn(Ref* sender){
	if(_screwLayer1){
		_screwLayer1->removeAllChildren();
	}
	_screwLayer1->setPositionY(154);
	if(isMemberSelect){
		this->postRequest(210);
	}else{
		this->postRequest(200);
	}

	day_btn->unselected();
	month_btn->unselected();
	week_btn->selected();

}



void  RakingLayer::onMonthBtn(Ref* sender){

	if(_screwLayer1){
		_screwLayer1->removeAllChildren();
		_screwLayer1->setPositionY(154);
	}

	if(isMemberSelect){
		this->postRequest(310);
	}else{
		this->makeCharList();
	}

	day_btn->unselected();
	week_btn->unselected();
	month_btn->selected();
}

void  RakingLayer::onJifenBtn(Ref* sender){

	if(_screwLayer1){
		_screwLayer1->removeAllChildren();
		_screwLayer1->setPositionY(154);
	}

	if(isGroupSelect){
		if(ranking_title){
			SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("ranking_title3.png");
			ranking_title->setSpriteFrame(frame);
			ranking_title->setPosition(Vec2(ranking_bg->getPositionX()-106,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-16));
		}
		this->postRequest(600);
	}else{
		this->postRequest(400);
	}


	battlePoint_btn->unselected();
	JifenPoint_btn->selected();

}
void  RakingLayer::onBattleBtn(Ref* sender){

	if(_screwLayer1){
		_screwLayer1->removeAllChildren();
		_screwLayer1->setPositionY(154);
	}

	if(isGroupSelect){
		if(ranking_title){
			SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("ranking_title3.png");
			ranking_title->setSpriteFrame(frame);
			ranking_title->setPosition(Vec2(ranking_bg->getPositionX()-106,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-16));
		}
		this->postRequest(700);
	}else{
	this->postRequest(500);
	}
	JifenPoint_btn->unselected();
	battlePoint_btn->selected();

}


void RakingLayer::makeCharList(){
	if(rankingDetail){
		rankingDetail->removeFromParent();
		rankingDetail=NULL;
	}
	char* heroList[]={"Naruto","Sakura","Sai","Kakashi",
		"Lee","Tenten","Neji",
		"Shikamaru","Ino","Choji","Asuma",
		"Shino","Hinata","Kiba",
		"Tobirama","Hiruzen","Minato",
		"Jiraiya","Tsunade","Gaara","Kankuro",
		"Chiyo",
		"Sasuke","Karin","Suigetsu","Jugo",
		"Deidara","Tobi","Hidan","Kakuzu",
		"Konan","Itachi","Kisame"
	};

	int num= sizeof(heroList)/sizeof(char*);

	for(int i=0;i<num;i++){

		const char* charName=heroList[i];
		std::string path=cocos2d::StringUtils::format("%s_small.png",charName);


		RankCharButton* char_btn=RankCharButton::create(path.c_str());
		char_btn->setDelegate(this);
		char_btn->setBtnType(charName);
		char_btn->setAnchorPoint(Vec2(0,0));
		int row=int(floor(float(i)/8));
		int column=i-8*row;

		char_btn->setPosition(Vec2(10+column*46,-10-row*46));
		_screwLayer1->line_height=56;
		_screwLayer1->addChild(char_btn);
		_screwLayer1->totalRow=row;

	};



}



void RakingLayer::onSelectGroupBtn(Ref* sender){
	if(_screwLayer1){
		_screwLayer1->setPositionY(154);
	}

	if(rankingDetail){
		rankingDetail->removeFromParent();
		rankingDetail=NULL;
	}

	if(ranking_title){
		SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName("ranking_title2.png");
		ranking_title->setSpriteFrame(frame);
		ranking_title->setPosition(Vec2(ranking_bg->getPositionX()-96,ranking_bg->getPositionY()+ranking_bg->getContentSize().height/2-16));
	}

	CCDictionary *strings = CCDictionary::createWithContentsOfFile("Element/strings.xml");
	const char *reply = ((CCString*)strings->objectForKey("RankingNull2"))->getCString();
	rankingDetail=Label::createWithSystemFont(reply,FONT_TYPE,12);
	rankingDetail->setAnchorPoint(Vec2(0,0.5f));
	rankingDetail->setPosition(Vec2(ranking_bg->getPositionX()-ranking_bg->getContentSize().width/2 +20,ranking_bg->getPositionY()-ranking_bg->getContentSize().height/2+14));
	ranking_layer->addChild(rankingDetail,500);


}




