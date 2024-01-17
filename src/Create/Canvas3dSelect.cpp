

#include "Canvas3d.h"

//#include <config.h>
#include <FL/Fl.H>

 
#include <FL/gl.h>

#include <FL/names.h>
#include <FL/Fl_Menu_Button.H>

#include <FL/fl_draw.H>

#include "Service.h"


#include <iostream>
#include <memory>


#include "Shape/ObjectLine.h"
#include "Shape/ObjectPoly.h"
#include "Shape/GLUtility.h"
#include "Shape/PrimitivFactory.h"
#include "Shape/Light.h"
#include "Shape/ViewProps.h"
#include "Shape/Selection.h"
#include "Shape/SortVisitor.h"
#include "Shape/PP3dUtils.h"
#include "Shape/MinMaxBox3d.h"
#include "Shape/Line3dVect.h"

#include "Creation.h"
#include "MyFlWidget.h"

#include "Dialogs.h"
#include "Win3d.h"

#include "Preference.h"
#include "Callback.h"


#include "SelFunct.h"

using namespace std;


namespace M3d {

//---------------------------
  bool Canvas3d::userSelectionRectangle(int pEvent, bool pFlagFinalize )
  {
    //  cout << "userSelectionRectangle x:" <<  cMouseInitPosX  << " y:" << cMouseInitPosY
    //	 << "   x:" <<  cMouseLastPosX  << " y:" <<  cMouseLastPosY << endl;

    
    if( cMouseInitPosX == -1 )
      {
	cMouseInitPosX = Fl::event_x();
	cMouseInitPosY = Fl::event_y();			
      }     
    
    //   cout << "userSelectionRectangle x:" <<  cMouseInitPosX  << " y:" << cMouseInitPosY
    //	 << "   x:" <<  cMouseLastPosX  << " y:" <<  cMouseLastPosY << endl;

    TheCreat.currentTransform().raz();
    int lLastY = pixel_h()-cMouseLastPosY;
    int lInitY = pixel_h()-cMouseInitPosY;
      
    //    float lMiddleX = (cMouseInitPosX + cMouseLastPosX)/2.0f;
    //    float lMiddleY = (lInitY + lLastY)/2.0f;
    float lMinX  = std::min( cMouseInitPosX , cMouseLastPosX );
    float lMinY  = std::min( lLastY , lInitY );
    float lMaxX  = std::max( cMouseInitPosX , cMouseLastPosX );
    float lMaxY  = std::max( lLastY , lInitY );
    float lSzX   = lMaxX - lMinX;
    float lSzY   = lMaxY - lMinY;
    
    //   cout << "userSelectionRectangle Middle x:" <<  lMinX<< " y:" <<lMinY
    //	 << "  Sz  x:" <<  lSzX  << " y:" <<  lSzY << endl;

    if( lSzX> 0 && lSzY > 0 )
      {
	if( pFlagFinalize )
	  {
	    pickingColor( lMinX, lMinY, false, lSzX, lSzY, true );
	    cMouseInitPosX = cMouseLastPosX = -1;
	    cMouseInitPosY = cMouseLastPosY = -1;
 	  }
	else
	  {
	    pickingColor( lMinX, lMinY, true, lSzX, lSzY, true );
	  }
      }
    return true;
  }
  //---------------------------------------------------------
  bool Canvas3d::userSelectionPointColor(int pEvent, bool pFlagMove)
  {
    return pickingColor( Fl::event_x(),   pixel_h() - Fl::event_y(), pFlagMove, MyPref.cSelectPickingSize, MyPref.cSelectPickingSize, false );
  }
  //---------------------------------------------------------
  bool Canvas3d::pickingColor( int pX, int pY, bool pFlagMove, int iSizeX, int iSizeY, bool pFlagRect  )
  {
    // cout << ":pickingColor x:" << pX << " y:" << pY << " sX:" << iSizeX << " sY:" << iSizeY << " R:"<<  pFlagRect<< endl; 
  
    drawForSelect(); // AJOUTER LA TAILLE
    glFinish(); 
  
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if( pFlagRect == false )
      {
	unsigned char lData[4];
	glReadPixels( pX, pY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, lData);
	////	cout << "Data: " << std::hex << (int)lData[0] <<" " <<   (int)lData[1]<<" "  <<  (int)lData[2] <<" " <<  (int)lData[3]  << endl;


	PP3d::EntityId lId = PP3d::ColorRGBA::GetId( lData );
	//	cout << "Canvas3d::pickingColor " << lId << endl; // AAAAAAAAAAAAAAAAAAA
	
	if( lId == 0 )
	  {
	    return false;
	  }

	int lMem = TheCreat.getInput().getCurrentLineSelectPoint();
	// Est ce l'un des points de la saisie courante
	if( TheInput.testAndSelectCurrentLinePoint( lId ) )
	  {
	    if( TheInput.getCurrentLineSelectPoint() == lMem )
		  {
		    cout << "Same Point !!!" << endl;
		    
		    TheInput.editCurrentLineSelectPoint();
		    if( getUserMode() == ModeUser::MODE_BASE
			&& Fl::event_button() == FL_LEFT_MOUSE  )
			  {
			    cout << "MODE_DRAG !!!" << endl;
			    changeUserMode( ModeUser::MODE_DRAG );			
			  }		    
		  }
	    /*
	    if( Fl::event_shift() )
	      {
		cout << "Shif" << endl;
		if( TheInput.getCurrentLineSelectPoint() == lMem )
		  {
		    TheInput.editCurrentLineSelectPoint();
		      
		    cout << "Same Point !!!" << endl;
		  }
	      }
	    */	   
	    //	    cout << "pickingColor testAndSelectCurrentLinePoint return true " << endl; //AAAAAAAAAAA
	    return true;
	  }

	selectEntity( lId, pFlagMove); 
      }
    else
      {
	if( iSizeX <= 0 || iSizeY <=  0 )
	  {
	    return false;
	  }
	
	/*	cout << "Canvas3d::pickingColor move:" << pFlagMove
	     << " Pos X:"<< pX << " Y:" <<pY
	     << " SzX:"  << iSizeX << " SzY:" << iSizeY
	     << " pFlagRect:" << pFlagRect
	     << endl;
	*/
	
	auto luData = std::make_unique<unsigned char[]>( iSizeX*4 );
	PP3d::EntityId lMemoId = 0;
	
	for( int lY = pY; lY < pY+iSizeY; lY++ )
	  {
	    ::memset( luData.get(), 0xFF, iSizeX*4 );
	    //    cout << std::endl << "pickingColor lY:" << lY << std::endl;
	    glReadPixels( pX, lY, iSizeX, 1, GL_RGBA, GL_UNSIGNED_BYTE, luData.get() );
	    
	    for( int lX = 0; lX< iSizeX; lX++ )
	      {
		PP3d::EntityId lId = PP3d::ColorRGBA::GetId(  &(luData)[lX*4]);
		if( lMemoId != lId )
		  {
		    //	    cout << "pickingColor lY:" << lY << " lId:" << lId << std::endl;

		    lMemoId = lId;
		    selectEntity( lId, pFlagMove );
		  }
	      }
	  }
      }
    return true;
  }
//---------------------------
  bool Canvas3d::selectEntity( PP3d::EntityId iId, bool pFlagMove  )
  {
    bool lFlagRedraw = false;
    
    //    cout << "Find Id : " << iId << endl;
    PP3d::EntityPtr lEntity = TheCreat.getDatabase()->findEntity( iId );
    if( lEntity != nullptr )
      {
	//	cout << "********** Find Entity : " << iId <<  endl;	       	
        
	////    cout <<" processHits before Select" <<endl; 
	
	//	PP3d::SelectMode lSelectMode = 	PP3d::SelectMode::Select;
	
	lFlagRedraw = TheSelect.selectPickingColor(  lEntity,
						    *TheCreat.getDatabase(),
						     cSelectMode,
						    pFlagMove );	
	////	    cout << " processHits after Select" <<endl;
	
	if( lFlagRedraw )	    
	  TheCreat.redrawAll(PP3d::Compute::Nothing);
      }
    return lFlagRedraw;
  }
  //---------------------------
  bool Canvas3d::initDragSelect()
  {
    if( cDragPoints.size() == 0
	&&TheSelect.getNbSelected() >0 )
      {
	std::cout << "******************* Canvas3d::initDragSelect " << TheSelect.getNbSelected() << std::endl;
	std::cout << "******************* Canvas3d::initDragSelect " << TheSelect.getSelection().size() << std::endl;

	cDragCenter =   TheSelect.getCenter( *TheCreat.getDatabase() );			
	// We keep all the adress of points of selected entities
	PP3d::GetPoints< PP3d::EntityPtrHash, PP3d::PointPtrSet>(TheSelect.getSelection(),
								 cDragPoints );

	// Save the original coordinates of points       	  
	cDragSavPoints.resize( cDragPoints.size() );
	int lP=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    cDragSavPoints[lP++] = lPoint->get();
	  }
	return true;
      }
    return false;
  }
  //---------------------------
  void Canvas3d::dragSelect( PP3d::Mat4 &pMat )
  {
    if( initDragSelect() == false )
      {				
	int lP=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    lPoint->get() = cDragSavPoints[lP++] * pMat;  // 
	  } 			
      }
  }
  //---------------------------
  void Canvas3d::validDragSelect( PP3d::Mat4 & pMat )
  {
    std::cout << "validDragSelect PushHistory " << std::endl;
    
    dragSelect( pMat ); // Inutile ? 
    cDragPoints.clear();
    cDragSavPoints.clear();
    PushHistory();
  }
  //---------------------------
  void Canvas3d::validDragSelect( )
  {
    std::cout << "validDragSelect PushHistory " << std::endl;
    
    cDragPoints.clear();
    cDragSavPoints.clear();
    PushHistory();
  }
  //---------------------------
  void Canvas3d::cancelDragSelect()
  {
    if( cDragPoints.size() != 0 )
      {
	int lP=0;
	for( PP3d::PointPtr lPoint : cDragPoints )
	  {
	    lPoint->get() = 	cDragSavPoints[lP++]; // restore original values 
	  }
	cDragPoints.clear();
	cDragSavPoints.clear();
      }
  }
} //namespace 
