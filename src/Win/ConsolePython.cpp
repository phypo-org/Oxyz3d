#ifdef USE_PYTHON

#include <string.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Editor.H>

#include <Python.h>

//*********************************************
class ConsolePython : public Fl_Text_Editor {

	Fl_Text_Buffer *buff;
	char cmd[1024];
	
	wchar_t *lProgram=nullptr;
	
public:
	ConsolePython(int X,int Y,int W,int H,const char* L=0) : Fl_Text_Editor(X,Y,W,H,L)
	{
		buff = new Fl_Text_Buffer();
		buffer(buff);
		textfont(FL_COURIER);
		textsize(12);
		cmd[0] = 0;
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
		fprintf(stderr, "EXECUTING: '%s'\n", command);

		PyRun_SimpleString( command );
		
		FILE *fp = popen(command, "r");
		if ( fp == 0 ) {
			append("Failed to execute: '");
			append(command);
			append("'\n");
		} else {
			char s[1024];
			while ( fgets(s, sizeof(s)-1, fp) ) {
				append(s);
			}
			pclose(fp);
		}
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
				strcat(cmd, " 2>&1");                // stderr + stdout
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
	//--------------------------------------------------
	//--------------------------------------------------
	//---------------------------------------------------
	void initPython()
	{
		lProgram = Py_DecodeLocale("", nullptr);
    if (lProgram == NULL) {
			fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
			//			exit(1);
    }
    Py_SetProgramName(lProgram);  /* optional but recommended */
    Py_Initialize();
	}
	//--------------------------------------------------
	void closePython()
	{
		Py_Finalize();
    PyMem_RawFree(lProgram);
	}
};
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
extern Fl_Double_Window* CallConsolePython()
{	
	static Fl_Double_Window* lWin = nullptr;

	if( lWin == nullptr )
		{
			lWin =new Fl_Double_Window(600, 400, "Python");
			ConsolePython* lConsol  = new  ConsolePython(10,10, lWin->w()-20, lWin->h()-20);
			lConsol->append(">");
			lWin->end();
			
			lWin->resizable(*lWin);
			
		}
	lWin->show();

	return lWin;
}
#endif
