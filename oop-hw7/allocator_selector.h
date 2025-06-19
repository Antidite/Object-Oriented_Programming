#pragma once

// 1 = StudentAllocator, 2 = std::allocator
#ifndef ALLOCATOR_MODE
  #define ALLOCATOR_MODE 1
#endif

#if ALLOCATOR_MODE == 1
  #include "My_code/MyAllocator.h"
  #include "My_code/MyMemoryPool.h"
  #define MyAllocator    StudentAllocator
  #define AllocatorLabel "StudentAllocator"
#elif ALLOCATOR_MODE == 2
  #include "Teacher_code/stdafx.h"
  #include "Teacher_code/myallocator.h"
  #define MyAllocator    std::allocator
  #define AllocatorLabel "std::allocator"
#else
  #error "ALLOCATOR_MODE 必须定义为 1 或 2"
#endif