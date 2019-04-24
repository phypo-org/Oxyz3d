#include "Selection.h"

namespace PP3d {

  // Memorize les coordonnees des points des entites selectionnes
  struct VisitorModifSelect : public EntityVisitor	{
	
    int                                   lCptSelect=0;
    
    virtual ~VisitorModifSelect() {;}
    //---------------------------------
    void execBeginLine ( Line* pEntity) override 
    {
      if( pEntity->isSelect())
	lCptSelect++;
    }
    //---------------------------------
    void execEndLine ( Line* pEntity) override
    {
      if( pEntity->isSelect() )	      
	lCptSelect--;
    }
    //---------------------------------		
    void execBeginFacet( Facet* pEntity ) override
    {
      if( pEntity->isSelect() )
	lCptSelect++;
    }
    //---------------------------------		
    void execEndFacet( Facet* pEntity) override
    {
      if( pEntity->isSelect() )
	lCptSelect--;
    }
    //---------------------------------			  
    void execBeginPoly ( Poly* pEntity ) override
    {	    
      if( pEntity->isSelect() )
	lCptSelect++;
    }	  
    //---------------------------------			  
    void execEndPoly ( Poly* pEntity ) override
    {	    
      if( pEntity->isSelect() )
	lCptSelect--;
    }	  
	  
    void execBeginObject( Object* pEntity) override
    {	    
      if( pEntity->isSelect() )
	lCptSelect++;
    }	  
    //---------------------------------			  
    void execEndObject( Object*pEntity ) override
    {	    
      if( pEntity->isSelect() )
	lCptSelect--;
    }
    void reset()
    {
      lCptSelect=0;
    }
  };
  //**************************
  struct VisitorModifPoints : public VisitorModifSelect
  {	
    std::unordered_map<EntityId, Point3d> lSavPt;
 
    enum class Mode{ SAV, MODIF, CANCEL };
    Mode  cMode = Mode::SAV; 
    double  cCoef=1;
    //---------------------------------
    VisitorModifPoints()
    {
    }    
    //---------------------------------
    void setCoef( double iCoef )
    {
      cCoef = iCoef;
    }   
    //---------------------------------			  
    virtual void execPoint( Point* pPoint )
    {
      if( cMode == Mode::SAV )
	{
	  if( pPoint->isSelect() || lCptSelect > 0 )
	    {
	      lSavPt.insert( { pPoint->getId(), pPoint->get() } ); 
	    }
	}
      else
	{ // mode restore
	  auto lIter = lSavPt.find( pPoint->getId() );
	  if(  lIter != lSavPt.end() )
	    {
	      pPoint->get() =  lIter->second;
	    }
	}
    }	  
    //---------------------------------		
    // mode SAV en premier -> sauvegarde les points
    // mode MODIF ensuite avec coef qui varie pour faire bouger
    // si CANCEL -> restore seul

    void modifSelection( Mode iMode)
    {
      cMode = iMode;
      
      for( EntityPtr lEntity : Selection::Instance().getSelection() )
	{	
	  lEntity->execVisitor( *this );	  	
	}           
    }
  };
  //**************************
  struct VisitorMoveNormal : public  VisitorModifPoints
  {
    Point3d cNorm;

    //---------------------------------			  
    VisitorMoveNormal()
    {
    }
    //---------------------------------			  
    virtual void execPoint( Point* pPoint )
    {
      VisitorModifPoints::execPoint( pPoint ); 
      if( cMode == Mode::MODIF )
	{
	  std::cout << "execPoint:" << pPoint->get() << std::endl;
	  pPoint->get() += cNorm*cCoef; // si plusieurs facette les modifs s'additionnent
	}
    }
    //---------------------------------		
    void execBeginFacet( Facet* pEntity ) override
    {
      VisitorModifPoints::execBeginFacet(pEntity); // sav/restore
      if( cMode == Mode::MODIF  )
	cNorm = pEntity->getNormal();
    }
    //---------------------------------		
    void execEndFacet( Facet* pEntity) override
    {
      VisitorModifPoints::execEndFacet(pEntity); // sav/restore
      if( cMode == Mode::MODIF )	
	cNorm.zero();      
    }
 
  };
  //*************************************
}
