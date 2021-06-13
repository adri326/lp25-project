#include <stdlib.h>
#include <check.h>
#include <defs.h>
#include <scan.h>
#include <stdbool.h>

START_TEST(test_scan_dir_simple) {
    directory_t* dir = scan_dir(TEST_DIR "/data/simple", true, false);

    ck_assert(dir != NULL);

    // Directory "simple"
    ck_assert_str_eq(dir->name, "simple");
    ck_assert(dir->files != NULL);
    ck_assert(dir->next_dir == NULL);
    ck_assert(dir->subdirs == NULL);

    // File "hello.txt"
    file_t* hello = dir->files;
    ck_assert_str_eq(hello->name, "hello.txt");
    ck_assert_uint_eq(hello->file_size, 7);

    uint8_t* md5_expected = parse_md5sum("e134ced312b3511d88943d57ccd70c83");
    ck_assert_msg(buffer_compare(hello->md5sum, md5_expected), "hello.txt yielded a different sum!");
    free(md5_expected);

    ck_assert(hello->next_file == NULL);
    ck_assert(hello->file_type == REGULAR_FILE);

    free_dir(dir);
}
END_TEST

START_TEST(test_scan_subdir_simple) {
    directory_t* dir = scan_dir(TEST_DIR "/data/subdir", true, false);

    ck_assert(dir != NULL);

    // Directory "subdir"
    ck_assert_str_eq(dir->name, "subdir");
    ck_assert(dir->files != NULL);
    ck_assert(dir->next_dir == NULL);
    ck_assert(dir->subdirs != NULL);

    // Directory "a"
    directory_t* a = dir->subdirs;
    ck_assert_str_eq(a->name, "a");
    ck_assert(a->subdirs == NULL);
    ck_assert(a->files != NULL);
    ck_assert(a->next_dir == NULL);

    // File "b.txt"
    file_t* b = dir->subdirs->files;
    ck_assert_str_eq(b->name, "b.txt");
    ck_assert_uint_eq(b->file_size, 8);

    uint8_t* md5_expected = parse_md5sum("31142af9a5c9f489c377946952bfe93b");
    ck_assert_msg(buffer_compare(b->md5sum, md5_expected), "b.txt yielded a different sum!");
    free(md5_expected);

    ck_assert(b->next_file == NULL);
    ck_assert(b->file_type == REGULAR_FILE);

    // File "c.txt"
    file_t* c = dir->files;
    ck_assert_str_eq(c->name, "c.txt");
    ck_assert_uint_eq(c->file_size, 8);

    md5_expected = parse_md5sum("ed5497285b3406740f54adb17b2da78d");
    ck_assert_msg(buffer_compare(c->md5sum, md5_expected), "c.txt yielded a different sum!");
    free(md5_expected);

    ck_assert(c->next_file == NULL);
    ck_assert(c->file_type == REGULAR_FILE);
}

Suite* scan_suite() {
    Suite* res = suite_create("scan");

    TCase* tc_basic = tcase_create("basic");

    tcase_add_test(tc_basic, test_scan_dir_simple);
    tcase_add_test(tc_basic, test_scan_subdir_simple);

    suite_add_tcase(res, tc_basic);

    return res;
}
