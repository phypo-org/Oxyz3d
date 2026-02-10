#ifndef H__CurrentInput__H
#define H__CurrentInput__H

#include <vector>
#include <stack>



#include "Object.h"

#include "PP3dType.h"


#include "Entity.h"

#include "UniqueId.h"
#include "Object.h"

#include "SubDiv.h"
#include "Spline/BSpline.h"


namespace PP3d {
  //************************************
  class CurrentInput {

    ObjectPtr              cCurrentCreation=nullptr;
    
    //   ObjectPtr              cCurrentLine=nullptr;
    Polyline               cCurrentPolyline;
    
    static const EntityId  kMinCurrentLineId = 0xF00000; 
    EntityId               cCurrentLineId = kMinCurrentLineId;  
    int                    cCurrentLineSelectPoint = -1;  
  public :
    //-------------------------------------
    static bool IsCurrentLineId( EntityId iId ) { return iId >= kMinCurrentLineId ; }
    //-------------------------------------
     bool testAndSelectCurrentLinePoint( EntityId iId )
    {
      if( IsCurrentLineId(iId) == false )
      {
	cCurrentLineSelectPoint = -1;
	return false;
      }
      cCurrentLineSelectPoint = iId-kMinCurrentLineId;
      //   std::cout << "testAndSelectCurrentLinePoint -> " << cCurrentLineSelectPoint << std::endl;
      return true;
    }
    //-------------------------------------
    void resetCurrentLine()
    {
      cCurrentPolyline.deleteAll();        
      cCurrentLineId = kMinCurrentLineId;
      cCurrentLineSelectPoint = -1;
      cCurrentPoint = nullptr;
      cCurrentPointObject = nullptr;
    }
     //-------------------------------------
    void  renumberPoints();   
    //-------------------------------------
    int getCurrentLineSelectPoint() { return cCurrentLineSelectPoint; }    
    //-------------------------------------
    void editCurrentLineSelectPoint()
    {
      
      PointPtr lPt = cCurrentPolyline.getPoint(cCurrentLineSelectPoint);	  
      if( lPt != nullptr )
        {
          viewCurrentPoint( lPt->get());
        }
    }
  
    //-------------------------------------
    void insertCurrentLineSelectPoint( Point3d iPt, DataBase & iBase)
    {            
       cCurrentPolyline.insertPoint( cCurrentLineSelectPoint, iPt,  iBase );     
      renumberPoints();
    }
    //-------------------------------------
    void delCurrentLineSelectPoint(DataBase & iBase)
    {            
      if( cCurrentPolyline.delPoint( cCurrentLineSelectPoint, iBase ) )
	{
	  resetCurrentLine();
	}
      else
	{
	  renumberPoints();
	}      
    }
    //-------------------------------------
    bool changeCurrentLineSelectPoint(Point3d pPt)
    {
      PointPtr lPt = cCurrentPolyline.getPoint(cCurrentLineSelectPoint);	  
      if( lPt != nullptr )
        {
          std::cout << "changeCurrentLineSelectPoint : Point  found" << std::endl;
          lPt->set(pPt);  ;
          cCurrentLineSelectPoint = -1;
          return true;
        }
      std::cout << "changeCurrentLineSelectPoint : Point not found" << std::endl;
	  
      return false;
    }
    //-------------------------------------
				 
  private:


    ObjectPtr     cCurrentPoint = nullptr;
    ObjectPtr     cCurrentPointObject = nullptr;


  public:
    CurrentInput();
    ~CurrentInput();


    Object*   getCurrentCreation();
    Object*   validCurrentCreation( DataBase & iBase);
    void      cancelCurrentCreation( );
    void      swapCurrentCreation( Object* pCurrentCreation, bool iFlagDel = true );
    void      addToInput( EntityPtr, bool pFlagLink );


		
    void      viewCurrentPoint( Point3d & pPt );
    void      hideCurrentPoint();
    void      addPointToCurrentLine( Point3d pPt );
    void      delLastPoint();
    void      delAllPoint();
    GLuint    getNbCurrentPoints();
    ObjectPtr convertCurrentLineToFacet(DataBase & iBase);
    ObjectPtr convertCurrentLineToFacetPoly(DataBase & iBase);
    ObjectPtr convertCurrentLineToBiFacetPoly(DataBase & iBase);
    ObjectPtr convertCurrentLineToPolyline(DataBase & iBase);
    ObjectPtr convertCurrentLineToLine(DataBase & iBase);
    ObjectPtr convertCurrentLineToBSpline(DataBase & iBase, size_t iMaille, bool iClosed);
    
    bool isCurrentPoints()   { return cCurrentPolyline.size() != 0; }
    bool isCurrentCreation() { return cCurrentCreation != nullptr; }
    Polyline & getCurrentLine()
    {
      return  cCurrentPolyline;
    }
    size_t  currentLineSize()
    {
      return cCurrentPolyline.size() ;
    }

    bool execVisitorOnCurrentLine( EntityVisitor & iVisit ){
      if( cCurrentPolyline.size()==0) return false;
      cCurrentPolyline.execVisitor( iVisit );
      return true;
    }
     
    void drawGL(ViewProps& iViewGen , ViewProps& iViewInputCursor, ViewProps& iViewInputPoly, ViewProps& iViewInputObject,  GLMode iSelectOrDrawMode, SelectType iSelectType);
  };
  
  //************************************
}



#endif
