
#pragma once
#include "point.h"

#define __DATA__
#define __FUNCTION__
#define __VIRTUAL__
#define __OVERRIDE__

#define CTOR(type)                                                          \
type* type##_new() {                                                        \
    struct type *self;                                                      \
    self = (struct type*)malloc(sizeof(struct type));                       \
    if(!self)                                                               \
    {                                                                       \
        return 0;                                                           \
    }                                                                       \
    type##_init(self);                                                      \
    return self;                                                            \
}                                                                           \


#define DTOR(type)                                                          \
void type##_delete(type* cthis)                                             \
{                                                                           \
        if(type##_exit(cthis))                                              \
        {                                                                   \
            free(cthis);                                                    \
        }                                                                   \
}                                                                           \


#define SUPER_PTR(cthis, father) ((father*)(&(cthis->##father)))