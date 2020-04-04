#include "Maker.h"


namespace PP3d {

	//***************************************************
	PolyPtr Maker::CreatePoly4FromFacet( FacetPtr pFacet, PIndex pNbPas, Mat4& pMat, bool pFlagCloseRevol,  bool pFlagCloseSeg, bool pFlagCloseSegEnd, bool pCloseHight, bool pCloseLow )
	{
		if( pNbPas <= 0 || pFacet == nullptr  )
			return nullptr;
		

		PIndex  lNbLine = pNbPas;

		
		std::vector<PointPtr> lPointsPtr;
		
		// on cree la premiere ligne qui est inchangés
		
		lPointsPtr.push_back( new Point( pFacet->getLines()[0]->getFirst()->get() ));
		PIndex p=1;
		for( LinePtr lLine : pFacet->getLines() )
			{
				if( lLine->getSecond() !=  pFacet->getLines()[0]->getFirst() )
					{
						lPointsPtr.push_back( new Point(lLine->getSecond()->get() ) );
						p++;
					}
			}
		
	

		PIndex  lNbCol  = lPointsPtr.size() ;

		std::cout << "Maker Line: " << lNbLine << " Col:" << lNbCol << std::endl;


		// on creer les points 

		// les autres sont multiplie par la matrice iterativement(pb de precision?)
		for( PIndex l=1; l< lNbLine; l++ )
			{
				for( PIndex lCol=0; lCol < lNbCol; lCol++, p++ )
					{
						std::cout << ">>>Points:" << p << " " << p-lNbCol  << std::endl;
						lPointsPtr.push_back( new Point( lPointsPtr[p-lNbCol]->get()*pMat ));
					}
			}

		std::cout << "Points:" << lPointsPtr.size() << std::endl;


		FacetPtr lFacetHight = nullptr;
		if( pCloseHight )
			{
				lFacetHight = new Facet();
			}
		
		FacetPtr lFacetLow = nullptr;
		if( pCloseLow )
			{
				lFacetLow = new Facet();
			}

		PolyPtr luPoly =  new Poly();


		if( pFlagCloseRevol )
			{
				lNbLine++;
			}
		// Attention il y a deux lignes de creer pour chaque facette jointive ( mais en sens contraire )
				 
		// Facettes carrées
		for( PIndex lLine=0; lLine< lNbLine-1; lLine++ )
			{
				// pour la cloture du segment
				PointPtr lMemFirstA=nullptr;
				PointPtr lMemFirstD=nullptr;
				
				PointPtr A ;
				PointPtr B ;
				PointPtr C ;
				PointPtr D ;


				for( PIndex lCol=0; lCol < lNbCol-1; lCol++ )
					{						
						FacetPtr lFacet = new Facet();                // nouvelle facette vide sans id
						//					std::cout << "==== Line:" <<  lLine  << " Col:" <<  lCol<< std::endl;
						
						A = lPointsPtr[ lLine*lNbCol     + lCol     ];
						//				std::cout << "A:" <<  lLine*lNbCol     + lCol<< std::endl;
						
						B = lPointsPtr[ lLine*lNbCol     + lCol + 1 ];
						//			std::cout << "B:" <<  lLine*lNbCol     + lCol + 1 << std::endl;
						
						C = lPointsPtr[ (lLine+1)*lNbCol + lCol + 1 ];
						//			std::cout << "C:" << (lLine+1)*lNbCol + lCol + 1  << std::endl;
						
						D = lPointsPtr[ (lLine+1)*lNbCol + lCol     ];
						//			std::cout << "D:" << (lLine+1)*lNbCol + lCol  << std::endl;


						// Fermeture a 360
						if( pFlagCloseRevol && lLine == lNbLine-2 )  
							{
								C = lPointsPtr[  lCol + 1 ];
								D = lPointsPtr[  lCol  ];
							}

						if( pFlagCloseSeg && lCol == 0)
							{
								lMemFirstA = A;
								lMemFirstD = D;
							}
						
					
						lFacet->addLine( new Line( A, B ));
						lFacet->addLine( new Line( B, C ));
						lFacet->addLine( new Line( C, D ));
						lFacet->addLine( new Line( D, A ));
						luPoly->addFacet( lFacet );


						if( lFacetHight != nullptr && lCol == 0 )
							{
								lFacetHight->addLine( new Line(A, D ) );
							}
						
						if( lFacetLow != nullptr && lCol == lNbCol-2 )
							{
								lFacetLow->addLine( new Line(B, C));
							}						
					}
				
				if( pFlagCloseSeg && lMemFirstA )
					{
						FacetPtr lFacet = new Facet();                // nouvelle facette vide sans id
						lFacet->addLine( new Line( B, lMemFirstA ));
						lFacet->addLine( new Line( lMemFirstA, lMemFirstD ));
						lFacet->addLine( new Line( lMemFirstD, C ));
						lFacet->addLine( new Line( C, B ));
						luPoly->addFacet( lFacet );					
					}
			}
	
		if( pFlagCloseSegEnd )
			{
				// pour toutes les facettes cree en bordure
				// on prend la premiere ligne		
		
				FacetPtr lNewFacet0 = new  Facet();
				FacetPtr lNewFacet1 = new  Facet();
				
				{
					PointPtr lLastPoint  = nullptr;
					
					FacetPtrVect lFacets = luPoly->getFacets();
					
					for( PIndex lCol=0; lCol < lNbCol; lCol++ )
						{
							std::cout << " lCol : " << lCol << std::endl;
							
							FacetPtr lFacet = lFacets[lCol];
							LinePtr lLine = lFacet->getLines()[0]; // on prend la premiere ligne
							if( lCol == 0 )
								{
									lLastPoint = lLine->getFirst();									
								}
							lNewFacet0->addLine( new Line( lLastPoint, lLine->getSecond()) );
							lLastPoint = lLine->getSecond();
						}
					
					lNewFacet0->inverseLines();
				}


				//la meme chose pour l'autre extremite
				
				// On prend les dernieres facettes
				{
					PointPtr lLastPoint  = nullptr;
					
					FacetPtrVect lFacets = luPoly->getFacets();
					
					PIndex   lBegin = lFacets.size() - lNbCol;
					
					for( PIndex lCol=0; lCol < lNbCol; lCol++ )
						{
							std::cout << " lCol : " << lCol << std::endl;
							
							FacetPtr lFacet = lFacets[lBegin+lCol];
							LinePtr lLine = lFacet->getLines()[2];  // On prend le coté oppose
							if( lCol ==0 )
								{
									lLastPoint = lLine->getSecond();
								}
							lNewFacet1->addLine( new Line( lLastPoint, lLine->getFirst()) );
							lLastPoint = lLine->getFirst();
						}
					
					//		lNewFacet1->inverseLines();					
				}
		 		
				
				luPoly->addFacet( lNewFacet0 );
				luPoly->addFacet( lNewFacet1 );
			}

		

		if( lFacetHight != nullptr && lFacetHight->getNbLines() > 2  )
			{
				luPoly->addFacet(lFacetHight);
			}
		else
			{
				delete lFacetHight;
			}
		
		if( lFacetLow != nullptr  && lFacetLow->getNbLines() > 2 )
			{
				luPoly->addFacet(lFacetLow);			
			}
		else
			{
				std::cout << "Delete facette low" << std::endl;
				delete lFacetLow;
			}


		

		// Fermer le dernier et le haut et le bas ????

		return luPoly;
	}
	//-------------------------------------------------------------
	FacetPtrVect Maker::Trianguler( DataBase& pBase, FacetPtrVect& pFacet, bool lForce )
	{
		FacetPtrVect lVect;
		
		for( FacetPtr lFac : pFacet )
			{				
				if( lFac->cLines.size() == 3 )
					{
						if( lForce == false )
							{
								lVect.push_back( lFac );
							}
						else
							{
								//division a faire
							}
					}
				else
					if( lFac->cLines.size() == 4 )
						{
							FacetPtr lNewFac = new Facet();
							lNewFac->addLine( lFac->cLines[2] );							
							lNewFac->addLine( lFac->cLines[3] );
							lNewFac->addLine( new Line( lFac->cLines[3]->second(), lFac->cLines[2]->first() ));

							LinePtr lLine = lFac->cLines.back();  // la 3
							lFac->cLines.pop_back();
							lLine->removeOwner( lFac );
							//		lBase->removeEntityIfNoOwner( lLine );
							
							lLine = lFac->cLines.back(); // la 2 
							lFac->cLines.pop_back();
							lLine->removeOwner( lFac );
							//		lBase->removeEntityIfNoOwner( lLine );

							
							lFac->addLine(  new Line( lFac->cLines[1]->second(), lFac->cLines[0]->first() ));
							
							lVect.push_back( lFac );
							lVect.push_back( lNewFac );							
						}				
			}

		return lVect;
	}		
	
	//***************************************************				
}
