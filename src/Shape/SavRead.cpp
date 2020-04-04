#include "SavRead.h"

#include "DataBase.h"

#include "ObjectLine.h"
#include "ObjectFacet.h"
#include "ObjectPolylines.h"
#include "ObjectPoly.h"


const char* TokPoint="Point:";
const char* TokLine="Line:";
const char* TokFacet="Facet:";
const char* TokPoly="Poly:";
const char* TokObject="Object:";



namespace PP3d {
	//*************************************
	bool MySav::save( DataBase& pData )
	{
		auto lEntities = pData.getEntities();

		//	for( auto lPairEntity :  lEntities )
		//		{
		//			std::cout << " - " <<  lPairEntity.first << " " << lPairEntity.second->getId() << " " << lPairEntity.second->getType() << std::endl;
		//	}		
		//=======================
		//	cOut << TokPoint << std::endl;

		for( auto lPairEntity :  lEntities )
			{		
				if( lPairEntity.second->getType() != ShapeType::Point )
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

				auto lEntity = dynamic_cast<ObjectPtr>(lPairEntity.second);
				cOut  << TokObject
					<< ' '  << lEntity->getId()
					<< ' '  << lEntity->getObjType()
					<< ' '  << lEntity->getShape()->getId() 
					<< " @"  << lEntity->getName()   // pour eviter de recuperer l'espace devant le nom
					<< std::endl;				

			}
					cOut << std::endl;
				return true;
	}
	
		//*************************************

#define ReadEndLine		std::getline(	cIn,  lEndOfLine, '\n' )

		//--------------------------------
		bool MyRead::read( DataBase& pData, bool pConserveOldId  )
		{
			try {
				std::unordered_map<EntityId, Entity*> lLocalDico;
				
				std::string lEndOfLine; // pour lire la fin de ligne 
				
				while( cIn.good() )
					{
						std::string lToken;
						cIn >> lToken;
						//				std::getline( cIn, lToken, ' ' );
				
						std::cout << "Token: <<" << lToken << ">> ";
						
						EntityId lId;
						if( lToken == TokPoint )
							{
								PDouble  lX,lY,lZ;
						
								cIn >> lId >> lX >> lY >> lZ;
					 
								std::cout << " - Read Point<<" << lId <<" : " << lX << " : " << lY << " : " << lZ <<  std::endl;
						
								Point* lPoint = new Point( Point3d( lX, lY, lZ ) );

								lLocalDico.insert( { lId, lPoint } );
								
								ReadEndLine;
							}
						else
							if( lToken == TokLine )
								{
									EntityId lFirstId, lSecondId;
									
									cIn >> lId >> lFirstId >> lSecondId ;	
									std::cout << " - Read Line <<" << lId <<" : " << lFirstId<< " : " << lSecondId <<  std::endl;
							
									Line* lLine = new Line( static_cast<PointPtr>(lLocalDico.at(lFirstId)),
																					static_cast<PointPtr>(lLocalDico.at(lSecondId)) );
									lLocalDico.insert( { lId, lLine } ); 

									ReadEndLine;
								}
									else
										if( lToken == TokFacet )
											{
												size_t lNb;
												EntityId lLineId;
												
												cIn >> lId >> lNb  ;
												std::cout << " - Read Facet <<" << lId <<" : " << lNb <<  std::endl;
												Facet *lFacet = new Facet();
												
												for( size_t i=0; i< lNb; i++)
													{
														cIn >> lLineId ;
														lFacet->addLine( static_cast<LinePtr>(lLocalDico.at(lLineId)) );
														
														std::cout << "    "  << i << ":" << lLineId <<  std::endl;														
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
													std::cout << " - Read Poly <<" << lId <<" : " << lNb <<  std::endl;
													
													Poly* lPoly = new Poly();
													
													for( size_t i=0; i< lNb; i++)
														{
															cIn >> lFacetId;
															
															std::cout << "   " <<  i << ":" << lFacetId <<  std::endl;
															lPoly->addFacet( static_cast<FacetPtr>(lLocalDico.at(lFacetId)));
															
															
															lLocalDico.insert( { lId, lPoly } );
														}
													ReadEndLine;
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
														
														std::cout << " - Read object <<" << lId <<" : " << lSubType << " : " <<  lSubId  << " <<" << lName << ">>" <<  std::endl;
														
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
																pData.addObject( lObj );
															}
													}
												else
													break;
					}
			}	catch( const std::exception & lEx ) {				
				std::cerr << __FILE__ << ":" << __LINE__ << ":Exception " << lEx.what() << std::endl;
			}
					
			return true;
		}	
//*************************************

}
