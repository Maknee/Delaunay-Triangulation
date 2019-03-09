#include "include/delaunay.h"

#include <OpenMesh/Core/IO/MeshIO.hh>

#include <random>

/*
Test cases:
simple.obj
random.obj
large_random.obj
*/

#define DEFAULT_RANDOM_LIMIT (100.0f)
float random_float(float min_x = -DEFAULT_RANDOM_LIMIT, float min_y = DEFAULT_RANDOM_LIMIT)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(min_x, min_y);
  return dis(gen);
}

// Test with points to form
//  /|---|\
// / | \ | \
// \ |  \| /
//  \|---|/
void TestSimple()
{
  Delaunay::Points points
  {
    Mesh::Point(-2.0f, 0.0f, 0.0f),
    Mesh::Point(-1.0f, 1.0f, 0.0f),
    Mesh::Point(-1.0f, -1.0f, 0.0f),

    Mesh::Point(1.0f, -1.0f, 0.0f),
    Mesh::Point(1.0f, 1.0f, 0.0f),
    Mesh::Point(2.0f, 0.0f, 0.0f),
  };

  Delaunay::DelaunayMesh mesh(std::move(points));
  
  // write mesh
  auto filename = "simple.obj";
  if (!OpenMesh::IO::write_mesh(mesh, filename))
  {
    std::cerr << "Cannot write mesh to file " << filename << std::endl;
  }
}

// Test with 10 points between [-100, 100]
void TestRandom()
{
  Delaunay::Points points;
  for (auto i = 0; i < 10; i++)
  {
    auto x = random_float();
    auto y = random_float();
    points.emplace_back(Mesh::Point(x, y, 0.0f));
  }

  Delaunay::DelaunayMesh mesh(std::move(points));

  // write mesh
  auto filename = "random.obj";
  if (!OpenMesh::IO::write_mesh(mesh, filename))
  {
    std::cerr << "Cannot write mesh to file " << filename << std::endl;
  }
}

// Test with 100 points between [-100, 100]
void TestLargeRandom()
{
  Delaunay::Points points;
  for (auto i = 0; i < 100; i++)
  {
    auto x = random_float();
    auto y = random_float();
    points.emplace_back(Mesh::Point(x, y, 0.0f));
  }

  Delaunay::DelaunayMesh mesh(std::move(points));

  // write mesh
  auto filename = "large_random.obj";
  if (!OpenMesh::IO::write_mesh(mesh, filename))
  {
    std::cerr << "Cannot write mesh to file " << filename << std::endl;
  }
}

int main(int argc, char **argv)
{
  TestSimple();
  TestRandom();
  TestLargeRandom();
    
  return 0;
}
