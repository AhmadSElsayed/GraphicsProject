//
// Created by root on 12/14/16.
//

#include<iostream>
using namespace std;
#include "GraphicsObject.h"
#include "Environment.h"

GraphicsObject::GraphicsObject(GLchar* path, bool vis, bool env)
{
    if(!env)
        model = new Model(path);
    else
    {
        if(!done)
        {
            Environment::Texturize("gray_floor_texture-wallpaper-1366x768.jpg", "Platform");
            done = true;
        }
        model = new Environment(path);
    }
    visible = vis;
    transformation = glm::scale(transformation, glm::vec3(0.2f,0.2f,0.2f));
}

void GraphicsObject::graph(Shader shader, glm::mat4 proj, glm::mat4 view)
{
    if(!visible)
        return;
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
    std::cout << "x: " << trans.getOrigin().getX()<<"\ty:"<<trans.getOrigin().getY()<<"\tz:"<<trans.getOrigin().getZ()<< std::endl;
}

Model *GraphicsObject::getModel() {
    return model;
}

