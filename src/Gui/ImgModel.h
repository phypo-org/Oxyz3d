#ifndef H__IMGMODEL__H
#define H__IMGMODEL__H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/gl.h>
#include <FL/Fl_Shared_Image.H>	// fl_register_images()

#include <string>

namespace M3d {
	

  //************************************
  class ImgModel  {
    
  public:
    std::string cFilename;
    bool        cAlreadyInit=false;
  private:
    GLuint      cTexID=0;
    int         cW=0;
    int         cH=0;
  
  public:
    bool valid() { return cTexID != 0; }
 
  
    //---------------------------------------------------
    ImgModel( const std::string &  iFilename )
      :cFilename( iFilename )
    {
    }
    //---------------------------------------------------
 
    //---------------------------------------------------
    ~ImgModel() {
    
      if (cTexID != 0)
        {
          glDeleteTextures(1, &cTexID);
        }   
    }
    bool loadImage(const char* iFilename);
    bool loadTexture(const char* iFilename);
    bool loadTexture() { return loadTexture( cFilename.c_str()); }
    void draw();
    /*
    //---------------------------------------------------
    bool loadImage(const char* iFilename)
    {
      Fl_Shared_Image *cImage = Fl_Shared_Image::get(iFilename.c_str());		// load texturemap
      if ( ! cImage )
        {
          fprintf(stderr, "loadImage: %s: unknown image format\n",
                  iFilename.c_str());
          return false;
        }
      if ( cImage->d() != 3 )
        {
          fprintf(stderr, "loadImage: %s: image depth is %d (expected 3)\n",
                  iFilename.c_str(),  cImage->d());
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
    bool loadTexture()
    {
      if( cAlreadyInit ) return true;
      
      cAlreadyInit = true;
      
      glGenTextures(1, &cTexID);
      glBindTexture(GL_TEXTURE_2D, cTexID);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
      // Load texture image, bind to TexID
      if( loadImage() )
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
    void draw()
    {
      fprintf(stderr, "loadTexture:draw %d\n", (int) valid() );
      
      loadTexture();
      
      if ( valid() == false ) return;
    
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
    */
  };
    
  //************************************

  //---------------------------------------------------

}  // namespace






#endif
