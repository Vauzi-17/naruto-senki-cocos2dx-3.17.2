#pragma once

#include "cocos2d.h"

namespace NarutoSenkiPayload {

inline cocos2d::__Dictionary* asHero(cocos2d::Ref* payload)
{
	return static_cast<cocos2d::__Dictionary*>(payload);
}

inline const char* value(cocos2d::Ref* payload, const char* key)
{
	return asHero(payload)->valueForKey(key)->getCString();
}

inline const char* character(cocos2d::Ref* payload)
{
	return value(payload, "character");
}

inline const char* role(cocos2d::Ref* payload)
{
	return value(payload, "role");
}

inline const char* group(cocos2d::Ref* payload)
{
	return value(payload, "group");
}

inline void addHero(cocos2d::__Array* heros, const char* character, const char* role, const char* group)
{
	cocos2d::__Dictionary* dic = cocos2d::__Dictionary::create();
	dic->setObject(cocos2d::__String::create(character), "character");
	dic->setObject(cocos2d::__String::create(role), "role");
	dic->setObject(cocos2d::__String::create(group), "group");
	heros->addObject(dic);
}

template <typename HeroT>
inline void setHeroId(HeroT* hero, cocos2d::Ref* payload)
{
	hero->setID(
		cocos2d::__String::create(character(payload)),
		cocos2d::__String::create(role(payload)),
		cocos2d::__String::create(group(payload)));
}

}
