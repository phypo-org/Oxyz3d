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
 
    std::unordered_set<Point*> cModifPt;
    
    enum class Mode{ SAV, MODIF, CANCEL };
    Mode    cMode = Mode::SAV; 
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
      switch( cMode )
	{
	case Mode::SAV :
	  {
	    if( pPoint->isSelect() || lCptSelect > 0 )
	      {
		lSavPt.insert( { pPoint->getId(), pPoint->get() } ); 
	      }
	  }
	  break;
	  
	case Mode::MODIF:
	  if( pPoint->isSelect() || lCptSelect > 0 )
	    {
	      cModifPt.emplace( pPoint );
	    }
	  break;
	  
	case Mode::CANCEL:
	  { // mode restore
	    auto lIter = lSavPt.find( pPoint->getId() );
	    if(  lIter != lSavPt.end() )
	      {
		pPoint->get() =  lIter->second;
	      }
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
    //    Point3d cNorm;

    //---------------------------------			  
    VisitorMoveNormal()
    {
    }
    //---------------------------------		
    void execEndFacet( Facet* pEntity) override
    {
      VisitorModifPoints::execEndFacet(pEntity); 

      if( cMode == Mode::MODIF )
	{
	  Point3d cNorm = pEntity->getNormal();
	  for( Point* lPt:cModifPt )
	    {
	      lPt->get() += cNorm*cCoef; // si plusieurs facette les modifs s'additionnent	      
	    }
	  cModifPt.clear(); 
	}  
      
   }
 
  };
  //*************************************
}
