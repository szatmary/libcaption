/**********************************************************************************************/
/* The MIT License                                                                            */
/*                                                                                            */
/* Copyright 2016-2017 Twitch Interactive, Inc. or its affiliates. All Rights Reserved.       */
/*                                                                                            */
/* Permission is hereby granted, free of charge, to any person obtaining a copy               */
/* of this software and associated documentation files (the "Software"), to deal              */
/* in the Software without restriction, including without limitation the rights               */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                  */
/* copies of the Software, and to permit persons to whom the Software is                      */
/* furnished to do so, subject to the following conditions:                                   */
/*                                                                                            */
/* The above copyright notice and this permission notice shall be included in                 */
/* all copies or substantial portions of the Software.                                        */
/*                                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                 */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                     */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,              */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN                  */
/* THE SOFTWARE.                                                                              */
/**********************************************************************************************/
#ifndef LIBCAPTION_TYPES_H
#define LIBCAPTION_TYPES_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h> // size_t/malloc/calloc
#include <string.h> // memset

#ifdef _MSC_VER
#ifndef inline
#define inline __inline
#endif
#endif

typedef void (*_ctor_t)(void*);
typedef void (*_dtor_t)(void*);

/*! \brief Comparese two values
    if a is equal to b, return value is 0
    if a is less than b, return value is < 0
    if b is less than a, return value is > 0
    strcmp and memcmp may be used
*/
typedef int (*_cmp_t)(void*, void*);
/////////////
typedef struct {
    size_t count;
    size_t alloc;
    size_t size;
    _ctor_t ctor;
    _ctor_t dtor;
    _cmp_t cmp;
} _vector_t;

/*! \brief Creates a new vector objec
*/
_vector_t* _vector_new(size_t s, _ctor_t ctor, _dtor_t dtor, _cmp_t cmp);

/*! \brief Returns pointer to vectory memory
*/
char* _vector_begin(_vector_t** v);

/*! \brief Returns pointer to vectory memory
*/
char* _vector_end(_vector_t** v);

/*! \brief Returns count of elements in vector
*/
size_t _vector_count(_vector_t** v);

/*! \brief Returns a pointer to the element at position c
*/
char* _vector_at(_vector_t** v, size_t c);

/*! \brief Returns a pointer to the element at the front of the vector, or 0 if vector is empty
*/
char* _vector_front(_vector_t** v);

/*! \brief Returns a pointer to the element at the front of the vector, or 0 if vector is empty
*/
char* _vector_back(_vector_t** v);
/*! \brief Reserves enough memry to store c elements
*/
size_t _vector_reserve(_vector_t** v, size_t c);

/*! \brief Resizes vector by inserting or removine elements as necessary
*/
size_t _vector_resize(_vector_t** v, size_t c);

/*! \brief 
*/
void _vector_clear(_vector_t** v);

/*! \brief
*/
size_t _vector_insert(_vector_t** v, size_t p, size_t c, char* d);
/*! \brief
*/
size_t _vector_append(_vector_t** v, size_t c, char* d);
/*! \brief
*/
size_t _vector_erase(_vector_t** v, size_t p, size_t c);

/*! \brief Inserts a new element at the back of the vector and returns its pointer
*/
char* _vector_push_back(_vector_t** v);
/*! \brief Inserts a new element at the back of the vector
*/
char* _vector_emplace_back(_vector_t** v, char* d);

/*! \brief Removes element at the back of the vector
*/
void _vector_pop_back(_vector_t** v);

/*! \brief
*/
void _vector_sort(_vector_t** v, int o);

/*! \brief
*/
void _vector_dup(_vector_t** t, _vector_t** f);

/*! \brief
*/
void _vector_del(_vector_t** v);
static inline void _vector_sort_ascending(_vector_t** v) { _vector_sort(v, 1); }
static inline void _vector_sort_descending(_vector_t** v) { _vector_sort(v, -1); }

/*! \brief Removes all element from the vector and free memory
*/

#define MAKE_VECTOR(TYPE, NAME, CTOR, DTOR, CMP)                                                                                                                       \
    typedef _vector_t NAME##_vector_t;                                                                                                                                 \
    typedef void (*NAME##_ctor_t)(TYPE*);                                                                                                                              \
    typedef void (*NAME##_dtor_t)(TYPE*);                                                                                                                              \
    typedef int (*NAME##_cmp_t)(TYPE*, TYPE*);                                                                                                                         \
    static inline NAME##_vector_t* NAME##_vector_new() { return (NAME##_vector_t*)_vector_new(sizeof(TYPE), (_ctor_t)CTOR, (_dtor_t)DTOR, (_cmp_t)CMP); }              \
    static inline TYPE* NAME##_vector_begin(NAME##_vector_t** v) { return (TYPE*)_vector_begin((_vector_t**)v); }                                                      \
    static inline TYPE* NAME##_vector_end(NAME##_vector_t** v) { return (TYPE*)_vector_end((_vector_t**)v); }                                                          \
    static inline size_t NAME##_vector_count(NAME##_vector_t** v) { return _vector_count((_vector_t**)v); }                                                            \
    static inline TYPE* NAME##_vector_at(NAME##_vector_t** v, size_t c) { return (TYPE*)_vector_at((_vector_t**)v, c); }                                               \
    static inline TYPE* NAME##_vector_front (NAME##_vector_t** v) { return (TYPE*)_vector_front((_vector_t**)v); }                                                      \
    static inline TYPE* NAME##_vector_back(NAME##_vector_t** v) { return (TYPE*)_vector_back((_vector_t**)v); }                                                        \
    static inline size_t NAME##_vector_reserve(NAME##_vector_t** v, size_t c) { return _vector_reserve((_vector_t**)v, c); }                                           \
    static inline size_t NAME##_vector_resize(NAME##_vector_t** v, size_t c) { return _vector_resize((_vector_t**)v, c); }                                             \
   static inline  void NAME##_vector_clear(NAME##_vector_t** v) { return _vector_clear((_vector_t**)v); } \
   static inline size_t NAME##_vector_insert(NAME##_vector_t** v, size_t p, size_t c, TYPE* d) { return _vector_insert((_vector_t**)v, p, c, (char*)d); } \
    static inline size_t NAME##_vector_append(NAME##_vector_t** v, size_t c, TYPE* d) { return _vector_append((_vector_t**)v, c, (char*)d); }              \
    static inline size_t NAME##_vector_erase(NAME##_vector_t** v, size_t p, size_t c) { return _vector_erase((_vector_t**)v, p, c); }                                  \
    static inline TYPE* NAME##_vector_push_back(_vector_t** v) { return (TYPE*)_vector_push_back((_vector_t**)v); }                                                    \
    static inline TYPE* NAME##_vector_emplace_back(_vector_t** v, TYPE* d) { return (TYPE*)_vector_emplace_back((_vector_t**)v, (char*)d); }                           \
    static inline void NAME##_vector_pop_back(NAME##_vector_t** v) { return _vector_pop_back((_vector_t**)v); }                                                        \
    static inline void NAME##_vector_sort(_vector_t** v, int o) { return _vector_sort((_vector_t**)v, o); }                                                            \
    static inline void NAME##_vector_sort_ascending(_vector_t** v) { return _vector_sort_ascending((_vector_t**)v); }                                                  \
    static inline void NAME##_vector_sort_descending(_vector_t** v) { return _vector_sort_descending((_vector_t**)v); }                                                \
    static inline void NAME##_vector_dup(_vector_t** t, _vector_t** f) { return _vector_dup((_vector_t**)t, (_vector_t**)f); }                                         \
    static inline void NAME##_vector_del(NAME##_vector_t** v) { return _vector_del((_vector_t**)v); }

// Common types
#define MAKE_VECTOR_INTTYPE(TYPE, NAME)                                                                \
    static inline void NAME##_ctor(TYPE* a) { (*a) = 0; }                                              \
    static inline int NAME##_cmp(TYPE* a, TYPE* b) { return (*a) == (*b) ? 0 : (*a) < (*b) ? -1 : 1; } \
    MAKE_VECTOR(TYPE, NAME, NAME##_ctor, 0, NAME##_cmp)

MAKE_VECTOR_INTTYPE(int8_t, int8);
MAKE_VECTOR_INTTYPE(uint8_t, uint8);
MAKE_VECTOR_INTTYPE(int16_t, int16);
MAKE_VECTOR_INTTYPE(uint16_t, uint16);
MAKE_VECTOR_INTTYPE(int32_t, int32);
MAKE_VECTOR_INTTYPE(uint32_t, uint32);
MAKE_VECTOR_INTTYPE(int64_t, int64);
MAKE_VECTOR_INTTYPE(uint64_t, uint64);
MAKE_VECTOR_INTTYPE(char, char);
MAKE_VECTOR_INTTYPE(float, float);
MAKE_VECTOR_INTTYPE(double, double);

#ifdef __cplusplus
}
#endif
#endif
