#include "ViewProps.h"

#include "PP3dType.h"


namespace PP3d {

  //******************************

  ViewProps::~ViewProps()
  {
  }
  //----------------------------
  ViewProps::ViewProps(SelectType pSelectType)
    :	cColorPoint      ( 0.0,     0.8,   0)
    , cColorPointSelect( 1,       0.5,   0)
    , cColorPointHighlight( 1,    0.7,   1)
		, cColorLine      (  0.8,     0.2,   0.8)
		, cColorLineSelect(  1,       0.5,   0)
		, cColorLineHighlight(  1,    0.9,   0)
    , cColorFacet     (  0.5,     0.5,   0.5)
    , cColorFacetSelect( 1,       0.4,   0.4)
    , cColorFacetHighlight( 1,    0.6,   0.4)
		, cSelectType(pSelectType)
  {
  }

  //******************************
   
};
