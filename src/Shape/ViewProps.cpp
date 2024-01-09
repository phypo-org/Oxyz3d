#include "ViewProps.h"

#include "PP3dType.h"


namespace PP3d {

  //******************************

  ViewProps::~ViewProps()
  {
  }
  //----------------------------
  ViewProps::ViewProps(SelectType pSelectType)
    : cColorPoint           ( 0.0f,    0.8f,     0          )
    , cColorPointSelect     ( 1,       0.5f,     0          )
    , cColorPointHighlight  ( 1,       0.7f,     1,     0.9f)
    , cColorPointMagnet     ( 0.4f,    0.4f,     1          )
    , cColorLine            ( 0.8f,    0.2f,     0.8f       )
    , cColorLineSelect      ( 1,       0.5f,     0          )
    , cColorLineHighlight   ( 1,       0.9f,     0,     0.9f)
    , cColorLineMagnet      ( 0.4f,    0.4f,     1          )
    , cColorFacet           ( 0.5f,    0.5f,     0.5f       )
    , cColorFacetSelect     ( 1,       0.4f,     0.4f       )
    , cColorFacetHighlight  ( 1,       0.6f,     0.4f,  0.9f)
    , cColorFacetMagnet     ( 0.4f,    0.4f,     1          )
    , cSelectType(pSelectType)
  {
  }

  //******************************
   
}
