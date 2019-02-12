
#include "Material.hpp"

// TODO ...


Material::Material() {
    _ambientIntensity=0.0;
    _shininess=0.0;
    _transparency=0.0;
}

Material::~Material() {
    delete this;
}

float Material::getAmbientIntensity() {
    return _ambientIntensity;
}

Color& Material::getDiffuseColor() {
    return _diffuseColor;
}

Color& Material::getEmissiveColor() {
    return _emissiveColor;
}

float Material::getShininess() {
    return _shininess;
}

Color& Material::getSpecularColor() {
    return _specularColor;
}

float Material::getTransparency() {
    return _transparency;
}

void Material::setAmbientIntensity(float value) {
    _ambientIntensity=value;
}

void Material::setDiffuseColor(Color& value) {
    _diffuseColor=value;
}
void Material::setEmissiveColor(Color& value) {
    _emissiveColor=value;
}
void Material::setShininess(float value) {
    _shininess=value;
}
void Material::setSpecularColor(Color& value) {
    _specularColor=value;
}
void Material::setTransparency(float value) {
    _transparency=value;
}













