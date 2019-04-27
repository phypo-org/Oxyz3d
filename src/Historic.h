#ifndef H__HISTORIQUE__H
#define H__HISTORIQUE__H

#include "Application.h"

namespace M3d{
	
  //************************************
	// Contains old database copy and
	// all further modifications since next Link
	
	class HistoLink{
		
		DataBase*  cLink=nullptr; // can be null for the first ==  no data
		std::vector< std::string > cAfterLink; // command

	public:
		HistoLink();
		virtual HistoLink()
		{
			delete cLink;
		}			
	};
	
  //************************************
	// Contains all history links 
	
	class Historic
	{
		std::vector< HistoLink* > cAllHisto;
	
		int cMaxDepthness=100;    // -1 : infinity, 0 no histo
		
		int iLinkFrequency=50;    // make a Datacopy each iLinkFrequency
		int currentPosition=-1;   // 0-n. if -1 -> the last ! position
		
	public:

		Historic();
		
		virtual Historic( int iMaxDepthness=100, int iLinkFrequency=50)
		{
			for( HistoLink* lLink : cAllHisto )
				{
					delete lLink;
				}
		}
		
		bool pushBackLink(DataBase*  iLink );
		bool pushBackCmd( const std::string& iCmd );
		bool pushBack( const std::string& iCmd, DataBase*  iLink );

		bool forgetOldestLinkr();

		
		int  getCurrentHistoDepth();
		bool reward( int iStep=1 );
		bool foward( int iStep=1 );		
		bool gotohisto( int pPosition );
		
	}
  //************************************


} // end namespace

#endif
