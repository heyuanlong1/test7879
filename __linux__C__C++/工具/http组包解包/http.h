/***
* 文件名称  : http.h
* 内容摘要  : 提供HTTP协议相关的一些函数，包括HTTP协议头的编码和解码。
              所有宏定义以及变量命令均以标准HTTP协议为准，请参考 HTTP RFC
              http://www.w3.org/Protocols/rfc2616/rfc2616.html
***/
#ifndef __HTTP_H_2015__
#define __HTTP_H_2015__

#include <vector>
#include <iostream>
#include <limits.h>
#include <map> 

          
// should be define in type.h
typedef unsigned char   byte;
typedef unsigned char   uint8;
typedef signed char     sint8;

typedef unsigned short  u_int16_t;
typedef  short          int16_t;
typedef unsigned int    u_int32_t;
typedef  int            int32_t;

typedef u_int16_t       uint16;
typedef int16_t         sint16;
typedef u_int32_t       uint32;
typedef int32_t         sint32;

typedef float           float32;
typedef double          float64;

#define UINT32_MAX INT_MAX





class http
{
public:
    // general
    static const char* SCHEME;                  //"http://"
    static uint16      PORT;                    //(80)

    static const char* VERSION_1_0;             //"HTTP/1.0"
    static const char* VERSION_1_1;             //"HTTP/1.1"

    static const char* SP;                      //" "
    static const char* CRLF;                    //"\r\n"

    // method
    static const char* OPTIONS;                 //"OPTIONS"
    static const char* GET;                     //"GET"
    static const char* HEAD;                    //"HEAD"
    static const char* POST;                    //"POST"
    static const char* PUT;                     //"PUT"
    static const char* METHOD_DELETE;           //"DELETE" 直接定义 DELETE 会与已有宏冲突
    static const char* TRACE;                   //"TRACE"
    static const char* CONNECT;                 //"CONNECT"

    // general-header
    static const char* CACHE_CONTROL;           //"Cache-Control"
    static const char* CONNECTION;              //"Connection"
    static const char* DATE;                    //"Date"
    static const char* PRAGMA;                  //"Pragma"
    static const char* TRAILER;                 //"Trailer"
    static const char* TRANSFER_ENCODING;       //"Transfer-Encoding"
    static const char* UPGRADE;                 //"Upgrade"
    static const char* VIA;                     //"Via"
    static const char* WARNING;                 //"Warning"

    // request-header
    static const char* ACCEPT;                  //"Accept"
    static const char* ACCEPT_CHARSET;          //"Accept-Charset"
    static const char* ACCEPT_ENCODING;         //"Accept-Encoding"
    static const char* ACCEPT_LANGUAGE;         //"Accept-Language"
    static const char* AUTHORIZATION;           //"Authorization"
    static const char* EXPECT;                  //"Expect"
    static const char* FROM;                    //"From"
    static const char* HOST;                    //"Host"
    static const char* IF_MATCH;                //"If-Match"

    static const char* IF_MODIFIED_SINCE;       //"If-Modified-Since"
    static const char* IF_NONE_MATCH;           //"If-None-Match"
    static const char* IF_RANGE;                //"If-Range"
    static const char* IF_UNMODIFIED_SINCE;     //"If-Unmodified-Since"
    static const char* MAX_FORWARDS;            //"Max-Forwards"
    static const char* PROXY_AUTHORIZATION;     //"Proxy-Authorization"
    static const char* RANGE;                   //"Range"
    static const char* REFERER;                 //"Referer"
    static const char* TE;                      //"TE"
    static const char* USER_AGENT;              //"User-Agent"

    // response-header
    static const char* ACCEPT_RANGES;           //"Accept-Ranges"
    static const char* AGE;                     //"Age"
    static const char* ETAG;                    //"ETag"
    static const char* LOCATION;                //"Location"
    static const char* PROXY_AUTHENTICATE;      //"Proxy-Authenticate"
    static const char* RETRY_AFTER;             //"Retry-After"
    static const char* SERVER;                  //"Server"
    static const char* VARY;                    //"Vary"
    static const char* WWW_AUTHENTICATE;        //"WWW-Authenticate"

    // entity-header
    static const char* ALLOW;                   //"Allow"
    static const char* CONTENT_ENCODING;        //"Content-Encoding"
    static const char* CONTENT_LANGUAGE;        //"Content-Language"
    static const char* CONTENT_LENGTH;          //"Content-Length"
    static const char* CONTENT_LOCATION;        //"Content-Location"
    static const char* CONTENT_MD5;             //"Content-MD5"
    static const char* CONTENT_RANGE;           //"Content-Range"
    static const char* CONTENT_TYPE;            //"Content-Type"
    static const char* EXPIRES;                 //"Expires"
    static const char* LAST_MODIFIED;           //"Last-Modified"

    enum status_code
    {
        CONTINUE                        = 100,
        SWITCHING_PROTOCOLS             = 101,
        OK                              = 200,
        CREATED                         = 201,
        ACCEPTED                        = 202,
        NON_AUTHORITATIVE_INFORMATION   = 203,
        NO_CONTENT                      = 204,
        RESET_CONTENT                   = 205,
        PARTIAL_CONTENT                 = 206,
        MULTIPLE_CHOICES                = 300,
        MOVED_PERMANENTLY               = 301,
        FOUND                           = 302,
        SEE_OTHER                       = 303,
        NOT_MODIFIED                    = 304,
        USE_PROXY                       = 305,
        TEMPORARY_REDIRECT              = 307,
        BAD_REQUEST                     = 400,
        UNAUTHORIZED                    = 401,
        PAYMENT_REQUIRED                = 402,
        FORBIDDEN                       = 403,
        NOT_FOUND                       = 404,
        METHOD_NOT_ALLOWED              = 405,
        NOT_ACCEPTABLE                  = 406,
        PROXY_AUTHENTICATION_REQUIRED   = 407,
        REQUEST_TIME_OUT                = 408,
        CONFLICT                        = 409,
        GONE                            = 410,
        LENGTH_REQUIRED                 = 411,
        PRECONDITION_FAILED             = 412,
        REQUEST_ENTITY_TOO_LARGE        = 413,
        REQUEST_URI_TOO_LARGE           = 414,
        UNSUPPORTED_MEDIA_TYPE          = 415,
        REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        EXPECTATION_FAILED              = 417,
        INTERNAL_SERVER_ERROR           = 500,
        NOT_IMPLEMENTED                 = 501,
        BAD_GATEWAY                     = 502,
        SERVICE_UNAVAILABLE             = 503,
        GATEWAY_TIME_OUT                = 504,
        HTTP_VERSION_NOT_SUPPORTED      = 505,
    };

    static const uint32 UNKNOWN_CONTENT_LENGTH;
public:

    /*******************************************************************************
    * 函数名称  : static std::string status_code_to_reason_phrase( uint32 status_code )
    * 功能描述  : 由状态码得到状态原因描述，例如由 200 得到 OK
    * 参　　数  : uint32 status_code    状态码
    * 返 回 值  : std::string               状态原因描述，如果 status_code 非法则返回空字符串
    *******************************************************************************/
    static std::string status_code_to_reason_phrase( uint32 status_code );

    /*******************************************************************************
    * 函数名称  : static bool get_server_addr_from_url( const std::string& url, std::string& server_host, uint16& server_port )
    * 功能描述  : 从 HTTP URL 中分析出服务器的域名和端口。
                  例如从 http://www.baidu.com/index.html 分析得到域名 www.baidu.com 和端口 80。
    * 参　　数  : const std::string& url        HTTP URL
    * 参　　数  : std::string& server_host      返回分析得到的服务器域名
    * 参　　数  : uint16& server_port       返回分析得到的服务器端口
    * 返 回 值  : bool                          分析成功返回 true，否则返回 false
    *******************************************************************************/
    static bool get_server_addr_from_url( const std::string& url, std::string& server_host, uint16& server_port );

    //
    /*******************************************************************************
    * 函数名称  : static std::string get_absolute_path_from_url( const std::string& url )
    * 功能描述  : 从 URL 中解析得到用于 HTTP 请求的 absolute path。
                  例如，从 http://www.xunlei.com/index.html 获得 /index.html
    * 参　　数  : const std::string& url        HTTP URL
    * 返 回 值  : std::string                   返回 absolute path。解析异常的情况下，返回空字符串。
    *******************************************************************************/
    static std::string get_absolute_path_from_url( const std::string& url );

    /*******************************************************************************
    * 函数名称  : static bool parse_http_packet( const char* data_buff, uint32 data_size, uint32& header_size, uint32& body_size )
    * 功能描述  : 在HTTP通信中，网络接收到一定字节后，用该函数可以分析包头情况，获得包头长度，以及包体长度。
    * 参　　数  : const char* data_buff     网络接收到的字节缓冲区
    * 参　　数  : uint32 data_size      缓冲区长度
    * 参　　数  : uint32& header_size   如果解析成功，返回HTTP包头长度。
    * 参　　数  : uint32& body_size     如果解析成功，返回HTTP包体长度，即Content-Length。
                                            注意：HTTP短连接回复中很可能不带Content-Length，
                                            这种情况下，返回 UNKNOWN_CONTENT_LENGTH。
    * 返 回 值  : bool                      只要数据中包含完整的 HTTP 包头，则解析成功，返回 true。否则返回 false。
    *******************************************************************************/
    static bool parse_http_packet( const char* data_buff, uint32 data_size, uint32& header_size, uint32& body_size );

public:
    struct REQUEST_LINE
    {
        std::string     method;
        std::string     request_uri;
        std::string     http_version;
    };
    struct STATUS_LINE
    {
        std::string     http_version;
        uint32          status_code;
        std::string     reason_phrase;
    };
    struct HEADER
    {
        std::string     field_name;
        std::string     field_value;
    };
    typedef std::vector<HEADER>     HEADERS;
    typedef std::string             BODY;
    struct REQUEST
    {
        REQUEST_LINE    request_line;
        HEADERS         headers;
        std::map<std::string,std::string> cookiekv;
        BODY            body;
    };
    struct RESPONSE
    {
        STATUS_LINE     status_line;
        HEADERS         headers;
        BODY            body;
    };
    static bool reparsecookie(REQUEST& request,char const *str);//用于cookie
    static bool concookie(REQUEST& request,char const *str);
    static std::string getCookie(REQUEST& request,std::string );
    static std::string* getPPCookie(REQUEST& request,std::string );

    static bool setCookie(REQUEST& request,std::string keystr,std::string valuestr);
    static std::string packet(REQUEST& request);

    //addContent

    /*******************************************************************************
    * 函数名称  : get_header
    * 功能描述  : 返回HTTP的头部中指定字段的值。如果该字段不存在，则返回空字符串。
    * 参　　数  : const HEADERS& headers            HTTP头部
    * 参　　数  : const std::string& field_name     字段名
    * 返 回 值  : std::string                       返回该字段对应的值。如果该字段不存在，则返回空字符串。
    *******************************************************************************/
    static std::string  get_header( const HEADERS& headers, const std::string& field_name );

    /*******************************************************************************
    * 函数名称  : set_header
    * 功能描述  : 设置HTTP头部中指定字段的值。如果field_value为空，则删除该字段。
    * 参　　数  : HEADERS& headers                  HTTP头部
    * 参　　数  : const std::string& field_name     指定字段
    * 参　　数  : const std::string& field_value    字段的值。如果为空，则删除该字段。
    //回添加的
    *******************************************************************************/
    static void         set_header( HEADERS& headers, const std::string& field_name, const std::string& field_value );

    /*******************************************************************************
    * 函数名称  : del_header
    * 功能描述  : 删除HTTP头部中指定字段。
    * 参　　数  : HEADERS& headers                  HTTP头部
    * 参　　数  : const std::string& field_name     字段名。
    * 返 回 值  : void
    *******************************************************************************/
    static void         del_header( HEADERS& headers, const std::string& field_name );

    /*******************************************************************************
    * 函数名称  : generate_request_from_url
    * 功能描述  : 从一个URL生成一个HTTP请求，例如从 http://www.baidu.com 将会得到一个
                  对应的 REQUEST。再利用 encode_request 就能得到如下的一个HTTP请求：
                  GET / HTTP/1.1\r\nHost: www.baidu.com\r\n\r\n
    * 参　　数  : const std::string& url    HTTP URL。必须以 http:// 开头。
    * 参　　数  : REQUEST& request          返回的 HTTP 请求结构体。
    * 返 回 值  : bool                      如果 URL 合法，则执行成功返回 true。否则返回 false。
    *******************************************************************************/
    static bool generate_request_from_url( const std::string& url, REQUEST& request );

    /*******************************************************************************
    * 函数名称  : encode_request
    * 功能描述  : 将 HTTP 请求结构体编码成为 HTTP 协议内容，得到协议内容后，即可通过网络发送。
                  如果 body 不为空，且用户没有指定 Content-Length，则会自动生成 Content-Length。
                  备注：编码中不会检查各个字段内容是否合法，所以必须使用者保证字段内容正确。
    * 参　　数  : std::ostream& os          输出流对象。可以是文件，字符串，缓冲区等。
    * 参　　数  : const REQUEST& request    HTTP 请求结构体。
    * 参　　数  : bool with_body = false    编码是否包括包体。否则忽略包体内容。
    * 返 回 值  : bool                      编码是否成功。
    *******************************************************************************/
    static bool encode_request( std::ostream& os, const REQUEST& request, bool with_body = false );

    /*******************************************************************************
    * 函数名称  : decode_request
    * 功能描述  : 解析 HTTP 协议内容，将解析结果以 HTTP 请求对象返回。
    * 参　　数  : std::istream& is          输入流对象。可以是文件，字符串，缓冲区等。
    * 参　　数  : REQUEST& request          返回的 HTTP 解析结果。
    * 参　　数  : bool with_body = false    解码是否包括包体。否则忽略包体内容。
    * 返 回 值  : bool                      解码是否成功。
    *******************************************************************************/
    static bool decode_request( std::istream& is, REQUEST& request, bool with_body = false );

    /*******************************************************************************
    * 函数名称  : encode_response
    * 功能描述  : 将 HTTP 回应结构体编码成为 HTTP 协议内容，得到协议内容后，即可通过网络发送。
    如果 body 不为空，且用户没有指定 Content-Length，则会自动生成 Content-Length。
    备注：编码中不会检查各个字段内容是否合法，所以必须使用者保证字段内容正确。
    * 参　　数  : std::ostream& os          输出流对象。可以是文件，字符串，缓冲区等。
    * 参　　数  : const RESPONSE& response  HTTP 回应结构体。
    * 参　　数  : bool with_body = false    编码是否包括包体。否则忽略包体内容。
    * 返 回 值  : bool                      编码是否成功。
    *******************************************************************************/
    static bool encode_response( std::ostream& os, const RESPONSE& response, bool with_body = false );

    /*******************************************************************************
    * 函数名称  : decode_response
    * 功能描述  : 解析 HTTP 协议内容，将解析结果以 HTTP 回应对象返回。
    * 参　　数  : std::istream& is          输入流对象。可以是文件，字符串，缓冲区等。
    * 参　　数  : RESPONSE& response        返回的 HTTP 解析结果。
    * 参　　数  : bool with_body = false    解码是否包括包体。否则忽略包体内容。
    * 返 回 值  : bool                      解码是否成功。
    *******************************************************************************/
    static bool decode_response( std::istream& is, RESPONSE& response, bool with_body = false );



};

#endif // end of __HTTP_H_2015__
