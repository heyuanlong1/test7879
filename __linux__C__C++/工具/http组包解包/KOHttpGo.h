#ifndef __KO_HTTPGO_H__
#define __KO_HTTPGO_H__

#include "KOHttpBase.h"
#include <string>
#include "http.h"

class CKOHttpGo:public CKOHttpBase
{
public:
    virtual int      addContent(const char  *pI8Str);
    virtual int      parse();
    virtual int      parse(ko_str_t  *pSStr);
    virtual int      parse(const char  *pI8Str);

    virtual int      packet(int  i32Type = 0);
    virtual int      packet(int  i32Type,  char **ppI8Data);///
    virtual int      getPacketData( char **ppI8Data);//
    virtual int      getPacketData( std::string **ppSData);//

    virtual int      setReq(const char *pI8Value);

    virtual int      setParam(const char *pI8Key, const char *pI8Value);

    virtual int      setHeaderParam(const char *pI8Key, const char *pI8Value);
    virtual int      setHost(const char *pI8Value);
    virtual int      setContentType(const char *pI8Value);
    virtual int      setCookie(const char *pI8Key, const char *pI8Value);
    virtual int      getCookie(const char *pI8Key,  char **ppI8Value);
    virtual int      getCookie(const char *pI8Key, std::string **ppSStr);//
    virtual int      getBody(char **ppI8Value);
    virtual int      getBody(std::string **ppSStr);//
    virtual int      setBody(ko_str_t  *pSStr);
    virtual int      setBody(const char  *pI8Str);
    void clear();


    int      m_i32Errno;
    

private:
   std::string mReqStr;
   http mHttp;

   static http::REQUEST mReqStrcut;
   static std::string mReqStrcutToStr;
};


#endif
