#pragma once
#include "cocos2d.h"
#include "Defines.h"
#include "JoyStick.h"
#include "GameLayer.h"
#include "Characters.h"
#include "Defines.h"
#include "ActionButton.h"
#include "SimpleAudioEngine.h"
#include <string>
#include <vector>

USING_NS_CC;

class GameLayer;

struct HudReportEntry {
	std::string name1;
	std::string name2;
	std::string kills;
	bool isDisplay;
};


class MiniIcon: public Sprite{
public: 
	MiniIcon(void);
	~MiniIcon();

	CC_SYNTHESIZE(unsigned int,_charNO,CharNO);
	static MiniIcon*	create(const char *szImage,bool isNotification);
	void	updateMap(EventCustom* event);
	void	updateState();

	void	updatePosition(Vec2 location);
	virtual bool init(const char *szImage,bool isNotification );

	Layer*	_delegate;
	EventListenerCustom* _updateMapListener;

};





class HudLayer : public Layer{
public:
	HudLayer(void);
	~HudLayer(void);


	void			updateSQL(float dt);
	bool			isOpenSQL;

	Sprite*			status_bar;
	Sprite*			status_hpbar;
	Sprite*			status_hpMark;
	Sprite*			enemy_status_bar;
	Sprite*			enemy_status_hpbar;
	Sprite*			enemy_status_hpMark;
	ProgressTimer*	status_expbar;
	
	ActionButton*		nAttackButton;
	ActionButton*		skill1Button;
    ActionButton*		skill2Button;
	ActionButton*		skill3Button;
	ActionButton*		skill4Button;
	ActionButton*		skill5Button;

	
	ActionButton*		item1Button;
		CC_SYNTHESIZE_RETAIN(ActionButton*,item2Button,Item2Button );
		CC_SYNTHESIZE_RETAIN(ActionButton*,item3Button,Item3Button );
		CC_SYNTHESIZE_RETAIN(ActionButton*,item4Button,Item4Button );
		/*ActionButton*		item2Button;
		ActionButton*		item3Button;
		ActionButton*		item4Button;*/
	ActionButton*		gear1Button;
	ActionButton*		gear2Button;
	ActionButton*		gear3Button;
	Sprite*			reportSPCSprite;

	Sprite* reportSprite;
	

	std::vector<HudReportEntry> _reportListArray;
	int _buffCount;

	LabelBMFont*		hpLabel;
	LabelBMFont*		expLabel;
	LabelBMFont*		enemyHpLabel;


	LabelBMFont*		coinLabel;
	LabelBMFont*		killLabel;
	LabelBMFont*		deadLabel;

	LabelBMFont*		KonoLabel;
	LabelBMFont*		AkaLabel;


    LabelBMFont*		gameClock;
	Menu*				pauseNenu;
	Menu*				gearMenu;
	std::vector<MiniIcon*> _towerIconArray;

	GameLayer*			_delegate;
	Layer*			miniLayer;
	JoyStick*			_joyStick;

	Texture2D* 		texUI;
	SpriteBatchNode*	uiBatch;

	void	setHPLose(float percent);
	void	setEnemyHPLose(float percent);
	void	setCKRLose(bool isCRK2);
	void	setEXPLose(float percent);

	void	setDelegate(GameLayer* layer);
	void	initHeroInterface();
	void	attackButtonClick(abType type);
	void	gearButtonClick(gearType type);
	void	attackButtonRelease();
	void	pauseButtonClick(Ref* sender);
	void	gearButtonClick(Ref* sender);
	void	setReport(const char* name1, const char* name2,const char* killNum);
	void	setReportCache();
	void    setBuffDisplay(const char* buffName,float buffStayTime);
	void	clearSPCReport();
	void	clearBuffDisplay(Node* sender);
	
	void	updateBuffDisplay(float dt);
	void	updateBuffDisplay2(float dt);
	void	stopSPCReport();
	void	updateSPCReprot(float dt);
	void	setTowerState(unsigned int charNO);
	void	onKaichang();

	LabelBMFont* bcdLabel1;
	LabelBMFont* bcdLabel2;
	
	void JoyStickRelease();
	void JoyStickUpdate(Vec2 direction);
	Sprite* Kaichang;

	Sprite* createReport(const char* name1, const char* name2,float &length);
	Sprite* createSPCReport(const char* killNum,int num);

	

	virtual bool	getSkillFinish();
	virtual bool    getOugisEnable(bool isCKR2);
	
	Layer* ougisLayer;
	void setOugis(const char* character,const char* group);
	void removeOugis(Node* sender);

	bool	_isAllButtonLocked;
	
	void addMapIcon();

	void   costCKR(int value,bool isCKR2);
	void   setCoin(const char* value);
	bool   offCoin(const char* value);

	
	virtual bool init();
	
	CREATE_FUNC(HudLayer);

	void updateGears();
	void initGearButton();

protected:
	virtual void	onEnter();
	virtual void	onExit();
  
};



