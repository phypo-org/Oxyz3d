#ifndef H__ObjProps__H
#define H__ObjProps__H

#include "MinMaxBox3d.h"
#include "ColorRGBA.h"

namespace PP3d {
	
	//*********************************
	struct ObjProps {
	public:
		MinMaxBox3d cMinMaxBox;   
		Point3d     cCenter;
		ColorRGBA   cColor;
		bool        cVisible=true;

		GLuint      cGLNameId; // valeur courante de l'objet pour glName
		int         cHandleFind=-1;
	};
	//*********************************
}


#endif
