#pragma once
#include "cocos2d.h"
#include "ActionManager.h"


class HPBar :public Sprite{
	HPBar(void);
	~HPBar(void);

	CC_SYNTHESIZE(NarutoSenki::ActionManager*,_delegate,Delegate);
	virtual bool		init(const char *szImage);
	static HPBar*	create(const char *szImage);
	void			changeBar(const char *szImage);
	void			loseHP(float percent);
	CC_SYNTHESIZE_RETAIN(Sprite*,hpBottom,HPBottom);
	CC_SYNTHESIZE_RETAIN(Sprite*,hpBar,HPBAR);


};



