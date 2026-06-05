#pragma once

#include "cocos2d.h"

namespace NarutoSenkiPayload {

inline void addHero(cocos2d::__Array* heros, const char* character, const char* role, const char* group)
{
	cocos2d::__Dictionary* dic = cocos2d::__Dictionary::create();
	dic->setObject(cocos2d::__String::create(character), "character");
	dic->setObject(cocos2d::__String::create(role), "role");
	dic->setObject(cocos2d::__String::create(group), "group");
	heros->addObject(dic);
}

}
