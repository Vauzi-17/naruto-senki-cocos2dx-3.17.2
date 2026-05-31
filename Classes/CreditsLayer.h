#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Defines.h"
USING_NS_CC;


class GameLayer;
class CreditsLayer : public Layer{

public:
	CreditsLayer(void);
	~CreditsLayer(void);

	bool	init();

	CREATE_FUNC(CreditsLayer);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onBackKeyClicked();

protected:
	void onEnter();
	void onExit();
};


