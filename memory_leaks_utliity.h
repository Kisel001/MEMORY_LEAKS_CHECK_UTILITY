//
// memory_leaks_utility.h
//
//      Copyright (C) Kisel Studios. All rights reserved.
//
// Memory leaks utility header file.
//
// Date: 30.06.2025
//

#ifndef __memory_leaks_utility_h_
#define __memory_leaks_utility_h_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdlib.h>

#ifndef __KS_OFF_MLU_

#if defined(_MSC_VER) && defined(_DEBUG)

// MLU enable utility function.
// ARGUMENTS: None.
// RETURNS: None.
void _MLU_EnableUtility( void );

// MLU malloc function.
// ARGUMENTS:
//      size_t Size - size of allocation.
//      const char *File - filename with allocation.
//      int Line - number of line with allocation
// RETUNRNS:
//      void * - pointer to allocated memory.
void * _MLU_Malloc( size_t Size, const char *File, int Line );

// MLU calloc function.
// ARGUMENTS:
//      size_t Count - count of elements for allocation.
//      size_t Size - size of one element.
//      const char *File - filename with allocation.
//      int Line - number of line with allocation
// RETUNRNS:
//      void * - pointer to allocated memory.
void * _MLU_Calloc( size_t Count, size_t Size, const char *File, int Line );

// MLU free function.
// ARGUMENTS:
//      void *Ptr - pointer to memory that needs to be freed up.
// RETURNS: None.
void _MLU_Free( void *Ptr );

// MLU realloc function.
// ARGUMENTS:
//      void *OldPtr - pointer to old memory block.
//      size_t NewSize - new size.
//      const char *File - filename with allocation.
//      int Line - number of line with allocation.
// RETURNS:
//       void * - pointer to reallocated memory.
void * _MLU_Realloc( void *OldPtr, size_t NewSize, const char *File, int Line );

#define malloc(s) _MLU_Malloc(s, __FILE__, __LINE__)
#define calloc(c, s) _MLU_Calloc(c, s, __FILE__, __LINE__)
#define free(p) _MLU_Free(p)
#define realloc(p, s) _MLU_Realloc(p, s, __FILE__, __LINE__)

#else // _MSC_VER && _DEBUG

#define _MLU_EnableUtility ((void)(0))
#define _MLU_Malloc(s, f, l) malloc(s)
#define _MLU_Calloc(c, s, f, l) calloc(c, s)
#define _MLU_Free(p) free(p)
#define _MLU_Realloc(p, s, f, l) realloc(p, s)

#endif // _MSC_VER && _DEBUG

#endif // !__KS_OFF_MLU_

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !__memory_leaks_utility_h_

// END OF 'memory_leaks_utility.h' FILE
