#include "SaverWrl.hpp"

const char* SaverWrl::_ext = "wrl";

void SaverWrl::saveMaterial(FILE* fp, string indent, Material* material) {
  const char* str = indent.c_str();

  // Material {
  //   SFFloat ambientIntensity 0.2
  //   SFColor diffuseColor     0.8 0.8 0.8
  //   SFColor emissiveColor    0 0 0
  //   SFFloat shininess        0.2
  //   SFColor specularColor    0 0 0
  //   SFFloat transparency     0
  // }

  fprintf(fp,"%sMaterial {\n",str);

  float  ambientIntensity = material->getAmbientIntensity();
  if(ambientIntensity!=0.2f)
    fprintf(fp,"%s ambientIntensity %8.4f\n",str,ambientIntensity);

  Color& diffuseColor     = material->getDiffuseColor();
  if(diffuseColor.r!=0.8f||diffuseColor.g!=0.8f||diffuseColor.b!=0.8f)
    fprintf(fp,"%s diffuseColor %8.4f %8.4f %8.4f\n",str,
            diffuseColor.r,diffuseColor.g,diffuseColor.b);

  Color& emissiveColor    = material->getEmissiveColor();
  if(emissiveColor.r!=0.0f||emissiveColor.g!=0.0f||emissiveColor.b!=0.0f)
    fprintf(fp,"%s emissiveColor %8.4f %8.4f %8.4f\n",str,
            emissiveColor.r,emissiveColor.g,emissiveColor.b);

  float  shininess        = material->getShininess();
  if(shininess!=0.2f)
    fprintf(fp,"%s shininess %8.4f\n",str,shininess);

  Color  specularColor    = material->getSpecularColor();
  if(specularColor.r!=0.0f||specularColor.g!=0.0f||specularColor.b!=0.0f)
    fprintf(fp,"%s specularColor %8.4f %8.4f %8.4f\n",str,
            specularColor.r,specularColor.g,specularColor.b);

  float  transparency     = material->getTransparency();
  if(transparency!=0.2f)
    fprintf(fp,"%s transparency %8.4f\n",str,transparency);

  fprintf(fp,"%s}\n",str);
}

void SaverWrl::saveImageTexture
(FILE* fp, string indent, ImageTexture* imageTexture) {
  const char* str = indent.c_str();

  // ImageTexture {
  //   MFString url []
  //   SFBool repeatS TRUE
  //   SFBool repeatT TRUE
  // }

  fprintf(fp,"%sImageTexture {\n",str);

  vector<string>& url = imageTexture->getUrl();
  if(url.size()==1) {
    fprintf(fp,"%s url %s\n",str,url[0].c_str());
  } else if(url.size()>1) {
    fprintf(fp,"%s url [\n",str);
    for(unsigned i=0;i<url.size();i++)
      fprintf(fp,"%s  %s\n",str,url[i].c_str());
    fprintf(fp,"%s ]\n",str);
  }

  bool repeatS = imageTexture->getRepeatS();
  if(repeatS!=true)
    fprintf(fp,"%s repeatS FALSE\n",str);

  bool repeatT = imageTexture->getRepeatT();
  if(repeatT!=true)
    fprintf(fp,"%s repeatT FALSE\n",str);

  fprintf(fp,"%s}\n",str);
}

void SaverWrl::saveAppearance
(FILE* fp, string indent, Appearance* appearance) {
  const char* str = indent.c_str();

  // Appearance {
  //   SFNode material NULL
  //   SFNode texture NULL
  //   // SFNode textureTransform NULL
  // }

  Node* node;

  fprintf(fp,"%sappearance Appearance {\n",str);

  node = appearance->getMaterial();
  if(node!=(Node*)0) {
    Material* material = (Material*)node;
    fprintf(fp,"%s material\n",str);
    saveMaterial(fp,indent+"  ",material);
  }
  node = appearance->getTexture();
  if(node!=(Node*)0) {
    if(node->isImageTexture()) {
      ImageTexture* imageTexture = (ImageTexture*)node;
      fprintf(fp,"%s texture\n",str);
      saveImageTexture(fp,indent+"  ",imageTexture);
    }
  }

  fprintf(fp,"%s}\n",str);
}

void SaverWrl::saveIndexedFaceSet
(FILE* fp, string indent, IndexedFaceSet* indexedFaceSet) {
  const char* str = indent.c_str();

  // IndexedFaceSet {
  //   SFNode  color             NULL
  //   SFNode  coord             NULL
  //   SFNode  normal            NULL
  //   SFNode  texCoord          NULL
  //   SFBool  ccw               TRUE
  //   MFInt32 colorIndex        []        # [-1,)
  //   SFBool  colorPerVertex    TRUE
  //   SFBool  convex            TRUE
  //   MFInt32 coordIndex        []        # [-1,)
  //   SFFloat creaseAngle       0         # [ 0,)
  //   MFInt32 normalIndex       []        # [-1,)
  //   SFBool  normalPerVertex   TRUE
  //   SFBool  solid             TRUE
  //   MFInt32 texCoordIndex     []        # [-1,)
  // }

  fprintf(fp,"%sIndexedFaceSet {\n",str);

  IndexedFaceSet& ifs = *indexedFaceSet;

  bool&          ccw             = ifs.getCcw();
  bool&          convex          = ifs.getConvex();
  float&         creaseAngle     = ifs.getCreaseangle();
  bool&          solid           = ifs.getSolid();
  bool&          normalPerVertex = ifs.getNormalPerVertex();
  bool&          colorPerVertex  = ifs.getColorPerVertex();
  vector<float>& coord           = ifs.getCoord();
  vector<int>&   coordIndex      = ifs.getCoordIndex();
  vector<float>& normal          = ifs.getNormal();
  vector<int>&   normalIndex     = ifs.getNormalIndex();
  vector<float>& color           = ifs.getColor();
  vector<int>&   colorIndex      = ifs.getColorIndex();
  vector<float>& texCoord        = ifs.getTexCoord();
  vector<int>&   texCoordIndex   = ifs.getTexCoordIndex();


  // default ccw TRUE
  if(ccw   ==false)   fprintf(fp,"%s ccw FALSE\n",str);
  // default convex TRUE
  if(convex==false)   fprintf(fp,"%s convex FALSE\n",str);
  // default solid TRUE
  if(solid ==false)   fprintf(fp,"%s solid FALSE\n",str);
  // default creaseAngle 0.0
  if(creaseAngle>0.0) fprintf(fp,"%s creaseAngle %8.4f\n",str,creaseAngle);

  {
    fprintf(fp,"%s coordIndex [\n",str);
    for(unsigned i=0;i<coordIndex.size();i++) {
      fprintf(fp,"%s%6d ",str,coordIndex[i]);
      if(coordIndex[i]<0) fprintf(fp,"%s\n",str);
    }
    fprintf(fp,"%s ]\n",str);
  }

  // COORD_PER_VERTEX
  {
    fprintf(fp,"%s coord Coordinate {\n",str);
    fprintf(fp,"%s  point [\n",str);
    for(unsigned i=0;i<coord.size();i++) {
      fprintf(fp,"%s%8.4f ",str,coord[i]);
      if(i%3==2) fprintf(fp,"%s\n",str);
    }
    fprintf(fp,"%s  ]\n",str);
    fprintf(fp,"%s }\n",str);
  }

  // if(normal.size()==0)
  //   NO_NORMALS
  // else if(normalPerVertex==FALSE)
  //   if(normalIndex.size()>0)
  //     NORMAL_PER_FACE_INDEXED;
  //     normalIndex.size()==getNumberOfFaces()
  //   else
  //     NORMAL_PER_FACE;
  //     normal.size()/3==getNumberOfFaces()
  // else // if(normalPerVertex==TRUE)
  //   if(normalIndex.size()>0)
  //     NORMAL_PER_CORNER_INDEXED;
  //     normalIndex.size()==coordIndex.size()
  //   else
  //     NORMAL_PER_VERTEX;
  //     normal.size()/3==coord.size()/3

  if(normal.size()>0) {
    unsigned i;
    fprintf(fp,"%s normalPerVertex %s\n",str,
            (normalPerVertex==true)?"TRUE":"FALSE");

    fprintf(fp,"%s normal Normal {\n",str);
    fprintf(fp,"%s  vector [\n",str);
    for(i=0;i<normal.size();i++) {
      fprintf(fp,"%s%8.4f ",str,normal[i]);
      if(i%3==2) fprintf(fp,"%s\n",str);
    }
    fprintf(fp,"%s  ]\n",str);
    fprintf(fp,"%s }\n",str);

    if(normalIndex.size()>0) {
      fprintf(fp,"%s normalIndex [\n",str);
      for(i=0;i<normalIndex.size();i++) {
        fprintf(fp,"%s%d ",str,normalIndex[i]);
        if(normalIndex[i]<0) fprintf(fp,"%s\n",str);
      }
      fprintf(fp,"%s ]\n",str);
    }
  }

  // if(color.size()==0)
  //   NO_COLORS
  // else if(colorPerVertex==FALSE)
  //   if(colorIndex.size()>0)
  //     COLOR_PER_FACE_INDEXED;
  //     colorIndex.size()==getNumberOfFaces()
  //   else
  //     COLOR_PER_FACE;
  //     color.size()/3==getNumberOfFaces()
  // else // if(colorPerVertex==TRUE)
  //   if(colorIndex.size()>0)
  //     COLOR_PER_CORNER_INDEXED;
  //     colorIndex.size()==coordIndex.size()
  //   else
  //     COLOR_PER_VERTEX;
  //     color.size()/3==coord.size()/3

  if(color.size()>0) {
    unsigned i;
    fprintf(fp,"%s colorPerVertex %s\n",str,
            (colorPerVertex==true)?"TRUE":"FALSE");

    fprintf(fp,"%s color Color {\n",str);
    fprintf(fp,"%s  color [\n",str);
    for(i=0;i<color.size();i++) {
      fprintf(fp,"%s%8.4f ",str,color[i]);
      if(i%3==2) fprintf(fp,"%s\n",str);
    }
    fprintf(fp,"%s  ]\n",str);
    fprintf(fp,"%s }\n",str);

    if(colorIndex.size()>0) {
      fprintf(fp,"%s colorIndex [\n",str);
      for(i=0;i<colorIndex.size();i++) {
        fprintf(fp,"%s%d ",str,colorIndex[i]);
        if(colorIndex[i]<0) fprintf(fp,"%s\n",str);
      }
      fprintf(fp,"%s ]\n",str);
    }
  }

  // if(texCoord.size()==0)
  //   NO_TEX_COORD
  // else if(texCoordIndex.size()>0)
  //   TEX_COORD_PER_CORNER_INDEXED;
  //   texCoordIndex.size()==coordIndex.size()
  // else
  //   TEX_COORD_PER_VERTEX;
  //   texCoord.size()/2==coord.size()/3

  if(texCoord.size()>0) {
    unsigned i;

    fprintf(fp,"%s texCoord TextureCoordinate {\n",str);
    fprintf(fp,"%s  point [\n",str);
    for(i=0;i<texCoord.size();i++) {
      fprintf(fp,"%s%8.4f ",str,texCoord[i]);
      if(i%2==1) fprintf(fp,"%s\n",str);
    }
    fprintf(fp,"%s  ]\n",str);
    fprintf(fp,"%s }\n",str);

    if(texCoordIndex.size()>0) {
      fprintf(fp,"%s texCoordIndex [\n",str);
      for(i=0;i<texCoordIndex.size();i++) {
        fprintf(fp,"%s%d ",str,texCoordIndex[i]);
        if(texCoordIndex[i]<0) fprintf(fp,"%s\n",str);
      }
      fprintf(fp,"%s ]\n",str);
    }
  }

  fprintf(fp,"%s}\n",str); // IndexedFaceSet
}

/////////////////////////////////////////////////////////////////
void SaverWrl::saveIndexedLineSet
(FILE* fp, string indent, IndexedLineSet* indexedLineSet) const {
    if(indexedLineSet==(IndexedLineSet*)0) return;
    
    // DEF NAME IndexedLineSet {
    //   SFNode  coord             NULL
    //   MFInt32 coordIndex        []        # [-1,)
    //   SFNode  color             NULL
    //   MFInt32 colorIndex        []        # [-1,)
    //   SFBool  colorPerVertex    TRUE
    // }
    
    // or
    
    // IndexedLineSet {
    //   SFNode  coord             NULL
    //   MFInt32 coordIndex        []        # [-1,)
    //   SFNode  color             NULL
    //   MFInt32 colorIndex        []        # [-1,)
    //   SFBool  colorPerVertex    TRUE
    // }
    
    // TODO
    const char* str = indent.c_str();

    
    fprintf(fp,"%sIndexedLineSet {\n",str);
    
    IndexedLineSet& ils = *indexedLineSet;

    bool&          colorPerVertex  = ils.getColorPerVertex();
    vector<float>& coord           = ils.getCoord();
    vector<int>&   coordIndex      = ils.getCoordIndex();
    vector<float>& color           = ils.getColor();
    vector<int>&   colorIndex      = ils.getColorIndex();

    

    {
        fprintf(fp,"%s coordIndex [\n",str);
        for(unsigned i=0;i<coordIndex.size();i++) {
            fprintf(fp,"%s%6d ",str,coordIndex[i]);
            if(coordIndex[i]<0) fprintf(fp,"%s\n",str);
        }
        fprintf(fp,"%s ]\n",str);
    }
    
    // COORD_PER_VERTEX
    {
        fprintf(fp,"%s coord Coordinate {\n",str);
        fprintf(fp,"%s  point [\n",str);
        for(unsigned i=0;i<coord.size();i++) {
            fprintf(fp,"%s%8.4f ",str,coord[i]);
            if(i%3==2) fprintf(fp,"%s\n",str);
        }
        fprintf(fp,"%s  ]\n",str);
        fprintf(fp,"%s }\n",str);
    }
   
    // if(color.size()==0)
    //   NO_COLORS
    // else if(colorPerVertex==FALSE)
    //   if(colorIndex.size()>0)
    //     COLOR_PER_FACE_INDEXED;
    //     colorIndex.size()==getNumberOfFaces()
    //   else
    //     COLOR_PER_FACE;
    //     color.size()/3==getNumberOfFaces()
    // else // if(colorPerVertex==TRUE)
    //   if(colorIndex.size()>0)
    //     COLOR_PER_CORNER_INDEXED;
    //     colorIndex.size()==coordIndex.size()
    //   else
    //     COLOR_PER_VERTEX;
    //     color.size()/3==coord.size()/3
    
    if(color.size()>0) {
        unsigned i;
        fprintf(fp,"%s colorPerVertex %s\n",str,
                (colorPerVertex==true)?"TRUE":"FALSE");
        
        fprintf(fp,"%s color Color {\n",str);
        fprintf(fp,"%s  color [\n",str);
        for(i=0;i<color.size();i++) {
            fprintf(fp,"%s%8.4f ",str,color[i]);
            if(i%3==2) fprintf(fp,"%s\n",str);
        }
        fprintf(fp,"%s  ]\n",str);
        fprintf(fp,"%s }\n",str);
        
        if(colorIndex.size()>0) {
            fprintf(fp,"%s colorIndex [\n",str);
            for(i=0;i<colorIndex.size();i++) {
                fprintf(fp,"%s%d ",str,colorIndex[i]);
                if(colorIndex[i]<0) fprintf(fp,"%s\n",str);
            }
            fprintf(fp,"%s ]\n",str);
        }
    }
    
    fprintf(fp,"%s}\n",str); // IndexedLineSet
}

//////////////////////////////////////////////////////////////////

void SaverWrl::saveShape
(FILE* fp, string indent, Shape* shape) {
  const char* str = indent.c_str();

  // Shape {
  //   SFNode appearance NULL
  //   SFNode geometry NULL
  // }

  Node* node;
  fprintf(fp,"%sShape {\n",str);
    //fprintf(fp,"%s appearance {\n",str); //added {
  node = shape->getAppearance();
  if(node!=(Node*)0) {
    Appearance* appearance = (Appearance*)node;
    saveAppearance(fp,indent+"  ", appearance);
  }
    //fprintf(fp, "%s}\n",str); //i added
    
  fprintf(fp,"%s geometry\n",str);
  node = shape->getGeometry();
  if(node!=(Node*)0) {
    if(node->isIndexedFaceSet()) {
      IndexedFaceSet* indexedFaceSet = (IndexedFaceSet*)node;
      saveIndexedFaceSet(fp,indent+"  ",indexedFaceSet);
    }
  }
  fprintf(fp,"%s}\n",str);
}

void SaverWrl::saveTransform
(FILE* fp, string indent, Transform* transform) {
  const char* str = indent.c_str();

  // Transform {
  //   SFVec3f    center            0 0 0
  //   SFRotation rotation          0 0 1 0
  //   SFVec3f    scale             1 1 1
  //   SFRotation scaleOrientation  0 0 1 0
  //   SFVec3f    translation       0 0 0
  //   SFVec3f    bboxCenter        0 0 0
  //   SFVec3f    bboxSize          -1 -1 -1
  //   MFNode     children          []
  // }

  // bboxCenter and bboxSize are not exposed
  // they should not be saved in teh VRML file

  fprintf(fp,"%sTransform {\n",str);

  Vec3f&    center           = transform->getCenter();
  if(center.x!=0.0f || center.y!=0.0f || center.z!= 0.0f)
    fprintf(fp,"%s center %8.4f %8.4f %8.4f\n",str,
            center.x,center.y,center.z);

  Rotation& rotation         = transform->getRotation();
  Vec3f&    axis             = rotation.getAxis();
  float     angle            = rotation.getAngle();
  if(axis.x!=0.0f || axis.y!=0.0f || axis.z!= 1.0f || angle!= 0.0f)
    fprintf(fp,"%s rotation %8.4f %8.4f %8.4f %8.4f\n",str,
            axis.x,axis.y,axis.z,angle);

  Vec3f&    scale            = transform->getScale();
  if(scale.x!=1.0f || scale.y!=1.0f || scale.z!= 1.0f)
    fprintf(fp,"%s scale %8.4f %8.4f %8.4f\n",str,
            scale.x,scale.y,scale.z);

  Rotation& scaleOrientation = transform->getScaleOrientation();
            axis             = scaleOrientation.getAxis();
            angle            = scaleOrientation.getAngle();
  if(axis.x!=0.0f || axis.y!=0.0f || axis.z!= 1.0f || angle!= 0.0f)
    fprintf(fp,"%s rotation %8.4f %8.4f %8.4f %8.4f\n",str,
            axis.x,axis.y,axis.z,angle);

  Vec3f&    translation      = transform->getTranslation();
  if(translation.x!=0.0f || translation.y!=0.0f || translation.z!= 0.0f)
    fprintf(fp,"%s translation %8.4f %8.4f %8.4f\n",str,
            translation.x,translation.y,translation.z);

  //   Vec3f&    bboxCenter       = transform->getBBoxCenter();
  //   if(bboxCenter.x!=0.0f || bboxCenter.y!=0.0f || bboxCenter.z!= 0.0f)
  //     fprintf(fp,"%s bboxCenter %8.4f %8.4f %8.4f\n",str,
  //             bboxCenter.x,bboxCenter.y,bboxCenter.z);
  //
  //   Vec3f&    bboxSize         = transform->getBBoxSize();
  //   if(bboxSize.x!=-1.0f || bboxSize.y!=-1.0f || bboxSize.z!= -1.0f)
  //     fprintf(fp,"%s bboxSize %8.4f %8.4f %8.4f\n",str,
  //             bboxSize.x,bboxSize.y,bboxSize.z);
  
  int nChildren = transform->getNumberOfChildren();
  if(nChildren>0) {
    Node* node;
    for(int i=0;i<nChildren;i++) {
      node = (*transform)[i];
      if(node->isShape()) {
        saveShape(fp,indent+" ",(Shape*)node);
	  } else if(node->isTransform()) {
        saveTransform(fp,indent+" ",(Transform*)node);
	  } else if(node->isGroup()) {
        saveGroup(fp,indent+" ",(Group*)node);
      } else {
        // throw StrException("unexpected node type as child of Transform");
      }
    }
  }

  fprintf(fp,"%s}\n",str);
}

void SaverWrl::saveGroup
(FILE* fp, string indent, Group* group) {
  const char* str = indent.c_str();

  // Group {
  //   SFVec3f bboxCenter  0 0 0
  //   SFVec3f bboxSize   -1 -1 -1
  //   MFNode children    []
  // }

  // bboxCenter and bboxSize are not exposed
  // they should not be saved in teh VRML file

  fprintf(fp,"%sGroup {\n",str);
    fprintf(fp,"%schildren [\n",str); // I added

//     Vec3f&    bboxCenter       = group->getBBoxCenter();
//     if(bboxCenter.x!=0.0f || bboxCenter.y!=0.0f || bboxCenter.z!= 0.0f)
//       fprintf(fp,"%s bboxCenter %8.4f %8.4f %8.4f\n",str,
//               bboxCenter.x,bboxCenter.y,bboxCenter.z);
//
//     Vec3f&    bboxSize         = group->getBBoxSize();
//     if(bboxSize.x!=-1.0f || bboxSize.y!=-1.0f || bboxSize.z!= -1.0f)
//       fprintf(fp,"%s bboxSize %8.4f %8.4f %8.4f\n",str,
//               bboxSize.x,bboxSize.y,bboxSize.z);

  int nChildren = group->getNumberOfChildren();
  if(nChildren>0) {
    Node* node;
    for(int i=0;i<nChildren;i++) {
      node = (*group)[i];
      if(node->isShape()) {
        saveShape(fp,indent+" ",(Shape*)node);
      } else if(node->isTransform()) {
        saveTransform(fp,indent+" ",(Transform*)node);
      } else if(node->isGroup()) {
        saveGroup(fp,indent+" ",(Group*)node);
      } else {
        // throw StrException("unexpected node type as child of Transform");
      }
    }
  }
    
    fprintf(fp,"%s]\n",str); //I added
  fprintf(fp,"%s}\n",str);
}

bool SaverWrl::save(const char* filename, SceneGraph& wrl) {
  bool success = false;
  if(filename!=(char*)0) {
    // TODO Wed Oct 24 00:34:03 2012
    // check that filename extension == ext()
    FILE* fp = fopen(filename,"w");
    if(	fp!=(FILE*)0) {

      fprintf(fp,"#VRML V2.0 utf8\n");
      string indent="";

      int nChildren = wrl.getNumberOfChildren();
      for(int i=0;i<nChildren;i++) {
        Node* node = wrl[i];

        if(node->isShape()) {
          Shape* shape = (Shape*)node;
          saveShape(fp,indent,shape);
        } else if(node->isTransform()) {
          Transform* transform = (Transform*)node;
          saveTransform(fp,indent,transform);
        } else if(node->isGroup()) {
          Group* group = (Group*)node;
          saveGroup(fp,indent,group);
        }

        // alternative implementation using dynamic_cast
        //
        // if(Shape* shape = dynamic_cast<Shape*>(node)) {
        //   saveShape(fp,indent,shape);
        // } else if(Transform* transform = dynamic_cast<Transform*>(node)) {
        //   saveTransform(fp,indent,transform);
        // } else if(Group* group = dynamic_cast<Group*>(node)) {
        //   saveGroup(fp,indent,group);
        // }

      }

      fclose(fp);
      success = true;
    }
  }
  return success;
}
