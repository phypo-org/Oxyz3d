#ifndef H__CurrentInput__H
#define H__CurrentInput__H


#include <vector>
#include <stack>



#include "Object.h"

#include "PP3dType.h"


#include "Entity.h"

#include "UniqueId.h"
#include "Object.h"
#include "ObjectPolylines.h"
#include "ObjectLine.h"

#include "SubDiv.h"
#include "Spline/BSpline.h"


namespace PP3d {
  //************************************
  class CurrentInput {

    Object               * cCurrentCreation=nullptr;
    
    ObjectPolylines      * cCurrentLine=nullptr;
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
      std::cout << "testAndSelectCurrentLinePoint -> " << cCurrentLineSelectPoint << std::endl;
      return true;
    }
    //-------------------------------------
    void resetCurrentLine()
    {
      // faire des delete  ?
      cCurrentLine=nullptr;
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
      if( cCurrentLine != nullptr )
	{
	  PointPtr lPt = cCurrentLine->getFacet()->getPoint(cCurrentLineSelectPoint);	  
	  if( lPt != nullptr )
	    {
	      viewCurrentPoint( lPt->get());
	    }
	}
    }
    //-------------------------------------
    void insertCurrentLineSelectPoint( Point3d iPt, DataBase & iBase)
    {            
      cCurrentLine->getFacet()->insertPoint( cCurrentLineSelectPoint, iPt,  iBase );     
      renumberPoints();
    }
    //-------------------------------------
    void delCurrentLineSelectPoint(DataBase & iBase)
    {            
      if( cCurrentLine->getFacet()->delPoint( cCurrentLineSelectPoint, iBase ) )
	{
	  delete cCurrentLine;
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
     if( cCurrentLine != nullptr )
	{
	  PointPtr lPt = cCurrentLine->getFacet()->getPoint(cCurrentLineSelectPoint);	  
	  if( lPt != nullptr )
	    {
	      std::cout << "changeCurrentLineSelectPoint : Point  found" << std::endl;
	      lPt->set(pPt);  ;
	      cCurrentLineSelectPoint = -1;
	      return true;
	    }
	  else
	    {
	      std::cout << "changeCurrentLineSelectPoint : Point not found" << std::endl;
	      
	    }
	}
     else
       std::cout << "changeCurrentLineSelectPoint : CurrentLine not found" << std::endl;

     return false;
    }
    //-------------------------------------
				 
  private:


    ObjectPolylines*     cCurrentPoint = nullptr;
    ObjectPolylines*     cCurrentPointObject = nullptr;


  public:
    CurrentInput();
    ~CurrentInput();


    Object*   getCurrentCreation();
    Object*   validCurrentCreation( DataBase & iBase);
    void      cancelCurrentCreation( );
    void      swapCurrentCreation( Object* pCurrentCreation, bool iFlagDel = true );
    void      addToInput( EntityPtr, bool pFlagLink );


		
    void             viewCurrentPoint( Point3d & pPt );
    void             hideCurrentPoint();
    void             addPointToCurrentLine( Point3d pPt );
    void             delLastPoint();
    void             delAllPoint();
    GLuint           getNbCurrentPoints();
    ObjectFacet *    convertCurrentLineToFacet(DataBase & iBase);
    ObjectPoly *     convertCurrentLineToFacetPoly(DataBase & iBase);
    ObjectPoly *     convertCurrentLineToBiFacetPoly(DataBase & iBase);
    ObjectPolylines* convertCurrentLineToPolylines(DataBase & iBase);
    ObjectLine *     convertCurrentLineToLine(DataBase & iBase);
    ObjBSpline*      convertCurrentLineToBSpline(DataBase & iBase, size_t iMaille, bool iClosed);
    
    bool isCurrentPoints()   { return cCurrentLine     != nullptr; }
    bool isCurrentCreation() { return cCurrentCreation != nullptr; }
    FacetPtr getCurrentLine()
    {
      if( cCurrentLine != nullptr)
	return  cCurrentLine->getFacet();
      return nullptr;
    }

    bool execVisitorOnCurrentLine( EntityVisitor & iVisit ){    
      if( cCurrentLine != nullptr)
	{
	  cCurrentLine->getShape()->execVisitor( iVisit );
	  return true;
	}
      return false;
    }
    
    ObjectPolylines* getObjectCurrentLine()
    {
      return cCurrentLine;
    }    
    void drawGL(ViewProps& iViewGen , ViewProps& iViewInputCursor, ViewProps& iViewInputPoly, ViewProps& iViewInputObject,  GLMode iSelectOrDrawMode, SelectType iSelectType);
  };
  
  //************************************
}



#endif
