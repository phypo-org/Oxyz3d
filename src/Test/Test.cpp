#include <iostream>


#include "PLua.h"


using namespace std;


using namespace PLua;

//***********************************
int main( int argc, char* argv[])
{
  cout << "Hello" << endl;

  PLuaSession* lLua=  PLuaSession::GetOrCreateSession( "Test", &cout );
  lLua->doCode( " print(\"Hello it's Lua\")" );  
}
