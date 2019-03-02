#ifndef H__MyFlWidget__H
#define H__MyFlWidget__H



#include "MyFlCallback.h"
#include <FL/Fl_Widget.H>

#include "FL/Fl_Check_Button.H"
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Menu_Item.H>

namespace M3d {
	
	//*********************************************************

//--------------------------------------------
	// Je derive de MyUserData pour beneficier de la desalocation memoire automatique que va faire
	// fltk sur le check button ! Sinon il faudrait gérer un vecteur de MyUserData et les detruire au
	// moment du rebuild (bof)

	template<typename TMY_FLTK>
	
		struct TMyFltk : public TMY_FLTK, public MyUserData{
		
		TMyFltk( int pX, int pY, int pW, int pH, const char*pCheck, Fl_Callback pCallback,  void* pUserData1=nullptr, void* pUserData2=nullptr, void* pUserData3=nullptr, void* pUserData4=nullptr, void* pUserData5=nullptr, void* pUserData6=nullptr, void* pUserData7=nullptr, void* pUserData8=nullptr )
			:TMY_FLTK( pX, pY, pW, pH, pCheck )
			,MyUserData( pUserData1, pUserData2, pUserData3, pUserData4, pUserData5, pUserData6, pUserData7,pUserData8)
		{	
			TMY_FLTK::callback( pCallback, (void*)this);
			//tooltip("undefine");
		}
	};
	using MyCheckbutton  = TMyFltk<Fl_Check_Button>;
	using MyInput        = TMyFltk<Fl_Input>;
	using MyButton       = TMyFltk<Fl_Button>;
	using MyToggleButton = TMyFltk<Fl_Toggle_Button>;




	struct MyMenuItem : public Fl_Menu_Item, public MyUserData
	{
		
		MyMenuItem( const char* pName, int pSc,  int pFlag, Fl_Callback pCallback,  void* pUserData1=nullptr, void* pUserData2=nullptr, void* pUserData3=nullptr, void* pUserData4=nullptr, void* pUserData5=nullptr, void* pUserData6=nullptr, void* pUserData7=nullptr, void* pUserData8=nullptr )
			
			:MyUserData( pUserData1, pUserData2, pUserData3, pUserData4, pUserData5, pUserData6, pUserData7,pUserData8)			
		{
			text      = pName;
			shortcut(  pSc);
			callback(  pCallback);
			user_data( (void*)this);
			flags = pFlag;
		}
	
	};

	//*********************************************************
};

#endif
