#ifndef H__ObjSpline__H
#define H__ObjSpline__H

#include "Shape/ObjectPolylines.h"
#include "SplineCalcul.h"

namespace PP3d {

  //******************************
  class ObjBSpline : public ObjectPolylines   // create point is in facet o polylines
  {
    VectDouble3 cPoles;                // poles are store localy 
    Facet*      cSplinePts = nullptr ; // Spline calcul result

  public:	
     ObjBSpline(  const char*pName, Facet* pFacet)
      : ObjectPolylines( pName, pFacet )
    {;}
    ObjBSpline(  const std::string &pName, Facet* pFacet)
      : ObjectPolylines( pName, pFacet )
    {;}
		
    ObjectType getObjType() const override { return ObjectType::ObjBSpline; }

    
    //------------------------------------
    void drawGL( ViewProps& pViewProps ) override;
    //---------------------------		
    virtual void drawSplinePointGL( ViewProps& pViewProps );
    virtual void drawSplineLineGL ( ViewProps& pViewProps );
    //---------------------------		
    void makePoles();
    void makePtsFromPoles( DataBase & iBase, size_t iMaille = 0 );   

  };
  //******************************

}
#endif
