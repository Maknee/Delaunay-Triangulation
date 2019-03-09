/**
 * @file triangle.h
 * @author Henry Zhu
 * @brief Triangle class
 */

#pragma once

#include "util.h"
#include "edge.h"

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

namespace Delaunay
{
  using TriangleEdges = std::array<Edge, NUM_TRIANGLE_POINTS>;

  enum class TrianglePoint
  {
    P1,
    P2,
    P3
  };

  /**
   * Triangle class containing vertices and edges
   */
  class Triangle
  {
  public:
    explicit Triangle() : mesh(nullptr)
    {

    }

    /**
     * Triangle constructor
     *
     * @param mesh_ Reference to mesh object
     * @param v1_ First vertex handle
     * @param v2_ Second vertex handle
     * @param v3_ Third vertex handle
     */

    explicit Triangle(const Mesh* mesh_, Mesh::VertexHandle v1_, Mesh::VertexHandle v2_, Mesh::VertexHandle v3_) :
      mesh(mesh_), v1(std::move(v1_)), v2(std::move(v2_)), v3(std::move(v3_)), e1{ v1, v2 }, e2{ v2, v3 }, e3{ v3, v1 }
    {
      //
      // Make sure is CCW
      // https://stackoverflow.com/questions/24205196/addfacecomplex-edge-error-in-openmesh
      //

      if (!IsCCW())
      {
        std::swap(v1, v3);
      }
    }

    /**
     * Gets corresponding point to vertex handle
     *
     * @param triangle_point Which point to to get
     * @return Reference to point
     */

    const Mesh::Point& GetPoint(TrianglePoint triangle_point) const;

    /**
     * Checks if orientation of triangle is CCW
     * 
     * @return True if CCW
     */

    bool IsCCW() const;

    /**
     * Checks if point is circumcircled by triangle
     * Algorithm used in:
     * https://en.wikipedia.org/wiki/Delaunay_triangulation
     *
     * @new_vertex Vertex that is checked for being circumcircled
     * @return True if CCW
     */

    bool Circumcircles(const Mesh::VertexHandle& new_vertex) const;

    /**
     * Returns three edges of triangle
     * 
     * @return Edges
     */

    TriangleEdges GetEdges() const;

    /**
     * Checks if the triangle contains an edge
     *
     * @return True if edge exists
     */

    bool FindEdge(const Edge& e) const;

    friend bool operator==(const Triangle& lhs, const Triangle& rhs);

    friend bool operator!=(const Triangle& lhs, const Triangle& rhs);

  private:
    const Mesh* mesh;

  public:
    Mesh::VertexHandle v1;
    Mesh::VertexHandle v2;
    Mesh::VertexHandle v3;

    Edge e1;
    Edge e2;
    Edge e3;
  };
}