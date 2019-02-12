
#ifndef _APP_LOADER_HPP_
#define _APP_LOADER_HPP_

// implements the Factory framework

#include <map>
#include <string>
#include "Loader.hpp"

using namespace std;

class AppLoader {

public:

  AppLoader() {}
  ~AppLoader() {}

  bool load(const char* filename, SceneGraph& wrl);
  void registerLoader(Loader* loader);

private:

  map<string, Loader*> _registry;

};

#endif // _APP_LOADER_HPP_
