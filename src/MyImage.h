#ifndef H__MyImage__H
#define H__MyImage__H


#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_JPEG_Image.H>

namespace M3d {

	//********************************************
	struct MyImage {
	  /*
		static Fl_Image* LoadImage( const char *pName, int pSize )
		{
			Fl_Image* lTmp = new Fl_PNG_Image(pName);
			if( lTmp->w() == pSize )
				return lTmp;

			
			Fl_Image* lTmp2  = lTmp->copy( pSize, pSize );
			delete lTmp;
			
			return lTmp2;
		}
	  */
		static Fl_RGB_Image* LoadImage( const char *pName, int pSize )
		{
			Fl_PNG_Image* lTmp = new Fl_PNG_Image(pName);
			if( lTmp->w() == pSize )
				return lTmp;

			
			Fl_PNG_Image* lTmp2  = (Fl_PNG_Image*)lTmp->copy( pSize, pSize );
			delete lTmp;
			
			return lTmp2;
		}

	};
	//********************************************
	
};


#endif
