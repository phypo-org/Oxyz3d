#include "WinHisto.h"

#include <FL/Fl_Group.H>


#include <FL/Fl_Widget.H>


#include "Shape/Selection.h"
#include "Application.h"


#include "MyFlWidget.h"




namespace M3d {

	WinHisto* WinHisto::sTheWinHisto =nullptr;

	
	//****************************************************
	WinHisto::WinHisto( )
	{
		cWin  = new Fl_Double_Window(400, 580, "Histoy");
		
    cTree = new Fl_Tree( 1, 1, 390, 570, "HistoyTree");
		
		cTree->callback((Fl_Callback*)CallBackTree, (void*)(this));
		cWin->end();
		cWin->resizable(*cWin);
		cWin->show( 0, nullptr);
		//   Fl::run();
	}
//--------------------------------------------
	void WinHisto::show()
	{
		cWin->show();
	}

//--------------------------------------------
	static void	SelectOneObject(Fl_Widget*w, void*pData)
	{
		MyCheckbutton* lCheck = reinterpret_cast<MyCheckbutton*>(pData);
	
		PP3d::Object*lObj  = reinterpret_cast<PP3d::Object*>(lCheck->cUserData2);

		//	std::cout << "Box:"  << (lCheck->value() != 0) << std::endl;
		if( PP3d::Selection::Instance().addToSelection( lObj->getShape() ) )
			{
				Application::Instance().validate( History::SaveMode::Mini);			
			}
	}
//--------------------------------------------
	static void	ViewOneObject(Fl_Widget*w, void*pData)
	{
		MyCheckbutton* lCheck = reinterpret_cast<MyCheckbutton*>(pData);
		
		//		WinHisto* lWinTree = reinterpret_cast<WinHisto*>(lCheck->cUserData1);
		PP3d::Object*lObj  = reinterpret_cast<PP3d::Object*>(lCheck->cUserData2);

		lObj->setVisible( (lCheck->value() != 0) );
		Application::Instance().redrawAllCanvas3d();
	}
//--------------------------------------------
	static void	RenameObject(Fl_Widget*w, void*pData)
	{
		MyInput* lInput = reinterpret_cast<MyInput*>(pData);

		
		History::HistoryLink*lObj  = reinterpret_cast<History::HistoryLink*>(lInput->cUserData2);
		
		std::cout << "Rename " << lInput->value() << std::endl;
		lObj->cName = lInput->value();
		//		Application::Instance().validate( History::SaveMode::Diff);
		
	}
//--------------------------------------------
	void WinHisto::draw()
	{
				rebuild();
	}		
//--------------------------------------------
	void WinHisto::rebuild()
	{
		std::cout << "+++++++ REBUILD TREE ++++++++++" << std::endl;
		cTree->clear();

		
		for( History::HistoryLink* lHistoLink : Application::Instance().getHistory()->getHistoLinks() )
			{
				cTree->begin();
				std::ostringstream lOstr;
				
				lOstr << lHistoLink->cIdLink
							<< ":" << lHistoLink->getStrSavMode()
							<< ":" << lHistoLink->cName
							<< ":" << lHistoLink->cTime 
							<< ":" << (lHistoLink->cuMyBase.get() != nullptr ? "*" : "0" ) << "/ 3333  ";
					
				Fl_Tree_Item* lNode = cTree->add( lOstr.str().c_str() );
						cTree->end();				
						cTree->begin();
						
				Fl_Group* lGroup = new Fl_Group(200,100,140,18);
				lGroup->color( FL_WHITE );
				lGroup->begin();
				std::ostringstream lOstrName;

				lOstrName << "type:" << lHistoLink->cName;

				int lX = lGroup->x();
				int lY = lGroup->y()+2;

				/*		//------------				
				MyCheckbutton *lCheckSel = new MyCheckbutton( lX, lY, 30,15, "S", SelectOneObject, this, lObj ); //lInput->x()+lInput->w()+4+40 ,
				lCheckSel->value( PP3d::Selection::Instance().isSelected( lObj->getShape() ));
				lX += lCheckSel->w()+4;


				//------------
				MyCheckbutton *lCheckView = new MyCheckbutton( lX, lY, 30,15, "V", ViewOneObject, this, lObj ); //lInput->x()+lInput->w()+4+40 ,
				lCheckView->value( lObj->isVisible());
				lX += lCheckView->w()+4;
				
				*/
				//------------
				
				MyInput* lInput = new MyInput( lX, lY, 100, 15, "", RenameObject, this, lHistoLink  );
				lInput->value( lHistoLink->cName.c_str() );
			
				//------------

				
				lGroup->end();
				lGroup->resizable(lGroup);
				cTree->end();
        
				lGroup->show();
				lNode->widget(lGroup);				
			}
		std::cout << "------- REBUILD TREE ---------" << std::endl;

		cTree->redraw();
	}
	//--------------------------------------------
	void WinHisto::CallBackTree( Fl_Widget* pWidget, void* pUserData )
	{
		//		WinHisto* lWinTree = reinterpret_cast<WinHisto*>( pUserData );
	}
	//****************************************************
	
};
