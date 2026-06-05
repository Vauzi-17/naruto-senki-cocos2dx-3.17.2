#include "StartMenu.h"
#include <cstdlib>
#include <string>
#include <vector>
using namespace CocosDenshion;
USING_NS_CC_EXT;



int Cheats=0;
int MemberID=NULL;
int GroupID=NULL;
std::string PWord="";
std::string CValue="999999";
bool isFirst=false;
bool isPlayed=true;

static const char* gTrainingHeroList[] = {
	"Konan","Sakura","Naruto","Sai","Deidara",
	"Kakashi","Itachi","Tenten","Jiraiya","Suigetsu",
	"Tsunade","Tobirama","Shisui","Neji","Ino","Asuma","Gaara",
	"Karin","Sasuke","Hidan","Choji","Kankuro",
	"Shino","Minato","Tobi","Kakuzu","Hinata",
	"Shikamaru","Chiyo","Kisame",
	"Hiruzen","Kiba","Jugo","Lee"
};

static bool trainingContainsHero(const std::vector<std::string>& used, const char* heroName)
{
	if(!heroName){
		return false;
	}
	for(const std::string& value : used)
	{
		if (value == heroName)
		{
			return true;
		}
	}
	return false;
}

static const char* trainingPickHero(std::vector<std::string>& used, const char** presetList, int presetCount, int presetIndex, bool randomPick)
{
	int heroCount = sizeof(gTrainingHeroList) / sizeof(char*);
	if (!randomPick && presetIndex < presetCount && !trainingContainsHero(used, presetList[presetIndex]))
	{
		used.push_back(presetList[presetIndex]);
		return presetList[presetIndex];
	}

	for (int attempt = 0; attempt < heroCount * 2; attempt++)
	{
		int index = random(heroCount);
		const char* heroName = gTrainingHeroList[index];
		if (!trainingContainsHero(used, heroName))
		{
			used.push_back(heroName);
			return heroName;
		}
	}

	for (int i = 0; i < heroCount; i++)
	{
		if (!trainingContainsHero(used, gTrainingHeroList[i]))
		{
			used.push_back(gTrainingHeroList[i]);
			return gTrainingHeroList[i];
		}
	}

	return gTrainingHeroList[0];
}

static void trainingAddHero(CCArray* heros, const char* character, const char* role, const char* group)
{
	CCDictionary* dic = CCDictionary::create();
	dic->setObject(CCString::create(character), "character");
	dic->setObject(CCString::create(role), "role");
	dic->setObject(CCString::create(group), "group");
	heros->addObject(dic);
}

static MenuItemLabel* trainingMenuItem(const char* text, const ccMenuCallback& callback, int tag, bool selected)
{
	Label* label = Label::createWithSystemFont(text, FONT_TYPE, selected ? 15 : 13);
	label->setColor(selected ? Color3B(150, 72, 28) : Color3B(82, 67, 50));
	MenuItemLabel* item = MenuItemLabel::create(label, callback);
	item->setTag(tag);
	return item;
}

static Label* trainingShadowLabel(Layer* layer, const char* text, int fontSize, const Color3B& color, const Vec2& position, int z)
{
	Label* shadow = Label::createWithSystemFont(text, FONT_TYPE, fontSize);
	shadow->setColor(Color3B(35, 24, 16));
	shadow->setOpacity(150);
	shadow->setPosition(Vec2(position.x + 1, position.y - 1));
	layer->addChild(shadow, z);

	Label* label = Label::createWithSystemFont(text, FONT_TYPE, fontSize);
	label->setColor(color);
	label->setPosition(position);
	layer->addChild(label, z + 1);
	return label;
}

static Label* trainingPlainLabel(Layer* layer, const char* text, int fontSize, const Color3B& color, const Vec2& position, int z)
{
	Label* label = Label::createWithSystemFont(text, FONT_TYPE, fontSize);
	label->setColor(color);
	label->setAnchorPoint(Vec2(0, 0.5f));
	label->setPosition(position);
	layer->addChild(label, z);
	return label;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

extern "C"
{
	void Java_net_zakume_game_NarutoSenki_onBannerInit( JNIEnv *env,jobject thiz,jint points)
	{
		adResult=points;

	}
}
#endif


/*----------------------
init MenuButton ;
----------------------*/
MenuButton::MenuButton(void)
{
	_isTop=false;
	_isBottom=false;
}


MenuButton::~MenuButton(void)
{

}


bool MenuButton::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());
		this->initWithSpriteFrameName(szImage);
		this->setAnchorPoint(Vec2(0.5,0));

		bRet=true;

	}while(0);

	return bRet;

}

void MenuButton::onEnter(){
	Sprite::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(MenuButton::ccTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MenuButton::ccTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MenuButton::ccTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

//#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
//
//	JniMethodInfo minfo;
//
//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/NarutoSenki","showBanner", "()V");
//	if (isHave) {
//		minfo.env -> CallStaticVoidMethod(minfo.classID,minfo.methodID);
//	}
//
//#endif


}
void MenuButton::onExit(){
	Sprite::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

Rect MenuButton::getRect(){
	Size size=this->getContentSize();
	return Rect(0,0, size.width, size.height);

}
bool MenuButton::containsTouchLocation(Touch *touch){

	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}


void MenuButton::setBtnType(btnType type){
	_btnType=type;
}

btnType	MenuButton::getBtnType(){
	return _btnType;
}

bool MenuButton::ccTouchBegan(Touch* touch,Event* event){
	// touch area
	if (!this->containsTouchLocation(touch))
		return false;

	//click();

	prePosY=NULL;

	return true;

}

void MenuButton::ccTouchMoved(Touch* touch,Event* event){
	// touch area
	if(prePosY==NULL){
		prePosY=touch->getLocation().y;

	}else{
		if(this->getBtnType()!=HardCore){

			if(abs(touch->getLocation().y-prePosY)>16){
				if(touch->getLocation().y<prePosY){
					_delegate->isClockwise=true;
				}else{
					_delegate->isClockwise=false;
				}
				_delegate->isDrag=true;
			}


		}

	}



}

void MenuButton::ccTouchEnded(Touch* touch,Event* event){
	if(_isTop && !_delegate->isDrag){

		switch(_btnType){
		case Training:
				SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
			_delegate->onTrainingCallBack();
			break;
		case Credits:
				SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
			_delegate->onCreditsCallBack();
			break;
		case Exit:
				SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
			_delegate->onExitCallBack();
			break;
		case Network:
			SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
			_delegate->onHardCoreCallBack();
			break;;
		case HardCore:
			/*SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
			SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName("menu05_text.png");
			_delegate->menuText->setSpriteFrame(frame);

			if( UserDefault::getInstance()->getBoolForKey("isHardCore")==true){
			UserDefault::getInstance()->setBoolForKey("isHardCore",false);
			if(_delegate->hardCore_btn){
			SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName("menu02_2.png");
			_delegate->hardCore_btn->setSpriteFrame(frame);
			}
			}else{
			_delegate->onHardLayerCallBack();
			}*/


			break;
		}
	}else{
		SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
		prePosY=NULL;
		_delegate->scrollMenu(this->getPositionY());
		_delegate->isDrag=false;
	}

};



void MenuButton::playSound(){

	SimpleAudioEngine::getInstance()->stopAllEffects();

	switch(_btnType){
	case Training:
		SimpleAudioEngine::getInstance()->playEffect(TRAINING_SOUND);
		/*if(this->getBtnType()==Training){
		_delegate->hardCore_btn->setVisible(true);
		}*/
		break;
	case Network:
		SimpleAudioEngine::getInstance()->playEffect(NETWORK_SOUND);
		break;
	case Credits:
		SimpleAudioEngine::getInstance()->playEffect(CREDITS_SOUND);
		break;
	case Exit:
		SimpleAudioEngine::getInstance()->playEffect(EXIT_SOUND);
		break;
	}

}

MenuButton* MenuButton::create( const char *szImage ){
	MenuButton* mb=new MenuButton();
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



/*----------------------
init StartMenu layer;
----------------------*/

StartMenu::StartMenu(void)
{
	isClockwise=false;
	isDrag=false;
	isNewAviable=false;
	hardCore_btn=NULL;
	hardCoreLayer=NULL;
	isBondAviable=false;
	isTasking=false;
	isPosting=false;
	input_layer=NULL;
	notice_layer=NULL;
	profile_layer=NULL;
	refreshBtn=NULL;
	noticeLabel=NULL;
	task_btn=NULL;
	login_btn=NULL;
	group_layer=NULL;
	leaderID=0;
	maxGroupNum=5;
	totalGroupNum=NULL;
	lock_btn=NULL;
	 isLock=false;
	notice_label=NULL;
	_screwLayer1=NULL;
	trainingBattleSize=3;
	trainingRandomTeam=false;
	trainingRandomEnemy=true;
	trainingModeOnly=false;
	trainingKillLimit=3;
}


StartMenu::~StartMenu(void)
{

}

bool StartMenu::init()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(!Layer::init());



		SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);

		//Vec2 origin = Director::getInstance()->getVisibleOrigin();


		//Sprite* bgSprite = Sprite::create("red_bg.png");
		////pSprite->setPosition(Vec2(winSize.width/2 + origin.x, winSize.height/2 + origin.y));
		//bgSprite->setAnchorPoint(Vec2(0,0));
		//bgSprite->setPosition(Vec2(0,0));
		//this->addChild(bgSprite, -5);


		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Result.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("NamePlate.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Element/Naruto/Naruto.plist");
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
		this->addChild(cloud_left,1);

		ActionInterval* cmv1=MoveBy::create(1,Vec2(-15,0));
		Action* cseq1=RepeatForever::create(Sequence::create(cmv1,cmv1->reverse(),nullptr));
		cloud_left->runAction(cseq1);

		Sprite* cloud_right=Sprite::createWithSpriteFrameName("cloud.png");
		cloud_right->setPosition(Vec2(winSize.width-cloud_right->getContentSize().width,
			winSize.height-(cloud_right->getContentSize().height+15)));
		cloud_right->setAnchorPoint(Vec2(0,0));
		this->addChild(cloud_right,1);

		ActionInterval* cmv2=MoveBy::create(1,Vec2(15,0));
		Action* cseq2=RepeatForever::create(Sequence::create(cmv2,cmv2->reverse(),nullptr));
		cloud_right->runAction(cseq2);




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



		Sprite* startmenu_title=Sprite::createWithSpriteFrameName("startmenu_title.png");
		startmenu_title->setAnchorPoint(Vec2(0,0));
		startmenu_title->setPosition(Vec2(2,winSize.height-startmenu_title->getContentSize().height-2));
		this->addChild(startmenu_title,3);

		//produce the menu button

		_menu_array.clear();

		MenuButton* network_btn=MenuButton::create("menu01.png");
		network_btn->setDelegate(this);
		network_btn->setBtnType(Network);
		network_btn->setScale(0.5f);
		network_btn->setPositionY(_pos03);
		_menu_array.push_back(network_btn);

		MenuButton* credits_btn=MenuButton::create("menu04.png");
		credits_btn->setDelegate(this);
		credits_btn->setBtnType(Credits);
		credits_btn->setScale(0.5f);
		credits_btn->setVisible(false);
		credits_btn->_isBottom=true;
		credits_btn->setPositionY(_pos02);
		_menu_array.push_back(credits_btn);


		MenuButton* training_btn=MenuButton::create("menu02.png");
		training_btn->setDelegate(this);
		training_btn->setBtnType(Training);
		training_btn->_isTop=true;
		training_btn->setPositionY(_pos02);
		_menu_array.push_back(training_btn);

		/*if(!UserDefault::getInstance()->getBoolForKey("isHardCore")){
			UserDefault::getInstance()->setBoolForKey("isHardCore", false);
			UserDefault::getInstance()->flush();
		}


		if(UserDefault::getInstance()->getBoolForKey("isHardCore")==true){
			hardCore_btn=MenuButton::create("menu02_1.png");
		}else{
			hardCore_btn=MenuButton::create("menu02_2.png");
		}

		hardCore_btn->setDelegate(this);
		hardCore_btn->setBtnType(HardCore);
		hardCore_btn->setPosition(Vec2(146,_pos02-18));
		hardCore_btn->_isTop=true;
		this->addChild(hardCore_btn,10);*/




		MenuButton* exit_btn=MenuButton::create("menu03.png");
		exit_btn->setDelegate(this);
		exit_btn->setBtnType(Exit);
		exit_btn->setScale(0.5f);
		exit_btn->setPositionY(_pos01);
		_menu_array.push_back(exit_btn);




		menuText=Sprite::createWithSpriteFrameName("menu02_text.png");
		menuText->setAnchorPoint(Vec2(0,0));
		menuText->setPosition(Vec2(12,2));
		this->addChild(menuText,5);
		menuText->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.9f,1.04f),ScaleTo::create(0.9f,1.0f),nullptr)));

		float menuPosX=winSize.width*0.22f;
		if(menuPosX<105){
			menuPosX=105;
		}
		if(menuPosX>150){
			menuPosX=150;
		}

		int menuIntroIndex=0;
		for(MenuButton* menu : _menu_array){
			float finalY=menu->getPositionY();
			menu->setPositionX(menuPosX-36);
			menu->setOpacity(0);
			this->addChild(menu,2);
			menu->runAction(Sequence::create(DelayTime::create(0.08f*menuIntroIndex),Spawn::create(MoveTo::create(0.35f,Vec2(menuPosX,finalY)),FadeIn::create(0.35f),nullptr),nullptr));
			menuIntroIndex++;
		};


		//�汾��
		LabelBMFont* versionLabel=LabelBMFont::create("1.22.1 Vauzi","Fonts/1.fnt");
		versionLabel->setScale(0.3f);
		versionLabel->setPosition(winSize.width-60,10);
		this->addChild(versionLabel,5);

		Sprite* avator=Sprite::createWithSpriteFrameName("avator1.png");
		avator->setAnchorPoint(Vec2(0,0));
		avator->setOpacity(0);
		avator->setPosition(Vec2(winSize.width-avator->getContentSize().width,19));
		this->addChild(avator,1);


		Vector<SpriteFrame*> tempArray;
		Vector<FiniteTimeAction*> list;

		int i=1;

		while (i<5)
		{
			std::string path=cocos2d::StringUtils::format("avator%d.png",i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->spriteFrameByName(path);
			tempArray.pushBack(frame);
			Animation* tempAnimation= Animation::createWithSpriteFrames(tempArray, 0.1f);
			FiniteTimeAction* tempAction=Animate::create(tempAnimation);
			list.pushBack(tempAction);
			ActionInterval* fadeIn=FadeIn::create(0.8f);
			DelayTime* delay=DelayTime::create(1.0f);
			ActionInterval* fadeOut=FadeOut::create(0.5f);
			list.pushBack(fadeIn);
			list.pushBack(delay);
			list.pushBack(fadeOut);
			i+=1;
		}


		avator->runAction(RepeatForever::create(Sequence::create(list)));

		//���水ť
		MenuItem* news_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("news_btn.png"),NULL,NULL,CC_CALLBACK_1(StartMenu::onNewsBtn, this));
		Menu* menu =Menu::create(news_btn,NULL);
		news_btn->setAnchorPoint(Vec2(0,0.5f));
		menu->setPosition(15,winSize.height-50);
		this->addChild(menu,5);


		this->setNotice();

		//����ǽ����
		if(MemberID){
			this->setGroup();
			this->setOfferWall();
			this->setProfile();
			this->scrollMenu(_pos03);

		}else{
			//��½��ť
			login_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("login_btn.png"),NULL,NULL,CC_CALLBACK_1(StartMenu::onLoginBtn, this));
			Menu* menu2 =Menu::create(login_btn,NULL);
			login_btn->setAnchorPoint(Vec2(1,0.5f));
			menu2->setPosition(winSize.width-15,winSize.height-50);
			this->addChild(menu2,5);
		}



		this->scheduleUpdate();

		bRet=true;

	}while(0);

	return bRet;

}
void StartMenu::onEnter(){
	Layer::onEnter();
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(StartMenu::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	SimpleAudioEngine::end();

	if(UserDefault::getInstance()->getBoolForKey("isBGM")!=false){
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/Music/menu_music.mp3",true);
	}

	if(isPlayed){
		isPlayed=false;

//#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
//
//		JniMethodInfo minfo;
//
//		bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/NarutoSenki","showSpotAd", "()V");
//		if (isHave) {
//			minfo.env -> CallStaticVoidMethod(minfo.classID,minfo.methodID);
//		}
//
//#endif
	}


}
void StartMenu::onExit(){
	_eventDispatcher->removeEventListenersForTarget(this);
	Layer::onExit();

}

void StartMenu::setOfferWall(){

	//if(MemberID && !task_btn){
	//	const std::string user_string=user_id->getCString();
	//
	//	#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
	//	JniMethodInfo minfo;

	//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/NarutoSenki","setupTask", "(Ljava/lang/String;)V");
	//	if (isHave) {
	//		jstring user_arg = minfo.env->NewStringUTF(user_string.c_str());
	//		minfo.env -> CallStaticVoidMethod(minfo.classID,minfo.methodID,user_arg);
	//	}
	//	#endif

	//	//����ǽ��ť
	//	task_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("tasks_btn.png"),NULL,NULL,CC_CALLBACK_1(StartMenu::onTasksBtn, this));
	//	Menu* menu3 =Menu::create(task_btn,NULL);
	//	task_btn->setAnchorPoint(Vec2(1,0.5f));
	//	menu3->setPosition(winSize.width-20,winSize.height-110);
	//	this->addChild(menu3,5);

	//}




}
void StartMenu::setGroup(){



	//��֯��ť
	MenuItemSprite* group_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("group_btn.png"),NULL,NULL,CC_CALLBACK_1(StartMenu::onGroupBtn, this));
	Menu* menu3 =Menu::create(group_btn,NULL);
	group_btn->setAnchorPoint(Vec2(1,0.5f));
	menu3->setPosition(winSize.width-70,winSize.height-110);
	this->addChild(menu3,5);




}
void  StartMenu::onGroupBtn(Ref* sender){
	if(!group_layer && !profile_layer){
		CCTips *tip=CCTips::create("ServerMainten");
		this->addChild(tip,5000);
	}
}
void StartMenu::onGroupManageRequestCompleted(Node *sender ,void *data){
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}
void StartMenu::onQuitBtn(Ref* sender){

	MenuItem* item=(MenuItem*) sender;


	input_layer=Layer::create();

	Sprite* confirm_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
	confirm_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));
	Sprite* quit_title;

	if(MemberID==item->getTag()){
		quit_title=Sprite::createWithSpriteFrameName("dissovle_title.png");
		Sprite* pw_tip=Sprite::createWithSpriteFrameName("coin_tips2.png");
		pw_tip->setPosition(Vec2(winSize.width/2,winSize.height/2));
		input_layer->addChild(pw_tip,700);
	}else{
		quit_title=Sprite::createWithSpriteFrameName("quit_title.png");

	}
	quit_title->setPosition(Vec2(winSize.width/2,winSize.height/2+30));
	input_layer->addChild(quit_title,650);





	MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(StartMenu::onQuitComfirm, this));
	yes_btn->setTag(item->getTag());
	MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(StartMenu::onInputBoxClose, this));

	Menu* confirm_menu = Menu::create(yes_btn,no_btn,NULL);
	confirm_menu->alignItemsHorizontallyWithPadding(24);
	confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-30));

	input_layer->addChild(confirm_bg,600);
	input_layer->addChild(confirm_menu,650);


	this->addChild(input_layer,700);

}
void StartMenu::onQuitComfirm(Ref* sender){
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
	this->onInputBoxClose(NULL);
}

void StartMenu::onQuitGroupRequestCompleted(Node *sender ,void *data) {
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}
void StartMenu::onNoticeBtn(Ref* sender){
	if(leaderID==MemberID){

		if(!input_layer){
			input_layer=Layer::create();

			Sprite* pw_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
			pw_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));




			gpwInput=NULL;
			cocos2d::ui::Scale9Sprite * bg01=cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("input_bg.png");
			gpwInput = cocos2d::ui::EditBox::create(Size(115, 25), bg01);
			gpwInput->setFontColor(Color3B(255,255,255));
			gpwInput->setPosition(Vec2(winSize.width/2,winSize.height/2+25));
			gpwInput->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
			gpwInput->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
			input_layer->addChild(gpwInput,200);

			/*Sprite* pw_tip=Sprite::createWithSpriteFrameName("gpw_tip.png");
			pw_tip->setPosition(Vec2(winSize.width/2,winSize.height/2));
			input_layer->addChild(pw_tip,200);*/

			MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(StartMenu::onNoticeComfirm, this));
			MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(StartMenu::onInputBoxClose, this));

			Menu* confirm_menu = Menu::create(yes_btn,no_btn,NULL);
			confirm_menu->alignItemsHorizontallyWithPadding(16);
			confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-35));
			input_layer->addChild(confirm_menu,500);

			input_layer->addChild(pw_bg,100);
			this->addChild(input_layer,500);
		}

	}

}
void StartMenu::onNoticeComfirm(Ref* sender){

	std::string data1=gpwInput->getText();
	if(strcmp(data1.c_str(),"")==0){
		CCTips *tip=CCTips::create("InputEmpty");
		this->addChild(tip,5000);
		return;
	}

	if(noticeLabel){
		noticeLabel->setString(data1.c_str());
	}
	CCTips *tip=CCTips::create("UpdateNotice");
	this->addChild(tip,5000);
	this->onInputBoxClose(NULL);
}
void StartMenu::onLockBtn(Ref* sender){
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
}
void StartMenu::onGPWComfirm(Ref* sender){
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
	this->onInputBoxClose(NULL);
}

void StartMenu::onGPWRequestCompleted(Node *sender ,void *data) {
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}
void StartMenu::onKickBtn(Ref* sender){

	MenuItem* item=(MenuItem*) sender;

	if(!input_layer){

		input_layer=Layer::create();

		Sprite* confirm_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
		confirm_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));
		Sprite* quit_title=Sprite::createWithSpriteFrameName("kick_title.png");
		quit_title->setPosition(Vec2(winSize.width/2,winSize.height/2+30));
		input_layer->addChild(quit_title,650);

		MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(StartMenu::onKickComfirm, this));
		yes_btn->setTag(item->getTag());
		MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(StartMenu::onInputBoxClose, this));

		Menu* confirm_menu = Menu::create(yes_btn,no_btn,NULL);
		confirm_menu->alignItemsHorizontallyWithPadding(24);
		confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-30));

		input_layer->addChild(confirm_bg,600);
		input_layer->addChild(confirm_menu,650);


		this->addChild(input_layer,700);

	}




}

void StartMenu::onKickComfirm(Ref* sender){
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
	this->onInputBoxClose(NULL);
}
void StartMenu::onGroupRequestCompleted(Node *sender ,void *data){
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}
void StartMenu::onJoinGroup(Ref* sender){
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
}
void StartMenu::onJoinComfirm(Ref* sender){
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
	this->onInputBoxClose(NULL);
}

void StartMenu::onJoinRequestCompleted(Node *sender ,void *data) {
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}

void StartMenu::onSearchBtn(Ref* sender){
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
}


void StartMenu::onSeachRequestCompleted(Node *sender ,void *data) {
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
	if(searchInput){
		searchInput->setText("");
	}
}


void  StartMenu::onCreateBtn(Ref* sender){
	if(std::atoi(CValue.c_str())>=50*30+500){

		if(!input_layer){
			input_layer=Layer::create();

			Sprite* create_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
			create_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));


			Sprite* nameInputTitle=Sprite::createWithSpriteFrameName("nameInput_title.png");
			nameInputTitle->setPosition(Vec2(winSize.width/2-62,winSize.height/2+25));
			input_layer->addChild(nameInputTitle,200);

			cocos2d::ui::Scale9Sprite * bg01=cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("input_bg.png");
			nameInput = cocos2d::ui::EditBox::create(Size(115, 25), bg01);
			nameInput->setFontColor(Color3B(255,255,255));
			nameInput->setPosition(Vec2(winSize.width/2+16,winSize.height/2+25));
			nameInput->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
			nameInput->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
			input_layer->addChild(nameInput,200);

			Sprite* create_tip=Sprite::createWithSpriteFrameName("createGroup_tip.png");
			create_tip->setPosition(Vec2(winSize.width/2,winSize.height/2));
			input_layer->addChild(create_tip,200);

			MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(StartMenu::onCreateComfirm, this));
			MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(StartMenu::onInputBoxClose, this));

			Menu* confirm_menu = Menu::create(yes_btn,no_btn,NULL);
			confirm_menu->alignItemsHorizontallyWithPadding(16);
			confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-35));
			input_layer->addChild(confirm_menu,500);

			input_layer->addChild(create_bg,100);
			this->addChild(input_layer,500);
		}


	}else{
		CCTips *tip=CCTips::create("CPError1");
		this->addChild(tip,5000);
		return;
	}
}

void StartMenu::onCreateComfirm(Ref* sender){
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
	this->onInputBoxClose(NULL);
}

void StartMenu::onCreateRequestCompleted(Node *sender ,void *data) {
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}


void StartMenu::onSelectGroupBtn(Ref* sender){

	MenuItemLabel* item=(MenuItemLabel*) sender;

	for (auto screwNode : _screwLayer1->getItemArray()){
		CCScrewItem* screwItem=(CCScrewItem*) screwNode;
		if(screwItem->getTag()==item->getTag()){
			if(screwItem->linemenu){
				screwItem->linemenu->setVisible(true);
			}


		}else{
			if(screwItem->linemenu){
				screwItem->linemenu->setVisible(false);
			}


		}
	}
}


void StartMenu::setProfile(){



	//������ť
	MenuItemSprite* profile_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("profile_btn.png"),NULL,NULL,CC_CALLBACK_1(StartMenu::onProfileBtn, this));
	Menu* menu3 =Menu::create(profile_btn,NULL);
	profile_btn->setAnchorPoint(Vec2(1,0.5f));
	menu3->setPosition(winSize.width-20,winSize.height-50);
	this->addChild(menu3,5);




}




void  StartMenu::onProfileBtn(Ref* sender){

	if(!group_layer && !profile_layer){
		isPosting=false;
		profile_layer=Layer::create();
		Sprite* profile_bg=Sprite::createWithSpriteFrameName("profile_bg.png");
		profile_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));

		profile_layer->addChild(profile_bg);

		// offline close button
		MenuItemSprite* close_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("profile_closeBtn.png"),NULL,NULL,CC_CALLBACK_1(StartMenu::onIterfaceClose, this));
		Menu* menu =Menu::create( close_btn,NULL);
		menu->setPosition(Vec2(profile_bg->getPositionX()+profile_bg->getContentSize().width/2-15,profile_bg->getPositionY()+profile_bg->getContentSize().height/2-12));
		profile_layer->addChild(menu);

		this->addChild(profile_layer,500);

		std::string profileName=UserDefault::getInstance()->getStringForKey("userName");
		if(profileName.empty()){
			profileName="Offline";
		}

		int point=std::atoi(CValue.c_str());
		std::string pointText=cocos2d::StringUtils::format("%d",point);
		std::string namePlate=UserDefault::getInstance()->getStringForKey("namePlateId");

		Label* group_name=Label::createWithSystemFont("Offline",FONT_TYPE,12);
		group_name->setPosition(Vec2(winSize.width/2+45,182));
		profile_layer->addChild(group_name);

		Label* user_name=Label::createWithSystemFont(profileName.c_str(),FONT_TYPE,12);
		user_name->setPosition(Vec2(winSize.width/2+45,210));
		profile_layer->addChild(user_name);

		Label* user_battle=Label::createWithSystemFont("0",FONT_TYPE,10);
		user_battle->setPosition(Vec2(winSize.width/2-35,150));
		profile_layer->addChild(user_battle);

		Label* user_point=Label::createWithSystemFont(pointText.c_str(),FONT_TYPE,10);
		user_point->setPosition(Vec2(winSize.width/2+72,150));
		profile_layer->addChild(user_point);

		std::string path="shinobiCard_null.png";
		if(!namePlate.empty()){
			path=cocos2d::StringUtils::format("%s.png",namePlate.c_str());
		}
		MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(path),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlate, this));
		Menu* np_menu =Menu::create(np_btn,NULL);
		np_btn->setAnchorPoint(Vec2(0.5f,0));
		np_menu->setPosition(Vec2(winSize.width/2+16,100));
		profile_layer->addChild(np_menu);
	}


}



void StartMenu::onProfileRequestCompleted(Node *sender ,void *data) {
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}
void  StartMenu::onNamePlate(Ref* sender){
	if(std::atoi(CValue.c_str())<2*1200+600){
		CCTips *tip=CCTips::create("CPError2");
		this->addChild(tip,5000);
		return;
	}
	Sprite* nameplate_bg=Sprite::createWithSpriteFrameName("shinobiCard_bg.png");
	nameplate_bg->setAnchorPoint(Vec2(0.5f,0));
	nameplate_bg->setPosition(Vec2(winSize.width/2,92));
	profile_layer->addChild(nameplate_bg);

	CCClippingNode* clipper = CCClippingNode::create();
	Node *stencil = Sprite::createWithSpriteFrameName("shinobiCard_mask.png");
	stencil->setAnchorPoint(Vec2(0,0));
	clipper->setStencil(stencil);


	_screwLayer1=CCScrewLayer::create();
	_screwLayer1->setAnchorPoint(Vec2(0,0));
	_screwLayer1->setPositionY(4);
	_screwLayer1->minY=4;
	_screwLayer1->line_height=30;

	clipper->setPosition(Vec2(nameplate_bg->getPositionX()-nameplate_bg->getContentSize().width/2,92));
	clipper->addChild(_screwLayer1,5);


	profile_layer->addChild(clipper,20);
	int coutRow=0;
	int np100=15;
	int np200=17;
	int np300=14;
	int np400=11;
	int np500=5;
	int np600=2;
	int np700=1;
	int np800=2;
	int np900=1;
	int np1000=1;
	int row=1;

	if(std::atoi(CValue.c_str())>=48000){
		for(int i=0;i<np1000;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np10%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(1000+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}

	if(std::atoi(CValue.c_str())>=36000){
		for(int i=0;i<np900;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np9%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(900+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}
	if(std::atoi(CValue.c_str())>=30000){
		for(int i=0;i<np800;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np8%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(800+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}



	if(std::atoi(CValue.c_str())>=24000){
		for(int i=0;i<np700;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np7%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(700+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}
	if(std::atoi(CValue.c_str())>=20000){
		for(int i=0;i<np600;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np6%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(600+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}

	if(std::atoi(CValue.c_str())>=16000){
		for(int i=0;i<np500;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np5%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(500+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}
	if(std::atoi(CValue.c_str())>=12000){
		for(int i=0;i<np400;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np4%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(400+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}

	if(std::atoi(CValue.c_str())>=9000){
		for(int i=0;i<np300;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np3%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(300+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}

	if(std::atoi(CValue.c_str())>=6000){

		for(int i=0;i<np200;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np2%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(200+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}
	if(std::atoi(CValue.c_str())>=3000){
		for(int i=0;i<np100;i++){
			CCScrewItem* screwItem=CCScrewItem::create();

			MenuItem* np_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName(cocos2d::StringUtils::format("np1%02d.png",i)),NULL,NULL,CC_CALLBACK_1(StartMenu::onNamePlateClick, this));
			np_btn->setTag(100+i);
			Menu* menu =Menu::create(np_btn,NULL);
			np_btn->setAnchorPoint(Vec2(0,0));
			menu->setPosition(Vec2(48,8));
			screwItem->addChild(menu);


			screwItem->setPosition(Vec2(0,-row*30+26));
			screwItem->setAnchorPoint(Vec2(0,0));
			_screwLayer1->addChild(screwItem,10);
			row++;
		}
	}



	_screwLayer1->totalRow=row;

}

void  StartMenu::onNamePlateClick(Ref* sender){
	MenuItem* item=(MenuItem*) sender;
		if(!input_layer){
			input_layer=Layer::create();

			Sprite* pw_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
			pw_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));



			Sprite* quit_title=Sprite::createWithSpriteFrameName("nameplate_title.png");

			quit_title->setPosition(Vec2(winSize.width/2,winSize.height/2+30));
			input_layer->addChild(quit_title,650);

			Sprite* pw_tip=Sprite::createWithSpriteFrameName("coin_tips2.png");
			pw_tip->setPosition(Vec2(winSize.width/2,winSize.height/2));
			input_layer->addChild(pw_tip,200);

			MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(StartMenu::onNamePlateComfrim, this));
			yes_btn->setTag(item->getTag());
			MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(StartMenu::onInputBoxClose, this));

			Menu* confirm_menu = Menu::create(yes_btn,no_btn,NULL);
			confirm_menu->alignItemsHorizontallyWithPadding(16);
			confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-35));
			input_layer->addChild(confirm_menu,500);

			input_layer->addChild(pw_bg,100);
			this->addChild(input_layer,500);
		}



}

void  StartMenu::onNamePlateComfrim(Ref* sender){

	MenuItem* item=(MenuItem*) sender;
	KTools* tool=KTools::create();
	std::string coinValue=tool->readFromSQLite();
	std::string tempCost2="EFDDD\x14";
	tool->decode(tempCost2);

	int tempCoin =std::atoi(coinValue.c_str())-std::atoi(tempCost2.c_str());
	if(tempCoin>=0){
		std::string namePlate=cocos2d::StringUtils::format("np%d",item->getTag());
		UserDefault::getInstance()->setStringForKey("namePlateId",namePlate);
		UserDefault::getInstance()->flush();

		std::string coin=cocos2d::StringUtils::format("%d",tempCoin);
		tool->saveToSQLite("GameRecord","coin",coin.c_str(),true);

		CCTips *tip=CCTips::create("ManagementSuccessed");
		this->addChild(tip,5000);

		this->onIterfaceClose(NULL);
		this->onInputBoxClose(NULL);
	}else{
		CCTips *tip=CCTips::create("CoinError2");
		this->addChild(tip,5000);
	}



}



void StartMenu::onNamePlateRequestCompleted(Node *sender ,void *data) {
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}


void  StartMenu::onTasksBtn(Ref* sender){

//#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
//
//	JniMethodInfo minfo;
//
//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/NarutoSenki","showOffWall", "()V");
//	if (isHave) {
//		minfo.env -> CallStaticVoidMethod(minfo.classID,minfo.methodID);
//	}
//
//#endif

}

void  StartMenu::onLoginBtn(Ref* sender){

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Result.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Record.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Record2.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Report.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis2.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Gears.plist");
	//Scene* pscene =Scene::create();
	//RakingLayer* rakingLayer=RakingLayer::create();
	////rakingLayer->setDelegate(this);
	//pscene->addChild(rakingLayer);
	//Director::getInstance()->pushScene(pscene);

	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);
	return;

	if(MemberID==NULL){
		if(!input_layer){
			input_layer=Layer::create();

			Sprite* login_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
			login_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));


			Sprite* userInputTitle=Sprite::createWithSpriteFrameName("userInput_title.png");
			userInputTitle->setPosition(Vec2(winSize.width/2-62,winSize.height/2+35));
			input_layer->addChild(userInputTitle,200);

			cocos2d::ui::Scale9Sprite * bg01=cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("input_bg.png");
			usernameInput = cocos2d::ui::EditBox::create(Size(115, 25), bg01);
			usernameInput->setFontColor(Color3B(255,255,255));
			usernameInput->setPosition(Vec2(winSize.width/2+16,winSize.height/2+35));
			usernameInput->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
			usernameInput->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
			input_layer->addChild(usernameInput,200);


			if(strcmp(UserDefault::getInstance()->getStringForKey("userName").c_str(),"")!=0){
				usernameInput->setText(UserDefault::getInstance()->getStringForKey("userName").c_str());
			}
			Sprite* pwInputTitle=Sprite::createWithSpriteFrameName("pwInput_title.png");
			pwInputTitle->setPosition(Vec2(winSize.width/2-60,winSize.height/2));
			input_layer->addChild(pwInputTitle,200);
			cocos2d::ui::Scale9Sprite * bg02=cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("input_bg.png");
			pwInput = cocos2d::ui::EditBox::create(Size(115, 25), bg02);
			pwInput->setFontColor(Color3B(255,255,255));
			pwInput->setPosition(Vec2(winSize.width/2+16,winSize.height/2));
			pwInput->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
			pwInput->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
			pwInput->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
			input_layer->addChild(pwInput,200);


			if(strcmp(UserDefault::getInstance()->getStringForKey("password").c_str(),"")!=0){
				pwInput->setText(UserDefault::getInstance()->getStringForKey("password").c_str());
			}

			MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("login_btn2.png"),Sprite::createWithSpriteFrameName("login_btn1.png"),CC_CALLBACK_1(StartMenu::onLoginOn, this));
			MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(StartMenu::onInputBoxClose, this));

			Menu* confirm_menu = Menu::create(yes_btn,no_btn,NULL);
			confirm_menu->alignItemsHorizontallyWithPadding(16);
			confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-35));
			input_layer->addChild(confirm_menu,500);

			input_layer->addChild(login_bg,100);
			this->addChild( input_layer,500);
		}


	}else{
		CCTips *tip=CCTips::create("LoginDuplicate");
		this->addChild(tip,5000);


	}





}

void  StartMenu::onLoginOn(Ref* sender){

	if(!isPosting){
		isPosting=false;
		std::string data1=usernameInput ? usernameInput->getText() : "";
		UserDefault::getInstance()->setStringForKey("userName",data1);

		std::string data2 = pwInput ? pwInput->getText() : "";
		std::string result;

		if(strcmp(UserDefault::getInstance()->getStringForKey("password").c_str(),data2.c_str())!=0){
			KTools *tool=new KTools();
			result=tool->encodeData(data2);
			delete tool;
		}else{
			result=data2;
		}
		PWord=result;

		UserDefault::getInstance()->setStringForKey("password",result);
		UserDefault::getInstance()->flush();

		CCTips *tip=CCTips::create("ServerMainten");
		this->addChild(tip,5000);
		this->onInputBoxClose(NULL);
	}


}
void StartMenu::update(float dt){

	if(!noticeLabel){
		return;
	}

	float currentX = noticeLabel->getPositionX();
	//float contentX = getContentSize().width;
	float lableX = noticeLabel->getContentSize().width;


	if(noticeLabel->getPositionX()>=-lableX){
		noticeLabel->setPositionX(noticeLabel->getPositionX()-0.6f);
	}
	else{
		noticeLabel->setPositionX(190);
	}



}

void StartMenu::setNotice(){

	if(!notice_layer){
		notice_layer=Layer::create();
		Sprite* notice_bg=Sprite::createWithSpriteFrameName("notice_bg.png");
		notice_bg->setAnchorPoint(Vec2(0,0));
		notice_bg->setPosition(Vec2(15,228));
		notice_layer->addChild(notice_bg);


		CCClippingNode* clipper = CCClippingNode::create();
		Node *stencil = Sprite::createWithSpriteFrameName("notice_mask.png");
		stencil->setAnchorPoint(Vec2(0,0));
		clipper->setStencil(stencil);

		noticeLabel=Label::createWithSystemFont("",FONT_TYPE,12);
		noticeLabel->setAnchorPoint(Vec2(0,0));
		clipper->addChild(noticeLabel);
		noticeLabel->retain();
		clipper->setPosition(Vec2(35,228));

		notice_layer->addChild(clipper);

		this->addChild(notice_layer);
		ValueMap strings=FileUtils::getInstance()->getValueMapFromFile("Element/strings.xml");
		auto noticeValue=strings.find("Notice");
		std::string reply=noticeValue != strings.end() ? noticeValue->second.asString() : "";

		noticeLabel->setString(reply);


	}


}
void StartMenu::onNoticeRequestCompleted(Node *sender ,void *data) {
	// Notice memakai teks lokal dari Element/strings.xml pada mode offline.
	(void)sender;
	(void)data;
	isPosting=false;
}
void StartMenu::onLoginRequestCompleted(Node *sender ,void *data) {
	(void)sender;
	(void)data;
	isPosting=false;
	if(refreshBtn){
		refreshBtn->stopAllActions();
		refreshBtn->removeFromParent();
		refreshBtn=NULL;
	}
}

void  StartMenu::onIterfaceClose(Ref* sender){
	if(!isPosting){
		if(profile_layer){
			profile_layer->removeAllChildren();
			profile_layer->removeFromParent();
			profile_layer=NULL;
		}

		if(group_layer){
			group_layer->removeAllChildren();
			group_layer->removeFromParent();
			group_layer=NULL;
		}

	}

}


void  StartMenu::onInputBoxClose(Ref* sender){
	if(input_layer){
	input_layer->removeAllChildren();
	input_layer->removeFromParent();
	input_layer=NULL;

	}



}




void  StartMenu::onNewsBtn(Ref* sender){

//#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
//
//	JniMethodInfo minfo;
//
//	bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/NarutoSenki","getInstance","()Lnet/zakume/game/NarutoSenki;");
//	jobject jobj;//�����
//	if (isHave) {
//		//����ĵ���getInstance������Test��Ķ���
//		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
//
//		isHave = JniHelper::getMethodInfo(minfo,"net/zakume/game/NarutoSenki","openWebview","()V");
//		if (isHave) {
//			//����openWebview, ����1��Test����   ����2������ID
//			minfo.env->CallVoidMethod(jobj, minfo.methodID);
//		}
//	}
//
//#endif
}

void StartMenu::onHardCoreOn(Ref* sender){
	UserDefault::getInstance()->setBoolForKey("isHardCore",true);
	if(hardCore_btn){
		SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName("menu02_1.png");
		hardCore_btn->setSpriteFrame(frame);
	}

	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
	if(hardCoreLayer){
		hardCoreLayer->removeAllChildren();
		hardCoreLayer->removeFromParent();
		hardCoreLayer=NULL;
	}
}

void StartMenu::onHardCoreOff(Ref* sender){

	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/cancel.ogg");
	if(hardCoreLayer){
		hardCoreLayer->removeAllChildren();
		hardCoreLayer->removeFromParent();
		hardCoreLayer=NULL;
	}

}

void StartMenu::onHardLayerCallBack(){





	if( UserDefault::getInstance()->getBoolForKey("isHardCore")==false){
		if(!hardCoreLayer){

		hardCoreLayer=Layer::create();

		Sprite* confirm_bg=Sprite::createWithSpriteFrameName("confirm_bg.png");
		confirm_bg->setPosition(Vec2(winSize.width/2,winSize.height/2));

		Sprite* hardcore_title=Sprite::createWithSpriteFrameName("hardcore_title.png");
		hardcore_title->setPosition(Vec2(winSize.width/2,winSize.height/2+38));

		Sprite* hardcore_text=Sprite::createWithSpriteFrameName("hardcore_text.png");
		hardcore_text->setPosition(Vec2(winSize.width/2,winSize.height/2+8));

		MenuItem* yes_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("yes_btn1.png"),Sprite::createWithSpriteFrameName("yes_btn2.png"),CC_CALLBACK_1(StartMenu::onHardCoreOn, this));
		MenuItem* no_btn=MenuItemSprite::create(Sprite::createWithSpriteFrameName("no_btn1.png"),Sprite::createWithSpriteFrameName("no_btn2.png"),CC_CALLBACK_1(StartMenu::onHardCoreOff, this));

		Menu* confirm_menu = Menu::create(yes_btn,no_btn,NULL);
		confirm_menu->alignItemsHorizontallyWithPadding(24);
		confirm_menu->setPosition(Vec2(winSize.width/2,winSize.height/2-30));

		hardCoreLayer->addChild(confirm_bg,600);
		hardCoreLayer->addChild(confirm_menu,650);
		hardCoreLayer->addChild(hardcore_title,650);
		hardCoreLayer->addChild(hardcore_text,650);
		this->addChild(hardCoreLayer,700);

		}
	}

}

void StartMenu::onHardCoreCallBack(){


	/*if(MemberID){



		//�������м�
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Select.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Report.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Gears.plist");

	Scene* selectScene=Scene::create();
	NetworkLayer* selectLayer=NetworkLayer::create();
	KTools* tool= KTools::create();


	selectScene->addChild(selectLayer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,selectScene));
	}*/
	CCTips *tip=CCTips::create("ServerMainten");
	this->addChild(tip,5000);

}

void StartMenu::onNormalCallBack(Ref* sender){

	//�������м�
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Select.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Report.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Gears.plist");

	Scene* selectScene=Scene::create();
	NetworkLayer* selectLayer=NetworkLayer::create();
	KTools* tool= KTools::create();


	selectScene->addChild(selectLayer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,selectScene));


	/*Scene* selectScene=Scene::create();
	SelectLayer* selectLayer=SelectLayer::create();
	selectScene->addChild(selectLayer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,selectScene));	*/
}

void StartMenu::onTrainingCallBack(){

	if(input_layer || profile_layer || group_layer) {
		return;
	}

	showTrainingOptions();
	return;


	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Select.plist");


	int i=1;

	if(adResult!=1){
	Cheats=0;
	}

	if(Cheats>10){
		i=2;
	}

	if(i==0){
		this->onNormalCallBack(NULL);
	}else if(i==1){

	//�������м�
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Report.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Gears.plist");


	CCDictionary* dic=CCDictionary::create();


	char* heroList[]={"Lee","Shino","Kakashi","Kakuzu",
		"Asuma"};

	srand((int)time(0));
	std::string tmpChar="Tsunade";
	CCString* tmpRole=CCString::create("Player");
	CCString* tmpGroup=CCString::create("Konoha");

	dic->setObject(CCString::create(tmpChar.c_str()),"character");
	dic->setObject(tmpRole,"role");
	dic->setObject(tmpGroup,"group");

	CCArray* tempHeros=CCArray::createWithObject(dic);

	std::vector<std::string> realHero;
	int num= sizeof(heroList)/sizeof(char*);
	for(int i=0;i<num;i++){
		if(strcmp(tmpChar.c_str(),heroList[i])==0){
			continue;
		}
		realHero.push_back(heroList[i]);
	}

	//�������
	//--------------

	for(int i=0;i<5;i++){

		if(i<2){
			int length=(int)realHero.size();
			srand((int)time(0));
			int index=random(length);
			if(index==(int)realHero.size()){

				index=(int)realHero.size()-1;
			}


			std::string hero=realHero[index];

			dic=CCDictionary::create();
			tmpChar=hero;
			tmpRole=CCString::create("Com");
			tmpGroup=CCString::create("Konoha");

			dic->setObject(CCString::create(tmpChar.c_str()),"character");
			dic->setObject(tmpRole,"role");
			dic->setObject(tmpGroup,"group");

			tempHeros->addObject(dic);
			realHero.erase(realHero.begin()+index);


		}else{

			int length=(int)realHero.size();
			srand((int)time(0));
			int index=random(length);
			if(index==(int)realHero.size()){
				index=(int)realHero.size()-1;
			}

			std::string hero=realHero[index];

			dic=CCDictionary::create();
			tmpChar=hero;
			tmpRole=CCString::create("Com");
			tmpGroup=CCString::create("Akatsuki");

			dic->setObject(CCString::create(tmpChar.c_str()),"character");
			dic->setObject(tmpRole,"role");
			dic->setObject(tmpGroup,"group");

			tempHeros->addObject(dic);
			realHero.erase(realHero.begin()+index);




		}

	}


	Scene* loadScene=Scene::create();
	LoadLayer* loadLayer=LoadLayer::create();
	loadLayer->tempHeros=tempHeros;
	loadLayer->_isHardCoreMode=true;
	loadScene->addChild(loadLayer);
	loadLayer->preloadAudio();
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,loadScene));

	}else if(i==2){

	//4v4

	//�������м�
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Report.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ougis2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Map.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Gears.plist");


	CCDictionary* dic=CCDictionary::create();


	char* heroList1[]={"Konan","Sakura","Naruto","Sai","Deidara",
		"Kakashi","Itachi","Tenten","Jiraiya","Suigetsu",
		"Tsunade","Tobirama","Shisui","Neji","Ino","Asuma","Gaara",
		"Karin","Sasuke","Hidan","Choji","Kankuro",
		"Shino","Minato","Tobi","Kakuzu","Hinata",
		"Shikamaru","Chiyo","Kisame",
		"Hiruzen","Kiba","Jugo","Lee"
	};

	srand((int)time(0));
	int index2=random(sizeof(heroList1)/sizeof(char*));
	std::string tmpChar=heroList1[index2];
	CCString* tmpRole=CCString::create("Player");
	CCString* tmpGroup=CCString::create("Konoha");

	dic->setObject(CCString::create(tmpChar.c_str()),"character");
	dic->setObject(tmpRole,"role");
	dic->setObject(tmpGroup,"group");

	CCArray* tempHeros=CCArray::createWithObject(dic);

	std::vector<std::string> realHero;
	int num= sizeof(heroList1)/sizeof(char*);
	for(int i=0;i<num;i++){
		if(strcmp(tmpChar.c_str(),heroList1[i])==0){
			continue;
		}
		realHero.push_back(heroList1[i]);
	}

	//�������
	//--------------

	for(int i=0;i<7;i++){

		if(i<3){
			int length=(int)realHero.size();
			srand((int)time(0));
			int index=random(length);
			if(index==(int)realHero.size()){

				index=(int)realHero.size()-1;
			}


			std::string hero=realHero[index];

			dic=CCDictionary::create();
			tmpChar=hero;
			tmpRole=CCString::create("Com");
			tmpGroup=CCString::create("Konoha");

			dic->setObject(CCString::create(tmpChar.c_str()),"character");
			dic->setObject(tmpRole,"role");
			dic->setObject(tmpGroup,"group");

			tempHeros->addObject(dic);
			realHero.erase(realHero.begin()+index);


		}else{

			int length=(int)realHero.size();
			srand((int)time(0));
			int index=random(length);
			if(index==(int)realHero.size()){
				index=(int)realHero.size()-1;
			}

			std::string hero=realHero[index];

			dic=CCDictionary::create();
			tmpChar=hero;
			tmpRole=CCString::create("Com");
			tmpGroup=CCString::create("Akatsuki");

			dic->setObject(CCString::create(tmpChar.c_str()),"character");
			dic->setObject(tmpRole,"role");
			dic->setObject(tmpGroup,"group");

			tempHeros->addObject(dic);
			realHero.erase(realHero.begin()+index);

		}

	}


	Scene* loadScene=Scene::create();
	LoadLayer* loadLayer=LoadLayer::create();
	loadLayer->tempHeros=tempHeros;
	loadScene->addChild(loadLayer);
	loadLayer->preloadAudio();
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,loadScene));


	}











}

void StartMenu::showTrainingOptions()
{
	if(input_layer)
	{
		return;
	}

	input_layer = Layer::create();

	LayerColor* blend = LayerColor::create(Color4B(0, 0, 0, 150), winSize.width, winSize.height);
	input_layer->addChild(blend, 0);

	cocos2d::ui::Scale9Sprite* bg = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("confirm_bg.png");
	if(!bg)
	{
		bg = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("input_bg2.png");
	}

	Size panelSize = Size(305, 242);

	if(bg)
	{
		bg->setContentSize(panelSize);
		bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		input_layer->addChild(bg, 1);
	}

	float cx = winSize.width / 2;
	float cy = winSize.height / 2;

	trainingShadowLabel(input_layer, "TRAINING SETUP", 15, Color3B(145, 55, 18), Vec2(cx, cy + 101), 2);

	Menu* modeMenu = Menu::create(
		trainingMenuItem("1v1", CC_CALLBACK_1(StartMenu::onTrainingModeOption, this), 1, trainingBattleSize == 1 && !trainingModeOnly),
		trainingMenuItem("2v2", CC_CALLBACK_1(StartMenu::onTrainingModeOption, this), 2, trainingBattleSize == 2 && !trainingModeOnly),
		trainingMenuItem("3v3", CC_CALLBACK_1(StartMenu::onTrainingModeOption, this), 3, trainingBattleSize == 3 && !trainingModeOnly),
		trainingMenuItem("4v4", CC_CALLBACK_1(StartMenu::onTrainingModeOption, this), 4, trainingBattleSize == 4 && !trainingModeOnly),
		trainingMenuItem("5v5", CC_CALLBACK_1(StartMenu::onTrainingModeOption, this), 5, trainingBattleSize == 5 && !trainingModeOnly),
		trainingMenuItem("TRAIN", CC_CALLBACK_1(StartMenu::onTrainingModeOption, this), 6, trainingModeOnly),
		NULL
	);
	modeMenu->alignItemsHorizontallyWithPadding(7);
	modeMenu->setPosition(Vec2(cx, cy + 68));
	input_layer->addChild(modeMenu, 2);

	trainingPlainLabel(input_layer, "TEAM", 10, Color3B(74, 54, 36), Vec2(cx - 122, cy + 35), 2);

	Menu* teamMenu = Menu::create(
		trainingMenuItem("Preset", CC_CALLBACK_1(StartMenu::onTrainingTeamOption, this), 0, !trainingRandomTeam),
		trainingMenuItem("Random", CC_CALLBACK_1(StartMenu::onTrainingTeamOption, this), 1, trainingRandomTeam),
		NULL
	);
	teamMenu->alignItemsHorizontallyWithPadding(20);
	teamMenu->setPosition(Vec2(cx + 32, cy + 35));
	input_layer->addChild(teamMenu, 2);

	trainingPlainLabel(input_layer, "ENEMY", 10, Color3B(74, 54, 36), Vec2(cx - 122, cy + 5), 2);

	Menu* enemyMenu = Menu::create(
		trainingMenuItem("Preset", CC_CALLBACK_1(StartMenu::onTrainingEnemyOption, this), 0, !trainingRandomEnemy),
		trainingMenuItem("Random", CC_CALLBACK_1(StartMenu::onTrainingEnemyOption, this), 1, trainingRandomEnemy),
		NULL
	);
	enemyMenu->alignItemsHorizontallyWithPadding(20);
	enemyMenu->setPosition(Vec2(cx + 32, cy + 5));
	input_layer->addChild(enemyMenu, 2);

	trainingPlainLabel(input_layer, "SCORE", 10, Color3B(74, 54, 36), Vec2(cx - 122, cy - 25), 2);

	Menu* killMenu = Menu::create(
		trainingMenuItem("1", CC_CALLBACK_1(StartMenu::onTrainingKillOption, this), 1, trainingKillLimit == 1),
		trainingMenuItem("3", CC_CALLBACK_1(StartMenu::onTrainingKillOption, this), 3, trainingKillLimit == 3),
		trainingMenuItem("5", CC_CALLBACK_1(StartMenu::onTrainingKillOption, this), 5, trainingKillLimit == 5),
		NULL
	);
	killMenu->alignItemsHorizontallyWithPadding(26);
	killMenu->setPosition(Vec2(cx + 14, cy - 25));
	input_layer->addChild(killMenu, 2);

	trainingPlainLabel(input_layer, "PIXEL", 10, Color3B(74, 54, 36), Vec2(cx - 122, cy - 55), 2);

	bool pixelSharp = isPixelArtEnabled();
	Menu* pixelMenu = Menu::create(
		trainingMenuItem("Sharp", CC_CALLBACK_1(StartMenu::onTrainingPixelOption, this), 1, pixelSharp),
		trainingMenuItem("Smooth", CC_CALLBACK_1(StartMenu::onTrainingPixelOption, this), 0, !pixelSharp),
		NULL
	);
	pixelMenu->alignItemsHorizontallyWithPadding(20);
	pixelMenu->setPosition(Vec2(cx + 32, cy - 55));
	input_layer->addChild(pixelMenu, 2);

	trainingShadowLabel(input_layer,
		trainingModeOnly ? "Enemy AI disabled" : "Preset uses fixed teams",
		9,
		Color3B(73, 61, 48),
		Vec2(cx, cy - 73),
		2);

	MenuItem* startBtn = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("yes_btn1.png"),
		Sprite::createWithSpriteFrameName("yes_btn2.png"),
		CC_CALLBACK_1(StartMenu::onTrainingStart, this)
	);

	MenuItem* cancelBtn = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName("no_btn1.png"),
		Sprite::createWithSpriteFrameName("no_btn2.png"),
		CC_CALLBACK_1(StartMenu::onInputBoxClose, this)
	);

	startBtn->setScale(0.85f);
	cancelBtn->setScale(0.85f);

	Menu* confirmMenu = Menu::create(startBtn, cancelBtn, NULL);
	confirmMenu->alignItemsHorizontallyWithPadding(26);
	confirmMenu->setPosition(Vec2(cx, cy - 96));
	input_layer->addChild(confirmMenu, 3);

	this->addChild(input_layer, 700);
}

void StartMenu::refreshTrainingOptions(){
	if(input_layer){
		input_layer->removeAllChildren();
		input_layer->removeFromParent();
		input_layer=NULL;
	}
	showTrainingOptions();
}

void StartMenu::onTrainingModeOption(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
	Node* node=(Node*)sender;
	int tag=node->getTag();
	trainingModeOnly=(tag==6);
	trainingBattleSize=trainingModeOnly ? 1 : tag;
	refreshTrainingOptions();
}

void StartMenu::onTrainingTeamOption(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
	trainingRandomTeam=((Node*)sender)->getTag()==1;
	refreshTrainingOptions();
}

void StartMenu::onTrainingEnemyOption(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
	trainingRandomEnemy=((Node*)sender)->getTag()==1;
	refreshTrainingOptions();
}

void StartMenu::onTrainingKillOption(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
	trainingKillLimit=((Node*)sender)->getTag();
	refreshTrainingOptions();
}

void StartMenu::onTrainingPixelOption(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect(SELECT_SOUND);
	bool enabled=((Node*)sender)->getTag()==1;
	UserDefault::getInstance()->setIntegerForKey("pixelArtSharp",enabled ? 1 : 0);
	UserDefault::getInstance()->flush();
	refreshTrainingOptions();
}

void StartMenu::onTrainingStart(Ref* sender){
	SimpleAudioEngine::getInstance()->playEffect("Audio/Menu/confirm.ogg");
	startTrainingBattle(trainingBattleSize,trainingRandomTeam,trainingRandomEnemy,trainingModeOnly,trainingKillLimit);
}

void StartMenu::startTrainingBattle(int teamSize, bool randomTeam, bool randomEnemy, bool trainingMode, int killLimit){
    if(input_layer){
        input_layer->removeAllChildren();
        input_layer->removeFromParent();
        input_layer=NULL;
    }

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Select.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Gears.plist");

    Scene* selectScene = Scene::create();
    if(!selectScene){
        CCLOG("ERROR: Scene::create failed in startTrainingBattle");
        showTrainingOptions();
        return;
    }

    NetworkLayer* selectLayer = NetworkLayer::createTraining(teamSize, randomTeam, randomEnemy, trainingMode, killLimit);
    if(!selectLayer){
        CCLOG("ERROR: NetworkLayer::createTraining returned NULL. teamSize=%d randomTeam=%d randomEnemy=%d trainingMode=%d",
              teamSize,
              randomTeam ? 1 : 0,
              randomEnemy ? 1 : 0,
              trainingMode ? 1 : 0
        );

        showTrainingOptions();
        return;
    }

    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    selectScene->addChild(selectLayer);
    Director::getInstance()->replaceScene(TransitionFade::create(1.5f,selectScene));
}
void StartMenu::onCreditsCallBack(){


	if(input_layer || profile_layer || group_layer) {

		return;
	}
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Scene* creditsScene=Scene::create();
	CreditsLayer* creditsLayer=CreditsLayer::create();
	creditsScene->addChild(creditsLayer);
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f,creditsScene));

}

void StartMenu::scrollMenu(int posY){
		if(posY>_pos02 || (isDrag && isClockwise)){ //˳ʱ��
			for(MenuButton* menu : _menu_array){
				if(menu->getPositionY()==_pos01){
					ActionInterval* spn=MoveTo::create(0.5,Vec2(menu->getPositionX(),_pos02));
					this->reorderChild(menu,1);
					menu->_isBottom=true;
					FadeOut* fo=FadeOut::create(0.3f);
					Spawn* seq=Spawn::createWithTwoActions(spn,fo);
					menu->runAction(seq);

				}else if(menu->getPositionY()==_pos03){
					FiniteTimeAction* spn=Spawn::create(
						MoveTo::create(0.5,Vec2(menu->getPositionX(),_pos02)),
						ScaleTo::create(0.5,1),
						NULL
						);
					this->reorderChild(menu,3);
					menu->_isTop=true;

					/*if(menu->getBtnType()!=Training){
						hardCore_btn->setVisible(false);
					}*/

					FiniteTimeAction *call = CallFunc::create(CC_CALLBACK_0(MenuButton::playSound, menu));
					ActionInterval* seq=Sequence::create(spn,call,nullptr);
					menu->runAction(seq);

				}else if(menu->getPositionY()==_pos02 && menu->_isBottom){

					ActionInterval* spn=MoveTo::create(0.5,Vec2(menu->getPositionX(),_pos03));
					this->reorderChild(menu,1);
					menu->setVisible(true);
					FadeIn* fi=FadeIn::create(0.3f);
					Spawn* seq=Spawn::createWithTwoActions(spn,fi);
					menu->_isBottom=false;
					menu->runAction(seq);
				}else if(menu->getPositionY()==_pos02 && !menu->_isBottom){

					Action* spn=Spawn::create(
						MoveTo::create(0.5,Vec2(menu->getPositionX(),_pos01)),
						ScaleTo::create(0.5,0.5),
						NULL
						);
					this->reorderChild(menu,1);
					menu->_isTop=false;
					menu->runAction(spn);
				}

			};

		}else{ //��ʱ��
			for(MenuButton* menu : _menu_array){
				if(menu->getPositionY()==_pos01){
					FiniteTimeAction* spn=Spawn::create(
						MoveTo::create(0.5,Vec2(menu->getPositionX(),_pos02)),
						ScaleTo::create(0.5,1),
						NULL
						);
					this->reorderChild(menu,3);
					menu->_isTop=true;
				/*	if(menu->getBtnType()!=Training){
						hardCore_btn->setVisible(false);
					}*/

					FiniteTimeAction *call = CallFunc::create(CC_CALLBACK_0(MenuButton::playSound, menu));
					ActionInterval* seq=Sequence::create(spn,call,nullptr);
					menu->runAction(seq);
				}else if(menu->getPositionY()==_pos03){
					ActionInterval* spn=MoveTo::create(0.5,Vec2(menu->getPositionX(),_pos02));
					this->reorderChild(menu,1);
					menu->_isBottom=true;
					FadeOut* fo=FadeOut::create(0.3f);
					Spawn* seq=Spawn::createWithTwoActions(spn,fo);
					menu->runAction(seq);

				}else if(menu->getPositionY()==_pos02 && menu->_isBottom){

					ActionInterval* spn=MoveTo::create(0.5,Vec2(menu->getPositionX(),_pos01));
					this->reorderChild(menu,2);
					menu->setVisible(true);
					FadeIn* fi=FadeIn::create(0.3f);
					Spawn* seq=Spawn::createWithTwoActions(spn,fi);
					menu->_isBottom=false;
					menu->runAction(seq);
				}

				else if(menu->getPositionY()==_pos02  && !menu->_isBottom){
					Action* spn=Spawn::create(
						MoveTo::create(0.5,Vec2(menu->getPositionX(),_pos03)),
						ScaleTo::create(0.5,0.5),
						NULL
						);
					this->reorderChild(menu,2);
					menu->_isTop=false;
					menu->runAction(spn);
				}

			};

		};
		std::string src="";
		for(MenuButton* menu : _menu_array){

			if(menu->_isTop){
				switch(menu->getBtnType()){
				case Training:
					 src="menu02_text.png";
					break;
				case Network:
					 src="menu01_text.png";
					break;
				case Credits:
					 src="menu04_text.png";
					break;
				case Exit:
					 src="menu03_text.png";
					break;
				}

			}
		}

		SpriteFrame *frame=SpriteFrameCache::getInstance()->spriteFrameByName(src.c_str());
		menuText->setSpriteFrame(frame);

}


void StartMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		onBackKeyClicked();
	}
}

void StartMenu::onBackKeyClicked(){


	#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID

	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/DialogUtils","showTipDialog", "(Ljava/lang/String;Ljava/lang/String;)V");

	if (isHave)
	{

		jstring jTitle = minfo.env->NewStringUTF("Exit Game");
		jstring jMsg = minfo.env->NewStringUTF("Do you really want to exit?");
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jTitle, jMsg);
		minfo.env->DeleteLocalRef(jTitle);
		minfo.env->DeleteLocalRef(jMsg);
	}

	#endif


};

void StartMenu::onExitCallBack(){

	this->onBackKeyClicked();
}

