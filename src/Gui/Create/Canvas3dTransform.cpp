
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
#include "Gui/MyFlWidget.h"

#include "Dialogs.h"
#include "Win3d.h"

#include "Gui/Preference.h"
#include "Callback.h"


#include "SelFunct.h"


#include "WarpPointer.h"

using namespace std;



namespace M3d {

  //---------------------------
  void Canvas3d::userTransformSelection(int pEvent, bool pFlagFinalize)
  {
    
    int lX = Fl::event_x();
    int lY = Fl::event_y();
 
    float lDx = (float)(lX - cMouseLastPosX);
    
    float lDy = (float)(cMouseLastPosY - lY);
    cMouseLastPosX = lX;
    cMouseLastPosY = lY;
    
    
    lDx /= 10.;   // MODIFIER EN FONCTION DE LA TRANSFORMATION !!!
    lDy /= 10.;   // MODIFIER EN FONCTION DE LA TRANSFORMATION !!!

    userTransformSelectionInternal( lDx, lDy,  pFlagFinalize);
  }

  //---------------------------
  void Canvas3d::userTransformSelectionInput( int pEvent )
  { 
    Transform lCurrent= TheCreat.getCurrentTransformType();

    std::cout << "currentValTransf:" << TheCreat.currentValTransf() << std::endl;
    
    if( CallDialogInputDouble( "Value", TheCreat.currentValTransf(), [&]( double ioVal, bool  ioRun ) {

      //    std::cout << " F val:" << ioVal << " " << ioRun << std::endl;
      TheCreat.setCurrentTransformType( lCurrent );
      userTransformSelectionInternal( ioVal, ioVal, !ioRun);
      TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing); 

      return true;
    }))
      {
        ;
      }
    else
      {
        std::cout << "userTransformSelectionInput CANCEL" << std::endl;        
        userActionCancel(  );
      }
  }
  //---------------------------   
  void Canvas3d::userTransformSelectionInternal( float iDx, float iDy, bool pFlagFinalize)
  {      
    std::cout << "***** Canvas3d::userTransformSelectionInternal **** " << iDx << std::endl;
    
    TheCreat.currentValTransf() = TheCreat.currentValTransf() + iDx;
 
    
    PP3d::Mat4 lMatTran;
    lMatTran.identity();

    if( TheCreat.getCurrentDeformType() != TypeDeform::Nothing )
      {
	/*
	  std::cout << "======== Grab Deform " << std::endl;
 
	  
	  if(cVisitModifSelect == nullptr )
          {
          cVisitModifSelect = new PP3d::VisitorDeform(TheSelect, TheCreat.getCurrentDeformType(), TheCreat.getCurrentTransformType()  );// DIFFERENCE !!!
          cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect);
          }
	  
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des modifs
	  cVisitModifSelect->setCoef( ((double)cMouseLastPosX-cMouseInitPosX)/30 );// DIFFERENCE !!!
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect);
	  
	  if( pFlagFinalize)
          validDragSelect( lMatTran );
	*/
        return;    //////////// ATTENTION 
      }
    
  
    switch( TheCreat.getCurrentTransformType() )
      {
	//================
      case Transform::Nothing:
	std::cout << "Nothing to do !!!" << std::endl;
	break;
	//	return ;

  	//================
      case Transform::MagnetMove:
        {
          //  std::cout << "======== Transform::MagnetMove "
          //          << TheCreat.currentValTransf()
          //         << std::endl;
          
          if(cVisitModifSelect == nullptr )  // la premiere fois !
	    {
	      cVisitModifSelect = new VisitorMagnet(TheSelect, getMagnet(), iDx, iDy );
              cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect); // sauvegarde des points originaux !
 	    }
          
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des anciennes modifs
          cVisitModifSelect->setCoef( TheCreat.currentValTransf() );  // prise en compte de la souris !!!
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect); // Modification
	  
	  if( pFlagFinalize)
            {
              std::cout << "======== Transform::MagnetMove finalize " << std::endl;
	    validDragSelect( );
            }
	  return;    //////////// ATTENTION 
	}

        
	//================
      case Transform::MoveNormal:
	{
	 
	  std::cout << "======== Grab Normal " << std::endl;
 
	  
	  if(cVisitModifSelect == nullptr )
	    {
	      cVisitModifSelect = new PP3d::VisitorMoveNormal(TheSelect);// DIFFERENCE !!!
	      cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect);
	    }
	  
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des modifs
          cVisitModifSelect->setCoef( TheCreat.currentValTransf()); //((double)cMouseLastPosX-cMouseInitPosX)/30 );// DIFFERENCE !!!//// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!! DIFFERENCE !!!
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect);
	  
	  if( pFlagFinalize)
	    validDragSelect(  );
	  return;    //////////// ATTENTION 
	}
	//================
      
      case Transform::ScaleNormal:
	{
	 
	  std::cout << "======== Grab Normal " << std::endl;
 
	  
	  if(cVisitModifSelect == nullptr )
	    {
	      cVisitModifSelect = new PP3d::VisitorScaleNormal(TheSelect); // DIFFERENCE !!!
	      cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect);
	    }
	  
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des modifs
          cVisitModifSelect->setCoef(TheCreat.currentValTransf()); // ((double)cMouseLastPosX-cMouseInitPosX)/100.0 );// DIFFERENCE !!!// D// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!!IFFERENCE !!!
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect);
	  
	  if( pFlagFinalize)
	    validDragSelect( ); //lMatTran );
	  return;    //(double)/////////// ATTENTI suricata.yamlON 
	}
	//================
	// Chaque facette tourne autour de sa normale
      case Transform::CenterRotFacetNorm:
	{
	 
	  std::cout << "======== Grab Normal " << std::endl;	  
	   
	  if(cVisitModifSelect == nullptr )
	    {
	      cVisitModifSelect = new PP3d::VisitorRotNormal(TheSelect); // DIFFERENCE !!!
	      cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect);
	    }
	  
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des modifs
	  TheCreat.currentTransform().angle().x() += M_PI*iDx/180.;            	
	  cVisitModifSelect->setCoef( TheCreat.currentTransform().angle().x() );
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect);
	  
	  if( pFlagFinalize)
	    validDragSelect();// lMatTran );
	  return;    //(double)/////////// ATTENTlON 
	}
	//================

	
			
      case Transform::MoveX:
	{
	  TheCreat.currentTransform().position().x() += iDx;
	  cMyWin3d.setCurrentVal(  "move x" , TheCreat.currentTransform().position().x());
	  lMatTran.initMove( TheCreat.currentTransform().position().x(), 0, 0 );
	}
	break;
				
      case Transform::MoveY:
	{
	  TheCreat.currentTransform().position().y() += iDx;
	  cMyWin3d.setCurrentVal( "move y" ,  TheCreat.currentTransform().position().y() );
	  lMatTran.initMove( 0, TheCreat.currentTransform().position().y(), 0 );
	}
	break;
				
      case Transform::MoveZ:
	{
	  TheCreat.currentTransform().position().z() += iDx;
	  cMyWin3d.setCurrentVal(  "move z" , TheCreat.currentTransform().position().z() );
	  lMatTran.initMove( 0, 0, TheCreat.currentTransform().position().z() );
	}
	break;

      case Transform::MoveAxis:
	{
	  PP3d::Point3d lPtZero;
	  PP3d::Point3d lAxis;
	  if( TheCreat.getAxis( lPtZero, lAxis ) )	{
	    {
	      TheCreat.currentTransform().position().x() += lAxis.cX*iDx;
	      TheCreat.currentTransform().position().y() += lAxis.cY*iDx;
	      TheCreat.currentTransform().position().z() += lAxis.cZ*iDx;
	      
	      cMyWin3d.setCurrentVal(  "move axis" , TheCreat.currentTransform().position().z() );
	      lMatTran.initMove( TheCreat.currentTransform().position() );
	    }
	  }
	}
	break;
      
	//========== SCALE =========================

      case Transform::ScaleUniform :
      case Transform::ScaleX :
      case Transform::ScaleY :
      case Transform::ScaleZ :
      case Transform::ScaleRX :
      case Transform::ScaleRY :
      case Transform::ScaleRZ :
	{
	  PP3d::Mat4 lMatRecenter;
	  lMatRecenter.initMove( cDragCenter ); //on revient au centre;
	    
	  PP3d::Point3d lNCenter =  -cDragCenter;					
	  PP3d::Mat4 lMatZero;
	  lMatZero.initMove( lNCenter ); //on se positionne en zero;
	    
	  PP3d::Mat4 lMatScale;	  
	  switch(  TheCreat.getCurrentTransformType() )
	    {		
	    case Transform::ScaleUniform :
	      {
		TheCreat.currentTransform().scale().x() += iDx;
		TheCreat.currentTransform().scale().y() += iDx;
		TheCreat.currentTransform().scale().z() += iDx;
		break;
	      }
	    case Transform::ScaleX :
	      TheCreat.currentTransform().scale().x() += iDx;
	      break;
	    case Transform::ScaleY :
	      TheCreat.currentTransform().scale().y() += iDx;
	      break;
	    case Transform::ScaleZ :
	      TheCreat.currentTransform().scale().z() += iDx;
	      break;
	    case Transform::ScaleRX :
	      TheCreat.currentTransform().scale().y() += iDx;
	      TheCreat.currentTransform().scale().z() += iDx;
	      break;
	    case Transform::ScaleRY :
	      TheCreat.currentTransform().scale().x() += iDx;
	      TheCreat.currentTransform().scale().z() += iDx;
	      break;
	    case Transform::ScaleRZ :
	      TheCreat.currentTransform().scale().x() += iDx;
	      TheCreat.currentTransform().scale().y() += iDx;
	      break;
	    case Transform::ScaleAxis :
	      {
		PP3d::Point3d lPtZero;
		PP3d::Point3d lAxis;
		if( TheCreat.getAxis( lPtZero, lAxis ) )
		  {
		    // normalize ?
		  } // A FAIRE
	      }
	      break;
	    default:;
	    }
	     
	  //	  CallDialogKeepFloat( TheCreat.currentTransform().scale.x());
	  lMatScale.initScale( TheCreat.currentTransform().scale() );
	  lMatTran = lMatRecenter * lMatScale *  lMatZero;					
	}
	break;
	//============ ROTATE =======================
      case Transform::CenterRotX :
      case Transform::CenterRotY :
      case Transform::CenterRotZ :
      case Transform::CenterRotNorm :
      case Transform::CenterRotAxis :
      case Transform::RotAxis :
	{		
	  std::cout << "Center:" << cDragCenter  << std::endl;
	  
	  PP3d::Mat4 lMatRecenter;
	  lMatRecenter.initMove( cDragCenter ); //on revient au centre;
					
	  PP3d::Point3d lNCenter =  -cDragCenter;					
	  PP3d::Mat4 lMatZero;
	  lMatZero.initMove( lNCenter ); //on se positionne en zero;

	  
	  PP3d::Mat4 lMatRot;	  
	  switch( TheCreat.getCurrentTransformType() )
	    {
	    case Transform::CenterRotX :
	      TheCreat.currentTransform().angle().x() += M_PI*iDx/180.;
	      CallDialogKeepFloat( TheCreat.currentTransform().angle().x());
	      
	      std::cout << " Angle:" << TheCreat.currentTransform().angle().x()
			<< " : " << (TheCreat.currentTransform().angle().x()*180)/M_PI
			<< std::endl;	  

	      lMatRot.initRotX( TheCreat.currentTransform().angle().x() );
	      break;
							
	    case Transform::CenterRotY :
	      TheCreat.currentTransform().angle().y() += M_PI*iDx/180.;
	      CallDialogKeepFloat( TheCreat.currentTransform().angle().y());
	      
	      std::cout << " Angle:" << TheCreat.currentTransform().angle().y()
			<< " : " << (TheCreat.currentTransform().angle().y()*180)/M_PI
			<< std::endl;	  

	      lMatRot.initRotY( TheCreat.currentTransform().angle().y() );
	      break;
	      
	    case Transform::CenterRotZ :
	      TheCreat.currentTransform().angle().z() += M_PI*iDx/180.;
	      CallDialogKeepFloat( TheCreat.currentTransform().angle().z());
	      
	      std::cout << " Angle:" << TheCreat.currentTransform().angle().z()
			<< " : " << (TheCreat.currentTransform().angle().z()*180)/M_PI
			<< std::endl;	  
	      
	      lMatRot.initRotZ( TheCreat.currentTransform().angle().z() );
	      break;
	      
	    case Transform::CenterRotNorm :
	      {
		if( TheSelect.getNbSelected() >0
		    && (TheSelect.getSelectType() == PP3d::SelectType::Object
			|| TheSelect.getSelectType() ==  PP3d::SelectType::Facet))
		  {
		    PP3d::SortEntityVisitorPointFacet lVisit;		    
		    TheSelect.execVisitorOnEntity(lVisit);
		
		    PP3d::Point3d lCenter;
		    PP3d::Point3d lNorm;
		    
		    if( PP3d::GetVectorFromFacets( lVisit, lNorm, lCenter ))
		      {			
			//	      std::cout << "VERIFICATION VECTEUR NORM:"
			//	      << lAxis.cX*lAxis.cX+lAxis.cY*lAxis.cY+lAxis.cZ*lAxis.cZ
			//	      << std::endl;
			// On prend x mais on pourrait prendre ce que l'on veut
			TheCreat.currentTransform().angle().x() += M_PI*iDx/180.;
			CallDialogKeepFloat( TheCreat.currentTransform().angle().x());
			 
			//			std::cout << " Angle:" << TheCreat.currentTransform().angle().x()
			//				  << " : " << (TheCreat.currentTransform().angle().x()*180)/M_PI
			//	  << std::endl;	 
			lNorm.normalize();
			lMatRot.initRotAxis( lNorm, TheCreat.currentTransform().angle().x() );
		      }
		  }
	      }
	      break;
	      
	    case Transform::CenterRotAxis :
	      {
		PP3d::Point3d lPtZero;
		PP3d::Point3d lAxis;
		if( TheCreat.getAxis( lPtZero, lAxis ))
		  {
		    lAxis -= lPtZero;
		    lAxis.normalize();		      
		      
		    PP3d::SortEntityVisitorPointFacet lVisit;		    
		    TheSelect.execVisitorOnEntity(lVisit);		    
		      
		    TheCreat.currentTransform().angle().x() += M_PI*iDx/180.;
		    CallDialogKeepFloat( TheCreat.currentTransform().angle().x());
		      
		    lMatRot.initRotAxis( lAxis, TheCreat.currentTransform().angle().x() );		      
		  }		      
	      }
	      break;
	      
	    case Transform::RotAxis :
	      {
		PP3d::Point3d lPtZero;
		PP3d::Point3d lAxis;
		if( TheCreat.getAxis( lPtZero, lAxis ))
		  {
		    lAxis -= lPtZero;
		    lAxis.normalize();		      
		      
		    lMatRecenter.initMove( lPtZero ); //on revient au centre;
	    
		    PP3d::Point3d lNCenter =  -lPtZero;					
		    lMatZero.initMove( lNCenter ); //on se positionne en zero;
	    
		    PP3d::SortEntityVisitorPointFacet lVisit;		    
		    TheSelect.execVisitorOnEntity(lVisit);		    
		      
		    TheCreat.currentTransform().angle().x() += M_PI*iDx/180.;
                    //	    CallDialogKeepFloat( TheCreat.currentTransform().angle().x());
		      
		    lMatRot.initRotAxis( lAxis, TheCreat.currentTransform().angle().x() );		      
		  }		      
	      }
	      break;

	    default:;
	    }					
					
	  lMatTran = lMatRecenter * lMatRot *  lMatZero;					
	}
	break;
	//===================================

      default:
	std::cout << "Nothing to do 2!!!" << std::endl;
      }

    //    std::cout << "pos:" << TheCreat.currentTransform().position() << std::endl;


    if( pFlagFinalize == false )
      {
	//				PP3d::Selection::Instance().setCurrentMatrice( &lMatTran );
	dragSelect( lMatTran );
      }
    else
      {
	cout << "============= VALIDATE ===================" << endl;
	//			cout << lMatTran << endl;
	validDragSelect( lMatTran );
	cout << "============= VALIDATE ===================" << endl;
				
	//				PP3d::Selection::Instance().execModificationMatrice( lMatTran );
      }
  }

    
  //---------------------------
  int Canvas3d::handleTransform(  int pEvent )
  {
    if( getGlobalMode() != GlobalMode::INPUT)
      {
        return 0;
      }
           
       
    if( pEvent == FL_PUSH )
      {        
	if( Fl::event_button() == FL_LEFT_MOUSE &&  getUserMode() == ModeUser::MODE_TRANSFORM )
	  {
	    userActionPrepare( );
	    return 1;
	  }
      }

    if( pEvent ==FL_RELEASE )
      {
        if(  getUserMode() == ModeUser::MODE_TRANSFORM
             && TheCreat.getCurrentTransformType() != Transform::Nothing )
          {
	    userTransformSelection(pEvent, true );
	    userActionTerminate(  );
	    TheCreat.redrawAllCanvas3d( PP3d::Compute::FacetAll);            
	    return 1;
          }
      }

    //    if( pEvent ==FL_DRAG)
      {
        if(  getUserMode() == ModeUser::MODE_TRANSFORM )
          {
            if( userActionIsRun() ) 
              {
		userTransformSelection(pEvent);
                //    setCursor3dPosition( Fl::event_x(), Fl::event_y());
                TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing); // a cause du curseur ou break;
                return 1;
              }
          }
      }
    return 0;
  }    
       
} //namespace 
