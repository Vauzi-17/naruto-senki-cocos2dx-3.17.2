#pragma once
#include "Defines.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SelectLayer.h"
#include "NetworkLayer.h"
#include "CreditsLayer.h"

#include "cocos-ext.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "MyUtils/KTools.h"
#include "MyUtils/CCStrokeLabel.h"
#include "MyUtils/CCScrewLayer.h"
#include <vector>


USING_NS_CC;


// declare menuButton
enum btnType{
	Network,
	Training,
	Exit,
	Credits,
	HardCore
};

class StartMenu;

class MenuButton : public Sprite {

public:
	MenuButton(void);
	~MenuButton(void);

	bool				_isTop;
	bool				_isBottom;
	btnType				_btnType;

	virtual bool		init(const char *szImage);
	static MenuButton*	create(const char *szImage);

	Rect				getRect();


	void				setBtnType(btnType type);
	btnType 			getBtnType();
	void				playSound();

	float			prePosY;



	CC_SYNTHESIZE(StartMenu*,_delegate,Delegate);





protected:

	virtual void	onEnter();
	virtual void	onExit();
	virtual bool	ccTouchBegan(Touch* touch,Event* event);
	virtual void    ccTouchMoved(Touch* touch, Event* event);
	virtual void	ccTouchEnded(Touch* touch,Event* event);

	bool			containsTouchLocation(Touch* touch);

};







class StartMenu : public Layer{

public:
	StartMenu(void);
	~StartMenu(void);



	bool				init();
	void				onTrainingCallBack();
	void				showTrainingOptions();
	void				refreshTrainingOptions();
	void				onTrainingModeOption(Ref* sender);
	void				onTrainingTeamOption(Ref* sender);
	void				onTrainingEnemyOption(Ref* sender);
	void				onTrainingKillOption(Ref* sender);
	void				onTrainingPixelOption(Ref* sender);
	void				onTrainingStart(Ref* sender);
	void				startTrainingBattle(int teamSize, bool randomTeam, bool randomEnemy, bool trainingMode, int killLimit);
	void				onHardCoreOn(Ref* sender);
	void				onHardCoreOff(Ref* sender);
	void				onHardLayerCallBack();
	void				onHardCoreCallBack();
	void				onNormalCallBack(Ref* sender);
	void				onCreditsCallBack();
	void				onExitCallBack();
	void				onNewsBtn(Ref* sender);
	void				onLoginBtn(Ref* sender);
	void				onTasksBtn(Ref* sender);
	void				onProfileBtn(Ref* sender);

	void				onIterfaceClose(Ref* sender);
	void				onInputBoxClose(Ref* sender);


	void				onLoginOn(Ref* sender);
	void				onLoginCancel(Ref* sender);
	void				setOfferWall();
	void				setProfile();
	void				setGroup();

	void				onGroupBtn(Ref* sender);
	void				onSearchBtn(Ref* sender);
	void				onCreateBtn(Ref* sender);
	void				onCreateComfirm(Ref* sender);
	void				onSelectGroupBtn(Ref* sender);
	void				onJoinGroup(Ref* sender);
	void				onKickBtn(Ref* sender);
	void				onQuitBtn(Ref* sender);
	void				onJoinComfirm(Ref* sender);
	void				onDissolveBtn(Ref* sender);
	void				onQuitComfirm(Ref* sender);
	void				onKickComfirm(Ref* sender);

	void				onGPWComfirm(Ref* sender);
	void				onLockBtn(Ref* sender);
	void				onNoticeBtn(Ref* sender);
	void				onNoticeComfirm(Ref* sender);

	void				onNamePlate(Ref* sender);
	void				onNamePlateClick(Ref* sender);
	void				onNamePlateComfrim(Ref* sender);
	void				scrollMenu(int posY);
	Sprite*			menuText;

	MenuButton* hardCore_btn;

	Layer* hardCoreLayer;


	Layer* profile_layer;
	Layer* group_layer;

	Layer* input_layer;

	cocos2d::ui::EditBox* usernameInput;
	cocos2d::ui::EditBox* pwInput;
	cocos2d::ui::EditBox* searchInput;
	cocos2d::ui::EditBox* nameInput;
	cocos2d::ui::EditBox* gpwInput;

	static const int	_pos01=48;
	static const int	_pos02=92;
	static const int	_pos03=150;

	bool  isClockwise;
	bool  isDrag;
	bool  isNewAviable;
	bool  isBondAviable;

	bool isTasking;
	bool isPosting;
	int trainingBattleSize;
	bool trainingRandomTeam;
	bool trainingRandomEnemy;
	bool trainingModeOnly;
	int trainingKillLimit;

	int leaderID;

	MenuItem*  lock_btn;
	bool isLock;
	 Label* notice_label;

	Label* totalGroupNum;
	int maxGroupNum;

	Sprite* refreshBtn;
	MenuItem* task_btn;
	MenuItem* login_btn;
	void setNotice();

	Layer* notice_layer;
	void update(float dt);
	Label* noticeLabel;

	CCScrewLayer* _screwLayer1;


	virtual void   onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onBackKeyClicked();

	std::vector<MenuButton*> _menu_array;
	CREATE_FUNC(StartMenu);


	void onLoginRequestCompleted(Node *sender ,void *data);
	void onGroupRequestCompleted(Node *sender ,void *data);
	void onGroupManageRequestCompleted(Node *sender ,void *data);
	void onJoinRequestCompleted(Node *sender ,void *data);
	void onSeachRequestCompleted(Node *sender ,void *data);
	void onCreateRequestCompleted(Node *sender ,void *data);
	void onQuitGroupRequestCompleted(Node *sender ,void *data);
	void onNoticeRequestCompleted(Node *sender ,void *data);
	void onProfileRequestCompleted(Node *sender ,void *data);
	void onGPWRequestCompleted(Node *sender ,void *data);
	void onNamePlateRequestCompleted(Node *sender ,void *data);

protected:
	void onEnter();
	void onExit();


};
