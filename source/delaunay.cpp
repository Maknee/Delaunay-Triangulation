#include "../include/delaunay.h"

using namespace Delaunay;

DelaunayMesh::DelaunayMesh(Points points) :
  sorted_points(std::move(points))
{
  //
  // Can't form triangle without three points
  //

  assert(sorted_points.size() >= 3);

  //
  // Allow for deletion of faces
  //

  request_halfedge_status();
  request_edge_status();
  request_face_status();
  request_vertex_status();

  //
  // Sort from smallest to biggest vertex based on x 
  // https://en.wikipedia.org/wiki/Point_set_triangulation
  //

  std::sort(std::begin(sorted_points), std::end(sorted_points),
            [](const Mesh::Point& p1, const Mesh::Point& p2)
            {
              return p1[0] < p2[0];
            });

  //
  // Run algorithm
  //

  BowyerWatson();
}

void DelaunayMesh::AddSuperTriangle()
{
  //
  // Construct super triangle 10 times min and max of all points
  //

  const auto [point_min_y, point_max_y] = std::minmax_element(std::begin(sorted_points), std::end(sorted_points), 
                                                              [](const auto& p1, const auto& p2)
                                                              {
                                                                return p1[1] < p2[1];
                                                              });
#define EXPANSION_SIZE 10.0f

  auto min_x = sorted_points[0][0] * EXPANSION_SIZE;
  auto max_x = sorted_points.back()[0] * EXPANSION_SIZE;
  auto min_y = (*point_min_y)[1] * EXPANSION_SIZE;
  auto max_y = (*point_max_y)[1] * EXPANSION_SIZE;

  auto middle_x = (min_x + max_x) / 2.0f;

  Mesh::Point a(min_x, min_y, 0.0f);
  Mesh::Point b(max_x, min_y, 0.0f);
  Mesh::Point c(middle_x, max_y, 0.0f);

  auto v1 = add_vertex(a);
  auto v2 = add_vertex(b);
  auto v3 = add_vertex(c);

  super_triangle = Triangle(this, v1, v2, v3);
  triangles.emplace_back(super_triangle);
}

void DelaunayMesh::BowyerWatson()
{
  //
  // Follows the psuedo code comments described in
  // https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm
  //

  //
  // add super-triangle to triangulation // must be large enough to completely contain all the points in pointList
  //

  AddSuperTriangle();

  for (const auto& point : sorted_points)
  {
    auto new_vertex = add_vertex(point);
    vertices.emplace_back(new_vertex);
    
    // keep track of not bad triangles (later put back into vertices)
    std::vector<Triangle> not_bad_triangles;

    //
    // badTriangles := empty set
    // for each triangle in triangulation do // first find all the triangles that are no longer valid due to the insertion
    //   if point is inside circumcircle of triangle
    //     add triangle to badTriangles
    //

    std::vector<Triangle> bad_triangles;
    for (const auto& triangle : triangles)
    {
      const auto& v1 = triangle.v1;
      const auto& v2 = triangle.v2;
      const auto& v3 = triangle.v3;

      if (triangle.Circumcircles(new_vertex))
      {
        bad_triangles.emplace_back(std::move(triangle));
      }
      else
      {
        not_bad_triangles.emplace_back(std::move(triangle));
      }
    }

    //
    // polygon := empty set
    // for each triangle in badTriangles do // find the boundary of the polygonal hole
    //   for each edge in triangle do
    //    if edge is not shared by any other triangles in badTriangles
    //      add edge to polygon
    //

    auto shared_by_other_triangles = [&](const auto& bad_triangle, const auto& edge)
    {
      return std::any_of(std::begin(bad_triangles), std::end(bad_triangles), [&](const auto& triangle)
      {
        return triangle.FindEdge(edge) && (&bad_triangle != &triangle);
      });
    };

    std::vector<::Edge> polygon;
    for (const auto& bad_triangle : bad_triangles)
    {
      for (const auto& edge : bad_triangle.GetEdges())
      {
        if (!shared_by_other_triangles(bad_triangle, edge))
        {
          polygon.emplace_back(edge);
        }
      }
    }
    
    //
    // for each triangle in badTriangles do // remove them from the data structure
    //   remove triangle from triangulation
    //
    // (in other words, keep track of triangles that are not bad in triangulation)
    //

    triangles = std::move(not_bad_triangles);

    // for each edge in polygon do // re-triangulate the polygonal hole
    //  newTri : = form a triangle from edge to point
    //  add newTri to triangulation

    for (const auto& edge : polygon)
    {
      AddTriangle(edge.v1, edge.v2, new_vertex);
    }
  }

  //
  // for each triangle in triangulation // done inserting points, now clean up
  //   if triangle contains a vertex from original super - triangle
  //     remove triangle from triangulation
  //

  triangles.erase(std::remove_if(std::begin(triangles), std::end(triangles),
                  [this](const auto& triangle)
                  {
                    for (const auto& edge : triangle.GetEdges())
                    {
                      if (super_triangle.FindEdge(edge))
                      {
                        return true;
                      }
                    }
                    return false;
                  }));

  //
  // Form mesh faces from triangles
  //

  for (const auto& triangle : triangles)
  {
    auto triangle_vertices = { triangle.v3, triangle.v2, triangle.v1 };
    add_face(triangle_vertices);
  }

  //clean super triangle
  delete_vertex(super_triangle.v1);
  delete_vertex(super_triangle.v2);
  delete_vertex(super_triangle.v3);

  //make sure to clean up
  garbage_collection();
}
