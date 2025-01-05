#ifndef H__Point__H
#define H__Point__H


#include "EntityBase.h"

namespace PP3d {

  
  //*********************************************
 class Point :  public Entity {
		
  public:
    Point3d cPt;

  public:
    Point( PDouble iX=0, PDouble iY=0,  PDouble iZ=0 )
      :cPt( iX, iY, iZ )
    {
    }
    
    Point( const Point3d & p )
      :cPt( p )
    {			
    }
    
    Point( const Point & iPt )
      :cPt( iPt.x(), iPt.y(), iPt.z() )
    {
    }
  
    PDouble x() const { return cPt.x(); }
    PDouble y() const { return cPt.y(); }
    PDouble z() const { return cPt.z(); }

    void set( PDouble iX=0, PDouble iY=0,  PDouble iZ=0 )
    {
      cPt.set( iX, iY, iZ );
    }

    
    bool isVoid() const override       { return false; }

    ShapeType getType() const override { return ShapeType::Point;}
    
    Point3d  getCenter3d() override    { return cPt;}  ;

    Point3d  get() const               { return cPt; }
    Point3d& get()                     { return cPt; }   
    operator const Point3d&() const    { return cPt; }    
    operator       Point3d()           { return cPt; }

    void     set(const Point3d & iPt)  {  cPt = iPt; }
		
    void execVisitor( EntityVisitor& pVisit ) override;

  public:
    void execVisitor( EntityVisitorNode& pVisit ) override;
    friend class Line;

    bool clear() override              { cPt.zero(); return Entity::clear(); }

    friend std::ostream & operator << ( std::ostream & pOs, Point& pEntity )
    {
      pOs << ((Entity&)pEntity) <<  " " << pEntity.get() ;
      return pOs;
    }


   virtual std::string  getStringInfo( ){
     std::ostringstream lStr;  
     lStr << Entity::getStringInfo() << " " << cPt;
     return lStr.str();
   }
  };
  //*********************************************

  using PointPtr     = Point*;
  using PointPtrPair = std::pair<PP3d::Point*,PP3d::Point*>;
  using PointPtrVect = std::vector<PP3d::Point*>;
  using PointPtrSet  = std::set<PP3d::Point*>;
  using PointPtrHash = std::unordered_set<PP3d::Point*>;

  

  inline static void operator *= (PointPtrSet pSet, const Mat4& pMat )
  {
    for( auto lIter = pSet.begin(); lIter != pSet.end(); ++lIter) 
      (*lIter)->cPt *= pMat;
  }
  inline static void operator *= (PointPtrHash pSet, const Mat4& pMat )
  {
    for( auto lIter = pSet.begin(); lIter != pSet.end(); ++lIter) 
      (*lIter)->cPt *= pMat;
  }
  inline static PointPtrPair Reverse(const PointPtrPair& iPair ) {  return PointPtrPair( iPair.second, iPair.first); }

   //*********************************************

}

#endif
