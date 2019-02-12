
#include "PixelTexture.hpp"
// TODO ...

PixelTexture::PixelTexture() {
    _repeatS=1;
    _repeatT=1;
}

PixelTexture::~PixelTexture() {
    delete this;
}

bool PixelTexture::getRepeatS() {
    return _repeatS;
}

bool PixelTexture::getRepeatT() {
    return _repeatT;
}

void PixelTexture::setRepeatS(bool value) {
    _repeatS=value;
}

void PixelTexture::setRepeatT(bool value) {
    _repeatT=value;
}


















