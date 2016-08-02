/*
uebersetzt mit
 - safec (https://sourceforge.net/p/safeclib/wiki/Home/) und
 - glib2 installiert

Aufruf:
gcc -I/usr/local/include/libsafec/ -L/usr/local/lib64 -lsafec-1.0 `pkg-config --cflags --libs glib-2.0` -Wall --std=c11 main.c && ./a.out

Output:
Copying '1234567890' to a new string destination of 5 byte length
=================================================================


Checking results of 'strcpy' function
-------------------------------------------
destination content: >>>1234567890<<<
destination length via strlen: 10
Check for NULL byte at index 4: FAIL


Checking results of 'strncpy' function
-------------------------------------------
destination content: >>>12345<<<
destination length via strlen: 5
Check for NULL byte at index 4: FAIL


Checking results of 'strlcpy' function
-------------------------------------------
destination content: >>>1234<<<
destination length via strlen: 4
Check for NULL byte at index 4: OK


Checking results of 'strcpy_s' function
-------------------------------------------
destination content: >>><<<
error code: 406
destination length via strlen: 0
Check for NULL byte at index 4: FAIL


Checking results of 'strcpy_s' function
-------------------------------------------
destination content: >>>1234<<<
error code: 0
destination length via strlen: 4
Check for NULL byte at index 4: OK
*/

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <safe_lib.h>

const char* LONG_STRING = "1234567890";
const errno_t NO_ERROR_CODE = -4242;
#define DESTINATION_LENGTH 5
const int EXPECTED_NULL_BYTE_INDEX = 4;

void print_header();
void check_x_byte_is_null(const char* string);
void check_result(const char* prefix, const char* string, errno_t error_code);

int main(){
    print_header();

    char destination_one[DESTINATION_LENGTH] = {0};
    strcpy(destination_one, LONG_STRING);
    check_result("strcpy", destination_one, NO_ERROR_CODE);

    char destination_two[DESTINATION_LENGTH] = {0};
    strncpy(destination_two, LONG_STRING, sizeof(destination_two));
    check_result("strncpy", destination_two, NO_ERROR_CODE);

    char destination_three[DESTINATION_LENGTH] = {0};
    g_strlcpy(destination_three, LONG_STRING, sizeof(destination_three));
    check_result("strlcpy", destination_three, NO_ERROR_CODE);

    char destination_four[DESTINATION_LENGTH] = {0};
    errno_t error_code = strcpy_s(destination_four, sizeof(destination_four), LONG_STRING);
    check_result("strcpy_s", destination_four, error_code);

    char destination_five[DESTINATION_LENGTH] = {0};
    error_code = strcpy_s(destination_five, sizeof(destination_five), "1234");
    check_result("strcpy_s", destination_five, error_code);
}

// ------------------------------------

void print_header() {
  printf("\nCopying '%s' to a new string destination of %i byte length\n", LONG_STRING, DESTINATION_LENGTH);
  puts("=================================================================");
  puts("");
}

void check_result(const char* prefix, const char* string, errno_t error_code) {
    printf("\nChecking results of '%s' function\n", prefix);
    printf("-------------------------------------------\n");
    printf("destination content: >>>%s<<<\n", string);

    if (error_code != NO_ERROR_CODE)
        printf("error code: %i\n", error_code);

    printf("destination length via strlen: %zu\n", strlen(string));
    check_x_byte_is_null(string);
    printf("\n");
}

void check_x_byte_is_null(const char* string) {
    printf("Check for NULL byte at index %i: ", EXPECTED_NULL_BYTE_INDEX);

    if (string[EXPECTED_NULL_BYTE_INDEX] != '\0') {
        printf("FAIL\n");
    }
    else {
        printf("OK\n");
    }
}
