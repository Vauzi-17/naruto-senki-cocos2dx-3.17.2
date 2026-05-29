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
		CC_BREAK_IF(!CCLayer::init());

		bRet=true;

	}while(0);

	return bRet;

}

void CCScrewLayer::onEnter(){
	CCLayer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CCScrewLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CCScrewLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CCScrewLayer::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
};

void CCScrewLayer::onExit(){
	CCLayer::onExit();
	cocos2d::cocos2dx2_compat::removeDelegate(this);
};

bool CCScrewLayer::onTouchBegan(CCTouch* touch,CCEvent* event){

	prePosY=NULL;
	return true;

};

void CCScrewLayer::onTouchMoved(CCTouch* touch,CCEvent* event){
	// touch area 
	CCPoint curPoint = touch->getLocation();
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


void CCScrewLayer::onTouchEnded(CCTouch* touch,CCEvent* event){
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
		CC_BREAK_IF(!CCSprite::init());
		
		
		bRet=true;

	}while(0);

	return bRet;

}

void CCScrewItem::onEnter(){
	CCSprite::onEnter();
	cocos2d::cocos2dx2_compat::addTargetedDelegate(this,0,true);
}
void CCScrewItem::onExit(){
	CCSprite::onExit();
	cocos2d::cocos2dx2_compat::removeDelegate(this);
}

CCRect CCScrewItem::getRect2(){
	CCSize size=this->getContentSize();
	return CCRect(0,0, size.width, size.height);

}

bool CCScrewItem::containsTouchLocation(CCTouch *touch){

	return getRect2().containsPoint(convertTouchToNodeSpace(touch));
}

bool CCScrewItem::ccTouchBegan(CCTouch* touch,CCEvent* event){
	// touch area 
	if (!this->containsTouchLocation(touch))
		return false;


	return true;

}
void CCScrewItem::ccTouchEnded(CCTouch* touch,CCEvent* event){
	linemenu->setVisible(true);
};




