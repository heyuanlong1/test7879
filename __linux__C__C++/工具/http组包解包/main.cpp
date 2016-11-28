#include <iostream>
#include <sstream>
#include <string>
#include "KOHttpGo.h"
#include "http.h"
#include "stdio.h"
#include <string.h>

using namespace std;

int main(int argc, char const *argv[])
{
		//request decode
		std::string str="POST http://www.telerik.com/WebResource.axd?d=eu7405110586 HTTP/1.1\r\nHost: www.telerik.com\r\nConnection: keep-alive\r\nAccept: */*\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.152 Safari/537.36\r\nReferer: http://www.telerik.com/download/fiddler/first-run\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: zh-CN,zh;q=0.8,en;q=0.6\r\nContent-Length: 3\r\nCookie: sid=YMJPoxaAYQZssNDEVeS7/QBRpp9QI=; optimizelyEndUserId=oeu14403046668768; _dc_gtm_UA-15-21=1; ki_r=; forcookie=i am cookie; elqUserId=c38-304a-47e5-ad38-920\r\n\r\n123";

		CKOHttpBase *ba = new CKOHttpGo;
		ba->addContent(str.c_str());
		ba->parse();
		//body
		printf("%s\n", "下面是body测试");
		char *body11= new char[50];
		ba->getBody( (  char **)&body11);
		printf("body:%s\n\n\n", body11);


		ba->setReq("POST");
		ba->setHeaderParam("nConnection","close");
		ba->setHeaderParam("fuck","fuck!!!!!!!!!!");
		ba->setHost("www.baidu.com");
		ba->setContentType("html-text");

		//request encode
		std::string *pstr122;
		ba->packet();
		ba->getPacketData(&pstr122);
		printf("request encode:\n%s\n\n\n",pstr122->c_str() );


		//cookie测试
		printf("%s\n", "下面是cookie测试");
		ba->setCookie("sid","sessionid");
		ba->setCookie("elqUserId","999");


		char *sid = new char[50];
		char *bid = new char[50];
		ba->getCookie("sid",( char **)&sid);
		ba->getCookie("_dc_gtm_UA-15-21",( char **)&bid);
		printf("%s   %s\n",sid,bid);

		std::string *pstrcoo;
		ba->getCookie("forcookie",&pstrcoo);
		printf("%s\n\n\n",pstrcoo->c_str() );


		//body
		printf("%s\n", "下面是body测试");
		char *body= new char[50];
		ba->getBody( (  char **)&body);

		ba->setBody("456");
		char *body1= new char[50];
		ba->getBody( (  char **)&body1);
		printf("%s\n\n\n",body,body1 );


//-------------------------------------



	return 0;
}