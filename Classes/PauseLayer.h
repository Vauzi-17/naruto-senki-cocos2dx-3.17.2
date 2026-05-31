#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"




USING_NS_CC;


class GameLayer;
class PauseLayer : public Layer{

public:
	PauseLayer(void);
	~PauseLayer(void);

	bool	init(RenderTexture* snapshoot);
	static PauseLayer*	create(RenderTexture* snapshoot);
	Menu* pauseMenu;
	Menu* soundMenu;
	Menu* preMenu;
	Layer* exitLayer;
	MenuItem* bgm_btn;
	MenuItem* voice_btn;
	MenuItem* pre_btn;
private:
	void	onResume(Ref* sender);
	void	onBackToMenu(Ref* sender);
	void	onLeft(Ref* sender);
	void	onCancel(Ref* sender);
	void    onBGM(Ref* sender);
	void    onPreload(Ref* sender);
	void    onVoice(Ref* sender);

	CC_SYNTHESIZE(GameLayer*,_delegate,Delegate);



};



