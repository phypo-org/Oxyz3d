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
     void execPoint( Point* pPoint ) override
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
      std::cout << "========  modifSelection " << Selection::Instance().getSelection().size()  <<std::endl;
      
      for( EntityPtr lEntity : Selection::Instance().getSelection() )
	{	
      std::cout << "========  modifSelection execVisitor1"  <<std::endl;
	  lEntity->execVisitor( *this );	  	
      std::cout << "========  modifSelection execVisitor2"  <<std::endl;
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
 	  std::cout << " cModifPt00000000222:" << cModifPt.size() << std::endl;
     VisitorModifPoints::execEndFacet(pEntity); 
	  std::cout << " cModifPt222:" << cModifPt.size() << std::endl;
	  std::cout << " cModifPt222:" << cModifPt.size() << std::endl;

      if( cMode == Mode::MODIF )
	{
	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  
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
