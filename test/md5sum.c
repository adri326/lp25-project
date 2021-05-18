#include <stdlib.h>
#include <check.h>
#include <inttypes.h>
#include <stdbool.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <md5sum.h>

uint8_t* parse_md5sum(char* raw) {
    uint8_t* res = malloc(MD5_DIGEST_LENGTH);

    for (size_t n = 0; n < MD5_DIGEST_LENGTH; n++) {
        char str[3] = {raw[2*n], raw[2*n+1], 0};
        res[n] = (uint8_t)strtoul(str, NULL, 16);
    }

    return res;
}

bool buffer_compare(uint8_t buffer_a[], uint8_t buffer_b[]) {
    for (size_t n = 0; n < MD5_DIGEST_LENGTH; n++) {
        if (buffer_a[n] != buffer_b[n]) {
            printf("  ERROR: Buffers differ at byte %zu: %" PRIx8 " != %" PRIx8 "\n", n, buffer_a[n], buffer_b[n]);
            return false;
        }
    }
    return true;
}

// La définition d'un test (les macros rajoutent les trucs nécéssaires au début et à la fin).
// Ne pas oublier le END_TEST!!!1!1!
START_TEST(test_md5sum_one) {
    uint8_t buffer[MD5_DIGEST_LENGTH] = {0};
    uint8_t* buffer_expected = parse_md5sum("b026324c6904b2a9cb4b88d6d61c81d1");

    // La macro TEST_DIR contient l'adresse absolute du dossier test/ (définie dans le CMakeLists.txt)
    ck_assert_msg(compute_md5(TEST_DIR "/data/one.txt", buffer), "compute_md5 returned false!");

    ck_assert_msg(buffer_compare(buffer, buffer_expected), "one.txt yielded a different sum!");

    free(buffer_expected);
}
END_TEST

Suite* md5sum_suite() {
    // On créer une suite de tests
    Suite* res = suite_create("md5sum");
    // On crée un test case
    TCase* tc_basic = tcase_create("basic");
    // On rajoute quelque tests
    tcase_add_test(tc_basic, test_md5sum_one);

    // On rajoute le test case à la suite
    suite_add_tcase(res, tc_basic);

    return res;
}
