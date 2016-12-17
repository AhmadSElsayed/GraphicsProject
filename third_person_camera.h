//-----------------------------------------------------------------------------
// Copyright (c) 2006-2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#if !defined(THIRD_PERSON_CAMERA_H)
#define THIRD_PERSON_CAMERA_H

#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "mathlib.h"
using namespace glm;

//-----------------------------------------------------------------------------
// A quaternion based third person camera class.
//
// Call the lookAt() method to establish the distance and position of the
// camera relative to the target look at position. The lookAt() method stores
// this relationship to the target look at position in the offset vector. The
// look at position can be changed using the lookAt(target) method.
//
// Aside from the lookAt() method there is no way to move the camera's eye
// position. The camera's eye position is automatically calculated and updated
// in the update() method using the offset vector.
//
// Rotating the camera using the rotate() method allows the camera to be
// orbited around the the look at position. 
//-----------------------------------------------------------------------------

class ThirdPersonCamera
{
public:
    ThirdPersonCamera();
    ~ThirdPersonCamera();

    void lookAt(const Vector3 &target);
    void lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);
    void perspective(float fovx, float aspect, float znear, float zfar);
    void rotate(float longitudeDegrees, float latitudeDegrees);
    void update(float elapsedTimeSec);

    // Getter methods.

    const Vector3 &getPosition() const;
    const Quaternion &getOrientation() const;
    const mat4 getViewMatrix() const;
    const Vector3 &getViewDirection() const;
    const mat4 getProjectionMatrix() const;
    const Vector3 &getXAxis() const;
    const Vector3 &getYAxis() const;
    const Vector3 &getZAxis() const;

    Vector3 m_target;
    Vector3 m_eye;
private:
    static const float DEFAULT_FOVX;
    static const float DEFAULT_ZFAR;
    static const float DEFAULT_ZNEAR;
    static const Vector3 WORLD_XAXIS;
    static const Vector3 WORLD_YAXIS;
    static const Vector3 WORLD_ZAXIS;

    float m_longitudeDegrees;
    float m_latitudeDegrees;
    float m_fovx;
    float m_znear;
    float m_zfar;
    Vector3 m_offset;
    Vector3 m_xAxis;
    Vector3 m_yAxis;
    Vector3 m_zAxis;
    Vector3 m_viewDir;
    Matrix4 m_viewMatrix;
    Matrix4 m_projMatrix;
    Quaternion m_orientation;
};

//-----------------------------------------------------------------------------

inline const Vector3 &ThirdPersonCamera::getPosition() const
{ return m_eye; }

inline const Quaternion &ThirdPersonCamera::getOrientation() const
{ return m_orientation; }

inline const glm::mat4 ThirdPersonCamera::getViewMatrix() const
{
    return mat4(m_viewMatrix[0][0],
                m_viewMatrix[0][1],
                m_viewMatrix[0][2],
                m_viewMatrix[0][3],
                     
                m_viewMatrix[1][0],
                m_viewMatrix[1][1],
                m_viewMatrix[1][2],
                m_viewMatrix[1][3],
                     
                m_viewMatrix[2][0],
                m_viewMatrix[2][1],
                m_viewMatrix[2][2],
                m_viewMatrix[2][3],
                     
                m_viewMatrix[3][0],
                m_viewMatrix[3][1],
                m_viewMatrix[3][2],
                m_viewMatrix[3][3]);
}

inline const Vector3 &ThirdPersonCamera::getViewDirection() const
{ return m_viewDir; }

inline const glm::mat4 ThirdPersonCamera::getProjectionMatrix() const
{
    return mat4(m_projMatrix[0][0],
                m_projMatrix[0][1],
                m_projMatrix[0][2],
                m_projMatrix[0][3],

                m_projMatrix[1][0],
                m_projMatrix[1][1],
                m_projMatrix[1][2],
                m_projMatrix[1][3],

                m_projMatrix[2][0],
                m_projMatrix[2][1],
                m_projMatrix[2][2],
                m_projMatrix[2][3],

                m_projMatrix[3][0],
                m_projMatrix[3][1],
                m_projMatrix[3][2],
                m_projMatrix[3][3]);
}

inline const Vector3 &ThirdPersonCamera::getXAxis() const
{ return m_xAxis; }

inline const Vector3 &ThirdPersonCamera::getYAxis() const
{ return m_yAxis; }

inline const Vector3 &ThirdPersonCamera::getZAxis() const
{ return m_zAxis; }

#endif