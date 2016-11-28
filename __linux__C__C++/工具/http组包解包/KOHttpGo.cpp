#include "KOHttpGo.h"
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
std::string CKOHttpGo::mReqStrcutToStr;
http::REQUEST CKOHttpGo::mReqStrcut;

void CKOHttpGo::clear()
{
  mReqStrcut.headers.clear();
  mReqStrcut.cookiekv.clear();
}

int      CKOHttpGo::addContent(const char  *pI8Str)
{
  mReqStr += (char *)pI8Str;
  return 0;
}

int      CKOHttpGo::parse()
{
  clear();
  std::istringstream is(mReqStr);
  bool b = mHttp.decode_request(is,mReqStrcut,true);
  if(false == b){
    m_i32Errno = KO_HTTP_ERRNO_STR_NULL;
    return -1;
  }
  return 0;
}

int      CKOHttpGo::parse(ko_str_t  *pSStr)
{

  clear();
  std::string str;
  char *buf = (char *)(pSStr->pData);
  str.assign(buf,pSStr->i32Size);

  std::istringstream is(str);
  bool b = mHttp.decode_request(is,mReqStrcut,true);
  if(false == b){
    m_i32Errno = KO_HTTP_ERRNO_STR_NULL;
    return -1;
  }
  return 0;

}

int      CKOHttpGo::parse(const char  *pI8Str)
{
  clear();
  
  char *buf = (char *)pI8Str;
  std::istringstream is(buf);
  bool b = mHttp.decode_request(is,mReqStrcut,true);
  if(false == b){
    m_i32Errno = KO_HTTP_ERRNO_STR_NULL;
    return -1;
  }
  return 0;
}

int      CKOHttpGo::packet(int  i32Type)
{
  std::string tempMethod;
  std::string tempBody;
  std::string tempContentLength;

  if (i32Type == KO_HTTP_TYPE_GET && (mReqStrcut.request_line.method != "GET") )//非get方法时候
  {
    tempMethod=mReqStrcut.request_line.method;
    mReqStrcut.request_line.method = "GET";

   tempBody=mReqStrcut.body;
    mReqStrcut.body = "";

    tempContentLength=mHttp.get_header(mReqStrcut.headers,"Content-Length");
    mHttp.set_header(mReqStrcut.headers,"Content-Length","");

    mReqStrcutToStr = mHttp.packet(mReqStrcut);
    mReqStrcut.request_line.method = tempMethod;
    mReqStrcut.body = tempBody;
    mHttp.set_header(mReqStrcut.headers,"Content-Length",tempContentLength);

  }
  else
  {
    mReqStrcutToStr = mHttp.packet(mReqStrcut);
  }

  if (mReqStrcutToStr == "")
  {
    m_i32Errno = KO_HTTP_ERRNO_STR_NULL;
    return -1;
  }
  return 0;

}

int      CKOHttpGo::packet(int  i32Type,  char **ppI8Data)
{
  mReqStrcutToStr = packet(i32Type);
  if (mReqStrcutToStr == "")
  {
    m_i32Errno = KO_HTTP_ERRNO_STR_NULL;
    return -1;
  }
  strcpy( *ppI8Data , mReqStrcutToStr.c_str());
  return 0;
}

int      CKOHttpGo::getPacketData( char **ppI8Data)
{

  if (mReqStrcutToStr == "")
  {
    return -1;
  }
  strcpy( *ppI8Data , mReqStrcutToStr.c_str());
  return 0;
}

int      CKOHttpGo::getPacketData( std::string **ppSData)
{

  if (mReqStrcutToStr == "")
  {
    *ppSData = NULL;
    m_i32Errno = KO_HTTP_ERRNO_STR_NULL;
    return -1;
  }

  *ppSData = &CKOHttpGo::mReqStrcutToStr;
  return 0;
}



int      CKOHttpGo::setReq(const char *pI8Value)
{
  mReqStrcut.request_line.method = (const char *)pI8Value;
  return 0;
}

int      CKOHttpGo::setParam(const char *pI8Key, const char *pI8Value)
{
  mHttp.set_header(mReqStrcut.headers,  (char *)pI8Key,(char *)pI8Value );
  return 0;
}

int      CKOHttpGo::setHeaderParam(const char *pI8Key, const char *pI8Value)
{
  mHttp.set_header(mReqStrcut.headers,  (char *)pI8Key,(char *)pI8Value );
  return 0;
}

int      CKOHttpGo::setHost(const char *pI8Value)
{
    mHttp.set_header(mReqStrcut.headers, "Host",(char *)pI8Value );
    return 0;
}
int      CKOHttpGo::setContentType(const char *pI8Value)
{
  mHttp.set_header(mReqStrcut.headers, "Content-Type",(char *)pI8Value );
  return 0;
}


int      CKOHttpGo::setCookie(const char *pI8Key, const char *pI8Value)
{
  mHttp.setCookie(mReqStrcut,(char *)pI8Key,(char *)pI8Value );
  return 0;

}
int      CKOHttpGo::getCookie(const char *pI8Key,  char **ppI8Value)
{

 std::string str = mHttp.getCookie(mReqStrcut,(char *)pI8Key );

 if(str == ""){
 m_i32Errno = KO_HTTP_ERRNO_COOKIE_NULL;
 return -1;
 }
 strcpy( *ppI8Value , str.c_str());
  return 0;
}
int      CKOHttpGo::getCookie(const char *pI8Key, std::string **ppSStr)
{
  *ppSStr = mHttp.getPPCookie(mReqStrcut,(char *)pI8Key );
  if (NULL == *ppSStr )
  {
    m_i32Errno = KO_HTTP_ERRNO_COOKIE_NULL;
    return -1;
  }
  return 0;
}



int      CKOHttpGo::getBody(char **ppI8Value)
{
  std::string str = mReqStrcut.body;
  if ("" == str )
  {
    m_i32Errno = KO_HTTP_ERRNO_BODY_NULL;
    return -1;
  }
  strcpy( (char *)*ppI8Value , str.c_str());
  return 0;
}
int      CKOHttpGo::getBody(std::string **ppSStr)//
{
  *ppSStr = &mReqStrcut.body;
  if (NULL == *ppSStr )
  {
    m_i32Errno = KO_HTTP_ERRNO_BODY_NULL;
    return -1;
  }
  return 0;
}

int      CKOHttpGo::setBody(ko_str_t  *pSStr)
{
  char *buf = (char *)(pSStr->pData);
  mReqStrcut.body.assign(buf,pSStr->i32Size);
  return 0;
}

int      CKOHttpGo::setBody(const char  *pI8Str)
{
    mReqStrcut.body = (char *)pI8Str;
    return 0;
}
