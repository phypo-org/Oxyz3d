#include <string.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Editor.H>

#include "Lua/PLua.h"
#include "ShapeLua.h"

using namespace std;
using namespace PLua;


CLUA_OPEN_CODE(LUA_Test1, 1) 


if( lua_isstring(pLua, 1) )
  {
    std::cout << lua_tostring(pLua, 1) << std::endl;
  }

lua_pushnumber( pLua, 1 );

CLUA_CLOSE_CODE(1);


//*********************************************
class ConsoleLua : public Fl_Text_Editor {
	
	
  Fl_Text_Buffer *buff;
  char cmd[1024];
	
  wchar_t *lProgram=nullptr;
  
  
	M3d::ShapeLua* cLua=nullptr;
  
public:
  ConsoleLua(PP3d::DataBase&iDataBase,int X,int Y,int W,int H,const char* L=0) : Fl_Text_Editor(X,Y,W,H,L)
  {
    M3d::ShapeLua::SetPrototype();
    
    buff = new Fl_Text_Buffer();
    buffer(buff);
    textfont(FL_COURIER);
    textsize(12);
    cmd[0] = 0;

    cLua=  (M3d::ShapeLua*)M3d::ShapeLua::GetOrCreateSession("Console", &cout );
		cLua->setDatabase( iDataBase );
    cLua->registerFunction( "", "Test1", LUA_Test1  );
		
		cLua->doCode( "PPrintln(\"Hello it's C++\" )");
		cLua->doCode( "PListLib()");
		cLua->doCode( "PListLibFtn()" );
    cLua->doCode( "print(\"Hello it's Lua\")" );
		cLua->doCode("Test1(\"Coucou\")");
		cLua->doCode("ShapeAddCurrentPoint(2,4,6)");
		cLua->doCode("ShapeAddCurrentPoint(4,5,7)");
		cLua->doCode("WinNewCanvas3d(800,600)");
  }
  //---------------------------------------------------
  ~ConsoleLua()
  {
    delete cLua;
    cLua = nullptr;
  }
  //---------------------------------------------------
  // Append to buffer, keep cursor at end
  void append(const char*s)
  {
    buff->append(s);
    // Go to end of line
    insert_position(buffer()->length());
    scroll(count_lines(0, buffer()->length(), 1), 0);
  }
  //---------------------------------------------------
  // Run the specified command in the shell, append output to terminal
  void RunCommand(const char *command)
  {
    append("\n");
    fprintf(stderr, "RunCommand: '%s'\n", command);

    cLua->doCode( command );
		
  }
  //---------------------------------------------------
  // Handle events in the Fl_Text_Editor
  int handle(int e) {
    switch (e) {
    case FL_KEYUP: {
      int key = Fl::event_key();
      if ( key == FL_Enter ) return(1);              // hide Enter from editor
      if ( key == FL_BackSpace && cmd[0] == 0 ) return(0);
      break;
    }
    case FL_KEYDOWN: {
      int key = Fl::event_key();
      // Enter key? Execute the command, clear command buffer
      if ( key == FL_Enter ) {
	// Execute your commands here
	strcat(cmd, "");                // stderr + stdout
	RunCommand(cmd);
	cmd[0] = 0;
	append("\n$ ");
	return(1);                          // hide 'Enter' from text widget
      }
      if ( key == FL_BackSpace ) {
	// Remove a character from end of command buffer
	if ( cmd[0] ) {
	  cmd[strlen(cmd)-1] = 0;
	  break;
	} else {
	  return(0);
	}
      } else {
	// Append text to our 'command' buffer
	strncat(cmd, Fl::event_text(), sizeof(cmd)-1);
	cmd[sizeof(cmd)-1] = 0;
      }
      break;
    }
    }
    return(Fl_Text_Editor::handle(e));
  }
};
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
extern Fl_Double_Window* CallConsoleLua(PP3d::DataBase&iDataBase)
{	
  static Fl_Double_Window* lWin = nullptr;

  if( lWin == nullptr )
    {
      lWin =new Fl_Double_Window(600, 400, "Lua");
      ConsoleLua* lConsol  = new  ConsoleLua(iDataBase, 10,10, lWin->w()-20, lWin->h()-20);
      lConsol->append(">");
      lWin->end();
			
      lWin->resizable(*lWin);
			
    }
  lWin->show();

  return lWin;
}
