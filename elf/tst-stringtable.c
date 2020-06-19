/* Unit test for ldconfig string tables.
   This file is part of the GNU C Library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <https://www.gnu.org/licenses/>.  */

#include <stdlib.h>
#include <string.h>
#include <stringtable.h>
#include <support/check.h>
#include <support/support.h>

static int
do_test (void)
{
  /* Empty string table.  */
  {
    struct stringtable s = { 0, };
    struct stringtable_finalized f;
    stringtable_finalize (&s, &f);
    TEST_COMPARE_STRING (f.strings, "");
    TEST_COMPARE (f.size, 0);
    free (f.strings);
    stringtable_free (&s);
  }

  /* String table with one empty string.  */
  {
    struct stringtable s = { 0, };
    struct stringtable_entry *e = stringtable_intern (&s, "");
    TEST_COMPARE_STRING (e->string, "");
    TEST_COMPARE (e->length, 0);
    TEST_COMPARE (s.count, 1);

    struct stringtable_finalized f;
    stringtable_finalize (&s, &f);
    TEST_COMPARE (e->offset, 0);
    TEST_COMPARE_STRING (f.strings, "");
    TEST_COMPARE (f.size, 1);
    free (f.strings);
    stringtable_free (&s);
  }

  /* String table with one non-empty string.  */
  {
    struct stringtable s = { 0, };
    struct stringtable_entry *e = stringtable_intern (&s, "name");
    TEST_COMPARE_STRING (e->string, "name");
    TEST_COMPARE (e->length, 4);
    TEST_COMPARE (s.count, 1);

    struct stringtable_finalized f;
    stringtable_finalize (&s, &f);
    TEST_COMPARE (e->offset, 0);
    TEST_COMPARE_STRING (f.strings, "name");
    TEST_COMPARE (f.size, 5);
    free (f.strings);
    stringtable_free (&s);
  }

  /* Two strings, one is a prefix of the other.  Tail-merging can only
     happen in one way in this case.  */
  {
    struct stringtable s = { 0, };
    struct stringtable_entry *suffix = stringtable_intern (&s, "suffix");
    TEST_COMPARE_STRING (suffix->string, "suffix");
    TEST_COMPARE (suffix->length, 6);
    TEST_COMPARE (s.count, 1);

    struct stringtable_entry *prefix
      = stringtable_intern (&s, "prefix-suffix");
    TEST_COMPARE_STRING (prefix->string, "prefix-suffix");
    TEST_COMPARE (prefix->length, strlen ("prefix-suffix"));
    TEST_COMPARE (s.count, 2);

    struct stringtable_finalized f;
    stringtable_finalize (&s, &f);
    TEST_COMPARE (prefix->offset, 0);
    TEST_COMPARE (suffix->offset, strlen ("prefix-"));
    TEST_COMPARE_STRING (f.strings, "prefix-suffix");
    TEST_COMPARE (f.size, sizeof ("prefix-suffix"));
    free (f.strings);
    stringtable_free (&s);
  }

  /* String table with various shared prefixes.  Triggers hash
     resizing.  */
  {
    enum { count = 1500 };
    char *strings[2 * count];
    struct stringtable_entry *entries[2 * count];
    struct stringtable s = { 0, };
    for (int i = 0; i < count; ++i)
      {
        strings[i] = xasprintf ("%d", i);
        entries[i] = stringtable_intern (&s, strings[i]);
        TEST_COMPARE (entries[i]->length, strlen (strings[i]));
        TEST_COMPARE_STRING (entries[i]->string, strings[i]);
        strings[i + count] = xasprintf ("prefix/%d", i);
        entries[i + count] = stringtable_intern (&s, strings[i + count]);
        TEST_COMPARE (entries[i + count]->length, strlen (strings[i + count]));
        TEST_COMPARE_STRING (entries[i + count]->string, strings[i + count]);
      }

    struct stringtable_finalized f;
    stringtable_finalize (&s, &f);

    for (int i = 0; i < 2 * count; ++i)
      {
        TEST_COMPARE (entries[i]->length, strlen (strings[i]));
        TEST_COMPARE_STRING (entries[i]->string, strings[i]);
        TEST_COMPARE_STRING (f.strings + entries[i]->offset, strings[i]);
        free (strings[i]);
      }

    free (f.strings);
    stringtable_free (&s);
  }

  return 0;
}

#include <support/test-driver.c>

/* Re-compile the string table implementation here.  It is not
   possible to link against the actual build because it was built for
   use in ldconfig.  */
#define _(arg) arg
#include "stringtable.c"
#include "stringtable_free.c"
