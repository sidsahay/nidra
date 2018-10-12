#ifndef NIDRA_CODE_SHADER_HPP
#define NIDRA_CODE_SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif //NIDRA_CODE_SHADER_HPP
