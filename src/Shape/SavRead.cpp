#include "SavRead.h"

#include "DataBase.h"
#include "Selection.h"

#include "ObjectLine.h"
#include "ObjectFacet.h"
#include "ObjectPolylines.h"
#include "ObjectPoly.h"

#include "Utils/PPContainer.h"

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

const char* TokGroups="Groups:";
const char* TokGroup="G:";

const char* TokSaisiePt="Saisie:";

const char* TokSelection="Sel:";


bool sDebugSav = false;

#define SAVCOUT if( sDebugSav ) std::cout

namespace PP3d {
  //*************************************
  bool MySav::save( DataBase& pData,  bool iFlagSavGroup, Selection * iSel, std::set<Entity*> * iFilter )
  {
    const std::unordered_map<EntityId, Entity*>& lEntities = pData.getEntities();

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



    if( iFlagSavGroup && pData.getGroups().size() )
      {
	cOut << TokGroups<< ' ' <<  pData.getGroups().size()<< std::endl;	    

        for( const GroupPtr lGroup: pData.getGroups())
          {
            cOut<< TokGroup << ' ' << lGroup->getGroupId() << ' ' << lGroup->values().size();
            for( const ObjectPtr lObj : lGroup->values() )
              {
                cOut  << ' ' << lObj->getId();
              }
            cOut << std::endl;
          }        
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
    cOut << std::endl;

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
      // std::unordered_map<EntityId, Entity*> lLocalDico;
      PPu::HashMapPtr<EntityId,Entity> lLocalDico;
				
      std::string lEndOfLine; // pour lire la fin de ligne
      
      //=============================		
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

	      lLocalDico.insertObj(  lId, lPoint );
								
	      ReadEndLine;
	    }
	  else
	    if( lToken == TokLine )
	      {
		EntityId lFirstId, lSecondId;
									
		cIn >> lId >> lFirstId >> lSecondId ;	
		SAVCOUT<< " - Read Line <<" << lId <<" : " << lFirstId<< " : " << lSecondId <<  std::endl;

                EntityPtr lFirst  = lLocalDico.findObj( lFirstId ); 
                EntityPtr lSecond = lLocalDico.findObj( lSecondId ); 

                if( lFirst && lSecond )
                  {
                    Line* lLine = new Line( dynamic_cast<PointPtr>(lFirst ),
                                            dynamic_cast<PointPtr>(lSecond));
		
                    if( pConserveOldId )
                      {
                        lLine->setId( lId );
                        pData.addValidEntityForUndo(lLine);
                      }
                    
                    lLocalDico.insertObj( lId, lLine  ); 
                  }
                else {
                  std::cerr << "Error Line : point not found :" << lFirstId << " or " << lSecondId <<  std::endl;
                }
                
                ReadEndLine;
	      }
	    else
	      if( lToken == TokFacet )
		{
		  size_t lNb;
		  EntityId lLineId;
												
		  cIn >> lId >> lNb  ;
		  SAVCOUT<< " - Read Facet <<" << lId <<" : " << lNb <<  std::endl;
		  FacetPtr lFacet = new Facet();
												
		  for( size_t i=0; i< lNb; i++)
		    {
		      cIn >> lLineId ;
                      EntityPtr lLinePtr = lLocalDico.findObj(lLineId );
                      if( lLinePtr )
                        {
                          lFacet->addLine( dynamic_cast<LinePtr>( lLinePtr ) );
														
                          SAVCOUT<< "- Read  "  << i << ":" << lLineId <<  std::endl;							}
                      else
                        {
                          std::cerr << "Error Facet : line not found:" << lLineId << std::endl;
                        }
		    }
		  if( pConserveOldId )
		    {
		      lFacet->setId( lId );
		      pData.addValidEntityForUndo( lFacet );
		    }
		  
		  lLocalDico.insertObj(  lId, lFacet );
												
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
                        EntityPtr lFacetPtr = lLocalDico.findObj( lFacetId );

                        if( lFacetPtr )
                          {
                            lPoly->addFacet( dynamic_cast<FacetPtr>(lFacetPtr));
                            
                            if( pConserveOldId )
                              {
                                lPoly->setId( lId );
                                pData.addValidEntityForUndo(lPoly);
                              }
                          }
                        else {
                          std::cerr << "Error Poly : facet not found :" << lFacetId << std::endl;
                        }
                            
			
			lLocalDico.insertObj( lId, lPoly );
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

		      EntityPtr lSubPtr = lLocalDico.findObj(lSubId);
                      if( lSubPtr )
                        {                        
                          switch( GetObjectTypeFromStr( lSubType.c_str() ))
                            {
                            case ObjectType::ObjPoint: break;
                            case ObjectType::ObjLine:
                              lObj = new ObjectLine( lName, dynamic_cast<LinePtr>(lSubPtr));
                              break;
                            case ObjectType::ObjFacet:
                              lObj = new ObjectFacet( lName, dynamic_cast<FacetPtr>(lSubPtr));
                              break;
                            case ObjectType::ObjPoly:
                              lObj = new ObjectPoly( lName, dynamic_cast<PolyPtr>(lSubPtr));
                              break;
                            case ObjectType::ObjPolyline:
                              lObj = new ObjectPolylines( lName, dynamic_cast<FacetPtr>(lSubPtr));
                              break;
                            case ObjectType::ObjBSpline:
                              lObj = new ObjectPolylines( lName, dynamic_cast<FacetPtr>(lSubPtr));
                              break;
                            case ObjectType::ObjNull: break;       				
                            }
                        }
                      else {
                        std::cerr << "Error Object - component not found :" << lSubId << std::endl;
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
                          lLocalDico.insertObj(  lId, lObj  );
			}
		    }
		  else	   //:::::::::: GROUP :::::::::::::
		    if( lToken == TokGroups )
		      {
			size_t lNbGroup;
			cIn >> lNbGroup;
                        SAVCOUT << "Read Groups sz:" << lNbGroup << std::endl;
                      }
                  else
                    if( lToken == TokGroup )
		      {                        
			size_t lGroupId ;
			cIn >> lGroupId;
                        
 			size_t lNb;
			cIn >>  lNb  ;
                        
                        SAVCOUT << "Group " << lGroupId << "  sz:" << lNb << std::endl;

                        
                        if( lNb > 0 )
                          {
                            GroupPtr lNewGroup = pData.newGroup();

                            SAVCOUT << "\t " << "new group " << lNewGroup->getGroupId() << std::endl;

                            for( size_t i=0; i< lNb; i++)
                              {			    
                                EntityId lId;
                                
                                cIn >> lId ;
                                SAVCOUT << i << ">\t Id " << lId  << std::endl;
                                
                                EntityPtr lEntity = lLocalDico.findObj(lId);
                                
                                if( lEntity != nullptr && lEntity->getType() == ShapeType::Object  )
                                  {
                                    ObjectPtr lObj = dynamic_cast<ObjectPtr>(lEntity);
                                    if( lObj != nullptr )
                                      {
                                        SAVCOUT << "\t  add " << lId  << std::endl;
                                        lNewGroup->addObject( lObj );
                                      }
                                    else
                                      {
                                        std::cerr << "Group : " <<  lGroupId
                                             << " adding Object:" << lId
                                                  << " failed " <<  std::endl;
                                      }
                                    
                                  }
                              }
                          }
                      } //:::::::::: GROUP :::::::::::::
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
			
			//	SAVCOUT << "Selection " << lStrTypeSelect << " " << lSelType
			//		  << " " << lNb << std::endl;
			
			for( size_t i=0; i< lNb; i++)
			  {			    
			    EntityId lId;
			    
			    cIn >> lId ;
			    
			    if( ioSel )
			      {
				//	SAVCOUT << "Sel " << lId ;
                                  EntityPtr lEntity = lLocalDico.findObj(lId);
                                  if( lEntity != nullptr )
                                    {
                                      ioSel->addEntity( lEntity, true );
                                      //	    SAVCOUT << " ok " <<  ioSel->getNbSelected()  << std::endl;
                                    }
                                  //	else
				//	  SAVCOUT << " ko " << std::endl;                                    
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
      return false;
    }
   					
    return true;
  }	
  //*************************************

}
