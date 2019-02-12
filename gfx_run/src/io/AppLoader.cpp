
#include "AppLoader.hpp"

// implements the Factory framework

bool AppLoader::load(const char* filename, SceneGraph& wrl) {
  bool success = false;
  if(filename!=(const char*)0) {
    // int n = (int)strlen(filename);
    int n = string(filename).size();
    int i;
    for(i=n-1;i>=0;i--)
      if(filename[i]=='.')
        break;
    if(i>=0) {
      string ext(filename+i+1);
      Loader* loader = _registry[ext];
      if(loader!=(Loader*)0)
        success = loader->load(filename,wrl);
    }
  }
  return success;
}

void AppLoader::registerLoader(Loader* loader) {
  if(loader!=(Loader*)0) {
    string ext(loader->ext()); // constructed from const char*
    pair<string,Loader*> ext_loader(ext,loader);
    _registry.insert(ext_loader);
  }
}
