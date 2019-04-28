#ifndef H__History__H
#define H__History__H




#include <vector>
#include <string>
#include <sstream>

#include "Shape/DataBase.h"

namespace M3d{
	
	// Il pourrait y avoir une sauvegarde allege
	// quand seul la saisie ou la selection on changé
	
	//La sauvegarde a lieu apres une modification
	// elles est faite par un thread et et protege par mutex
	// 
	// Pour la sauvegarde auto sur disque, ou la sauvegarde par l'utilisateur
	// on pourrait directement prendre le contenu de la derniere sauvegarde !
	// on pourrait aussi sauvegarde tout l'historique dans un repertoire


	// A terme faire une recopie profondes des objets ce sera plus efficae
	// et mettre un flag de modification dans Object du coup, sauvgarde que des objets modifié pas les autres 

  //************************************
  
  class History{

	public:
		
		enum class SaveMode { NoSav, Mini,  Diff, Full, Reset };
		
		static const char* SaveModeToStr( SaveMode iSav)
		{
			switch( iSav)
				{
				case SaveMode::NoSav: return "NoSav";
				case SaveMode::Mini: return "Mini";
				case SaveMode::Diff: return "Diff";
				case SaveMode::Full: return "Full";
				case SaveMode::Reset: return "Reset";
				}
			return "unknown";
		};
    //===================
    struct HistoryLink{
			SaveMode      cSaveMode=SaveMode::Mini;
      size_t        cIdLink=0;
      std::string   cName="no name";
      double        cTime=0;  //seconde.nano
      
			PP3d::EntityId      cSavId=0;
			
      std::stringstream cSavData;
			std::unique_ptr<PP3d::DataBase>  cuMyBase;

			const char* getStrSavMode() { return SaveModeToStr( cSaveMode ); }
			//Ajouter les saisie
			//Ajouter la selection et son type
			// point de vue ?

			//---------------------------
      HistoryLink( size_t iIdLink )
				:cIdLink( iIdLink )
      {
      }
      // ajouter cCurrentCreation
      // aj outer cCurrentLine
    };
    //===================
		int cMaxDepthness=100;    // -1 : infinity, 0 no histo

    std::vector<HistoryLink*>      cLinks;
    int                            cCurrentLink = -1; 
 
		PP3d::DataBase* internalRestore( int iPos );
		bool internalSave   ( PP3d::DataBase& iBase, SaveMode );

		//================================
	public:
		
    size_t size() { return cLinks.size();}
		std::vector<HistoryLink*> &getHistoLinks() { return cLinks; }

		bool save(  PP3d::DataBase& iBase, SaveMode  iSaveMode);
		bool restore( int iPos);
		bool reset( PP3d::DataBase& iBase );
		
		//************************************
	};
}

#endif
