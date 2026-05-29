#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "LoadLayer.h"
#include "SkillLayer.h"
#include "RankingLayer.h"
USING_NS_CC;

class NetworkLayer : public CCLayer{

public:
	NetworkLayer(void);
	~NetworkLayer(void);

	virtual bool		init();
	static NetworkLayer*	create();
	static NetworkLayer*	createTraining(int teamSize, bool randomTeam, bool randomEnemy, bool trainingMode, int killLimit);
	void    setSelected(CCObject* sender);

	const char* _selectHero;
	const char* _playerSelect;
	const char* _com1Select;
	const char* _com2Select;
	CCString* _trainingPlayerPick;
	CCArray* _trainingTeamPicks;
	CCArray* _trainingEnemyPicks;
	CCSprite* _trainingTeamSlots[5];
	CCSprite* _trainingEnemySlots[5];

	bool isPosting;
	CCSprite* refreshBtn;
	
	
	CCLayer* _teamSelector;
	
	CC_SYNTHESIZE_RETAIN(CCSprite*,_comSelector1,ComSelector1);
	CC_SYNTHESIZE_RETAIN(CCSprite*,_comLabel1,ComLabel1);
	

	CC_SYNTHESIZE_RETAIN(CCSprite*,_comSelector2,ComSelector2);
	CC_SYNTHESIZE_RETAIN(CCSprite*,_comLabel2,ComLabel2);

	CCArray* selectArray;

	CC_SYNTHESIZE_RETAIN(CCLayer*,_pageLayer1,Page1);
	CC_SYNTHESIZE_RETAIN(CCLayer*,_pageLayer2,Page2);
	CC_SYNTHESIZE_RETAIN(CCLayer*,_pageLayer3,Page3);


	CCMenuItemSprite* page1_btn;
	CCMenuItemSprite* page2_btn;
	CCMenuItemSprite* page3_btn;

	void  onPage1Btn(CCObject* sender);
	void  onPage2Btn(CCObject* sender);
	void  onPage3Btn(CCObject* sender);
	void  onMapBtn(CCObject* sender);
	void  updateMapPreview();

	CC_SYNTHESIZE_RETAIN(CCArray*,_selectList,SelectList);
	CCMenu*  page_menu;

	bool isStart;
	bool SaveError;
	bool _isHardCoreMode;
	bool _isRandomChar;
	bool _isTrainingSelect;
	bool _isTrainingMode;
	int _trainingTeamSize;
	bool _trainingRandomTeam;
	bool _trainingRandomEnemy;
	int _trainingKillLimit;
	int _selectedMap;
	CCSprite* _mapPreview;
	CCLabelBMFont* _mapLabel;
	

	CC_SYNTHESIZE_RETAIN(CCSprite*,_heroHalf,HeroHalf);
	CC_SYNTHESIZE_RETAIN(CCSprite*,_heroName,HeroName);
	CC_SYNTHESIZE_RETAIN(CCSprite*,_selectImg,SelectImg);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onBackKeyClicked();

private:
	void	onRankingBtn(CCObject* sender);
	void	onQuestBtn(CCObject* sender);
	void	onGameStart(CCObject* sender);
	void    onSkillMenu(CCObject* sender);
	void	onError(CCObject* sender);
	void	onBackToMenu(CCObject* sender);
	void	onListRequestCompleted(CCNode *sender ,void *data);
	
};



