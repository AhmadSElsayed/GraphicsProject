//
// Created by root on 12/14/16.
//

#include<iostream>
using namespace std;
#include "GraphicsObject.h"

GraphicsObject::GraphicsObject(GLchar* path, bool vis)
{
    model = new Model(path);
    visible = vis;
    transformation = glm::scale(transformation, glm::vec3(0.2f,0.2f,0.2f));
}

void GraphicsObject::graph(Shader shader, glm::mat4 proj, glm::mat4 view)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"),1, GL_FALSE, glm::value_ptr(transformation));
    model->Draw(shader);
}

void GraphicsObject::hide()
{

}

GraphicsObject::~GraphicsObject()
{
    delete model;
}

bool GraphicsObject::getVisible()
{
    return visible;
}

void GraphicsObject::setVisible(bool x)
{
    visible = x;
}
//
void GraphicsObject::Transform(btTransform trans)
{
    trans.getOpenGLMatrix(glm::value_ptr(transformation));
    std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
}

Model *GraphicsObject::getModel() {
    return model;
}

