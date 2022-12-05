#include "EntityVisitor.h"

#include "Entity.h"
#include "Object.h"
#include "ViewProps.h"
#include "ObjProps.h"


namespace PP3d {
  //*********************************************
	
  void VisitorTrace::execPoint( Point* pPoint )
  {
    cOs << "\t\t\t\t"<< (*pPoint) << " " <<  pPoint->cPt << std::endl;
  }
	
  void VisitorTrace::execBeginLine ( Line* pLine)
  {
    cOs << "\t\t\t" <<(*pLine) << " point:" << pLine->isPoint()  << std::endl;
  }
  void VisitorTrace::execBeginFacet( Facet* pFacet )
  {
    cOs << "\t\t" <<(*pFacet) << " Lines:" << pFacet->getNbLines()  << std::endl;
  }	
  void VisitorTrace::execBeginPoly ( Poly* pPoly )
  {
    cOs << "\t" <<(*pPoly)
	<< " Facets:" << pPoly->getFacets().size()
	<< std::endl;
  }		
  void VisitorTrace::execBeginObject( Object* pObj)
  {
    cOs << (*pObj) << " " << pObj->getName()
	<< " shape:" << (void*)pObj->getShape()
	<< std::endl;
  }

  //*********************************************
  void VisitorMinMax::execPoint(  Point* pPoint )
  {
    add( pPoint->get() );
  }
  //*********************************************
  void VisitorInvertNormal::execBeginLine( Line* pLine ) 
    {
      pLine->inversePoint();
      //cOs << "\t" <<(*pPoly)
      //	<< " Facets:" << pPoly->getFacets().size()
      //	<< std::endl;
  }		
  //*********************************************
  void VisitorNormalFacet::execBeginFacet( Facet* pFacet )
  {
    pFacet->computeNormal();
  }		
  //*********************************************
  void VisitorConcaveFacet::execBeginFacet( Facet* pFacet )
  {
    //   std::cout << "    VisitorConcaveFacet::execBeginFacet" << std::endl;
   
    if(  pFacet->computeConcave() ){
      cCpt++;
    }
  }		
 }// namespace
