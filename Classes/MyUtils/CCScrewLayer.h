#pragma once
#include "cocos2d.h"
USING_NS_CC;  

class CCScrewItem;

class CCScrewLayer : public Layer{

public:
	CCScrewLayer(void);
	~CCScrewLayer(void);

	float			prePosY;
	virtual	bool	init();
	CREATE_FUNC(CCScrewLayer);
	const Vector<Node*>& getItemArray() const;
	void clearItemArray();
	void addItem(CCScrewItem* item);
	float minY;
	int totalRow;
	int line_height;

protected:

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);


	void onEnter();
	void onExit();

private:
	Vector<Node*> itemArray;

};


class CCScrewItem : public Sprite{

public:
	CCScrewItem(void);
	~CCScrewItem(void);
	virtual	bool	init();

	CREATE_FUNC(CCScrewItem);
	Rect			getRect2();
	Menu*      linemenu;
	CC_SYNTHESIZE(CCScrewLayer*,_delegate,Delegate);
	Sprite*	lockItem;
protected:

	virtual void	onEnter();
	virtual void	onExit();
	virtual bool	onTouchBegan(Touch* touch, Event* event);
	virtual void	onTouchEnded(Touch* touch, Event* event);


	bool			containsTouchLocation(Touch* touch);



};





