#ifndef _IFS_WRL_SAVER_HPP_
#define _IFS_WRL_SAVER_HPP_

#include "Saver.hpp"
#include <wrl/Shape.hpp>
#include <wrl/Appearance.hpp>
#include <wrl/Material.hpp>
#include <wrl/IndexedFaceSet.hpp>
#include <wrl/IndexedLineSet.hpp>
#include <wrl/ImageTexture.hpp>
#include <wrl/Transform.hpp>
#include <wrl/SceneGraphTraversal.hpp>

class SaverWrl : public Saver {

private:

const static char* _ext;

public:

  SaverWrl()  {};
  ~SaverWrl() {};

  bool  save(const char* filename, SceneGraph& wrl);
  const char* ext() const { return _ext; }

private:

  // using C-style FILE pointers and fprintf() to save the files

  void saveAppearance
  (FILE* fp, string indent, Appearance* appearance);
  void saveGroup
  (FILE* fp, string indent, Group* group);
  void saveImageTexture
  (FILE* fp, string indent, ImageTexture* imageTexture);
  void saveIndexedFaceSet
  (FILE* fp, string indent, IndexedFaceSet* indexedFaceSet);
  void saveIndexedLineSet
    (FILE* fp, string indent, IndexedLineSet* indexedLineSet) const;
  void saveMaterial
  (FILE* fp, string indent, Material* material);
  void saveShape
  (FILE* fp, string indent, Shape* shape);
  void saveTransform
  (FILE* fp, string indent, Transform* transform);

};

#endif // _IFS_WRL_SAVER_HPP_
