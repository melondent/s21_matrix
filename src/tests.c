#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

START_TEST(s21_create_matrix_1) {
  matrix_t A;
  int result = s21_create_matrix(5, 50, &A);
  ck_assert_int_eq(A.rows, 5);
  ck_assert_int_eq(A.columns, 50);
  ck_assert_int_eq(result, OK);
  s21_remove_matrix(&A);

  result = s21_create_matrix(0, 50, &A);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

START_TEST(s21_create_matrix_2) {
  matrix_t A;
  int rows = 2, columns = 3;
  int result = s21_create_matrix(rows, columns, &A);
  for (int i = 0; i < A.rows; i++)
    for (int j = 0; j < A.columns; j++) ck_assert_int_eq(A.matrix[i][j], 0.0);
  ck_assert_int_eq(result, OK);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_eq_matrix_1) {
  matrix_t A;
  matrix_t B;
  int A_columns;
  int A_rows;
  int B_columns;
  int B_rows;

  A_columns = B_columns = 5;
  A_rows = B_rows = 6;

  s21_create_matrix(A_rows, A_columns, &A);
  s21_create_matrix(B_rows, B_columns, &B);

  A.matrix[0][0] = 1;
  B.matrix[0][0] = 1;

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  A_columns = 2;
  B_columns = 4;
  A_rows = 8;
  B_rows = 16;

  s21_create_matrix(A_rows, A_columns, &A);
  s21_create_matrix(B_rows, B_columns, &B);

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_eq_matrix_2) {
  matrix_t A;
  matrix_t B;
  int A_columns;
  int A_rows;
  int B_columns;
  int B_rows;

  A_columns = B_columns = 5;
  A_rows = B_rows = 6;

  s21_create_matrix(A_rows, A_columns, &A);
  s21_create_matrix(B_rows, B_columns, &B);

  A.matrix[0][0] = 1;
  B.matrix[0][0] = 1.0000003;

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(s21_sum_matrix_1) {
  matrix_t matrix1, matrix2, result_matrix;
  s21_create_matrix(3, 3, &matrix1);
  s21_create_matrix(3, 3, &matrix2);
  int counter = 0;
  for (int i = 0; i < matrix1.rows; i++) {
    for (int j = 0; j < matrix1.columns; j++) {
      matrix1.matrix[i][j] = counter++;
    }
  }
  counter = -358;
  for (int i = 0; i < matrix2.rows; i++) {
    for (int j = 0; j < matrix2.columns; j++) {
      matrix2.matrix[i][j] = counter++;
    }
  }
  int result = s21_sum_matrix(&matrix2, &matrix1, &result_matrix);
  for (int i = 0; i < matrix2.rows; i++) {
    for (int j = 0; j < matrix2.columns; j++) {
      ck_assert_int_eq(result_matrix.matrix[i][j],
                       matrix2.matrix[i][j] + matrix1.matrix[i][j]);
    }
  }
  ck_assert_int_eq(result, OK);
  s21_remove_matrix(&result_matrix);
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&matrix2);
}
END_TEST

START_TEST(s21_sum_matrix_2) {
  matrix_t matrix1, matrix2, result_matrix;
  s21_create_matrix(2, 2, &matrix1);
  s21_create_matrix(3, 3, &matrix2);
  s21_create_matrix(3, 3, &result_matrix);
  int result = s21_sum_matrix(&matrix2, &matrix1, &result_matrix);
  ck_assert_int_eq(result, INCORRECT_CALCULATION);
  s21_remove_matrix(&result_matrix);
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&matrix2);
}
END_TEST

START_TEST(s21_sub_matrix_1) {
  matrix_t matrix1, matrix2, result;
  s21_create_matrix(3, 3, &matrix1);
  s21_create_matrix(3, 3, &matrix2);
  s21_sub_matrix(&matrix2, &matrix1, &result);
  for (int i = 0; i < matrix2.rows; i++) {
    for (int j = 0; j < matrix2.columns; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j],
                              matrix2.matrix[i][j] - matrix1.matrix[i][j],
                              1e-6);
    }
  }
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&matrix2);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(s21_sub_matrix_2) {
  matrix_t matrix1, matrix2, result_matrix;
  s21_create_matrix(2, 2, &matrix1);
  s21_create_matrix(3, 3, &matrix2);
  s21_create_matrix(3, 3, &result_matrix);
  int result = s21_sub_matrix(&matrix2, &matrix1, &result_matrix);
  ck_assert_int_eq(result, INCORRECT_CALCULATION);
  s21_remove_matrix(&result_matrix);
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&matrix2);
}
END_TEST

START_TEST(s21_mult_1) {
  matrix_t matrix;
  matrix_t matrix2;
  matrix_t matrix3;

  s21_create_matrix(2, 2, &matrix);
  s21_create_matrix(2, 2, &matrix3);

  int counter = 1;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      matrix.matrix[i][j] = counter++;
    }
  }

  int result = s21_mult_number(&matrix, 5, &matrix2);

  ck_assert_int_eq(matrix2.matrix[0][0], 5);
  ck_assert_int_eq(matrix2.matrix[0][1], 10);
  ck_assert_int_eq(matrix2.matrix[1][0], 15);
  ck_assert_int_eq(matrix2.matrix[1][1], 20);
  ck_assert_int_eq(result, OK);
  s21_remove_matrix(&matrix2);

  result = s21_mult_number(&matrix3, 5, &matrix2);
  ck_assert_int_eq(result, OK);
  s21_remove_matrix(&matrix2);

  s21_remove_matrix(&matrix);
  // s21_remove_matrix(&matrix2);
  s21_remove_matrix(&matrix3);
}
END_TEST

START_TEST(s21_mult_2) {
  matrix_t matrix1;
  matrix_t matrix2;
  matrix_t result;

  s21_create_matrix(2, 2, &matrix1);
  s21_create_matrix(2, 2, &matrix2);

  matrix1.matrix[0][0] = 2.35;
  matrix1.matrix[0][1] = 2.45;
  matrix1.matrix[1][0] = 345.25;
  matrix1.matrix[1][1] = 88.569;

  matrix2.matrix[0][0] = 456.324;
  matrix2.matrix[0][1] = 7895.687;
  matrix2.matrix[1][0] = 1.569874;
  matrix2.matrix[1][1] = 2.3568;

  s21_mult_matrix(&matrix1, &matrix2, &result);

  ck_assert_double_eq(result.matrix[0][0], 1076.2075913);
  ck_assert_double_eq(result.matrix[0][1], 18560.63861);
  ck_assert_double_eq(result.matrix[1][0], 157684.903170306);
  ck_assert_double_eq(result.matrix[1][1], 2726194.6761692);

  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&matrix2);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(s21_transpose_1) {
  matrix_t matrix1, matrix2, result;
  s21_create_matrix(3, 3, &matrix1);
  s21_create_matrix(3, 3, &matrix2);
  int counter = 0;
  for (int i = 0; i < matrix1.rows; i++) {
    for (int j = 0; j < matrix1.columns; j++) {
      matrix1.matrix[i][j] = counter++;
    }
  }
  matrix2.matrix[0][0] = 0;
  matrix2.matrix[0][1] = 3;
  matrix2.matrix[0][2] = 6;
  matrix2.matrix[1][0] = 1;
  matrix2.matrix[1][1] = 4;
  matrix2.matrix[1][2] = 7;
  matrix2.matrix[2][0] = 2;
  matrix2.matrix[2][1] = 5;
  matrix2.matrix[2][2] = 8;

  s21_transpose(&matrix1, &result);

  for (int i = 0; i < result.rows; i++) {
    for (int j = 0; j < result.columns; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j], matrix2.matrix[i][j], 1e-6);
    }
  }

  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&matrix2);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(s21_transpose_2) {
  matrix_t matrix1 = {NULL, 0, 0};
  matrix_t result;
  s21_create_matrix(3, 2, &result);
  int return_value = s21_transpose(&matrix1, &result);
  ck_assert_int_eq(return_value, INCORRECT_MATRIX);
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(s21_calc_complements_1) {
  matrix_t matrix1, result;
  s21_create_matrix(3, 2, &matrix1);
  int k = 0;
  for (int i = 0; i < matrix1.rows; i++) {
    for (int j = 0; j < matrix1.columns; j++) {
      matrix1.matrix[i][j] = k++;
    }
  }
  int return_value = s21_calc_complements(&matrix1, &result);
  ck_assert_int_eq(return_value, INCORRECT_CALCULATION);
  s21_remove_matrix(&matrix1);

  s21_create_matrix(3, 3, &matrix1);
  k = 0;
  for (int i = 0; i < matrix1.rows; i++) {
    for (int j = 0; j < matrix1.columns; j++) {
      matrix1.matrix[i][j] = k++;
    }
  }
  return_value = s21_calc_complements(&matrix1, &result);
  ck_assert_int_eq(return_value, OK);
  s21_remove_matrix(&matrix1);

  s21_remove_matrix(&result);
  s21_create_matrix(3, 3, &matrix1);
  matrix1.matrix[0][0] = 1;
  matrix1.matrix[0][1] = 2;
  matrix1.matrix[0][2] = 3;

  matrix1.matrix[1][0] = 0;
  matrix1.matrix[1][1] = 4;
  matrix1.matrix[1][2] = 2;

  matrix1.matrix[2][0] = 5;
  matrix1.matrix[2][1] = 2;
  matrix1.matrix[2][2] = 1;
  s21_calc_complements(&matrix1, &result);
  matrix_t expected;
  s21_create_matrix(3, 3, &expected);
  expected.matrix[0][0] = 0;
  expected.matrix[0][1] = 10;
  expected.matrix[0][2] = -20;

  expected.matrix[1][0] = 4;
  expected.matrix[1][1] = -14;
  expected.matrix[1][2] = 8;

  expected.matrix[2][0] = -8;
  expected.matrix[2][1] = -2;
  expected.matrix[2][2] = 4;
  for (int i = 0; i < result.rows; i++) {
    for (int j = 0; j < result.columns; j++) {
      ck_assert_double_eq(result.matrix[i][j], expected.matrix[i][j]);
    }
  }
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);

  matrix_t wrong;
  s21_create_matrix(2, 3, &matrix1);
  return_value = s21_calc_complements(&matrix1, &wrong);
  ck_assert_int_eq(return_value, 2);
  s21_remove_matrix(&matrix1);
}
END_TEST

START_TEST(s21_calc_complements_2) {
  matrix_t wrong = {NULL, 0, 0};
  matrix_t matrix1;
  s21_create_matrix(3, 3, &matrix1);
  int return_value = s21_calc_complements(&wrong, &matrix1);
  ck_assert_int_eq(return_value, INCORRECT_MATRIX);
  s21_remove_matrix(&matrix1);
}
END_TEST

START_TEST(s21_inverse_1) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;

  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;

  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;

  matrix_t expected_result;
  s21_create_matrix(3, 3, &expected_result);

  expected_result.matrix[0][0] = -1;
  expected_result.matrix[0][1] = 1;
  expected_result.matrix[0][2] = -1;

  expected_result.matrix[1][0] = 38;
  expected_result.matrix[1][1] = -41;
  expected_result.matrix[1][2] = 34;

  expected_result.matrix[2][0] = -27;
  expected_result.matrix[2][1] = 29;
  expected_result.matrix[2][2] = -24;

  matrix_t actual_result;
  s21_inverse_matrix(&A, &actual_result);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq(expected_result.matrix[i][j],
                          actual_result.matrix[i][j]);
    }
  }
  s21_remove_matrix(&expected_result);
  s21_remove_matrix(&actual_result);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(s21_inverse_2) {
  matrix_t matrix1 = {NULL, 0, 0};
  matrix_t result;
  s21_create_matrix(3, 3, &result);
  int return_value = s21_inverse_matrix(&matrix1, &result);
  ck_assert_int_eq(return_value, INCORRECT_MATRIX);
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&result);

  s21_create_matrix(3, 2, &matrix1);
  s21_create_matrix(3, 2, &result);
  return_value = s21_inverse_matrix(&matrix1, &result);
  ck_assert_int_eq(return_value, INCORRECT_CALCULATION);
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(s21_determinant_1) {
  matrix_t matrix1;
  s21_create_matrix(3, 2, &matrix1);
  int k = 0;
  for (int i = 0; i < matrix1.rows; i++) {
    for (int j = 0; j < matrix1.columns; j++) {
      matrix1.matrix[i][j] = k++;
    }
  }
  double result;
  int return_value = s21_determinant(&matrix1, &result);
  ck_assert_int_eq(return_value, 2);
  s21_remove_matrix(&matrix1);

  s21_create_matrix(3, 3, &matrix1);
  k = 0;
  for (int i = 0; i < matrix1.rows; i++) {
    for (int j = 0; j < matrix1.columns; j++) {
      matrix1.matrix[i][j] = k++;
    }
  }
  return_value = s21_determinant(&matrix1, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_double_eq(result, 0);
  s21_remove_matrix(&matrix1);

  s21_create_matrix(3, 3, &matrix1);
  matrix1.matrix[0][0] = 2;
  matrix1.matrix[0][1] = 5;
  matrix1.matrix[0][2] = 5;

  matrix1.matrix[1][0] = 6;
  matrix1.matrix[1][1] = 3;
  matrix1.matrix[1][2] = 7;

  matrix1.matrix[2][0] = 79;
  matrix1.matrix[2][1] = -88;
  matrix1.matrix[2][2] = 88;
  return_value = s21_determinant(&matrix1, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_double_eq(result, -1940);
  s21_remove_matrix(&matrix1);
}
END_TEST

START_TEST(s21_determinant_2) {
  matrix_t matrix1 = {NULL, 0, 0};
  double result;
  int return_value = s21_determinant(&matrix1, &result);
  ck_assert_int_eq(return_value, INCORRECT_MATRIX);
  s21_remove_matrix(&matrix1);
}
END_TEST

START_TEST(s21_determinant_3) {
  matrix_t matrix1;
  s21_create_matrix(1, 1, &matrix1);
  matrix1.matrix[0][0] = 7;
  double result;
  int return_value = s21_determinant(&matrix1, &result);
  ck_assert_int_eq(result, 7);
  ck_assert_int_eq(return_value, OK);
  s21_remove_matrix(&matrix1);
}
END_TEST

Suite *lib_suite(void) {
  Suite *s;
  s = suite_create("Check");

  TCase *tc_create_matrix_1;
  tc_create_matrix_1 = tcase_create("s21_create_matrix_1");
  suite_add_tcase(s, tc_create_matrix_1);
  tcase_add_test(tc_create_matrix_1, s21_create_matrix_1);

  TCase *tc_create_matrix_2;
  tc_create_matrix_2 = tcase_create("s21_create_matrix_2");
  suite_add_tcase(s, tc_create_matrix_2);
  tcase_add_test(tc_create_matrix_2, s21_create_matrix_2);

  TCase *tc_eq_matrix_1;
  tc_eq_matrix_1 = tcase_create("s21_eq_matrix_1");
  suite_add_tcase(s, tc_eq_matrix_1);
  tcase_add_test(tc_eq_matrix_1, s21_eq_matrix_1);

  TCase *tc_eq_matrix_2;
  tc_eq_matrix_2 = tcase_create("s21_eq_matrix_2");
  suite_add_tcase(s, tc_eq_matrix_2);
  tcase_add_test(tc_eq_matrix_2, s21_eq_matrix_2);

  TCase *tc_sum_matrix_1;
  tc_sum_matrix_1 = tcase_create("s21_sum_matrix_1");
  suite_add_tcase(s, tc_sum_matrix_1);
  tcase_add_test(tc_sum_matrix_1, s21_sum_matrix_1);

  TCase *tc_sum_matrix_2;
  tc_sum_matrix_2 = tcase_create("s21_sum_matrix_2");
  suite_add_tcase(s, tc_sum_matrix_2);
  tcase_add_test(tc_sum_matrix_2, s21_sum_matrix_2);

  TCase *tc_sub_matrix_1;
  tc_sub_matrix_1 = tcase_create("s21_sub_matrix_1");
  suite_add_tcase(s, tc_sub_matrix_1);
  tcase_add_test(tc_sub_matrix_1, s21_sub_matrix_1);

  TCase *tc_sub_matrix_2;
  tc_sub_matrix_2 = tcase_create("s21_sub_matrix_2");
  suite_add_tcase(s, tc_sub_matrix_2);
  tcase_add_test(tc_sub_matrix_2, s21_sub_matrix_2);

  TCase *tc_mult_1;
  tc_mult_1 = tcase_create("s21_mult_1");
  suite_add_tcase(s, tc_mult_1);
  tcase_add_test(tc_mult_1, s21_mult_1);

  TCase *tc_mult_2;
  tc_mult_2 = tcase_create("s21_mult_2");
  suite_add_tcase(s, tc_mult_2);
  tcase_add_test(tc_mult_2, s21_mult_2);

  TCase *tc_transpose_1;
  tc_transpose_1 = tcase_create("s21_transpose_1");
  suite_add_tcase(s, tc_transpose_1);
  tcase_add_test(tc_transpose_1, s21_transpose_1);

  TCase *tc_transpose_2;
  tc_transpose_2 = tcase_create("s21_transpose_2");
  suite_add_tcase(s, tc_transpose_2);
  tcase_add_test(tc_transpose_2, s21_transpose_2);

  TCase *tc_calc_complements_1;
  tc_calc_complements_1 = tcase_create("s21_calc_complements_1");
  suite_add_tcase(s, tc_calc_complements_1);
  tcase_add_test(tc_calc_complements_1, s21_calc_complements_1);

  TCase *tc_calc_complements_2;
  tc_calc_complements_2 = tcase_create("s21_calc_complements_2");
  suite_add_tcase(s, tc_calc_complements_2);
  tcase_add_test(tc_calc_complements_2, s21_calc_complements_2);

  TCase *tc_determinant_1;
  tc_determinant_1 = tcase_create("s21_determinant_1");
  suite_add_tcase(s, tc_determinant_1);
  tcase_add_test(tc_determinant_1, s21_determinant_1);

  TCase *tc_determinant_2;
  tc_determinant_2 = tcase_create("s21_determinant_2");
  suite_add_tcase(s, tc_determinant_2);
  tcase_add_test(tc_determinant_2, s21_determinant_2);

  TCase *tc_determinant_3;
  tc_determinant_3 = tcase_create("s21_determinant_3");
  suite_add_tcase(s, tc_determinant_3);
  tcase_add_test(tc_determinant_3, s21_determinant_3);

  TCase *tc_inverse_1;
  tc_inverse_1 = tcase_create("s21_inverse_1");
  suite_add_tcase(s, tc_inverse_1);
  tcase_add_test(tc_inverse_1, s21_inverse_1);

  TCase *tc_inverse_2;
  tc_inverse_2 = tcase_create("s21_inverse_2");
  suite_add_tcase(s, tc_inverse_2);
  tcase_add_test(tc_inverse_2, s21_inverse_2);

  return s;
}

int main(void) {
  Suite *s;
  SRunner *sr;

  s = lib_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}
