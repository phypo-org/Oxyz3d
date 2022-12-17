#include "SavRead.h"

#include "DataBase.h"
#include "Selection.h"

#include "ObjectLine.h"
#include "ObjectFacet.h"
#include "ObjectPolylines.h"
#include "ObjectPoly.h"

/*
const char* TokPoint="Point:";
const char* TokLine="Line:";
const char* TokFacet="Facet:";
const char* TokPoly="Poly:";
const char* TokObject="Object:";

const char* TokSaisiePt="SaisiePt:";

const char* TokSelection="Selection:";
*/

const char* TokPoint="p:";
const char* TokLine="L:";
const char* TokFacet="F:";
const char* TokPoly="P:";
const char* TokObject="O:";

const char* TokSaisiePt="Saisie:";

const char* TokSelection="Sel:";


bool sDebugSav = false;

#define SAVCOUT if( sDebugSav ) std::cout

namespace PP3d {
  //*************************************
  bool MySav::save( DataBase& pData, Selection * iSel, std::set<Entity*> * iFilter)
  {
    auto lEntities = pData.getEntities();

    //	for( auto lPairEntity :  lEntities )
    //		{
    //			SAVCOUT<< " - " <<  lPairEntity.first << " " << lPairEntity.second->getId() << " " << lPairEntity.second->getType() << std::endl;
    //	}		
    //=======================
    //	cOut << TokPoint << std::endl;

    for( auto lPairEntity :  lEntities )
      {	
	if( lPairEntity.second->getType() != ShapeType::Point )
	  continue;
	
	if( iFilter != nullptr && iFilter->find( lPairEntity.second ) == iFilter->end())
	  continue;

	auto lEntity = dynamic_cast<PointPtr>(lPairEntity.second);

	cOut << TokPoint 
	     << ' ' << lEntity->getId()
	     << ' ' << lEntity->get().cX
	     << ' ' << lEntity->get().cY
	     << ' ' << lEntity->get().cZ
	     << std::endl;				
      }
    cOut << std::endl;

    //=======================
    //		cOut << TokLine << std::endl;

    for( auto lPairEntity :  lEntities )
      {				
	if( lPairEntity.second->getType() != ShapeType::Line )
	  continue;
	
	if( iFilter != nullptr && iFilter->find( lPairEntity.second ) == iFilter->end())
	  continue;

	auto lEntity = dynamic_cast<LinePtr>(lPairEntity.second);
	cOut << TokLine 
	     << ' ' << lEntity->getId()
	     << ' ' << lEntity->first()->getId()
	     << ' ' << lEntity->second()->getId()			
	     << std::endl;				
      }
    cOut << std::endl;

    //=======================		
    //		cOut << TokFacet << std::endl;

    for( auto lPairEntity : lEntities )
      {				
	if( lPairEntity.second->getType() != ShapeType::Facet )
	  continue;

	if( iFilter != nullptr && iFilter->find( lPairEntity.second ) == iFilter->end())
	  continue;
	
	auto lEntity = dynamic_cast<FacetPtr>(lPairEntity.second);
	cOut << TokFacet
	     << ' ' << lEntity->getId()
	     << ' ' << lEntity->getLines().size();

	for( auto lLine : lEntity->getLines() )
	  {
	    cOut << ' ' << lLine->getId(); 
	  }					
	cOut	 << std::endl;				
      }
    cOut << std::endl;

    //=======================		
    //	cOut << TokPoly << std::endl;

    for( auto lPairEntity :  lEntities )
      {				
	if( lPairEntity.second->getType() != ShapeType::Poly )
	  continue;
	
	if( iFilter != nullptr && iFilter->find( lPairEntity.second ) == iFilter->end())
	  continue;

	auto lEntity = dynamic_cast<PolyPtr>(lPairEntity.second);
	cOut << TokPoly
	     << ' ' << lEntity->getId()
	     << ' ' << lEntity->getFacets().size();
				
	for( auto lFacet : lEntity->getFacets() )
	  {
	    cOut << ' ' << lFacet->getId();
	  }					
	cOut << std::endl;				

      }
    cOut << std::endl;

    //=======================
		
    //	cOut << TokObject << std::endl;

    for( auto lPairEntity :  lEntities )
      {				
	if( lPairEntity.second->getType() != ShapeType::Object )
	  continue;

	if( iFilter != nullptr && iFilter->find( lPairEntity.second ) == iFilter->end())
	  continue;

	auto lEntity = dynamic_cast<ObjectPtr>(lPairEntity.second);
	cOut  << TokObject
	      << ' '  << lEntity->getId()
	      << ' '  << lEntity->getObjType()
	      << ' '  << lEntity->getShape()->getId() 
	      << " @"  << lEntity->getName()   // pour eviter de recuperer l'espace devant le nom
	      << std::endl;				

      }
    cOut << std::endl;

    
    FacetPtr lSaiseFacet = pData.getInput().getCurrentLine();
    
    if( lSaiseFacet != nullptr )
      {
	size_t lNbLine = lSaiseFacet->getLines().size();
	cOut << TokSaisiePt << ' ' << lNbLine;
		
	for( size_t l=0; l< lNbLine; l++ )
	  {
	    LinePtr lLine = lSaiseFacet->getLines()[l];
	    
	    cOut << ' ' << lLine->first()->get().cX
		 << ' ' << lLine->first()->get().cY
		 << ' ' << lLine->first()->get().cZ;
	    if( l == lNbLine-1
		&& lLine->isPoint() == false )
	      {
		cOut << ' ' << lLine->second()->get().cX
		     << ' ' << lLine->second()->get().cY
		     << ' ' << lLine->second()->get().cZ;
	      }
	      
	  }	    
	cOut << std::endl;	    
      }

    if( iSel )
      {
	cOut << TokSelection << ' ' 
	     <<	Selection::GetStrSelectType( iSel->getSelectType() )
	     << ' ' << iSel->getNbSelected();
	
	for( auto lEntity : iSel->getSelectionVect() )
	  {
	    cOut << ' ' << lEntity->getId();
	  }					
	cOut << std::endl;	    
      }
    
    return true;
  }
	
  //*************************************

#define ReadEndLine		std::getline(	cIn,  lEndOfLine, '\n' )

  //--------------------------------
  bool MyRead::read( DataBase& pData, Selection * ioSel, bool pConserveOldId  )
  {
    try {
      std::unordered_map<EntityId, Entity*> lLocalDico;
				
      std::string lEndOfLine; // pour lire la fin de ligne 
				
      while( cIn.good() )
	{
	  std::string lToken;
	  cIn >> lToken;
	  //				std::getline( cIn, lToken, ' ' );
				
	  SAVCOUT<< "Token: <<" << lToken << ">> ";
						
	  EntityId lId;
	  if( lToken == TokPoint )
	    {
	      PDouble  lX,lY,lZ;
						
	      cIn >> lId >> lX >> lY >> lZ;
					 
	      SAVCOUT<< " - Read Point<<" << lId <<" : " << lX << " : " << lY << " : " << lZ <<  std::endl;
						
	      Point* lPoint = new Point( Point3d( lX, lY, lZ ) );
	      
	      if( pConserveOldId )
		{
		  lPoint->setId( lId );
		  pData.addValidEntityForUndo(lPoint);
		}

	      lLocalDico.insert( { lId, lPoint } );
								
	      ReadEndLine;
	    }
	  else
	    if( lToken == TokLine )
	      {
		EntityId lFirstId, lSecondId;
									
		cIn >> lId >> lFirstId >> lSecondId ;	
		SAVCOUT<< " - Read Line <<" << lId <<" : " << lFirstId<< " : " << lSecondId <<  std::endl;
							
		Line* lLine = new Line( static_cast<PointPtr>(lLocalDico.at(lFirstId)),
					static_cast<PointPtr>(lLocalDico.at(lSecondId)) );
		
		if( pConserveOldId )
		  {
		    lLine->setId( lId );
		    pData.addValidEntityForUndo(lLine);
		  }
		
		lLocalDico.insert( { lId, lLine } ); 

		ReadEndLine;
	      }
	    else
	      if( lToken == TokFacet )
		{
		  size_t lNb;
		  EntityId lLineId;
												
		  cIn >> lId >> lNb  ;
		  SAVCOUT<< " - Read Facet <<" << lId <<" : " << lNb <<  std::endl;
		  Facet *lFacet = new Facet();
												
		  for( size_t i=0; i< lNb; i++)
		    {
		      cIn >> lLineId ;
		      lFacet->addLine( static_cast<LinePtr>(lLocalDico.at(lLineId)) );
														
		      SAVCOUT<< "- Read  "  << i << ":" << lLineId <<  std::endl;														
		    }
		  if( pConserveOldId )
		    {
		      lFacet->setId( lId );
		      pData.addValidEntityForUndo( lFacet );
		    }
		  
		  lLocalDico.insert( { lId, lFacet } );
												
		  ReadEndLine;
		}
	      else
		if( lToken == TokPoly )
		  {
		    size_t lNb;
		    EntityId lFacetId;
													
		    cIn >> lId >> lNb  ;
		    SAVCOUT<< " - Read Poly <<" << lId <<" : " << lNb <<  std::endl;
													
		    Poly* lPoly = new Poly();
													
		    for( size_t i=0; i< lNb; i++)
		      {
			cIn >> lFacetId;

			SAVCOUT<< " - Read   " <<  i << ":" << lFacetId <<  std::endl;
			lPoly->addFacet( static_cast<FacetPtr>(lLocalDico.at(lFacetId)));

			if( pConserveOldId )
			  {
			    lPoly->setId( lId );
			    pData.addValidEntityForUndo(lPoly);
			  }
			
			lLocalDico.insert( { lId, lPoly } );
		      }
		    ReadEndLine;
		  }
	      else
		if( lToken == TokSaisiePt )
		  {
		    size_t lNb;
		    cIn >> lNb;
		    
		    for( size_t i=0; i< lNb; i++)
		      {
			PDouble  lX,lY,lZ;						
			cIn >> lX >> lY >> lZ;
			pData.getInput().addPointToCurrentLine( Point3d( lX, lY, lZ ) );
		      }
		  }
		else
		  if( lToken == TokObject )
		    {
		      std::string lSubType;
		      EntityId    lSubId;
		      std::string lName;

		      cIn >> lId  >> lSubType >> lSubId  ;
		      std::getline(	cIn,  lName, '@' ); // pour eviter de recuperer l'espace devant le nom
		      std::getline(	cIn,  lName, '\n' );

		      
		      SAVCOUT<< " - Read object <<" << lId <<" : " << lSubType << " : " <<  lSubId  << " <<" << lName << ">>" <<  std::endl;

		      
		      Object* lObj=nullptr;

		      
		      switch( GetObjectTypeFromStr( lSubType.c_str() ))
			{
			case ObjectType::ObjPoint: break;
			case ObjectType::ObjLine:
			  lObj = new ObjectLine( lName, static_cast<LinePtr>(lLocalDico.at(lSubId)));
			  break;
			case ObjectType::ObjFacet:;
			  lObj = new ObjectFacet( lName, static_cast<FacetPtr>(lLocalDico.at(lSubId)));
			  break;
			case ObjectType::ObjPoly:  ;
			  lObj = new ObjectPoly( lName, static_cast<PolyPtr>(lLocalDico.at(lSubId)));
			  break;
			case ObjectType::ObjPolyline:;
			  lObj = new ObjectPolylines( lName, static_cast<FacetPtr>(lLocalDico.at(lSubId)));
			  break;
			case ObjectType::ObjNull: break;								
			}


		      if( lObj != nullptr )
			{
			  if( pConserveOldId )
			    {
			      lObj->setId( lId );
			      pData.addValidEntityForUndo(lObj);
			    }
			  
			  SAVCOUT<< "************** pData.addObject *******************" << std::endl;
			  pData.addObject( lObj );
			  if( cCreateResult != nullptr )
			    {
			      cCreateResult->push_back( lObj );
			    }
			  SAVCOUT<< "************** pData.addObject 22222 *******************" << std::endl;
			}
		    }
		  else		    
		    if( lToken == TokSelection )
		      {
			std::string lStrTypeSelect;
			cIn >> lStrTypeSelect;

			SelectType lSelType = Selection::GetSelectTypeFromStr( lStrTypeSelect.c_str() );
		
			if( ioSel )
			{
			  ioSel->changeSelectType( lSelType );
			}

			size_t lNb;
			cIn >>  lNb  ;
			
			//	std::cout << "Selection " << lStrTypeSelect << " " << lSelType
			//		  << " " << lNb << std::endl;
			
			for( size_t i=0; i< lNb; i++)
			  {			    
			    EntityId lId;
			    
			    cIn >> lId ;
			    
			    if( ioSel )
			      {
				//	std::cout << "Sel " << lId ;
	
				EntityPtr lEntity = lLocalDico.at(lId);
				if( lEntity != nullptr )
				  {
				    ioSel->addEntity( lEntity, true );
				    //	    std::cout << " ok " <<  ioSel->getNbSelected()  << std::endl;
				  }
				//	else
				//	  std::cout << " ko " << std::endl;

			      }
			  }
			
		      }	  
		    else
		    {
		      SAVCOUT<< " Unknown Token :" << lToken << std::endl;
		    }
	}
    }	catch( const std::exception & lEx ) {				
      std::cerr << __FILE__ << ":" << __LINE__ << ":Exception " << lEx.what() << std::endl;
    }
					
    return true;
  }	
  //*************************************

}
