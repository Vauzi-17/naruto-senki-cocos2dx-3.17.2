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
class GameOver : public Layer{

public:
	GameOver(void);
	~GameOver(void);

	bool	init(RenderTexture* snapshoot);
	CC_SYNTHESIZE(bool,_isWin,Win);

	static GameOver*	create(RenderTexture* snapshoot);
	
	Layer* exitLayer;
	Layer* cheatLayer;
	Sprite* result_bg;

	bool isPosting;
	Sprite* refreshBtn;
	Layer* changeCharLayer;
	CCString* changePlayerPick;
	CCString* changeEnemyPick;
	bool changeTargetEnemy;

	MenuItem* upload_btn;

	//CC_SYNTHESIZE_RETAIN(CCString*,detailRecord,DetialRecord);
	 std::string detailRecord;
	float finnalScore;
	CC_SYNTHESIZE_RETAIN(CCString*,resultChar,ResultChar);

private:
	void	onBackToMenu(Ref* sender);
	void	listResult();
	void	onCancel(Ref* sender);
	void	onLeft(Ref* sender);
	void	onRematch(Ref* sender);
	void	onChangeChar(Ref* sender);
	void	onChangeCharTarget(Ref* sender);
	void	onChangeCharPick(Ref* sender);
	void	onChangeCharConfirm(Ref* sender);
	void	onChangeCharCancel(Ref* sender);
	void	onUPloadBtn(Ref* sender);
	CC_SYNTHESIZE(GameLayer*,_delegate,Delegate);
	void onHttpRequestCompleted(Node *sender ,void *data);
};



