static int o, x;
void *arr[] = {
  &o, &o, &o, &o, &o, &o, &o, &o, &o, &o, &o, &o, &o, &o, &o, &o,
  0,
  &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x,
};

static int
do_test (void)
{
  for (int i = 0; i < 16; i++)
    if (arr[i] != &o)
      return 1;
  for (int i = 17; i < 33; i++)
    if (arr[i] != &x)
      return 1;
  return 0;
}

#include <support/test-driver.c>
