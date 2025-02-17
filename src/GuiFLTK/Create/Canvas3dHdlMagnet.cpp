#include "Shape/SortVisitor.h"
#include "Shape/PP3dUtils.h"
#include "Shape/MinMaxBox3d.h"
#include "Shape/Line3dVect.h"

#include "Creation.h"
#include "GuiFLTK/MyFlWidget.h"

#include "Dialogs.h"
#include "Win3d.h"

#include "GuiFLTK/Preference.h"
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
        // CTRL LEFT => BEGIN MAGNETISE
        if( pEvent == FL_PUSH &&  Fl::event_button() == FL_LEFT_MOUSE  && Fl::event_ctrl() )
          {
            userActionPrepare();
            getMagnet().setMemSize( getMagnet().getSize() );
            
            changeUserMode( ModeUser::MODE_TRANSFORM );
            Creation::Instance().setCurrentTransformType(Transform::MagnetMove);

            PP3d::EntityPtr lLastHightLast = getMagnet().getLastHightLigth();
            if( lLastHightLast == nullptr )
              {
                cout << "BEGIN TRANSFORM  failed : no hightlight" << endl;
                return 1;
              }
            
            PP3d::Point3d  lPos = lLastHightLast->getCenter3d()+lLastHightLast->getNormal3d();;   
            std::cout << "*** MAGNET FIRST POS:" << lPos << std::endl;

            TheCreat.getDatabase()->setCursorPosition(lPos );
            TheInput.viewCurrentPoint( lPos );

            getMagnet().setPosition( lPos );

            if( getMagnet().magnetise( TheBase, TheSelect ) == false )
              {
                cout << "    handleMagnet - begin transf - magnetise return false" << endl;
                return 1;
              }
            
            getMagnet().prepareDrawMagnet();
            TheCreat.redrawAllCanvas3d( PP3d::Compute::Nothing);
            return 1;                                
          }
      }
    //========================================
    if( getUserMode() == ModeUser::MODE_TRANSFORM )
      {
        // RELEASE LEFT => TERMINATE MAGNETISE
        if( pEvent == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE )
          {
            if( TheCreat.getCurrentTransformType() != Transform::Nothing )
              {
                userTransformSelection(pEvent, true );
                stopMagnetise();
                return 1;
              }
          }
        // DRAG MAGNETISE 
        if( pEvent == FL_DRAG  )
          {
            if( cMouseLastPosX != -1 )
              {
                if( Fl::event_shift() ) //CHANGE OF SIZE 
                  {
                    cMouseLastPosX = Fl::event_x();
                    cMouseLastPosY = Fl::event_y();
                    double lSz = (((double)cMouseLastPosX)-((double)cMouseInitPosX))/100.;
            
                    lSz += getMagnet().getMemSize();         
                        
                    if( lSz < 0 )
                      lSz = - lSz;
                
                    // A CHANGER ---- TENIR COMPTE DE L'ECHELLE !!!!!!!!!!!!!!!!!!!!            

                    if( lSz < 0.01 ) lSz = 0.01;
                    if( lSz > 50 ) lSz = 50;
                    
                    std::cout << "lSz:" << lSz ;

                    getMagnet().setSize( lSz );                    
                    getMagnet().magnetise( TheBase, TheSelect );                    
                    getMagnet().prepareDrawMagnet();                    
                    MajDialogMagnet();                    
                    TheCreat.redrawAllCanvas3d( PP3d::Compute::Nothing);
                    return 1;
                  }
            
                else { // TRANSFORMATION OF MAGNETISED ENTITY

                  //        setCursor3dPosition( Fl::event_x(), Fl::event_y());                  
                 PP3d::EntityPtr lLastHightLast = getMagnet().getLastHightLigth();
                  if( lLastHightLast == nullptr )
                    {
                      cout << "BEGIN TRANSFORM  failed : no hightlight" << endl;
                      return 1;
                    }                  
                  getMagnet().magnetise( TheBase, TheSelect );
                  
                  PP3d::Point3d lPos = userInputPoint( Fl::event_x(), Fl::event_y(), false );
                  getMagnet().setTransformPosition( lPos ); 
                 
                  TheInput.swapCurrentCreation( nullptr  );
                  //   getMagnet().prepareDrawMagnet();
                  
                  userTransformSelection(pEvent);    // Modification via visiteur ...
                  
                  TheCreat.redrawAllCanvas3d(PP3d::Compute::Nothing); 
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
                    stopMagnetise();
                    return 1;
                  }
              }
          }                        
      }
    //========================================       
  
    return 0;
  }
  //----------------------------------
  void Canvas3d::stopMagnetise()
  {
    getMagnet().unMagnetise();
    getMagnet().setMemSize( getMagnet().getSize() );
    TheInput.swapCurrentCreation( nullptr  );
    userActionTerminate();
    TheCreat.setCurrentTransformType( Transform::Nothing );
    TheCreat.redrawAllCanvas3d( PP3d::Compute::FacetAll);
  }

  //***************************************


} //namespace 
