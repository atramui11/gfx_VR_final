
#ifndef _APP_SAVER_HPP_
#define _APP_SAVER_HPP_

#include <map>
#include <string>
#include "Saver.hpp"

using namespace std;

class AppSaver {

public:

  AppSaver() {}
  ~AppSaver() {}

  bool save(const char* filename, SceneGraph& wrl);
  void registerSaver(Saver* saver);

private:

  map<string, Saver*> _registry;

};

#endif // _APP_SAVER_HPP_
