#include "ExportObj.h"

#include "DataBase.h"

#include "ObjectLine.h"
#include "ObjectFacet.h"
#include "ObjectPolylines.h"
#include "ObjectPoly.h"


#include <sstream>


namespace PP3d {

  //*************************************
  class VisitorSavPoints : public EntityVisitor	{
		
    MyExportObj & cObj;
		
  public:
    VisitorSavPoints( MyExportObj& pObj )
      :cObj(pObj) {;}
		
    void execPoint( Point* pPoint ) override
    {
      DBG4(  'v'
	     << ' ' << pPoint->get().cX
	     << ' ' << pPoint->get().cY
	     << ' ' << pPoint->get().cZ 
	);
    cObj.cHashId.insert( { pPoint->getId(), cObj.cCurrentPointId++} );
  }
};
//*************************************
	
class VisitorSavFacets : public EntityVisitor	{
		
  MyExportObj & cObj;

		
public:
  VisitorSavFacets(  MyExportObj& pObj )
    :cObj(pObj) {;}

		
  void execPoint( Point* pPoint ) override
  {
    auto lIter = cObj.cHashId.find( pPoint->getId() );
    if( lIter ==  cObj.cHashId.end() )
      {
	return ;
      }
    cObj.cOut << ' ' << lIter->second << "//";
  }
  void execBeginFacet(  Facet* pFacet ) override
  {
    cObj.cOut << 'f';
  }
  virtual void execEndFacet( Facet* pFacet )
  {
    cObj.cOut << std::endl;
  }
};
//*************************************

// On pourrait mettre des meta information
// dans le nom et/ou les commentaires 
//--------------------------------------

bool MyExportObj::save( Object* lEntity )
{				
  DBG3( "   ExportObj::save Entity" );
  switch( lEntity->getObjType() )
    {
    case ObjectType::ObjPoly:
      {
	DBG3( "      ExportObj::save Poly" );

	auto lEntityPoly = dynamic_cast<ObjectPoly*>(lEntity);
	DBG1( "o " << lEntity->getName() << '_' <<  lEntity->getObjType() );
									
      VisitorSavPoints lVisPts( *this );
      lEntityPoly->getPoly()->execVisitor( lVisPts );
      cOut << 'g' << lEntity->getName() << '_' <<  lEntity->getObjType() << std::endl;
  cOut <<  "usemtl default" << std::endl;
  cOut << "s 1" <<  std::endl;
VisitorSavFacets lVisFacs( *this );
lEntityPoly->getPoly()->execVisitor( lVisFacs );														}
break;	
 default: ;
}
return true;
}
//--------------------------------------

bool MyExportObj::save( DataBase& pData )
{
  DBG3( "ExportObj::save" );

					    
  auto lEntities = pData.getEntities();
		
  for( auto lPairEntity :  lEntities )
    {				
      if( lPairEntity.second->getType() != ShapeType::Object )
	continue;
					
      auto lEntity = dynamic_cast<Object*>(lPairEntity.second);
      save( lEntity );
    }
  return true;
}
 
//*************************************
//*************************************
//*************************************

#define ReadEndLine		std::getline(	cIn,  lEndOfLine, '\n' )

//--------------------------------
bool MyImportObj::read( DataBase& pData, bool pConserveOldId  )
{
  std::unordered_map<EntityId,EntityId>  lHashId;
  try {				
    std::string lEndOfLine; // pour lire la fin de ligne 

    std::string lNameObj;
    std::string lName;
    Poly * lPoly = nullptr;
    EntityId lCurrentPointId =1;
				
    while( cIn.good() )
      {
	std::string lToken;
	cIn >> lToken;
	//				std::getline( cIn, lToken, ' ' );						
	DBG3( "Token: <<" << lToken << ">> ");
						
	      if( lToken.size() == 0 || lToken[0] == '#' )
		{
		  ReadEndLine;
		  continue;
		}
	      if( lToken == "g" )
		{
		  cIn >> lNameObj;
		  if( lPoly != nullptr )
		    {
		      ObjectPoly * lObj = new ObjectPoly( lNameObj, lPoly );				
		      pData.addObject( lObj );
		      //	lPoly = nullptr;
		    }
		  DBG3( "NameObj=" << lNameObj );
		  ReadEndLine;
		}
	      else	if( lToken == "o")
		{
		  cIn >> lName;
		  ReadEndLine;
		  lPoly = new Poly();
		  DBG3( "Name=" << lName );
		}															
	      else	if( lToken == "v")
		{
		  PDouble  lX,lY,lZ;
		  cIn >> lX >> lY >> lZ;
								
						
		  DBG3( "Point X:" << lX << ":" << lY << ":" << lZ );							
								
								
			Point* lPoint = new Point( Point3d( lX, lY, lZ ) );
			pData.validOneEntityLevel(lPoint);
			lHashId.insert( { lCurrentPointId, lPoint->getId() } );
			lCurrentPointId++;
								
			DBG3( "createPoint " <<lCurrentPointId-1 << " " <<  *lPoint );
			ReadEndLine;
			}
		  else	if( lToken == "f")
		    {		
		      ReadEndLine;
		      std::istringstream iStr( lEndOfLine );

		      DBG3( "readFacet (" << lEndOfLine << ")" );
								
			    Facet* lFacet = new Facet();
			    Line* lLine = nullptr;
								
			    //===================
			    int i=0;
			    while(  iStr.eof() == false )
			      {
				EntityId lPointId=0;

				char c1, c2;
				std::string lStr;
				iStr >> lPointId >> c1 >> c2 ;
				if( lPointId == 0)
				  continue;
										
				DBG3( "    PointId:" << lPointId << " " << c1 << c2 );

									
				//		std::getline( cIn, lStr, '/' );
				//			std::getline( cIn, lStr, '/' );		

				auto lIter = lHashId.find( lPointId);
										
				if( lIter ==  lHashId.end() )
				  {
				    std::cerr << "PointId " << lPointId << " not found " << std::endl;
				return false;
			      }
										
			    EntityId lPointIdDB = lIter->second ;
									
										
			    EntityPtr lPtr = pData.findEntity( lPointIdDB );
			    if( lPtr == nullptr )
			      {
				std::cerr << "Point " << lPointIdDB << " not found " << std::endl;
			    return false;
			    }
			DBG3( "point : " <<  lPointIdDB << " found" );
										
		      if( lLine == nullptr )
			{
			  lLine = new Line( (PointPtr)lPtr, (PointPtr)lPtr ); 
			  DBG3( "createLine ");
			}
		      else
			{
			  lLine->second() =  (PointPtr)lPtr;
			  lFacet->addLine( lLine );
			  lLine = new Line(  (PointPtr)lPtr, (PointPtr)lPtr  );
			  DBG3( "createLine2 " );
			}
		      i++;
		    }
		  //===================
		  DBG3( " nbline:" << i );
		  lFacet->closeFacet();
		  lPoly->addFacet( lFacet );
		}
	      else
		{
		  ReadEndLine;
		  std::cerr << "unknown command : " << lToken	<< ":" << lEndOfLine << std::endl;
		}	
	      }
      }	catch( const std::exception & lEx ) {				
      std::cerr << __FILE__ << ":" << __LINE__ << ":Exception " << lEx.what() << std::endl;
    }
			
    return true;
  }
  //*************************************

};
