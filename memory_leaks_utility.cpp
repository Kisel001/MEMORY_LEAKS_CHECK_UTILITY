//
// memory_leaks_utility.cpp
//
//      Copyright (C) Kisel Studios. All rights reserved.
//
// Memory leaks utility implementation module.
//
// Date: 30.06.2025
//

#define __KS_OFF_MLU_
#include "memory_leaks_utliity.h"
#include <windows.h>
#include <map>

#pragma pack(push, 8)
#pragma warning(push)
#pragma warning(disable: 4996)

// Debug allocation structure with information about allocation.
struct _MLU_ALLOCATION
{
  size_t Size;      // Size of allocation.
  const char *File; // Pointer to filename with allocation.
  int Line;         // Line number with allocation.
};

// Map with all allocations.
static std::map<void *, _MLU_ALLOCATION> AllocationMap;

// MLU log function.
// ARGUMENTS:
//      const char *Msg - message for logging.
// RETURNS: None.
//
extern "C" static void _MLU_Log( const char *Msg, ... )
{
  static char Buf[10000];
  memset(Buf, 0, sizeof(Buf));

  va_list List;
  
  va_start(List, Msg);
  vsprintf(Buf, Msg, List);
  va_end(List);

  OutputDebugStringA("\nMemory Leaks Utility\nCopyright (C) Kisel Studios. All rights reserved.\n\n");
  OutputDebugStringA(Buf);
  OutputDebugStringA("\n");
}

// MLU block log function.
// ARGUMENTS:
//      const char *Msg - message for logging.
// RETURNS: None.
//
extern "C" static void _MLU_BlockLog( const char *Msg, ... )
{
  static char Buf[10000];
  memset(Buf, 0, sizeof(Buf));

  va_list List;
  
  va_start(List, Msg);
  vsprintf(Buf, Msg, List);
  va_end(List);

  OutputDebugStringA("\n");
  OutputDebugStringA(Buf);
  OutputDebugStringA("\n");
}

// MLU finish function.
// ARGUMENTS: None.
// RETURNS: None.
extern "C" static void _MLU_Finish( void )
{
  if (!AllocationMap.empty())
  {
    _MLU_Log("Detected memory leaks!");

    for (auto &i : AllocationMap)
    {
      _MLU_BlockLog("Pointer: 0x%p.\n%s(%d).\nSize: %zu", i.first, i.second.File, i.second.Line, i.second.Size);
      free(i.first);
    }

    AllocationMap.clear();
  }
}

// MLU enable utility function.
// ARGUMENTS: None.
// RETURNS: None.
extern "C" void _MLU_EnableUtility( void )
{
  atexit(_MLU_Finish);
}

// MLU malloc function.
// ARGUMENTS:
//      size_t Size - size of allocation.
//      const char *File - filename with allocation.
//      int Line - number of line with allocation.
// RETUNRNS:
//      void * - pointer to allocated memory.
extern "C" void * _MLU_Malloc( size_t Size, const char *File, int Line )
{
  void *Ptr = malloc(Size);

  if (Ptr == NULL)
    return NULL;

  AllocationMap.insert({Ptr, {Size, File, Line}});

  return Ptr;
}

// MLU calloc function.
// ARGUMENTS:
//      size_t Count - count of elements for allocation.
//      size_t Size - size of one element.
//      const char *File - filename with allocation.
//      int Line - number of line with allocation.
// RETUNRNS:
//      void * - pointer to allocated memory.
extern "C" void * _MLU_Calloc( size_t Count, size_t Size, const char *File, int Line )
{
  size_t NewSize = Size * Count;
  void *Ptr = _MLU_Malloc(NewSize, File, Line);

  if (Ptr == NULL)
    return NULL;

  memset(Ptr, 0, sizeof(NewSize));

  return Ptr;
}

// MLU free function.
// ARGUMENTS:
//      void *Ptr - pointer to memory that needs to be freed up.
// RETURNS: None.
extern "C" void _MLU_Free( void *Ptr )
{
  if (Ptr == nullptr)
  {
    _MLU_Log("Pointer is nullptr!");
    return;
  }

  auto i = AllocationMap.find(Ptr);

  if (i == AllocationMap.end())
  {
    _MLU_Log("Allocation %p is not valid!", Ptr);
    return; 
  }
  
  free(i->first);
  AllocationMap.erase(i);
}

// MLU realloc function.
// ARGUMENTS:
//      void *OldPtr - pointer to old memory block.
//      size_t NewSize - new size.
//      const char *File - filename with allocation.
//      int Line - number of line with allocation.
// RETURNS:
//       void * - pointer to reallocated memory.
extern "C" void * _MLU_Realloc( void *OldPtr, size_t NewSize, const char *File, int Line )
{
  if (NewSize == 0)
  {
    _MLU_Free(OldPtr);
    return NULL;
  }
  
  auto i = AllocationMap.find(OldPtr);

  if (i == AllocationMap.end())
  {
    _MLU_Log("Allocation %p is not valid!", OldPtr);
    return NULL;
  }

  size_t OldSize = i->second.Size;

  if (OldSize > NewSize)
  {
    i->second.Size = NewSize;
    i->second.File = File;
    i->second.Line = Line;
    return OldPtr;
  }
  else
  {
    void *NewBlock = _MLU_Malloc(NewSize, File, Line);
    memcpy(NewBlock, OldPtr, OldSize);
    _MLU_Free(OldPtr);
    return NewBlock;
  }
}

#pragma warning(pop)
#pragma pack(pop)

// END OF 'memory_leaks_utility.cpp' FILE
