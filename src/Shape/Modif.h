#include "Selection.h"

namespace PP3d {

  struct VisitorModifSelect : public EntityVisitor
  {	
    int    lCptSelect=0;
    
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
    //---------------------------------			  
    void reset()
    {
      lCptSelect=0;
    }
  };
  //****************************************************
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
	  // on sauve les points qui font partie de la selection pour pouvoir les restorer
	  {
	    if( pPoint->isSelect() || lCptSelect > 0 )
	      {
		lSavPt.insert( { pPoint->getId(), pPoint->get() } ); 
	      }
	  }
	  break;
	  
	case Mode::MODIF:
	  // on stockes les points qui font partie de la selection 
	  cModifPt.emplace( pPoint );
	  break;
	  
	case Mode::CANCEL:
	  { // on restore les points a leur etat d'origine
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

    virtual void modifSelection( Mode iMode)=0;
    
  };
  //**************************************************
  struct VisitorFacetMoveNormal : public  VisitorModifPoints
  {
    //---------------------------------		
    void execBeginFacet( Facet* pEntity) override
    {
      VisitorModifPoints::execBeginFacet(pEntity); 
      cModifPt.clear(); // on ne prend que ceux de la facette
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
	}        
    }
    //---------------------------------		
    virtual void modifSelection( Mode iMode)
    {
      cMode = iMode;    
      
      for( EntityPtr lEntity : Selection::Instance().getSelection() )
	{	
	  lEntity->execVisitor( *this );	  	
	}           
    }
  };
  //****************************************************
  struct VisitorPointLineMoveNormal : public  VisitorModifPoints
  {    
    // Il faut retrouver les facettes des Entity selectionnées pour les normales
    std::unordered_set<FacetPtr>    cFacets;    
    //---------------------------------
    void addFacets( EntityPtr iEntity )
    {
      if( iEntity->getType() == ShapeType::Facet )
	{
	  cFacets.emplace( (FacetPtr)iEntity );  // ne stocke pas les doublons
	  return; // et on s'arrete la !
	}     
      
      for( EntityPtr lEntity : iEntity->getOwners() )
	{
	  addFacets( lEntity );
	}       
    }
    //---------------------------------
    void modifSelection( Mode iMode)
    {
      cMode = iMode;
          
      if( iMode == Mode::SAV || iMode== Mode::CANCEL )
	{
	  for( EntityPtr lEntity : Selection::Instance().getSelection() )
	    {	
	      lEntity->execVisitor( *this );	  	
	    }
	}
      
      if( iMode == Mode::MODIF )
	{
	  for( EntityPtr lEntity : Selection::Instance().getSelection() )
	    {
	      cFacets.clear();
	      addFacets( lEntity ); // on recupere toutes les facettes auquel appartient le point ou la ligne
	      for( FacetPtr lFacet: cFacets )
		{
		  lFacet->execVisitor( *this );	  	
		}
	    }
	  
	}
    }
 
    //---------------------------------		
    void execBeginFacet( Facet* pEntity) override
    {
      //      VisitorModifPoints::execBeginFacet(pEntity);      
      cModifPt.clear(); // on ne prend que ceux de la facette
    }
    //---------------------------------		
    void execEndFacet( Facet* pEntity) override
    {
      //   VisitorModifPoints::execEndFacet(pEntity);  
      
      if( cMode == Mode::MODIF )
	{
	  Point3d cNorm = pEntity->getNormal();
	  for( Point* lPt:cModifPt )
	    {
	      lPt->get() += cNorm*cCoef; // si plusieurs facette les modifs s'additionnent	      
	    }
	}        
    } 
  };
  //*************************************
}
