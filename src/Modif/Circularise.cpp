


#include "Modif.h"

#include <limits>
#include <vector>


using namespace PP3d;
using namespace std;

#define COUT if( true ) cout

//--------------------------------------------------
bool Modif::Circularise(  FacetPtr ioFac )
{
  size_t lNbPt =  ioFac->getNbLines();

  
  
  if( lNbPt < 3 )
    return false;

  if( ioFac->isClose() == false )
    {
      lNbPt++; 
    }
  
  COUT << "##################  Modif::Circularise : "<< lNbPt <<  endl;
    
  Point3d lCenter   = ioFac->getCenter3d();
  Point3d lNormFac  = ioFac->getNormal3d();

  double lDistAverage=0;
  for( size_t i = 0; i<lNbPt; i++)
    {
      Point3d lDist3 = ioFac->getPoint(i)->get() - lCenter ;
      lDistAverage +=  lDist3.distance();
    }
  lDistAverage /= lNbPt;
  
  // PP3d::Mat4 lMatGrow;
  //  lMatGrow.initScale( lDist );

  COUT << "##################  Modif::Circularise Center :  "<< lCenter <<  endl;
  COUT << "##################  Modif::Circularise Distance :  "<< lDistAverage <<  endl;

  if( lNbPt ==  3 )
      {
	Calcul3d::Normal( ioFac->getPoint(0)->get(),
                          ioFac->getPoint(1)->get(),
                          ioFac->getPoint(2)->get(),
                          lNormFac );
      }
  else
    {
      int i = lNbPt/3;
      int j = (lNbPt*2)/3;
      Calcul3d::Normal(  ioFac->getPoint(0)->get(),
                         ioFac->getPoint(i)->get(),
                         ioFac->getPoint(j)->get(),		
                         lNormFac );
      }

  COUT << "##################  Modif::Circularise Normal facette :  "<< lNormFac <<  endl;
    

  PP3d::Mat4 lMatReput;
  lMatReput.initMove( lCenter ); // on positionne en fonction du centre de rotation ;;
  PP3d::Point3d lNCenter =  -lCenter;


  PP3d::Mat4 lMatZero;
  lMatZero.initMove( lNCenter ); // on revient a la postion originale en zero;


  // On veut poser la facette à plat sur la repere x,z
  PP3d::Mat4 lMatAlign;
  PP3d::Point3d lNormDest( 0, 1, 0 );

  lMatAlign.rotateAlign( lNormDest, lNormFac ); // pour aligne la normale avec l'axe Y
  
  PP3d::Mat4 lMatAlignInv;
  lMatAlignInv.rotateAlignInv( lNormDest,  lNormFac ); // le contraire
  

  // On determine  l'angle du premier point par rapport à l'axe x,z
  Point3d lFirst = ioFac->getPoint(0)->get();
  lFirst *=  lMatAlign *  lMatZero;
  lFirst.y() = 0;  
  COUT << "##################  Modif::Circularise first :  "<<  lFirst <<  endl;
  lFirst.normalize();
  COUT << "##################  Modif::Circularise first :  "<<  lFirst <<  endl;

  double lBeginAngle = ::atan2( lFirst.z(), lFirst.x() );  // L'angle de debut de la facette
    COUT << "##################  Modif::Circularise BeginAngle :  "<<  lBeginAngle * 180.0 / M_PI <<  endl;


  double lStepAngle = -M_PI*2 / lNbPt;


  COUT << "##################  Modif::Circularise Step :  "<< lStepAngle * 180.0 / M_PI  <<  endl;

  Point3d lOrigin( lDistAverage, 0, 0);
                                               
  for( GLuint i = 0; i<lNbPt; i++)
    {
      COUT << "      ###### i  :  "<< i << "  " << (lBeginAngle+lStepAngle*i) * 180.0 / M_PI  <<  endl;
      
      PP3d::Mat4 lMatRot;        
      //      lMatRot.initRotAxis( lNormFac, lBeginAngle + lStepAngle * i);
      lMatRot.initRotY( lBeginAngle + lStepAngle * i);
      
      //  lMatSpin.initRotAxis( lNormToMove, lStep * i);
            // PP3d::Mat4 lMatTran = lMatReput  *  lMatSpin   * lMatZero;
      //      PP3d::Mat4 lMatTran = lMatReput * lMatGrow  * lMatRot *  lMatZero;
      //      lMatTran = lMatReput * lMatAlignInv, lMatRot * lMatAlign *  lMatZero;
      PP3d::Mat4 lMatTran = lMatReput * lMatAlignInv * lMatRot;
      ioFac->getPoint(i)->get() = lOrigin* lMatTran;
    }

  ioFac->computeNormal();
  
  return true;
}
 
//--------------------------------------------------
bool  Modif::Circularise( std::vector<PointPtr>  & ioPt )
{
  size_t lNbPt = ioPt.size();

  if( lNbPt < 3 )
    return false;
  
  /*
  if(ioPt[0]->get() == ioPt[lNbPt-1]->get()  )
    {
      lNbPt--; 
    }
  */
  

  Point3d lCenter;
  Point3d lNormFac;

  
  for( PointPtr lPt : ioPt )
    {
      lCenter += lPt->get();
    }  
  lCenter /= lNbPt;

 
  double lDistAverage=0;
  for( PointPtr lPt : ioPt )
    {
      Point3d lDist3 = lPt->get() - lCenter ;
      lDistAverage +=  lDist3.distance();
    }  
  lDistAverage /= lNbPt;


   
  if( lNbPt ==  3 )
      {
	Calcul3d::Normal( ioPt[0]->get(),
                          ioPt[1]->get(),
                          ioPt[2]->get(),
                          lNormFac );
      }
  else
    {
      int i = lNbPt/3;
      int j = (lNbPt*2)/3;
      Calcul3d::Normal(  ioPt[0]->get(),
                         ioPt[i]->get(),
                         ioPt[j]->get(),		
                         lNormFac );
      }

  
  PP3d::Mat4 lMatReput;
  lMatReput.initMove( lCenter ); // on positionne en fonction du centre de rotation ;;
  PP3d::Point3d lNCenter =  -lCenter;


  PP3d::Mat4 lMatZero;
  lMatZero.initMove( lNCenter ); // on revient a la postion originale en zero;



  // On veut poser la facette à plat sur la repere x,z
  PP3d::Mat4 lMatAlign;
  PP3d::Point3d lNormDest( 0, 1, 0 );

  lMatAlign.rotateAlign( lNormDest, lNormFac ); // pour aligne la normale avec l'axe Y
  
  PP3d::Mat4 lMatAlignInv;
  lMatAlignInv.rotateAlignInv( lNormDest,  lNormFac ); // le contraire
  

  // On determine  l'angle du premier point par rapport à l'axe x,z
  Point3d lFirst = ioPt[0]->get();
  lFirst *=  lMatAlign *  lMatZero;
  lFirst.y() = 0;  
  COUT << "##################  Modif::Circularise first :  "<<  lFirst <<  endl;
  lFirst.normalize();
  COUT << "##################  Modif::Circularise first :  "<<  lFirst <<  endl;

  double lBeginAngle = ::atan2( lFirst.z(), lFirst.x() );  // L'angle de debut de la facette
  COUT << "##################  Modif::Circularise BeginAngle :  "<<  lBeginAngle * 180.0 / M_PI <<  endl;


  double lStepAngle = -M_PI*2 / lNbPt;


  Point3d lOrigin( lDistAverage, 0, 0);
  

  for( GLuint i = 0; i<lNbPt; i++)
    {
      COUT << "      ###### i  :  "<< i << "  " << (lBeginAngle+lStepAngle*i) * 180.0 / M_PI  <<  endl;
      
      PP3d::Mat4 lMatRot;        
      //      lMatRot.initRotAxis( lNormFac, lBeginAngle + lStepAngle * i);
      lMatRot.initRotY( lBeginAngle + lStepAngle * i);
      
      //  lMatSpin.initRotAxis( lNormToMove, lStep * i);
            // PP3d::Mat4 lMatTran = lMatReput  *  lMatSpin   * lMatZero;
      //      PP3d::Mat4 lMatTran = lMatReput * lMatGrow  * lMatRot *  lMatZero;
      //      lMatTran = lMatReput * lMatAlignInv, lMatRot * lMatAlign *  lMatZero;
      PP3d::Mat4 lMatTran = lMatReput * lMatAlignInv * lMatRot;
      
      ioPt[i]->get() = lOrigin* lMatTran;   
    }

  
  return true;
}
