#include <check.h>
#include <defs.h>
#include "md5sum.c"
#include "tree.c"
#include "save.c"
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    DIR* out_dir = opendir(TEST_DIR "/out");
    if (out_dir) {
        closedir(out_dir);
        // Too lazy to do a proper rm -rf:
        system("rm -rf " TEST_DIR "/out");
    }

    mkdir(TEST_DIR "/out", 0777);

    Suite* md5sum_s = md5sum_suite();
    SRunner* md5sum_sr = srunner_create(md5sum_s);

    Suite* tree_s = tree_suite();
    SRunner* tree_sr = srunner_create(tree_s);

    Suite* save_s = save_suite();
    SRunner* save_sr = srunner_create(save_s);

    srunner_run_all(md5sum_sr, CK_NORMAL);
    srunner_run_all(tree_sr, CK_NORMAL);
    srunner_run_all(save_sr, CK_NORMAL);

    int fails = 0;
    fails += srunner_ntests_failed(md5sum_sr);
    fails += srunner_ntests_failed(tree_sr);
    fails += srunner_ntests_failed(save_sr);

    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
