#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "LoadLayer.h"
#include "SkillLayer.h"

USING_NS_CC;



class SelectLayer : public Layer{

public:
	SelectLayer(void);
	~SelectLayer(void);

	virtual bool		init();
	static SelectLayer*	create();
	void    setSelected(Ref* sender);

	const char* _selectHero;
	const char* _playerSelect;


	CCArray* selectArray;

	bool isStart;
	bool SaveError;
	bool _isHardCoreMode;
	bool _isTrainingMode;
	int _trainingTeamSize;
	bool _trainingRandomTeam;
	bool _trainingRandomEnemy;
	int num2;
	CC_SYNTHESIZE_RETAIN(Sprite*,_heroHalf,HeroHalf);
	CC_SYNTHESIZE_RETAIN(Sprite*,_heroName,HeroName);
	CC_SYNTHESIZE_RETAIN(Sprite*,_selectImg,SelectImg);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onBackKeyClicked();

protected:
	void onEnter();
	void onExit();

private:
	void	onGameStart(Ref* sender);
	void    onSkillMenu(Ref* sender);
	void	onError(Ref* sender);
	void	onBackToMenu(Ref* sender);
};
