#include "../usdmat.h"

#include <iostream>


int main(int argc, char* argv[])
{
  Eigen::MatrixXi F;
  Eigen::MatrixXd A;
  usdmat::read(OCTOPUS_FILE, "/octopus_low", F, A);

  // Tensor for animation data, where rows are the samples/frames,
  //   and cols are flattened point positions (#points x 3)
  std::cout << "A: " << A.rows() << " x " << A.cols() << std::endl;

  // ATM LibIGL only officially supports the column-major order matrices.
  // To convert a row of A into V, we need to transpose it after reshape,
  //   i.e.: (n: # of points)
  //          Row Vector            Reshape (3 x n)  Transpose (n x 3)
  //   ..v0...  ..v1...  ..v2...      | 1, 4, 7 |    | 1, 2, 3 | ..v1
  // | 1, 2, 3, 4, 5, 6, 7, 8, 9 | >> | 2, 5, 8 | >> | 4, 5, 6 | ..v2
  //                                  | 3, 6, 9 |    | 7, 8, 9 | ..v3
  Eigen::MatrixXd V = A.row(0).reshaped(3, A.cols()/3).transpose();
  std::cout << "V: " << V.rows() << " x " << V.cols() << ", at A.row(0)" << std::endl;
  std::cout << "F: " << F.rows() << " x " << F.cols() << std::endl;

  return 0;
}