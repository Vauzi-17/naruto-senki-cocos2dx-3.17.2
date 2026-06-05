#pragma once
#include "cocos2d.h"
#include "ActionManager.h"
#include "HPBar.h"
#include "cocos-ext.h"

class HPBar;

class HeroElement : public NarutoSenki::ActionManager  {
public:
	HeroElement(void);
	~HeroElement(void);

	//CREATE_FUNC(HeroElement);
	bool	init();
	void	initAction();
	void	setHPbar();
	void	setShadows();
	void    changeHPbar();
	void	applyStartLevel6();
	void	neicun(float dt);
	void	setID(__String* character,__String* role,__String* group);
	virtual void setAI(float dt);
	

	Sprite* rebornSprite;
	LabelBMFont* rebornLabel;
	void	reborn(float dt);
	void	dealloc();

protected: 
	
	
	void	countDown(float dt);
};


class Flog: public NarutoSenki::ActionManager{
public:
	Flog(void);
	~Flog(void);

	CREATE_FUNC(Flog);
	bool	init();
	void	initAction();
	void	setHPbar();

	float   _mainPosY;
	float	_randomPosX;
	float	_randomPosY;

	void	setID(__String* character,__String* role,__String* group);
	

protected: 
	void	dealloc();
	void	setAI(float dt);


};

class Tower: public NarutoSenki::ActionManager{
public:
	Tower(void);
	~Tower(void);

	CREATE_FUNC(Tower);
	bool	init();
	void	initAction();
	void	setHPbar();
	void	dealloc();    
	void	setID(__String* character,__String* role,__String* group);

};


class Monster: public NarutoSenki::ActionManager{
public:
	Monster(void);
	~Monster(void);

	CREATE_FUNC(Monster);
	bool	init();
	void	initAction(); 
	void	setID(__String* character,__String* role,__String* group);
	void	setHPbar();
	void    changeHPbar();
	void	setDirectMove(unsigned int length,float delay,bool isReverse);
	void	setEaseIn(unsigned int length,float delay);
	void	setDirectMoveBy(unsigned int length,float delay);
protected: 
	void	dealloc();
	void	dealloc2();
	void	setAI(float dt);
	void	setResume();
	
};

class Bullet: public NarutoSenki::ActionManager{
public:
	Bullet(void);
	~Bullet(void);

	CREATE_FUNC(Bullet);
	bool	init();
	void	initAction(); 
	void	setID(__String* character,__String* role,__String* group);
	
	void	setMove(unsigned int length,float delay,bool isReverse);
	void	setEaseIn(unsigned int length,float delay);
	
	void    setAttack(float dt);

protected: 
	
	void	dealloc();
	void    dealloc2();

};


