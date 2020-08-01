#ifndef H__Light__H
#define H__Light__H

#include "Point3d.h"
#include "ColorRGBA.h"


namespace PP3d {
  //******************************

	class Light{

	public:
		Point3dF   cPosition;
		//		Point3d cDirection;

		ColorRGBA  cAmbient;
		ColorRGBA  cDiffuse;
		ColorRGBA  cSpecular;

		bool cEnable;
		// Mettre un flag pour dire si avec la kamera
		

	protected:
		int  cLightId;

	public:
		Light();

		void raz();
		void disable();
		void GL();
		
		static const int MaxLight=8;
		static const int BAD_LIGHT=-1;
		static Light sLights[MaxLight];	


		static Light* GetNewLight();

		static int ConvLightToPos( int pLight );
		static int ConvPosToLight( int pPos );

		static void RazAll();
		static void DisableAll();
		static void RainbowOn();
		static void GreyOn();

		
		static void AllGL();
	};

  //******************************

}

#endif
