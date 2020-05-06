#ifndef  H__ExportObj__H
#define  H__ExportOb__H

#include <ostream>
#include <istream>
#include <unordered_map>
#include <set>


#include "Entity.h"

// Wavefront 3D Object File

namespace PP3d {

  class DataBase;
	
  //*************************************
  class MyExportObj{


  public:
    std::ostream & cOut;

    std::unordered_map<EntityId,EntityId>  cHashId;
    EntityId                               cCurrentPointId=1;
											 

    MyExportObj( std::ostream& pOut )
      :cOut( pOut )
    {;}

    bool save( Object* lObject );
    bool save( DataBase& pData, std::set<Entity*> * iFilter = nullptr );		
  };
  //*************************************
  class MyImportObj{

    std::istream& cIn;
    std::string   cName = "import";

    public:
    MyImportObj( std::istream& pIn )
      :cIn( pIn )
    {
    }
    bool read( DataBase& pData, bool lConserverOldId = false );
    void setName( std::string & iName ) { cName = iName; }
  };
	
  //*************************************

	
}


#endif
