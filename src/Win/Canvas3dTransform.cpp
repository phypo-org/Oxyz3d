
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

#include "Application.h"
#include "MyFlWidget.h"

#include "Dialogs.h"
#include "Win3d.h"

#include "Preference.h"
#include "Callback.h"


#include "SelFunct.h"


using namespace std;



namespace M3d {

  //---------------------------
  void Canvas3d::userTransformSelection(int pEvent, bool pFlagFinalize)
  {
    
    int lX = Fl::event_x();
    int lY = Fl::event_y();
 
    float lDx = (float)(lX - cMouseLastPosX);
    
    float lDy = (float)(cMouseLastPosY - lY); // NOT USE !!!!
    cMouseLastPosX = lX;
    cMouseLastPosY = lY;
    
    lDx /= 10.;   // MODIFIER EN FONCTION DE LA TRANSFORMATION !!!
    userTransformSelectionInternal( lDx,  pFlagFinalize);
  }

  //---------------------------
  void Canvas3d::userTransformSelectionInput( int pEvent )
  { 
    Transform lCurrent= TheAppli.getCurrentTransformType();

    std::cout << "currentValTransf:" << TheAppli.currentValTransf() << std::endl;
    
    if( CallDialogInputDouble( "Value", TheAppli.currentValTransf(), [&]( double ioVal, bool  ioRun ) {

      //    std::cout << " F val:" << ioVal << " " << ioRun << std::endl;
      TheAppli.setCurrentTransformType( lCurrent );
      userTransformSelectionInternal( ioVal, !ioRun);
      TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing); 

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
  void Canvas3d::userTransformSelectionInternal( float lDx, bool pFlagFinalize)
  {      
    std::cout << "***** Canvas3d::userTransformSelectionInternal **** " << lDx << std::endl;
    
    TheAppli.currentValTransf() = TheAppli.currentValTransf() + lDx;
 
    
    PP3d::Mat4 lMatTran;
    lMatTran.identity();

    if( TheAppli.getCurrentDeformType() != TypeDeform::Nothing )
      {
	/*
	  std::cout << "======== Grab Deform " << std::endl;
 
	  
	  if(cVisitModifSelect == nullptr )
          {
          cVisitModifSelect = new PP3d::VisitorDeform(TheSelect, TheAppli.getCurrentDeformType(), TheAppli.getCurrentTransformType()  );// DIFFERENCE !!!
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
    
  
    switch( TheAppli.getCurrentTransformType() )
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
          //          << TheAppli.currentValTransf()
          //         << std::endl;
          
          if(cVisitModifSelect == nullptr )  // la premiere fois !
	    {
	      cVisitModifSelect = new VisitorMagnet(TheSelect, getMagnet());
              cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::SAV, TheSelect); // sauvegarde des points originaux !
 	    }
          
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::CANCEL, TheSelect); // remise a zero des anciennes modifs
          cVisitModifSelect->setCoef( TheAppli.currentValTransf() );  // prise en compte de la souris !!!
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
          cVisitModifSelect->setCoef( TheAppli.currentValTransf()); //((double)cMouseLastPosX-cMouseInitPosX)/30 );// DIFFERENCE !!!//// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!! DIFFERENCE !!!
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
          cVisitModifSelect->setCoef(TheAppli.currentValTransf()); // ((double)cMouseLastPosX-cMouseInitPosX)/100.0 );// DIFFERENCE !!!// D// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!!// DIFFERENCE !!!IFFERENCE !!!
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
	  TheAppli.currentTransform().angle().x() += M_PI*lDx/180.;            	
	  cVisitModifSelect->setCoef( TheAppli.currentTransform().angle().x() );
	  cVisitModifSelect->modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, TheSelect);
	  
	  if( pFlagFinalize)
	    validDragSelect();// lMatTran );
	  return;    //(double)/////////// ATTENTlON 
	}
	//================

	
			
      case Transform::MoveX:
	{
	  TheAppli.currentTransform().position().x() += lDx;
	  cMyWin3d.setCurrentVal(  "move x" , TheAppli.currentTransform().position().x());
	  lMatTran.initMove( TheAppli.currentTransform().position().x(), 0, 0 );
	}
	break;
				
      case Transform::MoveY:
	{
	  TheAppli.currentTransform().position().y() += lDx;
	  cMyWin3d.setCurrentVal( "move y" ,  TheAppli.currentTransform().position().y() );
	  lMatTran.initMove( 0, TheAppli.currentTransform().position().y(), 0 );
	}
	break;
				
      case Transform::MoveZ:
	{
	  TheAppli.currentTransform().position().z() += lDx;
	  cMyWin3d.setCurrentVal(  "move z" , TheAppli.currentTransform().position().z() );
	  lMatTran.initMove( 0, 0, TheAppli.currentTransform().position().z() );
	}
	break;

      case Transform::MoveAxis:
	{
	  PP3d::Point3d lPtZero;
	  PP3d::Point3d lAxis;
	  if( TheAppli.getAxis( lPtZero, lAxis ) )	{
	    {
	      TheAppli.currentTransform().position().x() += lAxis.cX*lDx;
	      TheAppli.currentTransform().position().y() += lAxis.cY*lDx;
	      TheAppli.currentTransform().position().z() += lAxis.cZ*lDx;
	      
	      cMyWin3d.setCurrentVal(  "move axis" , TheAppli.currentTransform().position().z() );
	      lMatTran.initMove( TheAppli.currentTransform().position() );
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
	  switch(  TheAppli.getCurrentTransformType() )
	    {		
	    case Transform::ScaleUniform :
	      {
		TheAppli.currentTransform().scale().x() += lDx;
		TheAppli.currentTransform().scale().y() += lDx;
		TheAppli.currentTransform().scale().z() += lDx;
		break;
	      }
	    case Transform::ScaleX :
	      TheAppli.currentTransform().scale().x() += lDx;
	      break;
	    case Transform::ScaleY :
	      TheAppli.currentTransform().scale().y() += lDx;
	      break;
	    case Transform::ScaleZ :
	      TheAppli.currentTransform().scale().z() += lDx;
	      break;
	    case Transform::ScaleRX :
	      TheAppli.currentTransform().scale().y() += lDx;
	      TheAppli.currentTransform().scale().z() += lDx;
	      break;
	    case Transform::ScaleRY :
	      TheAppli.currentTransform().scale().x() += lDx;
	      TheAppli.currentTransform().scale().z() += lDx;
	      break;
	    case Transform::ScaleRZ :
	      TheAppli.currentTransform().scale().x() += lDx;
	      TheAppli.currentTransform().scale().y() += lDx;
	      break;
	    case Transform::ScaleAxis :
	      {
		PP3d::Point3d lPtZero;
		PP3d::Point3d lAxis;
		if( TheAppli.getAxis( lPtZero, lAxis ) )
		  {
		    // normalize ?
		  } // A FAIRE
	      }
	      break;
	    default:;
	    }
	     
	  //	  CallDialogKeepFloat( TheAppli.currentTransform().scale.x());
	  lMatScale.initScale( TheAppli.currentTransform().scale() );
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
	  switch( TheAppli.getCurrentTransformType() )
	    {
	    case Transform::CenterRotX :
	      TheAppli.currentTransform().angle().x() += M_PI*lDx/180.;
	      CallDialogKeepFloat( TheAppli.currentTransform().angle().x());
	      
	      std::cout << " Angle:" << TheAppli.currentTransform().angle().x()
			<< " : " << (TheAppli.currentTransform().angle().x()*180)/M_PI
			<< std::endl;	  

	      lMatRot.initRotX( TheAppli.currentTransform().angle().x() );
	      break;
							
	    case Transform::CenterRotY :
	      TheAppli.currentTransform().angle().y() += M_PI*lDx/180.;
	      CallDialogKeepFloat( TheAppli.currentTransform().angle().y());
	      
	      std::cout << " Angle:" << TheAppli.currentTransform().angle().y()
			<< " : " << (TheAppli.currentTransform().angle().y()*180)/M_PI
			<< std::endl;	  

	      lMatRot.initRotY( TheAppli.currentTransform().angle().y() );
	      break;
	      
	    case Transform::CenterRotZ :
	      TheAppli.currentTransform().angle().z() += M_PI*lDx/180.;
	      CallDialogKeepFloat( TheAppli.currentTransform().angle().z());
	      
	      std::cout << " Angle:" << TheAppli.currentTransform().angle().z()
			<< " : " << (TheAppli.currentTransform().angle().z()*180)/M_PI
			<< std::endl;	  
	      
	      lMatRot.initRotZ( TheAppli.currentTransform().angle().z() );
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
			TheAppli.currentTransform().angle().x() += M_PI*lDx/180.;
			CallDialogKeepFloat( TheAppli.currentTransform().angle().x());
			 
			//			std::cout << " Angle:" << TheAppli.currentTransform().angle().x()
			//				  << " : " << (TheAppli.currentTransform().angle().x()*180)/M_PI
			//	  << std::endl;	 
			lNorm.normalize();
			lMatRot.initRotAxis( lNorm, TheAppli.currentTransform().angle().x() );
		      }
		  }
	      }
	      break;
	      
	    case Transform::CenterRotAxis :
	      {
		PP3d::Point3d lPtZero;
		PP3d::Point3d lAxis;
		if( TheAppli.getAxis( lPtZero, lAxis ))
		  {
		    lAxis -= lPtZero;
		    lAxis.normalize();		      
		      
		    PP3d::SortEntityVisitorPointFacet lVisit;		    
		    TheSelect.execVisitorOnEntity(lVisit);		    
		      
		    TheAppli.currentTransform().angle().x() += M_PI*lDx/180.;
		    CallDialogKeepFloat( TheAppli.currentTransform().angle().x());
		      
		    lMatRot.initRotAxis( lAxis, TheAppli.currentTransform().angle().x() );		      
		  }		      
	      }
	      break;
	      
	    case Transform::RotAxis :
	      {
		PP3d::Point3d lPtZero;
		PP3d::Point3d lAxis;
		if( TheAppli.getAxis( lPtZero, lAxis ))
		  {
		    lAxis -= lPtZero;
		    lAxis.normalize();		      
		      
		    lMatRecenter.initMove( lPtZero ); //on revient au centre;
	    
		    PP3d::Point3d lNCenter =  -lPtZero;					
		    lMatZero.initMove( lNCenter ); //on se positionne en zero;
	    
		    PP3d::SortEntityVisitorPointFacet lVisit;		    
		    TheSelect.execVisitorOnEntity(lVisit);		    
		      
		    TheAppli.currentTransform().angle().x() += M_PI*lDx/180.;
                    //	    CallDialogKeepFloat( TheAppli.currentTransform().angle().x());
		      
		    lMatRot.initRotAxis( lAxis, TheAppli.currentTransform().angle().x() );		      
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

    //    std::cout << "pos:" << TheAppli.currentTransform().position() << std::endl;


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
             && TheAppli.getCurrentTransformType() != Transform::Nothing )
          {
	    userTransformSelection(pEvent, true );
	    userActionTerminate(  );
	    TheAppli.redrawAllCanvas3d( PP3d::Compute::FacetAll);            
	    return 1;
          }
      }

    if( pEvent ==FL_DRAG)
      {
        if(  getUserMode() == ModeUser::MODE_TRANSFORM )
          {
            if( userActionIsRun() ) 
              {
		userTransformSelection(pEvent);
                setCursor3dPosition( Fl::event_x(), Fl::event_y());
                TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing); // a cause du curseur ou break;
                return 1;
              }
          }
      }
    return 0;
  }    
       
} //namespace 
