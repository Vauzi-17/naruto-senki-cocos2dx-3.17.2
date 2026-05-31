#include "CCScrewLayer.h"


CCScrewLayer::CCScrewLayer(void)
{
	itemArray=NULL;
	totalRow=100;
	minY=154;
	line_height=26;
}


CCScrewLayer::~CCScrewLayer(void)
{
}


bool CCScrewLayer::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Layer::init());

		bRet=true;

	}while(0);

	return bRet;

}

void CCScrewLayer::onEnter(){
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CCScrewLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CCScrewLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CCScrewLayer::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
};

void CCScrewLayer::onExit(){
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	Layer::onExit();
};

bool CCScrewLayer::onTouchBegan(Touch* touch, Event* event){

	prePosY=NULL;
	return true;

};

void CCScrewLayer::onTouchMoved(Touch* touch, Event* event){
	// touch area 
	Vec2 curPoint = touch->getLocation();
	if(prePosY==NULL){
		prePosY=curPoint.y;
	}else{
		float distanceY=curPoint.y-prePosY;
		if(this->getPositionY()<totalRow*line_height || distanceY<0){
			this->setPositionY(this->getPositionY()+distanceY);

		}

		prePosY=curPoint.y;

	}

};


void CCScrewLayer::onTouchEnded(Touch* touch, Event* event){
	prePosY=NULL;

	//CCLOG("y:%f",this->getPositionY());
	if(this->getPositionY()>totalRow*line_height-line_height){
		this->setPositionY(totalRow*line_height);
	}
	if(this->getPositionY()<minY){
		this->setPositionY(minY);
	}


};



/*----------------------
init CCScrewItem ;
----------------------*/
CCScrewItem::CCScrewItem(void)
{
	 linemenu=NULL;
	 lockItem=NULL;
}


CCScrewItem::~CCScrewItem(void)
{

}

bool CCScrewItem::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());
		
		
		bRet=true;

	}while(0);

	return bRet;

}

void CCScrewItem::onEnter(){
	Sprite::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CCScrewItem::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(CCScrewItem::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
void CCScrewItem::onExit(){
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	Sprite::onExit();
}

Rect CCScrewItem::getRect2(){
	Size size=this->getContentSize();
	return Rect(0,0, size.width, size.height);

}

bool CCScrewItem::containsTouchLocation(Touch *touch){

	return getRect2().containsPoint(convertTouchToNodeSpace(touch));
}

bool CCScrewItem::onTouchBegan(Touch* touch, Event* event){
	// touch area 
	if (!this->containsTouchLocation(touch))
		return false;


	return true;

}
void CCScrewItem::onTouchEnded(Touch* touch, Event* event){
	if (linemenu)
	{
		linemenu->setVisible(true);
	}
};




