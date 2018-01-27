/*************************************************************************************
g3::Web
Copyright (c) 2013 John Rohrssen
Email: johnrohrssen@gmail.com
*************************************************************************************/

#include "g3Util.h"
#include "g3System.h"
#include "g3Web.h"
#include "g3Log.h"
#include "g3TaskManager.h"



namespace g3 
{

  // This function will be called by mongoose on every new request.
  static int begin_request_handler(struct mg_connection *conn) 
  {
    WebRequest req( conn );
    return Web::getInstance()->handleRequest( req );
  }

  // Web
  //
  Web* Web::m_sInstance = NULL;

  Web::Web() :
    mMgCtx(NULL)
  {
    if( m_sInstance == NULL )
      m_sInstance = this;
  }

  Web* Web::getInstance()
  {
    return m_sInstance;
  }

  int Web::initialize(const InitInfo& info)
  {
    //struct mg_callbacks callbacks;

    // List of options. Last element must be NULL.
    const char *options[] = {"listening_ports", "80", NULL};

    // Prepare callbacks structure. We have only one callback, the rest are NULL.
    //memset(&callbacks, 0, sizeof(callbacks));
    //callbacks.begin_request = begin_request_handler;

    // Start the web server.
    //mMgCtx = mg_start(&callbacks, NULL, options);

    Log::info( "Web initialized" );
    return 1;
  }


  int Web::update()
  {
    return 1;
  }

  int Web::shutdown()
  {
    //if( mMgCtx != NULL )
    //  mg_stop(mMgCtx);

    Log::info( "Web shutdown" );
    return 1;
  }

  void Web::registerHandler( const char* url, MessageHandler& handler )
  {
    mHandlers[ url ] = handler;
  }

  int Web::handleRequest( WebRequest& request )
  {
    return 0;

    /*HandlerMap::iterator it = mHandlers.find( request.mRequestInfo->uri );
    if( it == mHandlers.end() )
    return 0;

    MessageHandler& handler = it->second;
    return handler(request);*/
  }

  // WebRequest
  //
  WebRequest::WebRequest( struct mg_connection* conn ) : 
    mConnection(conn),
    //mRequestInfo(mg_get_request_info(conn)),
    mResponsCode(404)
  {

  }

  void WebRequest::send()
  {
    mg_printf(mConnection,
      "HTTP/1.1 %d OK\r\n"
      "Content-Type: %s\r\n"
      "Content-Length: %u\r\n"        // Always set Content-Length
      "%s"
      "\r\n"
      "%s",
      mResponsCode, 
      mContentType.c_str(),
      mResponse.length(),
      mHeaders.c_str(),
      mResponse.c_str());

  }

}