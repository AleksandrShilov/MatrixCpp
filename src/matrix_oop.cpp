#include "matrix_oop.h"

Matrix::Matrix() : rows_(0), cols_(0), matrix_(nullptr) {}

Matrix::Matrix(int rows, int cols) : rows_(cols), cols_(rows) {
  if (rows <= 0 || cols <= 0) {
    throw std::invalid_argument(
        "Error, rows and cols must be greater than zero");
  }
  MemoryAllocation(rows_, cols_);
}

Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_) {
  MemoryAllocation(other);
}

Matrix::Matrix(Matrix&& other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

Matrix::~Matrix() { FreeMemory(); }

Matrix Matrix::operator-(const Matrix& other) const {
  Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
  Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

Matrix operator*(const double num, const Matrix& other) {
  Matrix result(other);
  result.MulNumber(num);
  return result;
}

Matrix Matrix::operator*(const double num) const {
  Matrix result(*this);
  result.MulNumber(num);
  return result;
}

bool Matrix::operator==(const Matrix& other) const {
  bool result = EqMatrix(other);
  return result;
}

Matrix& Matrix::operator=(const Matrix& other) {
  FreeMemory();
  rows_ = other.rows_;
  cols_ = other.cols_;
  MemoryAllocation(other);
  return *this;
}

void Matrix::operator+=(const Matrix& other) { SumMatrix(other); }

void Matrix::operator-=(const Matrix& other) { SubMatrix(other); }

void Matrix::operator*=(const Matrix& other) { MulMatrix(other); }

void Matrix::operator*=(const double num) { MulNumber(num); }

double& Matrix::operator()(int i, int j) {
  EmptyMatrixError();
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Error, index incorrect");
  }
  return matrix_[i][j];
}

double Matrix::operator()(int i, int j) const {
  EmptyMatrixError();
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::out_of_range("Error, index incorrect");
  }
  return matrix_[i][j];
}

void Matrix::SetRows(int rows) {
  if (rows <= 0) {
    throw std::invalid_argument("Error, the string cannot be less than zero");
  }

  if (rows != rows_) {
    Matrix temp(*this);
    this->~Matrix();

    // создание новой матрицы и заполнение нулями
    MemoryAllocation(rows, cols_);

    if (rows >= rows_) {
      CopyElementMatrix(temp, rows_, cols_);
    } else {
      CopyElementMatrix(temp, rows, cols_);
    }
    rows_ = rows;
  }
}

void Matrix::SetCols(int cols) {
  if (cols <= 0) {
    throw std::invalid_argument("Error, the string cannot be less than zero");
  }

  if (cols != cols_) {
    Matrix temp(*this);
    this->~Matrix();

    // создание новой матрицы и заполнение нулями
    MemoryAllocation(rows_, cols);

    if (cols >= cols_) {
      CopyElementMatrix(temp, rows_, cols_);
    } else {
      CopyElementMatrix(temp, rows_, cols);
    }
    cols_ = cols;
  }
}

int Matrix::GetRows() const { return rows_; }

int Matrix::GetCols() const { return cols_; }

bool Matrix::EqMatrix(const Matrix& other) const {
  bool result = true;
  if (rows_ == other.rows_ && cols_ == other.cols_ && matrix_ &&
      other.matrix_) {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        if (fabs(matrix_[i][j] - other.matrix_[i][j]) > ACCURACY) {
          result = false;
          break;
        }
      }
    }
  } else {
    result = false;
  }
  return result;
}

void Matrix::SumMatrix(const Matrix& other) {
  EmptyMatrixError(other);
  SizeError(other);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void Matrix::SubMatrix(const Matrix& other) {
  EmptyMatrixError(other);
  SizeError(other);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void Matrix::MulNumber(const double num) {
  EmptyMatrixError();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void Matrix::MulMatrix(const Matrix& other) {
  if (cols_ != other.rows_) {
    throw std::invalid_argument(
        "Error, the number of columns of the first matrix is \
                          not equal to the number of rows of the second matrix");
  }
  EmptyMatrixError(other);
  Matrix res(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      res.matrix_[i][j] = 0;
      for (int k = 0; k < cols_; k++) {
        res.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = res;
}

Matrix Matrix::Transpose() const {
  if (matrix_ == nullptr) {
    throw std::invalid_argument("Error, incorect matrix");
  }
  Matrix result(rows_, cols_);
  for (int i = 0; i < cols_; i++) {
    for (int j = 0; j < rows_; j++) {
      result.matrix_[i][j] = matrix_[j][i];
    }
  }
  return result;
}

Matrix Matrix::CalcComplements() const {
  EmptyMatrixError();
  if (cols_ != rows_) {
    throw std::invalid_argument("Error, the matrix is not square");
  }
  Matrix result(rows_, cols_);
  if (rows_ == 1) {
    result.matrix_[0][0] = 1;
  } else {
    Matrix reducation(rows_ - 1, cols_ - 1);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        MatrixReduction(i, j, reducation);
        double d = reducation.Determinant();
        result.matrix_[i][j] = pow(-1., (double)(i + j)) * d;
      }
    }
  }
  return result;
}

double Matrix::Determinant() const {
  EmptyMatrixError();
  if (cols_ != rows_) {
    throw std::invalid_argument("Error, the matrix is not square");
  }
  Matrix temp(*this);
  double det = 1;
  if (rows_ == 1) {
    det = temp.matrix_[0][0];
  } else {
    ReducingMatrixTriangular(temp);
    for (int i = 0; i < rows_; i++) {
      det *= temp.matrix_[i][i];
    }
  }
  return det;
}

Matrix Matrix::InverseMtrix() const {
  EmptyMatrixError();
  if (rows_ < 2) {
    throw std::invalid_argument("Erorr, incorrect matrix");
  }
  Matrix result(rows_, cols_);
  double det = Determinant();

  if (det == 0) {
    throw std::invalid_argument("Erorr, the determinant of the matrix is ZERO");
  }

  Matrix tempMatr = CalcComplements().Transpose();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      result.matrix_[i][j] = tempMatr.matrix_[i][j] / det;
    }
  }
  return result;
}

// Выделение памяти
void Matrix::MemoryAllocation(const int rows, const int cols) {
  if (rows >= 0 && cols >= 0) {
    matrix_ = new double*[rows];
    for (int i = 0; i < rows; i++) {
      matrix_[i] = new double[cols]{};
    }
  }
}

Matrix Matrix::operator+(const Matrix& other) const {
  Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

Matrix::Matrix(int rows, int cols, const double data[]) {
  if (rows > 0 && cols > 0) {
    rows_ = rows;
    cols_ = cols;
    matrix_ = new double*[rows_];
    int k = 0;
    for (int i = 0; i < rows_; i++) {
      matrix_[i] = new double[cols_];
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = data[k];
        k++;
      }
    }
  }
}

// Выделение памяти для копирования
void Matrix::MemoryAllocation(const Matrix& other) {
  if (rows_ >= 0 && cols_ >= 0) {
    matrix_ = new double*[rows_];
    for (int i = 0; i < rows_; i++) {
      matrix_[i] = new double[cols_];
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
}

// Копирование элементов матрицы
void Matrix::CopyElementMatrix(const Matrix& temp, const int rows,
                               const int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix_[i][j] = temp.matrix_[i][j];
    }
  }
}

void Matrix::FreeMemory() {
  if (matrix_) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
}

// Ошибка размерности матриц
void Matrix::SizeError(const Matrix& other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("Error, different dimension of matrices");
  }
}

// Ошибка пустой матрицы
void Matrix::EmptyMatrixError() const {
  if (matrix_ == nullptr) {
    throw std::invalid_argument("Error, incorect matrix");
  }
}

// Ошибка пустых матриц
void Matrix::EmptyMatrixError(const Matrix& other) const {
  if (matrix_ == nullptr || other.matrix_ == nullptr) {
    std::invalid_argument("Error, incorect matrix");
  }
}

// Приведение матрицы к треугольной
void Matrix::ReducingMatrixTriangular(const Matrix& temp) const {
  double number_zero;
  for (int i = 0; i < temp.rows_; i++) {
    if (!temp.matrix_[i][i]) {
      for (int j = i + 1; j < rows_; j++) {
        for (int k = 0; k < rows_; k++) {
          temp.matrix_[i][k] += temp.matrix_[j][k];
        }
      }
    }
    if (temp.matrix_[i][i]) {
      for (int j = i + 1; j < temp.rows_; j++) {
        number_zero = temp.matrix_[j][i] / temp.matrix_[i][i];
        for (int k = 0; k < temp.rows_; k++) {
          temp.matrix_[j][k] -= temp.matrix_[i][k] * number_zero;
        }
      }
    }
  }
}

void Matrix::MatrixReduction(const int r, const int c,
                             Matrix& matr_reduction) const {
  int x = 0;
  for (int i = 0; i < rows_; i++) {
    if (r != i) {
      int y = 0;
      for (int j = 0; j < cols_; j++) {
        if (c != j) {
          matr_reduction.matrix_[x][y] = matrix_[i][j];
          y++;
        }
      }
      x++;
    }
  }
}
