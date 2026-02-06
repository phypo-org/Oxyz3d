#include "ViewProps.h"
#include "EntityVisitor.h"
#include "Poly.h"
#include "SortVisitor.h"
#include "Selection.h"

namespace PP3d{

  //*********************************************
  void Poly::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execBeginPoly( this );
		
    for( FacetPtr lFacet : getFacets() )
      {
	lFacet->execVisitor( pVisit );
      }
    pVisit.execEndPoly( this );
  }
  //-------------------------------------
  void Poly::execVisitor( EntityVisitorNode& pVisit )
  {
    pVisit.execNode(  this, nullptr );
    for( FacetPtr lFacet : getFacets() )
      {
	pVisit.execNode( lFacet, this );
	lFacet->execVisitor( pVisit );
	pVisit.execEndNode( lFacet, this );
      }
    pVisit.execEndNode( this, nullptr );
  }
  //-------------------------------------
  Point3d Poly::getNormal3d()
  {
    Point3d lNorm;
    for( FacetPtr lFacet : getFacets() )
      {
        lNorm += lFacet->getNormal();
      }
    if( cFacets.size() )
      lNorm /=  cFacets.size();
    return lNorm;
  }
	


   //---------------------------		
 
  void Poly::drawPoints( ViewProps& pViewProps )
  {
    for( FacetPtr lFacet : cFacets )
      {
        lFacet->drawPoints( pViewProps );
      }
  }
  //---------------------------		
  void Poly::drawLines( ViewProps& pViewProps )
  {
    for( FacetPtr lFacet : cFacets )
      {
        lFacet->drawLines( pViewProps );
      }
  }
  //---------------------------	
  void Poly::drawPointsLines( ViewProps& pViewProps ) 
  {
    for( FacetPtr lFacet : cFacets )
      lFacet->drawPointsLines( pViewProps );
  }
  //---------------------------		 
  void Poly::drawFacets( ViewProps& pViewProps )
  {
    for( FacetPtr lFacet : cFacets )
      lFacet->drawFacet( pViewProps );
  }
  //---------------------------
  void Poly::drawGL  ( ViewProps& pViewProps )
  {
    switch( pViewProps.cSelectType )
      {       
      case SelectType::Point:
	{
	  if( pViewProps.cViewMode == ViewMode::FULL )
            {
              glEnable(GL_LIGHTING);
              drawFacets( pViewProps );
            }
					
          glDisable(GL_LIGHTING);	    
	  drawPointsLines( pViewProps);
	}
	break;
				
      case SelectType::Null:
      case SelectType::All:
      case SelectType::Line:			
      case SelectType::Facet:
      case SelectType::Poly:				
      case SelectType::Group:
      case SelectType::Object:
	{
          if( pViewProps.cViewMode == ViewMode::FULL)		
            {
              glEnable(GL_LIGHTING);
              drawFacets( pViewProps );
            }
					
          glDisable(GL_LIGHTING);
	  drawLines( pViewProps );			         	
	}
	break;
      }
  }
  //---------------------------		
  //---------------------------		
  //---------------------------			  
  void Poly::pickingFacets( )    
  {
    for( FacetPtr lFacet : cFacets )
      lFacet->pickingFacet();
  }
  //---------------------------		
  void Poly::selectGL( ViewProps& pViewProps )
  {
        std::cout << ">>>>>>>> Poly::pickingGL " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;


    if( cMyProps.cVisible == false )
      {
        std::cout << ">>>>>>>> Poly::pickingGL NOT VISIBLE " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;

        return;
      }
     
    
    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	break;


      case SelectType::All:
      case SelectType::Point:
      case SelectType::Line:       
      case SelectType::Facet:
        {        // On renvoie le picking à la facette 
          for( FacetPtr lFacet : cFacets )
            lFacet->selectGL( pViewProps );
        }
        break;


        
      case SelectType::Group:
      case SelectType::Object:
      case SelectType::Poly:
        {
          if( pViewProps.cViewMode == ViewMode::FULL)		
            {
              // la facette mais avec l'id de l'objet
              pViewProps.facetGL( isSelect(), isHighlight(), isMagnet());
              ColorRGBA::Id( getId());     
              pickingFacets();
            }
          // sinon rien !!!
        }
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  }
  //********************************
}
