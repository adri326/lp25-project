#include <check.h>
#include <defs.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

char* read_test_file(const char* path);
bool compare_test_file(const char* expected_path, const char* actual_path);

#include "md5sum.c"
#include "tree.c"
#include "save.c"
#include "scan.c"

char* read_test_file(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    size_t length = (size_t)ftell(f);

    char* res = malloc(length + 1);
    fseek(f, 0, SEEK_SET);
    if (!fread(res, length, 1, f)) {
        free(res);
        return NULL;
    }
    res[length] = 0;
    fclose(f);

    return res;
}

bool compare_test_file(const char* expected_path, const char* actual_path) {
    char* expected = read_test_file(expected_path);
    if (!expected) return false;

    char* actual = read_test_file(actual_path);
    if (!actual) {
        free(expected);
        return false;
    }

    int res = strcmp(expected, actual);

    free(expected);
    free(actual);

    return res == 0;
}

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

    Suite* scan_s = scan_suite();
    SRunner* scan_sr = srunner_create(scan_s);

    srunner_run_all(md5sum_sr, CK_NORMAL);
    srunner_run_all(tree_sr, CK_NORMAL);
    srunner_run_all(save_sr, CK_NORMAL);
    srunner_run_all(scan_sr, CK_NORMAL);

    int fails = 0;
    fails += srunner_ntests_failed(md5sum_sr);
    fails += srunner_ntests_failed(tree_sr);
    fails += srunner_ntests_failed(save_sr);
    fails += srunner_ntests_failed(scan_sr);

    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
