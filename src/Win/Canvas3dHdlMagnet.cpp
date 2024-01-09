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
        // CTRL LEFT => BEGIN TRANSFORM par le Magnet
        if( pEvent == FL_PUSH &&  Fl::event_button() == FL_LEFT_MOUSE  && Fl::event_ctrl() )
          {
            userActionPrepare( pEvent );
            getMagnet().setMemSize( getMagnet().getSize() );

            
            changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::MagnetMove);

            //    PP3d::Point3d lPos = userInputPoint( Fl::event_x(), Fl::event_y() , false ); // just view the possible position of point

            //            setCursor3dPosition( Fl::event_x(), Fl::event_y());
            //            PP3d::Point3d lPos = TheAppli.getDatabase()->getCursorPosition();
            PP3d::EntityPtr lLastHightLast = getMagnet().getLastHightLigth();
            if( lLastHightLast == nullptr )
              {
                cout << "BEGIN TRANSFORM  failed : no hightlight" << endl;
                return 1;
              }
            
            PP3d::Point3d  lPos = lLastHightLast->getCenter3d()+lLastHightLast->getNormal3d();;   
            std::cout << "*** MAGNET FIRST POS:" << lPos << std::endl;

            TheAppli.getDatabase()->setCursorPosition(lPos );
            TheInput.viewCurrentPoint( lPos );

            getMagnet().setPosition( lPos );

            if( getMagnet().magnetise( TheBase, TheSelect ) == false )
              {
                cout << "    handleMagnet - begin transf - magnetise return false" << endl;
                return 1;
              }
            
            getMagnet().prepareDrawMagnet();


            //     setCursor3dPosition( Fl::event_x(), Fl::event_y());
            TheAppli.redrawAllCanvas3d( PP3d::Compute::Nothing);
            return 1;                                
          }
      }

    //========================================
    if( getUserMode() == ModeUser::MODE_TRANSFORM )
      {
        // RELEASE LEFT => Terminate action
        if( pEvent == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE )
          {
            if( TheAppli.getCurrentTransformType() != Transform::Nothing )
              {
                getMagnet().unMagnetise();
                getMagnet().setMemSize( getMagnet().getSize() );

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
                if( Fl::event_shift() ) //CHANGEMENT DE TAILLE
                  {
                    cMouseLastPosX = Fl::event_x();
                    cMouseLastPosY = Fl::event_y();
                    double lSz = (((double)cMouseLastPosX)-((double)cMouseInitPosX))/100.;
            
                    lSz += getMagnet().getMemSize();         
            
                    std::cout << "lSz:" << lSz ;
            
                    if( lSz < 0 )
                      lSz = - lSz;
                
                    // A CHANGER            

                    if( lSz < 0.01 ) lSz = 0.01;
                    if( lSz > 50 ) lSz = 50;
            
                    std::cout << " lSz2:" << lSz<< std::endl; 
                      
                    getMagnet().setSize( lSz );
                    
                    getMagnet().magnetise( TheBase, TheSelect );
                    
                    getMagnet().prepareDrawMagnet();
                    
                    MajDialogMagnet();
                    
                    TheAppli.redrawAllCanvas3d( PP3d::Compute::Nothing);
                    return 1;
                  }
            
                else {       // TRANSFORMATION
                  
                  cout << " magnet FL_DRAG TRANSFORM" << endl;
                  //        setCursor3dPosition( Fl::event_x(), Fl::event_y());
                  PP3d::Point3d lPos = userInputPoint( Fl::event_x(), Fl::event_y() , false );
                  std::cout << "MAGNET POS:" << lPos << std::endl;
                  // just view the possible position of point
                  getMagnet().setTransformPosition( lPos );
                  getMagnet().magnetise( TheBase, TheSelect );
                  
                  
                  // TheInput.swapCurrentCreation( nullptr  );
                  getMagnet().prepareDrawMagnet();
                  
                  userTransformSelection(pEvent);
                  
                  TheAppli.redrawAllCanvas3d(PP3d::Compute::Nothing); // a cause du curseur ou break
                  return 1;                                   
                }
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
  
    return 0;
  }


  //***************************************


} //namespace 
