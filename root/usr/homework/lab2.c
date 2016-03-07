//lab2.c -- page memory management

#include <u.h>

enum { // page table entry flags
  PTE_P   = 0x001,       // Present
  PTE_W   = 0x002,       // Writeable
  PTE_U   = 0x004,       // User
  PTE_A   = 0x020,       // Accessed
  PTE_D   = 0x040,       // Dirty
};


