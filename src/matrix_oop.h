#ifndef MATRIX_CPP_MATRIX_OOP_H
#define MATRIX_CPP_MATRIX_OOP_H

#include <cmath>
#include <exception>
#include <iostream>

namespace my {

const double KAccuracy = 1e-7;

class Matrix {
 public:
  Matrix();
  Matrix(int rows, int cols);
  Matrix(int rows, int cols, const double data[]);
  Matrix(const Matrix &other);
  Matrix(Matrix &&other);
  ~Matrix();

 public:
  Matrix operator+(const Matrix &other) const;
  Matrix operator-(const Matrix &other) const;
  Matrix operator*(const Matrix &other) const;
  friend Matrix operator*(const double num, const Matrix &other);
  Matrix operator*(const double num) const;
  bool operator==(const Matrix &other) const;
  Matrix &operator=(const Matrix &other);
  void operator+=(const Matrix &other);
  void operator-=(const Matrix &other);
  void operator*=(const Matrix &other);
  void operator*=(const double num);
  double &operator()(int i, int j);
  double operator()(int i, int j) const;

 public:
  void SetRows(int rows);
  void SetCols(int cols);

 public:
  int GetRows() const;
  int GetCols() const;

 public:
  bool EqMatrix(const Matrix &other) const;
  void SumMatrix(const Matrix &other);
  void SubMatrix(const Matrix &other);
  void MulNumber(const double num);
  void MulMatrix(const Matrix &other);
  Matrix Transpose() const;
  Matrix CalcComplements() const;
  double Determinant() const;
  Matrix InverseMatrix() const;

 private:
  void MemoryAllocation();
  void MemoryAllocation(const int rows, const int cols);
  void MemoryAllocation(const Matrix &other);
  void CopyElementMatrix(const Matrix &temp, const int rows, const int cols);
  void FreeMemory();
  void SizeError(const Matrix &other) const;
  void EmptyMatrixError() const;
  void EmptyMatrixError(const Matrix &other) const;
  void ReducingMatrixTriangular(const Matrix &temp) const;
  void MatrixReduction(const int r, const int c,
                       Matrix &result_reduction) const;

 private:
  int rows_, cols_;
  double **matrix_;
};

}  // my

#endif  // MATRIX_CPP_MATRIX_OOP_H