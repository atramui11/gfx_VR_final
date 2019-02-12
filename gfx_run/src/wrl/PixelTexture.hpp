
#ifndef _PIXEL_TEXTURE_HPP_
#define _PIXEL_TEXTURE_HPP_

// PixelTexture {
//   exposedField SFImage image 0 0 0
//   field SFBool repeatS TRUE
//   field SFBool repeatT TRUE
// }

#include "Node.hpp"

class PixelTexture : public Node {

private:

  // we will not implement this field yet
  // Image _image;

  bool _repeatS;
  bool _repeatT;

public:
  
  PixelTexture();
  virtual ~PixelTexture();

  bool getRepeatS();
  bool getRepeatT();

  void setRepeatS(bool value);
  void setRepeatT(bool value);

  virtual bool isPixelTexture() const { return true; }

};

#endif // _PIXEL_TEXTURE_HPP_
