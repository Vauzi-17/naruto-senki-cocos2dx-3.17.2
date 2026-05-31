#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Defines.h"

USING_NS_CC;

class GameLayer;
class Hero;
class ScrewLayer;


enum gearbtnType{
	buyType,
	sellType
};

class GearLayer : public Layer{

public:
	GearLayer(void);
	~GearLayer(void);

	bool	init(RenderTexture* snapshoot);
	static GearLayer*	create(RenderTexture* snapshoot);
	
	Layer* gears_layer;
	Layer* currentGear_layer;
	LabelBMFont* coinLabel;

	Sprite* gearDetail;
	Sprite* gears_bg;

	gearType currentGear;
	void	updatePlayerGear();
	void	updateGearList();

	ScrewLayer* _screwLayer;
	Sprite* _buyBtn;

private:
	void	setupBuyButton(Sprite* gearsBg);
	void	onResume(Ref* sender);
	void	onGearBuy(Ref* sender);
	
	CC_SYNTHESIZE(GameLayer*,_delegate,Delegate);
	

};





class GearButton : public Sprite{

public:
	GearButton(void);
	~GearButton(void);

	gearType				_gearType;
	gearbtnType  			_btnType;

	virtual bool		init(const char *szImage);
	static GearButton*	create(const char *szImage);

	Rect				getRect();
	void				setBtnType(gearType type,gearbtnType type2,bool isBuyed);
	gearType 			getBtnType();
	void				playSound();

	bool				_isBuyed;

	CC_SYNTHESIZE(GearLayer*,_delegate,Delegate);

	Sprite* soIcon;

	bool			containsTouchLocation(Touch* touch);

protected:

	virtual void	onEnter();
	virtual void	onExit();
	virtual bool	ccTouchBegan(Touch* touch,Event* event);
	virtual void    ccTouchMoved(Touch* touch, Event* event);
	virtual void	ccTouchEnded(Touch* touch,Event* event);



};



class ScrewLayer : public Layer{

public:
	ScrewLayer(void);
	~ScrewLayer(void);

	float			prePosY;
	virtual	bool	init();
	CREATE_FUNC(ScrewLayer);

	CC_SYNTHESIZE(GearLayer*,_delegate,Delegate);

	Sprite* screwBar;

	int totalRow;
	int gearNum;
	CC_SYNTHESIZE_RETAIN(CCArray*,_gearArray,GearArray);

protected:

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	bool isTouchInScrollArea(Touch* touch);

	void onEnter();
	void onExit();


};


