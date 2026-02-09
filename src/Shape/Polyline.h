#ifndef H__Polyline__H
#define H__Polyline__H


#include "Facet.h"

namespace PP3d {

  //*********************************************
  class  Polyline :  public Facet{
		
  public:
    Polyline() {;}
    Polyline(  LinePtr iLine )                   { addLine( iLine ); }
    Polyline(  LinePtr iLine1, LinePtr iLine2  ) { addLine( iLine1 ); addLine( iLine2 ); }
    Polyline(  LinePtr iLine1, LinePtr iLine2, LinePtr iLine3  ) { addLine( iLine1 ); addLine( iLine2 );addLine( iLine3 ); }
    Polyline(  LinePtr iLine1, LinePtr iLine2, LinePtr iLine3, LinePtr iLine4  ) { addLine( iLine1 ); addLine( iLine2 );addLine( iLine3 );addLine( iLine4 ); }

    ShapeType getShapeType() const  override { return ShapeType::Polyline;}	
    Polyline* dupPolyline() const;
 
    virtual void drawGL( ViewProps& pViewProps, bool iForceSelect=false, bool iSelect=false )   override;
    virtual void selectGL( ViewProps& pViewProps, EntityId iForceId=0 ) override;

  }; 
  //*********************************************
  using PolylinePtr     = Polyline*;

  using PolylinePtrVect = std::vector<PolylinePtr>;

  //*********************************************

}

#endif
