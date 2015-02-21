#ifndef BF_COMMON_H
#define BF_COMMON_H

#define Constrtuctor1(ClassName)                         \
  static ClassName * New(Allocator & a)       {          \
    ClassName * toAlloc;                                 \
    a.New(&toAlloc);                                     \
    return toAlloc;                                      \
  }                                                      \

#define Constrtuctor2(ClassName, ArgClass)               \
  static ClassName * New(Allocator & a, ArgClass arg) {  \
    ClassName * toAlloc;                                 \
    a.New(&toAlloc, arg);                                \
    return toAlloc;                                      \
  }                                                      \

#endif
