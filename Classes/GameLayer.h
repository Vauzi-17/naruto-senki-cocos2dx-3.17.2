#pragma once
#include "cocos2d.h"
#include "LegacyData.h"
#include "SimpleAudioEngine.h"
#include "Defines.h"
#include "cocos-ext.h"
#include "PauseLayer.h"
#include "GearLayer.h"
#include "GameOver.h"

#include <vector>


/*#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif  */

USING_NS_CC;




class Hero;
namespace NarutoSenki { class ActionManager; }
class BGLayer;
class HudLayer;

using GameLayerLegacyArray = NarutoSenkiLegacy::Array;

class GameLayer :public Layer{
public:
	GameLayer(void);
	~GameLayer(void);

	TMXTiledMap*		currentMap;
	Hero*				currentPlayer;
	Vec2				spawnPoint;
	
	unsigned int _second;
	unsigned int _minute;
	int randomMap;
	int selectedMap;

	const char* kName;
	const char* aName;
    int kEXPBound;
	int aEXPBound;

	GameLayerLegacyArray* Heros;
	bool	_isAttackButtonRelease;
	bool	_isGuardian;
	GameLayerLegacyArray* _KonohaFlogArray;
	GameLayerLegacyArray* _AkatsukiFlogArray;
	GameLayerLegacyArray* _TowerArray;
	GameLayerLegacyArray* _CharacterArray;

	
	bool _isShacking;

	int _playNum;
	void checkBackgroundMusic(float dt);


	CC_SYNTHESIZE(HudLayer*,_hudLayer,HudLayer);

	CC_SYNTHESIZE(int,totalKills,TotalKills);
	CC_SYNTHESIZE(int,totalTM,TotalTM);

	SpriteBatchNode* skillEffectBatch;
	SpriteBatchNode* damageEffectBatch;
	SpriteBatchNode* bulletBatch;
	SpriteBatchNode* shadowBatch;

	bool isPosting;
	int postTime;

	
		
	bool init();
	void initTileMap();
	void initHeros();
	void initFlogs();
	void initTower();
	void initGard();
	void initEffects();

	//void update(float dt);
	void updateViewPoint(float dt);
	void updateGameTime(float dt);

	void addFlog(float dt);
	void onStart();
	
	void attackButtonClick(abType type);
	void gearButtonClick(gearType type);	
	void attackButtonRelease();
	

	void JoyStickRelease();
	void JoyStickUpdate(Vec2 direction);

	CC_SYNTHESIZE(bool,_isSkillFinish,SkillFinish);
	void checkPost(float dt);
	void checkTower();
	void checkBackgroundMusic();

	void onPause();
	void onGear();
	void onKaichang(float dt);
	void onGameStart(float dt);
	void onGameOver(bool isWin);

	void setHPLose(float percent);
	void setCKRLose(bool isCRK2);

	void setReport(const char* name1, const char* name2,const char* killNum);
	void clearDoubleClick();
	void resetStatusBar();
	void setCoin(const char* value);
	void removeOugisMark(int type);
	bool shouldUnlockAllOugis() const;
	void refreshOugisSkillLocks();
	void applyDuelTrainStartSetup();
	void fillCKRToMax(NarutoSenki::ActionManager* hero);
	bool _isUnlimitedCKR;
	void setOugis(Node* sender);
	void removeOugis();
	Node* ougisChar;

	Node* controlChar;
	
	Layer* blend;

	void onLeft();
	void onRematch();


	bool _isSurrender;

	bool _isOugis2Game;
	bool _isHardCoreGame;
	bool _isRandomChar;
	bool _isTrainingMode;
	bool _isDuelMode;
	bool _isDuelFinished;
	bool _isRematching;
	GameLayerLegacyArray* _rematchHerosOverride;
	int _duelKillLimit;
	int _duelPlayerScore;
	int _duelEnemyScore;
	bool onDuelKill(NarutoSenki::ActionManager* killer, NarutoSenki::ActionManager* victim);


	int zhenying;
	bool _isExiting;

	CREATE_FUNC(GameLayer);
	std::vector<char*> KeyList;
protected:
	void onHttpRequestCompleted(Node *sender ,void *data);
	virtual void	onEnter();
	virtual void	onExit();

};



