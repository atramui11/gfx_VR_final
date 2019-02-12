
#ifndef _IMAGE_TEXTURE_HPP_
#define _IMAGE_TEXTURE_HPP_

// ImageTexture{
//   exposedField MFString url []
//   field SFBool repeatS TRUE
//   field SFBool repeatT TRUE
// }

#include <vector>
#include "PixelTexture.hpp"

class ImageTexture : public PixelTexture {

private:

  vector<string> _url;

public:
  
  ImageTexture();
  virtual ~ImageTexture();

  vector<string>& getUrl();
  string getUrl(int i);
  void adToUrl(const string& str);

  virtual bool isImageTexture() const { return true; }

};

#endif // _IMAGE_TEXTURE_HPP_
