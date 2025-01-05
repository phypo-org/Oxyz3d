#ifndef H__ViewProps__H
#define H__ViewProps__H

#include "PP3dType.h"
#include "Matrice.h"

#include <unordered_set>
#include "Entity.h"

namespace PP3d {

  class Mat4;
  struct ObjProps;

  enum class ViewMode{ FULL, SKELETON };


#undef TestEnumViewMode  
#define TestEnumViewMode(TYPE) case ViewMode::TYPE : return #TYPE;
  
  inline static const char * GetStrViewMode( ViewMode pType)
  {
    switch(pType){ 
    TestEnumViewMode( FULL );
    TestEnumViewMode( SKELETON );
    }
    return "FULL";
  }
 
#undef TestEnumViewMode  
#define TestEnumViewMode(TYPE) if(  ::strcmp( pStr, #TYPE ) == 0 ) return ViewMode::TYPE;
  
  inline static ViewMode GetViewModeFromStr( const char* pStr )
  {
    TestEnumViewMode( FULL );
    TestEnumViewMode( SKELETON );
    return ViewMode::FULL;
  }

  inline std::ostream& operator << ( std::ostream& pOs, ViewMode pType )
  {
    pOs <<  GetStrViewMode ( pType );
    return pOs;
  }
   
  //******************************
  class ViewProps{

    
  public:
    
    bool cFlagDrawPoint;
    bool cFlagDrawLine;
    bool cFlagDrawFacet;

 

    ColorRGBA  cColorPoint;
    ColorRGBA  cColorPointSelect;
    ColorRGBA  cColorPointHighlight;
    ColorRGBA  cColorPointMagnet;

    ColorRGBA  cColorLine;
    ColorRGBA  cColorLineSelect;
    ColorRGBA  cColorLineHighlight;
    ColorRGBA  cColorLineMagnet;

    ColorRGBA  cColorFacet;
    ColorRGBA  cColorFacetSelect;
    ColorRGBA  cColorFacetHighlight;
    ColorRGBA  cColorFacetMagnet;

		
    SelectType cSelectType;
    GLMode     cGLMode = GLMode::Draw;

    float      cPointSize=8;
    float      cPointSizeSelect=10;
    float      cPointSizeHighlight=12;
    float      cPointSizeMagnet=6;
	
    float      cLineWidth=2;
    float      cLineWidthSelect=4;
    float      cLineWidthHighlight=6;
    float      cLineWidthMagnet=3;
		
    ViewMode   cViewMode=ViewMode::FULL;        
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
    void pointMagnetGL()
    {
      if( cGLMode == GLMode::Draw )     cColorPointMagnet.GL();
      glPointSize( cPointSizeMagnet );
      
    }
    void pointHighlightGL()
    {
      if( cGLMode == GLMode::Draw )	cColorPointHighlight.GL();
      glPointSize( cPointSizeHighlight );				
    }
    void pointGL( bool pSelect, bool pHighlight, bool pMagnet)
    {
      if( pHighlight )
        pointHighlightGL();
      else if( pSelect )
        pointSelectGL();
      else if( pMagnet )
        pointMagnetGL();
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
    void lineMagnetGL()
    {
      if( cGLMode == GLMode::Draw )	cColorLineMagnet.GL() ;
      glLineWidth( cLineWidthMagnet );
    }
    void lineHighlightGL()
    {
      if( cGLMode == GLMode::Draw )	cColorLineHighlight.GL() ;
      glLineWidth( cLineWidthHighlight );
    }
    void lineGL( bool pSelect, bool pHighlight, bool pMagnet)
    {
      if( pHighlight )
        lineHighlightGL();
      else if( pSelect )
        lineSelectGL();
      else if( pMagnet )
        lineMagnetGL();
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
    void facetMagnetGL()
    {
      if( cGLMode == GLMode::Draw )	cColorFacetMagnet.materialGL();
    }
    void facetHighlightGL()
    {
      if( cGLMode == GLMode::Draw )	cColorFacetHighlight.materialGL();
    }
    void facetGL( bool pSelect, bool pHighlight, bool pMagnet )
    {      
      if( pHighlight )					
        facetHighlightGL();
      else if( pSelect )
        facetSelectGL();
      else if( pMagnet )
        facetMagnetGL();
      else
	facetGL();
    }

    friend class Exec;
  };
  //******************************

}

#endif
