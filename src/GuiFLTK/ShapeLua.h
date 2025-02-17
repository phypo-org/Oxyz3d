#ifndef H__SHAPELUA__H
#define H__SHAPELUA__H

#ifdef USING_LUA

#include "Shape/DataBase.h"

#include "Lua/PLua.h"


namespace M3d {

  //****************************************

  class ShapeLua : public PLua::PLuaSession{
		
  public:		
				
    ShapeLua(){ std::cout << "************************* ShapeLua::prototype" << std::endl;}
    ShapeLua(  const std::string&, std::ostream* pStream=nullptr );
    PLua::PLuaSession* getNewPrototypeSession(  const std::string&  iNameSession,  std::ostream* iStream ) override
    {
      return new ShapeLua( iNameSession, iStream );
    }
  public :
    static void SetPrototype()
    {
      std::cout	 << "11111111111 ShapeLua::prototype" << std::endl;

      ShapeLua * lTmp  =	new ShapeLua ();
      PLuaSession::SetPrototype( lTmp );
    }
    void registerMyFunction();
	  
  };
  //****************************************

}
#endif

#endif
