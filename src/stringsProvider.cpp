#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/misc.h>
#include <fxcg/app.h>
#include <fxcg/serial.h>
#include <fxcg/rtc.h>
#include <fxcg/heap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "stringsProvider.hpp"

char tolower(char c)
{
  if (c >= 'A' && c <= 'Z')
          c += 32;
  return c;
}
int strncasecmp(const char *s1, const char *s2, size_t n)
{
        if (n != 0) {
                const unsigned char *us1 = (const unsigned char *)s1;
                const unsigned char *us2 = (const unsigned char *)s2;

                do {
                        if (tolower(*us1) != tolower(*us2++))
                                return (tolower(*us1) - tolower(*--us2));
                        if (*us1++ == '\0')
                                break;
                } while (--n != 0);
        }
        return (0);
}
char *strcasestr(const char *s, const char *find)
{
    char c;

    if ((c = *find++) != 0) {
            c = tolower((unsigned char)c);
            size_t len = strlen(find);
            do {
                    char sc;
                    do {
                            if ((sc = *s++) == 0)
                                    return (NULL);
                    } while ((char)tolower((unsigned char)sc) != c);
            } while (strncasecmp(s, find, len) != 0);
            s--;
    }
    return ((char *)s);
}


/* copy over the next token from an input string, after
skipping leading blanks (or other whitespace?). The
token is terminated by the first appearance of tokchar,
or by the end of the source string.

The caller must supply sufficient space in token to
receive any token, Otherwise tokens will be truncated.

Returns: a pointer past the terminating tokchar.

This will happily return an infinity of empty tokens if
called with src pointing to the end of a string. Tokens
will never include a copy of tokchar.

A better name would be "strtkn", except that is reserved
for the system namespace. Change to that at your risk.

released to Public Domain, by C.B. Falconer.
Published 2006-02-20. Attribution appreciated.
*/

unsigned char *toksplit(unsigned char *src, /* Source of tokens */
char tokchar, /* token delimiting char */
unsigned char *token, /* receiver of parsed token */
int lgh) /* length token can receive */
/* not including final '\0' */
{
  if (src) {
    //while (' ' == *src) *src++;
    while (' ' == *src) *src=*src+1; // avoids a compiler warning

    while (*src && (tokchar != *src)) {
      if (lgh) {
        *token++ = *src;
        --lgh;
      }
      src++;
    }
    if (*src && (tokchar == *src)) src++;
  }
  *token = '\0';
  return src;
} /* toksplit */


int EndsIWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    //return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
    return strncasecmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}