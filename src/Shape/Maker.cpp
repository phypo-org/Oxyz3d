#include "Maker.h"


namespace PP3d {

  //***************************************************
  // Pb de creation de plusieurs point au lieu d'un seul si
  // le point est sur l'axe de rotation( sommet de cone ou pole de sphere ! )
  // il faudrait detecter si des points sont sur l'axe et ne les creer qu'une fois !
  
  PolyPtr Maker::CreatePoly4FromFacet( FacetPtr pFacet, PIndex pNbPas, Mat4& pMat,
                                       CloseRevol  iFlagCloseRevol,
                                       CloseSeg    iFlagCloseSeg,
                                       CloseSegEnd iFlagCloseSegEnd,
                                       CloseHight  iCloseHight,
                                       CloseLow    iCloseLow,
                                       WithGrid    iFlagGrid,
                                       Point3d  *  iGearDecal,
                                       PIndex      iGearMod,
                                       bool        iGearInv,
				       long double iEpsilon )
  {
    if( pNbPas <= 0 || pFacet == nullptr  )
      return nullptr;
		

    PIndex  lNbLine = pNbPas;


    std::cout << "Maker::CreatePoly4FromFacet Gear:" << (void*)  iGearDecal << std::endl;
		
    std::vector<PointPtr> lPointsPtr;
		
    // on cree la premiere ligne qui est inchangés
		
    lPointsPtr.push_back( new Point( pFacet->getLines()[0]->getFirst()->get() ));
    PIndex p=1;
    for( LinePtr lLine : pFacet->getLines() )
      {
	if( lLine->getSecond() !=  pFacet->getLines()[0]->getFirst() )
	  {
	    lPointsPtr.push_back( new Point(lLine->getSecond()->get() ));      
	    p++;
	  }
      }	
	

    PIndex  lNbCol  = lPointsPtr.size() ;

    std::cout << "Maker Line: " << lNbLine << " Col:" << lNbCol << std::endl;


    // on creer les points 

    // les autres sont multiplie par la matrice iterativement(pb de precision?) // il faudrait recalculer la matrice a partir de l'angle veritable !!!!!!!!!!!
    // Il faudrait aussi reperer que certain point sont les memes (sommet de cone par ex :) dans ce cas il faudrait ne creer qu'un seul point
   
    
    for( PIndex l=1; l< lNbLine; l++ )
      {
	for( PIndex lCol=0; lCol < lNbCol; lCol++, p++ )
	  {
	    ////	    std::cout << ">>>Points:" << p << " " << p-lNbCol  << std::endl;
            
            Point3d lPt3d = lPointsPtr[p-lNbCol]->get()*pMat;                         
              
	    PointPtr lTmpPt = nullptr;	    

	    // Ou essaye de detecte si le point existe deja !!!
	    // PAS EFFICACE DU TOUT : A REFAIRE 
	    for( PointPtr lSearch : lPointsPtr )
	      {
		if( lSearch->get().sameEpsi( iEpsilon, lPt3d ) )
		  {
		    lTmpPt = lSearch;
		    break;
		  }
	      }
	    if( lTmpPt == nullptr )
              {
                lTmpPt = new Point( lPt3d );
              }
	    
	    lPointsPtr.push_back(lTmpPt );
	  }
      }

    
    //--------- Pour Gear !!!
   if( iGearDecal != nullptr && iGearMod >= 2 )
      {
        int ppp=0;
        for( PIndex l=0; l< lNbLine; l++ )
          {
            if( l > 0 )
              *iGearDecal *= pMat; 
            
            for( PIndex lCol=0; lCol < lNbCol; lCol++, ppp++ )
              {
                ////	    std::cout << ">>>Points:" << p << " " << p-lNbCol  << std::endl;
                
                if( iGearDecal != nullptr && iGearMod >= 2 && (l%iGearMod) == 0)
                  {
                    if( iGearInv )
                      lPointsPtr[ppp]->get() -= *iGearDecal;
                    else
                      lPointsPtr[ppp]->get() += *iGearDecal;
                  }
              }
          }
      }
    //-------- Pour Gear !!!


    //    std::cout << "Points:" << lPointsPtr.size() << std::endl;


    FacetPtr lFacetHight = nullptr;
    if( iCloseHight == CloseHight::Yes )
      {
	lFacetHight = new Facet();
      }
		
    FacetPtr lFacetLow = nullptr;
    if( iCloseLow  == CloseLow::Yes)
      {
	lFacetLow = new Facet();
      }

    PolyPtr luPoly =  new Poly();


    if( iFlagCloseRevol == CloseRevol::Yes )
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
				
	PointPtr A=nullptr ;
	PointPtr B=nullptr ;
	PointPtr C=nullptr ;
	PointPtr D=nullptr ;


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
	    if( ( iFlagCloseRevol == CloseRevol::Yes)  && lLine == lNbLine-2 )  
	      {
		C = lPointsPtr[  lCol + 1 ];
		D = lPointsPtr[  lCol  ];
	      }

	    if( (iFlagCloseSeg == CloseSeg::Yes) && lCol == 0)
	      {
		lMemFirstA = A;
		lMemFirstD = D;
	      }
						
					
	    lFacet->addTrueLine( new Line( A, B ));
	    lFacet->addTrueLine( new Line( B, C ));
	    lFacet->addTrueLine( new Line( C, D ));
	    lFacet->addTrueLine( new Line( D, A ));
	    luPoly->addFacet( lFacet );


	    if( lFacetHight != nullptr && lCol == 0 )
	      {
		lFacetHight->addTrueLine( new Line(A, D ) );
	      }
						
	    if( lFacetLow != nullptr && lCol == lNbCol-2 )
	      {
		lFacetLow->addTrueLine( new Line(B, C));
	      }						
	  }
				
	if( (iFlagCloseSeg == CloseSeg::Yes) && lMemFirstA )
	  {
	    FacetPtr lFacet = new Facet();                // nouvelle facette vide sans id
	    lFacet->addTrueLine( new Line( B, lMemFirstA ));
	    lFacet->addTrueLine( new Line( lMemFirstA, lMemFirstD ));
	    lFacet->addTrueLine( new Line( lMemFirstD, C ));
	    lFacet->addTrueLine( new Line( C, B ));
	    luPoly->addFacet( lFacet );					
	  }
      }
	
    if( iFlagCloseSegEnd == CloseSegEnd::No)
      {
	// pour toutes les facettes cree en bordure
	// on prend la premiere ligne		
		
	FacetPtr lNewFacet0 = new  Facet();
	FacetPtr lNewFacet1 = new  Facet();

        //:::::::::::::::::::::::::::::::::
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
	      lNewFacet0->addTrueLine( new Line( lLastPoint, lLine->getSecond()) );
	      lLastPoint = lLine->getSecond();
	    }
					
	  lNewFacet0->inverseLines();
	}
        //:::::::::::::::::::::::::::::::::

	//la meme chose pour l'autre extremite				
	// On prend les dernieres facettes
	{
	  PointPtr lLastPoint  = nullptr;
					
	  FacetPtrVect lFacets = luPoly->getFacets();
					
	  PIndex   lBegin = lFacets.size() - lNbCol;

          for( int i = lNbCol; i >0; i-- )
	    {
              PIndex lCol= lFacets.size()-i;
	      std::cout << " lCol : " << lCol  << std::endl;
							
	      FacetPtr lFacet = lFacets[lCol];
	      LinePtr lLine = lFacet->getLines()[2];  // On prend le coté oppose
	     
              lNewFacet1->addTrueLine( new Line( lLine->getSecond(), lLine->getFirst()));
            }					
          
	}
        //:::::::::::::::::::::::::::::::::

        if( iFlagGrid == WithGrid::Yes || lNewFacet0->getNbLines() <= 4 )
          {		 						
            luPoly->addFacet( lNewFacet0 );
            luPoly->addFacet( lNewFacet1 );              

          }
        else
          {
            FacetPtrVect lVectFacPtr;
            
            lNewFacet1->inverseLines();
            QuadrangulerConcave( *lNewFacet1, lVectFacPtr );
            for( FacetPtr lFacPtr : lVectFacPtr )
              {
                lFacPtr->inverseLines();
              }
              
            QuadrangulerConcave( *lNewFacet0, lVectFacPtr );
            luPoly->addFacet( lVectFacPtr );
            
            delete lNewFacet1;           
            delete lNewFacet0;                          
          }
      }

    	

    if( lFacetHight != nullptr && lFacetHight->getNbLines() > 2  )
      {
	luPoly->addFacet(lFacetHight);
      }
    else
      {
	std::cout << "Delete facette hight" << std::endl;
	if( lFacetHight != nullptr )
	  delete lFacetHight;
      }
		
    if( lFacetLow != nullptr  && lFacetLow->getNbLines() > 2 )
      {
	lFacetLow->inverseLines();
	luPoly->addFacet(lFacetLow);			
      }
    else
      {	
	std::cout << "Delete facette low" << std::endl;
	if( lFacetLow != nullptr )
	  delete lFacetLow;
      }
		

    // Fermer le dernier et le haut et le bas ????

    return luPoly;
  }
  //-------------------------------------------------------------
  GLuint Maker::QuadrangulerConcave( Facet & iFacet, FacetPtrVect & oFacVect )
  {
    LinePtrVect & lLines =   iFacet.getLines();

    if( lLines.size() <= 4 ) return 0;
    
    GLuint  lNbLine  = lLines.size();
    GLuint  lMaxLine = (lNbLine/2)-1;
    LinePtr lLastLine = nullptr;
    
    //    std::cout << "QuadrangulerConcave line:"<< std::dec << lNbLine << " Max:" << lMaxLine << std::endl;
    
    for( GLuint l = 0; l< lMaxLine; l++)
      {
        LinePtr lLine0 ;
        LinePtr lLine2 ;
        
        LinePtr lLine1 = lLines[l+1];
        LinePtr lLine3 = lLines[(lNbLine-1)-l];
        
        //        std::cout << "QuadrangulerConcave line:" << l << std::endl;
        
        if( l == 0 )  // le debut
          {
            lLine0 = lLines[0];
          }
        else
          {
            lLine0 =  new Line( lLastLine->second(),  lLastLine->first());
          }
      
        if( l == lMaxLine-1 ) // la fin
          {
            lLine2  = lLines[l+2];;
          }
        else
          {
            lLine2 = lLastLine = new Line( lLine1->second(),  lLine3->first() );
          }  

        FacetPtr lFacPtr;
        oFacVect.push_back( lFacPtr =  new Facet( lLine0, lLine1, lLine2, lLine3 ) );    
      }

    //    std::cout << "QuadrangulerConcave size:" <<  oFacVect.size() << std::endl;

    return oFacVect.size();
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
	      lNewFac->addTrueLineEpsi( kEpsilon, lFac->cLines[2] );							
	      lNewFac->addTrueLineEpsi( kEpsilon, lFac->cLines[3] );
	      lNewFac->addTrueLineEpsi( kEpsilon, new Line( lFac->cLines[3]->second(), lFac->cLines[2]->first() ));

	      LinePtr lLine = lFac->cLines.back();  // la 3
	      lFac->cLines.pop_back();
	      lLine->removeOwner( lFac );
	      //		lBase->removeEntityIfNoOwner( lLine );
							
	      lLine = lFac->cLines.back(); // la 2 
	      lFac->cLines.pop_back();
	      lLine->removeOwner( lFac );
	      //		lBase->removeEntityIfNoOwner( lLine );

							
	      lFac->addTrueLineEpsi( kEpsilon,  new Line( lFac->cLines[1]->second(), lFac->cLines[0]->first() ));
							
	      lVect.push_back( lFac );
	      lVect.push_back( lNewFac );							
	    }				
      }

    return lVect;
  }		
	
  //***************************************************				
}
