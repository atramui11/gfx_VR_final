
#include "AppSaver.hpp"

bool AppSaver::save(const char* filename, SceneGraph& wrl) {
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
      Saver* saver = _registry[ext];
      if(saver!=(Saver*)0)
        success = saver->save(filename,wrl);
    }
  }
  return success;
}

void AppSaver::registerSaver(Saver* saver) {
  if(saver!=(Saver*)0) {
    string ext(saver->ext()); // constructed from const char*
    pair<string,Saver*> ext_saver(ext,saver);
    _registry.insert(ext_saver);
  }
}
