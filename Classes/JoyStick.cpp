#include "JoyStick.h"
#include "HudLayer.h"

JoyStick::JoyStick(void)
{
	_delegate=NULL;

}


JoyStick::~JoyStick(void)
{
}


bool JoyStick::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());
		//CC_BREAK_IF(!Sprite:initWithSpriteFrameName("JoyStickBg.png"));
		this->initWithSpriteFrameName("JoyStickBg.png");
		this->setAnchorPoint(Vec2(0,0));


		_joyStickControl=Sprite::createWithSpriteFrameName("JoyStick.png");
	    _joyStickControl->setAnchorPoint(Vec2(0,0));

		_defaultPotion=Vec2(this->getContentSize().width/2-_joyStickControl->getContentSize().width/2,
			this->getContentSize().height/2-_joyStickControl->getContentSize().height/2);

		_joyStickControl->setPosition(_defaultPotion);
		this->addChild(_joyStickControl);

		bRet=true;

	}while(0);

	return bRet;

}

void JoyStick::onEnter(){
	Sprite::onEnter();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(JoyStick::ccTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(JoyStick::ccTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(JoyStick::ccTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
};

void JoyStick::onExit(){
	Sprite::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
};

bool JoyStick::ccTouchBegan(Touch* touch,Event* event){
	Vec2 curPoint = touch->getLocation();
	if (curPoint.x > winSize.width / 2 || curPoint.y > winSize.height / 2){
		return false;
	}

	return true;


};

void JoyStick::ccTouchMoved(Touch* touch,Event* event){
	if(!_delegate->_isAllButtonLocked){
		this->updateDirectionForTouchLocation(touch);
	}


};


void JoyStick::ccTouchEnded(Touch* touch,Event* event){
	if(!_delegate->_isAllButtonLocked){
	_joyStickControl->setPosition(_defaultPotion);
	_delegate->JoyStickRelease();
	}

};

void JoyStick::updateDirectionForTouchLocation(Touch* touch){

	Vec2 startPoint=Vec2(32+this->getContentSize().width/2,
		32+this->getContentSize().height/2);
	Vec2 curPoint = touch->getLocation();
	//向量
	Vec2 sp=(curPoint - startPoint);
	//弧度
	float radians = sp.getAngle();
	//角度
	float degrees = -1 * CC_RADIANS_TO_DEGREES(radians);
	float distance = curPoint.distance(startPoint);

	if (degrees <= 22.5 && degrees >= -22.5){
		//right
		_direction = Vec2(1.0, 0.0);}
	else if (degrees > 22.5 && degrees < 67.5){
		//bottom right
		_direction = Vec2(1.0, -1.0);}
	else if (degrees >= 67.5 && degrees <= 112.5){
		//bottom
		_direction = Vec2(0.0, -1.0);}
	else if (degrees > 112.5 && degrees < 157.5){
		//bottom left
		_direction = Vec2(-1.0, -1.0);}
	else if (degrees >= 157.5 || degrees <= -157.5){
		//left
		_direction = Vec2(-1.0, 0.0);}
	else if (degrees < -22.5 && degrees > -67.5){
		//top right
		_direction = Vec2(1.0, 1.0);}
	else if (degrees <= -67.5 && degrees >= -112.5){
		//top
		_direction = Vec2(0.0, 1.0);}
	else if (degrees < -112.5 && degrees > -157.5){
		//top left
		_direction = Vec2(-1.0, 1.0);};

		this->updateJoyStick(distance,_direction);
		_delegate->JoyStickUpdate(_direction);
}

void JoyStick::updateJoyStick(float distance,Vec2 direction){
	if (distance < 27.5) {
		_joyStickControl->setPosition((_defaultPotion + (direction * distance)));
	} /*else if (distance > 96) {
		_joyStickControl->setPosition((_defaultPotion + (direction * 53)));
	} */else {
		_joyStickControl->setPosition((_defaultPotion  + (direction * 26.5)));

	}

};

