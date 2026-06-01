#pragma once

#include "cocos2d.h"
#include "Defines.h"

USING_NS_CC;

class HudLayer;

class ActionButton : public Sprite{
public:
	ActionButton(void);
	~ActionButton(void);

	int						_clickNum;
	float					_clickTime;
	ProgressTimer*		markSprite;
	Sprite*				ougismarkSprite;
	Sprite*		proressmarkSprite;
	Sprite*		proressblinkSprite ;
	Sprite*		progressPointSprite;
	Sprite*       proressblinkMask;
	const char*				_cost;
	ClippingNode* clipper;
	bool					_isLock;

	bool					_isColdChanged;
	int						_coldDown;
	int						_timeCout;

	virtual bool			init(const char *szImage);
	static ActionButton*	create(const char *szImage);
	virtual bool			isCanClick();
	Rect					getRect();

	void					click();
	void					setMarkSprite(const char* mark);
	void					setOugisMark();
	void					setProgressMark();
	void					updateProgressMark();

	Sprite*		gearSign ;
	LabelBMFont*			cdLabel;

	LabelBMFont*			lockLabel1;
	LabelBMFont*			lockLabel2;

	bool					_isMarkVisable;

	CC_SYNTHESIZE(abType,_abType,ABType);
	gearType				_gearType;
	
	void setCD(CCString* value);
	void setCD(int value);
	int getCD() const;
	CC_SYNTHESIZE(bool,_isDoubleSkill,DoubleSkill);
	void setTimeCout(int value);
	int getTimeCout() const;
	

	void			setLock();
	void			unLock();
	void			clearOugisMark();
	void			setGearType(int tmpGearType);

	CC_SYNTHESIZE_RETAIN(Action*,_freezeAction,FreezeAction);
	//CC_SYNTHESIZE_RETAIN(ProgressTimer*,markSprite,MarkSprite);
	CC_SYNTHESIZE(HudLayer*,_delegate,Delegate);
	virtual void	beganAnimation(bool isLock = false);

	

protected:
	

	void			updateCDLabel(float dt);

	virtual void	onEnter();
	virtual void	onExit();
	virtual bool	ccTouchBegan(Touch* touch,Event* event);
	virtual void	ccTouchEnded(Touch* touch,Event* event);

	
	void		    createFreezeAnimation();
	void			clearClick();


	bool			containsTouchLocation(Touch* touch);

	

};



