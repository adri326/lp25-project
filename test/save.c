#include <stdlib.h>
#include <check.h>
#include <defs.h>
#include <save.h>

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

START_TEST(test_save_subdirs) {
    directory_t dir1 = {0};
    strcpy(dir1.name, "dir1");
    dir1.mod_time = 100;

    directory_t subdir1 = {0};
    strcpy(subdir1.name, "subdir1");
    subdir1.mod_time = 200;

    dir1.subdirs = &subdir1;

    directory_t subdir2 = {0};
    strcpy(subdir2.name, "subdir2");
    subdir2.mod_time = 201;

    subdir1.next_dir = &subdir2;


    file_t file1 = {0};
    strcpy(file1.name, "file1");
    file1.mod_time = 10;
    file1.md5sum[0] = 16; // Expected is 01000000...

    dir1.files = &file1;

    file_t file2 = {0};
    strcpy(file2.name, "file2");
    file2.mod_time = 11;
    file2.md5sum[0] = 5;
    file2.md5sum[1] = 19; // Expected is 5031000000...

    file2.next_file = &file2;

    file_t file3 = {0};
    strcpy(file3.name, "file3");
    file3.mod_time = 20;
    file3.md5sum[0] = 15; // Expected is A000000...

    subdir1.files = &file3;

    save_to_file(&dir1, TEST_DIR "/out/savetest.txt", 0, "dir1");

    ck_assert(compare_test_file(TEST_DIR "/data/savetest-expected.txt", TEST_DIR "/out/savetest.txt"));
}
END_TEST


Suite* save_suite() {
    Suite* res = suite_create("save");

    TCase* tc_basic = tcase_create("basic");

    tcase_add_test(tc_basic, test_save_subdirs);


    suite_add_tcase(res, tc_basic);

    return res;
}
