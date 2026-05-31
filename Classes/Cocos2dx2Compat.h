#ifndef NARUTO_SENKI_COCOS2DX2_COMPAT_H_
#define NARUTO_SENKI_COCOS2DX2_COMPAT_H_

#include "cocos2d.h"
#include "deprecated/CCDeprecated.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UIScale9Sprite.h"
#include "extensions/GUI/CCScrollView/CCScrollView.h"

#include <chrono>
#include <cstdlib>
#include <string>

NS_CC_BEGIN

struct cc_timeval
{
    long tv_sec;
    long tv_usec;
};

class CCTime
{
public:
    static void gettimeofdayCocos2d(cc_timeval* tp, void* tzp)
    {
        CC_UNUSED_PARAM(tzp);
        if (!tp)
        {
            return;
        }

        const auto now = std::chrono::system_clock::now().time_since_epoch();
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now);
        const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(now - seconds);
        tp->tv_sec = static_cast<long>(seconds.count());
        tp->tv_usec = static_cast<long>(micros.count());
    }
};

typedef DictElement CCDictElement;

namespace cocos2dx2_compat
{
    inline int atoi(const char* value)
    {
        return ::atoi(value);
    }

    inline int atoi(const std::string& value)
    {
        return ::atoi(value.c_str());
    }

    inline Vector<SpriteFrame*> toSpriteFrameVector(CCArray* frames)
    {
        Vector<SpriteFrame*> result;
        if (!frames)
        {
            return result;
        }

        CCObject* object = nullptr;
        CCARRAY_FOREACH(frames, object)
        {
            auto frame = dynamic_cast<SpriteFrame*>(object);
            if (frame)
            {
                result.pushBack(frame);
            }
        }
        return result;
    }

    inline Animation* createAnimationWithSpriteFrames(CCArray* frames, float delay, unsigned int loops = 1)
    {
        return Animation::createWithSpriteFrames(toSpriteFrameVector(frames), delay, loops);
    }

    inline Sequence* createSequence(CCArray* actions)
    {
        if (!actions || actions->count() == 0)
        {
            return nullptr;
        }

        Vector<FiniteTimeAction*> result;
        CCObject* object = nullptr;
        CCARRAY_FOREACH(actions, object)
        {
            auto action = dynamic_cast<FiniteTimeAction*>(object);
            if (action)
            {
                result.pushBack(action);
            }
        }
        return Sequence::create(result);
    }

    inline CCArray* toCCArray(const Vector<Node*>& nodes)
    {
        auto result = CCArray::createWithCapacity(static_cast<unsigned int>(nodes.size()));
        for (auto node : nodes)
        {
            if (node)
            {
                result->addObject(node);
            }
        }
        return result;
    }

    inline CCArray* toCCArray(const ValueVector& values)
    {
        auto result = CCArray::createWithCapacity(static_cast<unsigned int>(values.size()));
        for (const auto& value : values)
        {
            if (value.getType() == Value::Type::MAP)
            {
                auto dict = CCDictionary::create();
                for (const auto& item : value.asValueMap())
                {
                    dict->setObject(CCString::create(item.second.asString()), item.first);
                }
                result->addObject(dict);
            }
            else
            {
                result->addObject(CCString::create(value.asString()));
            }
        }
        return result;
    }

    template <typename... Args>
    inline Sequence* createSequence(Args... args)
    {
        return Sequence::create(args...);
    }

    inline void visitNode(Node* node)
    {
        if (node)
        {
            node->visit(Director::getInstance()->getRenderer(), Mat4::IDENTITY, 0);
        }
    }

}

NS_CC_END

NS_CC_EXT_BEGIN

class CCHttpRequest : public cocos2d::network::HttpRequest
{
public:
    static constexpr Type kHttpGet = Type::GET;
    static constexpr Type kHttpPost = Type::POST;
    static constexpr Type kHttpPut = Type::PUT;
    static constexpr Type kHttpDelete = Type::DELETE;
    static constexpr Type kHttpUnkown = Type::UNKNOWN;
};

typedef cocos2d::network::HttpClient CCHttpClient;
typedef cocos2d::network::HttpResponse CCHttpResponse;
typedef cocos2d::ui::EditBox CCEditBox;
typedef cocos2d::ui::Scale9Sprite CCScale9Sprite;
typedef ScrollView CCScrollView;

static const ScrollView::Direction kCCScrollViewDirectionNone = ScrollView::Direction::NONE;
static const ScrollView::Direction kCCScrollViewDirectionHorizontal = ScrollView::Direction::HORIZONTAL;
static const ScrollView::Direction kCCScrollViewDirectionVertical = ScrollView::Direction::VERTICAL;
static const ScrollView::Direction kCCScrollViewDirectionBoth = ScrollView::Direction::BOTH;

static const cocos2d::ui::EditBox::InputMode kEditBoxInputModeAny = cocos2d::ui::EditBox::InputMode::ANY;
static const cocos2d::ui::EditBox::InputMode kEditBoxInputModeEmailAddr = cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS;
static const cocos2d::ui::EditBox::InputMode kEditBoxInputModeNumeric = cocos2d::ui::EditBox::InputMode::NUMERIC;
static const cocos2d::ui::EditBox::InputMode kEditBoxInputModePhoneNumber = cocos2d::ui::EditBox::InputMode::PHONE_NUMBER;
static const cocos2d::ui::EditBox::InputMode kEditBoxInputModeUrl = cocos2d::ui::EditBox::InputMode::URL;
static const cocos2d::ui::EditBox::InputMode kEditBoxInputModeDecimal = cocos2d::ui::EditBox::InputMode::DECIMAL;
static const cocos2d::ui::EditBox::InputMode kEditBoxInputModeSingleLine = cocos2d::ui::EditBox::InputMode::SINGLE_LINE;

static const cocos2d::ui::EditBox::InputFlag kEditBoxInputFlagPassword = cocos2d::ui::EditBox::InputFlag::PASSWORD;
static const cocos2d::ui::EditBox::InputFlag kEditBoxInputFlagSensitive = cocos2d::ui::EditBox::InputFlag::SENSITIVE;
static const cocos2d::ui::EditBox::InputFlag kEditBoxInputFlagInitialCapsWord = cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_WORD;
static const cocos2d::ui::EditBox::InputFlag kEditBoxInputFlagInitialCapsSentence = cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE;
static const cocos2d::ui::EditBox::InputFlag kEditBoxInputFlagInitialCapsAllCharacters = cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS;

static const cocos2d::ui::EditBox::KeyboardReturnType kKeyboardReturnTypeDefault = cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT;
static const cocos2d::ui::EditBox::KeyboardReturnType kKeyboardReturnTypeDone = cocos2d::ui::EditBox::KeyboardReturnType::DONE;
static const cocos2d::ui::EditBox::KeyboardReturnType kKeyboardReturnTypeSend = cocos2d::ui::EditBox::KeyboardReturnType::SEND;
static const cocos2d::ui::EditBox::KeyboardReturnType kKeyboardReturnTypeSearch = cocos2d::ui::EditBox::KeyboardReturnType::SEARCH;
static const cocos2d::ui::EditBox::KeyboardReturnType kKeyboardReturnTypeGo = cocos2d::ui::EditBox::KeyboardReturnType::GO;

NS_CC_EXT_END

#define atoi cocos2d::cocos2dx2_compat::atoi

#endif


