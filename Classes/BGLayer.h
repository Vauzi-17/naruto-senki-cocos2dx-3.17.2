#pragma once
#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;
class GameLayer;

class BGLayer: public Layer{

public:
	BGLayer(void);
	~BGLayer(void);

	Texture2D*	texMap;
	Sprite*		bgMap;

	GameLayer*			_delegate;

	void	setDelegate(GameLayer* layer);
	void	initBg();
	virtual bool init();

	CREATE_FUNC(BGLayer);

};
