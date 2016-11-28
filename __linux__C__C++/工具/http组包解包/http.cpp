/**
* 文件名称  : http.cpp
***/

#include "http.h"
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


const char* http::SCHEME                  = "http://";
uint16      http::PORT                    = 80;

const char* http::VERSION_1_0             = "HTTP/1.0";
const char* http::VERSION_1_1             = "HTTP/1.1";

const char* http::SP                      = " ";
const char* http::CRLF                    = "\r\n";


// method
const char* http::OPTIONS                 = "OPTIONS";
const char* http::GET                     = "GET";
const char* http::HEAD                    = "HEAD";
const char* http::POST                    = "POST";
const char* http::PUT                     = "PUT";
const char* http::METHOD_DELETE           = "DELETE";
const char* http::TRACE                   = "TRACE";
const char* http::CONNECT                 = "CONNECT";

// general-header

const char* http::CACHE_CONTROL           = "Cache-Control";
const char* http::CONNECTION              = "Connection";
const char* http::DATE                    = "Date";
const char* http::PRAGMA                  = "Pragma";
const char* http::TRAILER                 = "Trailer";
const char* http::TRANSFER_ENCODING       = "Transfer-Encoding";
const char* http::UPGRADE                 = "Upgrade";
const char* http::VIA                     = "Via";
const char* http::WARNING                 = "Warning";

// request-header

const char* http::ACCEPT                  = "Accept";
const char* http::ACCEPT_CHARSET          = "Accept-Charset";
const char* http::ACCEPT_ENCODING         = "Accept-Encoding";
const char* http::ACCEPT_LANGUAGE         = "Accept-Language";
const char* http::AUTHORIZATION           = "Authorization";
const char* http::EXPECT                  = "Expect";
const char* http::FROM                    = "From";
const char* http::HOST                    = "Host";
const char* http::IF_MATCH                = "If-Match";

const char* http::IF_MODIFIED_SINCE       = "If-Modified-Since";
const char* http::IF_NONE_MATCH           = "If-None-Match";
const char* http::IF_RANGE                = "If-Range";
const char* http::IF_UNMODIFIED_SINCE     = "If-Unmodified-Since";
const char* http::MAX_FORWARDS            = "Max-Forwards";
const char* http::PROXY_AUTHORIZATION     = "Proxy-Authorization";
const char* http::RANGE                   = "Range";
const char* http::REFERER                 = "Referer";
const char* http::TE                      = "TE";
const char* http::USER_AGENT              = "User-Agent";

// response-header
const char* http::ACCEPT_RANGES           = "Accept-Ranges";
const char* http::AGE                     = "Age";
const char* http::ETAG                    = "ETag";
const char* http::LOCATION                = "Location";
const char* http::PROXY_AUTHENTICATE      = "Proxy-Authenticate";
const char* http::RETRY_AFTER             = "Retry-After";
const char* http::SERVER                  = "Server";
const char* http::VARY                    = "Vary";
const char* http::WWW_AUTHENTICATE        = "WWW-Authenticate";

// entity-header
const char* http::ALLOW                   = "Allow";
const char* http::CONTENT_ENCODING        = "Content-Encoding";
const char* http::CONTENT_LANGUAGE        = "Content-Language";
const char* http::CONTENT_LENGTH          = "Content-Length";
const char* http::CONTENT_LOCATION        = "Content-Location";
const char* http::CONTENT_MD5             = "Content-MD5";
const char* http::CONTENT_RANGE           = "Content-Range";
const char* http::CONTENT_TYPE            = "Content-Type";
const char* http::EXPIRES                 = "Expires";
const char* http::LAST_MODIFIED           = "Last-Modified";

const uint32 http::UNKNOWN_CONTENT_LENGTH = UINT32_MAX;

uint32 kostr_to_uint32(const char* s)//uint32 kostr_to_uint32(std::string &str)
{
    uint32 out = 0;

    sscanf(s, "%u", &out);
    return out;
}

/*******************************************************************************
* 函数名称  : kostrnstr
* 功能描述  : 在指定的字符串中寻找子字符串。类似 strstr，但是严格指定长度，不依赖字符串末尾0。
* 参　　数  : const char * str     用于寻找的源字符串
* 参　　数  : uint32 str_len        str的长度
* 参　　数  : const char * sub     被寻找的子字符串
* 参　　数  : uint32 sub_len        sub的长度
* 返 回 值  : char*                返回sub在str中首次出现的地址。如果没有找到，则返回NULL。
*******************************************************************************/

const char * kostrnstr(const char * str, uint32 str_len, const char * sub, uint32 sub_len )
{
    if ( !str || !sub )
    {
        return NULL;
    }
    const char* p = (const char*)str;
    uint32 unread_len = str_len;
    while( unread_len >= sub_len )
    {
        bool equal = true;
        const char* src = p, *dst = sub;
        for ( uint32 i=0; i<sub_len; i++ )
        {
            if ( *src != *dst )
            {
                equal = false;
                break;
            }
            else
            {
                src++;
                dst++;
            }
        }
        if ( equal )
        {
            return p;
        }
        else
        {
            p++;
            unread_len--;
        }
    }
    return NULL;
}



std::string http::status_code_to_reason_phrase( uint32 status_code )
{
    switch ( status_code )
    {
        case 100: return "Continue";
        case 101: return "Switching Protocols";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 307: return "Temporary Redirect";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Time-out";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Request Entity Too Large";
        case 414: return "Request-URI Too Large";
        case 415: return "Unsupported Media Type";
        case 416: return "Requested range not satisfiable";
        case 417: return "Expectation Failed";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Time-out";
        case 505: return "HTTP Version not supported";
        default: return "";
    }
}

bool http::get_server_addr_from_url( const std::string& url, std::string& server_host, uint16& server_port )
{
    server_host.erase();
    server_port = 0;

    std::string url_body = url;
    size_t scheme_size = strlen(SCHEME);
    if ( url_body.size() > scheme_size && strncasecmp(url_body.c_str(),SCHEME,scheme_size)==0 )
    {
        url_body = url_body.substr(scheme_size);
    }
    else
    {
        // url 不以 http:// 开头，视为不合法
        return false;
    }

    std::string url_server = url_body.substr( 0, url_body.find("/") );
    if ( !url_server.empty() )
    {
        server_host = url_server.substr( 0, url_server.rfind(":") );
        if ( !server_host.empty() )
        {
            std::string::size_type pos = url_server.rfind(":");
            if ( pos==std::string::npos )
            {
                server_port = PORT;
            }
            else
            {
                std::string url_port = url_server.substr(pos+1);
                if ( !url_port.empty() )
                {
                    server_port = atoi(url_port.c_str());
                }
            }
        }
    }

    if ( server_host.empty() || server_port==0 )
    {
        server_host.erase();
        server_port = 0;
        return false;
    }
    else
    {
        return true;
    }
}


std::string http::get_absolute_path_from_url( const std::string& url )
{
    std::string url_body = url;

    size_t scheme_size = strlen(SCHEME);
    if ( url.size()>scheme_size
        && strncasecmp( url.c_str(), SCHEME, scheme_size )==0 )
    {
        url_body = url.substr( scheme_size );
    }
    else
    {
        // url 不以 http:// 开头，视为不合法
        return "";
    }

    std::string abs_path;
    std::string::size_type abs_path_pos = url_body.find("/");
    if ( abs_path_pos!=std::string::npos )
    {
        abs_path = (abs_path_pos==0?"":url_body.substr( abs_path_pos ));
    }
    else
    {
        abs_path = "/";
    }

    return abs_path;
}


bool http::parse_http_packet( const char* data_buff, uint32 data_size, uint32& header_size, uint32& body_size )
{
    const char* HEADER_END = "\r\n\r\n";
    const char* header_end_str = kostrnstr( data_buff, data_size, HEADER_END, (uint32)strlen(HEADER_END) );
    if ( header_end_str )
    {
        body_size = 0;

        // 查找 Content-Length
        uint32 content_length_strlen = (uint32)strlen(CONTENT_LENGTH);
        const char* content_length_field = kostrnstr(
            data_buff, data_size, CONTENT_LENGTH, content_length_strlen );
        if ( content_length_field )
        {
            body_size = atol( content_length_field
                + content_length_strlen
                + 1 );
        }
        else
        {
            // 如果没有 Content-Length，那么就视为长度未知
            body_size = UNKNOWN_CONTENT_LENGTH;
        }

        const char* content_str = header_end_str + strlen(HEADER_END);
        header_size = (uint32)(content_str - data_buff);
        return true;
    }
    else
    {
        return false;
    }
}

bool http::generate_request_from_url( const std::string& url, REQUEST& request )
{
    std::string server_host;
    uint16      server_port;
    if ( !get_server_addr_from_url( url, server_host, server_port ) )
    {
        return false;
    }

    std::string absolute_path = get_absolute_path_from_url( url );
    if( absolute_path.empty() )
    {
        return false;
    }

    request.request_line.method = GET;
    request.request_line.request_uri = absolute_path;
    request.request_line.http_version = VERSION_1_1;

    std::ostringstream host_field_value;
    host_field_value << server_host;
    if ( server_port != http::PORT )
    {
        host_field_value << ":" << server_port;
    }

    HEADER host;
    host.field_name = HOST;
    host.field_value = host_field_value.str();
    request.headers.push_back( host );

    return true;
}

void http::set_header( HEADERS& headers, const std::string& field_name, const std::string& field_value )
{
    HEADERS::iterator it = headers.begin(), it_end = headers.end();
    for ( ; it!=it_end; it++ )
    {
        HEADER& header = *it;
        if ( header.field_name == field_name )
        {
            if ( field_value.empty() )
            {
                headers.erase(it);
            }
            else
            {
                header.field_value = field_value;
            }
            return;
        }
    }

    HEADER new_header;
    new_header.field_name   = field_name;
    new_header.field_value  = field_value;
    headers.push_back( new_header );
}

void http::del_header( HEADERS& headers, const std::string& field_name )
{
    HEADERS::iterator it = headers.begin(), it_end = headers.end();
    for ( ; it!=it_end; it++ )
    {
        HEADER& header = *it;
        if ( header.field_name == field_name )
        {
            headers.erase(it);
            return;
        }
    }
}

std::string http::get_header( const HEADERS& headers, const std::string& field_name )
{
    HEADERS::const_iterator it = headers.begin(), it_end = headers.end();
    for ( ; it!=it_end; it++ )
    {
        const HEADER& header = *it;
        if ( header.field_name == field_name )
        {
            return header.field_value;
        }
    }
    return std::string();
}

bool http::encode_request( std::ostream& os, const REQUEST& request, bool with_body )
{
    os << request.request_line.method;
    os << SP;
    os << request.request_line.request_uri;
    os << SP;
    os << request.request_line.http_version;
    os << CRLF;

    bool has_content_length = false;
    HEADERS::const_iterator it = request.headers.begin(), it_end = request.headers.end();
    for ( ; it!=it_end; it++ )
    {
        const HEADER& header = *it;
        os << header.field_name << ": " << header.field_value << CRLF;
        if ( header.field_name == CONTENT_LENGTH )
        {
            has_content_length = true;
        }
    }

    if ( with_body && !has_content_length && !request.body.empty() )
    {
        os << CONTENT_LENGTH << ": " << (uint32)request.body.size() << CRLF;
    }

    os << CRLF;

    if ( with_body && !request.body.empty() )
    {
        os.write( request.body.c_str(), (std::streamsize)request.body.size() );
    }

    return os.good();
}

bool http::decode_request( std::istream& is, REQUEST& request, bool with_body )
{
    std::getline( is, request.request_line.method, ' ' );
    std::getline( is, request.request_line.request_uri, ' ' );
    std::getline( is, request.request_line.http_version, '\r' );
    if ( !is.good()
        || request.request_line.method.empty()
        || request.request_line.request_uri.empty()
        || request.request_line.http_version.empty() )
    {
        return false;
    }

    char c;
    is.get(c);
    if ( c != '\n' )
    {
        return false;
    }

    uint32 content_length = 0;

    request.headers.clear();
    while ( true )
    {
        char sp = 0, cr = 0, lf = 0;
        cr = is.get();
        lf = is.get();
        if ( !is.good() )
        {
            return false;
        }
        if ( cr == '\r' )
        {
            if ( lf != '\n' )
            {
                return false;
            }
            else
            {
                break;
            }
        }
        is.unget();
        is.unget();

        HEADER header;

        std::getline( is, header.field_name, ':' );
        is.get(sp);
        std::getline( is, header.field_value, '\r' );
        is.get(lf);

        if ( !is.good()
            || header.field_name.empty()
            || header.field_value.empty()
            || sp != ' '
            || lf != '\n' )
        {
            return false;
        }

        if(strcmp(header.field_name.c_str(),"Cookie") == 0)
        {
            concookie(request,header.field_value.c_str() );
        }
        else
        request.headers.push_back( header );

        if ( with_body && header.field_name == CONTENT_LENGTH )
        {
            content_length = kostr_to_uint32(  header.field_value.c_str() );//change
        }
    }

    if ( with_body && content_length )
    {
        request.body.resize( content_length );
        char *buff=(char *)malloc(content_length+1);//change
        is.read( buff, content_length );
        buff[content_length] = '\0';
        request.body = buff;
        free (buff);
    }

    return is.good();
}

bool http::reparsecookie(REQUEST& request,char const *str)
{
    std::pair<std::map<std::string ,std::string>::iterator,bool> ret;
    char const *ctemp;

    ctemp = strchr(str,'=');
    std::string keystr(str,ctemp - str);

    //sid=YMJPSQNvel
    std::string valuestr(ctemp+1);

    ret=request.cookiekv.insert(make_pair(keystr,valuestr ) );//加上判断
}

bool http::concookie(REQUEST& request,char const *str)
{
    char const *pdestfrom, *pdestto;
    std::string temp;
    pdestfrom = str;
    while(1){
        pdestto = strstr(pdestfrom,"; ");
        if (!pdestto){
            break;
        }
        temp.assign(pdestfrom,pdestto - pdestfrom);
        reparsecookie(request,temp.c_str());
        pdestfrom =pdestto + strlen("; ");
    }
    reparsecookie(request,pdestfrom);
    return true;
}

std::string http::getCookie(REQUEST& request,std::string str)
{

    std::map<std::string, std::string>::iterator f = request.cookiekv.find(str);
    if (f != request.cookiekv.end())//存在
    {
        return f->second;
    }
    else return "";

}
std::string* http::getPPCookie(REQUEST& request,std::string str)
{
    std::map<std::string, std::string>::iterator f = request.cookiekv.find(str);
    if (f != request.cookiekv.end())//存在
    {
        return &(f->second);
    }
    else return   NULL;
}


bool http::setCookie(REQUEST& request,std::string keystr,std::string valuestr)
{

    std::map<std::string, std::string>::iterator f = request.cookiekv.find(keystr);
    if (f != request.cookiekv.end())//存在
    {
        f->second = valuestr;
    }
    else{
        std::pair<std::map<std::string ,std::string>::iterator,bool> ret;
        ret=request.cookiekv.insert(make_pair(keystr,valuestr ) );//加上判断
        if(ret.second == false)
            return false;
    }
    return true;

}

std::string  http::packet(REQUEST& request)
{
    std::string crlf="\r\n";
    std::string str;
    str += request.request_line.method;
    str += " ";
    str += request.request_line.request_uri;
    str += " ";
    str += request.request_line.http_version;
    str += crlf;

    std::vector<HEADER>::iterator f = request.headers.begin();
    for (; f !=request.headers.end() ; ++f)
    {
        str += f->field_name + ": " + f->field_value + crlf;
    }

    str += "Cookie: ";
    std::map<std::string, std::string>::iterator fcookie = request.cookiekv.begin();
    for (; fcookie !=request.cookiekv.end() ; ++fcookie)
    {
        str += fcookie->first + "=" + fcookie->second + ";";
    }
    str += crlf +crlf;

    str += request.body;

    return str;

}




bool http::encode_response( std::ostream& os, const RESPONSE& response, bool with_body )
{
    os << response.status_line.http_version;
    os << SP;
    os << response.status_line.status_code;
    os << SP;
    os << response.status_line.reason_phrase;
    os << CRLF;

    bool has_content_length = false;
    HEADERS::const_iterator it = response.headers.begin(), it_end = response.headers.end();
    for ( ; it!=it_end; it++ )
    {
        const HEADER& header = *it;
        os << header.field_name << ": " << header.field_value << CRLF;
        if ( header.field_name == CONTENT_LENGTH )
        {
            has_content_length = true;
        }
    }

    if ( with_body && !has_content_length && !response.body.empty() )
    {
        os << CONTENT_LENGTH << ": " << (uint32)response.body.size() << CRLF;
    }

    os << CRLF;

    if ( with_body && !response.body.empty() )
    {
        os.write( response.body.c_str(), (std::streamsize)response.body.size() );
    }

    return os.good();
}

bool http::decode_response( std::istream& is, RESPONSE& response, bool with_body )
{
    std::getline( is, response.status_line.http_version, ' ' );
    std::string status_code;
    std::getline( is, status_code, ' ' );
    response.status_line.status_code = kostr_to_uint32( status_code.c_str() );
    std::getline( is, response.status_line.reason_phrase, '\r' );
    if ( !is.good()
        || response.status_line.http_version.empty()
        || response.status_line.status_code==0
        || response.status_line.reason_phrase.empty() )
    {
        return false;
    }

    char c;
    is.get(c);
    if ( c != '\n' )
    {
        return false;
    }

    uint32 content_length = 0;

    response.headers.clear();
    while ( true )
    {
        char sp = 0, cr = 0, lf = 0;
        cr = is.get();
        lf = is.get();
        if ( !is.good() )
        {
            return false;
        }
        if ( cr == '\r' )
        {
            if ( lf != '\n' )
            {
                return false;
            }
            else
            {
                break;
            }
        }
        is.unget();
        is.unget();

        HEADER header;

        std::getline( is, header.field_name, ':' );
        is.get(sp);
        std::getline( is, header.field_value, '\r' );
        is.get(lf);

        if ( !is.good()
            || header.field_name.empty()
            || header.field_value.empty()
            || sp != ' '
            || lf != '\n' )
        {
            return false;
        }

        response.headers.push_back( header );

        if ( with_body && header.field_name == CONTENT_LENGTH )
        {
            content_length = kostr_to_uint32( header.field_value.c_str() );
        }
    }

    if ( with_body && content_length )
    {
        response.body.resize( content_length );
        is.read( (char*)response.body.c_str(), content_length );
    }

    return is.good();
}
