#pragma once

#include "LegacyData.h"

namespace NarutoSenkiPayload {

using LegacyArray = NarutoSenkiLegacy::Array;
using LegacyDictionary = NarutoSenkiLegacy::Dictionary;
using LegacyString = NarutoSenkiLegacy::String;

inline LegacyDictionary* asHero(cocos2d::Ref* payload)
{
	return static_cast<LegacyDictionary*>(payload);
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

inline int gearValue(cocos2d::Ref* payload)
{
	return static_cast<LegacyString*>(payload)->intValue();
}

inline LegacyArray* createHeroList()
{
	return LegacyArray::create();
}

inline LegacyDictionary* createStringPayload(const char* value, int key = 1)
{
	auto payload = LegacyDictionary::create();
	payload->setObject(LegacyString::create(value), key);
	return payload;
}

inline void addHero(LegacyArray* heros, const char* character, const char* role, const char* group)
{
	LegacyDictionary* dic = LegacyDictionary::create();
	dic->setObject(LegacyString::create(character), "character");
	dic->setObject(LegacyString::create(role), "role");
	dic->setObject(LegacyString::create(group), "group");
	heros->addObject(dic);
}

template <typename HeroT>
inline void setId(HeroT* hero, const char* character, const char* role, const char* group)
{
	hero->setID(
		LegacyString::create(character),
		LegacyString::create(role),
		LegacyString::create(group));
}

template <typename HeroT>
inline void setHeroId(HeroT* hero, cocos2d::Ref* payload)
{
	setId(hero, character(payload), role(payload), group(payload));
}

template <typename HeroT>
inline void setMaxHP(HeroT* hero, const char* value)
{
	hero->setMaxHP(LegacyString::create(value));
}

template <typename HeroT>
inline void setHP(HeroT* hero, const char* value)
{
	hero->setHP(LegacyString::create(value));
}

template <typename HeroT>
inline void setCKR(HeroT* hero, const char* value)
{
	hero->setCKR(LegacyString::create(value));
}

template <typename HeroT>
inline void setCKR2(HeroT* hero, const char* value)
{
	hero->setCKR2(LegacyString::create(value));
}

}
