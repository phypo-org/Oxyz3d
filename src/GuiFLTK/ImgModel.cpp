
#include "ImgModel.h"

#include <iostream>


namespace M3d {


//---------------------------------------------------
bool ImgModel::loadImage(const char* iFilename)
{
  std::cout << "ImgModel::loadImage:" << iFilename << std::endl;

  Fl_Shared_Image *cImage = Fl_Shared_Image::get(iFilename);		// load texturemap
  if ( ! cImage )
    {
      std::cerr << "*** ERROR : ImgModel::loadImage:" << iFilename << " - load file failed for texture" << std::endl;

      return false;
    }
  if ( cImage->d() != 3 )
    {
      std::cerr << "*** ERROR : ImgModel::loadImage:" << iFilename << " - bad depth" << std::endl;
      cImage->release();
      return false;
    }
  cW = cImage->w();
  cH = cImage->h();
    
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cW, cH, 0,
               GL_RGB, GL_UNSIGNED_BYTE, cImage->data()[0]);
  cImage->release();
  return true;
}
//---------------------------------------------------
bool ImgModel::loadTexture(const char* iFilename)
{
  std::cout << "ImgModel::loadTexture:" << iFilename << std::endl;
  
  if( cAlreadyInit ) return true;
      
  cAlreadyInit = true;
      
  glGenTextures(1, &cTexID);
  std::cout << "ImgModel::loadTexture glGenTextures :" << cTexID<< std::endl;

  
  glBindTexture(GL_TEXTURE_2D, cTexID);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
  // Load texture image, bind to TexID
  if( loadImage(iFilename) )
    {
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glEnable(GL_TEXTURE_2D);
      // Lighting Init
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glShadeModel(GL_SMOOTH);
      // Misc OpenGL Init
      glEnable(GL_DEPTH_TEST);	// hidden surface
      glDepthFunc(GL_LEQUAL);
      return true;
    } 
    
  return false;
}
//---------------------------------------------------
void ImgModel::draw()
{
  //  fprintf(stderr, "loadTexture:draw %d\n", (int) valid() );
      
  //     loadTexture();
  //std::cout << "ImgModel::draw:" <<  valid() << std::endl;
    
  if ( valid() == false ) {
    loadTexture();
  }

  //  glClear(GL_COLOR_BUFFER_BIT);
    
  glBindTexture(GL_TEXTURE_2D, cTexID);
     
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
    
  fprintf(stderr, "XXXXXXXX %d %d\n", cW, cH);
    
  glTexCoord2f(0,1); glVertex2f(0, 0);
  glTexCoord2f(1,1); glVertex2f(cW, 0);
  glTexCoord2f(1,0); glVertex2f(cW, cH);
  glTexCoord2f(0,0); glVertex2f(0, cH);
    
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
} // namespace
//************************************



