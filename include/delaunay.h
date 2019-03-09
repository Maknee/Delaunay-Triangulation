/**
 * @file delaunay.h
 * @author Henry Zhu
 * @brief This file contains a class to compute the Delaunay triangulation using the Bowyer Waston algorithm
 * https://en.wikipedia.org/wiki/Delaunay_triangulation
 * https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm
 */

#pragma once

#include "util.h"
#include "triangle.h"

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <algorithm>
#include <utility>
#include <type_traits>

namespace Delaunay
{
  using Points = std::vector<Mesh::Point>;

  /**
   * DelaunayMesh is a mesh that constructed from points using an algorithm (in this case, Bowyer Watson) 
   */
  
  class DelaunayMesh : public Mesh
  {
  public:

    /**
     * DelaunayMesh constructor
     *
     * @param points Points to construct the Delaunay Mesh
     */

    explicit DelaunayMesh(Points points);

    /**
     * Creates a triangle out of points and appends it to mesh
     *
     * Example of usage:
     *
     * Mesh::VertexHandle v1, v2, v3;
     * ...
     * AddTriangle(v1, v2, v3);
     *
     * @param ts Points to create a triangle
     * @return Newly formed triangle
     */

    template<typename... Ts>
    decltype(auto) AddTriangle(Ts&&... ts)
    {
      static_assert(sizeof...(Ts) == NUM_TRIANGLE_POINTS, "Number of vertices must be NUM_TRIANGLE_POINTS");
      //static_assert(std::is_same<Mesh::VertexHandle, std::decay_t<Ts...>>::value, "Parameters do not contain VertexHandles");
      Triangle t{ this, std::forward<Ts>(ts)... };
      return triangles.emplace_back(t);
    }

    /**
     * Creates a super triangle that contains all other triangles and appends it to mesh
     */

    void AddSuperTriangle();

    /**
     * Performs the Bowyer Watson algorithm to compute the correct triangle and faces
     * 
     * First computes vertices / edges
     * Then actually creates the faces in the mesh using OpenMesh
     */

    void BowyerWatson();

  private:
    std::vector<Mesh::Point> sorted_points; // User input points sorted
    std::vector<Mesh::VertexHandle> vertices; // Vertices of the mesh

    Delaunay::Triangle super_triangle; // Super triangle
    std::vector<Triangle> triangles; // Triangles that consist the mesh
  };
}