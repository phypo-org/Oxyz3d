#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/math.h>

#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Choice.H>

#include <FL/Fl_File_Icon.H>
#include <FL/Fl_Button.H>

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>


#include "Shape/DataBase.h"
#include "Shape/ObjectFacet.h"
#include "Shape/ObjectLine.h"
#include "Shape/UndoHistory.h"
#include "Shape/SortVisitor.h"
#include "Shape/Selection.h"
#include "Shape/SavRead.h"
#include "Shape/SavReadStl.h"
#include "Shape/ExportObj.h"

#include "Utils/PPFile.h"
#include "Shape/PP3dUtils.h"

#include "Win3d.h"
#include "Canvas3d.h"
#include "Dialogs.h"

#include "Creation.h"
#include "Gui/MyFlWidget.h"
#include "Gui/MyImage.h"
#include "Gui/Preference.h"



#include <memory>

#include <fstream>
#include <stdlib.h>

#include "Callback.h"

using namespace std;

namespace M3d {

//-------------------------------------------
  bool SaveBase( PP3d::DataBase * ioDatabase, const std::string & iName, void* iUseSelect )
  {
    // FAIRE FICHIER .backup
    std::ofstream lOut;						
    lOut.open( iName );
    if( lOut.good() )
      {
	bool lRet;
	PP3d::MySav lSav( lOut );				

	if( iUseSelect )
	  {
	    PP3d::SortVisitorEntity  lVisit;
	    TheSelect.execVisitorOnlyOnObjects( lVisit );	    	    
	    lRet = lSav.save( *ioDatabase, true, &TheSelect, &lVisit.cSetAllEntity );
	  }
	else
	  {
	    lRet = lSav.save( *ioDatabase, true,  &TheSelect, nullptr );
	  }
	
	lOut.close();
	return lRet;
      }
    return false;
  }

  //-------------------------------------------
  bool OpenBase( PP3d::DataBase * ioDatabase,const std::string & iName, bool iKeepId )
  {
    std::ifstream lIn;						
    lIn.open( iName );
    
    if( lIn.good() )
      {									
	PP3d::MyRead lRead( lIn );
	bool lRet = lRead.read( *ioDatabase, &TheSelect, iKeepId );
	lIn.close();
	return lRet;
      }
    return false;
  }  //-------------------------------------------
  //-------------------------------------------
   //-------------------------------------------

  bool ExportD3dObj( PP3d::DataBase * ioDatabase, const std::string & iName, void* iUseSelect )
  {
    std::ofstream lOut;						
    lOut.open( iName );
    if( lOut.good() )
      {
	bool lRet;
	PP3d::MyExportObj lExpObj( lOut );				
	
	if( iUseSelect )
	  {
	    PP3d::SortVisitorEntity  lVisit;
	    TheSelect.execVisitorOnlyOnObjects( lVisit );	    
	    lRet = lExpObj.save( *ioDatabase, &lVisit.cSetAllEntity);
	  }
	else
	  {
	   lRet = lExpObj.save( *ioDatabase, nullptr);
	  }
	lOut.close();
	
	return lRet;
      }
    return false;
  }  //-------------------------------------------
  bool ImportD3dObj( PP3d::DataBase * ioDatabase, const std::string & iName )
  {
    std::ifstream lIn;						
    lIn.open( iName );
    
    if( lIn.good() )
      {									
	PP3d::MyImportObj lRead( lIn );
	std::string lTmp = PPu::PPFile::WithoutExtension( PPu::PPFile::LastPath( iName ) );
	lRead.setName( lTmp );
	bool lRet = lRead.read( *ioDatabase );
	lIn.close();
	return lRet;
      }
    return false;
  }
  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------

  void SaveBaseCB( Fl_File_Chooser *cFc,	// I - File chooser
			  void            *cData )	// I - Data
  {
    printf(" SaveAsCB filename = \"%s\"\n",  cFc->value() ?  cFc->value() : "(null)");        
    
    if( cFc->value() )
      {
	std::string lFilename = cFc->value();

	if( SaveBase( TheCreat.getDatabase(), lFilename, cData) )
	  {
	    MyPref.cLastSave = lFilename;
	  }
	else
	  {
	    fl_alert( "Saving database in <%s> failed", lFilename.c_str());
	  }
      }
  }

   //-------------------------------------------
  void ExportStlCB( Fl_File_Chooser * cFc, void * iUseSelect)
  {    
    std::string lFilename = cFc->value();

    bool lRet = false;
    if( iUseSelect )
      {
	lRet = PP3d::MySavStl::Save( lFilename.c_str(), TheSelect );
      }
    else
      {
	lRet = PP3d::MySavStl::Save( lFilename.c_str(),   *TheCreat.getDatabase());
      }
    if( lRet == false )
      {
	fl_alert( "Export for stl format  in <%s> failed", lFilename.c_str());
      }
 }
  //------------------------------------------------------
  void ExportD3dObjCB(Fl_File_Chooser *cFc,	// I - File chooser
			     void            *cData)	// I - Data
  {    
    printf(" ExportD3dObjCB filename = \"%s\"\n",  cFc->value() ?  cFc->value() : "(null)");        
    
    if( cFc->value()  )
      {
	std::string lFilename = cFc->value();

	
	if( ExportD3dObj( TheCreat.getDatabase(), lFilename, cData ))
	  {
	  }
	else
	  {
	    fl_alert( "Export for D3D format  in <%s> failed", lFilename.c_str());
	  }
      }
  }   //-------------------------------------------
  // on ajoute a la base courante la nlle base
  
  void ImportD3dObjCB( Fl_File_Chooser *cFc,	// I - File chooser
			      void            *cData)	// I - Data
  {    
    printf(" ImportD3dObjCB = \"%s\"\n",  cFc->value() ?  cFc->value() : "(null)");        
    
    if( cFc->value()  )
      {
	PushHistory(); 

	std::string lFilename = cFc->value();
	if( ImportD3dObj(  TheCreat.getDatabase(), lFilename ) ) // On change les ID !!!!!!!!!!!!!!!!!!!!!
	  {
	    TheCreat.redrawAll(PP3d::Compute::FacetAll);
	  }
	else
	  {
	    fl_alert( "Import  for D3D format file <%s> failed", lFilename.c_str() );
	    // A FAIRE : Restore de la base
	  }
      }
  }
  //-------------------------------------------------------------
  //-------------------------------------------------------------
  //-------------------------------------------------------------
  void  UndoCB(Fl_Widget*w, void*pData)
  {
    MyButton* lButton = reinterpret_cast<MyButton*>( pData);
    Win3d*    lWin3d = reinterpret_cast<Win3d*>( lButton->cUserData1);
    
    TheSelect.removeAll();


    std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );
    if( PP3d::UndoHistory::Instance().readPrev( *luBase, &TheSelect ) )
      {
	luBase->resetIdFromMax();
	//	std::cout << "UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU" << std::endl;

 	TheCreat.setDatabase( luBase, false );
      }
    else
      {
      TheSelect.removeAll();
   }

    TheCreat.redrawAll(PP3d::Compute::FacetAll);
  }
  //-------------------------------------------
  void RedoCB(Fl_Widget*w, void*pData)
  {
    MyButton* lButton = reinterpret_cast<MyButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lButton->cUserData1);
    
    TheSelect.removeAll();
    
    std::unique_ptr<PP3d::DataBase> luBase( new PP3d::DataBase() );
    if( PP3d::UndoHistory::Instance().readNext( *luBase, &TheSelect ) )
      {
	luBase->resetIdFromMax();

	TheCreat.setDatabase( luBase, false );
      } 
    else{
      TheSelect.removeAll();
    }
      
 	
    TheCreat.redrawAll(PP3d::Compute::FacetAll);
  }
  //-------------------------------------------
  void PushHistoryCB(Fl_Widget*w, void*pData)
  {
    PushHistory();    
  }
  //------------------------------------------------------
  void CB_ViewTransfrom(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
    
    lWin3d->cuCanvas3d->setViewGeo(  (bool)lToggle->value() );
    lWin3d->canvasRedraw();
  }

  //-------------------------------------------
  //-------------------------------------------
  //-------------------------------------------
  // Change selection mode
  
  void	BasculeSelModeCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
	
    union ConvVoid
    {
      void *             cPtr;
      GLuint             cVal;
      PP3d::SelectType   cSelType; 
    };
	
    ConvVoid lVal;
    lVal.cPtr=  lToggle->cUserData2;

    /*
    // Traitement special pour les groupes 
    TheSelect.changeSelectGroup( false );
    if(  lVal.cSelType == PP3d::SelectType::Group )
      {
        TheSelect.changeSelectGroup( true );
        lVal.cSelType = PP3d::SelectType::Object;
      }
    // Traitement special pour les groupes 
    */
    
    TheSelect.changeSelectType( lVal.cSelType );
 
    // std::cout << "  BasculeSelModeCB " << PP3d::Selection::GetStrSelectType(lVal.cSelType) << std::endl;
    

		
    MyToggleButton* 	lBut0 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData3 );
    MyToggleButton* 	lBut1 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData4 );
    MyToggleButton* 	lBut2 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData5 );
    MyToggleButton* 	lBut3 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData6 );
    MyToggleButton* 	lBut4 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData7 );
    
   //    MyToggleButton* 	lBut4 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData8 );
    //    MyToggleButton* 	lBut5 =  reinterpret_cast<MyToggleButton*>( lToggle->cUserData9 );

    lBut0->value( true );
    lBut1->value( false );
    lBut2->value( false );
    lBut3->value( false );
    lBut4->value( false );
    //    lBut4->value( false );
    //    lBut5->value( false );

    //    std::cout << "BasculeSelModeCB " << lVal.cVal << std::endl;

    TheCreat.redrawAll(PP3d::Compute::FacetAll);
  }
   //-------------------------------------------
  // Change Geo mode
  
  void	BasculeSelModeGeoCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
	

    //    std::cout << " value:" << (int)lToggle->value();
    if( lToggle->value() )
      lToggle->value( true );
    else
      lToggle->value( false );
    //    std::cout << " ---> value:" << (int)lToggle->value() << std::endl;

		
    lWin3d->cuCanvas3d->setSelectGeo(  (int)lToggle->value() );		

    lWin3d->canvasRedraw();
  }
 //-------------------------------------------
  // Change view mode
  
  void	BasculeViewModeCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
	

    //    std::cout << " value:" << (int)lToggle->value();
    if( lToggle->value() )
      lToggle->value( true );
    else
      lToggle->value( false );
    //    std::cout << " ---> value:" << (int)lToggle->value() << std::endl;

		
    lWin3d->cuCanvas3d->setVisualMode( lToggle->value() ? PP3d::ViewMode::SKELETON : PP3d::ViewMode::FULL );		

    lWin3d->canvasRedraw();
  }
  //-------------------------------------------
  void	BasculeBoolCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d = reinterpret_cast<Win3d*>( lToggle->cUserData1);
    bool* lBool = reinterpret_cast<bool*>( lToggle->cUserData2);
    *lBool = ! *lBool;
		
    lWin3d->canvasRedraw();
  }
  //-------------------------------------------
	
  void	BasculeGridCB(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d        = reinterpret_cast<Win3d*>( lToggle->cUserData1);
		
    ModeGrid *lGridMode = reinterpret_cast<ModeGrid*>( lToggle->cUserData2);
	 
    if( *lGridMode == ModeGrid::NO_GRID )
      {
	*lGridMode = ModeGrid::GRID_2D;
      }
    else 
      {
	*lGridMode = ModeGrid::NO_GRID;
      }
		
    lWin3d->canvasRedraw();
  }
 //-------------------------------------------
   void	ChangeUnity(Fl_Widget*w, void*pData)
   {
     MyChoiceButton* lChoice = reinterpret_cast<MyChoiceButton*>( pData);
     Win3dBase* lWin3d           = reinterpret_cast<Win3dBase*>( lChoice->cUserData1);
    cout << "ChangeUnity button : " << lChoice->value() << endl;
    lWin3d->getKamera().setScaleToMeasurement( lChoice->value() );
    lWin3d->setMeasurement( lChoice->value() );
    lWin3d->canvasRedraw();
   }
 //-------------------------------------------
	
  void	BasculePerspective(Fl_Widget*w, void*pData)
  {
    MyToggleButton* lToggle = reinterpret_cast<MyToggleButton*>( pData);
    Win3d* lWin3d        = reinterpret_cast<Win3d*>( lToggle->cUserData1);

    cout << "Changement perpective button" << endl;
    lWin3d->getKamera().chgModeKamera();
		
    lWin3d->canvasRedraw();
  }		
}
