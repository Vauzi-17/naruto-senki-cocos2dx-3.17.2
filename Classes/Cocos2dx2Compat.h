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

#include <cstdlib>
#include <string>

NS_CC_BEGIN

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


