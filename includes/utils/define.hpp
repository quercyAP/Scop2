#ifndef DEFINE_HPP
#define DEFINE_HPP

#define CYAN "\033[0;36m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define CLEAR "\033[0m"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include "Vec3.hpp"
#include "Mat4.hpp"

struct Vertex {
    float x, y, z;

    Vec3 toVec3() const {
        return Vec3(x, y, z);
    }
};

using namespace std;

#endif