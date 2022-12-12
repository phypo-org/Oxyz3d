#include "Modif.h"
#include "VisitorModif.h"

using namespace PP3d;

//-----------------------------------------------

void Modif::ExtrudePath( PP3d::DataBase & iBase, Selection & iSelect,  PP3d::Point3d lPtZero, PP3d::Point3d lAxis, FacetPtr iFacet )
{
  if( iFacet == nullptr ) return;


  std::cout << "Modif::ExtrudePath" << std::endl;

  
  for( LinePtr lLine : iFacet->getLines() )
    {      
      PP3d::SortEntityVisitorPointFacet lVisit;
      iSelect.execVisitorOnEntity( lVisit );
  
      std::vector<PP3d::EntityPtr> lNewFacets;

  std::cout << "Modif::ExtrudePath line " << std::endl;

      
      if( PP3d::Modif::SubDivAngle( &iBase, lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, PP3d::SubDivFacetType::ANGLE_FACET_MARGE, PP3d::SubDivSelectType::SELECT_CENTRAL, 0.01))
	{	  
  std::cout << "Modif::ExtrudePath SubDivAngle OK" << std::endl;
	  iSelect.removeAll();
	  iSelect.addGoodEntityFor(lNewFacets);
	  
	  
	  PP3d::Mat4 lMatTran;
	  lMatTran.identity();
	  lMatTran.initMove( lLine->getVector3d() );
	  
	  PP3d::SortEntityVisitorPoint lVisit;
	  iSelect.execVisitorOnEntity( lVisit );
	  for( PP3d::PointPtr lPoint : lVisit.cSetPoints  )
	    {
	      lPoint->get() = lPoint->get() * lMatTran;
	    }
	}
      }
  }
//-----------------------------------------------
void Modif::ExtrudePathNorm( PP3d::DataBase & iBase, Selection & iSelect,  PP3d::Point3d lPtZero, PP3d::Point3d lAxis, FacetPtr iFacet )
{
  if( iFacet == nullptr ) return;


  std::cout << "Modif::ExtrudePath" << std::endl;

  LinePtr Line0 = iFacet->getLines()[0];
  PP3d::Vector3d lVect0 ( 1, 0, 0 ); // axe X

  for( LinePtr lLine : iFacet->getLines() )
    {      
      PP3d::SortEntityVisitorPointFacet lVisit;
      iSelect.execVisitorOnEntity( lVisit );
  
      std::vector<PP3d::EntityPtr> lNewFacets;

      std::cout << "Modif::ExtrudePath line " << std::endl;

 
  
      if( PP3d::Modif::SubDivAngle( &iBase, lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, PP3d::SubDivFacetType::ANGLE_FACET_MARGE, PP3d::SubDivSelectType::SELECT_CENTRAL, 0.01))
	{	  
  std::cout << "Modif::ExtrudePath SubDivAngle OK" << std::endl;
	  iSelect.removeAll();
	  iSelect.addGoodEntityFor(lNewFacets);
	  
	  
	  PP3d::Mat4 lMatTran;
	  lMatTran.identity();
	  lMatTran.initMove( lLine->getVector3d() );

	  VisitorChgMat lVisit(iSelect, lVect0, lMatTran );
	  lVisit.setCoef(1);
	  lVisit.modifSelection(PP3d::VisitorModifPoints::Mode::MODIF, iSelect );
	}
      }
  }
//-----------------------------------------------
