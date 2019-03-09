/**
 * @file util.h
 * @author Henry Zhu
 * @brief Any helper aliases or variable exist here
 */

#pragma once

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

/**
 * Number of points in a triangle
 */
constexpr size_t NUM_TRIANGLE_POINTS = 3;

/**
 * Typedef for mesh
 */
using Mesh = OpenMesh::TriMesh_ArrayKernelT<>;
