#ifndef H__ViewProps__H
#define H__ViewProps__H

#include "PP3dType.h"
#include "Matrice.h"

#include <unordered_set>
#include "Entity.h"

namespace PP3d {

  class Mat4;
  struct ObjProps;

 
  //******************************
  class ViewProps{

  public:
    
    bool cFlagDrawPoint;
    bool cFlagDrawLine;
    bool cFlagDrawFacet;

 

    ColorRGBA  cColorPoint;
    ColorRGBA  cColorPointSelect;
    ColorRGBA  cColorPointHighlight;
    ColorRGBA  cColorLine;
    ColorRGBA  cColorLineSelect;
    ColorRGBA  cColorLineHighlight;
    ColorRGBA  cColorFacet;
    ColorRGBA  cColorFacetSelect;
    ColorRGBA  cColorFacetHighlight;

		
    SelectType cSelectType;
    GLMode     cGLMode = GLMode::Draw;

    float      cPointSize=8;
    float      cPointSizeSelect=10;
    float      cPointSizeHighlight=12;
	
    float      cLineWidth=2;
    float      cLineWidthSelect=4;
    float      cLineWidthHighlight=6;
		
    GLuint     cViewMode=0;        // Skeleton
    GLuint     cFlagViewNormal=0;

    bool       cDebug=false;
    GLuint     cDebugView=0;
    GLuint     cDebugSel=0;

    Mat4*      cSelectMatrice=nullptr;
	
    ViewProps(SelectType pSelectType= SelectType::Poly );
    virtual ~ViewProps();

    //------------------
    void pointGL()
    {
      if( cGLMode == GLMode::Draw )	cColorPoint.GL();
      glPointSize( cPointSize );				
    }
    void pointSelectGL()
    {
      if( cGLMode == GLMode::Draw )	cColorPointSelect.GL();
      glPointSize( cPointSizeSelect );				
    }
    void pointHighlightGL()
    {
      if( cGLMode == GLMode::Draw )	cColorPointHighlight.GL();
      glPointSize( cPointSizeHighlight );				
    }
    void pointGL( bool pSelect, bool pHighlight)
    {
      if(  pSelect || pHighlight )
	{
	  if( pHighlight )
	    {
	      pointHighlightGL();
	      //	      pHighlight = false ; // argh ...
	    }
	  else
	    if( pSelect )
	      pointSelectGL();
	}						
      else
	pointGL();
    }
    //------------------
    float setPointSize( float iVal ) {
      float lTmp =  cPointSize;
      cPointSize = iVal;
      return lTmp;
    }
   float deflatePointSize() {
      float lTmp =  cPointSize;
      cPointSize -= std::max( cPointSize / 4.0, 1.0);
      return lTmp;
    }

    
    //------------------
    void lineGL()
    {
      if( cGLMode == GLMode::Draw )	cColorLine.GL() ;
      
      glLineWidth( cLineWidth );
    }
    void lineSelectGL()
    {
      if( cGLMode == GLMode::Draw )	cColorLineSelect.GL() ;
      glLineWidth( cLineWidthSelect );
    }
    void lineHighlightGL()
    {
      if( cGLMode == GLMode::Draw )	cColorLineHighlight.GL() ;
      glLineWidth( cLineWidthHighlight );
    }
    void lineGL( bool pSelect, bool pHighlight)
    {
      if( pSelect || pHighlight )
	{
	  if( pHighlight )
	    {
	      lineHighlightGL();
	      //	      pHighlight = false ; // argh ...
	    }
	  else if( pSelect )
	    lineSelectGL();
	}
      else
	lineGL();				
    }
    //------------------
    void facetGL( )
    {
      if( cGLMode == GLMode::Draw )	cColorFacet.materialGL();
    }
    void facetSelectGL()
    {
      if( cGLMode == GLMode::Draw )	cColorFacetSelect.materialGL();
    }
    void facetHighlightGL()
    {
      if( cGLMode == GLMode::Draw )	cColorFacetHighlight.materialGL();
    }
    void facetGL( bool pSelect, bool pHighlight )
    {
      if( pSelect || pHighlight )
	{
	  if( pHighlight )
	    {
							
	      facetHighlightGL();
	      //	      pHighlight = false ; // argh ...
	    }
	  else if( pSelect )
	    facetSelectGL();
	}
      else
	facetGL();
    }

    friend class Exec;
  };
  //******************************

}

#endif
