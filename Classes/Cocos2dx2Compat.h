#ifndef NARUTO_SENKI_COCOS2DX2_COMPAT_H_
#define NARUTO_SENKI_COCOS2DX2_COMPAT_H_

#include "cocos2d.h"
#include "deprecated/CCDeprecated.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"

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

#endif


