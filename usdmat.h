/*
USDMAT (https://github.com/jerenchen/usdmat)

A simple C++ header for reading USD mesh/point-based geoms as Eigen matrices,
  intended for libIGL, or as ML tensors.

This source code is licensed under the BSD 3-Clause "New" or "Revised" License
*/
#ifndef USD_MAT_H_
#define USD_MAT_H_
#include <Eigen/Dense>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/mesh.h>

#include <stdexcept>


namespace usdmat
{
namespace // anonymous scope
{
using namespace pxr;

void triangulate_mesh_(
  const UsdPrim& prim,
  Eigen::MatrixXi& F,
  UsdTimeCode time = UsdTimeCode()
)
{
  UsdGeomMesh mesh(prim);

  VtArray<int> counts;
  VtArray<int> indices;
  mesh.GetFaceVertexCountsAttr().Get(&counts, time);
  mesh.GetFaceVertexIndicesAttr().Get(&indices, time);

  size_t num_faces = 0;
  for (const auto cnt : counts)
    num_faces += cnt - 2; // num triangle faces = num poly face vertices - 2

  F.resize(num_faces, 3);

  size_t r = 0;
  size_t first_index = 0;
  for (const auto cnt : counts)
  {
    for (size_t i = 0; i < (cnt - 2); ++i)
    {
      for (size_t c = 0; c < 3; ++c)
      {
        const size_t corner_index = first_index + (c == 0 ? 0 : i + c);
        F(r, c) = indices[corner_index];
      }
      r++;
    }
    first_index += cnt;
  }
}

void animation_tensor_(
  const UsdPrim& prim,
  Eigen::MatrixXd& A
)
{
  auto attr = UsdGeomPointBased(prim).GetPointsAttr();

  std::vector<double> timesamples;
  attr.GetTimeSamples(&timesamples);
  size_t num_rows = timesamples.size();
  A.resize(num_rows, 0);
  size_t row = 0;
  for (const auto time : timesamples)
  {
    VtVec3fArray points;
    attr.Get(&points, time);
    size_t num_cols = points.size() * 3;
    if (num_cols > A.cols())
      A.conservativeResizeLike(Eigen::MatrixXd::Zero(num_rows, num_cols));

    size_t col = 0;
    for (const auto& p : points)
    {
      A(row, col++) = double(p[0]);
      A(row, col++) = double(p[1]);
      A(row, col++) = double(p[2]);
    }
    row++;
  }
}
};

void read(
  const std::string& filename,
  const std::string& primpath,
  Eigen::MatrixXi& F,
  Eigen::MatrixXd& A
)
{
  auto stage = pxr::UsdStage::Open(filename);
  UsdPrim prim;
  if (!(prim = stage->GetPrimAtPath(pxr::SdfPath(primpath))))
    throw std::runtime_error("Prim not found!");

  if (!prim.IsA<pxr::UsdGeomMesh>())
    throw std::runtime_error("Prim is not a mesh!");

  triangulate_mesh_(prim, F);
  animation_tensor_(prim, A);
}

void read(
  const std::string& filename,
  const std::string& primpath,
  Eigen::MatrixXd& A
)
{
  auto stage = pxr::UsdStage::Open(filename);
  UsdPrim prim;
  if (!(prim = stage->GetPrimAtPath(pxr::SdfPath(primpath))))
    throw std::runtime_error("Prim not found!");

  if (!prim.IsA<pxr::UsdGeomPointBased>())
    throw std::runtime_error("Prim is not point based!");

  animation_tensor_(prim, A);
}
};

#endif