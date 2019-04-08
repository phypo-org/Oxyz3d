#include <iostream>


#include "PLua.h"


using namespace std;


using namespace PLua;




CLUA_OPEN_CODE(LUA_Test1, 1) 


if( lua_isstring(pLua, 1) )
{
  std::cout << lua_tostring(pLua, 1) << std::endl;
}

lua_pushnumber( pLua, 1 );

CLUA_CLOSE_CODE(1);




//***********************************
int main( int argc, char* argv[])
{
  cout << "Hello" << endl;

  PLuaSession* lLua=  PLuaSession::GetOrCreateSession( "Test", &cout );

  lLua->registerFunction( "TOTO", "Test1", LUA_Test1  );
  
  lLua->doCode( "print(\"Hello it's Lua\")" );
  lLua->doCode( "print(\"Hello it's Lua\")" );
  lLua->doCode( "PLC_Println(\"Hello it's C++\" )");
  lLua->doCode( "PLC_ListLib()");
  lLua->doCode( "PLC_ListLibFtn()" );
  lLua->doCode( "TOTO_Test1(\"It's toto\")" );

  lLua->doCode( "print(\"Hello it's Lua\")" );

  
}

