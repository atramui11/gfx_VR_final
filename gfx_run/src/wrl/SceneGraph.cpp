
#include <iostream>
#include "SceneGraph.hpp"
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"
#include "Appearance.hpp"

SceneGraph::SceneGraph() {
    _parent = this; // SceneGraph is always a tree root
}

SceneGraph::~SceneGraph() {
}

void SceneGraph::clear() {
    // TODO: delete all children and clear the children array
    vector<pNode> & temp = this->Group::getChildren();
    
    for (int i=0; i<temp.size(); i++) {delete temp.at(i);}
    temp.clear();
    
}

string& SceneGraph::getUrl() {
    return _url;
}

void SceneGraph::setUrl(const string& url) {
    _url = url;
}


// NOTES

// 1) a more efficient way to implement this function is to use a
// map<string,Node*> which we would fill using the Node::setName()
// method; but for small SceneGraphs, such as the ones we are
// operating on, doing a search each time we need to find a node by
// name is acceptable.

// 2) we have decided not to implementing the USE keyword yet; we
// would need smart pointers to do so, in such a way that the node
// would be deleted only when the number of references to it go back
// to zero.

Node* SceneGraph::find(const string& name) {
    // TODO
    
    // - using a SceneGraphTraversal, look for a node with the given
    // - name
    
    // - remember the the SceneGraphTraversal does not visit the nodes
    //   pointed to inside the Shape nodes
    
    // - for each Shape node, you need to check the names of
    //   1) the Appearance node (if any) pointed to by the appearance
    //   field of the Shape node
    //   2) the Material node (if any) pointed to by the material field
    //   of the Appearnce node
    //   3) the PixelTexture or ImageTexture node (if any) pointed to by
    //   the texture field of the Appearnce node
    //   4) the IndexedFaceSet or IndexedLineSe nodes (if any) pointed
    //   to by the geometry field of the Shape node
    
    Node* node = (Node*)0;
    SceneGraphTraversal t= SceneGraphTraversal(*this);
    Node* child;
    t.start();
    
    while ((child=t.next())!=NULL) {
        if (child->isShape()) {
            Shape* shape= (Shape*) child;
            
            Node* geom=shape->getGeometry();
            Node* appr=shape->getAppearance();
                
            //Node* mat=appr->
            string _gname;
            string _aname;
            string _sname=shape->getName();
            
            if (geom!=(Node*)0) {_gname=geom->getName();}
            else {_gname="";}
            
            
            if (appr!=(Node*)0) {_aname=appr->getName();}
            
            
            if (_sname==name) {node=shape;}
            if (_gname==name) {node=geom;}
            if (_aname==name) {node=appr;}
        }
    }

    return node;
}

void SceneGraph::printInfo(string indent) {
    std::cout << indent;
    if(_name!="") std::cout << "DEF " << _name << " ";
    std::cout << "SceneGraph {\n";
    std::cout << indent << "  " << "_url = \"" << _url << "\"\n";
    std::cout << indent << "  " << "children [\n";
    int nChildren = getNumberOfChildren();
    for(int i=0;i<nChildren;i++) {
        Node* node = (*this)[i];
        node->printInfo(indent+"    ");
    }
    std::cout << indent << "  " << "]\n";
    std::cout << indent << "}\n";
}
