#ifndef H__IMGMODEL__H
#define H__IMGMODEL__H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_PNG_Image.H>

namespace M3d {
	

  //************************************
  class ImgModel  {
  public:

  private:
    Fl_Image* cImage=nullptr;       //    Fl_PNG_Image* cImage;
    GLuint    cTexture=0;

  public:
    bool valid() { return cTexture != 0; }
 
  
  //---------------------------------------------------
  ImgModel()
  {
  }
  //---------------------------------------------------
  ~ImgModel() {
    
    if (cTexture)
      {
        glDeleteTextures(1, &cTexture);
      }
   
    delete cImage;
  }
  //---------------------------------------------------
  bool loadImage(const char* iFilename)
  {
    cImage = new Fl_PNG_Image(iFilename);
    
    if (cImage->w() > 0 && cImage->h() > 0)
      {      
        glGenTextures(1, &cTexture);
        glBindTexture(GL_TEXTURE_2D, cTexture);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cImage->w(), cImage->h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, cImage->data()[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        return true;
      }
 
    delete cImage;
    cImage = nullptr;
    
    fprintf(stderr, "Failed to load image %s\n", iFilename);
    
    return false;
  }
  //---------------------------------------------------
  void draw()
  {
    if ( cImage == nullptr ) return;
    
    //  glClear(GL_COLOR_BUFFER_BIT);
    
    glBindTexture(GL_TEXTURE_2D, cTexture);
    
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    
    fprintf(stderr, "XXXXXXXX %d %d\n", cImage->w(), cImage->h());
    
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(cImage->w(), 0);
    glTexCoord2f(1, 1); glVertex2f(cImage->w(), cImage->h());
    glTexCoord2f(0, 1); glVertex2f(0, cImage->h());
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }
};
//************************************

//---------------------------------------------------

}  // namespace






#endif
