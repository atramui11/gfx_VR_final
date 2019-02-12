
#include <stdio.h>
#include <iostream>
#include <fstream>

// TODO : use your own
// #include "Tokenizer.hpp"

#include "LoaderWrl.hpp"
#include "StrException.hpp"
#include "Tokenizer.hpp"

#define VRML_HEADER "#VRML V2.0 utf8"

const char* LoaderWrl::_ext = "wrl";

bool LoaderWrl::load(const char* filename, SceneGraph& wrl) {
  bool success = false;

  try {

    // TODO : open the file
      ifstream ifstr(filename);
      if (!ifstr.is_open()) {throw 0;}
      cout <<"loading, file is open: " << ifstr.is_open() << endl;
      
      char cstr[512];
      ifstr.getline(cstr,512);
      string str = cstr;
      
      string mystr=VRML_HEADER;
      
    // TODO : clear the SceneGraph
    wrl.clear();
    wrl.setUrl(filename);

    // TODO : read and check header line
    if(strstr(str.c_str(), mystr.c_str())==NULL) {throw string("'VRML_HEADER' not in file");}
      
    // TODO : create a Tokenizer and start parsing
      Tokenizer tokenizer(ifstr);
    loadSceneGraph(tokenizer, wrl);

    // TODO : recursively compute bounding boxes
    wrl.updateBBox();
    
    // TODO : close the input file
      ifstr.close();
      
    // if we have reached this point we have succeeded

    success = true;

  } catch(StrException* e) { 

    // TODO : clean up

    delete e;
    wrl.clear();
    wrl.setUrl("");

  }

  return success;
}

bool LoaderWrl::loadSceneGraph(Tokenizer& tkn, SceneGraph& wrl) {

  bool success = false;
  string currentName="";
    
  while(tkn.get()) {
    if(tkn=="Group") {
      Group* g = new Group();
        if (currentName!="") {
            g->setName(currentName);
            currentName="";
        }
      wrl.addChild(g);
        
        if(tkn.expecting("{")==false)
            throw new StrException("expecting \"{\"");
       
        /*if(tkn.expecting("children")==false) {throw StrException("expecting 'children'");}*/
        
      loadGroup(tkn, *g);
    } else if (tkn=="DEF") {
        tkn.get();
        currentName=tkn;
    } else if(tkn=="Transform") {
      Transform* t = new Transform();
        if (currentName!="") {
            t->setName(currentName);
            currentName="";
        }
      wrl.addChild(t);
      loadTransform(tkn, *t);
    } else if(tkn=="Shape") {
      Shape* s = new Shape();
      if (currentName!="") {
          s->setName(currentName);
          currentName="";
      }
      wrl.addChild(s);
      loadShape(tkn, *s);
        std::cout << s->getName() << std::endl;
    } else if(tkn=="") {
      break;
    } else {
      fprintf(stderr,"tkn=\"%s\"\n",tkn.c_str());
      throw new StrException("unexpected token while parsing Group");
    }
  }
  success = true;

  return success;
}

bool LoaderWrl::loadGroup(Tokenizer& tkn,  Group& group) {

  bool success = false;

  // Group {
  //   MFNode children    []
  //   SFVec3f bboxCenter  0 0 0
  //   SFVec3f bboxSize   -1 -1 -1
  // }

  // bboxCenter and bboxSize are not exposed
  // they should be computed on the fly after all the children are loaded

    //if(tkn.expecting("{")==false)
        //throw new StrException("expecting \"{\"");
    
    /*if(tkn.expecting("children")==false) {throw new StrException("expecting 'children'");}
    
    if(tkn.expecting("[")==false) {throw new StrException("expecting \"[\"");}*/
    
  while(success==false && tkn.get()) {
      // TODO ...
      
      //if (tkn=="{" || tkn =="[") {//pass as usual, opening brackets.}
      
        if (tkn=="children" ||tkn=="[" ) {
            continue;
        }
      
        if (tkn=="Shape") {
            Shape* s = new Shape();
            group.addChild(s);
            loadShape(tkn, *s);
        }
        else if(tkn=="Transform") {
            Transform* t = new Transform();
            group.addChild(t);
            loadTransform(tkn, *t);
        }
      
        //closing bracket for children...
        else if(tkn=="]") {
            
        }
      
        else if(tkn=="}") {
            success = true;
        }
      
        else {
            throw StrException("unexpected token while parsing Group");
        }
  }
    
  return success;
}

bool LoaderWrl::loadTransform(Tokenizer& tkn,  Transform& transform) {

  bool success = false;

  // Transform {
  //   MFNode     children          []
  //   SFVec3f    bboxCenter        0 0 0
  //   SFVec3f    bboxSize          -1 -1 -1
  //   SFVec3f    center            0 0 0
  //   SFRotation rotation          0 0 1 0
  //   SFVec3f    scale             1 1 1
  //   SFRotation scaleOrientation  0 0 1 0
  //   SFVec3f    translation       0 0 0
  // }

  // bboxCenter and bboxSize are not exposed
  // they should be computed on the fly
  // after all the children are loaded

  if(tkn.expecting("{")==false)
    throw StrException("expecting \"{\"");
  while(success==false && tkn.get()) {
      if (tkn=="Group" || tkn=="{") {
          continue;
      }
      
    if(tkn=="children") {
      // TODO ...
        Group* g = new Group();
        transform.addChild(g);
        loadGroup(tkn, *g);

    } else if(tkn=="center") {
      // TODO ...
        Vec3f myVec;
        
        tkn.get();
        string temp=tkn;
        myVec.x=stof(temp);
        
        tkn.get();
        string temp1=tkn;
        myVec.y=stof(temp1);
        
        tkn.get();
        string temp2=tkn;
        myVec.z=stof(temp2);
        
        transform.setCenter(myVec);

    } else if(tkn=="rotation") {
      // TODO ...
        Vec4f myVec;
        
        tkn.get();
        string temp=tkn;
        myVec.x=stof(temp);
        
        tkn.get();
        string temp1=tkn;
        myVec.y=stof(temp1);
        
        tkn.get();
        string temp2=tkn;
        myVec.z=stof(temp2);
        
        tkn.get();
        string temp3=tkn;
        myVec.w=stof(temp3);
        
        transform.setRotation(myVec);

    } else if(tkn=="scale") {
      // TODO ...
        Vec3f myVec;
        
        tkn.get();
        string temp=tkn;
        myVec.x=stof(temp);
        
        tkn.get();
        string temp1=tkn;
        myVec.y=stof(temp1);
        
        tkn.get();
        string temp2=tkn;
        myVec.z=stof(temp2);
        
        
        transform.setScale(myVec);


    } else if(tkn=="scaleOrientation") {
      // TODO ...
        Vec4f myVec;
        
        tkn.get();
        string temp=tkn;
        myVec.x=stof(temp);
        
        tkn.get();
        string temp1=tkn;
        myVec.y=stof(temp1);
        
        tkn.get();
        string temp2=tkn;
        myVec.z=stof(temp2);
        
        tkn.get();
        string temp3=tkn;
        myVec.w=stof(temp3);
        
        transform.setScaleOrientation(myVec);

    } else if(tkn=="translation") {
      // TODO ...
        Vec3f myVec;
        
        tkn.get();
        string temp=tkn;
        myVec.x=stof(temp);
        
        tkn.get();
        string temp1=tkn;
        myVec.y=stof(temp1);
        
        tkn.get();
        string temp2=tkn;
        myVec.z=stof(temp2);
        
        transform.setTranslation(myVec);

    } else if(tkn=="]") {
        //do nothing, closing children
    } else if(tkn=="}") {
      success = true;
    } else {
      throw StrException("unexpected token while parsing Group");
    }
  }
  return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadChildren(Tokenizer& tkn, Group& group) {
    bool   success = false;
    // TODO
    return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadIndexedLineSet(Tokenizer& tkn, IndexedLineSet& ils) {
    
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
    
    bool success = false;
    
    
    
    // TODO
    //if (tkn.expecting("IndexedLineSet")==false) {throw StrException("expecting 'IndexedLineSet'");}
    
    if(tkn.expecting("{")==false) {throw StrException("expecting \"{\"");}
    
    bool temp=true;
    
    try {
        while (tkn.get()) {
            if(tkn=="coord") {
                temp=tkn.expecting("Coordinate");
                temp=tkn.expecting("{");
                temp=tkn.expecting("point");
                temp=tkn.expecting("[");
                
                vector<float>& _coordVector=ils.getCoord();
                
                while(tkn.get() && tkn!="]") {
                    // ... parse float value from tokenizer and save value in Ifs._coord // ... throw exception if unable to
                    try {
                        float currentFloat=stof(tkn);
                        _coordVector.push_back(currentFloat);
                    }
                    
                    catch(const std::invalid_argument& ia) {ils.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
                }
                bool a=tkn.expecting("}");
                
                if (a==false) {
                    ils.clear();
                    cout << "expecting '}'" << endl;
                    return 0;
                }
            }
            
            else if(tkn=="coordIndex") {
                temp=tkn.expecting("[");
                vector<int>& _coordIndexVector = ils.getCoordIndex();
                
                while(tkn.get() && tkn!="]") {
                    // ... parse int value from tokenizer and save value in Ifs._coordIndex // ... throw exception if unable to
                    try {
                        int currentInt=stoi(tkn);
                        _coordIndexVector.push_back(currentInt);
                    }
                    
                    catch(const std::invalid_argument& ia) {ils.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
                    
                }
                
            }
            
            // parse other IndexedFaceSet fields here, such as color, normal, ccw, etc ...

            
            else if(tkn=="colorPerVertex") {
                tkn.get();
                if (tkn=="TRUE") {
                    bool & holder=ils.getColorPerVertex();
                    holder=true;
                }
                
                else if (tkn=="FALSE") {
                    bool & holder=ils.getColorPerVertex();
                    holder=false;
                }
                
                else {throw 6;}
            }
            
            else if(tkn=="colorIndex") {
                temp=tkn.expecting("[");
                vector<int>& colorIndex = ils.getColorIndex();
                
                while(tkn.get() && tkn!="]") {
                    try {
                        int currentInt=stoi(tkn);
                        colorIndex.push_back(currentInt);
                    }
                    
                    catch(const std::invalid_argument& ia) {ils.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
                }
                
            }
            
            
            else if(tkn=="color") {
                temp=tkn.expecting("Color");
                temp=tkn.expecting("{");
                temp=tkn.expecting("color");
                temp=tkn.expecting("[");
                
                vector<float>& _colorVector=ils.getColor();
                
                while(tkn.get() && tkn!="]") {
                    // ... parse float value from tokenizer and save value in Ils._coord // ... throw exception if unable to
                    try {
                        float currentFloat=stof(tkn);
                        _colorVector.push_back(currentFloat);
                    }
                    
                    catch(const std::invalid_argument& ia) {ils.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
                }
                
                temp=tkn.expecting("}");
            }

            else if(tkn=="}") {// matches “{“ found after “IndexedFaceSet”
                success=true;
                break;
            }
            
            else {/*syntax ERROR*/ throw 25;}
            
        }//close brace for while loop
        
    }
    
    catch(int n) {
        if (n==0) {cout << "file could not be opened"<<endl; exit(0);}
        if (n==1) {cout << "'IndexedLineSet' not found in file" << endl; }
        if (n==6) {ils.clear(); return 0; cout << "incorrect or missing value for colorPerVertex" << endl;}
        
        if (n==25) {ils.clear(); return 0; cout << "found unexpected token while parsing IndexedFaceSet" << endl;}
    }
    
    if (temp==false)  {
        cout << "an expecting() call returned false" << endl;
        ils.clear(); return 0;
    }
    
    
    return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadVecFloat(Tokenizer&tkn,vector<float>& vec) {
    
    bool success = false;
    
    // TODO
    
    return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadVecInt(Tokenizer&tkn,vector<int>& vec) {
    
    bool success = false;
    
    // TODO
    
    return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadVecString(Tokenizer&tkn,vector<string>& vec) {
    
    bool success = false;
    
    // TODO
    
    return success;
}


bool LoaderWrl::loadShape(Tokenizer& tkn,  Shape& shape) {
  // Shape {
  //   SFNode appearance NULL
  //   SFNode geometry NULL
  // }

    bool success = false;
    // TODO ...
    
    if(tkn.expecting("{")==false)
        throw StrException("expecting \"{\"");
    
    while(success==false && tkn.get()) {
        if (tkn=="appearance") {
            Appearance* t = new Appearance();
            shape.setAppearance(t);
            loadAppearance(tkn, *t);
        }
        
        else if(tkn=="geometry") {
            tkn.get();
            
            if (tkn=="IndexedFaceSet") {
                IndexedFaceSet* ifs = new IndexedFaceSet();
                shape.setGeometry(ifs);
                loadIndexedFaceSet(tkn, *ifs);
            }
            
            if (tkn=="IndexedLineSet") {
                IndexedLineSet* ils = new IndexedLineSet();
                shape.setGeometry(ils);
                loadIndexedLineSet(tkn, *ils);
            }

        }
        
        else if(tkn=="}") {
            success=true;
        }
        
        else {
            throw StrException("unexpected token while parsing Group");
        }
        
    }
    
  return success;
}


bool LoaderWrl::loadAppearance(Tokenizer& tkn, Appearance& appearance) {

  // Appearance {
  //   SFNode material NULL
  //   SFNode texture NULL
  //   // SFNode textureTransform NULL
  // }

    // TODO ...
    bool success = false;

    //if (tkn!="Appearance") {return 0;}
    
    if (tkn.expecting("Appearance")==false) {
        throw StrException("expecting 'Appearance' ");
    }
    
    if (tkn.expecting("{")==false) {
        throw StrException("expecting '{'");
    }
     
    
    while (success==false && tkn.get()) {
        if (tkn=="material") {
            Material* m = new Material();
            appearance.setMaterial(m);
            loadMaterial(tkn, *m);
        }
        
        else if(tkn=="texture") {
            ImageTexture* imt= new ImageTexture();
            appearance.setTexture(imt);
            loadImageTexture(tkn, *imt);
        }
        
        
        else if(tkn=="}") {
            success = true;
        }
        
        else {
            throw new StrException("unexpected token while parsing Group");
        }
        
    }

  return success;
}

bool LoaderWrl::loadMaterial(Tokenizer& tkn, Material& material) {

  // Material {
  //   SFFloat ambientIntensity 0.2
  //   SFColor diffuseColor     0.8 0.8 0.8
  //   SFColor emissiveColor    0 0 0
  //   SFFloat shininess        0.2
  //   SFColor specularColor    0 0 0
  //   SFFloat transparency     0
  // }

  bool success = false;
  // TODO ...
    if (tkn.expecting("Material")==false) {
        throw StrException("expecting 'Material' ");
    }
    
    if (tkn.expecting("{")==false) {
        throw StrException("expecting '{'");
    }
    
    while (success==false && tkn.get()) {
        if(tkn=="ambientIntensity") {
            tkn.get();
            string temp= tkn;
            material.setAmbientIntensity(stof(temp));
        }
        
        else if(tkn=="diffuseColor") {
            Color c= Color();
            
            tkn.get();
            string temp= tkn;
            c.r=stof(temp);
            
            tkn.get();
            string temp1= tkn;
            c.g=stof(temp1);
            
            tkn.get();
            string temp2= tkn;
            c.b=stof(temp2);
            
            material.setDiffuseColor(c);

        }
        
        else if(tkn=="emissiveColor") {
            Color c= Color();
            
            tkn.get();
            string temp= tkn;
            c.r=stof(temp);
            
            tkn.get();
            string temp1= tkn;
            c.g=stof(temp1);
            
            tkn.get();
            string temp2= tkn;
            c.b=stof(temp2);
            
            material.setEmissiveColor(c);
        }
        
        else if(tkn=="shininess") {
            tkn.get();
            string temp= tkn;
            material.setShininess(stof(temp));
        }
        
        else if(tkn=="specularColor") {
            Color c= Color();
            
            tkn.get();
            string temp= tkn;
            c.r=stof(temp);
            
            tkn.get();
            string temp1= tkn;
            c.g=stof(temp1);
            
            tkn.get();
            string temp2= tkn;
            c.b=stof(temp2);
            
            material.setSpecularColor(c);
        }
        
        else if(tkn=="transparency") {
            tkn.get();
            string temp=tkn;
            material.setTransparency(stof(temp));
        }
        
        
        
        else if(tkn=="}") {
            success = true;
        }
        
        else {
            throw StrException("unexpected token while parsing Group");
        }
    }
    
    
    

  return success;
}

bool LoaderWrl::loadImageTexture(Tokenizer& tkn, ImageTexture& imageTexture) {

  // ImageTexture {
  //   MFString url []
  //   SFBool repeatS TRUE
  //   SFBool repeatT TRUE
  // }
    
    // TODO ...
    bool success = false;
    
    if (tkn.expecting("ImageTexture")==false) {
        throw StrException("expecting 'ImageTexture' ");
    }
    
    if (tkn.expecting("{")==false) {
        throw StrException("expecting '{'");
    }
    
    while (success==false && tkn.get()) {
        if (tkn=="url") {
            tkn.get();
            string temp=tkn;
            imageTexture.adToUrl(temp);
        }
        
        else if(tkn=="repeatS") {
            tkn.get();
            
            if (tkn=="TRUE") {imageTexture.setRepeatS(true);}
            
            if (tkn=="FALSE") {imageTexture.setRepeatS(false);}
            
        }
        
        else if(tkn=="repeatT") {
            tkn.get();
            
            if (tkn=="TRUE") {imageTexture.setRepeatT(true);}
            
            if (tkn=="FALSE"){imageTexture.setRepeatT(false);}
        }

        
        else if(tkn=="}") {
            success = true;
        }
        
        else {
            throw new StrException("unexpected token while parsing Group");
        }
        
    }
    
  return success;
}

bool LoaderWrl::loadIndexedFaceSet(Tokenizer& tkn,  IndexedFaceSet& ifs) {

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

    bool success = false;
  // TODO ...
    
    //if (tkn.expecting("IndexedFaceSet")==false) {throw StrException("expecting 'IndexedFaceSet'");}
    
    if(tkn.expecting("{")==false) {throw StrException("expecting \"{\"");}
    
    bool temp=true;
    
    try {
    while (tkn.get()) {
        if(tkn=="coord") {
            temp=tkn.expecting("Coordinate");
            temp=tkn.expecting("{");
            temp=tkn.expecting("point");
            temp=tkn.expecting("[");
            
            vector<float>& _coordVector=ifs.getCoord();
            
            while(tkn.get() && tkn!="]") {
                // ... parse float value from tokenizer and save value in Ifs._coord // ... throw exception if unable to
                try {
                    float currentFloat=stof(tkn);
                    _coordVector.push_back(currentFloat);
                }
                
                catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
            }
            bool a=tkn.expecting("}");
            
            if (a==false) {
                ifs.clear();
                cout << "expecting '}'" << endl;
                return 0;
            }
        }
        
        else if(tkn=="coordIndex") {
            temp=tkn.expecting("[");
            vector<int>& _coordIndexVector = ifs.getCoordIndex();
            
            while(tkn.get() && tkn!="]") {
                // ... parse int value from tokenizer and save value in Ifs._coordIndex // ... throw exception if unable to
                try {
                    int currentInt=stoi(tkn);
                    _coordIndexVector.push_back(currentInt);
                }
                
                catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
                
            }
            
        }
        
        // parse other IndexedFaceSet fields here, such as color, normal, ccw, etc ...
        else if(tkn=="ccw") {
            tkn.get();
            if (tkn=="TRUE") {
                bool & holder=ifs.getCcw();
                holder=true;
            }
            
            else if (tkn=="FALSE") {
                bool & holder=ifs.getCcw();
                holder=false;
            }
            
            else {throw 2;}
        }
        
        else if(tkn=="convex") {
            tkn.get();
            if (tkn=="TRUE") {
                bool & holder=ifs.getConvex();
                holder=true;
            }
            
            else if (tkn=="FALSE") {
                bool & holder=ifs.getConvex();
                holder=false;
            }
            
            else {throw 3;}
        }
        
        else if(tkn=="creaseAngle") {
            tkn.get();
            try {
                int currentInt=stoi(tkn);
                float& holder= ifs.getCreaseangle();
                holder=currentInt;
            }
            catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
        }
        
        else if(tkn=="solid") {
            tkn.get();
            if (tkn=="TRUE") {
                bool & holder=ifs.getSolid();
                holder=true;
            }
            
            else if (tkn=="FALSE") {
                bool & holder=ifs.getSolid();
                holder=false;
            }
            
            else {throw 4;}
        }
        
        else if(tkn=="normalPerVertex") {
            tkn.get();
            if (tkn=="TRUE") {
                bool & holder=ifs.getNormalPerVertex();
                holder=true;
            }
            
            else if (tkn=="FALSE") {
                bool & holder=ifs.getNormalPerVertex();
                holder=false;
            }
            
            else {throw 5;}
        }
        
        else if(tkn=="colorPerVertex") {
            tkn.get();
            if (tkn=="TRUE") {
                bool & holder=ifs.getColorPerVertex();
                holder=true;
            }
            
            else if (tkn=="FALSE") {
                bool & holder=ifs.getColorPerVertex();
                holder=false;
            }
            
            else {throw 6;}
        }
        
        else if(tkn=="colorIndex") {
            temp=tkn.expecting("[");
            vector<int>& colorIndex = ifs.getColorIndex();
            
            while(tkn.get() && tkn!="]") {
                try {
                    int currentInt=stoi(tkn);
                    colorIndex.push_back(currentInt);
                }
                
                catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
            }
            
        }
        
        else if(tkn=="normal") {
            temp=tkn.expecting("Normal");
            temp=tkn.expecting("{");
            temp=tkn.expecting("vector");
            temp=tkn.expecting("[");
            
            vector<float>& normal=ifs.getNormal();
            
            while(tkn.get() && tkn!="]") {
                // ... parse float value from tokenizer and save value in Ifs._coord // ... throw exception if unable to
                try {float currentFloat=stof(tkn); normal.push_back(currentFloat);}
                
                catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
            }
            
            temp=tkn.expecting("}");
        }
        
        else if(tkn=="color") {
            temp=tkn.expecting("Color");
            temp=tkn.expecting("{");
            temp=tkn.expecting("color");
            temp=tkn.expecting("[");
            
            vector<float>& _colorVector=ifs.getColor();
            
            while(tkn.get() && tkn!="]") {
                // ... parse float value from tokenizer and save value in Ifs._coord // ... throw exception if unable to
                try {
                    float currentFloat=stof(tkn);
                    _colorVector.push_back(currentFloat);
                }
                
                catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
            }
            
            temp=tkn.expecting("}");
        }
        
        
        else if(tkn=="normalIndex") {
            temp=tkn.expecting("[");
            vector<int>& normalIndex=ifs.getNormalIndex();
            
            while(tkn.get() && tkn!="]") {
                try {
                    int currentInt=stof(tkn);
                    normalIndex.push_back(currentInt);
                }
                
                catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
            }
            
        }
        
        
        else if(tkn=="texCoord") {
            temp=tkn.expecting("TextureCoordinate");
            temp=tkn.expecting("{");
            temp=tkn.expecting("point");
            temp=tkn.expecting("[");
            
            vector<float>& texCoord=ifs.getTexCoord();
            
            while(tkn.get() && tkn!="]") {
                try {
                    float currentFloat=stof(tkn);
                    texCoord.push_back(currentFloat);
                }
                
                catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
            }
            
            temp=tkn.expecting("}");
        }
        
        
        else if(tkn=="texCoordIndex") {
            temp=tkn.expecting("[");
            vector<int>& texCoordIndex = ifs.getTexCoordIndex();
            
            while(tkn.get() && tkn!="]") {
                try {
                    int currentInt=stoi(tkn);
                    texCoordIndex.push_back(currentInt);
                }
                
                catch(const std::invalid_argument& ia) {ifs.clear(); return 0; std::cerr<< "Invalid argument: " << ia.what() << endl;}
            }
            
        }
        
        else if(tkn=="}") {// matches “{“ found after “IndexedFaceSet”
            success=true;
            break;
        }
        
        else {/*syntax ERROR*/ throw 25;}
        
    }//close brace for while loop

}

catch(int n) {
    if (n==0) {cout << "file could not be opened"<<endl; exit(0);}
        if (n==1) {cout << "'IndexedFaceSet' not found in file" << endl; }
        if (n==2) {ifs.clear(); return 0; cout << "incorrect or missing value for ccw" << endl;}
        if (n==3) {ifs.clear(); return 0; cout << "incorrect or missing value for convex" << endl;}
        if (n==4) {ifs.clear(); return 0; cout << "incorrect or missing value for solid" << endl;}
        if (n==5) {ifs.clear(); return 0; cout << "incorrect or missing value for normalPerVertex" << endl;}
        if (n==6) {ifs.clear(); return 0; cout << "incorrect or missing value for colorPerVertex" << endl;}
    
        if (n==25) {ifs.clear(); return 0; cout << "found unexpected token while parsing IndexedFaceSet" << endl;}
    }
        
        if (temp==false)  {
            cout << "an expecting() call returned false" << endl;
            ifs.clear(); return 0;
        }
        
    
        return success;
}
