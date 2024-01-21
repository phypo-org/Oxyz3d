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

  

  enum class Compute{
    Nothing,
      FacetNormal,
      FacetConcave,
      FacetAll
      };
    
  

  class Entity;
  class Point;
  class Line;
  class Facet;
  class Poly;
  class Object;
  class Mat4;
  class DataBase;

  class EntityVisitorType;

	
  using EntityId      = GLuint; // au moins 32 bits
	
  using VectEntityPtr = std::vector<Entity*>;

							 
  //*********************************************

  class Entity {
		
  protected:

    EntityId          cId=0;
		
    std::set<Entity*>       cOwners;

    
    bool              cIsMagnet=false;
    bool              cIsSelected=false;
    bool              cIsHighlight=false;
    bool              cTreeOpen=false;      // for interface 
    
  private:
    void     setId(EntityId iId) { cId = iId; }
	         
  public:
    virtual ShapeType getType() const=0;
		
    Entity()	 	      
    {
    }
    virtual ~Entity();

    void deleteAllHieracrchy();
		
    EntityId getId() { return cId; }
    bool     isIdVoid() { return cId==0;}
    void     razId() { cId=0;}

    virtual bool isVoid() const =0;

    bool isTreeOpen() { return cTreeOpen; }
    void setTreeOpen( bool iVal ) { cTreeOpen=iVal;}

    virtual Point3d getCenter3d() =0;
    virtual Point3d getNormal3d();

  public:
    bool isMagnet()                 { return   cIsMagnet;}
    void setMagnet( bool pFlag )    { cIsMagnet  = pFlag;}		
    bool isSelect()                 { return   cIsSelected;}
    void setSelect( bool pFlag )    { cIsSelected  = pFlag;}		
    bool isHighlight()              { return   cIsHighlight;} // la reference pour pouvoir la remettre a zero ! c'est vraiment moche ! du coup ca marche mal ! un seul canvas le dessinne !
    
    void setHighlight( bool pFlag ) { cIsHighlight =  pFlag;}

    auto & getOwners() { return  cOwners; }
    Entity * firstOwner()
    {
      Entity* lTmp = nullptr;
      for( Entity* lOwner  :  cOwners ) { return lOwner;}
      return lTmp;
    }    
    

    //    void   addOwner     ( Entity* pOwner ) { cOwners.emplace( pOwner); }
    void   addOwner     ( Entity* pOwner ) { cOwners.emplace( pOwner); }
    void   removeOwner  ( Entity* pOwner ) { cOwners.erase( pOwner); }
    size_t howManyOwner ( ) { return cOwners.size(); }
    void   clearOwner   ( Entity* pOwner ) { cOwners.erase( pOwner); }
    void   clearAllOwner( ) { cOwners.clear(); }
    
    virtual void remove( Entity* lEntity )
    {
      lEntity->removeOwner( this );
    }

    void removeFromOwners()
    {
      for( Entity* lOwner : getOwners() )
	{
	  lOwner->remove( this );
	}
    }

    virtual bool clear() { clearAllOwner();  return true; }
		
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
    void scale  ( Point3d p ); // A n'utiliser que si l'objet est centre sur l'origine !
    void modify ( Mat4& pMat );

    static Object* FindMyObject();

    friend class DataBase;
    friend class CurrentInput;
    friend class MyRead;
    friend class Line;
    friend class Facet;
    friend class Poly;
    friend class Poly;
  };
	
  using EntityPtr=Entity*;
  using EntityPtrVect = std::vector<Entity*>;
  using EntityPtrSet  = std::set<Entity*>;
  using EntityPtrHash = std::unordered_set<Entity*>;

  template <class SET = std::set<Entity*> >
  static inline void ExecVisitor( SET & ioSet, EntityVisitor & pVisit )    
  {
    for( Entity * lEntity : ioSet )
      lEntity->execVisitor( pVisit );
  }
  template <class SET=std::set<Entity*>>
  static inline void ExecVisitorNode( SET  & ioSet, EntityVisitorNode & pVisit )    
  {
    for( Entity * lEntity : ioSet )
      lEntity->execVisitor( pVisit );
  }
  
  using OwnerPtr     = Entity*;
  using OwnerPtrSet  = std::set<OwnerPtr>;

  
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

    
    bool isVoid() const override { return false; }

    ShapeType getType() const override { return ShapeType::Point;}
    
    Point3d  getCenter3d() override { return cPt;}  ;

    Point3d  get() const { return cPt; }
    Point3d& get()       { return cPt; }
    operator const Point3d&() const { return cPt; }    
    operator       Point3d()        { return cPt; }

    void     set(const Point3d & iPt)  {  cPt = iPt; }
		
    void execVisitor( EntityVisitor& pVisit ) override;

  public:
    void execVisitor( EntityVisitorNode& pVisit ) override;
    friend class Line;

    bool clear() override { cPt.zero(); return Entity::clear(); }

    friend std::ostream & operator << ( std::ostream & pOs, Point& pEntity )
    {
      pOs << ((Entity&)pEntity) <<  " " << pEntity.get() ;
      return pOs;
    }
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
  inline static PointPtrPair Reverse(const PointPtrPair& iPair ) {  return PointPtrPair( iPair.second, iPair.first); }

  //*********************************************
   class Line  :  public Entity {

    PointPtrPair cPoints={nullptr,nullptr};
		
		
  public:
    
    Line( PointPtr lA, PointPtr lB )
    {		 
      set( lA, lB );			
    }
     Line( PointPtr lA )
    {		 
      set( lA, lA );			
    }

    bool isPoint() { return cPoints.first == cPoints.second; } // pour la saisie du premier point d'une facette
    bool isSamePointXYZ() { return cPoints.first->get() == cPoints.second->get(); } // Meme coordonnes xyz de deux points
    bool isSamePointEpsi(long double iEpsi ) { return cPoints.first->get().sameEpsi(iEpsi, cPoints.second->get() ); } // Meme coordonnes xyz de deux points

     bool isVoid() const override { if(  cPoints.first == nullptr &&  cPoints.second == nullptr ) return true; return false; }
       
    ShapeType   getType() const override { return ShapeType::Line;}
		
    PointPtrPair& getPoints() { return cPoints; }
    
    //    PointPtrPair  getInversedPoints(){ cPoints.PointPtrPair lPair( second(), first(); }
    PointPtr      getFirst()  const { return cPoints.first;}
    PointPtr      getSecond() const { return cPoints.second;}
    PointPtr      first()  const { return cPoints.first;}
    PointPtr      second() const { return cPoints.second;}
    PointPtr&     first() { return cPoints.first;}
    PointPtr&     second(){ return cPoints.second;}

    Point3d       getVector3d() { return (second()->get() - first()->get()); }

    Point3d   getCenter3d()  override { Point3d lRes = (second()->get() + first()->get()); lRes/=2; return lRes; };
     
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
  class  Facet :  public Entity{
		
    LinePtrVect cLines;

    Point3d cNorm;
    Point3d cNoNorm;
    bool    cIsConcave = false;

  public:
    Facet() {;}
    Facet(  LinePtr iLine ) { addLine( iLine ); }
    Facet(  LinePtr iLine1, LinePtr iLine2  ) { addLine( iLine1 ); addLine( iLine2 ); }
    Facet(  LinePtr iLine1, LinePtr iLine2, LinePtr iLine3  ) { addLine( iLine1 ); addLine( iLine2 );addLine( iLine3 ); }
    Facet(  LinePtr iLine1, LinePtr iLine2, LinePtr iLine3, LinePtr iLine4  ) { addLine( iLine1 ); addLine( iLine2 );addLine( iLine3 );addLine( iLine4 ); }

    bool isVoid() const override { return getNbLines()>0; }
      
    ShapeType getType() const  override { return ShapeType::Facet;}
	
    void closeFacet();
    
    bool isClose()
    {
      if( cLines.size() <= 1 )
	return false;
      
      return cLines[0]->first() == cLines[ cLines.size()-1]->second();
    }
    bool isClosable()
    {
      if( cLines.size() <= 1 )
	return false;
      
      return cLines[0]->first() != cLines[ cLines.size()-1]->second();
    }
   
    void computeNormal();
    
    void addPoint( const  Point3d & pPt );


    bool addTrueLine( LinePtr iLine )
    {
      if( iLine->isPoint())
	{
	  //	  std::cout << "addTrueLine - Line is point : same ID" << std::endl;
	  return false;
	}
      
      if( iLine->isSamePointXYZ() )
	{
	  //	  std::cout << "addTrueLine - Line is point : same XYZ" << std::endl;
	  return false;
	}	
      
      addLine( iLine );
      return true;
    }
    bool addTrueLineEpsi( long double iEpsi ,  LinePtr iLine)
    {
      if( iLine->isPoint())
	{
	  //	  std::cout << "addTrueLine - Line is point : same ID" << std::endl;
	  return false;
	}
      
      if( iLine->isSamePointEpsi( iEpsi) )
	{
	  //	  std::cout << "addTrueLine - Line is point : same XYZ" << std::endl;
	  return false;
	}	
      
      addLine( iLine );
      return true;
    }
    void addLine( LinePtr iLine )
    {
      cLines.push_back( iLine );
      iLine->addOwner( this );
    }
    void setLine( PIndex iPos, LinePtr iLine )
    {
      cLines[iPos] =  iLine ;
      iLine->addOwner( this );
    }
    void insertLine( PIndex iPos, LinePtr iLine )
    {
      cLines.insert( cLines.begin()+iPos, iLine );
      iLine->addOwner( this );
    }
    void addLineAfter( LinePtr iOriginal, LinePtr iNewLine )
    {
      size_t i =0;
      for( size_t i=0; i< cLines.size(); i++ )
	{
	  if( cLines[i] == iOriginal )
	    {
	      insertLine( i+1, iNewLine );
	      return;
	    }
	}
      addLine( iNewLine );
    }

    LinePtr      getLine(  PIndex iPos ) { return cLines[iPos]; }
    LinePtr      getLineModulo(  PIndex iPos ) { return cLines[iPos%cLines.size()]; }
    void         removeLineModulo( PIndex iPos ) { cLines.erase( cLines.begin()+(iPos%cLines.size())); }
    LinePtr      swapLine( PIndex iPos,  LinePtr iLine )
    {
      LinePtr lTmp =  cLines[iPos];
      iLine->addOwner( this );
      cLines[iPos] = iLine;
      return lTmp;
    }
    bool         clear() override { cLines.clear(); return Entity::clear(); }
    
    Facet*       duplicate() const;

    LinePtrVect& getLines()   { return cLines;}
    GLuint       getNbLines()  const { return (GLuint )cLines.size(); }
    PointPtr     getPoint( PIndex iPos );
    int          getPoints( VectDouble3 & oPts ) const;
    int          getPoints( VectPoint3d & oPts ) const;


    const Point3d &    getNormal()     { return cNorm; }

    void         insertPoint( PIndex iPos, Point3d & lPt, DataBase & iBase );
    void         insertPoint( PIndex iPos, PointPtr lPt, DataBase & iBase );
    bool         delPoint( PIndex iPos, DataBase & iBase );
    void         deleteAll();
    
    void         execVisitor( EntityVisitor& pVisit )override;
    void         inverseLines();
    Point3d      getCenter3d() override;
    Point3d      getNormal3d() override { return getNormal(); };

    bool         computeConcave();
    bool         isConcave() { return cIsConcave; }
    bool         testFlat();

    Poly* myPoly() { return (Poly*) firstOwner(); }
    
    friend std::ostream & operator << ( std::ostream & pOs, Facet& pEntity )
    {
      pOs << ((Entity&)pEntity) ;
      int i=0;
      for( LinePtr l: pEntity.getLines() ){ pOs << i++ << '>' << (*l) << " ";}
      return pOs;
    }

    
  protected:
    void execVisitor( EntityVisitorNode& pVisit )override;
    friend class Poly;
    friend class Maker;
     friend class VisitorNormalFacet;
  };
  using FacetPtr     = Facet*;
  using FacetPtrVect = std::vector<FacetPtr>;

  //*********************************************
  class  Poly :  public Entity{
		
    FacetPtrVect cFacets;
		
  public:
    bool isVoid() const override { return getNbFacets() >0; }
    ShapeType getType() const override { return ShapeType::Poly;}

    Point3d getCenter3d() override {
      Point3d lRes;
      for( FacetPtr lFac :  cFacets) 
	{
          lRes += lFac->getCenter3d();
        }
      if( cFacets.size()) lRes /= (double) cFacets.size(); 
      return lRes;
    } ;

    Point3d  getNormal3d() override;

    void addFacet( FacetPtr pFacet)
    {
      cFacets.push_back( pFacet );
      pFacet->addOwner( this );
    }

    void addFacet( const FacetPtrVect & iVectFacet)
    {
      for( FacetPtr lFacet : iVectFacet )
        addFacet( lFacet );
    }
    
    void removeFacet(  FacetPtr pFacet)
    {
      for( size_t i=0;  cFacets.size(); i++) 
	{
	  if( cFacets[i] == pFacet )
	    {
	      cFacets.erase( cFacets.begin() + i); 
	      pFacet->removeOwner(this);
	      break;
	    }
	}
    }
    
    void removeAllFacet()
    {
      for( FacetPtr lFacet :  cFacets ) 
	{
          lFacet->removeOwner(this);
	}
      cFacets.clear();
      Entity::clear();
    }
    
    virtual void remove( Entity* lEntity ) override
    {
      if( lEntity->getType() ==  ShapeType::Facet )
	{
	  removeFacet(  ((FacetPtr)lEntity) );
 	}
    }

    FacetPtrVect&  getFacets()  { return cFacets; }
    PIndex  getNbFacets() const { return cFacets.size(); }
    void execVisitor( EntityVisitor& pVisit )override;

    bool clear() override { cFacets.clear();  return Entity::clear(); }

  protected:
    void execVisitor( EntityVisitorNode& pVisit )override;
		
    friend class Object;
    friend class Maker;
  };
  using PolyPtr     = Poly*;
  using PolyPtrVect = std::vector<Poly*>;

  //*********************************************

  template <class MYCLASS>
  class Alloc{   
    static std::vector<MYCLASS> EntityHeap;
    
  public:
    void free( MYCLASS * iPtr, bool iFlagClearOwner = false )
      {
	if( iPtr == nullptr )
	  return;
	
	if( iFlagClearOwner )
	  {
	    iPtr->getOwners().clear();
	  }
	else
	  {
	  if( iPtr->getOwners().size() )
	    return ;
	  }

	
      }
  };
   //*********************************************
  
  inline EntityPtr GetOwnerWithType( EntityPtr iShape, ShapeType iSearchType ) {
    
    EntityPtr lOwner = iShape;
    for( int i = 0; i< 8; i++ )
      {
	if( lOwner == nullptr )
	  return nullptr;
	
	if(lOwner->getType() == iSearchType )
	  return lOwner;
	
 	lOwner = lOwner->firstOwner();
     }
    return nullptr;
  }

  //------------------------------------------------------
  inline PolyPtr GetOwnerPolyFromFacet( EntityPtr iShape) {
    if( iShape != nullptr && iShape->getType() ==  ShapeType::Facet )
      {
	EntityPtr lOwner =  iShape->firstOwner();
	if( lOwner != nullptr && lOwner->getType() == ShapeType::Poly ) 
	      {
		return (PolyPtr) lOwner;
	      }
      }
    return nullptr;
  }
 
  //------------------------------------------------------
  inline FacetPtr GetOwnerFacetFromLine( EntityPtr iShape) {
    if( iShape != nullptr && iShape->getType() ==  ShapeType::Line )
      {
	EntityPtr lOwner =  iShape->firstOwner();
	if( lOwner != nullptr && lOwner->getType() == ShapeType::Facet ) 
	      {
		return (FacetPtr) lOwner;
	      }
      }
    return nullptr;
  }

  // ATTENTION PLUSIEURS OWNERS !!!
  /*  inline LinePtr getOwnerLineFromPoint( EntityPtr iShape) {
    if( iShape != nullptr && iShape->getType() ==  ShapeType::Point )
      {
	EntityPtr lOwner =  iShape->firstOwner();
	if( lOwner != nullptr && lOwner->getType() == ShapeType::Line ) 
	      {
		return (LinePtr) lOwner;
	      }
      }
    return nullptr;
  }
  */
    
}

#endif
