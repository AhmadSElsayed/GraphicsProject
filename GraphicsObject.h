//
// Created by root on 12/14/16.
//

#ifndef LOADER_GRAPHICSOBJECT_H
#define LOADER_GRAPHICSOBJECT_H


#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <LinearMath/btTransform.h>
#include "Model.h"

class GraphicsObject
{
private:
    bool visible;
    glm::mat4 transformation;
    Model* model;
public:
    GraphicsObject(GLchar* path, bool vis);
    void graph(Shader shader, glm::mat4 proj, glm::mat4 view);
    void hide();
    void setVisible(bool x);
    bool getVisible();
    void Transform(btTransform trans);
    Model* getModel();
    ~GraphicsObject();
};

#endif //LOADER_GRAPHICSOBJECT_H
