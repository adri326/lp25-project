#include <check.h>
#include <defs.h>
#include "md5sum.c"

int main(int argc, char* argv[]) {
    Suite* md5sum_s = md5sum_suite();
    SRunner* md5sum_sr = srunner_create(md5sum_s);

    srunner_run_all(md5sum_sr, CK_NORMAL);

    int fails = 0;
    fails += srunner_ntests_failed(md5sum_sr);

    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
