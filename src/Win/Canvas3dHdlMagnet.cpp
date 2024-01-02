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
  int Canvas3d::handleMagnet(  int pEvent )
  {
    if( getGlobalMode() != GlobalMode::MAGNET)
      return 0;
      
    //========================================
    if( getUserMode() == ModeUser::MODE_BASE )
      {
        // SHIFT LEFT => BEGIN DRAG diametre du magnet //
        if( pEvent == FL_PUSH &&  Fl::event_button() == FL_LEFT_MOUSE  && Fl::event_shift() )
          {
            userPrepareAction( pEvent );                   
            cMagnet.drag(*this);
            PP3d::Point3d lTmp(cMouseInitPosX, pixel_h()-cMouseInitPosY,0);
            cMagnet.setPosition( lTmp );
            //cMagnet.prepareMagnetDraw();
                
            changeUserMode( ModeUser::MODE_DRAG );
                
            setCursor3dPosition( Fl::event_x(), Fl::event_y());
            TheAppli.redrawAllCanvas3d( PP3d::Compute::Nothing);                
            return 1;
          }

        // CTRL LEFT => BEGIN TRANSFORM par le Magnet
        if( pEvent == FL_PUSH &&  Fl::event_button() == FL_LEFT_MOUSE  && Fl::event_ctrl() )
          {
            userPrepareAction( pEvent );                   
            PP3d::Point3d lTmp(cMouseInitPosX, pixel_h()-cMouseInitPosY,0);
            cMagnet.setPosition( lTmp );
            changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::MagnetMove);

            setCursor3dPosition( Fl::event_x(), Fl::event_y());
            TheAppli.redrawAllCanvas3d( PP3d::Compute::Nothing);
            return 1;                                
          }
      }

    //========================================
    if( getUserMode() == ModeUser::MODE_TRANSFORM )
      {
        // LEFT => BEGIN TRANSFORM par le Magnet
        if( pEvent == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE )
          {
            PP3d::Point3d lTmp(cMouseInitPosX, pixel_h()-cMouseInitPosY,0);
            cMagnet.setTransformPosition( lTmp );
            userPrepareAction( pEvent );
            userTerminateAction( pEvent );
            setCursor3dPosition( Fl::event_x(), Fl::event_y());
            userInputPoint( Fl::event_x(), Fl::event_y() , false ); // just view the possible position of point
            return 1;
          }
        // RELEASE LEFT => Terminate action
        if( pEvent == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE )
          {
            if( TheAppli.getCurrentTransformType() != Transform::Nothing )
              {
                userTransformSelection(pEvent, true );
                   
                userTerminateAction( pEvent );
                TheAppli.redrawAllCanvas3d( PP3d::Compute::FacetAll);
                return 1;
              }
          }
        // DRAG TRANSFORM 
        if( pEvent == FL_DRAG  )
          {
            if( cMouseLastPosX != -1 )
              {
                userTransformSelection(pEvent);
                PP3d::Point3d lTmp(cMouseInitPosX, pixel_h()-cMouseInitPosY,0);
                //   PP3d::Point3d lTmp(cMouseLastPosX, pixel_h()-cMouseLastPosY,0);
                PP3d::Point3d lTmp2(cMouseLastPosX, pixel_h()-cMouseLastPosY,0);
                cMagnet.setTransformPosition( lTmp2 );
                   
                userInputPoint(  Fl::event_x(), Fl::event_y() , false ); // just view the possible position of point
                setCursor3dPosition( Fl::event_x(), Fl::event_y());
                TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing); // a cause du curseur ou break
                return 1;                                   
              }
          }
        if( pEvent == FL_KEYDOWN )
          {
            if(  Fl::event_key() >= 256 &&  Fl::event_key() == FL_Tab )
              {
                userTransformSelectionInput(pEvent);
                return 1;
              }
            else
              {
                const char *lStr = Fl::event_text(); 
                cout << " txt<" << lStr << "> " ;
                   
                if( strcmp( lStr, ANNULE_ACTION )==0)
                  {
                    userTerminateAction( pEvent );
                    TheAppli.setCurrentTransformType( Transform::Nothing );
                    return 1;
                  }
              }
          }                      
      }
    //========================================
    if( getUserMode() == ModeUser::MODE_DRAG )
      {
        // RELEASE LEFT => STOP DRAG diametre du magnet 
        if( pEvent == FL_RELEASE &&  Fl::event_button() == FL_LEFT_MOUSE )
          {
            DBG_ACT(" **************** MAGNET RELEASE  " );
            std::cout << " ****************  MAGNET RELEASE " << std::endl;
            cMagnet.releaseMagnet();
            changeUserMode( ModeUser::MODE_BASE );
            TheAppli.redrawAllCanvas3d( PP3d::Compute::FacetAll);
            return 1;
          }

        // DRAG => RESIZE MAGNET 
        if( pEvent == FL_DRAG  )
          {
            cout << "Canvas3d::userDrag Magnet " << endl;
                
            cMouseLastPosX = Fl::event_x();
            cMouseLastPosY = Fl::event_y();
                
            int lSz = (cMouseLastPosX-cMouseInitPosX)/3;
            if( lSz < 0 )
              {
                lSz = - std::sqrt(-lSz);
              }
            else
              lSz = std::sqrt(lSz);
                
            lSz +=  cMagnet.getSize();
            if( lSz > 1000 ) lSz = 1000;
            if( lSz < 5 )    lSz = 5;
                
            cMagnet.setSize( lSz );
            TheAppli.redrawAllCanvas3d( PP3d::Compute::FacetAll);
            return 1;
          }
      }
    //========================================       
  
    return 0;
  }
	


  //***************************************


} //namespace 
