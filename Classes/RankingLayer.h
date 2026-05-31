#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Defines.h"
#include "cocos-ext.h"
#include "MyUtils/CCScrewLayer.h"
#include "MyUtils/CCStrokeLabel.h"

USING_NS_CC;

class NetworkLayer;
using namespace rapidjson; 

class RakingLayer : public Layer{

public:
	RakingLayer(void);
	~RakingLayer(void);

	bool	init();


	CREATE_FUNC(RakingLayer);

	MenuItem* day_btn;
	MenuItem* week_btn;
	MenuItem* month_btn;


	MenuItem* battlePoint_btn;
	MenuItem* JifenPoint_btn;

	MenuItem* side0_btn;
	MenuItem* side1_btn;
	MenuItem* side2_btn;
	MenuItem* side3_btn;

	Menu* order_menu;
	Sprite* ranking_title;
	Label* rankingDetail;

	Layer* ranking_layer;
	Sprite* ranking_bg;
	CCScrewLayer* _screwLayer1;


	SpriteBatchNode* rankingBatch;

	bool isPosting;
	

	Sprite* refreshBtn;
	CC_SYNTHESIZE(NetworkLayer*,_delegate,Delegate);

	void postRequest(int code,CCString* useChar=NULL);

	bool				isMemberSelect;
	bool				isGroupSelect;
	int postingCode;

private:
	void produceRanking(int type);

	void				onDayBtn(Ref* sender);
	void				onWeekBtn(Ref* sender);
	void				onMonthBtn(Ref* sender);

	void				onBattleBtn(Ref* sender);
	void				onJifenBtn(Ref* sender);
	void				onSelectGroupBtn(Ref* sender);

	void				onSideBar1(Ref* sender);
	void				onSideBar2(Ref* sender);
	void				onSideBar3(Ref* sender);
	void				onSideBar0(Ref* sender);
	void				onCancel(Ref* sender);

	
	void				makeCharList();

};



class RankCharButton : public Sprite{

public:
	RankCharButton(void);
	~RankCharButton(void);


	virtual bool		init(const char *szImage);
	static RankCharButton*	create(const char *szImage);

	Rect				getRect();
	void				setBtnType(CCString* buttonChar);

	CC_SYNTHESIZE(CCString*,_btnType,btnType);

	CC_SYNTHESIZE(RakingLayer*,_delegate,Delegate);


protected:

	virtual void	onEnter();
	virtual void	onExit();
	virtual bool	onTouchBegan(Touch* touch, Event* event);
	virtual void    onTouchMoved(Touch* touch, Event* event);
	virtual void	onTouchEnded(Touch* touch, Event* event);

	bool			containsTouchLocation(Touch* touch);


};

