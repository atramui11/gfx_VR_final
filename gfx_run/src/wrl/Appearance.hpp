

#ifndef _APPEARANCE_HPP_
#define _APPEARANCE_HPP_

// Appearance {
//   exposedField SFNode material NULL
//   exposedField SFNode texture NULL
//   exposedField SFNode textureTransform NULL
// }

#include "Node.hpp"

class Appearance : public Node {

private:

  Node* _material;
  Node* _texture;

  // we will not implement this field
  //
  // Node* _textureTransform;

public:

  Appearance();
  virtual ~Appearance();

  Node* getMaterial();
  Node* getTexture();

  // we will not implement this field
  //
  // Node* getTextureTransform();

  void setMaterial(Node* material);
  void setTexture(Node* texture);

  // void setTextureTransform(Node* textureTransform);

  virtual bool isAppearance() const { return true; }

};

#endif // _APPEARANCE_HPP_
