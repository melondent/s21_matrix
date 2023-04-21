#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int return_value = OK;
  if ((rows > 0) && (columns > 0)) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = calloc(rows, sizeof(double *));
    for (int i = 0; i < result->rows; i++) {
      result->matrix[i] = calloc(columns, sizeof(double));
    }
  } else {
    return_value = INCORRECT_MATRIX;
  }
  return return_value;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix) {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i]) free(A->matrix[i]);
    }
    free(A->matrix);
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int return_value = SUCCESS;
  if (A->rows != B->rows || A->columns != B->columns || !matrix_check(A) ||
      !matrix_check(B)) {
    return_value = FAILURE;
  } else {
    int n = A->rows, m = A->columns;
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-07)
          return_value = FAILURE;
  }
  return return_value;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_value = OK;
  if (!result || !matrix_check(A) || !matrix_check(B)) {
    return_value = INCORRECT_MATRIX;
  } else if (A->columns != B->columns || A->rows != B->rows) {
    return_value = INCORRECT_CALCULATION;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
  }
  return return_value;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_value = OK;
  if (!result || !matrix_check(A) || !matrix_check(B)) {
    return_value = INCORRECT_MATRIX;
  } else if (A->columns != B->columns || A->rows != B->rows) {
    return_value = INCORRECT_CALCULATION;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
  }
  return return_value;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int return_value = OK;
  if (!matrix_check(A)) {
    return_value = INCORRECT_MATRIX;
  } else {
    s21_create_matrix(A->rows, A->columns, result);

    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return return_value;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_value = OK;
  if (!matrix_check(A) || !matrix_check(B)) {
    return_value = INCORRECT_MATRIX;
  } else if (A->columns != B->rows || A->rows != B->columns) {
    return_value = INCORRECT_CALCULATION;
  } else {
    s21_create_matrix(A->rows, B->columns, result);

    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }
  return return_value;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int return_value = OK;
  if (!matrix_check(A)) {
    return_value = INCORRECT_MATRIX;
  } else {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[j][i] = A->matrix[i][j];
  }
  return return_value;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int return_value = OK;
  if (!matrix_check(A)) {
    return_value = INCORRECT_MATRIX;
  } else if (A->columns != A->rows) {
    return_value = INCORRECT_CALCULATION;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        matrix_t minor;
        get_matrix(i, j, *A, &minor);
        double tmp_determinant;
        s21_determinant(&minor, &tmp_determinant);
        s21_remove_matrix(&minor);
        tmp_determinant *= pow(-1, 2 + i + j);
        result->matrix[i][j] = tmp_determinant;
      }
    }
  }
  return return_value;
}

void get_matrix(int i, int j, matrix_t A, matrix_t *result) {
  s21_create_matrix(A.rows - 1, A.columns - 1, result);
  int res_rows = 0, res_columns = 0;
  for (int r = 0; r < A.rows; r++) {
    if (r == i) continue;
    for (int c = 0; c < A.columns; c++) {
      if (c == j) continue;
      result->matrix[res_rows][res_columns] = A.matrix[r][c];
      res_columns++;
    }
    res_columns = 0;
    res_rows++;
  }
}

int s21_determinant(matrix_t *A, double *result) {
  int return_value = OK;
  if (!result || !matrix_check(A)) {
    return_value = INCORRECT_MATRIX;
  } else if (A->columns != A->rows) {
    return_value = INCORRECT_CALCULATION;
  } else {
    if (A->rows == 2) {
      *result =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    } else if (A->rows == 1) {
      *result = A->matrix[0][0];
    } else {
      *result = 0;
      for (int i = 0; i < A->columns; i++) {
        matrix_t minor;
        get_matrix(0, i, *A, &minor);
        double tmp_determinant;
        s21_determinant(&minor, &tmp_determinant);
        s21_remove_matrix(&minor);
        double tmp_res = pow(-1, 2 + i) * A->matrix[0][i] * tmp_determinant;
        *result += tmp_res;
      }
    }
  }
  return return_value;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int return_value = OK;
  if (!result || !matrix_check(A)) {
    return_value = INCORRECT_MATRIX;
  } else if (A->rows != A->columns) {
    return_value = INCORRECT_CALCULATION;
  } else {
    double determinant = 0;
    matrix_t tmp_matrix_1;
    matrix_t tmp_matrix_2;
    s21_determinant(A, &determinant);
    if (fabs(determinant) > 1e-7) {
      s21_calc_complements(A, &tmp_matrix_1);
      s21_transpose(&tmp_matrix_1, &tmp_matrix_2);
      s21_remove_matrix(&tmp_matrix_1);
      s21_mult_number(&tmp_matrix_2, 1.0 / determinant, result);
      s21_remove_matrix(&tmp_matrix_2);
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          result->matrix[i][j] *= -1;
        }
      }
    } else {
      return_value = INCORRECT_CALCULATION;
    }
  }
  return return_value;
}

int matrix_check(matrix_t *A) {
  int check = 1;
  if (!A || !A->matrix || A->rows <= 0 || A->columns <= 0) check = 0;
  return check;
}
