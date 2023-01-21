#ifndef MATRIX_MATRIX_OOP_
#define MATRIX_MATRIX_OOP_

#include <cmath>
#include <exception>
#include <iostream>

#define ACCURACY 1e-7

class Matrix {
 public:
  // Конструкторы и деструктор
  Matrix();
  Matrix(int rows, int cols);
  Matrix(int rows, int cols, const double data[]);
  Matrix(const Matrix& other);
  Matrix(Matrix&& other);
  ~Matrix();

  // Перегрузка операторов

  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  Matrix operator*(const Matrix& other) const;
  friend Matrix operator*(const double num, const Matrix& other);
  Matrix operator*(const double num) const;
  bool operator==(const Matrix& other) const;
  Matrix& operator=(const Matrix& other);
  void operator+=(const Matrix& other);
  void operator-=(const Matrix& other);
  void operator*=(const Matrix& other);
  void operator*=(const double num);
  double& operator()(int i, int j);
  double operator()(int i, int j) const;

  // Сеттеры

  void SetRows(int rows);
  void SetCols(int cols);

  // Геттеры

  int GetRows() const;
  int GetCols() const;

  // Методы класса

  bool EqMatrix(const Matrix& other) const;
  void SumMatrix(const Matrix& other);
  void SubMatrix(const Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const Matrix& other);
  Matrix Transpose() const;
  Matrix CalcComplements() const;
  double Determinant() const;
  Matrix InverseMtrix() const;

 private:
  void MemoryAllocation();
  void MemoryAllocation(const int rows, const int cols);
  void MemoryAllocation(const Matrix& other);
  void CopyElementMatrix(const Matrix& temp, const int rows, const int cols);
  void FreeMemory();
  void SizeError(const Matrix& other) const;
  void EmptyMatrixError() const;
  void EmptyMatrixError(const Matrix& other) const;
  void ReducingMatrixTriangular(const Matrix& temp) const;
  void MatrixReduction(const int r, const int c,
                       Matrix& result_reduction) const;

 private:
  int rows_, cols_;
  double** matrix_ = nullptr;
};

#endif  // MATRIX_MATRIX_OOP_