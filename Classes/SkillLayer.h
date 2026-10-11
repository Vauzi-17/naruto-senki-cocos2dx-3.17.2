#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <string>
USING_NS_CC;


// declare menuButton
enum sbtnType{
	SkillMenu,
	Unlock1,
	Unlock2,
	skill1,
	skill2,
	skill3,
	Ougis1,
	Ougis2
};
class SelectLayer;
class NetworkLayer;
class SkillLayer : public Layer{

public:
	SkillLayer(void);
	~SkillLayer(void);

	bool	init();

	void initInterface();

	CREATE_FUNC(SkillLayer);
	Sprite* _skillExplain;

	bool isPosting;
	Sprite* refreshBtn;

	LabelBMFont* coinLabel;

	Layer* buyLayer;
	sbtnType _buyType;
	Sprite* _heroFull;
	Menu* changemenu;
	bool _autoMove;

	Sprite* fengSprite;
	Sprite* fengSprite2;
	Sprite* skillGroup;

	Sprite* skillSprite;

	void update(float dt);

	const char* selectHero;


	void	setSkillExplain(sbtnType type);
	ProgressTimer* bonds;

	CC_SYNTHESIZE(SelectLayer*,_delegate,Delegate);
	CC_SYNTHESIZE(NetworkLayer*,_delegate2,Delegate2);
private:

	void    onCancel(Ref* sender);

	void	updateSkillGroup();
	void	onChangeBtn(Ref* sender);
	void	onResume(Ref* sender);
	void	onSkillRequestCompleted(Node *sender ,void *data);
};



class SelectButton : public Sprite {

public:
	SelectButton(void);
	~SelectButton(void);

	std::string _charName;
	const char* getCharName() const;
	void setCharName(const char* charName);

	bool _isAviable;
	virtual bool		init(const char *szImage);
	static SelectButton*	create(const char *szImage);
	Rect				getRect();
	void				click();
	int				_clickTime;
	CC_SYNTHESIZE(sbtnType,_btnType,BtnType);

	bool				_isCanBuy;
	bool				_isCanBuy2;
	bool				_isUnlock;

	CC_SYNTHESIZE(SelectLayer*,_delegate1,Delegate1);
	CC_SYNTHESIZE(NetworkLayer*,_delegate2,Delegate2);
	CC_SYNTHESIZE(SkillLayer*,_delegate3,Delegate3);

	void    spriteToGrey();
	void    spriteResetGrey();
protected:

	virtual void	onEnter();
	virtual void	onExit();
	virtual bool	ccTouchBegan(Touch* touch,Event* event);
	virtual void	ccTouchEnded(Touch* touch,Event* event);


	bool			containsTouchLocation(Touch* touch);



};
