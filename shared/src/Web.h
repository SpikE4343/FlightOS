/*************************************************************************************
  g3::Web
  Copyright (c) 2013 John Rohrssen
  Email: johnrohrssen@gmail.com
*************************************************************************************/

#ifndef g3_Web_H_INCLUDED
#define g3_Web_H_INCLUDED

#include "g3Util.h"
#include "mongoose/mongoose.h"

#include <string>
#include <unordered_map>

namespace g3
{



class WebRequest
{
public:
  WebRequest( struct mg_connection* conn );

  void send();
  int mResponsCode;
  std::string mContentType;
  std::string mHeaders;

  std::string mResponse;

  struct mg_connection* mConnection;
  struct mg_request_info* mRequestInfo;
    
};

class Web
{
public:

  typedef Delegate< int, WebRequest& > MessageHandler;

	Web();
	static Web* getInstance();

	struct InitInfo
  {
		int mMaxActiveConnections;
		int mListenPort;
  };

	int initialize( const InitInfo& info );
	int update();
	int shutdown();

  void registerHandler( const char* url, MessageHandler& handler );
  int handleRequest( WebRequest& request );

private:
	static Web* m_sInstance;

  typedef std::unordered_map< std::string, MessageHandler > HandlerMap;

  HandlerMap mHandlers;

  struct mg_context* mMgCtx;

};

#define g3_web_declare_handler( f ) int WebHandler_##f(g3::WebRequest& request )
#define g3_web_register_handler( c, n, f ) Web::getInstance()->registerHandler( n, Web::MessageHandler::Method<c,&c##::WebHandler_##f>(this) ) 
#define g3_web_handler( c, f ) int c##::WebHandler_##f( g3::WebRequest& request )
}


#endif