#ifndef H__ObjSpline__H
#define H__ObjSpline__H

#include "Shape/ObjectPolylines.h"
#include "SplineCalcul.h"

namespace PP3d {

  //******************************
  class ObjBSpline : public ObjectPolylines   // create point is in facet o polylines
  {
    VectDouble3 cPoles;                // poles are store localy 
    Facet*      cSplinePts = nullptr ; // Spline calcul result. Les points de la splines
    size_t      cMyMaille = 12;
    bool        cClosedSpline = false;
  public:	
    ObjBSpline(  const char*pName, Facet* pFacet, bool iClosed )
      : ObjectPolylines( pName, pFacet )
      , cClosedSpline( iClosed)
    {;}
    ObjBSpline(  const std::string &pName, Facet* pFacet, bool iClosed )
      : ObjectPolylines( pName, pFacet )
      , cClosedSpline( iClosed)
    {;}
    virtual ~ObjBSpline();
		
    ObjectType getObjType() const override { return ObjectType::ObjBSpline; }
    
    FacetPtr getSplinePts() { return cSplinePts; }

    
    //------------------------------------
    void drawGL( ViewProps& pViewProps )   override;
    void selectGL( ViewProps& pViewProps ) override;  // phipo 20231204
   //---------------------------		
    virtual void drawSplinePointGL( ViewProps& pViewProps );
    virtual void drawSplineLineGL ( ViewProps& pViewProps );
    //---------------------------		
    void makePoles();
    void makePtsFromPoles( size_t iMaille );   
    //---------------------------		
    void remakeAll(size_t iMaille ) {
      cPoles.clear();
      makePtsFromPoles( iMaille  );
    }
    //---------------------------		
    static ObjectPolylines* CreatePolyline( ObjBSpline * iSpline, size_t iMaille);

  };
  //******************************

}
#endif
