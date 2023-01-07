#include "hw1.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <random>

namespace algebra {

Matrix zeros(size_t n, size_t m) {
  return Matrix(n, Col(m, 0));
}

Matrix ones(size_t n, size_t m) {
  return Matrix(n, Col(m, 1));
}

Matrix random(size_t n, size_t m, double min, double max) {
  if (min > max) throw std::logic_error("min should less than max");
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_real_distribution<double> distribution(min, max);
  return Matrix(n, Col(m, distribution(generator)));
}

void show(const Matrix &matrix) {
  std::cout << std::fixed;
  for (auto i = 0; i < matrix.size(); ++i) {
    for (auto j = 0; j < matrix[i].size(); ++j) {
      std::cout << std::setprecision(3) << matrix[i][j] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << std::flush;
}

Matrix multiply(const Matrix &matrix, double c) {
  Matrix res = matrix;
  for (auto i = 0; i < matrix.size(); ++i) {
    for (auto j = 0; j < matrix[i].size(); ++j) {
      res[i][j] *= c;
    }
  }
  return res;
}

Matrix multiply(const Matrix &matrix1, const Matrix &matrix2) {
  auto col1 = matrix1.size(), row1 = (col1 == 0 ? 0 : matrix1[0].size());
  auto col2 = matrix2.size(), row2 = (col2 == 0 ? 0 : matrix2[0].size());
  if (row1 != col2) throw std::logic_error("matrix1\'s row should equal matrix2\'s col");
  Matrix res{zeros(col1, row2)};
  for (auto i = 0; i < res.size(); ++i) {
    for (auto j = 0; j < res[i].size(); ++j) {
      for (auto m = 0; m < row1; ++m) {
        res[i][j] += matrix1[i][m] * matrix2[m][j];
      }
    }
  }
  return res;
}

Matrix sum(const Matrix &matrix, double c) {
  Matrix res = matrix;
  for (auto i = 0; i < matrix.size(); ++i) {
    for (auto j = 0; j < matrix[i].size(); ++j) {
      res[i][j] += c;
    }
  }
  return res;
}

Matrix sum(const Matrix &matrix1, const Matrix &matrix2) {
  auto col1 = matrix1.size(), row1 = (col1 == 0 ? 0 : matrix1[0].size());
  auto col2 = matrix2.size(), row2 = (col2 == 0 ? 0 : matrix2[0].size());
  if (col1 != col2 || row1 != row2) throw std::logic_error("matrix not have equal col/row");
  Matrix res{zeros(col1, row1)};
  for (auto i = 0; i < col1; ++i) {
    for (auto j = 0; j < row1; ++j) {
      res[i][j] = matrix1[i][j] + matrix2[i][j];
    }
  }
  return res;
}

Matrix transpose(const Matrix &matrix) {
  auto col = matrix.size(), row = (col == 0 ? 0 : matrix[0].size());
  Matrix res{zeros(row, col)};
  for (auto i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      res[i][j] = matrix[j][i];
    }
  }
  return res;
}

Matrix minor(const Matrix &matrix, size_t n, size_t m) {
  auto col = matrix.size(), row = (col == 0 ? 0 : matrix[0].size());
  if (n >= col || m >= row) throw std::logic_error("n/m to large");
  Matrix res{zeros(col - 1, row - 1)};
  for (auto i = 0; i < col; ++i) {
    for (auto j = 0; j < row; ++j) {
      if (i == n || j == m) continue;
      res[i > n ? i - 1 : i][j > m ? j - 1 : j] = matrix[i][j];
    }
  }
  return res;
}

double determinant(const Matrix &matrix) {
  auto col = matrix.size(), row = (col == 0 ? 0 : matrix[0].size());
  if (col != row) throw std::logic_error("col should equal to row");
  if (col == 0) return 1;
  if (col == 1) return matrix[0][0];

  double res = 0;
  for (auto i = 0; i < row; ++i) {
    res += pow(-1, i) * matrix[0][i] * determinant(minor(matrix, 0, i));
  }
  return res;
}

Matrix inverse(const Matrix &matrix) {
  auto col = matrix.size(), row = (col == 0 ? 0 : matrix[0].size());
  if (col != row) throw std::logic_error("col should equal to row");
  if (determinant(matrix) == 0) throw std::logic_error("singular matrices have no inverse");
  Matrix res{zeros(col, row)};
  for (auto i = 0; i < col; ++i) {
    for (auto j = 0; j < row; ++j) {
      res[i][j] = pow(-1, i + j) * determinant(minor(matrix, j, i)) / determinant(matrix);
    }
  }
  return res;
}

Matrix concatenate(const Matrix &matrix1, const Matrix &matrix2, int axis) {
  if (axis != 0 && axis != 1) throw std::logic_error("axis value shoule be 0/1");
  auto col1 = matrix1.size(), row1 = (col1 == 0 ? 0 : matrix1[0].size());
  auto col2 = matrix2.size(), row2 = (col2 == 0 ? 0 : matrix2[0].size());
  Matrix res = matrix1;
  if (axis == 0) {
    if (row1 != row2) throw std::logic_error("");
    for (auto i = 0; i < col2; ++i) {
      res.push_back(matrix2[i]);
    }
  } else {
    if (col1 != col2) throw std::logic_error("");
    for (auto i = 0; i < col1; ++i) {
      for (auto j = 0; j < row2; ++j) {
        res[i].push_back(matrix2[i][j]);
      }
    }
  }
  return res;
}

Matrix ero_swap(const Matrix &matrix, size_t r1, size_t r2) {
  auto col = matrix.size(), row = (col == 0 ? 0 : matrix[0].size());
  if (r1 >= col || r2 >= col) throw std::logic_error("");
  Matrix res = matrix;
  using std::swap;
  swap(res[r1], res[r2]);
  return res;
}

Matrix ero_multiply(const Matrix &matrix, size_t r, double c) {
  auto col = matrix.size(), row = (col == 0 ? 0 : matrix[0].size());
  if (r >= col) throw std::logic_error("");
  Matrix res = matrix;
  for (auto i = 0; i < row; ++i) {
    res[r][i] *= c;
  }
  return res;
}

Matrix ero_sum(const Matrix &matrix, size_t r1, double c, size_t r2) {
  auto col = matrix.size(), row = (col == 0 ? 0 : matrix[0].size());
  if (r1 >= col || r2 >= col) throw std::logic_error("");
  Matrix res = matrix;
  for (auto i = 0; i < row; ++i) {
    res[r2][i] += res[r1][i] * c;
  }
  return res;
}

Matrix upper_triangular(const Matrix &matrix) {
  auto col = matrix.size(), row = (col == 0 ? 0 : matrix[0].size());
  if (col != row) throw std::logic_error("");
  Matrix res = matrix;
  for (auto i = 0; i < col; ++i) {
    if (res[i][i] == 0) {
      for (auto j = i + 1; j < col; ++j) {
        if (res[j][i] != 0) {
          res = ero_swap(res, i, j);
          break;
        }
      }
    }
    for (auto j = i + 1; j < col; ++j) {
      res = ero_sum(res, i, (-res[j][i] / res[i][i]), j);
    }
  }
  show(res);
  return res;
}
} // namespace algebra