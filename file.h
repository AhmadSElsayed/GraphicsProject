#pragma once
#include <dirent.h>
#include<vector>
#include<string.h>
#include <iostream>
using namespace std;
std::vector<string> getFiles(char* path)
{
    std::vector<string> names;

    DIR* dirFile = opendir( path );
    if ( dirFile )
    {
        struct dirent* hFile;
        errno = 0;
        while (( hFile = readdir( dirFile )) != NULL )
        {
            if ( !strcmp( hFile->d_name, "."  )) continue;
            if ( !strcmp( hFile->d_name, ".." )) continue;

            // in linux hidden files all start with '.'
            if (( hFile->d_name[0] == '.' )) continue;

            // dirFile.name is the name of the file. Do whatever string comparison
            // you want here. Something like:
            if ( strstr( hFile->d_name, ".obj" ))
            {
                cout<<path <<hFile->d_name<< endl;
                names.push_back(string(path) + string(hFile->d_name));
            }
        }
        closedir( dirFile );
    }
return names;
}

