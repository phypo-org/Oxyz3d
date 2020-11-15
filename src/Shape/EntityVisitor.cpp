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
    }
  //*********************************************
  void VisitorNormalFacet::execBeginFacet( Facet* pFacet )
  {
    std::vector<LinePtr> & lLines = pFacet->getLines();
    
    size_t lNb = lLines.size();
    
    if( lNb ==  3 || lNb == 2 )
      {
	Calcul3d::Normal( lLines[0]->first()->get(),
			  lLines[0]->second()->get(),
			  lLines[1]->second()->get(), // pas first !!!		
			  pFacet->getNormal() );
	return;
      }
    
     if( lNb > 3 )
       {
	 int i = lNb/3;
	 int j = (lNb*2)/3;
	 Calcul3d::Normal( lLines[0]->first()->get(),
			   lLines[i]->second()->get(),
			   lLines[j]->second()->get(), // pas first !!!		
			   pFacet->getNormal() );
	 return;
       }  
  }

  //*********************************************
  //*********************************************

  //*********************************************

  //*********************************************
 
}// namespace

