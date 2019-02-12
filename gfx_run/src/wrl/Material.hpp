
#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

// Material {
//   exposedField SFFloat ambientIntensity 0.2
//   exposedField SFColor diffuseColor 0.8 0.8 0.8
//   exposedField SFColor emissiveColor 0 0 0
//   exposedField SFFloat shininess 0.2
//   exposedField SFColor specularColor 0 0 0
//   exposedField SFFloat transparency 0
// }

#include "Node.hpp"

class Material : public Node {

private:

  float _ambientIntensity;
  Color _diffuseColor;
  Color _emissiveColor;
  float _shininess;
  Color _specularColor;
  float _transparency;

public:
  
  Material();
  virtual ~Material();

  float  getAmbientIntensity();
  Color& getDiffuseColor();
  Color& getEmissiveColor();
  float  getShininess();
  Color& getSpecularColor();
  float  getTransparency();

  void   setAmbientIntensity(float value);
  void   setDiffuseColor(Color& value);
  void   setEmissiveColor(Color& value);
  void   setShininess(float value);
  void   setSpecularColor(Color& value);
  void   setTransparency(float value);

  virtual bool isMaterial() const { return true; }

};

#endif // _MATERIAL_HPP_
