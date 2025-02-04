#include "test.h"

int main(void) {
  int failed = 0, total = 0;
  Suite *s21_decimal_test[] = {test_s21_convertors_test(),
                               test_s21_arithmetic_suite(),
                               test_s21_other_tests(), NULL};
  int all_tests = 0;
  for (; s21_decimal_test[all_tests] != NULL; all_tests++) {
    SRunner *sr = srunner_create(s21_decimal_test[all_tests]);
    int a = 0;
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    if ((a = srunner_ntests_failed(sr)) != 0) {
      failed += a;
      printf(RED "=============== FAILED ================\n" NOCOLOR);
    } else
      printf(GREEN "=============== SUCCESS ===============\n" NOCOLOR);

    total += srunner_ntests_run(sr);
    srunner_free(sr);
  }
  printf("\nTEST RESULT " GREEN "\nTOTAL:\t%d" RED "\nFAILED:\t"
         "%d" NOCOLOR "\n",
         total, failed);
  return failed == 0 ? 0 : 1;
}
