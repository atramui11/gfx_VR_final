
#ifndef _LOADER_WRL_HPP_
#define _LOADER_WRL_HPP_

#include "Loader.hpp"
#include "Tokenizer.hpp"

// TODO : use your own tokenizer
// #include "Tokenizer.hpp"

#include <wrl/Transform.hpp>
#include <wrl/Shape.hpp>
#include <wrl/Appearance.hpp>
#include <wrl/Material.hpp>
#include <wrl/ImageTexture.hpp>
#include <wrl/IndexedFaceSet.hpp>
#include <wrl/IndexedLineSet.hpp>

class LoaderWrl : public Loader {

private:

  const static char* _ext;

public:

  LoaderWrl()  {};
  ~LoaderWrl() {};

  const char* ext() const { return _ext; }
  bool  load(const char* filename, SceneGraph& wrl);

private:

  // TODO :
  // This is how I decided to implement this class
  // Feel free to structure your code in a different way

  bool loadSceneGraph(Tokenizer& tkn,  SceneGraph& wrl);
  bool loadGroup(Tokenizer& tkn, Group& group);
  bool loadTransform(Tokenizer& tkn, Transform& transform);
  bool loadShape(Tokenizer& tkn, Shape& transform);
  bool loadAppearance(Tokenizer& tkn, Appearance& appearance);
  bool loadMaterial(Tokenizer& tkn, Material& material);
  bool loadImageTexture(Tokenizer& tkn, ImageTexture& imageTexture);
  bool loadIndexedFaceSet(Tokenizer& tkn, IndexedFaceSet& ifs);
    bool loadChildren(Tokenizer& tkn, Group& group);
    bool loadIndexedLineSet(Tokenizer& tkn, IndexedLineSet& ifs);
    bool loadVecFloat(Tokenizer& tkn,vector<float>& vec);
    bool loadVecInt(Tokenizer &tkn,vector<int>& vec);
    bool loadVecString(Tokenizer &tkn,vector<string>& vec);

};

#endif // _LOADER_WRL_HPP_
