#include <check.h>
#include <defs.h>
#include "md5sum.c"
#include "tree.c"

int main(int argc, char* argv[]) {
    Suite* md5sum_s = md5sum_suite();
    SRunner* md5sum_sr = srunner_create(md5sum_s);

    Suite* tree_s = tree_suite();
    SRunner* tree_sr = srunner_create(tree_s);

    srunner_run_all(md5sum_sr, CK_NORMAL);
    srunner_run_all(tree_sr, CK_NORMAL);

    int fails = 0;
    fails += srunner_ntests_failed(md5sum_sr);
    fails += srunner_ntests_failed(tree_sr);

    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
