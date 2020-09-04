#ifndef H__VISITOR_MODIF__H
#define H__VISITOR_MODIF__H

#include "Shape/Selection.h"


namespace PP3d {

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

// ATTENTION si un point appartient a plusieurs facettes, les transformations s'additionnent
// Ce n'est pas forcement un effet voulu !!!
// Comment faire autrement ?
// compter precocement le nombre de owners actifs pour chaque points, stocker le nombre et
// lors des transformation diviser la transformation par ce nombre ?

  struct VisitorModifPoints : public VisitorModifSelect
  {	
   // Memorize les coordonnees des points des entites selectionnes
    
    std::unordered_map<EntityId, Point3d> lSavPt;
    std::unordered_map<EntityId, int> lNbOwner;

    std::unordered_set<Point*>    cModifPt;    
    std::unordered_set<EntityPtr> cOwners;
    
    
    enum class Mode{ SAV, MODIF, CANCEL };
    Mode    cMode = Mode::SAV; 
    double  cCoef=1;
    
    //---------------------------------
    void setOwner( EntityPtr iEntity )
    {
      if( iEntity->getType() >= ShapeType::Facet )
	{
	  cOwners.emplace( iEntity );
	}     
      else
	for( EntityPtr lEntity : iEntity->getOwners() )
	  {
	    setOwner( lEntity );
	  }       
    }
    //---------------------------------
    VisitorModifPoints( PP3d::Selection & iSelect )
    {
      for( EntityPtr lEntity : iSelect.getSelection() )
	{
	  setOwner( lEntity );
	}  
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
		
		auto lIter = lNbOwner.find( pPoint->getId() );
		if(  lIter != lNbOwner.end() )
		  {
		    lIter->second++;
		  }
		else
		  {
		    lNbOwner.insert(  { pPoint->getId(), 1 } );
		  }
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

    void modifSelection( Mode iMode, PP3d::Selection & iSelect)
    {
      cMode = iMode;
      std::cout << "========  modifSelection " << iSelect.getSelection().size()  <<std::endl;

      /*
      for( EntityPtr lEntity : Selection::Instance().getSelection() )
	{	
      std::cout << "========  modifSelection execVisitor1"  <<std::endl;
	  lEntity->execVisitor( *this );	  	
      std::cout << "========  modifSelection exelEntity != nullptr &&cVis		r2"  <<std::endl;
	} 	}
      */
    
      for( EntityPtr lEntity : cOwners )
	{	
      std::cout << "========  modif execVisitor1"  <<std::endl;
	  lEntity->execVisitor( *this );	  	
      std::cout << "========  modif execVisitor2"  <<std::endl;
	}
    }
  };
  //**************************
  struct VisitorMoveNormal : public  VisitorModifPoints
  {
    //---------------------------------			  
    VisitorMoveNormal( PP3d::Selection & iSelect)
      :VisitorModifPoints( iSelect )
    {

    }
    //---------------------------------		
    void execEndFacet( Facet* pEntity) override
    {
      // 	  std::cout << " cModifPt00000000222:" << cModifPt.size() << std::endl;
     VisitorModifPoints::execEndFacet(pEntity); 
     //	  std::cout << " cModifPt222:" << cModifPt.size() << std::endl;

      if( cMode == Mode::MODIF )
	{
	  //	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  
	  Point3d cNorm = pEntity->getNormal();
	  for( Point* lPt:cModifPt )
	    {
	      double lNb=1;
	      auto lIter = lNbOwner.find( lPt->getId() );
	      if(  lIter != lNbOwner.end() )
		lNb = lIter->second;

	      lPt->get() += (cNorm*cCoef)/lNb; // si plusieurs facette les modifs s'additionnent !!!
	    }
	  cModifPt.clear(); 
	}        
   }
 
  };
  //**************************
  struct VisitorScaleNormal : public  VisitorModifPoints
  {
    //---------------------------------			  
    VisitorScaleNormal( PP3d::Selection & iSelect)
      :VisitorModifPoints( iSelect )
    {

    }
    //---------------------------------		
    void execEndFacet( Facet* pEntity) override
    {
      //      std::cout << " cModifPt00000000222:" << cModifPt.size() << std::endl;
      VisitorModifPoints::execEndFacet(pEntity); 
      //      std::cout << " cModifPt222:" << cModifPt.size() << std::endl;

      if( cMode == Mode::MODIF )
	{
	  std::cout << " cScalePt:" << cModifPt.size() << std::endl;
	  
	  //	  Point3d cNorm = pEntity->getNormal();
	  Point3d cCenter = pEntity->getCenter();
	  
	  for( Point* lPt:cModifPt )
	    {
	      double lNb=1;
	      auto lIter = lNbOwner.find( lPt->getId() );
	      if(  lIter != lNbOwner.end() )
		lNb = lIter->second;


	      Point3d lVect = lPt->get() - cCenter;
	      lVect.normalize(cCoef);	      
	      
	      lPt->get() += lVect/lNb;	      
	    }
	  cModifPt.clear(); 
	}        
   }
 
  };
  //*************************************
  //**************************
  struct VisitorRotNormal : public  VisitorModifPoints
  {
    //---------------------------------			  
    VisitorRotNormal( PP3d::Selection & iSelect)
      :VisitorModifPoints( iSelect )
    {

    }
    //---------------------------------		
    void execEndFacet( Facet* pEntity) override
    {
      //      std::cout << " cModifPt00000000222:" << cModifPt.size() << std::endl;
      VisitorModifPoints::execEndFacet(pEntity); 
      //      std::cout << " cModifPt222:" << cModifPt.size() << std::endl;

      if( cMode == Mode::MODIF )
	{
	  std::cout << " cScalePt:" << cModifPt.size() << std::endl;
	  
	  //	  Point3d cNorm = pEntity->getNormal();
	  Point3d lCenter = pEntity->getCenter();
	  Point3d lNorm = pEntity->getNormal();

	  PP3d::Mat4 lMatRecenter;
	  lMatRecenter.initMove( lCenter ); //on revient au centre de la facette;
	  PP3d::Point3d lNCenter =  -lCenter;					
	  PP3d::Mat4 lMatZero;
	  lMatZero.initMove( lNCenter ); //on se positionne en zero;



	  for( Point* lPt:cModifPt )
	    {
	      double lNb=1;
	      auto lIter = lNbOwner.find( lPt->getId() );
	      if(  lIter != lNbOwner.end() )
		lNb = lIter->second;
	      
	      PP3d::Mat4 lMatRot;	  
	      lMatRot.initRotAxis( lNorm, cCoef/lNb );

	      PP3d::Mat4 lMatTran;
	      lMatTran.Identity();
	      lMatTran = lMatRecenter * lMatRot *  lMatZero;					
      	      
	      lPt->get()  = lPt->get() * lMatTran;	      
	    }
	  cModifPt.clear(); 
	}        
   }
 
  };
  //*************************************
} // namespace

#endif
