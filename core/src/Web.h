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

#ifndef FlightOS_Web_H_INCLUDED
#define FlightOS_Web_H_INCLUDED

#include "Util.h"
#include "mongoose/mongoose.h"

#include <string>
#include <unordered_map>

namespace FlightOS
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

class Web : public SystemModule
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

  void DrawUI(float s);

private:
	static Web* m_sInstance;

  typedef std::unordered_map< std::string, MessageHandler > HandlerMap;

  HandlerMap mHandlers;

  struct mg_context* mMgCtx;

};

#define FlightOS_web_declare_handler( f ) int WebHandler_##f(FlightOS::WebRequest& request )
#define FlightOS_web_register_handler( c, n, f ) Web::getInstance()->registerHandler( n, Web::MessageHandler::Method<c,&c##::WebHandler_##f>(this) ) 
#define FlightOS_web_handler( c, f ) int c##::WebHandler_##f( FlightOS::WebRequest& request )
}


#endif