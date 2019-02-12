
#include "ImageTexture.hpp"
// TODO ...


ImageTexture::ImageTexture() {}

ImageTexture::~ImageTexture() {
    delete this;
}

vector<string>& ImageTexture::getUrl() {
    return _url;
}
string ImageTexture::getUrl(int i) {
    return _url[i];
}

void ImageTexture::adToUrl(const string& str) {
    //if this is "AddtoUrl", then this code will work.
    _url.push_back(str);
}


















