#include "../include/triangle.h"

using namespace Delaunay;

const Mesh::Point& Triangle::GetPoint(TrianglePoint triangle_point) const
{
  switch (triangle_point)
  {
  case TrianglePoint::P1:
    return mesh->point(v1);
    break;
  case TrianglePoint::P2:
    return mesh->point(v2);
    break;
  case TrianglePoint::P3:
    return mesh->point(v3);
    break;
  default:
    assert(false);
  }
}

bool Triangle::IsCCW() const
{
  const auto& p1 = GetPoint(TrianglePoint::P1);
  const auto& p2 = GetPoint(TrianglePoint::P2);
  const auto& p3 = GetPoint(TrianglePoint::P3);

  auto Ax = p1[0];
  auto Ay = p1[1];

  auto Bx = p2[0];
  auto By = p2[1];

  auto Cx = p3[0];
  auto Cy = p3[1];

  auto r1_c1 = Ax - Cx;
  auto r1_c2 = Ay - Cy;
  auto r2_c1 = Bx - Cx;
  auto r2_c2 = By - Cy;

  auto determinant = r1_c1 * r2_c2 - r2_c1 * r1_c2;

  return determinant > 0.0f;
}

bool Triangle::Circumcircles(const Mesh::VertexHandle& new_vertex) const
{
  const auto& p1 = GetPoint(TrianglePoint::P1);
  const auto& p2 = GetPoint(TrianglePoint::P2);
  const auto& p3 = GetPoint(TrianglePoint::P3);

  const auto& p4 = mesh->point(new_vertex);

  auto Ax = p1[0];
  auto Ay = p1[1];

  auto Bx = p2[0];
  auto By = p2[1];

  auto Cx = p3[0];
  auto Cy = p3[1];

  auto Dx = p4[0];
  auto Dy = p4[1];

  auto r1_c1 = Ax - Dx;
  auto r2_c1 = Bx - Dx;
  auto r3_c1 = Cx - Dx;

  auto r1_c2 = Ay - Dy;
  auto r2_c2 = By - Dy;
  auto r3_c2 = Cy - Dy;

  auto r1_c3 = (Ax - Dx) * (Ax - Dx) + (Ay - Dy) * (Ay - Dy);
  auto r2_c3 = (Bx - Dx) * (Bx - Dx) + (By - Dy) * (By - Dy);
  auto r3_c3 = (Cx - Dx) * (Cx - Dx) + (Cy - Dy) * (Cy - Dy);

  //probably can make faster using LU vectorization

  auto find_determinant_2x2 = [](auto r1_c1, auto r2_c1, auto r1_c2, auto r2_c2)
  {
    return r1_c1 * r2_c2 - r2_c1 * r1_c2;
  };

  auto determinant =
    r1_c1 * find_determinant_2x2(r2_c2, r3_c2, r2_c3, r3_c3) -
    r1_c2 * find_determinant_2x2(r2_c1, r3_c1, r2_c3, r3_c3) +
    r1_c3 * find_determinant_2x2(r2_c1, r3_c1, r2_c2, r3_c2);

  return determinant > 0.0f;
}

TriangleEdges Triangle::GetEdges() const
{
  return { e1, e2, e3 };
}

bool Triangle::FindEdge(const Edge& e) const
{
  auto edges = GetEdges();
  return std::any_of(std::begin(edges), std::end(edges), [&e](const auto& edge) { return edge == e; });
}

bool Delaunay::operator==(const Triangle& lhs, const Triangle& rhs)
{
  auto lhs_vs = { lhs.v1, lhs.v2, lhs.v3 };

  auto lhs_contains_vertex = [&lhs_vs](const auto& rhs_vertex)
  {
    return std::any_of(std::begin(lhs_vs), std::end(lhs_vs), 
                      [&rhs_vertex](const auto& lhs_vertex)
                      { 
                        return lhs_vertex.idx() == rhs_vertex.idx();
                      });
  };
   
  return lhs_contains_vertex(rhs.v1) && lhs_contains_vertex(rhs.v2) && lhs_contains_vertex(rhs.v3);
}

bool Delaunay::operator!=(const Triangle& lhs, const Triangle& rhs)
{
  return !(lhs == rhs);
}
