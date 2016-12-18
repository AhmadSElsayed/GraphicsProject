//
// Created by root on 12/17/16.
//

#ifndef LOADER_ENVIRONMENT_H
#define LOADER_ENVIRONMENT_H


#include "Model.h"

class Environment : public Model
{
public:
    Environment(GLchar *path);
    static int ID;
    static void Texturize(const char* path, string directory);
    virtual vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};


#endif //LOADER_ENVIRONMENT_H
