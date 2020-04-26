#ifndef H__Entity__H
#define H__Entity__H


#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <set>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <set>
#include <unordered_set>
#include <vector>


#include "Point3d.h"
#include "Calcul3d.h"
#include "Matrice.h"

#include "EntityVisitor.h"

namespace PP3d {

  class Entity;
  class Point;
  class Line;
  class Facet;
  class Poly;
  class Object;
  class Mat4;

  class EntityVisitorType;

	
  using EntityId      = GLuint; // au moins 32 bits
	
  using VectEntityPtr = std::vector<Entity*>;

							 
  //*********************************************

  class Entity {
		
  protected:

    EntityId          cId=0;
		
    std::set<Entity*> cOwners;

    bool              cIsSelected=false;
    bool              cIsHighlight=false;


    bool              cTreeOpen=false;  // for interface
	         
  public:
    virtual ShapeType getType() const=0;
		
    Entity()
    {
    }
    virtual ~Entity();

		
    EntityId getId() { return cId; }
    bool     isIdVoid() { return cId==0;}

    bool isTreeOpen() { return cTreeOpen; }
    void setTreeOpen( bool iVal ) { cTreeOpen=iVal;}


  public:
    bool isSelect()                 { return   cIsSelected;}
    void setSelect( bool pFlag )    { cIsSelected  = pFlag;}		
    bool& isHighlight()              { return   cIsHighlight;} // la reference pour pouvoir la remettre a zero ! c'est vraiment moche !
    void setHighlight( bool pFlag ) { cIsHighlight =  pFlag;}

    auto & getOwners() { return  cOwners; }

    void   addOwner     ( Entity* pOwner ) { cOwners.emplace( pOwner); }
    void   removeOwner  ( Entity* pOwner ) { cOwners.erase( pOwner); }
    size_t howManyOwner ( ) { return cOwners.size(); }

		
    const char* getStrType() const { return GetStrShapeType( getType()) ; }
		
    friend std::ostream & operator << ( std::ostream & pOs, Entity& pEntity )
    {
      pOs << "Id:"      << pEntity.cId
	  << " Owners:" << pEntity.cOwners.size()
	  << " Type:"   << pEntity.getStrType() 
	  << " " << (void*)&pEntity ;
      return pOs;
    }
		

    virtual void execVisitor( EntityVisitor& pVisit ) = 0;
    virtual void execVisitor( EntityVisitorNode& pVisit ) = 0;

			
    void move   ( Point3d p );
    void scale  ( Point3d p );
    void modify ( Mat4& pMat );

    static Object* FindMyObject();

    friend class DataBase;
    friend class Line;
    friend class Facet;
    friend class Poly;
  };
	
  using EntityPtr=Entity*;
  using EntityPtrVect = std::vector<Entity*>;
  using EntityPtrSet  = std::set<Entity*>;
  using EntityPtrHash = std::unordered_set<Entity*>;

  //*********************************************
  class Point :  public Entity {
		
  public:
    Point3d cPt;

  public:
    Point( const Point3d p )
      :cPt( p )
    {			
    }
		
		
    ShapeType getType() const override { return ShapeType::Point;}

    Point3d  get() const { return cPt; }
    Point3d& get()       { return cPt; }
		
    void execVisitor( EntityVisitor& pVisit ) override;

  protected:
    void execVisitor( EntityVisitorNode& pVisit ) override;
    friend class Line;
  };
	  
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

  //*********************************************
  class Line  :  public Entity {

    PointPtrPair cPoints={nullptr,nullptr};
		
		
  public:
    Line( PointPtr lA, PointPtr lB )
    {		 
      cPoints = { lA, lB };
      lA->addOwner( this );
      if( lA != lB )
	lB->addOwner( this );
			
    }
    bool isPoint() { return cPoints.first == cPoints.second; } // pour la saisie du premier point d'une facette
								 
    ShapeType   getType() const override { return ShapeType::Line;}
		
    PointPtrPair& getPoints() { return cPoints; }
    PointPtr      getFirst()  const { return cPoints.first;}
    PointPtr      getSecond() const { return cPoints.second;}
    PointPtr      first()  const { return cPoints.first;}
    PointPtr      second() const { return cPoints.second;}
    PointPtr&     first() { return cPoints.first;}
    PointPtr&     second(){ return cPoints.second;}

    void execVisitor( EntityVisitor& pVisit )override;

  public:
    void execVisitor( EntityVisitorNode& pVisit )override;
		
    void inversePoint() {
      PointPtr lTmp = cPoints.first;
      cPoints.first = cPoints.second;
      cPoints.second = lTmp;
    }

				
    friend class Facet;
    friend class Maker;
  };
	
  using LinePtr     = Line*;
  using LinePtrVect = std::vector<Line*>;

  //*********************************************
  class  Facet :  public Entity{
		
    LinePtrVect cLines;

    Point3d cNorm;

  public:
    ShapeType getType() const  override { return ShapeType::Facet;}
	
    void closeFacet();

    void addLine( LinePtr pLine )
    {
      cLines.push_back( pLine );
      pLine->addOwner( this );
    }
    LinePtrVect& getLines()   { return cLines;}
    GLuint getNbLines()  const { return (GLuint )cLines.size(); }

    Point3d & getNormal()     { return cNorm; }
		
    void execVisitor( EntityVisitor& pVisit )override;
    void inverseLines( );

  protected:
    void execVisitor( EntityVisitorNode& pVisit )override;
    friend class Poly;
    friend class Maker;
  };
	
  using FacetPtr     = Facet*;
  using FacetPtrVect = std::vector<Facet*>;

  //*********************************************
  class  Poly :  public Entity{
		
    FacetPtrVect cFacets;
		
  public:
    ShapeType getType() const override { return ShapeType::Poly;}
		
    void addFacet( FacetPtr pFacet)
    {
      cFacets.push_back( pFacet );
      pFacet->addOwner( this );
    }

    FacetPtrVect&  getFacets()  { return cFacets; }
    void execVisitor( EntityVisitor& pVisit )override;


  protected:
    void execVisitor( EntityVisitorNode& pVisit )override;
		
    friend class Object;
    friend class Maker;
  };
  using PolyPtr     = Poly*;
  using PolyPtrVect = std::vector<Poly*>;
  //*********************************************
	
}

#endif
