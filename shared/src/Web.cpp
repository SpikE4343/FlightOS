//=====================================================================
// This file is part of FlightOS.
//
// FlightOS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FlightOS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlightOS.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Util.h"
#include "System.h"
#include "Web.h"
#include "Log.h"
#include "TaskManager.h"

namespace FlightOS 
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

  void Web::DrawUI(float s)
  {
    if (!ImGui::CollapsingHeader("Web"))
    {
      return;
    }

    ImGui::LabelText("Handlers", "%u", mHandlers.size());
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