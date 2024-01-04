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
            userActionPrepare( pEvent );                   
            getMagnet().drag(*this);
            PP3d::Point3d lTmp(cMouseInitPosX, pixel_h()-cMouseInitPosY,0);
            //   TheAppli.cMagnet.setPosition2D( lTmp );
            //TheAppli.cMagnet.prepareMagnetDraw();
                
            changeUserMode( ModeUser::MODE_DRAG );

            getMagnet().prepareDrawMagnet();

            setCursor3dPosition( Fl::event_x(), Fl::event_y());
            TheAppli.redrawAllCanvas3d( PP3d::Compute::Nothing);                
            return 1;
          }

        // CTRL LEFT => BEGIN TRANSFORM par le Magnet
        if( pEvent == FL_PUSH &&  Fl::event_button() == FL_LEFT_MOUSE  && Fl::event_ctrl() )
          {
            userActionPrepare( pEvent );                   
            PP3d::Point3d lTmp(cMouseInitPosX, pixel_h()-cMouseInitPosY,0);
            // TheAppli.cMagnet.setPosition2D( lTmp );

            getMagnet().prepareDrawMagnet();

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
            userActionPrepare( pEvent );
            
                //        setCursor3dPosition( Fl::event_x(), Fl::event_y());
             
            PP3d::Point3d lPos = userInputPoint( Fl::event_x(), Fl::event_y() , false ); // just view the possible position of point

            std::cout << "*** MAGNET FIRST POS:" << lPos << std::endl;
            getMagnet().setPosition( lPos );
            getMagnet().prepareDrawMagnet();

            TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing); // a cause du curseur ou break
            return 1;
          }
        // RELEASE LEFT => Terminate action
        if( pEvent == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE )
          {
            if( TheAppli.getCurrentTransformType() != Transform::Nothing )
              {
                TheInput.swapCurrentCreation( nullptr  );
                userTransformSelection(pEvent, true );                   
                userActionTerminate( pEvent );
                TheAppli.redrawAllCanvas3d( PP3d::Compute::FacetAll);
                return 1;
              }
          }
        // DRAG TRANSFORM 
        if( pEvent == FL_DRAG  )
          {
            if( cMouseLastPosX != -1 )
              {
              //        setCursor3dPosition( Fl::event_x(), Fl::event_y());
                 PP3d::Point3d lPos = userInputPoint( Fl::event_x(), Fl::event_y() , false );                 std::cout << "MAGNET POS:" << lPos << std::endl;
 // just view the possible position of point
                 getMagnet().setTransformPosition( lPos );

                //        PP3d::Point3d lTmp(  Fl::event_x(), Fl::event_y(), 0 );
                //                TheAppli.cMagnet.setTransformPosition2D( lTmp );

                 getMagnet().prepareDrawMagnet();
                 userTransformSelection(pEvent);
                   
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
                    TheInput.swapCurrentCreation( nullptr  );

                    userActionTerminate( pEvent );
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
            getMagnet().releaseMagnet();
            
    TheInput.swapCurrentCreation( nullptr  );

            
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
            double lSz = (((double)cMouseLastPosX)-((double)cMouseInitPosX))/100.;
            
            std::cout << "lSz:" << lSz;

            if( lSz < 0 )
              lSz = -lSz;
            
           // A CHANGER            

            if( lSz < 0.01 ) lSz = 0.01;
            if( lSz > 50 ) lSz = 50;
            
            std::cout << " lSz2:" << lSz<< std::endl; 
  
       
            getMagnet().setSize( lSz ); 
            
            getMagnet().prepareDrawMagnet();

            TheAppli.redrawAllCanvas3d( PP3d::Compute::Nothing);
            return 1;
          }
      }
    //========================================       
  
    return 0;
  }
	


  //***************************************


} //namespace 
