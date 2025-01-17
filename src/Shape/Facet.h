#ifndef H__Facet__H
#define H__Facet__H


#include "Line.h"

namespace PP3d {

  //*********************************************
  class  Facet :  public Entity{
		
    LinePtrVect cLines;

    Point3d cNorm;
    Point3d cNoNorm;
    bool    cIsConcave = false;

  public:
    Facet() {;}
    Facet(  LinePtr iLine )                   { addLine( iLine ); }
    Facet(  LinePtr iLine1, LinePtr iLine2  ) { addLine( iLine1 ); addLine( iLine2 ); }
    Facet(  LinePtr iLine1, LinePtr iLine2, LinePtr iLine3  ) { addLine( iLine1 ); addLine( iLine2 );addLine( iLine3 ); }
    Facet(  LinePtr iLine1, LinePtr iLine2, LinePtr iLine3, LinePtr iLine4  ) { addLine( iLine1 ); addLine( iLine2 );addLine( iLine3 );addLine( iLine4 ); }

    bool isVoid() const override        { return getNbLines()>0; }      
    ShapeType getType() const  override { return ShapeType::Facet;}
	
    void closeFacet();
    
    bool isClose() const
    {
      if( cLines.size() <= 1 )
        return false;
      
      return cLines[0]->first() == cLines[ cLines.size()-1]->second();
    }
    bool isClosable() const
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
    void addLines( LinePtrVect & iLines ) { for( LinePtr lLine: iLines ) addLine( lLine ); }
   
    LinePtr      getLine(  PIndex iPos )         { return cLines[iPos]; }
    LinePtr      getLineModulo(  PIndex iPos )   { return cLines[iPos%cLines.size()]; }
    void         removeLineModulo( PIndex iPos ) { cLines.erase( cLines.begin()+(iPos%cLines.size())); }
    LinePtr      swapLine( PIndex iPos,  LinePtr iLine )
    {
      LinePtr lTmp =  cLines[iPos];
      iLine->addOwner( this );
      cLines[iPos] = iLine;
      return lTmp;
    }
    bool         clear() override  { cLines.clear(); return Entity::clear(); }
    
    Facet*       duplicate() const; // Beware of close

    LinePtrVect& getLines()   { return cLines;}
    GLuint       getNbLines()  const { return (GLuint )cLines.size(); }
    PointPtr     getPoint( PIndex iPos );
    
    int          getCopyPointsWithDuplicate( VectDouble3 & oPts ) const;
    int          getCopyPointsWithDuplicate( VectPoint3d & oPts ) const;
    
    void         removeLine( LinePtr iLine ) // phipo 20240124
    {
      for (auto it = cLines.begin(); it != cLines.end(); it++)
        {
          if( *it == iLine )
            {
              cLines.erase(it);
              return;
            }
        }
    }


    virtual void remove( Entity* lEntity, bool iFlagRmOwner ) override // phipo 20240124
    {
      if( lEntity->getType() ==  ShapeType::Line )
        {
          removeLine(  ((LinePtr)lEntity) );
          Entity::remove( lEntity, iFlagRmOwner );
        }
    }

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

    Poly* myPoly()
    {
      OwnerPtr lOwner= firstOwner();
      if( lOwner->getType() == ShapeType::Poly )
        return (Poly*)lOwner;
      return nullptr;
    }
    
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

}

#endif
