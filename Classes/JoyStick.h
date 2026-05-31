#pragma once
#include "cocos2d.h"
#include "Defines.h"


USING_NS_CC;

class HudLayer;

class JoyStick : public Sprite {
public:
	JoyStick(void);
	~JoyStick(void);


	Vec2		    _direction;
	Sprite*		_joyStickControl;
	Vec2			 _defaultPotion;

	virtual	bool	init();
	virtual void	onEnter();
	virtual void	onExit();


	void			keyUpdate(float direction);
	void			keyEnded(float direction);

	CREATE_FUNC(JoyStick);
	CC_SYNTHESIZE(HudLayer*,_delegate,Delegate);


protected:



	virtual bool	ccTouchBegan(Touch* touch,Event* event);
	virtual void    ccTouchMoved(Touch* touch, Event* event);
	virtual void	ccTouchEnded(Touch* touch,Event* event);

	void updateDirectionForTouchLocation(Touch* touch);
	void updateJoyStick(float distance,Vec2 direction);

};
