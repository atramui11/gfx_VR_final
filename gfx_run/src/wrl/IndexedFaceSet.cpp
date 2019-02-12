

#include "IndexedFaceSet.hpp"
#include <cassert>
#include <iostream>

// VRML'97
//
// IndexedFaceSet {
//   eventIn       MFInt32 set_colorIndex
//   eventIn       MFInt32 set_coordIndex
//   eventIn       MFInt32 set_normalIndex
//   eventIn       MFInt32 set_texCoordIndex
//   exposedField  SFNode  color             NULL
//   exposedField  SFNode  coord             NULL
//   exposedField  SFNode  normal            NULL
//   exposedField  SFNode  texCoord          NULL
//   field         SFBool  ccw               TRUE
//   field         MFInt32 colorIndex        []        # [-1,)
//   field         SFBool  colorPerVertex    TRUE
//   field         SFBool  convex            TRUE
//   field         MFInt32 coordIndex        []        # [-1,)
//   field         SFFloat creaseAngle       0         # [ 0,)
//   field         MFInt32 normalIndex       []        # [-1,)
//   field         SFBool  normalPerVertex   TRUE
//   field         SFBool  solid             TRUE
//   field         MFInt32 texCoordIndex     []        # [-1,)
// }

// TODO ...
// Refactor your IfsViewer code


IndexedFaceSet::IndexedFaceSet() {
    /* remember to properly initialize all the class variables which do
     not have a default constructor */
    _ccw=true;
    _colorPerVertex=true;
    _convex=true;
    _creaseAngle=0;
    _normalPerVertex=true;
    _solid=true;
    
}

IndexedFaceSet::~IndexedFaceSet() {
    
}

void IndexedFaceSet::clear() {
    _ccw=true;
    _colorPerVertex=true;
    _convex=true;
    _creaseAngle=0;
    _normalPerVertex=true;
    _solid=true;
    
    _coord.clear();
    _coordIndex.clear();
    
    _normal.clear();
    _normalIndex.clear();
    
    _color.clear();
    _colorIndex.clear();
    
    _texCoord.clear();
    _texCoordIndex.clear();
    
}

bool&          IndexedFaceSet::getCcw() {return _ccw;}
bool&          IndexedFaceSet::getConvex() {return _convex;}
float&         IndexedFaceSet::getCreaseangle() {return _creaseAngle;}
bool&          IndexedFaceSet::getSolid() {return _solid;}
bool&          IndexedFaceSet::getNormalPerVertex() {return _normalPerVertex;}
bool&          IndexedFaceSet::getColorPerVertex() {return _colorPerVertex;}
vector<float>& IndexedFaceSet::getCoord() { return _coord; }
vector<int>&   IndexedFaceSet::getCoordIndex() { return _coordIndex;}
vector<float>& IndexedFaceSet::getNormal() {return _normal;}
vector<int>&   IndexedFaceSet::getNormalIndex() {return _normalIndex;}
vector<float>& IndexedFaceSet::getColor() {return _color;}
vector<int>&   IndexedFaceSet::getColorIndex() {return _colorIndex;}
vector<float>& IndexedFaceSet::getTexCoord() {return _texCoord;}
vector<int>&   IndexedFaceSet::getTexCoordIndex() {return _texCoordIndex;}

    int IndexedFaceSet::getNumberOfFaces() {
            int count=0;
            for (int i=0; i<_coordIndex.size(); i++) {
                if (_coordIndex.at(i)==-1) {
                    count++;
                }
            }
        cout << "Faces -1 count for coord index is: " << count << endl;
        return count;
            
    }

    int IndexedFaceSet::getNumberOfCorners() {
        /*int count=0;
        for (int i=0; i<_texCoordIndex.size(); i++) {
            if (_texCoordIndex.at(i)==-1) {
                count++;
            }
        }
        cout << "Corners -1 count for texcoord index is: " << count << endl;
        return count;
        */
    }
int            IndexedFaceSet::getNumberOfCoord() {return _coord.size()/3;}
int            IndexedFaceSet::getNumberOfNormal() {return _normal.size()/3;}
int            IndexedFaceSet::getNumberOfColor() {return _color.size();}
int            IndexedFaceSet::getNumberOfTexCoord() {return _texCoord.size()/3;}
void           IndexedFaceSet::setNormalPerVertex(bool value) {_normalPerVertex=value;}
void           IndexedFaceSet::setColorPerVertex(bool value) {_colorPerVertex=value;}
void           IndexedFaceSet::removeTexCoord() {_texCoord.clear();}


// TODO
IndexedFaceSet::Binding IndexedFaceSet::getNormalBinding() {
    if(_normal.size()==0) {
        // NO_NORMALS
        return PB_NONE;
    }
    
    else if(_normalPerVertex==false) {
        
        if(_normalIndex.size()>0) {
            //NORMAL_PER_FACE_INDEXED;
            return PB_PER_FACE_INDEXED;
            assert(_normalIndex.size()==getNumberOfFaces());
        }
        
        else {
            //NORMAL_PER_FACE;
            return PB_PER_FACE;
            assert(_normal.size()/3==getNumberOfFaces());
        }
        
    }
    
    else { // if(normalPerVertex==TRUE) {
        
        if(_normalIndex.size()>0) {
            //NORMAL_PER_CORNER;
            return PB_PER_CORNER;
            assert(_normalIndex.size()==_coordIndex.size());
        }
        
        else {
            //NORMAL_PER_VERTEX;
            return PB_PER_VERTEX;
            assert(_normal.size()/3==_coord.size()/3);
        }
        
    }
    return PB_NONE;
}

IndexedFaceSet::Binding IndexedFaceSet::getCoordBinding() {
    if(_coord.size()==0) {
        // NO_NORMALS
        return PB_NONE;
    }
    
    else if(_normalPerVertex==false) {
        
        if(_normalIndex.size()>0) {
            //NORMAL_PER_FACE_INDEXED;
            return PB_PER_FACE_INDEXED;
            assert(_normalIndex.size()==getNumberOfFaces());
        }
        
        else {
            //NORMAL_PER_FACE;
            return PB_PER_FACE;
            assert(_normal.size()/3==getNumberOfFaces());
        }
        
    }
    
    else { // if(normalPerVertex==TRUE) {
        
        if(_coordIndex.size()>0) {
            //Coord_PER_CORNER;
            return PB_PER_CORNER;
            assert(_normalIndex.size()==_coordIndex.size());
        }
        
        else {
            //NORMAL_PER_VERTEX;
            return PB_PER_VERTEX;
            assert(_normal.size()/3==_coord.size()/3);
        }
        
    }
    return PB_NONE;
    
}



// TODO
IndexedFaceSet::Binding IndexedFaceSet::getColorBinding() {
    if(_color.size()==0){
        return PB_NONE;
    }
    else if(_colorPerVertex==false) {
        if(_colorIndex.size()>0) {
            return PB_PER_FACE_INDEXED;
            assert(_colorIndex.size()==getNumberOfFaces());
        }
        else {
            return PB_PER_FACE;
            assert(_color.size()/3==getNumberOfFaces());
        }
        
    }
    else { // if(colorPerVertex==TRUE) {
        if(_colorIndex.size()>0) {
            return PB_PER_CORNER;
            assert(_colorIndex.size()==_coordIndex.size());
        }
        
        else {
            return PB_PER_VERTEX;
            assert(_color.size()/3==_coord.size()/3);
        }
    }
    return PB_NONE;
}

// TODO
IndexedFaceSet::Binding IndexedFaceSet::getTexCoordBinding() {
    if(_texCoord.size()==0) {
        return PB_NONE;
    }
    else if(_texCoordIndex.size()>0) {
        return PB_PER_CORNER;
        assert(_texCoordIndex.size()==_coordIndex.size());
    }
    else {
        return PB_PER_VERTEX;
        assert(_texCoord.size()/3==_coord.size()/3);
    }
    return PB_NONE;
}

