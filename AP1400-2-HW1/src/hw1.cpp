#include "hw1.h"
static inline void check_size(size_t n, size_t m) {
  if (n <= 0 || m <= 0) {
    throw std::invalid_argument("size num is error!");
  }
}
static inline void check_logic(double min, double max) {
  if (min > max) {
    throw std::logic_error("min big than max!");
  }
}
static inline void check_empty(const Matrix& matrix) {
  if (matrix.empty()) {
    throw std::logic_error("matrix is empty!");
  }
}
namespace algebra {

Matrix zeros(size_t n, size_t m) {
  check_size(n, m);
  return Matrix(n, std::vector<double>(m, 0));
};
Matrix ones(size_t n, size_t m) {
  check_size(n, m);
  return Matrix(n, std::vector<double>(m, 1));
}
Matrix random(size_t n, size_t m, double min, double max) {
  check_size(n, m);
  check_logic(min, max);
  std::random_device rd;   // 随机设备用于生成种子
  std::mt19937 gen(rd());  // Mersenne Twister引擎作为随机数生成器
  std::uniform_real_distribution<double> dis(min, max);  // 创建实数分布
  Matrix temp = zeros(n, m);
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      temp[i][j] = dis(gen);
    }
  }
  return temp;
}
void show(const Matrix& matrix) {
  for (auto i : matrix) {
    for (auto j : i) {
      std::cout << std::fixed << std::setprecision(3) << j << " ";
    }
    std::cout << "\n";
  }
}
Matrix multiply(const Matrix& matrix, double c) {
  check_empty(matrix);
  Matrix temp(matrix);
  for (auto& i : temp) {
    for (auto& j : i) {
      j *= c;
    }
  }
  return temp;
}
Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
  if (matrix1.empty()) return matrix1;
  if (matrix2.empty()) return matrix2;
  size_t n = matrix1.size(), m = matrix1[0].size(), n1 = matrix2.size(),
         m1 = matrix2[0].size();
  if (m != n1) {
    throw std::logic_error(
        " matrices with wrong dimensions cannot be multiplied!");
  }
  Matrix temp = zeros(n, m1);
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m1; ++j) {
      for (size_t k = 0; k < m; ++k) {
        temp[i][j] += matrix1[i][k] * matrix2[k][j];
      }
    }
  }
  return temp;
}
Matrix sum(const Matrix& matrix, double c) {
  if (matrix.empty()) return matrix;
  Matrix temp(matrix);
  for (auto& i : temp) {
    for (auto& j : i) {
      j += c;
    }
  }
  return temp;
}
Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
  if (matrix1.empty() && matrix2.empty()) return matrix1;
  if (matrix1.empty() || matrix2.empty()) {
    throw std::logic_error("matrix is non!");
  }
  size_t n1 = matrix1.size(), m1 = matrix1[0].size(), n2 = matrix2.size(),
         m2 = matrix2[0].size();
  if (n1 != n2 || m1 != m2) {
    throw std::logic_error("matrices with wrong dimensions cannot be summed!");
  }
  Matrix temp = zeros(n1, m1);
  for (size_t i = 0; i < n1; ++i) {
    for (size_t j = 0; j < m1; ++j) {
      temp[i][j] = matrix1[i][j] + matrix2[i][j];
    }
  }
  return temp;
}
Matrix transpose(const Matrix& matrix) {
  if (matrix.empty()) return matrix;
  Matrix temp = zeros(matrix[0].size(), matrix.size());
  int n = temp.size(), m = temp[0].size();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      temp[i][j] = matrix[j][i];
    }
  }
  return temp;
}
Matrix minor(const Matrix& matrix, size_t n, size_t m) {
  Matrix temp = zeros(matrix.size() - 1, matrix[0].size() - 1);
  std::vector<double> t;
  for (int i = 0; i < matrix.size(); ++i) {
    for (int j = 0; j < matrix[0].size(); ++j) {
      if (i == n || j == m) continue;
      t.push_back(matrix[i][j]);
    }
  }
  int idx = 0;
  for (int i = 0; i < temp.size(); ++i) {
    for (int j = 0; j < temp[0].size(); ++j) {
      temp[i][j] = t[idx++];
    }
  }
  return temp;
}
double det2(const Matrix& t) { return t[0][0] * t[1][1] - t[0][1] * t[1][0]; }
double det3(const Matrix& matrix) {
  double det = 0;
  det += matrix[0][0] * matrix[1][1] * matrix[2][2];
  det += matrix[0][1] * matrix[1][2] * matrix[2][0];
  det += matrix[0][2] * matrix[1][0] * matrix[2][1];

  // 计算反对角线元素之积
  det -= matrix[0][2] * matrix[1][1] * matrix[2][0];
  det -= matrix[0][0] * matrix[1][2] * matrix[2][1];
  det -= matrix[0][1] * matrix[1][0] * matrix[2][2];
  return det;
}
double determinant(const Matrix& matrix) {
  if (matrix.empty()) return 1;
  int n = matrix.size(), m = matrix[0].size();
  if (n != m) {
    throw std::logic_error("non-square matrices have no determinant!");
  }
  if (n == 1) return matrix[0][0];
  if (n == 2) return det2(matrix);
  if (n == 3) return det3(matrix);
  double res = 0;
  for (int i = 0; i < n; ++i) {
    Matrix t = minor(matrix, 0, i);
    int sign = (i & 1) ? -1 : 1;
    res += determinant(t) * sign * matrix[0][i];
  }
  return res;
}
Matrix inverse(const Matrix& matrix) {
  if (matrix.empty()) return matrix;
  int n = matrix.size(), m = matrix[0].size();
  if (n != m) {
    throw std::logic_error("error");
  }
  double det = determinant(matrix);
  if (!det) {
    throw std::logic_error("error");
  }
  Matrix res = zeros(n, m);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      Matrix t = minor(matrix, i, j);
      double sign = (i + j) & 1 ? -1 : 1;
      double det_temp = determinant(t);
      res[i][j] = sign * det_temp / det;
    }
  }
  res = transpose(res);
  return res;
}
Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) {
  int n = matrix1.size(), m = matrix1[0].size(), n1 = matrix2.size(),
      m1 = matrix2[0].size();
  if (axis && n != n1) {
    throw std::logic_error("error");
  }
  if (!axis && m != m1) {
    throw std::logic_error("error");
  }

  if (axis) {
    Matrix t = zeros(n, m + m1);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m + m1; ++j) {
        if (j <= m - 1) {
          t[i][j] = matrix1[i][j];
        } else {
          t[i][j] = matrix2[i][j - m];
        }
      }
    }
    return t;
  } else {
    Matrix t = zeros(n + n1, m);
    for (int i = 0; i < n + n1; ++i) {
      for (int j = 0; j < m; ++j) {
        if (i <= n - 1) {
          t[i][j] = matrix1[i][j];
        } else {
          t[i][j] = matrix2[i - n][j];
        }
      }
    }
    return t;
  }
  return matrix1;
}
Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
  int n = matrix.size();
  int m = matrix[0].size();
  if (r1 >= n || r2 >= n) {
    throw std::logic_error("error");
  }
  Matrix t = zeros(n, matrix[0].size());
  for (int i = 0; i < n; ++i) {
    if (i == r1) {
      for (int j = 0; j < m; ++j) {
        t[i][j] = matrix[r2][j];
      }
    } else if (i == r2) {
      for (int j = 0; j < m; ++j) {
        t[i][j] = matrix[r1][j];
      }
    } else {
      for (int j = 0; j < m; ++j) {
        t[i][j] = matrix[i][j];
      }
    }
  }
  return t;
}
Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
  Matrix t(matrix);
  for (int i = 0; i < matrix[0].size(); ++i) {
    t[r][i] *= c;
  }
  return t;
}
Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
  Matrix t(matrix);
  for (int i = 0; i < matrix[0].size(); ++i) {
    t[r2][i] += t[r1][i] * c;
  }
  return t;
}
Matrix upper_triangular(const Matrix& matrix) {
  if (matrix.empty()) return matrix;
  int n = matrix.size(), m = matrix[0].size();
  if (n != m) {
    throw std::logic_error("error");
  }
  Matrix t(matrix);
  for(int i=0;i<n;++i){
    for(int j=i+1;j<n;++j){
      if(!t[i][i])t=ero_swap(t,i,i+1);
      double ratio=t[j][i]/t[i][i];
      for(int k=0;k<n;++k){
        t[j][k]-=ratio*t[i][k];
      }
    }
  }
  return t;
}
}  // namespace algebra
