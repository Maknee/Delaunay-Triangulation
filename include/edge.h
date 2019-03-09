/**
 * @file edge.h
 * @author Henry Zhu
 * @brief Edge class
 */

#pragma once

#include "util.h"

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

namespace Delaunay
{
  /**
   * Edge class representing connection between two vertices
   */

  struct Edge
  {
    Mesh::VertexHandle v1;
    Mesh::VertexHandle v2;

    friend bool operator==(const Edge& lhs, const Edge& rhs)
    {
      return (lhs.v1.idx() == rhs.v1.idx() && lhs.v2.idx() == rhs.v2.idx()) ||
        (lhs.v1.idx() == rhs.v2.idx() && lhs.v2.idx() == rhs.v1.idx());
    }
  };
};
