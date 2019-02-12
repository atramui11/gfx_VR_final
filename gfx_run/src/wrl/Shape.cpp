

#include "Shape.hpp"
// TODO ...


Shape::Shape() {
    _appearance=(Node*) 0;
    _geometry=(Node*) 0;
}

Shape::~Shape() {
    delete _appearance;
    delete _geometry;
}


Node* Shape::getAppearance() {
    return _appearance;
}

Node* Shape::getGeometry() {
    return _geometry;
}

void Shape::setAppearance(Node* node) {
    _appearance=node;
}

void Shape::setGeometry(Node* node) {
    _geometry=node;
}






