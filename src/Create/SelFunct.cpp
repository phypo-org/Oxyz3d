#include "SelFunct.h"

#include <iostream>
#include <iomanip>

#include "Shape/OwnerVisitor.h"
#include "Shape/EntityHelp.h"


using namespace PP3d;
using namespace std;

//**************************************************
void SelFunct::SelectMore(  PP3d::Selection & iSel, PP3d::DataBase & iBase  )
{
  SortEntityVisitor lSelVisit;
  
  iSel.execVisitorOnEntity( lSelVisit );

  // cout <<  "+SelFunct::SelectMore\t" << lSelVisit << endl;


  // toutes les lignes ayant un point deja selectionné sont concerné
 
  OwnerEntityVisitor lOwner;
  lOwner.addOwnersOf( lSelVisit.cSetAllEntity );
  //cout <<  "+SelFunct::Owner\t" << lOwner << endl;

  SortEntityVisitor lAllVisit;

  for( ObjectPtr lObj : lOwner.cSetObjects )
    {
      lObj->execVisitor( lAllVisit );
    }
  // Pour toutes les lignes
  std::set<EntityPtr>     lSetPoints;
  std::set<EntityPtr>     lSetLines;
  std::set<EntityPtr>     lSetFacets;
    
  for( LinePtr lLine : lAllVisit.cSetLines )
    {       
      // Si un des points est deja dans la selection
      if( lSelVisit.cSetPoints.find( lLine->getFirst()) != lSelVisit.cSetPoints.end()
	  || lSelVisit.cSetPoints.find( lLine->getSecond()) != lSelVisit.cSetPoints.end())
	{	    
	  if( iSel.getSelectType() == SelectType::Point )
	    {
	      // Si le point n'est pas dans la selection
	      if( lSelVisit.cSetPoints.find( lLine->getFirst()) == lSelVisit.cSetPoints.end() )
		{
		  lSetPoints.insert( lLine->getFirst() );
		}
	      // Si le point n'est pas dans la selection
	      if( lSelVisit.cSetPoints.find( lLine->getSecond()) == lSelVisit.cSetPoints.end() )
		{
		  lSetPoints.insert(  lLine->getSecond() );
		}
	    }
	    
	  if( iSel.getSelectType() == SelectType::Line
	      && lSelVisit.cSetLines.find( lLine) == lSelVisit.cSetLines.end())//pas dans la selection
	    {
	      lSetLines.insert( lLine );
	    }

	  if(  iSel.getSelectType() == SelectType::Facet
	       && lSelVisit.cSetFacets.find((FacetPtr)lLine->firstOwner() ) == lSelVisit.cSetFacets.end())//pas dans la selection
	    {
	      lSetFacets.insert(  lLine->firstOwner() );
	    }
	}
    }
  if( iSel.getSelectType() == SelectType::Point )
    {
      iSel.addEntitys<std::set<EntityPtr>>( lSetPoints );	
    }
  if( iSel.getSelectType() == SelectType::Line )
    {
      iSel.addEntitys<std::set<EntityPtr>>( lSetLines );	
    }
  if(  iSel.getSelectType() == SelectType::Facet )	      
    {
      iSel.addEntitys<std::set<EntityPtr>>( lSetFacets );	
    }
  
}
//--------------------------------------------------
void SelFunct::SelectLess(  PP3d::Selection & iSel, PP3d::DataBase & iBase  )
{
  SortEntityVisitor lSelVisit;
  
  iSel.execVisitorOnEntity( lSelVisit );

  // cout <<  "-SelFunct::SelectLess\t" << lSelVisit << endl;


  // toutes les lignes ayant un point deja selectionné sont concerné
 
  OwnerEntityVisitor lOwner;
  lOwner.addOwnersOf( lSelVisit.cSetAllEntity );
  // cout <<  "-SelFunct::Owner\t" << lOwner << endl;

  SortEntityVisitor lAllVisit;

  for( ObjectPtr lObj : lOwner.cSetObjects )
    {
      lObj->execVisitor( lAllVisit );
    }
  // Pour toutes les lignes
  std::set<EntityPtr>     lSetPoints;
  std::set<EntityPtr>     lSetLines;
  std::set<EntityPtr>     lSetFacets;
    
  for( LinePtr lLine : lAllVisit.cSetLines )
    {       
      // On teste si les points sont  dans la selection
      bool lFirstSelect  = lSelVisit.cSetPoints.find( lLine->getFirst())  != lSelVisit.cSetPoints.end();
      bool lSecondSelect = lSelVisit.cSetPoints.find( lLine->getSecond()) != lSelVisit.cSetPoints.end();
	
      if( (lFirstSelect && lSecondSelect) ||  ( !lFirstSelect && !lSecondSelect))
	continue;

      // Un des deux est selectionné pas l'autre
      PointPtr lSelPt = lFirstSelect ? lLine->getFirst() : lLine->getSecond();
	  
      if( iSel.getSelectType() == SelectType::Point )
	{
	  lSetPoints.insert( lSelPt );	  
	}

      
      if( iSel.getSelectType() == SelectType::Line)
	{
	  for( EntityPtr lOwner : lSelPt->getOwners())
	    lSetLines.insert( lOwner );	  
	}
      
      
      if( iSel.getSelectType() == SelectType::Facet )
	{
	  for( EntityPtr lOwner : lSelPt->getOwners())
	    {
	      for( EntityPtr lOwner2 : lOwner->getOwners())     
		lSetFacets.insert( lOwner2 );
	    }
	}  
	
    }
  if( iSel.getSelectType() == SelectType::Point )
    {
      iSel.removeEntitys<std::set<EntityPtr>>( lSetPoints );	
    }
  if( iSel.getSelectType() == SelectType::Line )
    {
      iSel.removeEntitys<std::set<EntityPtr>>( lSetLines );	
    }
  if(  iSel.getSelectType() == SelectType::Facet )	      
    {
      iSel.removeEntitys<std::set<EntityPtr>>( lSetFacets );	
    }
}
//--------------------------------------------------
void
SelFunct::SelectEdgeLoop( PP3d::Selection & iSel, PP3d::DataBase & iBase, double iEpsilon  )
{  
  if( iSel.getSelectType() != SelectType::Line)   return;

  SortEntityVisitor lSelVisit;
  //  SortEntityVisitorLine lSelVisit;
  iSel.execVisitorOnEntity( lSelVisit );

  
  OwnerEntityVisitor lOwner;
  lOwner.addOwnersOf( lSelVisit.cSetAllEntity );
  //cout <<  "+SelFunct::Owner\t" << lOwner << endl;

  SortEntityVisitor lAllVisit;

  for( ObjectPtr lObj : lOwner.cSetObjects )
    {
      lObj->execVisitor( lAllVisit );
    }


  
  cout <<  "-SelFunct::SelectEdgeLoop\t" << iEpsilon << endl;

  std::set<EntityPtr>     lSetLines;
 
  for( LinePtr lLineSel : lSelVisit.cSetLines )
    {
      cout << "--------------" << lAllVisit.cSetLines.size() << endl;
      for( LinePtr lLine : lAllVisit.cSetLines )
	{
	  if( lLineSel == lLine )
	    continue;
	  
	  cout << std::dec  << "("<< lLine->getId() <<")" ;
	  
	  if( EntityHelp::AsCommonPoint(lLineSel, lLine ) )
	    {	      
	      double  lMax = 0;
	      size_t  lMaxPos=-1;
	      LinePtr lGood = nullptr;

	      cout << " + " ;

	      double lVal = lLineSel->getVector3d().dot( lLine->getVector3d() );
	      if( lVal < 0 ) { lVal = -lVal; }
	      
	      cout << lVal << "-" << (lVal >= iEpsilon) << "\t " ;
	      
	      if(  lVal >= iEpsilon  )
		{
		  lSetLines.insert( lLine );
		}
	    }
	  else {
	    cout << " - " ;
	  }
	}
      cout << endl;
  }
  cout <<  "---SelFunct::SelectEdgeLoop add selection  " << std::dec  << lSetLines.size()  << endl;
  
  iSel.addEntitys<std::set<EntityPtr>>( lSetLines);	  
}
//**************************************************
