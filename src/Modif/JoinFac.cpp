#include "Modif.h"

#include <limits>
#include <vector>


using namespace PP3d;
using namespace std;
//--------------------------------------------------
FacetPtr CreateFacetSquare(  DataBase * iBase,
                             LinePtr iLineA,
                             LinePtr iLineB, bool iFlagInv )
{
  FacetPtr lFacPtr = iBase->getNewFacet();

  
  PointPtr lA = iLineA->first();
  PointPtr lB = iLineA->second();
  
  PointPtr lC = iLineB->second();
  PointPtr lD = iLineB->first();
   
  lFacPtr->addLine( iBase->getNewLine(lA, lB)); 
  lFacPtr->addLine( iBase->getNewLine(lB, lC)); 
  lFacPtr->addLine( iBase->getNewLine(lC, lD)); 
  lFacPtr->addLine( iBase->getNewLine(lD, lA));
  
  if( iFlagInv)
    {
      lFacPtr->inverseLines();
    }
  iBase->validEntity( lFacPtr, true );
  
  
  return lFacPtr;
}
//--------------------------------------------------
FacetPtr CreateFacetTriangle( DataBase * iBase,
                              PointPtr iPt,
                              LinePtr iLine, bool iFlagInv )
{
  FacetPtr lFacPtr = iBase->getNewFacet();
  
  PointPtr lA = iPt;
  PointPtr lB = iLine->second();  
  PointPtr lC = iLine->first();
  
  lFacPtr->addLine( iBase->getNewLine(lA, lB)); 
  lFacPtr->addLine( iBase->getNewLine(lB, lC)); 
  lFacPtr->addLine( iBase->getNewLine(lC, lA)); 
  if( iFlagInv)
    {
      lFacPtr->inverseLines();
    }
  iBase->validEntity( lFacPtr, true );
 
  return lFacPtr;
}

//--------------------------------------------------
int CreateFacetsByJoinLines( DataBase * iBase,
                             LinePtr iLine,
                             std::vector<LinePtr> iVectLine,
                             std::vector<FacetPtr> & oNewFacets,
                             bool iMiddleSquare  )
{
  int lSz = iVectLine.size();
  bool lSquare = ( (lSz%2) == 1 ? true : false );  // nb de line pair ou impair
  
  int lNbTriangle = (lSz-1)/2;
  
  cout << "   CreateFacetsByJoinLines Sz:" << lSz << " Square:" << lSquare<< " NbTri:" <<  lNbTriangle << endl;
  
  int i=0;
  for( i = 0 ; i< lNbTriangle; i++ )
    {
      cout << "   G    i:" << i << endl;
      oNewFacets.push_back( CreateFacetTriangle( iBase, iLine->first(), iVectLine[ i ], true ));
    }

  
  if( lSquare )
    {
      if(iMiddleSquare )
        {
          cout << "   MO4    i:" << i << endl;
          oNewFacets.push_back( CreateFacetSquare( iBase, iLine, iVectLine[i], true ));
        }
      else
        {
          cout << "   MO3    i:" << i << endl;
          oNewFacets.push_back( CreateFacetTriangle( iBase, iLine->first(), iVectLine[i], true ));
          oNewFacets.push_back( CreateFacetTriangle( iBase, iVectLine[i]->first(), iLine, false ));
        }
      i++;
    }
  else
    {
          cout << "   M     i:" << i << endl;
          oNewFacets.push_back( CreateFacetTriangle( iBase, iVectLine[i]->first(), iLine, false ));
    }
  

  for( ; i< lSz; i++ )
    {
      cout << "   D     i:" << i << endl;
      oNewFacets.push_back( CreateFacetTriangle( iBase, iLine->second(), iVectLine[i], true) );
    }

  cout << "   CreateFacetsByJoinLines nb Fac:" <<  lNbTriangle << oNewFacets.size() << endl;

  return oNewFacets.size();
}

//--------------------------------------------------
// Dans certain cas il faudrait inverser le sens des facettes
// Actuellement on ne le fait pas, peut etre en comparant les normales ?
// Sinon c'est à l'utilisateur de le faire, ce qui est génant

bool Modif::JoinTwoFacets( DataBase * iBase,
                           FacetPtr iF1,
                           FacetPtr iF2,
                           std::vector<FacetPtr> & oNewFacets,
                           bool iMiddleSquare,
                           bool iInv, int iDecal  )  // unuseds
{
  FacetPtr lFa, lFb;


  // On met dans lFa la facette avec le moins de ligne
  if( iF1->getNbLines() <= iF2->getNbLines() )
    {
      lFa = iF1;
      lFb = iF2;
    }
  else
    {
      lFa = iF2;
      lFb = iF1;
    }
  
  cout << "*****************" << endl;
  
  double lRatio = lFb->getNbLines() / (double)lFa->getNbLines();  // Ratio between lines of lFa and lFb
 
  cout <<  lFa->getNbLines() << "  " << lFb->getNbLines() << "   -->   Ratio = " << lRatio << endl << endl;

  if( iInv )
    lFa->inverseLines();
  
  PointPtr lPtA = lFa->getPoint( 0 );

  // Find the point of  Fb closest to the first point of Fa
  // to try to reduce the twisting of facets
  
  PIndex lFirstLineB = 0;

  // find the couple of line in the two facet that have the minus angle between then
  double lMinAngle = std::numeric_limits<double>::max();
  PIndex lFirstLineA = 0;

  for( PIndex a=0; a< lFa->getNbLines(); a++ )
    {
      Point3d lVectA =  lFa->getLine( a )->getVector3d();
      lVectA.normalize();
        
      for( PIndex b=0; b< lFb->getNbLines(); b++ )
        {
          Point3d lVectB =  lFb->getLine( b )->getVector3d();
          lVectB.normalize();
          
          double lAngle = Point3d::GetAngleRadBetween( lVectA, lVectB );
          
          cout << "a b: " << a << '-' << b << " = " << lAngle <<    endl;
          
          if( lAngle < lMinAngle )
            {
              lMinAngle = lAngle;
              lFirstLineB = b;
              lFirstLineA = a;
            }
        }
    }
    cout << "**************************************************" <<  endl;
    cout << "Min line distance : " << lFirstLineA << "-" << lFirstLineB << " with "    << lMinAngle <<  endl;
    cout << "**************************************************" <<  endl;

    // lFirstLineB=iDecal;
  
  std::vector<LinePtr>  lVectLineB;
  int lNbCreate = 0;

  // lFirstLineB  =0;

  // Les facettes n'ont pas forcement le meme nombre de points
  // il va falloir eventuellement racordé un point d'une facette à plusieurs de l'autre
  // Rest sert à déclenché le changement de point en ajoutant lRation
  
  double lRest = 0;
  PIndex lBeginIndexB = 0;

  //=====================================
  // for all the lines of lFa
  for( long lIndexA = 0; lIndexA  < (long)lFa->getNbLines(); lIndexA++ )
    {
      LinePtr lLineA = lFa->getLine( (lIndexA+lFirstLineA)% lFa->getNbLines()); 
     
      PIndex lIndexB = lBeginIndexB;
      double lLastIndexFloatB  = lBeginIndexB + lRatio + lRest; // therorical last index for lFb
      PIndex lLastIndexB = (PIndex)(lLastIndexFloatB+0.1); // practical last index for lFb

      cout << "***  A=" << lIndexA
           << "       B=" << lBeginIndexB << " to " << lLastIndexFloatB
           << " -> " << lLastIndexB<< endl;

      // keep the line of lFb we can use with lLineA
      lVectLineB.clear();
      for(  ; lIndexB < lLastIndexB; lIndexB++ )
        {
          // Modify indexB for anti-twisting
          int lIndexBFinal = (lIndexB + lFirstLineB); // % lFb->getNbLines();
            // int lIndexBFinal = (lIndexB + lFirstLineB);
            //   if( lIndexBFinal > (long)(lFb->getNbLines()-1) )
            //     {
            //       lIndexBFinal -=  lFb->getNbLines();          
            //      }
          lVectLineB.push_back(  lFb->getLineModulo( lIndexBFinal));
          cout << "  " <<  lIndexB;
        }
      lRest = lLastIndexFloatB-lIndexB; // must be reported at the next round   

      lBeginIndexB = lIndexB;
      
      cout << "*  Rest: " <<  lRest  << " next beginB: " << lBeginIndexB << endl;
      

      lNbCreate += CreateFacetsByJoinLines( iBase, lLineA, lVectLineB, oNewFacets, iMiddleSquare );
         
    }
  //=====================================

  //   IL faut detruire les deux facettes et aussi leurs lignes !!!!
  //   c'est a l'appelant de le faire !


  

  

  return lNbCreate > 0 ;
}
