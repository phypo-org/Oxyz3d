#ifndef  H__Selection__H
#define  H__Selection__H


#include "Object.h"

#include "Matrice.h"

#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <memory>


#include "Entity.h"

#include "Utils/PPSingletonCrtp.h"


namespace PP3d {

	
  class DataBase;
  class Kamera;
  class Object;
	
	
  //****************************************
  struct PickingHit{
    GLuint  cName;
    PDouble cZ1;
    PDouble cZ2;

    Entity* cEntity=nullptr;  // Positionner avant le tri
    
    // Output via SelEncode::Decode
		
    PickingHit( GLuint pName, PDouble Z1, PDouble Z2 )
      :cName( pName), cZ1(Z1), cZ2( Z2 ), cEntity(nullptr)
    {			
    }
    PickingHit( const PickingHit& pP )
      :cName( pP.cName), cZ1(pP.cZ1), cZ2( pP.cZ2 ), cEntity( pP.cEntity)
    {			
    }

    void operator = (  const PickingHit& pP )
    {
      cName =  pP.cName;
      cZ1  =pP.cZ1;
      cZ2 = pP.cZ2 ;
      cEntity =  pP.cEntity;
    }
	    
    friend std::ostream& operator <<( std::ostream& pOs,  const PickingHit& pHit )
    {
      pOs << pHit.cName << " z:" << pHit.cZ1 << " -> " << pHit.cZ2 << " " << (void*) pHit.cEntity << std::flush ;
      pOs << (pHit.cEntity!=nullptr? pHit.cEntity->getType() : ShapeType::Null) ;
			
      return pOs;
    }
  };
	
  //****************************************
  class Selection :  public PPSingletonCrtp<Selection>
  {
    friend PPSingletonCrtp;
    
    EntityId cLastHightLightEntityId = 0;
 
  public:
    
    EntityId getLastHightLightEntityId() { return cLastHightLightEntityId; }

    //===========================
		
  private:
    Selection();
		
    //#define Selection::sTheSelection &Selection::
#define TheSelect PP3d::Selection::Instance()
		
  protected:
		
    SelectType     cSelectType = SelectType::Object;

    std::unordered_set<EntityPtr> cSelectObj;
    // METTRE AUSI UN VECTEUR POUR GARDER L ORDRE DE SELECTION
		
  public:
		
    void addEntity(  EntityPtr pEntity, bool iSelectAll=true );	
    void removeEntity( EntityPtr pEntity, bool iSelectAll=true );		
    void removeAll();
    void clear() {  cSelectObj.clear(); }

    bool addToSelection( EntityPtr lEntity);
    bool isSelected( EntityPtr lEntity )
    {
      return cSelectObj.find(lEntity) != cSelectObj.end() ;
    }
    const std::unordered_set<EntityPtr>& getSelection() { return cSelectObj;}
		

    size_t getNbSelected() { return cSelectObj.size();  }
									
    void getSelectPoints( std::unordered_set<Point*>& pPoints );


    void               changeSelectType( SelectType pType );
    SelectType&        getSelectType() { return  cSelectType; }
    static const char* GetStrSelectType(SelectType);

    //	void drawGL( DataBase& pDatabase, ViewProps& pViewProps );

    Point3d getCenter( DataBase& pDatabase );

    bool selectPickingHit( std::vector< PP3d::PickingHit > & pHits, DataBase& cBase, SelectMode& pSelectMode, bool pFlagOnlyHightlight );
    void deleteAllFromDatabase(DataBase& pDatabase);
		
    void addSelectionToInput( DataBase& pDatabase, bool pFlagLink );				

    //----------------------------
    friend std::ostream& operator <<( std::ostream& pOs, Selection& pSel )
    {
      pOs << "Type:" << GetStrSelectType( pSel.cSelectType ) << ":" << pSel.cSelectObj.size() << std::endl;
      for( auto lIter = pSel.cSelectObj.begin(); lIter != pSel.cSelectObj.end(); ++lIter) 
	{
	  pOs << "  " << (*lIter) << std::endl;
	}
      return pOs;
    }
    //----------------------------
    void execVisitorOnEntity(  EntityVisitor& pVisit )
    {
      for(  EntityPtr lEntity : cSelectObj )
	lEntity->execVisitor( pVisit );
    }
    //----------------------------
    void execVisitorOnlyOnObjects(  EntityVisitor& pVisit )
    {
      for(  EntityPtr lEntity : cSelectObj )
	{
	  //	  std::cout << " exec " <<  lEntity->getType() << " ? " << ShapeType::Object << std::endl;
	  if( lEntity->getType() == ShapeType::Object )
	    //	  if( lEntity->getType() == ShapeType::Object )
	    {
	      //	      std::cout << " Exec " << std::endl;
	      lEntity->execVisitor( pVisit );
	    }
	}
    }

  };
  //******************************************

}


#endif
	
