#ifndef __KO_HTTP_H__
#define __KO_HTTP_H__

#include <string>
#define    KO_HTTP_ERRNO_STR_NULL                   101
#define    KO_HTTP_ERRNO_PARAM_ERROR                102
#define    KO_HTTP_ERRNO_COOKIE_NULL                103
#define    KO_HTTP_ERRNO_BODY_NULL                  104

#define KO_HTTP_TYPE_GET                0
#define KO_HTTP_TYPE_POST               1

typedef  struct ko_str_t
{
    void *pData;
    int i32Size;
}ko_str_t;


class CKOHttpBase
{
public:

    virtual int      addContent(const char  *pI8Str) = 0;
    virtual int      parse() = 0;
    virtual int      parse(ko_str_t  *pSStr) = 0;
    virtual int      parse(const char  *pI8Str) = 0;

    virtual int      packet(int  i32Type = 0) = 0;
    virtual int      packet(int  i32Type,  char **ppI8Data) = 0;///
    virtual int      getPacketData( char **ppI8Data) = 0;//
    virtual int      getPacketData( std::string **ppSData) = 0;//

    virtual int      setReq(const char *pI8Value) = 0;//method

    virtual int      setParam(const char *pI8Key, const char *pI8Value) = 0;

    virtual int      setHeaderParam(const char *pI8Key, const char *pI8Value) = 0;
    virtual int      setHost(const char *pI8Value) = 0;
    virtual int      setContentType(const char *pI8Value) = 0;
    
    virtual int      setCookie(const char *pI8Key, const char *pI8Value) = 0;
    virtual int      getCookie(const char *pI8Key,  char **ppI8Value) = 0;//错误接口 const  char **
    virtual int      getCookie(const char *pI8Key, std::string **ppSStr) = 0;//

    virtual int      getBody(char **ppI8Value) = 0;
    virtual int      getBody(std::string **ppSStr) = 0;//
    virtual int      setBody(ko_str_t  *pSStr) = 0;
    virtual int      setBody(const char  *pI8Str) = 0;

            int      m_i32Errno;
};


#endif
