#include "functions.h"

#include <boost/math/constants/constants.hpp>

#include <cmath>

/** this file contains function which are not dependent from any state */

constexpr bool inRange(const int value, const int min, const int max) {
    return value >= min && value < max;
}

bool insideCurrentSupercube(const Coordinate & coord, const Coordinate & center, const int & cubesPerDimension, const int & cubeSize) {
    const int halfSupercube = cubeSize * cubesPerDimension * 0.5;
    const int xcube = center.x - center.x % cubeSize + cubeSize / 2;
    const int ycube = center.y - center.y % cubeSize + cubeSize / 2;
    const int zcube = center.z - center.z % cubeSize + cubeSize / 2;
    bool valid = true;
    valid &= inRange(coord.x, xcube - halfSupercube, xcube + halfSupercube);
    valid &= inRange(coord.y, ycube - halfSupercube, ycube + halfSupercube);
    valid &= inRange(coord.z, zcube - halfSupercube, zcube + halfSupercube);
    return valid;
}

bool currentlyVisible(const Coordinate & coord, const Coordinate & center, const int & cubesPerDimension, const int & cubeSize) {
    const bool valid = insideCurrentSupercube(coord, center, cubesPerDimension, cubeSize);
    const int xmin = center.x - center.x % cubeSize;
    const int ymin = center.y - center.y % cubeSize;
    const int zmin = center.z - center.z % cubeSize;
    const bool xvalid = valid & inRange(coord.x, xmin, xmin + cubeSize);
    const bool yvalid = valid & inRange(coord.y, ymin, ymin + cubeSize);
    const bool zvalid = valid & inRange(coord.z, zmin, zmin + cubeSize);
    return xvalid || yvalid || zvalid;
}

int roundFloat(float number) {
    if(number >= 0) return (int)(number + 0.5);
    else return (int)(number - 0.5);
}

int sgn(float number) {
    if(number > 0.) return 1;
    else if(number == 0.) return 0;
    else return -1;
}


//Some math helper functions
float radToDeg(float rad) {
    return ((180. * rad) / boost::math::constants::pi<float>());
}

float degToRad(float deg) {
    return ((deg / 180.) * boost::math::constants::pi<float>());
}

void rotateAndNormalize(floatCoordinate &v, floatCoordinate axis, float angle) {
    // axis must be a normalized vector
    float matrix[3][3];
    const auto c = cosf(angle);
    const auto s = sinf(angle);
    matrix[0][0] = axis.x*axis.x*(1 - c) + c;
    matrix[0][1] = axis.x*axis.y*(1 - c) - axis.z*s;
    matrix[0][2] = axis.x*axis.z*(1 - c) + axis.y*s;

    matrix[1][0] = axis.y*axis.x*(1 - c) + axis.z*s;
    matrix[1][1] = axis.y*axis.y*(1 - c) + c;
    matrix[1][2] = axis.y*axis.z*(1 - c) - axis.x*s;

    matrix[2][0] = axis.x*axis.z*(1 - c) - axis.y*s;
    matrix[2][1] = axis.y*axis.z*(1 - c) + axis.x*s;
    matrix[2][2] = axis.z*axis.z*(1 - c) + c;

    const auto x = matrix[0][0]*v.x + matrix[0][1]*v.y + matrix[0][2]*v.z;
    const auto y = matrix[1][0]*v.x + matrix[1][1]*v.y + matrix[1][2]*v.z;
    const auto z = matrix[2][0]*v.x + matrix[2][1]*v.y + matrix[2][2]*v.z;
    v = {x, y, z};
    v.normalize();
}

bool intersectLineAndPlane(const floatCoordinate planeNormal, const floatCoordinate planeUpVec,
                           const floatCoordinate lineUpVec, const floatCoordinate lineDirectionVec,
                           floatCoordinate & intersectionPoint) {
    if (std::abs(lineDirectionVec.dot(planeNormal)) > 0.0001) {
        const float lambda = (planeNormal.dot(planeUpVec) - planeNormal.dot(lineUpVec)) / lineDirectionVec.dot(planeNormal);
        intersectionPoint = lineUpVec + lineDirectionVec * lambda;
        return true;
    }
    return false;
}

bool checkTreeParameter(int id, float r, float g, float b, float a) {
    if(id < 0 || r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1 || a < 0 || a > 1) {
        return false;
    }
    return true;
}

bool checkNodeParameter(int id, int x, int y, int z) {
    if(id < 0 || x < 0 || y < 0 || z < 0) {
        return false;
    }
    return true;
}

bool checkNodeParameter(int id) {
    if(id < 0) {
        return false;
    }

    return true;
}

