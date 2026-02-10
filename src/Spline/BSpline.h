#ifndef H__BSpline__H
#define H__BSpline__H

#include "Shape/Polyline.h"
#include "SplineCalcul.h"


namespace PP3d {

  //******************************
  class BSpline : public Polyline  // create point is in facet o polylines
  {
    VectDouble3 cPoles;                // poles are store localy 
    Polyline*   cSplinePts = nullptr ; // Spline calcul result. Les points de la splines
    size_t      cMyMaille = 12;
    bool        cClosedSpline = false;
  public:	
    BSpline( LinePtrVect & iLines, bool iClosed )
      : Polyline( iLines )
      , cClosedSpline( iClosed )
    {;}
 
    virtual ~BSpline();
		
     ShapeType getShapeType() const  override { return ShapeType::BSpline;}	
    
    PolylinePtr getSplinePts() { return cSplinePts; }

    
    //------------------------------------
    void drawGL( ViewProps& pViewProps, bool iForceSelect=false, bool iSelect=false )   override;
    void selectGL( ViewProps& pViewProps, EntityId iForceId=0 ) override;  // phipo 20231204

    
   
    //---------------------------		
    void makePoles();
    void makePtsFromPoles( size_t iMaille );   
    //---------------------------		
    void remakeAll(size_t iMaille ) {
      cPoles.clear();
      makePtsFromPoles( iMaille  );
    }
    //---------------------------		
    static PolylinePtr CreatePolyline( BSpline * iSpline, size_t iMaille);

  };
  //******************************
 using BSplinePtr     = BSpline*;

  using BSplinePtrVect = std::vector<BSplinePtr>;

  
  //******************************

}
#endif
