#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "LoadLayer.h"
#include "SkillLayer.h"
#include "RankingLayer.h"
#include <string>
#include <vector>
USING_NS_CC;

class NetworkLayer : public Layer{

public:
	NetworkLayer(void);
	~NetworkLayer(void);

	virtual bool		init();
	static NetworkLayer*	create();
	static NetworkLayer*	createTraining(int teamSize, bool randomTeam, bool randomEnemy, bool trainingMode, int killLimit);
	void    setSelected(Ref* sender);
	bool    isHeroConfirmedSelected(const char* heroName);
	void    unselectHero(const char* heroName);
	void    unselectAll();
	void    refreshTeamSelectorUI();
	void    refreshTrainingTeamSlots();
	void    refreshTrainingEnemySlots();
	SelectButton* findSelectButton(const char* heroName);

	const char* _selectHero;
	const char* _playerSelect;
	const char* _com1Select;
	const char* _com2Select;
	std::string _trainingPlayerPick;
	std::vector<std::string> _trainingTeamPicks;
	std::vector<std::string> _trainingEnemyPicks;
	Sprite* _trainingTeamSlots[5];
	Sprite* _trainingEnemySlots[5];

	bool isPosting;
	Sprite* refreshBtn;


	Layer* _teamSelector;

	CC_SYNTHESIZE_RETAIN(Sprite*,_comSelector1,ComSelector1);
	CC_SYNTHESIZE_RETAIN(Sprite*,_comLabel1,ComLabel1);


	CC_SYNTHESIZE_RETAIN(Sprite*,_comSelector2,ComSelector2);
	CC_SYNTHESIZE_RETAIN(Sprite*,_comLabel2,ComLabel2);

	CCArray* selectArray;

	CC_SYNTHESIZE_RETAIN(Layer*,_pageLayer1,Page1);
	CC_SYNTHESIZE_RETAIN(Layer*,_pageLayer2,Page2);
	CC_SYNTHESIZE_RETAIN(Layer*,_pageLayer3,Page3);


	MenuItemSprite* page1_btn;
	MenuItemSprite* page2_btn;
	MenuItemSprite* page3_btn;

	void  onPage1Btn(Ref* sender);
	void  onPage2Btn(Ref* sender);
	void  onPage3Btn(Ref* sender);
	void  onMapBtn(Ref* sender);
	void  updateMapPreview();

	CC_SYNTHESIZE_RETAIN(CCArray*,_selectList,SelectList);
	Menu*  page_menu;

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
	Sprite* _mapPreview;
	LabelBMFont* _mapLabel;


	CC_SYNTHESIZE_RETAIN(Sprite*,_heroHalf,HeroHalf);
	CC_SYNTHESIZE_RETAIN(Sprite*,_heroName,HeroName);
	CC_SYNTHESIZE_RETAIN(Sprite*,_selectImg,SelectImg);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onBackKeyClicked();

protected:
	void onEnter();
	void onExit();

private:
	void	onUnselectAllBtn(Ref* sender);
	void	onRankingBtn(Ref* sender);
	void	onQuestBtn(Ref* sender);
	void	onGameStart(Ref* sender);
	void    onSkillMenu(Ref* sender);
	void	onError(Ref* sender);
	void	onBackToMenu(Ref* sender);
	void	onListRequestCompleted(Node *sender ,void *data);

};
