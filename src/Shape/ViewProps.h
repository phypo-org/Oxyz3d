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
	
		float	     cLineWidth=2;
		float      cLineWidthSelect=4;
		float      cLineWidthHighlight=6;
		
		GLuint     cViewMode=0;

		bool       cDebug=false;
		GLuint     cDebugView=0;
		GLuint     cDebugSel=0;

		Mat4*      cSelectMatrice=nullptr;
	
    ViewProps(SelectType pSelectType= SelectType::Poly );
   virtual ~ViewProps();

		//------------------
		void pointGL()
		{
			cColorPoint.GL();
			glPointSize( cPointSize );				
		}
		void pointSelectGL()
		{
			cColorPointSelect.GL();
			glPointSize( cPointSizeSelect );				
		}
		void pointHighlightGL()
		{
			cColorPointHighlight.GL();
			glPointSize( cPointSizeHighlight );				
		}
		void pointGL( bool pSelect, bool& pHighlight)
		{
			if( pSelect || pHighlight )
				{
					if( pHighlight )
						{
							pointHighlightGL();
							pHighlight = false ; // argh ...
						}
					else
						if( pSelect )
							pointSelectGL();
				}						
			else
				pointGL();
		}
		//------------------
		void lineGL()
		{
			cColorLine.GL() ;
			glLineWidth( cLineWidth );
		}
		void lineSelectGL()
		{
			cColorLineSelect.GL() ;
			glLineWidth( cLineWidthSelect );
		}
		void lineHighlightGL()
		{
			cColorLineHighlight.GL() ;
			glLineWidth( cLineWidthHighlight );
		}
		void lineGL( bool pSelect, bool& pHighlight)
		{
			if( pSelect || pHighlight )
				{
					if( pHighlight )
						{
							lineHighlightGL();
							pHighlight = false ; // argh ...
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
			cColorFacet.materialGL();
		}
		void facetSelectGL()
		{
			cColorFacetSelect.materialGL();
		}
		void facetHighlightGL()
		{
			cColorFacetHighlight.materialGL();
		}
		void facetGL( bool pSelect, bool& pHighlight )
		{
			if( pSelect || pHighlight )
				{
					if( pHighlight )
						{
							
							facetHighlightGL();
							pHighlight = false ; // argh ...
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

};

#endif
