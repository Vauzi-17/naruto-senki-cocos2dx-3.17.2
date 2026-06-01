#ifndef NARUTO_SENKI_COCOS2DX2_COMPAT_H_
#define NARUTO_SENKI_COCOS2DX2_COMPAT_H_

#include "cocos2d.h"
#include "deprecated/CCDeprecated.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"

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

NS_CC_EXT_END

#define atoi cocos2d::cocos2dx2_compat::atoi

#endif


