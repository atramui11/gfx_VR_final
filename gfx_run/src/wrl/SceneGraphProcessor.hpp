
#ifndef _SCENE_GRAPH_PROCESSOR_HPP_
#define _SCENE_GRAPH_PROCESSOR_HPP_

#include <iostream>
#include "SceneGraph.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include "IndexedLineSet.hpp"
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;

class SceneGraphProcessor {

public:

  SceneGraphProcessor(SceneGraph& wrl);
  ~SceneGraphProcessor();

  void normalClear();
  void normalInvert();
  void computeNormalPerFace();
  void computeNormalPerVertex();
  void computeNormalPerCorner();

  void bboxAdd(int depth=0, float scale=1.0f, bool cube=true);
  void bboxRemove();
  bool hasBBox();

  void edgesAdd();
  void edgesRemove();
  bool hasEdges();

  bool hasIndexedFaceSetFaces();
  bool hasIndexedFaceSetNormalNone();
  bool hasIndexedFaceSetNormalPerFace();
  bool hasIndexedFaceSetNormalPerVertex();
  bool hasIndexedFaceSetNormalPerCorner();

  bool hasIndexedLineSetColorNone();
  bool hasIndexedLineSetColorPerVertex();
  bool hasIndexedLineSetColorPerPolyline();

  bool hasIndexedFaceSetShown();
  bool hasIndexedFaceSetHidden();
  bool hasIndexedLineSetShown();
  bool hasIndexedLineSetHidden();

  void shapeIndexedFaceSetShow();
  void shapeIndexedFaceSetHide();
  void shapeIndexedLineSetShow();
  void shapeIndexedLineSetHide();

  void removeSceneGraphChild(const string& name);
  void pointsRemove();
  void surfaceRemove();

  void fitSinglePlane
  (const Vec3f& center, const Vec3f& size, const float scale, const bool cube,
   Vec4f& f /* temporary output */ );

  void fitMultiplePlanes
  (const Vec3f& center, const Vec3f& size,
   const int depth, const float scale, const bool cube,
   vector<float>& fVec );

  void fitContinuous
  (const Vec3f& center, const Vec3f& size,
   const int depth, const float scale, const bool cube);

    //assignment 12 methods
    void fitWatertight (const Vec3f& center, const Vec3f& size,
                        const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */);
    
    void computeIsosurface (const Vec3f& center, const Vec3f& size,
                        const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */);
    
    void optimalCGHard (const Vec3f& center, const Vec3f& size,
                            const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */);
    
    void optimalCGSoft (const Vec3f& center, const Vec3f& size,
                        const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */);
    
    void optimalJacobi (const Vec3f& center, const Vec3f& size,
                        const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */);
    
    void multiGridFiner (const Vec3f& center, const Vec3f& size,
                        const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */);
    
    void multiGridCoarser (const Vec3f& center, const Vec3f& size,
                         const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */);
    
private:

  SceneGraph&    _wrl;

  // computation grid and point set partition
  int  _nGrid;
  int  _nPoints;
  int* _next;
  int* _first;
  int  _nCells() { return _nGrid*_nGrid*_nGrid; }
  int  _createPartition(Vec3f& min, Vec3f& max, int depth, vector<float>&coord);
  void _deletePartition();


  void _applyToIndexedFaceSet(IndexedFaceSet::Operator p);
    void meanFit(const vector<float>& coordPoints, const vector<float>& normalPoints, const Vec3f& min, const Vec3f& max, Vec4f& f);
    void eigenFit(const vector<float>& coordPoints, const Vec3f& min, const Vec3f& max, Vec4f& f);

  // IndexedFaceSet::Operator
  static void _normalClear(IndexedFaceSet& ifs);
  static void _normalInvert(IndexedFaceSet& ifs);
  static void _computeNormalPerFace(IndexedFaceSet& ifs);
  static void _computeNormalPerVertex(IndexedFaceSet& ifs);
  static void _computeNormalPerCorner(IndexedFaceSet& ifs);

  static void _computeFaceNormal
              (vector<float>& coord, vector<int>&   coordIndex,
               int i0, int i1, Vector3d& n, bool normalize);

  bool        _hasShapeProperty(Shape::Property p);
  bool        _hasIndexedFaceSetProperty(IndexedFaceSet::Property p);
  bool        _hasIndexedLineSetProperty(IndexedLineSet::Property p);

  // Shape::Property
  static bool _hasEdges(Shape& shape);
  static bool _hasIndexedFaceSetShown(Shape& shape);
  static bool _hasIndexedFaceSetHidden(Shape& shape);
  static bool _hasIndexedLineSetShown(Shape& shape);
  static bool _hasIndexedLineSetHidden(Shape& shape);

  // IndexedFaceSet::Property
  static bool _hasFaces(IndexedFaceSet& ifs);
  static bool _hasNormalNone(IndexedFaceSet& ifs);
  static bool _hasNormalPerFace(IndexedFaceSet& ifs);
  static bool _hasNormalPerVertex(IndexedFaceSet& ifs);
  static bool _hasNormalPerCorner(IndexedFaceSet& ifs);
    
  // IndexedLineSet::Property
  static bool _hasColorNone(IndexedLineSet& ils);
  static bool _hasColorPerVertex(IndexedLineSet& ils);
  static bool _hasColorPerPolyline(IndexedLineSet& ils);

  IndexedFaceSet* _getNamedShapeIFS(const string& name, bool create);
};

#endif // _SCENE_GRAPH_PROCESSOR_HPP_
