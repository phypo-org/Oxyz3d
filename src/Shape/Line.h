#ifndef H__Line__H
#define H__Line__H


#include "Point.h"

namespace PP3d {

    //*********************************************
  class Line  :  public Entity {
    
    PointPtrPair cPoints={nullptr,nullptr};
    
    
  public:
    
    Line( PointPtr lA, PointPtr lB ) { set( lA, lB ); }
    Line( PointPtr lA )              { set( lA, lA ); }

    bool isPoint() { return cPoints.first == cPoints.second; } // pour la saisie du premier point d'une facette
    bool isSamePointXYZ() { return cPoints.first->get() == cPoints.second->get(); } // Meme coordonnes xyz de deux points
    bool isSamePointEpsi(long double iEpsi ) { return cPoints.first->get().sameEpsi(iEpsi, cPoints.second->get() ); } // Meme coordonnes xyz de deux points

    bool isVoid() const override { if(  cPoints.first == nullptr &&  cPoints.second == nullptr ) return true; return false; }
       
    ShapeType     getType() const override   { return ShapeType::Line;}
		
    PointPtrPair& getPoints()                { return cPoints; }
    
    PointPtr      getFirst()  const          { return cPoints.first;}
    PointPtr      getSecond() const          { return cPoints.second;}
    PointPtr      first()  const             { return cPoints.first;}
    PointPtr      second() const             { return cPoints.second;}
    PointPtr&     first()                    { return cPoints.first;}
    PointPtr&     second()                   { return cPoints.second;}

    Point3d       getVector3d()              { return (second()->get() - first()->get()); }

    Point3d       getCenter3d()  override    { Point3d lRes = (second()->get() + first()->get()); lRes/=2; return lRes; };
     
    void set( PointPtr lA, PointPtr lB )
    {
      if( getFirst() )  getFirst()->removeOwner(this);
      if( getSecond() ) getSecond()->removeOwner(this);
	
      cPoints = { lA, lB };
      lA->addOwner( this );
      if( lA != lB )
	lB->addOwner( this );			
    }
    void setFirst(PointPtr iPoint) {
      if( getFirst() )  getFirst()->removeOwner(this);
      cPoints.first = iPoint;
      iPoint->addOwner( this );
    }
    void setSecond(PointPtr iPoint) {
      if( getSecond() )  getSecond()->removeOwner(this);
      cPoints.second = iPoint;
      iPoint->addOwner( this );
    }


    void execVisitor( EntityVisitor& pVisit )override;
 
  public:
    void execVisitor( EntityVisitorNode& pVisit )override;

     
    void inversePoint() {
      PointPtr lTmp = cPoints.first;
      cPoints.first = cPoints.second;
      cPoints.second = lTmp;
    }
    
    Line* getReverseLineByOwner();
    Line* getReverseLineByPoint();

    friend class Facet;
    friend class Maker;

    friend std::ostream & operator << ( std::ostream & pOs, Line& pEntity )
    {
      pOs << ((Entity&)pEntity) <<  " " << pEntity.first() << " " << pEntity.second() ;
      return pOs;
    }

    
    Facet* myFacet() { return (Facet*) firstOwner(); }
    
  };
	
  using LinePtr     = Line*;
  using LinePtrVect = std::vector<Line*>;
  
  //--------------------------------
  
  struct FindPointPairVisitor : public EntityVisitor{
    const PointPtrPair & cPtPair;
    std::set<LinePtr>    cInternalSet;
    std::set<LinePtr>  & cSet;  // le set elimine les doublons
    	  
    virtual void execBeginLine ( LinePtr pLine );
  public:
    std::set<LinePtr> &getLines() { return cSet;}
	  
    FindPointPairVisitor(const PointPtrPair & iPtPair , std::set<LinePtr> & ioSet );	  
    FindPointPairVisitor(const PointPtrPair & iPtPair );  
  }; 
  
  inline Line * MakeLine( const Point3d & iA,   const Point3d & iB )
  {
    return new Line( new Point( iA ), new Point (iB ));
  }
  //*********************************************

}

#endif
