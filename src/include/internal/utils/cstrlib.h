/*
 *
 *
 *
 *
 *
 */

#include <string.h>

/*
 * Erase the leading space and ending space, new line in string
 *
 *  @param[in/out]  str The pointer to the string
 *  @return The length of the string after chomp.
 */
int chomp(char *str) {
    int len = strlen(str);

    while (*(str + len - 1) == ' ' ||
            *(str + len - 1) == '\r' ||
            *(str + len - 1) == '\n') {
        str[-- len] = 0;
    }

    while (*str == ' ' ||
            *str == '\r' ||
            *str == '\n') {
        ++ str;
        -- len;
    }

    return len;
}

/*
 *
 *
 *
 *
 */
