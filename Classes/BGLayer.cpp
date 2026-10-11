#include "BGLayer.h"
#include "Defines.h"


BGLayer::BGLayer(void)
{
	texMap=NULL;
}



BGLayer::~BGLayer(void)
{
}


bool BGLayer::init(){
	bool bRet=false;
	do{

		CC_BREAK_IF(!Layer::init());

		bRet=true;

	}while(0);

	return bRet;

}

void BGLayer::initBg(){

	if(_delegate->randomMap==0){
		bgMap=Sprite::create("Tiles/map_bg1.png");
	}else if(_delegate->randomMap==1){
		bgMap=Sprite::create("Tiles/map_bg2.png");
	}else if(_delegate->randomMap==2){
		bgMap=Sprite::create("Tiles/map_bg3.png");
	}else if(_delegate->randomMap==3){
		bgMap=Sprite::create("Tiles/map_bg4.png");
	}else if(_delegate->randomMap==4){
		bgMap=Sprite::create("Tiles/map_bg5.png");
	}



	bgMap->setAnchorPoint(Vec2(0,0));
	scaleSpriteToScreenWidth(bgMap);
	bgMap->setPosition(Vec2(0,192));

	this->addChild(bgMap);

}

void BGLayer::setDelegate(GameLayer* layer){
	_delegate=layer;
	this->initBg();
}
