#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

namespace NarutoSenki { class ActionManager; }

class Effect : public Sprite{
public:
	Effect(void);
	~Effect(void);

	virtual bool			init(const char *szImage,Ref* Attacker);
	static Effect*			create(const char *szImage,Ref* Attacker);

	NarutoSenki::ActionManager* at;

private:
	
	void					removeEffect();
	void					removeFontEffect();
	Action*				createEffectAnimation(const char* file,int frameCount,float fps,bool isRepeat);
	Action*				createFontAnimation();
};



