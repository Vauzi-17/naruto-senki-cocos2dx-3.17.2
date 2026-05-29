#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Defines.h"
#include "cocos-ext.h"
/*#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif  */
USING_NS_CC;


class GameLayer;
class Hero;
class GameOver : public CCLayer{

public:
	GameOver(void);
	~GameOver(void);

	bool	init(CCRenderTexture* snapshoot);
	CC_SYNTHESIZE(bool,_isWin,Win);

	static GameOver*	create(CCRenderTexture* snapshoot);
	
	CCLayer* exitLayer;
	CCLayer* cheatLayer;
	CCSprite* result_bg;

	bool isPosting;
	CCSprite* refreshBtn;
	CCLayer* changeCharLayer;
	CCString* changePlayerPick;
	CCString* changeEnemyPick;
	bool changeTargetEnemy;

	CCMenuItem* upload_btn;

	//CC_SYNTHESIZE_RETAIN(CCString*,detailRecord,DetialRecord);
	 std::string detailRecord;
	float finnalScore;
	CC_SYNTHESIZE_RETAIN(CCString*,resultChar,ResultChar);

private:
	void	onBackToMenu(CCObject* sender);
	void	listResult();
	void	onCancel(CCObject* sender);
	void	onLeft(CCObject* sender);
	void	onRematch(CCObject* sender);
	void	onChangeChar(CCObject* sender);
	void	onChangeCharTarget(CCObject* sender);
	void	onChangeCharPick(CCObject* sender);
	void	onChangeCharConfirm(CCObject* sender);
	void	onChangeCharCancel(CCObject* sender);
	void	onUPloadBtn(CCObject* sender);
	CC_SYNTHESIZE(GameLayer*,_delegate,Delegate);
	void onHttpRequestCompleted(CCNode *sender ,void *data);
};



