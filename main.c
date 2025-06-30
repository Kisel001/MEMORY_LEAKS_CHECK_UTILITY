//
// main.c
//
//      Copyright (C) Kisel Studios. All rights reserved.
//
// Module for testing memory leaks utility.
//
// Date: 30.06.2025
//

#include "memory_leaks_utliity.h"

// Main function.
// ARGUMENTS: None.
// RETURNS:
//   int - return code.
int main( void )
{
  _MLU_EnableUtility();
  free(malloc(1));
  realloc(malloc(2), 3);
  malloc(4);
  free(realloc(malloc(6), 5));
  return 0;
}

// END OF 'main.c' FILE
