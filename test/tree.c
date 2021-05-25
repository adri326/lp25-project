#include <stdlib.h>
#include <check.h>
#include <defs.h>
#include <tree.h>

START_TEST(test_tree_append_file) {
    file_t a = {
        .name = "file a",
        .next_file = NULL
    };
    file_t b = {
        .name = "file b",
        .next_file = NULL
    };
    directory_t dir = {
        .files = NULL
    };

    append_file(&a, &dir);

    ck_assert(dir.files != NULL);
    ck_assert_str_eq(dir.files->name, a.name);
    ck_assert(dir.files->next_file == NULL);

    append_file(&b, &dir);
    ck_assert(dir.files != NULL);
    ck_assert_str_eq(dir.files->name, a.name);
    ck_assert_str_eq((dir.files->next_file)->name, b.name);
    ck_assert((dir.files->next_file)->next_file == NULL);
}
END_TEST

START_TEST(test_tree_append_subdir) {
    directory_t a = {
        .name = "file a",
        .next_dir = NULL
    };
    directory_t b = {
        .name = "file b",
        .next_dir = NULL
    };
    directory_t dir = {
        .files = NULL
    };

    append_subdir(&a, &dir);

    ck_assert(dir.subdirs != NULL);
    ck_assert_str_eq(dir.subdirs->name, a.name);
    ck_assert(dir.subdirs->next_dir == NULL);

    append_subdir(&b, &dir);
    ck_assert(dir.subdirs != NULL);
    ck_assert_str_eq(dir.subdirs->name, a.name);
    ck_assert_str_eq((dir.subdirs->next_dir)->name, b.name);
    ck_assert((dir.subdirs->next_dir)->next_dir == NULL);
}

Suite* tree_suite() {
    Suite* res = suite_create("tree");

    TCase* tc_basic = tcase_create("basic");

    tcase_add_test(tc_basic, test_tree_append_file);
    tcase_add_test(tc_basic, test_tree_append_subdir);


    suite_add_tcase(res, tc_basic);

    return res;
}
