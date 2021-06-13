#include <stdlib.h>
#include <check.h>
#include <defs.h>
#include <save.h>
#include <scan.h>

START_TEST(test_save_artificial) {
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
    file1.file_type = OTHER_TYPE;

    dir1.files = &file1;

    file_t file2 = {0};
    strcpy(file2.name, "file2");
    file2.mod_time = 11;
    file2.file_size = 32;
    file2.file_type = REGULAR_FILE;
    file2.md5sum[0] = 5;
    file2.md5sum[1] = 19; // Expected is 0513000000...

    file1.next_file = &file2;

    file_t file3 = {0};
    strcpy(file3.name, "file3");
    file3.mod_time = 20;
    file3.file_type = REGULAR_FILE;
    file3.md5sum[0] = 10; // Expected is 0a00000...

    subdir1.files = &file3;

    save_to_file(&dir1, TEST_DIR "/out/savetest.txt", "dir1", false);

    ck_assert(compare_test_file(TEST_DIR "/data/savetest-expected.txt", TEST_DIR "/out/savetest.txt"));
}
END_TEST

START_TEST(test_save_scanned) {
    directory_t* dir = scan_dir(TEST_DIR "/data/simple", true, false);
    ck_assert(save_to_file(dir, TEST_DIR "/out/save_simple.txt", "data/simple", false));
    free(dir);

    dir = scan_dir(TEST_DIR "/data/subdir", true, false);
    ck_assert(save_to_file(dir, TEST_DIR "/out/save_subdir.txt", "data/subdir", false));
    free(dir);

    dir = scan_dir(TEST_DIR "/data", true, false);
    ck_assert(save_to_file(dir, TEST_DIR "/out/save_data.txt", "data", false));
    free(dir);
}
END_TEST

Suite* save_suite() {
    Suite* res = suite_create("save");

    TCase* tc_basic = tcase_create("basic");

    tcase_add_test(tc_basic, test_save_artificial);
    tcase_add_test(tc_basic, test_save_scanned);

    suite_add_tcase(res, tc_basic);

    return res;
}
