
#include <iostream>
#include "SceneGraphProcessor.hpp"
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include "IndexedLineSet.hpp"
#include "util/SimpleGraphMap.hpp"
#include "Appearance.hpp"
#include "Material.hpp"
#include "IsoSurf.hpp"

using namespace Eigen;

SceneGraphProcessor::SceneGraphProcessor(SceneGraph& wrl):
  _wrl(wrl),
  _nGrid(0),
  _nPoints(0),
  _next((int*)0),
  _first((int*)0) {
}

SceneGraphProcessor::~SceneGraphProcessor() {
  _deletePartition();
}

int SceneGraphProcessor::_createPartition
(Vec3f& min, Vec3f& max, int depth, vector<float>&coord) {
  int nPointsInPartition = 0;
  _deletePartition();
  float dx = max.x-min.x;
  float dy = max.y-min.y;
  float dz = max.z-min.z;
  if(dx>0.0f && dy>0.0f && dz>0.0f) {
    _nPoints = (int)(coord.size()/3);
    if(depth<0) depth = 0;
    _nGrid = 1<<depth;
    int nCells  = _nCells();
    _next    = new int[_nPoints];
    _first   = new int[nCells];
    int iCell,iPoint,ix,iy,iz;
    float x,y,z,nG=(float)_nGrid;
    for(iCell=0;iCell<nCells;iCell++)
      _first[iCell] = -1;
    for(iPoint=0;iPoint<_nPoints;iPoint++) {  
      if((x = coord[3*iPoint  ])<min.x || x>max.x) continue;
      if((y = coord[3*iPoint+1])<min.y || y>max.y) continue;
      if((z = coord[3*iPoint+2])<min.z || z>max.z) continue;
      ix = (int)((nG*(x-min.x))/dx);
      iy = (int)((nG*(y-min.y))/dy);
      iz = (int)((nG*(z-min.z))/dz);
      iCell = ix+_nGrid*(iy+_nGrid*iz);
      _next[iPoint] = _first[iCell];
    _first[iCell] = iPoint;
    nPointsInPartition++;
    }
  }
  return nPointsInPartition;
}

void SceneGraphProcessor::_deletePartition() {
  if(_first!=(int*)0) delete [] _first;
  if(_next !=(int*)0) delete [] _next;
  _nGrid   = 0;
  _nPoints = 0;
  _next    = (int*)0;
  _first   = (int*)0;
}

void SceneGraphProcessor::normalClear() {
    _applyToIndexedFaceSet((void(*)(Node &))_normalClear);
}

void SceneGraphProcessor::normalInvert() {
  _applyToIndexedFaceSet((void(*)(Node &))_normalInvert);
}

void SceneGraphProcessor::computeNormalPerFace() {
  _applyToIndexedFaceSet((void(*)(Node &))_computeNormalPerFace);
}

void SceneGraphProcessor::computeNormalPerVertex() {
  _applyToIndexedFaceSet((void(*)(Node &))_computeNormalPerVertex);
}

void SceneGraphProcessor::computeNormalPerCorner() {
  _applyToIndexedFaceSet((void(*)(Node &))_computeNormalPerCorner);
}

void SceneGraphProcessor::_applyToIndexedFaceSet(IndexedFaceSet::Operator o) {
  SceneGraphTraversal traversal(_wrl);
  traversal.start();
  Node* node;
  while((node=traversal.next())!=(Node*)0) {
    if(node->isShape()) {
      Shape* shape = (Shape*)node;
      node = shape->getGeometry();
      if(node!=(Node*)0 && node->isIndexedFaceSet()) {
        IndexedFaceSet& ifs = *((IndexedFaceSet*)node);
        o(ifs);
      }
    }
  }
}

void SceneGraphProcessor::_normalClear(IndexedFaceSet& ifs) {
  vector<float>& normal      = ifs.getNormal();
  vector<int>&   normalIndex = ifs.getNormalIndex();
  ifs.setNormalPerVertex(true);
  normal.clear();
  normalIndex.clear();
}

void SceneGraphProcessor::_normalInvert(IndexedFaceSet& ifs) {
  vector<float>& normal = ifs.getNormal();
  for(int i=0;i<(int)normal.size();i++)
    normal[i] = -normal[i];
}

void SceneGraphProcessor::_computeFaceNormal
(vector<float>& coord, vector<int>& coordIndex,
 int i0, int i1, Vector3d& n, bool normalize) {
  // TODO
  // compute normal vector to face defined by the vertex indices
    if (coordIndex.size()==0) return;
    //coordindex has vertex indices
    
    //this is how to acccess all vertex indices of this face
    int iV, i;
    float x0,x1,x2,xVecCount = 0.0,yVecCount = 0.0,zVecCount = 0.0;
    
    
    for (i=i0; i<i1; i++) {
        //i goes upto i1 and iterate thru whole face (i1 last place hit -1)
        iV=coordIndex[i]; //iV is start index of shape to bind to
        
        if (iV==-1) {return;}//end of face
        
        //p0
        x0=coord[3*iV];
        x1=coord[3*iV+1];
        x2=coord[3*iV+2];
        
        xVecCount+=x0;
        yVecCount+=x1;
        zVecCount+=x2;
        //need more than one face vertex to compute face normal
    }
    
    //normalize the summed "vector"
    if (normalize) {
        float tempnormN=i1;
        xVecCount=xVecCount/tempnormN;
        yVecCount=yVecCount/tempnormN;
        zVecCount=zVecCount/tempnormN;
    }
    
    
    n.x()=xVecCount;
    n.y()=yVecCount;
    n.z()=zVecCount;
    
    
  // {coordIndex[i0],...,coordIndex[i1-1]}

  // store the result in the 3D vector n

  // normalize to unit length or not, depending on the 'normalize' variable 

    
    
}

void SceneGraphProcessor::_computeNormalPerFace(IndexedFaceSet& ifs) {
  if(ifs.getNormalBinding()==IndexedFaceSet::PB_PER_FACE) return;
    
    vector<float>& coord = ifs.getCoord();
    vector<int>& coordIndex=ifs.getCoordIndex();
    vector<float>& normal= ifs.getNormal();
    vector<int>& normalIndex= ifs.getNormalIndex();
    normal.clear();
    normalIndex.clear();
    ifs.getNormalPerVertex()=false;
    
    Vector3d norm;
    int iF, i0, i1;
    float n0 = 0.0, n1 = 0.0, n2 = 0.0;
    norm.x()=n0;
    norm.y()=n1;
    norm.z()=n2;
    
    for (iF=i0=i1=0; i1<coordIndex.size(); i1++) {
        if (coordIndex[i1]<0) { //end of face marker found
            //face iF
        //compute n0, n1, n2 as function of coordinates of all the
        //vertices of this face
        
        _computeFaceNormal(coord, coordIndex, i0, i1, norm, true);
        
        //push back() the three values onto the normal array
        normal.push_back(norm.x());
        normal.push_back(norm.y());
        normal.push_back(norm.z());
        
        i0=i1+1; iF++;
            
        }
    }
   //iF== number of faces here
}

void SceneGraphProcessor::_computeNormalPerVertex(IndexedFaceSet& ifs) {
  if(ifs.getNormalBinding()==IndexedFaceSet::PB_PER_VERTEX) return;
  // TODO
    vector<float>& coord = ifs.getCoord();
    vector<int>& coordIndex=ifs.getCoordIndex();
    vector<float>& normal= ifs.getNormal();
    vector<int>& normalIndex= ifs.getNormalIndex();
    int nV = coordIndex.size();
    
    normal.clear();
    normalIndex.clear(); // will not be used
    ifs.getNormalPerVertex()=false;
    
    // compute face normals without normalization
    // ...
    // save the computed face normal and clear the ifs normals
    
    vector<float> faceNormal;
    faceNormal.swap(normal);
    // at this point normal.size()==0
    // insert coord.size() zeros onto the normal array
    
    // at this point coord.size()==normal.size();
    ifs.getNormalPerVertex()=true;
    
    int iF,iV,i,i0,i1;
    Vector3d norm;
    
    //first fill up facenormal
    for(iF=i0=i1=0;i1<coordIndex.size();i1++) {
        if(coordIndex[i1]<0) { // end of face marker found
            _computeFaceNormal(coord, coordIndex, i0, i1, norm, false);
            faceNormal.push_back(norm.x()); faceNormal.push_back(norm.y()); faceNormal.push_back(norm.z());
            i0 = i1+1; iF++;
        }
    }
    
    
    for(iF=i0=i1=0;i1<coordIndex.size();i1++) {
        if(coordIndex[i1]<0) { // end of face marker found
            // face iF
            // retrieve this face normal coordinates from the faceNormal array
            for(i=i0;i<i1;i++) {
                int iV1 = coordIndex[i];
                // add face normal coordinates to this vertex normal accumulator
                normal.push_back(faceNormal.at(3*iV1));
                normal.push_back(faceNormal.at(3*iV1+1));
                normal.push_back(faceNormal.at(3*iV1+2));
            }
            
            i0 = i1+1; iF++;
        }
    }
    
    //traverse normal
    for(iV=0;iV<nV;iV++) {
        // normalize vertex normal to unit length
        float fnV=(float) nV;
        normal.at(3*iV)=(normal.at(3*iV))/fnV;
    }
    
}


void SceneGraphProcessor::_computeNormalPerCorner(IndexedFaceSet& ifs) {
  if(ifs.getNormalBinding()==IndexedFaceSet::PB_PER_CORNER) return;
      // TODO
    vector<float>& coord = ifs.getCoord();
    vector<int>& coordIndex=ifs.getCoordIndex();
    vector<float>& normal= ifs.getNormal();
    vector<int>& normalIndex= ifs.getNormalIndex();
    
    normal.clear();
    normalIndex.clear();
    ifs.getNormalPerVertex()=true;
    
    Vector3d norm;
    int iF,iV0,iV1,iV2,i,i0,i1,iN;
    float x0,x1,x2,y0,y1,y2,z0,z1,z2;
    for(iF=i0=i1=0;i1<coordIndex.size();i1++) {
        if(coordIndex[i1]<0) { // end of face marker found
            // face iF
            // this is how you can access all the vertex indices of this face
            for(i=i0;i<i1;i++) {
                if (i==0) {iV0=coordIndex[i1-1];}
                if (i==(i0-1)) {iV2=coordIndex[i0];}
                
                iV0 = coordIndex[i-1]; // [i1-1] if i==0
                iV1 = coordIndex[i];
                iV2 = coordIndex[i+1]; // [i0] if i==i0-1
                
                // compute unit length corner normal ...
                //p0
                x0=coord[3*iV0];
                x1=coord[3*iV0+1];
                x2=coord[3*iV0+2];
                
                //p1
                y0=coord[3*iV1];
                y1=coord[3*iV1+1];
                y2=coord[3*iV1+2];
                
                //p2
                z0=coord[3*iV2];
                z1=coord[3*iV2+1];
                z2=coord[3*iV2+2];
                
                //determinant
                float n1temp=(x1*y2-x2*y1);
                float n2temp=(-(x0*y2-x2*y0));
                float n3temp=(x0*y1-x1*y0);
              
                
                iN = normal.size()/3; // index to be assigned to this normal vector
                // push_back computed normal coordinates onto normal array
                normal.push_back(n1temp);
                normal.push_back(n2temp);
                normal.push_back(n3temp);
                
                // push_back iN onto normalIndex array
                normalIndex.push_back(iN);
            }

            // push_back a -1 onto the normalIndex array
            normalIndex.push_back(-1);
            
            i0 = i1+1; iF++;
        }
        
    }
}

void SceneGraphProcessor::bboxAdd(int depth, float scale, bool cube) {
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    
    IndexedLineSet* ils;
    Node* node;
    bool bboxCheck=0;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            // bbox set already
            if (node->getName()=="BOUNDING-BOX") {bboxCheck=1;}
        
                /*Shape* shape= (Shape*) node;
                node=shape->getGeometry();
                 
                //IFS OR ILS???
                if (node->isIndexedLineSet()) {
                    delete node;
                    Shape* shape= (Shape*) node;
                    shape->setGeometry((Node*)0);
                    node=shape->getGeometry();}
                
                //empty node, no bbox
                else if (node==(Node*)0) {
                    ils= new IndexedLineSet();
                    ils->clear();
                    _wrl.updateBBox();}*/

        }
    }
    
    //no bbox node already, time to create & insert one
    if (!bboxCheck) {
        Shape* bbox= new Shape();
        ils = new IndexedLineSet();
        bbox->setGeometry(ils);
        
        bbox->setName("BOUNDING-BOX");
        
        Appearance* appr= new Appearance();
        Material* matr= new Material();
        Color diffCol=matr->getDiffuseColor();
        diffCol=*(new Color());
        
        appr->setMaterial(matr);
        bbox->setAppearance(appr);
        
        _wrl.addChild(bbox);
    
    
        //store vertices of grid in ils
        vector<float> & _ilscoord = ils->getCoord();
        vector<int> & _ilsCoordIndex = ils->getCoordIndex();
        _ilscoord.clear();
        _ilsCoordIndex.clear();
    
        int N=pow(2, depth);
    
        //get bbox data
        Vec3f _bboxCenter =_wrl.Group::getBBoxCenter();
        Vec3f _bboxSize = _wrl.Group::getBBoxSize();
    
        float xSize=_bboxSize.x;
        float ySize=_bboxSize.y;
        float zSize=_bboxSize.z;
    
        float x0,y0,z0;
    
        //fill min corner x0,y0,z0
        x0=_bboxCenter.x-xSize/2.0f;
        y0=_bboxCenter.y-ySize/2.0f;
        z0=_bboxCenter.z-zSize/2.0f;

        //int numVertices=(N+1)*(N+1)*(N+1);
        float dx = xSize/((float)(N));
        float dy = ySize/((float)(N));
        float dz = zSize/((float)(N));
        
        
        //if cube get max dimension and make all equal to that
        float cubeSize;
        if (cube) {
            float temp;
            temp=max(xSize, ySize);
            cubeSize=max(temp, zSize);
            x0=_bboxCenter.x-cubeSize/2.0f;
            y0=_bboxCenter.y-cubeSize/2.0f;
            z0=_bboxCenter.z-cubeSize/2.0f;
            dx=cubeSize/((float)(N)); dy=cubeSize/((float)(N)); dz=cubeSize/((float)(N));
        }
        
        //scale the bounding box
        dx*=scale;
        dy*=scale;
        dz*=scale;
        
        //create new loop that loops thru vertices themselves and adds them to _coord
        for (int i=0; i<=N; i++)
        {
            for (int j=0; j<=N; j++)
            {
                for (int k=0; k<=N; k++)
                {
                    // coordinates
                    //i, j, k are off center, match them to bbox min corner
                    float xcoord=i*dx+x0;
                    float ycoord=j*dy+y0;
                    float zcoord=k*dz+z0;
                
                    _ilscoord.push_back(xcoord);
                    _ilscoord.push_back(ycoord);
                    _ilscoord.push_back(zcoord);
                    
                    // edges
                    // (i,j,k)->(i+1,j,k)
                    if(i<N) {
                        // push these onto _ilsCoordindex
                        // (N+1)*(N+1)*(i  )+(N+1)*(j  )+(k  )
                        // (N+1)*(N+1)*(i+1)+(N+1)*(j  )+(k  )
                        // -1
                        _ilsCoordIndex.push_back((N+1)*(N+1)*(i  )+(N+1)*(j  )+(k  ));
                        _ilsCoordIndex.push_back((N+1)*(N+1)*(i+1)+(N+1)*(j  )+(k  ));
                        _ilsCoordIndex.push_back(-1);
                    }
                    
                    // (i,j,k)->(i,j+1,k)
                    if(j<N) {
                        // push these onto _ilsCoordindex
                        // (N+1)*(N+1)*(i  )+(N+1)*(j  )+(k  )
                        // (N+1)*(N+1)*(i  )+(N+1)*(j+1)+(k  )
                        // -1
                        _ilsCoordIndex.push_back((N+1)*(N+1)*(i  )+(N+1)*(j  )+(k  ));
                        _ilsCoordIndex.push_back((N+1)*(N+1)*(i  )+(N+1)*(j+1)+(k  ));
                        _ilsCoordIndex.push_back(-1);
                    }
                    
                    // (i,j,k)->(i,j,k+1)
                    if(k<N) {
                        // push these onto _ilsCoordindex
                        // (N+1)*(N+1)*(i  )+(N+1)*(j  )+(k  )
                        // (N+1)*(N+1)*(i  )+(N+1)*(j  )+(k+1)
                        // -1
                        _ilsCoordIndex.push_back((N+1)*(N+1)*(i  )+(N+1)*(j  )+(k  ));
                        _ilsCoordIndex.push_back((N+1)*(N+1)*(i  )+(N+1)*(j  )+(k+1));
                        _ilsCoordIndex.push_back(-1);
                    }
                    
                }
            }
        }
    
    }
}


void SceneGraphProcessor::bboxRemove() {
  // 1) look for a child of the SceneGraph named "BOUNDING-BOX"
  // 2) if found, remove from the children array and delete
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    
    while((node=traversal.next())!=(Node*)0) {
        if ((node->getName())=="BOUNDING-BOX") {
            _wrl.removeChild(node);
            delete node;
        }
    }
}

void SceneGraphProcessor::edgesAdd() { // TODO
  
  // 1) using a SceneGraphTraversal, look for Shape nodes
    
  // 2) for each Shape node containing an IndexedFaceSet as geometry

  // 3) look for another Shape node, sibiling of the previous Shape
  // node (i.e. child of the same Group node), named "EDGES"; if note
  // found create a new one, and insert it as a new child of the Group
  // node which is parent of the Shape node; if the "EDGES" Shape node
  // does not contain an IndexedLineSet as geometry, delete the
  // geometry node, and set to (Node*)0; if the geometry node is equal
  // to (Node*)0, create a new IndexedLineSet and save it in the
  // geometry field of the first Shape node

  // 4) make sure that the appearance field of the EDGES Shape node
  // contains and Appearance node, and the material field of the
  // Appearance node contains a Material node; if node create them;
  // set the diffuseColor field of the Material node to an appropriate
  // value

  // 5) clear the IndexedLineSet node

  // 6) copy the coord field from the IndexedFaceSet node to the
  // IndexedLineSet node

  // 7) for each face of the IndexedFaceSet node, determine the
  // corresponding boundary edges and store them in the coordIndex
  // array of the IndexedLineSet

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    bool edgesCheck=false;
    IndexedLineSet* edgesIls;
    
    vector<float> ifsCoordHolder;
    vector<int> ifsCoordIndexHolder;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
                if (node->nameEquals("EDGES")) {edgesCheck=true;}
                Shape* shape= (Shape*) node;
                node=shape->getGeometry();
            
                //for each shape node that has ifs geom....
                if (node!=(Node*)0 && node->isIndexedFaceSet()) {
                    IndexedFaceSet* holder=(IndexedFaceSet*) node;
                    ifsCoordHolder=holder->getCoord();
                    ifsCoordIndexHolder=holder->getCoordIndex();

                    /*SceneGraphTraversal traversal2(_wrl);
                    traversal2.start();
                    Node* node1;
                    
                    while((node1=traversal2.next())!=(Node*)0) {
                        if (node1->isShape()) {
                            if (node1->nameEquals("EDGES")) {
                                edgesCheck=true;
                                
                                //test to make sure EDGES is right...
                                Shape* shape1= (Shape*) node1;
                                node1=shape1->getGeometry();
                                
                                if (!(node1->isIndexedLineSet())) {
                                    delete node1;
                                    Shape* shape1= (Shape*) node1;
                                    shape1->setGeometry((Node*)0);
                                }
                                
                                if (node1==(Node*)0) {
                                    edgesIls= new IndexedLineSet();
                                    shape->setGeometry(edgesIls); //first shape node
                                }
                                
                                Node* apprHolder;
                                Node* appr=shape->getAppearance();
                                apprHolder=(Appearance*) appr;
                                
                                //no appearance, create
                                if (appr==(Node*)0) {
                                    Appearance* appr= new Appearance();
                                    Material* matr= new Material();
                                    Color diffCol=matr->getDiffuseColor();
                                    diffCol=*(new Color());
                                    appr->setMaterial(matr);
                                }
   
                            }
                        }
                        
                    }*/
                    
                }
 
        }
        
    }
    
    

    if (!edgesCheck) {
        Shape* edgesShape= new Shape();
        edgesIls=new IndexedLineSet();
        edgesShape->setGeometry(edgesIls);
        
        edgesShape->setName("EDGES");
        
        Appearance* appr= new Appearance();
        Material* matr= new Material();
        Color diffCol=matr->getDiffuseColor();
        diffCol=*(new Color());
        
        appr->setMaterial(matr);
        edgesShape->setAppearance(appr);
        
        _wrl.addChild(edgesShape);
    }
    
    if (edgesCheck) {return;}

    //for each face of the IndexedFaceSet node, determine the corresponding boundary edges and store them in the coordINdex array of the Indexexedlineset edgesIls
    edgesIls->getCoord()=ifsCoordHolder;
    vector<int>& tempIlsCoordIndex= edgesIls->getCoordIndex();
    vector<int>& tempIfsHolderCoordIndex=ifsCoordIndexHolder;
    
    for (int n=0; n<tempIfsHolderCoordIndex.size(); n++) { //new face
        if (tempIfsHolderCoordIndex.at(n)==-1) {tempIlsCoordIndex.push_back(-1);} //new face
        
        if (n<tempIfsHolderCoordIndex.size()-2) {
            tempIlsCoordIndex.push_back(tempIfsHolderCoordIndex.at(n+1));
            tempIlsCoordIndex.push_back(tempIfsHolderCoordIndex.at(n+2));
            tempIlsCoordIndex.push_back(-1);
        }
    }
    
    
    
}

void SceneGraphProcessor::edgesRemove() {
  // TODO,using a SceneGraphTraversal, look for all the shape nodes
  // named "EDGES", remove them from the scene graph and delete them
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    Node* temp;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            if (node->nameEquals("EDGES")) {
                _wrl.removeChild(node);
                delete node;
            }
        }
    }
    
    
}

void SceneGraphProcessor::shapeIndexedFaceSetShow() {
  // TODO
  // using a SceneGraphTraversal look for all Shape nodes containing
  // an IndexedFaceSet as geometry, and set the show variable to true

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    Node* temp;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            Shape* shape = (Shape*)node;
            temp = shape->getGeometry();
            if(temp!=(Node*)0 && temp->isIndexedFaceSet()) {
                node->setShow(true);
            }
        }
    }
    
}

void SceneGraphProcessor::shapeIndexedFaceSetHide() {
  // TODO
  // using a SceneGraphTraversal look for all Shape nodes containing
  // an IndexedFaceSet as geometry, and set the show variable to false
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    Node* temp;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            Shape* shape = (Shape*)node;
            temp = shape->getGeometry();
            if(temp!=(Node*)0 && temp->isIndexedFaceSet()) {
                auto ifs= (IndexedFaceSet*) node;

                node->setShow(false);
            }
        }
    }
    

}

void SceneGraphProcessor::shapeIndexedLineSetShow() {
  // TODO, using a SceneGraphTraversal look for all Shape nodes containing
  // an IndexedLineSet as geometry, and set the show variable to true
    
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    Node* temp;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            Shape* shape = (Shape*)node;
            temp = shape->getGeometry();
            if(temp!=(Node*)0 && temp->isIndexedLineSet()) {
                node->setShow(true);
            }
        }
    }
    

}

void SceneGraphProcessor::shapeIndexedLineSetHide() {
  // TODO, using a SceneGraphTraversal look for all Shape nodes containing
  // an IndexedLineSet as geometry, and set the show variable to true

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    Node* temp;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            Shape* shape = (Shape*)node;
            temp = shape->getGeometry();
            if(temp!=(Node*)0 && temp->isIndexedLineSet()) {
                node->setShow(false);
            }
        }
    }
    
}

bool SceneGraphProcessor::hasBBox() {
  //return _wrl.getChild("BOUNDING-BOX")!=(Node*)0;
    return _wrl.find("BOUNDING-BOX")!=(Node*)0; //mine
}

// a Property is define in Node.h as a function with a single Node&
// argument which returns a boolean value

bool SceneGraphProcessor::_hasShapeProperty(Shape::Property p) {
  // TODO,using a SceneGraphTraversal look for Shape nodes; return true if
  // the Property p is true for at least one Shape node, and false
  // otherwise
    bool value = false;
    SceneGraphTraversal t= SceneGraphTraversal(_wrl);
    Node* child;
    t.start();
    
    while ((child=t.next())!=NULL) {
        if (child->isShape()) {
            Shape* shape= (Shape*) child;
            
            
            //hasEdges property
            if ((bool (*) (Shape&))p==_hasEdges) {
                if (_hasEdges(*shape)) {value=true;}
            }
            
            //hasIndexedFaceSetShown property
            if ((bool (*) (Shape&))p==_hasIndexedFaceSetShown) {
                if (_hasIndexedFaceSetShown(*shape)) {value=true;}
            }
            
            //_hasIndexedFaceSetHidden property
            if ((bool (*) (Shape&))p==_hasIndexedFaceSetHidden) {
                if (_hasIndexedFaceSetHidden(*shape)) {value=true;}
            }
            
            //_hasIndexedLineSetShown property
            if ((bool (*) (Shape&))p==_hasIndexedLineSetShown) {
                if (_hasIndexedLineSetShown(*shape)) {value=true;}
            }
            
            //_hasIndexedLineSetHidden property
            if ((bool (*) (Shape&))p==_hasIndexedLineSetHidden) {
                if (_hasIndexedLineSetHidden(*shape)) {value=true;}
            }
            
        }
    }
    
  return value;
}

bool SceneGraphProcessor::_hasIndexedFaceSetProperty(IndexedFaceSet::Property p) {
  bool value = false;
  // TODO
  // using a SceneGraphTraversal look for Shape nodes containing and
  // IndexedFaceSet nodes as geometry; return true if the Property p
  // is true for at least one of these IndexedFaceSets node, and false
  // otherwise
    
    SceneGraphTraversal t= SceneGraphTraversal(_wrl);
    Node* child;
    t.start();
    
    while ((child=t.next())!=NULL) {
        if (child->isShape()) {
            Shape* shape= (Shape*) child;
            child=shape->getGeometry();
            
            if (child==(Node*)0) {continue;} //no geometry
            
            if (child->isIndexedFaceSet()) {
                //hasnormalNone property
                if ((bool (*) (IndexedFaceSet&))p==_hasNormalNone) {
                    if (_hasNormalNone(*((IndexedFaceSet*) child))) {value=true;}
                }
                
                //hasnormalperface property
                if ((bool (*) (IndexedFaceSet&))p==_hasNormalPerFace) {
                    if (_hasNormalPerFace(*((IndexedFaceSet*) child))) {value=true;}
                }
                
                //hasnormalpervertexProperty
                if ((bool (*) (IndexedFaceSet&))p==_hasNormalPerVertex) {
                    if (_hasNormalPerVertex(*((IndexedFaceSet*) child))) {value=true;}
                }
                
                //hasnormalperCorner Property
                if ((bool (*) (IndexedFaceSet&))p==_hasNormalPerCorner) {
                    if (_hasNormalPerCorner(*((IndexedFaceSet*) child))) {value=true;}
                }
            
                //hasFaces property
                if ((bool (*) (IndexedFaceSet&))p==_hasFaces) {
                    if (_hasFaces(*((IndexedFaceSet*) child))) {value=true;}
                }
                
            }
        }
    }

  return value;
}

bool SceneGraphProcessor::_hasIndexedLineSetProperty(IndexedLineSet::Property p) {
  bool value = false;

    SceneGraphTraversal t= SceneGraphTraversal(_wrl);
    Node* child;
    t.start();
    
    while ((child=t.next())!=NULL) {
        if (child->isShape()) {
            Shape* shape= (Shape*) child;
            child=shape->getGeometry();
            if (child->isIndexedFaceSet()) {
                
                //hasColorNone property
                if ((bool (*) (IndexedLineSet&))p==_hasColorNone) {
                    if (_hasColorNone(*((IndexedLineSet*)child))) {value=true;}
                }
                
                //hasColorPerVertex property
                if ((bool (*) (IndexedLineSet&))p==_hasColorPerVertex) {
                    if (_hasColorPerVertex(*((IndexedLineSet*)child))) {value=true;}
                }
                
                //hasColorPerPolyline property
                if ((bool (*) (IndexedLineSet&))p==_hasColorPerPolyline) {
                    if (_hasColorPerPolyline(*((IndexedLineSet*)child))) {value=true;}
                }
                
            }
        }
    }
  return value;
}

// this is a Property
bool SceneGraphProcessor::_hasFaces(IndexedFaceSet& ifs) {
    bool value = false;
    if (ifs.getCoordIndex().size()!=0) {value=true;}
    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasNormalNone(IndexedFaceSet& ifs) {
    bool value = false;
    vector<float> & normal = ifs.getNormal();
    if (normal.size()==0) {value=true;}
    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasNormalPerFace(IndexedFaceSet& ifs) {
    bool value = false;
    IndexedFaceSet::Binding binding= ifs.getNormalBinding();
    if (binding==IndexedFaceSet::PB_PER_FACE) {value=true;}
    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasNormalPerVertex(IndexedFaceSet& ifs) {
    bool value = false;
    if (ifs.getNormalPerVertex()) {value=true;}
    return value;
}

// this is a IndexedFaceSet::Property
bool SceneGraphProcessor::_hasNormalPerCorner(IndexedFaceSet& ifs) {
    bool value = false;
    IndexedFaceSet::Binding binding= ifs.getNormalBinding();
    if (binding==IndexedFaceSet::PB_PER_CORNER) {value=true;}
    return value;
}

bool SceneGraphProcessor::hasIndexedFaceSetFaces() {
  return _hasIndexedFaceSetProperty((bool (*) (Node &))_hasFaces);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalNone() {
  return _hasIndexedFaceSetProperty((bool (*) (Node &))_hasNormalNone);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerFace() {
  return _hasIndexedFaceSetProperty((bool (*) (Node &))_hasNormalPerFace);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerVertex() {
  return _hasIndexedFaceSetProperty((bool (*) (Node &))_hasNormalPerVertex);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerCorner() {
  return _hasIndexedFaceSetProperty((bool (*) (Node &))_hasNormalPerCorner);
}

// VRML'97
//
// If the color field is not NULL, it shall contain a Color node, and
// the colours are applied to the line(s) as follows:
//
// If colorPerVertex is FALSE:
//
// If the colorIndex field is not empty, then one colour is used for
// each polyline of the IndexedLineSet. There must be at least as many
// indices in the colorIndex field as there are polylines in the
// IndexedLineSet. If the greatest index in the colorIndex field is N,
// then there must be N+1 colours in the Color node. The colorIndex
// field must not contain any negative entries.
//
// If the colorIndex field is empty, then the colours from the Color
// node are applied to each polyline of the IndexedLineSet in
// order. There must be at least as many colours in the Color node as
// there are polylines.
//
// If colorPerVertex is TRUE:
//
// If the colorIndex field is not empty, then colours are applied to
// each vertex of the IndexedLineSet in exactly the same manner that
// the coordIndex field is used to supply coordinates for each vertex
// from the Coordinate node. The colorIndex field must contain at
// least as many indices as the coordIndex field and must contain
// end-of-polyline markers (-1) in exactly the same places as the
// coordIndex field. If the greatest index in the colorIndex field is
// N, then there must be N+1 colours in the Color node.
//
// If the colorIndex field is empty, then the coordIndex field is used
// to choose colours from the Color node. If the greatest index in the
// coordIndex field is N, then there must be N+1 colours in the Color
// node.
//
// If the color field is NULL and there is a Material defined for the
// Appearance affecting this IndexedLineSet, the emissiveColor of the
// Material shall be used to draw the lines.

// this is a Property
 bool SceneGraphProcessor::_hasColorNone(IndexedLineSet& ils) {
   bool value = false;
     if ((ils.getColor()).size()==0) {value=true;}
   return value;
}

// this is a Property
bool SceneGraphProcessor::_hasColorPerVertex(IndexedLineSet& ils) {
  bool value = false;
    
    value=ils.getColorPerVertex();
    
  return value;
}

// this is a Property
bool SceneGraphProcessor::_hasColorPerPolyline(IndexedLineSet& ils) {
    bool value = false;
    vector<float>& ilsColorRef= ils.getColor();
    int numPolylines=ils.getNumberOfPolylines();
  
    if (ilsColorRef.size()==numPolylines) {value=true;}
  
    return value;
}

bool SceneGraphProcessor::hasIndexedLineSetColorNone() {
  return _hasIndexedLineSetProperty(_hasColorNone);
}

bool SceneGraphProcessor::hasIndexedLineSetColorPerVertex() {
  return _hasIndexedLineSetProperty(_hasColorPerVertex);
}

bool SceneGraphProcessor::hasIndexedLineSetColorPerPolyline() {
  return _hasIndexedLineSetProperty(_hasColorPerPolyline);
}

// this is a Property
bool SceneGraphProcessor::_hasEdges(Shape& shape) {
  bool value = false;
    if (shape.nameEquals("EDGES")) {value=true;}
    
  return value;
}

// this is a Property
bool SceneGraphProcessor::_hasIndexedFaceSetShown(Shape& shape) {
    Node* node=shape.getGeometry();
    
    if (node!=(Node*)0 && node->isIndexedFaceSet()) {
        if (shape.getShow()) {return true;}
        else {return false;}
    }
    
}

// this is a Property
bool SceneGraphProcessor::_hasIndexedFaceSetHidden(Shape& shape) {
    Node* node=shape.getGeometry();
    if (node!=(Node*)0 && node->isIndexedFaceSet()) {
        if (!shape.getShow()) {return true;}
        else {return false;}
    }
}

// this is a Property
bool SceneGraphProcessor::_hasIndexedLineSetShown(Shape& shape) {
    Node* node=shape.getGeometry();
    if (node!=(Node*)0 && node->isIndexedLineSet()) {
        if (shape.getShow()) {return true;}
        else {return false;}
    }
}

// this is a Property
bool SceneGraphProcessor::_hasIndexedLineSetHidden(Shape& shape) {
    Node* node=shape.getGeometry();
    if (node!=(Node*)0 && node->isIndexedLineSet()) {
        if (!shape.getShow()) {return true;}
        else {return false;}
    }

}

bool SceneGraphProcessor::hasEdges() {
    return _hasShapeProperty((bool (*)(Node &)) _hasEdges);
}

bool SceneGraphProcessor::hasIndexedFaceSetShown() {
  return _hasShapeProperty((bool (*) (Node &)) _hasIndexedFaceSetShown);
    //return _hasShapeProperty(_hasIndexedFaceSetShown);
}

bool SceneGraphProcessor::hasIndexedFaceSetHidden() {
  return _hasShapeProperty((bool (*) (Node &))_hasIndexedFaceSetHidden);
}

bool SceneGraphProcessor::hasIndexedLineSetShown() {
  return _hasShapeProperty((bool (*) (Node &))_hasIndexedLineSetShown);
}

bool SceneGraphProcessor::hasIndexedLineSetHidden() {
  return _hasShapeProperty((bool (*) (Node &))_hasIndexedLineSetHidden);
}

void SceneGraphProcessor::removeSceneGraphChild(const string& name) {
    Node* child = _wrl.find(name);
    if (child!=(Node*)0) {_wrl.removeChild(child);}
}

void SceneGraphProcessor::pointsRemove() {
  removeSceneGraphChild("POINTS");
}

void SceneGraphProcessor::surfaceRemove() {
  removeSceneGraphChild("SURFACE");
}

IndexedFaceSet* SceneGraphProcessor::_getNamedShapeIFS
(const string& name, bool create) {

  IndexedFaceSet* ifs = (IndexedFaceSet*)0;

  // TODO

  // 1) look for a child of the SceneGraph node with the given name

  // 2) if found, and the geometry field contains an IndexedFaceSet
  // node, return the pointer to the IndexedFaceSet node.

  // 3) otherwise, if create==true, create a new Shape node, make it a
  // child of the SceneGraph, add an Appearance node as appearance,
  // add an IndexedFaceSet as geometry, and return the pointer to the
  // IndexedFaceSet node.

  // 4) make sure tha the Appearance node has a Material node in the
  // material field with a properly defined diffuseColor field, before
  // you return

    SceneGraphTraversal t= SceneGraphTraversal(_wrl);
    Node* child;
    t.start();
    
    while ((child=t.next())!=NULL) {
        if (child->isShape()) {
            Shape* shape= (Shape*) child;
            child=shape->getGeometry();
            if (child->isIndexedFaceSet()) {
                return (IndexedFaceSet*)child;
            }
            else {
                if (create) {
                    Shape* newNode=new Shape();
                    _wrl.addChild(newNode);
                    
                    Appearance* _appearance= new Appearance();
                    IndexedFaceSet* _geom= new IndexedFaceSet();
                    
                    newNode->setAppearance(_appearance);
                    newNode->setGeometry(_geom);
                }
            }

        }
    }
    
  return ifs;
}

//////////////////////////////////////////////////////////////////////
void SceneGraphProcessor::eigenFit(const vector<float>& coordPoints,
              const Vec3f& min, const Vec3f& max, Vec4f& f) {

  char str[256];
  cerr << "eigenFit() {" <<endl;

  int nPoints = (int)((coordPoints.size())/3);
  float x0 = min.x, x1 = max.x, dx = x1-x0;
  float y0 = min.y, y1 = max.y, dy = y1-y0;
  float z0 = min.z, z1 = max.z, dz = z1-z0;
  float dMax = dx; if(dy>dMax) dMax = dy; if(dz>dMax) dMax = dz;

  // compute the mean of the points contained in the 
  double x,y,z;
  double xMean = 0.0f;
  double yMean = 0.0f;
  double zMean = 0.0f;
  int   nMean = 0;
  for(int iP=0;iP<nPoints;iP++) {
    x = (double)(coordPoints[3*iP  ]);
    y = (double)(coordPoints[3*iP+1]);
    z = (double)(coordPoints[3*iP+2]);
    if(x0<=x && x<=x1 && y0<=y && y<=y1 && z0<=z && z<=z1) {
      xMean += x;
      yMean += y;
      zMean += z;
      nMean++;
    }
  }

  cerr << "  nMean = " << nMean << endl;
  if(nMean==0) {
    // throw exception ??
    return;
  }

  xMean /= ((double)nMean);
  yMean /= ((double)nMean);
  zMean /= ((double)nMean);

  cerr << "  pMean = [" <<endl;
  sprintf(str,"    %12.6f",xMean); cerr << str << endl;
  sprintf(str,"    %12.6f",yMean); cerr << str << endl;
  sprintf(str,"    %12.6f",zMean); cerr << str << endl;
  cerr << "  ]" <<endl;

  // compute the scatter matrix
  double dxp,dyp,dzp;
  double M00=0.0,M01=0.0,M02=0.0,M11=0.0,M12=0.0,M22=0.0;
  for(int iP=0;iP<nPoints;iP++) {
    x = (double)(coordPoints[3*iP  ]);
    y = (double)(coordPoints[3*iP+1]);
    z = (double)(coordPoints[3*iP+2]);
    if(x0<=x && x<=x1 && y0<=y && y<=y1 && z0<=z && z<=z1) {
      dxp = (x-xMean)/dMax;
      dyp = (y-yMean)/dMax;
      dzp = (z-zMean)/dMax;
      M00 += dxp*dxp; M01 += dxp*dyp; M02 += dxp*dzp;
                      M11 += dyp*dyp; M12 += dyp*dzp;
                                      M22 += dzp*dzp;
    }
  }

  double dMean = (double)nMean;
  M00 /= dMean; M01 /= dMean; M02 /= dMean;
                M11 /= dMean; M12 /= dMean;
                              M22 /= dMean;
  Matrix3d M;
  M << M00,M01,M02,M01,M11,M12,M02,M12,M22;

  // double dMean = (double)nMean;
  // M(0,0) /= dMean; M(0,1) /= dMean; M(0,2) /= dMean;
  // M(1,0) /= dMean; M(1,1) /= dMean; M(1,2) /= dMean;
  // M(2,0) /= dMean; M(2,1) /= dMean; M(2,2) /= dMean;

  cerr << "  M = [" << endl;
  sprintf(str,"    %12.6f %12.6f %12.6f",M(0,0),M(0,1),M(0,2)); cerr << str << endl;
  sprintf(str,"    %12.6f %12.6f %12.6f",M(1,0),M(1,1),M(1,2)); cerr << str << endl;
  sprintf(str,"    %12.6f %12.6f %12.6f",M(2,0),M(2,1),M(2,2)); cerr << str << endl;
  cerr << "  ]" << endl;

  SelfAdjointEigenSolver<Matrix3d> eigensolver(M);

  if(eigensolver.info() != Success) {
    // trow exception ?
    // abort();
    return;
  }

  Vector3d L(eigensolver.eigenvalues());
  Matrix3d E(eigensolver.eigenvectors());

  cerr << "  eigenvalues(M)"<< endl;
  cerr << "  L = [" << endl;
  sprintf(str,"    %12.6f",L(0)); cerr << str << endl;
  sprintf(str,"    %12.6f",L(1)); cerr << str << endl;
  sprintf(str,"    %12.6f",L(2)); cerr << str << endl;
  cerr << "  ]" << endl;

  cerr << "  eigenvectors(M)"<< endl;
  cerr << "  E = [" << endl;
  sprintf(str,"    %12.6f %12.6f %12.6f",E(0,0),E(0,1),E(0,2)); cerr << str << endl;
  sprintf(str,"    %12.6f %12.6f %12.6f",E(1,0),E(1,1),E(1,2)); cerr << str << endl;
  sprintf(str,"    %12.6f %12.6f %12.6f",E(2,0),E(2,1),E(2,2)); cerr << str << endl;
  cerr << "  ]" << endl;

  // L(0)                   minimum eigenvalue
  // (E(0,0),E(1,0),E(2,0)) minimum eigenvector

  f.x =  (float)(E(0,0));
  f.y =  (float)(E(1,0));
  f.z =  (float)(E(2,0));
  f.w = -(float)(E(0,0)*xMean+E(1,0)*yMean+E(2,0)*zMean);

  cerr << "  f = [" << endl;
  sprintf(str,"    %12.6f",f.x);  cerr << str << endl;
  sprintf(str,"    %12.6f",f.y);  cerr << str << endl;
  sprintf(str,"    %12.6f",f.z);  cerr << str << endl;
  sprintf(str,"    %12.6f",f.w);  cerr << str << endl;
  cerr << "  ]" << endl;

  cerr << "}" <<endl;
}

//////////////////////////////////////////////////////////////////////
void SceneGraphProcessor::meanFit(const vector<float>& coordPoints,
             const vector<float>& normalPoints,
             const Vec3f& min, const Vec3f& max, Vec4f& f) {

  int nPoints = (int)((coordPoints.size())/3);
  float x0 = min.x, x1 = max.x, dx = x1-x0;
  float y0 = min.y, y1 = max.y, dy = y1-y0;
  float z0 = min.z, z1 = max.z, dz = z1-z0;
  float dMax = dx; if(dy>dMax) dMax = dy; if(dz>dMax) dMax = dz;

  // compute the mean of the points contained in the 
  double x,y,z,nn;
  double xMean = 0.0f;
  double yMean = 0.0f;
  double zMean = 0.0f;
  double nxMean = 0.0f;
  double nyMean = 0.0f;
  double nzMean = 0.0f;
  int    nMean = 0;
  for(int iP=0;iP<nPoints;iP++) {
    x = (double)(coordPoints[3*iP  ]);
    y = (double)(coordPoints[3*iP+1]);
    z = (double)(coordPoints[3*iP+2]);
    if(x0<=x && x<=x1 && y0<=y && y<=y1 && z0<=z && z<=z1) {
      xMean += x;
      yMean += y;
      zMean += z;
      x = (double)(normalPoints[3*iP  ]);
      y = (double)(normalPoints[3*iP+1]);
      z = (double)(normalPoints[3*iP+2]);
      nxMean += x;
      nyMean += y;
      nzMean += z;
      nMean++;
    }
  }

  if(nMean==0) {
    // throw exception ??
    return;
  }

  // normalize the point mean
  xMean /= ((double)nMean);
  yMean /= ((double)nMean);
  zMean /= ((double)nMean);
  // normalize the normal mean to unit length
  nn = nxMean*nxMean+nyMean*nyMean+nzMean*nzMean;
  if(nn>0.0) {
    nn = sqrt(nn); nxMean/=nn; nyMean/=nn; nzMean/=nn;
  }
  // set the linear function coefficients
  f.x =  (float)(nxMean);
  f.y =  (float)(nyMean);
  f.z =  (float)(nzMean);
  f.w = -(float)(nxMean*xMean+nyMean*yMean+nzMean*zMean);
}

//////////////////////////////////////////////////////////////////////
void SceneGraphProcessor::fitSinglePlane
(const Vec3f& center, const Vec3f& size,
 const float scale, const bool cube, Vec4f& f) {

  cerr << "SceneGraphProcessor::fitSinglePlane() {" << endl;

  // 1) make sure that the bounding box is not empty; if empty throw a
  // StrExeption with a proper message
    if (size.x==0 && size.y==0 && size.z==0) {throw string("bounding box is empty");}
 
  // 2) find or create the input POINTS set in the scene graph; input
  // points must have coordinates and normal vector; otherwise throw
  // an StrException
    
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    
    IndexedFaceSet* points;
    Node* node;
    bool pointsCheck=false;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            Shape* shape = (Shape*) node;
            if (node->getName()=="POINTS") {
                pointsCheck=true;
                node=shape->getGeometry();
                
                if (node->isIndexedFaceSet()) {
                    points=(IndexedFaceSet*) node;
                    if ((points->getCoord()).size()==0 ) {throw string("coord array size 0");}
                    if ((points->getNormal()).size()==0) {throw string("normal array size 0");}
                }
            }
        }
    }
    
    if (!pointsCheck) {return;}


  // get from the POINTS IndexedFaceSet
    vector<float>& coordPoints= points->getCoord();
    vector<float>& normalPoints=points->getNormal();

  // 3) compute the coordinates of the SceneGraph bounding box corners
    //get bbox data
    Vec3f _bboxCenter =_wrl.getBBoxCenter();
    Vec3f _bboxSize = _wrl.getBBoxSize();
    
    float xSize=_bboxSize.x; float ySize=_bboxSize.y; float zSize=_bboxSize.z;
    
    //get all 8 corners
    std::vector<Vec3f> corners(8);
    //left four corners
    corners.at(0).x=_bboxCenter.x-xSize/2.0;
    corners.at(0).y=_bboxCenter.y-ySize/2.0;
    corners.at(0).z=_bboxCenter.z-zSize/2.0;
    
    corners.at(1).x=_bboxCenter.x-xSize/2.0;
    corners.at(1).y=_bboxCenter.y-ySize/2.0;
    corners.at(1).z=_bboxCenter.z+zSize/2.0;
    
    corners.at(2).x=_bboxCenter.x-xSize/2.0;
    corners.at(2).y=_bboxCenter.y+ySize/2.0;
    corners.at(2).z=_bboxCenter.z-zSize/2.0;
    
    corners.at(3).x=_bboxCenter.x-xSize/2.0;
    corners.at(3).y=_bboxCenter.y+ySize/2.0;
    corners.at(3).z=_bboxCenter.z+zSize/2.0;
    
    //right four corners
    corners.at(4).x=_bboxCenter.x+xSize/2.0;
    corners.at(4).y=_bboxCenter.y-ySize/2.0;
    corners.at(4).z=_bboxCenter.z-zSize/2.0;
    
    corners.at(5).x=_bboxCenter.x+xSize/2.0;
    corners.at(5).y=_bboxCenter.y-ySize/2.0;
    corners.at(5).z=_bboxCenter.z+zSize/2.0;
    
    corners.at(6).x=_bboxCenter.x+xSize/2.0;
    corners.at(6).y=_bboxCenter.y+ySize/2.0;
    corners.at(6).z=_bboxCenter.z-zSize/2.0;
    
    corners.at(7).x=_bboxCenter.x+xSize/2.0;
    corners.at(7).y=_bboxCenter.y+ySize/2.0;
    corners.at(7).z=_bboxCenter.z+zSize/2.0;
    
    corners.at(0).x=_bboxCenter.x-xSize/2.0;
    corners.at(0).y=_bboxCenter.y-ySize/2.0;
    corners.at(0).z=_bboxCenter.z-zSize/2.0;
    
    corners.at(7).x=_bboxCenter.x+xSize/2.0;
    corners.at(7).y=_bboxCenter.y+ySize/2.0;
    corners.at(7).z=_bboxCenter.z+zSize/2.0;
    
    Vec3f min=corners.at(0);
    Vec3f max=corners.at(7);
    
  // 4) fit a plane to the points contained in the bounding box
    //eigenFit(coordPoints,min,max,f);
    meanFit(coordPoints,normalPoints,min,max,f);
    // what to do with Vec4f& f?
    
  // 5) find or create the output SURFACE in the scene graph ("PLANE")
    SceneGraphTraversal traversal1(_wrl);
    traversal1.start();
    
    IndexedFaceSet* plane= (IndexedFaceSet*) 0;
    Node* nodePlane;
    bool planeCheck=false;
    
    //MAYBE DONT SET NAME "PLANE"
    while((nodePlane=traversal1.next())!=(Node*)0) {
        if(nodePlane->isShape()) {
            Shape* shape1 = (Shape*) nodePlane;
            if (nodePlane->getName()=="PLANE") {
                planeCheck=true;
                nodePlane=shape1->getGeometry();
                
                if (nodePlane->isIndexedFaceSet()) {
                    plane=(IndexedFaceSet*) nodePlane;
                    if ((plane->getCoord()).size()==0 ) {throw string("coord array size 0");}
                    if ((plane->getNormal()).size()==0) {throw string("normal array size 0");}
                }}}}
    
    if (!planeCheck) {
        //CREATE input SURFACE set??
        plane=new IndexedFaceSet();
        Shape* surface= new Shape(); surface->setGeometry(plane); surface->setName("PLANE");
        Appearance* appr= new Appearance(); Material* matr= new Material(); Color diffCol=matr->getDiffuseColor(); diffCol=*(new Color()); appr->setMaterial(matr); surface->setAppearance(appr);
        _wrl.addChild(surface);
    }
    
    
    //get from the SURFACE IndexedFaceSet
    vector<float>& coordIfs=plane->getCoord();
    coordIfs.clear();

    
//MY IDEA FOR DRAWING A SINGLE PLANE
    //vector<int>& coordIndexIfs=plane->getCoordIndex();
    //coordIndexIfs.clear();
    //draw the plane
    //linear algebra first
    //x y z coords of normal, w is distance from origin to normal (Hessian Plane Format/Coordinates)
    
    //coordIndexIfs.push_back(f.x);
    
    
    
  // 6) evaluate the linear function at bounding box corners
  float F[8]; // function values at bbox corners
  bool  b[8]; // function is positive or negative ?
  for(int i=0;i<8;i++) {
      float x= (corners.at(i)).x;
      float y= (corners.at(i)).y;
      float z= (corners.at(i)).z;

    // set (x,y,z) the the i=th corner coordinates
    // computed in step 3) above
    b[i] = (F[i] = x*f.x+y*f.y+z*f.z+f.w)<0.0f;
  }

  cerr << "//                6 ----- 7 = (x1,y1,z1)" << endl;
  cerr << "//               /|      /|             " << endl;
  cerr << "//              4 ----- 5 |             " << endl;
  cerr << "//              | |     | |             " << endl;
  cerr << "//              | 2 ----| 3             " << endl;
  cerr << "//              |/      |/              " << endl;
  cerr << "// (x0,y0,z0) = 0 ----- 1 x               " << endl;

  cerr << "  b = [" << endl;
  cerr << "    " << b[0] << endl;
  cerr << "    " << b[1] << endl;
  cerr << "    " << b[2] << endl;
  cerr << "    " << b[3] << endl;
  cerr << "    " << b[4] << endl;
  cerr << "    " << b[5] << endl;
  cerr << "    " << b[6] << endl;
  cerr << "    " << b[7] << endl;
  cerr << "  ]" << endl;

  //////////////////////////////////////////////////////////////////////
  //
  //    vertices      //    edges                 //    faces
  //      6-----7     //        [6]---11---[7]    //        1
  //     /|    /|     //        /|         /|     //        | 3
  //    4-----5 |     //       6 2        7 3     //        |/
  //    | 2---|-3     //      /  |       /  |     //    4---+---5
  //    |/    |/      //    [4]---10---[5]  |     //       /|
  //    0-----1       //     |   |      |   |     //      2 |
  //                  //     |  [2]--9--|--[3]    //        0
  //                  //     0  /       1  /      //
  //                  //     | 4        | 5       //
  //                  //     |/         |/        //
  //                  //    [0]---8----[1]        //
  //

  const int (*edge)[2] = IsoSurf::getEdgeTable();

  // 7) compute the isovertex coordinates;
  // can you explain what is going on here?
  float tj,tk;
  int   iE[12],iV,i,j,k;
  for(i=0;i<12;i++) {
    iV   = -1;
    j    = edge[i][0];
    k    = edge[i][1];
    if(b[j]!=b[k]) {
      // isvertex index
        //iV = (int)((coordIfs.size()/3));
      // isovertex coordinates
      tk = F[j]/(F[j]-F[k]);
      tj = F[k]/(F[k]-F[j]);
      int x  = tj*corners[j].x+tk*corners[k].x;
      int y  = tj*corners[j].y+tk*corners[k].y;
      int z  = tj*corners[j].z+tk*corners[k].z;
      coordIfs.push_back(x);
      coordIfs.push_back(y);
      coordIfs.push_back(z);
    }
    iE[i] = iV;
  }
  cerr << "  edge to isovertex table" << endl;
  cerr << "  iE = [" << endl;
    char* str;
  for(i=0;i<12;i++) {
    //sprintf(str,"    %2d (%2d, %2d) -> %3d",i, edge[i][0],edge[i][1],iE[i]);
    //cerr << str << endl;
  }
  cerr << "  ]" << endl;

  // 8) create isosurface faces
  vector<int>& coordIndex = plane->getCoordIndex(); // coordIndex.size()==0
  int nFaces = IsoSurf::makeCellFaces(b,iE,coordIndex); //fills coordindex
  cerr << "  nFaces = " << nFaces << endl;

  // 9) save face normal
  plane->setNormalPerVertex(false);
  vector<float>& normal = plane->getNormal();
  // normal.size()==0 here
  normal.push_back(f.x);
  normal.push_back(f.y);
  normal.push_back(f.z);

  cerr << "}" << endl;
 
}

//////////////////////////////////////////////////////////////////////
void SceneGraphProcessor::fitMultiplePlanes
(const Vec3f& center, const Vec3f& size,
 const int depth, const float scale, const bool cube,
 vector<float>& f) {
    
  // 1) find or create the POINTS Shape node; extract coordinates and normal
  // vectors from the IndexedFaceSet stored in the geometry field
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    
    IndexedFaceSet* points;
    Node* node;
    bool pointsCheck=false;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            Shape* shape = (Shape*) node;
            if (node->getName()=="POINTS") {
                pointsCheck=true;
                node=shape->getGeometry();
                
                if (node->isIndexedFaceSet()) {
                    points=(IndexedFaceSet*) node;
                    if ((points->getCoord()).size()==0 ) {throw string("coord array size 0");}
                    if ((points->getNormal()).size()==0) {throw string("normal array size 0");}
                }
            }
        }
    }
    
    if (!pointsCheck) {return;}
    
    vector<float>& coordPoints = points->getCoord();
    vector<float>& normalPoints = points->getNormal();
    

  // 2) find or create the SURFACE Shape node, and clear the
  // IndexedFaceSet stored in the geometry field
    SceneGraphTraversal traversal1(_wrl);
    traversal1.start();
    
    IndexedFaceSet* surfaceIfs; //add all the marching cubes output here
    Node* node1;
    bool surfaceCheck=false;
    
    while((node1=traversal1.next())!=(Node*)0) {
        if(node1->isShape()) {
            Shape* shape1 = (Shape*) node1;
            if (node1->getName()=="SURFACE") {
                surfaceCheck=true;
                node1=shape1->getGeometry();
                
                if (node!=(Node*)0 && node->isIndexedFaceSet()) {
                    surfaceIfs=(IndexedFaceSet*) node;
                    if ((surfaceIfs->getCoord()).size()==0 ) {throw string("coord array size 0");}
                    if ((surfaceIfs->getNormal()).size()==0) {throw string("normal array size 0");}
                }
            }
        }
    }
    
    if (!surfaceCheck) {
        //create surface
        Shape* surface= new Shape();
        surfaceIfs=new IndexedFaceSet();
        surface->setGeometry(surfaceIfs);
        
        surface->setName("SURFACE");
        
        Appearance* appr= new Appearance();
        Material* matr= new Material();
        Color diffCol=matr->getDiffuseColor();
        diffCol=*(new Color());
        
        appr->setMaterial(matr);
        surface->setAppearance(appr);
        
        _wrl.addChild(surface);
    }

  // 3) the argument vector<float>&f will contain one scalar values; // clear it here
    f.clear();

  // 4) if the bounding box is empty (size.x==size.y==size.z==0) or scale<=0 // throw an StrException
    if ((size.x==0.0 && size.y==0.0 && size.z==0.0)|| scale<=0) {throw string("bounding box is empty");}

  // 5) compute the coordinates of the 8 vertices of the scaled bounding box
    Vec3f _bboxCenter =_wrl.getBBoxCenter();
    Vec3f _bboxSize = _wrl.getBBoxSize();
    
    float xSize=_bboxSize.x;
    float ySize=_bboxSize.y;
    float zSize=_bboxSize.z;
    
    //min and max corners
    Vec3f minCornerAll; Vec3f maxCornerAll;
    
    minCornerAll.x=_bboxCenter.x-xSize/2.0;
    minCornerAll.y=_bboxCenter.y-ySize/2.0;
    minCornerAll.z=_bboxCenter.z-zSize/2.0;
    
    maxCornerAll.x=_bboxCenter.x+xSize/2.0;
    maxCornerAll.y=_bboxCenter.y+ySize/2.0;
    maxCornerAll.z=_bboxCenter.z+zSize/2.0;
    
  // 6) create a partition of the points as an array of linked lists
    int nPointsInPartition=_createPartition(minCornerAll,maxCornerAll,depth,coordPoints);

  // 7) for each occupied cell, fit a plane, and compute the isosurface polygon within the corresponding cube; accumulate polygons as vertices faces, and face normals of the SURFACE IndexedFaceSet
    int N=pow(2, depth);
    
    //You only have to add the construction of the partition, and a loop to visit all the cells.
    //Within each cell that contains points, you should compute the minCell and maxCell coordinates
    //and then perform the same steps as in the fitSinglePlane() method.
    
    vector<float>& surfaceIfsCoord=surfaceIfs->getCoord(); //surfaceIfs.clear();
    vector<int>& surfaceIfsCoordIndex=surfaceIfs->getCoordIndex(); //surfaceIfsCoordIndex.clear();
    vector<float>& surfaceIfsNormal=surfaceIfs->getNormal(); //surfaceIfsNormal.clear();
    
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            for (int k=0; k<N; k++) {
                
                bool containsPoints=false;
                int iCell = k+N*(j+N*i);
                
                //if cell contains points
                if (_first[iCell]>=0) {containsPoints=true;}
            
                if (containsPoints) {
                    Vec3f minCell; Vec3f maxCell; Vec4f hessianPlane4vec; //replace "f" with this edit all in scope
                
                    minCell.x = ((N-i )*minCornerAll.x+(i )*maxCornerAll.x)/(N);
                    maxCell.x = ((N-i-1)*minCornerAll.x+(i+1)*maxCornerAll.x)/(N);
                    minCell.y = ((N-j )*minCornerAll.x+(j )*maxCornerAll.x)/(N);
                    maxCell.y = ((N-j-1)*minCornerAll.x+(j+1)*maxCornerAll.x)/(N);
                    minCell.z = ((N-k )*minCornerAll.x+(k )*maxCornerAll.x)/(N);
                    maxCell.z = ((N-k-1)*minCornerAll.x+(k+1)*maxCornerAll.x)/(N);
            
                     // 3) compute the coordinates of the current cell bounding box corners
                     Vec3f _currentCellBboxCenter; Vec3f _currentCellBboxSize;
                     
                     //get all 8 corners
                     std::vector<Vec3f> currentCellCorners;

               /* cerr << "//                6 ----- 7 = (x1,y1,z1)" << endl;
                cerr << "//               /|      /|             " << endl;
                cerr << "//              4 ----- 5 |             " << endl;
                cerr << "//              | |     | |             " << endl;
                cerr << "//              | 2 ----| 3             " << endl;
                cerr << "//              |/      |/              " << endl;
                cerr << "// (x0,y0,z0) = 0 ----- 1 x               " << endl; */
                
                    //currentCellCorners
                    Vec3f corner0;Vec3f corner1;Vec3f corner2;Vec3f corner3;Vec3f corner4;Vec3f corner5;Vec3f corner6;Vec3f corner7;
                    float xCellSize=(maxCell.x-minCell.x);
                    float yCellSize=(maxCell.y-minCell.y);
                    float zCellSize=(maxCell.z-minCell.z);
                    
                    corner0.x=minCell.x;
                    corner0.y=minCell.y;
                    corner0.z=minCell.z;
                    currentCellCorners.push_back(corner0);
                
                    corner1.x=(minCell.x+xCellSize);
                    corner1.y=minCell.y;
                    corner1.z=minCell.z;
                    currentCellCorners.push_back(corner1);
                    
                    corner2.x=minCell.x;
                    corner2.y=(minCell.y+yCellSize);
                    corner2.z=minCell.z;
                    currentCellCorners.push_back(corner2);
                    
                    corner3.x=maxCell.x;
                    corner3.y=maxCell.y;
                    corner3.z=(maxCell.z-zCellSize);
                    currentCellCorners.push_back(corner3);
                    
                    corner4.x=minCell.x;
                    corner4.y=minCell.y;
                    corner4.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner4);
                    
                    corner5.x=(minCell.x+xCellSize);
                    corner5.y=minCell.y;
                    corner5.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner5);
                    
                    corner6.x=minCell.x;
                    corner6.y=(minCell.y+yCellSize);
                    corner6.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner6);
                    
                    corner7.x=maxCell.x;
                    corner7.y=maxCell.y;
                    corner7.z=maxCell.z;
                    currentCellCorners.push_back(corner7);
                    
                
                     // 4) fit a plane to the points contained in the bounding box
                     //not used in stencil code eigenFit(coordPoints,min,max,f);
                    meanFit(coordPoints, normalPoints, minCell, maxCell, hessianPlane4vec);
                    
                    // 6) evaluate the linear function at bounding box corners
                    float F[8]; // function values at bbox corners
                    bool  b[8]; // function is positive or negative ?
                    for(int i=0;i<8;i++) {
                        float x= (currentCellCorners.at(i)).x;
                        float y= (currentCellCorners.at(i)).y;
                        float z= (currentCellCorners.at(i)).z;

                        // set (x,y,z) the the i=th corner coordinates
                        // computed in step 3) above
                        b[i] = (F[i] = x*hessianPlane4vec.x+y*hessianPlane4vec.y+z*hessianPlane4vec.z+hessianPlane4vec.w)<0.0f;
                    }
                
                     const int (*edge)[2] = IsoSurf::getEdgeTable();
                     
                     // 7) compute the isovertex coordinates; can you explain what is going on here?
                     float tj,tk;
                     int   iE[12],iV,i1,j1,k1;
                     for(i1=0;i1<12;i1++) {
                         iV   = -1;
                         j1    = edge[i1][0];
                         k1    = edge[i1][1];
                         if(b[j1]!=b[k1]) {
                             // isvertex index
                             iV = (int)((surfaceIfsCoord.size()/3));
                             // isovertex coordinates
                             tk = F[j1]/(F[j1]-F[k1]);
                             tj = F[k1]/(F[k1]-F[j1]);
                             //changed int to float
                             float x1  = tj*currentCellCorners[j1].x+tk*currentCellCorners[k1].x;
                             float y1  = tj*currentCellCorners[j1].y+tk*currentCellCorners[k1].y;
                             float z1  = tj*currentCellCorners[j1].z+tk*currentCellCorners[k1].z;
                            
                             //problem populating surfaceIfscoord, it is all 0's with bunny-opts
                             //x1+=minCell.x; y1+=minCell.y; z1+=minCell.z;
                             surfaceIfsCoord.push_back(x1);
                             surfaceIfsCoord.push_back(y1);
                             surfaceIfsCoord.push_back(z1);
                          }
                     iE[i1] = iV;
                     }
                     
                     // 8) create isosurface faces
                     int nFaces = IsoSurf::makeCellFaces(b,iE,surfaceIfsCoordIndex);
                    
                     // 9) save face normal
                     surfaceIfs->setNormalPerVertex(false);
                     surfaceIfsNormal.push_back(hessianPlane4vec.x);
                     surfaceIfsNormal.push_back(hessianPlane4vec.y);
                     surfaceIfsNormal.push_back(hessianPlane4vec.z);
                
                } //end of if (points in the cell) statement...
                
            }
        }
    }
    //for debuggging
    /*cout << "is the surface shown? getshow() : " << surfaceIfs->getShow() << endl;
    
    cout << "final surfaceIfsCoord size is: " << surfaceIfsCoord.size() << endl;
    
    cout << "final surfacecoordIfsIndex size is: " << surfaceIfsCoordIndex.size() << endl;
    
    cout << "final surfaceIfsNormal size is: " << surfaceIfsNormal.size() << endl;
    
    cout << "printing all surface point coords..."<< endl;
    for (int p=0; p<surfaceIfsCoord.size(); p++) {
        cout << surfaceIfsCoord.at(p) << " ";
    }
    cout << endl;
    
    cout << "printing all surfaceCoordIndex entries..."<< endl;
    for (int p=0; p<surfaceIfsCoordIndex.size(); p++) {
        cout << surfaceIfsCoordIndex.at(p) << " ";
    }
    cout << endl;
    
    
    cout << "printing all surfaceIfsNormal entries..."<< endl;
    for (int p=0; p<surfaceIfsNormal.size(); p++) {
        cout << surfaceIfsNormal.at(p) << " ";
    }
    cout << endl;*/
    

  // 8) delete partition
  _deletePartition();

}



//////////////////////////////////////////////////////////////////////
void SceneGraphProcessor::fitContinuous
(const Vec3f& center, const Vec3f& size,
 const int depth, const float scale, const bool cube) {

  // 1) same as fitMultiplePlanes, but rather than computing the
  // isosurfaces within the hexahedral cells right away, first save
  // the linear function values at the corners of the cells

  // 2) note that empty cells do not have function values associated with

  // 3) for each grid vertex of an occupied incident cell, compute the
  // average of the values saved on the incident cell corners; and
  // then replace the incident cell corner values by the average.

  // 4) now compute the isosurface polygon with the cells, as in the
  // fitMultiplePlanes method, but using the new values instead

  // 5) fill the SURFACE IndexedFaceSet exactly as in the
  // fitMultiplePlanes method
    
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    
    IndexedFaceSet* points;
    Node* node;
    bool pointsCheck=false;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            Shape* shape = (Shape*) node;
            if (node->getName()=="POINTS") {
                pointsCheck=true;
                node=shape->getGeometry();
                
                if (node->isIndexedFaceSet()) {
                    points=(IndexedFaceSet*) node;
                    if ((points->getCoord()).size()==0 ) {throw string("coord array size 0");}
                    if ((points->getNormal()).size()==0) {throw string("normal array size 0");}
                }
            }
        }
    }
    
    if (!pointsCheck) {return;}
    
    vector<float>& coordPoints = points->getCoord();
    vector<float>& normalPoints = points->getNormal();
    
    
    // 2) find or create the SURFACE Shape node, and clear the IndexedFaceSet stored in the geometry field
    SceneGraphTraversal traversal1(_wrl);
    traversal1.start();
    
    IndexedFaceSet* surfaceIfs; //add all the marching cubes output here
    Node* node1;
    bool surfaceCheck=false;
    
    while((node1=traversal1.next())!=(Node*)0) {
        if(node1->isShape()) {
            Shape* shape1 = (Shape*) node1;
            if (node1->getName()=="SURFACE") {
                surfaceCheck=true;
                node1=shape1->getGeometry();
                
                if (node!=(Node*)0 && node->isIndexedFaceSet()) {
                    surfaceIfs=(IndexedFaceSet*) node;
                    if ((surfaceIfs->getCoord()).size()==0 ) {throw string("coord array size 0");}
                    if ((surfaceIfs->getNormal()).size()==0) {throw string("normal array size 0");}
                }
            }
        }
    }
    
    if (!surfaceCheck) {
        //create surface
        Shape* surface= new Shape();
        surfaceIfs=new IndexedFaceSet();
        surface->setGeometry(surfaceIfs);
        
        surface->setName("SURFACE");
        
        Appearance* appr= new Appearance();
        Material* matr= new Material();
        Color diffCol=matr->getDiffuseColor();
        diffCol=*(new Color());
        
        appr->setMaterial(matr);
        surface->setAppearance(appr);
        
        _wrl.addChild(surface);
    }
    
    if (size.x<0.0 || size.y<0.0 || size.z<0.0) {throw string("invalid size");} //invalid size
    
    // 4) if the bounding box is empty (size.x==size.y==size.z==0) or scale<=0 //throw an StrException
    if ((size.x==0.0 && size.y==0.0 && size.z==0.0)|| scale<=0) {throw string("bounding box is empty");}

    // 5) compute the coordinates of the 8 vertices of the scaled bounding box
    Vec3f _bboxCenter =_wrl.getBBoxCenter();
    Vec3f _bboxSize = _wrl.getBBoxSize();
    
    float xSize=_bboxSize.x;
    float ySize=_bboxSize.y;
    float zSize=_bboxSize.z;
    
    xSize*=scale; ySize*=scale; zSize*=scale;
    
    //min and max corners for entire box
    Vec3f minCornerAll; Vec3f maxCornerAll;
    
    minCornerAll.x=_bboxCenter.x-xSize/2.0;
    minCornerAll.y=_bboxCenter.y-ySize/2.0;
    minCornerAll.z=_bboxCenter.z-zSize/2.0;
    
    maxCornerAll.x=_bboxCenter.x+xSize/2.0;
    maxCornerAll.y=_bboxCenter.y+ySize/2.0;
    maxCornerAll.z=_bboxCenter.z+zSize/2.0;
    
    // 6) create a partition of the points as an array of linked lists
    int nPointsInPartition=_createPartition(minCornerAll,maxCornerAll,depth,coordPoints);
    int N=pow(2, depth);
    
    //CONTINUOUS FIT CODE
    int nGridVertices=(N+1)*(N+1)*(N+1);
    float* fGridVertex = new float[nGridVertices];
    float* wGridVertex = new float[nGridVertices];
    
    //initialize to 0’s
    memset(fGridVertex, 0.0f, 4*nGridVertices);
    memset(wGridVertex, 0.0f, 4*nGridVertices);
    
    //fGridVertex[nGridVertices]= {0}; //wGridVertex[nGridVertices]= {0};
    
    vector<float>& surfaceIfsCoord=surfaceIfs->getCoord();
    vector<int>& surfaceIfsCoordIndex=surfaceIfs->getCoordIndex();
    vector<float>& surfaceIfsNormal=surfaceIfs->getNormal();
    vector<int> iVB;
    
    
    float F[8]; // function values at bbox corners
    bool  b[8]; // function is positive or negative ?
    SimpleGraphMap map(N);
    
    //for non empty each cell (i,j,k)
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            for (int k=0; k<N; k++) {
                bool containsPoints=false;
                int iCell = k+N*(j+N*i);
                
                //if cell contains points
                if (_first[iCell]>=0) {containsPoints=true;}
                
                if (containsPoints) {
                    int temp=iCell;
                    for (int p=0; p<8; p++) {iVB.push_back(temp); temp++;}
                    
                    std::vector<Vec3f> currentCellCorners;
                    Vec4f hessianPlane4vec;
                    Vec3f minCell; Vec3f maxCell;
                    
                    // 3) compute the coordinates of the current cell bounding box corners
                    //here min and max corners are computed by index
                    minCell.x = ((N-i )*minCornerAll.x+(i )*maxCornerAll.x)/(N);
                    maxCell.x = ((N-i-1)*minCornerAll.x+(i+1)*maxCornerAll.x)/(N);
                    minCell.y = ((N-j )*minCornerAll.x+(j )*maxCornerAll.x)/(N);
                    maxCell.y = ((N-j-1)*minCornerAll.x+(j+1)*maxCornerAll.x)/(N);
                    minCell.z = ((N-k )*minCornerAll.x+(k )*maxCornerAll.x)/(N);
                    maxCell.z = ((N-k-1)*minCornerAll.x+(k+1)*maxCornerAll.x)/(N);
                    
                    //get all 8 corners and put in currentCellCorners
                    Vec3f corner0;Vec3f corner1;Vec3f corner2;Vec3f corner3;Vec3f corner4;Vec3f corner5;Vec3f corner6;Vec3f corner7;
                    float xCellSize=(maxCell.x-minCell.x);
                    float yCellSize=(maxCell.y-minCell.y);
                    float zCellSize=(maxCell.z-minCell.z);
                    
                    corner0.x=minCell.x; corner0.y=minCell.y; corner0.z=minCell.z;
                    currentCellCorners.push_back(corner0);
                    
                    corner1.x=(minCell.x+xCellSize);
                    corner1.y=minCell.y;
                    corner1.z=minCell.z;
                    currentCellCorners.push_back(corner1);
                    
                    corner2.x=minCell.x;
                    corner2.y=(minCell.y+yCellSize);
                    corner2.z=minCell.z;
                    currentCellCorners.push_back(corner2);
                    
                    corner3.x=maxCell.x;
                    corner3.y=maxCell.y;
                    corner3.z=(maxCell.z-zCellSize);
                    currentCellCorners.push_back(corner3);
                    
                    corner4.x=minCell.x;
                    corner4.y=minCell.y;
                    corner4.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner4);
                    
                    corner5.x=(minCell.x+xCellSize);
                    corner5.y=minCell.y;
                    corner5.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner5);
                    
                    corner6.x=minCell.x;
                    corner6.y=(minCell.y+yCellSize);
                    corner6.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner6);
                    
                    corner7.x=maxCell.x; corner7.y=maxCell.y; corner7.z=maxCell.z;
                    currentCellCorners.push_back(corner7);
                    
                    //fit linear function Vec4f hessianplane4vec to points contained in the cell
                    meanFit(coordPoints, normalPoints, minCell, maxCell, hessianPlane4vec);
                    
                    surfaceIfs->setNormalPerVertex(false);
                    surfaceIfsNormal.push_back(hessianPlane4vec.x);
                    surfaceIfsNormal.push_back(hessianPlane4vec.y);
                    surfaceIfsNormal.push_back(hessianPlane4vec.z);
                    
                    //from fitMultiplePlanes 6) evaluate the linear function at bounding box corners
                    //for each corner of the cell
                    for (int iC=0; iC<8; iC++) {
                        //determine the coordinates of the corner
                        float cornerx=(currentCellCorners.at(iC)).x;
                        float cornery=(currentCellCorners.at(iC)).y;
                        float cornerz=(currentCellCorners.at(iC)).z;
                        
                        //evaluate the linear function
                        float temp = cornerx*hessianPlane4vec.x+cornery*hessianPlane4vec.y+cornerz*hessianPlane4vec.z+hessianPlane4vec.w;

                        int iV=0;
                        //determine the grid vertex index iV of the corner
                        if (iC==0) iV = (i  )+(N+1)*((j  )+(N+1)*(k  ));
                        if (iC==1) iV = (i  )+(N+1)*((j  )+(N+1)*(k+1));
                        if (iC==2) iV = (i  )+(N+1)*((j+1)+(N+1)*(k  ));
                        if (iC==3) iV = (i  )+(N+1)*((j+1)+(N+1)*(k+1));
                        if (iC==4) iV = (i+1)+(N+1)*((j  )+(N+1)*(k  ));
                        if (iC==5) iV = (i+1)+(N+1)*((j  )+(N+1)*(k+1));
                        if (iC==6) iV = (i+1)+(N+1)*((j+1)+(N+1)*(k  ));
                        if (iC==7) iV = (i+1)+(N+1)*((j+1)+(N+1)*(k+1));

                        //add the evaluated value to fGridVertex[iV]
                            fGridVertex[iV]=temp;
                        
                        //increment wGridVertex[iV] by one
                            wGridVertex[iV]++;
                    }
                    
                    currentCellCorners.clear();
                }
                
            }//end for loops
        }
    }
    
    //normalize function values
    for(int iV=0;iV<nGridVertices;iV++) {
        if(wGridVertex[iV]>0.0f) {
            cout << "wgridvertex normalize val at " << iV <<" is: " << wGridVertex[iV] << endl;
            fGridVertex[iV] /= wGridVertex[iV];
        }
        cout << "func value at index " << iV <<" is: " << fGridVertex[iV] << endl;
    }
    
    //for non empty each cell (i,j,k) { isosurface generation
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            for (int k=0; k<N; k++) {
                bool containsPoints=false;
                int iCell = k+N*(j+N*i);
                if (_first[iCell]>=0) {containsPoints=true;} //check if cell contains points
                
                if (containsPoints) {
                    std::vector<Vec3f> currentCellCorners; Vec3f minCell; Vec3f maxCell;
                    
                    // 3) compute the coordinates of the current cell bounding box corners
                    //here min and max corners are computed by index
                    minCell.x = ((N-i )*minCornerAll.x+(i )*maxCornerAll.x)/(N);
                    maxCell.x = ((N-i-1)*minCornerAll.x+(i+1)*maxCornerAll.x)/(N);
                    minCell.y = ((N-j )*minCornerAll.x+(j )*maxCornerAll.x)/(N);
                    maxCell.y = ((N-j-1)*minCornerAll.x+(j+1)*maxCornerAll.x)/(N);
                    minCell.z = ((N-k )*minCornerAll.x+(k )*maxCornerAll.x)/(N);
                    maxCell.z = ((N-k-1)*minCornerAll.x+(k+1)*maxCornerAll.x)/(N);
                    
                    //get all 8 corners and put in currentCellCorners
                    Vec3f corner0;Vec3f corner1;Vec3f corner2;Vec3f corner3;Vec3f corner4;Vec3f corner5;Vec3f corner6;Vec3f corner7; Vec4f hessianPlane4vec;
                    float xCellSize=(maxCell.x-minCell.x);
                    float yCellSize=(maxCell.y-minCell.y);
                    float zCellSize=(maxCell.z-minCell.z);
                    
                    corner0.x=minCell.x; corner0.y=minCell.y; corner0.z=minCell.z;
                    currentCellCorners.push_back(corner0);
                    
                    corner1.x=(minCell.x+xCellSize);
                    corner1.y=minCell.y;
                    corner1.z=minCell.z;
                    currentCellCorners.push_back(corner1);
                    
                    corner2.x=minCell.x;
                    corner2.y=(minCell.y+yCellSize);
                    corner2.z=minCell.z;
                    currentCellCorners.push_back(corner2);
                    
                    corner3.x=maxCell.x;
                    corner3.y=maxCell.y;
                    corner3.z=(maxCell.z-zCellSize);
                    currentCellCorners.push_back(corner3);
                    
                    corner4.x=minCell.x;
                    corner4.y=minCell.y;
                    corner4.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner4);
                    
                    corner5.x=(minCell.x+xCellSize);
                    corner5.y=minCell.y;
                    corner5.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner5);
                    
                    corner6.x=minCell.x;
                    corner6.y=(minCell.y+yCellSize);
                    corner6.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner6);
                    
                    corner7.x=maxCell.x; corner7.y=maxCell.y; corner7.z=maxCell.z;
                    currentCellCorners.push_back(corner7);
                
                    
                    for(int iC=0;iC<8;iC++) { // for each corner of the cell
                        //determine the grid vertex index iV of the corner
                        int iV=0;
                        if (iC==0) iV = (i  )+(N+1)*((j  )+(N+1)*(k  ));
                        if (iC==1) iV = (i  )+(N+1)*((j  )+(N+1)*(k+1));
                        if (iC==2) iV = (i  )+(N+1)*((j+1)+(N+1)*(k  ));
                        if (iC==3) iV = (i  )+(N+1)*((j+1)+(N+1)*(k+1));
                        if (iC==4) iV = (i+1)+(N+1)*((j  )+(N+1)*(k  ));
                        if (iC==5) iV = (i+1)+(N+1)*((j  )+(N+1)*(k+1));
                        if (iC==6) iV = (i+1)+(N+1)*((j+1)+(N+1)*(k  ));
                        if (iC==7) iV = (i+1)+(N+1)*((j+1)+(N+1)*(k+1));
                        
                        //get the function value fGridVertex[iV] and store it in F[i]
                        F[iC]= fGridVertex[iV];
                        //determine the function signs b[i]= (F[i]<0);
                        b[iC]= (F[iC]<0.0f);
                    }
                    
                    //same as for fitSimplePlane() and fitMultiplePlanes()
                    const int (*edge)[2] = IsoSurf::getEdgeTable();
                    // 7) compute the isovertex coordinates; can you explain what is going on here?
                    float tj,tk;
                    int   iE[12],iV,i1,j1,k1;
                    for(i1=0;i1<12;i1++) {
                        iV   = -1;
                        j1    = edge[i1][0];
                        k1    = edge[i1][1];
                        if(b[j1]!=b[k1]) {
                            // isvertex index
                            iV = map.get(iVB[j1], iVB[k1]);
                            if (iV<0) {
                                // isovertex coordinates
                                tk = F[j1]/(F[j1]-F[k1]);
                                tj = F[k1]/(F[k1]-F[j1]);
                                //changed int to float
                                float x1  = tj*currentCellCorners[j1].x+tk*currentCellCorners[k1].x;
                                float y1  = tj*currentCellCorners[j1].y+tk*currentCellCorners[k1].y;
                                float z1  = tj*currentCellCorners[j1].z+tk*currentCellCorners[k1].z;
                                
                                surfaceIfsCoord.push_back(x1); surfaceIfsCoord.push_back(y1); surfaceIfsCoord.push_back(z1);
                                map.insert(iVB[j1], iVB[k1], iV);
                            }
                            iE[i1] = iV;
                        }
                        // 8) create isosurface faces
                        int nFaces = IsoSurf::makeCellFaces(b,iE,surfaceIfsCoordIndex);
                        currentCellCorners.clear();
                    }
                    
                }
            }
        }
        
    }
    
    //for debuggging
     /*cout << "is the surface shown? getshow() : " << surfaceIfs->getShow() << endl;
     
     cout << "final surfaceIfsCoord size is: " << surfaceIfsCoord.size() << endl;
     
     cout << "final surfacecoordIfsIndex size is: " << surfaceIfsCoordIndex.size() << endl;
     
     cout << "final surfaceIfsNormal size is: " << surfaceIfsNormal.size() << endl;
     
     cout << "printing all surface point coords..."<< endl;
     for (int p=0; p<surfaceIfsCoord.size(); p++) {cout << surfaceIfsCoord.at(p) << " ";}
     cout << endl;
     
     cout << "printing all surfaceCoordIndex entries..."<< endl;
     for (int p=0; p<surfaceIfsCoordIndex.size(); p++) {cout << surfaceIfsCoordIndex.at(p) << " ";}
     cout << endl;
     
     cout << "printing all surfaceIfsNormal entries..."<< endl;
     for (int p=0; p<surfaceIfsNormal.size(); p++) {cout << surfaceIfsNormal.at(p) << " ";}
     cout << endl;*/
    
}

void SceneGraphProcessor::fitWatertight (const Vec3f& center, const Vec3f& size,
                                         const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */) {
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    
    IndexedFaceSet* points;
    Node* node;
    bool pointsCheck=false;
    
    while((node=traversal.next())!=(Node*)0) {
        if(node->isShape()) {
            Shape* shape = (Shape*) node;
            if (node->getName()=="POINTS") {
                pointsCheck=true;
                node=shape->getGeometry();
                
                if (node->isIndexedFaceSet()) {
                    points=(IndexedFaceSet*) node;
                    if ((points->getCoord()).size()==0 ) {throw string("coord array size 0");}
                    if ((points->getNormal()).size()==0) {throw string("normal array size 0");}
                }
            }
        }
    }
    
    if (!pointsCheck) {return;}
    
    vector<float>& coordPoints = points->getCoord();
    vector<float>& normalPoints = points->getNormal();
    
    
    // 2) find or create the SURFACE Shape node, and clear the IndexedFaceSet stored in the geometry field
    SceneGraphTraversal traversal1(_wrl);
    traversal1.start();
    
    IndexedFaceSet* surfaceIfs; //add all the marching cubes output here
    Node* node1;
    bool surfaceCheck=false;
    
    while((node1=traversal1.next())!=(Node*)0) {
        if(node1->isShape()) {
            Shape* shape1 = (Shape*) node1;
            if (node1->getName()=="SURFACE") {
                surfaceCheck=true;
                node1=shape1->getGeometry();
                
                if (node!=(Node*)0 && node->isIndexedFaceSet()) {
                    surfaceIfs=(IndexedFaceSet*) node;
                    if ((surfaceIfs->getCoord()).size()==0 ) {throw string("coord array size 0");}
                    if ((surfaceIfs->getNormal()).size()==0) {throw string("normal array size 0");}
                }
            }
        }
    }
    
    if (!surfaceCheck) {
        //create surface
        Shape* surface= new Shape();
        surfaceIfs=new IndexedFaceSet();
        surface->setGeometry(surfaceIfs);
        
        surface->setName("SURFACE");
        
        Appearance* appr= new Appearance();
        Material* matr= new Material();
        Color diffCol=matr->getDiffuseColor();
        diffCol=*(new Color());
        
        appr->setMaterial(matr);
        surface->setAppearance(appr);
        
        _wrl.addChild(surface);
    }
    
    if (size.x<0.0 || size.y<0.0 || size.z<0.0) {throw string("invalid size");} //invalid size
    
    // 4) if the bounding box is empty (size.x==size.y==size.z==0) or scale<=0 //throw an StrException
    if ((size.x==0.0 && size.y==0.0 && size.z==0.0)|| scale<=0) {throw string("bounding box is empty");}
    
    // 5) compute the coordinates of the 8 vertices of the scaled bounding box
    Vec3f _bboxCenter =_wrl.getBBoxCenter();
    Vec3f _bboxSize = _wrl.getBBoxSize();
    
    float xSize=_bboxSize.x;
    float ySize=_bboxSize.y;
    float zSize=_bboxSize.z;
    
    xSize*=scale; ySize*=scale; zSize*=scale;
    
    //min and max corners for entire box
    Vec3f minCornerAll; Vec3f maxCornerAll;
    
    minCornerAll.x=_bboxCenter.x-xSize/2.0;
    minCornerAll.y=_bboxCenter.y-ySize/2.0;
    minCornerAll.z=_bboxCenter.z-zSize/2.0;
    
    maxCornerAll.x=_bboxCenter.x+xSize/2.0;
    maxCornerAll.y=_bboxCenter.y+ySize/2.0;
    maxCornerAll.z=_bboxCenter.z+zSize/2.0;
    
    // 6) create a partition of the points as an array of linked lists
    int nPointsInPartition=_createPartition(minCornerAll,maxCornerAll,depth,coordPoints);
    int N=pow(2, depth);
    
    //CONTINUOUS FIT CODE
    int nGridVertices=(N+1)*(N+1)*(N+1);
    float* fGridVertex = new float[nGridVertices];
    float* wGridVertex = new float[nGridVertices];
    
    //initialize to 0’s
    memset(fGridVertex, 0.0f, 4*nGridVertices);
    memset(wGridVertex, 0.0f, 4*nGridVertices);
    
    //fGridVertex[nGridVertices]= {0}; //wGridVertex[nGridVertices]= {0};
    
    vector<float>& surfaceIfsCoord=surfaceIfs->getCoord();
    vector<int>& surfaceIfsCoordIndex=surfaceIfs->getCoordIndex();
    vector<float>& surfaceIfsNormal=surfaceIfs->getNormal();
    
    float F[8]; // function values at bbox corners
    bool  b[8]; // function is positive or negative ?
    
    //for non empty each cell (i,j,k)
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            for (int k=0; k<N; k++) {
                bool containsPoints=false;
                int iCell = k+N*(j+N*i);
                
                //if cell contains points
                if (_first[iCell]>=0) {containsPoints=true;}
                
                if (containsPoints) {
                    std::vector<Vec3f> currentCellCorners;
                    Vec4f hessianPlane4vec;
                    Vec3f minCell; Vec3f maxCell;
                    
                    // 3) compute the coordinates of the current cell bounding box corners
                    //here min and max corners are computed by index
                    minCell.x = ((N-i )*minCornerAll.x+(i )*maxCornerAll.x)/(N);
                    maxCell.x = ((N-i-1)*minCornerAll.x+(i+1)*maxCornerAll.x)/(N);
                    minCell.y = ((N-j )*minCornerAll.x+(j )*maxCornerAll.x)/(N);
                    maxCell.y = ((N-j-1)*minCornerAll.x+(j+1)*maxCornerAll.x)/(N);
                    minCell.z = ((N-k )*minCornerAll.x+(k )*maxCornerAll.x)/(N);
                    maxCell.z = ((N-k-1)*minCornerAll.x+(k+1)*maxCornerAll.x)/(N);
                    
                    //get all 8 corners and put in currentCellCorners
                    Vec3f corner0;Vec3f corner1;Vec3f corner2;Vec3f corner3;Vec3f corner4;Vec3f corner5;Vec3f corner6;Vec3f corner7;
                    float xCellSize=(maxCell.x-minCell.x);
                    float yCellSize=(maxCell.y-minCell.y);
                    float zCellSize=(maxCell.z-minCell.z);
                    
                    corner0.x=minCell.x; corner0.y=minCell.y; corner0.z=minCell.z;
                    currentCellCorners.push_back(corner0);
                    
                    corner1.x=(minCell.x+xCellSize);
                    corner1.y=minCell.y;
                    corner1.z=minCell.z;
                    currentCellCorners.push_back(corner1);
                    
                    corner2.x=minCell.x;
                    corner2.y=(minCell.y+yCellSize);
                    corner2.z=minCell.z;
                    currentCellCorners.push_back(corner2);
                    
                    corner3.x=maxCell.x;
                    corner3.y=maxCell.y;
                    corner3.z=(maxCell.z-zCellSize);
                    currentCellCorners.push_back(corner3);
                    
                    corner4.x=minCell.x;
                    corner4.y=minCell.y;
                    corner4.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner4);
                    
                    corner5.x=(minCell.x+xCellSize);
                    corner5.y=minCell.y;
                    corner5.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner5);
                    
                    corner6.x=minCell.x;
                    corner6.y=(minCell.y+yCellSize);
                    corner6.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner6);
                    
                    corner7.x=maxCell.x; corner7.y=maxCell.y; corner7.z=maxCell.z;
                    currentCellCorners.push_back(corner7);
                    
                    //fit linear function Vec4f hessianplane4vec to points contained in the cell
                    meanFit(coordPoints, normalPoints, minCell, maxCell, hessianPlane4vec);
                
                    
                    surfaceIfs->setNormalPerVertex(false);
                    surfaceIfsNormal.push_back(hessianPlane4vec.x);
                    surfaceIfsNormal.push_back(hessianPlane4vec.y);
                    surfaceIfsNormal.push_back(hessianPlane4vec.z);
                    
                    //from fitMultiplePlanes 6) evaluate the linear function at bounding box corners
                    //for each corner of the cell
                    for (int iC=0; iC<8; iC++) {
                        //determine the coordinates of the corner
                        float cornerx=(currentCellCorners.at(iC)).x;
                        float cornery=(currentCellCorners.at(iC)).y;
                        float cornerz=(currentCellCorners.at(iC)).z;
                        
                        //evaluate the linear function
                        float temp = cornerx*hessianPlane4vec.x+cornery*hessianPlane4vec.y+cornerz*hessianPlane4vec.z+hessianPlane4vec.w;
                        
                        int iV=0;
                        //determine the grid vertex index iV of the corner
                        if (iC==0) iV = (i  )+(N+1)*((j  )+(N+1)*(k  ));
                        if (iC==1) iV = (i  )+(N+1)*((j  )+(N+1)*(k+1));
                        if (iC==2) iV = (i  )+(N+1)*((j+1)+(N+1)*(k  ));
                        if (iC==3) iV = (i  )+(N+1)*((j+1)+(N+1)*(k+1));
                        if (iC==4) iV = (i+1)+(N+1)*((j  )+(N+1)*(k  ));
                        if (iC==5) iV = (i+1)+(N+1)*((j  )+(N+1)*(k+1));
                        if (iC==6) iV = (i+1)+(N+1)*((j+1)+(N+1)*(k  ));
                        if (iC==7) iV = (i+1)+(N+1)*((j+1)+(N+1)*(k+1));
                        
                        //add the evaluated value to fGridVertex[iV]
                        fGridVertex[iV]=temp;
                        
                        //increment wGridVertex[iV] by one
                        wGridVertex[iV]++;
                    }
                    currentCellCorners.clear();
                }
                
            }//end for loops
        }
    }
    
    //normalize function values
    for(int iV=0;iV<nGridVertices;iV++) {
        if(wGridVertex[iV]>0.0f) {
            cout << "wgridvertex normalize val at " << iV <<" is: " << wGridVertex[iV] << endl;
            fGridVertex[iV] /= wGridVertex[iV];
        }
        cout << "func value at index " << iV <<" is: " << fGridVertex[iV] << endl;
    }
    
    //for non empty each cell (i,j,k) { isosurface generation
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            for (int k=0; k<N; k++) {
                bool containsPoints=false;
                int iCell = k+N*(j+N*i);
                if (_first[iCell]>=0) {containsPoints=true;} //check if cell contains points
                
                if (containsPoints) {
                    std::vector<Vec3f> currentCellCorners; Vec3f minCell; Vec3f maxCell;
                    
                    // 3) compute the coordinates of the current cell bounding box corners
                    //here min and max corners are computed by index
                    minCell.x = ((N-i )*minCornerAll.x+(i )*maxCornerAll.x)/(N);
                    maxCell.x = ((N-i-1)*minCornerAll.x+(i+1)*maxCornerAll.x)/(N);
                    minCell.y = ((N-j )*minCornerAll.x+(j )*maxCornerAll.x)/(N);
                    maxCell.y = ((N-j-1)*minCornerAll.x+(j+1)*maxCornerAll.x)/(N);
                    minCell.z = ((N-k )*minCornerAll.x+(k )*maxCornerAll.x)/(N);
                    maxCell.z = ((N-k-1)*minCornerAll.x+(k+1)*maxCornerAll.x)/(N);
                    
                    //get all 8 corners and put in currentCellCorners
                    Vec3f corner0;Vec3f corner1;Vec3f corner2;Vec3f corner3;
                    Vec3f corner4;Vec3f corner5;Vec3f corner6;Vec3f corner7; Vec4f hessianPlane4vec;
                    float xCellSize=(maxCell.x-minCell.x);
                    float yCellSize=(maxCell.y-minCell.y);
                    float zCellSize=(maxCell.z-minCell.z);
                    
                    corner0.x=minCell.x; corner0.y=minCell.y; corner0.z=minCell.z;
                    currentCellCorners.push_back(corner0);
                    
                    corner1.x=(minCell.x+xCellSize);
                    corner1.y=minCell.y;
                    corner1.z=minCell.z;
                    currentCellCorners.push_back(corner1);
                    
                    corner2.x=minCell.x;
                    corner2.y=(minCell.y+yCellSize);
                    corner2.z=minCell.z;
                    currentCellCorners.push_back(corner2);
                    
                    corner3.x=maxCell.x;
                    corner3.y=maxCell.y;
                    corner3.z=(maxCell.z-zCellSize);
                    currentCellCorners.push_back(corner3);
                    
                    corner4.x=minCell.x;
                    corner4.y=minCell.y;
                    corner4.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner4);
                    
                    corner5.x=(minCell.x+xCellSize);
                    corner5.y=minCell.y;
                    corner5.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner5);
                    
                    corner6.x=minCell.x;
                    corner6.y=(minCell.y+yCellSize);
                    corner6.z=(minCell.z+zCellSize);
                    currentCellCorners.push_back(corner6);
                    
                    corner7.x=maxCell.x; corner7.y=maxCell.y; corner7.z=maxCell.z;
                    currentCellCorners.push_back(corner7);
                    
                    
                    for(int iC=0;iC<8;iC++) { // for each corner of the cell
                        //determine the grid vertex index iV of the corner
                        int iV=0;
                        if (iC==0) iV = (i  )+(N+1)*((j  )+(N+1)*(k  ));
                        if (iC==1) iV = (i  )+(N+1)*((j  )+(N+1)*(k+1));
                        if (iC==2) iV = (i  )+(N+1)*((j+1)+(N+1)*(k  ));
                        if (iC==3) iV = (i  )+(N+1)*((j+1)+(N+1)*(k+1));
                        if (iC==4) iV = (i+1)+(N+1)*((j  )+(N+1)*(k  ));
                        if (iC==5) iV = (i+1)+(N+1)*((j  )+(N+1)*(k+1));
                        if (iC==6) iV = (i+1)+(N+1)*((j+1)+(N+1)*(k  ));
                        if (iC==7) iV = (i+1)+(N+1)*((j+1)+(N+1)*(k+1));
                        
                        //get the function value fGridVertex[iV] and store it in F[i]
                        F[iC]= fGridVertex[iV];
                        //determine the function signs b[i]= (F[i]<0);
                        b[iC]= (F[iC]<0.0f);
                    }
                    
                    //fit waterrtight step
                    computeIsosurface(_bboxCenter, _bboxSize, depth, scale, cube, fGrid);
                    
                }
            }
        }
        
    }
    
    //for debuggging
    /*cout << "is the surface shown? getshow() : " << surfaceIfs->getShow() << endl;
     
     cout << "final surfaceIfsCoord size is: " << surfaceIfsCoord.size() << endl;
     
     cout << "final surfacecoordIfsIndex size is: " << surfaceIfsCoordIndex.size() << endl;
     
     cout << "final surfaceIfsNormal size is: " << surfaceIfsNormal.size() << endl;
     
     cout << "printing all surface point coords..."<< endl;
     for (int p=0; p<surfaceIfsCoord.size(); p++) {cout << surfaceIfsCoord.at(p) << " ";}
     cout << endl;
     
     cout << "printing all surfaceCoordIndex entries..."<< endl;
     for (int p=0; p<surfaceIfsCoordIndex.size(); p++) {cout << surfaceIfsCoordIndex.at(p) << " ";}
     cout << endl;
     
     cout << "printing all surfaceIfsNormal entries..."<< endl;
     for (int p=0; p<surfaceIfsNormal.size(); p++) {cout << surfaceIfsNormal.at(p) << " ";}
     cout << endl;*/
    
    
}

void SceneGraphProcessor::computeIsosurface (const Vec3f& center, const Vec3f& size,
                                             const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */) {
    
}

void SceneGraphProcessor::optimalCGHard (const Vec3f& center, const Vec3f& size,
                                         const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */) {
    
}

void SceneGraphProcessor::optimalCGSoft (const Vec3f& center, const Vec3f& size,
                                         const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */) {
    
}

void SceneGraphProcessor::optimalJacobi (const Vec3f& center, const Vec3f& size,
                                         const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */) {
    
}

void SceneGraphProcessor::multiGridFiner (const Vec3f& center, const Vec3f& size,
                                          const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */) {
    
}

void SceneGraphProcessor::multiGridCoarser (const Vec3f& center, const Vec3f& size,
                                            const int depth, const float scale, const bool cube, vector<float>& fGrid /* output */) {
    
}

