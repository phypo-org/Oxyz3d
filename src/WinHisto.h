#ifndef  H__WinHisto__H
#define  H__WinHisto__H



#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Tree_Item.H>



#include <string>
#include <memory>

#include "Canvas3d.h"
#include "MyFloatInput.h"

#include "Shape/DataBase.h"



namespace M3d {
	
	//****************************************
	class WinHisto 
	{
	protected:
		
		Fl_Double_Window     *cWin;
		Fl_Tree              *cTree;
		//	Fl_Tree_Item     cRoot;
		WinHisto();
		
	public:
		void rebuild();
		void show();

		static void CallBackTree( Fl_Widget* pWidget, void* pUserData );
		static void NodeButton_CB( Fl_Widget* pWidget, void* pUserData );

	private:
		static WinHisto* sTheWinHisto;

	public:
		static WinHisto& Instance()
		{
			if( sTheWinHisto == nullptr )
				{
					sTheWinHisto = new WinHisto();
				}
			return *sTheWinHisto ; 
		}
		
		void redraw() { cWin->redraw(); }
		void draw();
	};
//******************************************
};


#endif
	
