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

typedef void (*_vector_ctor_t)(void*);
typedef void (*_vector_dtor_t)(void*);

/*! \brief Comparese two values
    if a is equal to b, return value is 0
    if a is less than b, return value is < 0
    if b is less than a, return value is > 0
    strcmp and memcmp may be used
*/
typedef int (*_vector_cmp_t)(void*, void*);
typedef struct {
    size_t count;
    size_t alloc;
    size_t size;
    _vector_ctor_t ctor;
    _vector_ctor_t dtor;
    _vector_cmp_t cmp;
} _vector_t;

/*! \brief Creates a new vector objec
*/
static inline _vector_t* _vector_new(size_t s, _vector_ctor_t ctor, _vector_dtor_t dtor, _vector_cmp_t cmp)
{
    _vector_t* v = (_vector_t*)calloc(sizeof(_vector_t) + s, 1);
    v->size = s, v->ctor = ctor, v->dtor = dtor, v->cmp = cmp;
    return v;
}

/*! \brief Returns pointer to vectory memory
*/
static inline char* _vector_begin(_vector_t** v)
{
    return ((char*)(*v)) + sizeof(_vector_t);
}

/*! \brief Returns pointer to vectory memory
*/
static inline char* _vector_end(_vector_t** v)
{
    return ((char*)(*v)) + sizeof(_vector_t) + ((*v)->count * (*v)->size);
}

/*! \brief Returns count of elements in vector
*/
static inline size_t _vector_count(_vector_t** v)
{
    return (*v)->count;
}

/*! \brief Returns a pointer to the element at position c
*/
static inline char* _vector_at(_vector_t** v, size_t c)
{
    if ((*v)->count <= c) {
        return 0;
    }

    return _vector_begin(v) + (c * (*v)->size);
}

/*! \brief Returns a pointer to the element at the front of the vector, or 0 if vector is empty
*/
static inline char* _vector_front(_vector_t** v)
{
    return _vector_at(v, 0);
}

/*! \brief Returns a pointer to the element at the front of the vector, or 0 if vector is empty
*/
static inline char* _vector_back(_vector_t** v)
{
    return _vector_at(v, _vector_count(v) - 1);
}

/*! \brief Reserves enough memry to store c elements
*/
static inline size_t _vector_reserve(_vector_t** v, size_t c)
{
    if (c > (*v)->alloc) {
        size_t bytes = (1 + c) * (*v)->size;
        char* new_ptr = (char*)realloc((*v), sizeof(_vector_t) + bytes);

        if (new_ptr) {
            (*v) = (_vector_t*)new_ptr;
            (*v)->alloc = c;
        }
    }
    return (*v)->alloc;
}

/*! \brief Resizes vector by inserting or removine elements as necessary
*/
static inline size_t _vector_resize(_vector_t** v, size_t c)
{
    if (c > _vector_reserve(v, c)) {
        return 0; // could not alocate storage
    }

    while ((*v)->ctor && (*v)->count < c) {
        ++(*v)->count, (*v)->ctor(_vector_back(v));
    }

    while ((*v)->dtor && (*v)->count > c) {
        (*v)->dtor(_vector_back(v)), --(*v)->count;
    }

    // _vector_reserve always alocates an extra element, so this is safe
    memset(_vector_begin(v) + ((*v)->size * (*v)->count), 0, (*v)->size);
    return (*v)->count;
}

/*! \brief
*/
static inline size_t _vector_insert(_vector_t** v, size_t p, size_t c, const char* d)
{
    if ((*v)->count < p) {
        return 0;
    }

    size_t new_count = (*v)->count + c;
    if (c > _vector_reserve(v, new_count)) {
        return 0;
    }

    char* insert_at = _vector_begin(v) + ((p + 0) * (*v)->size);
    char* insert_to = _vector_begin(v) + ((p + c) * (*v)->size);
    memmove(insert_to, insert_at, _vector_end(v) - insert_at);
    memmove(insert_at, d, c * (*v)->size);
    (*v)->count = new_count;
    // null out last element
    memset(_vector_begin(v) + ((*v)->size * (*v)->count), 0, (*v)->size);
    return (*v)->count;
}

/*! \brief
*/
static inline size_t _vector_append(_vector_t** v, size_t c, const char* d)
{
    return _vector_insert(v, (*v)->count, c, d);
}

/*! \brief
*/
static inline size_t _vector_erase(_vector_t** v, size_t p, size_t c)
{
    if ((*v)->count < p + c) {
        return 0;
    }

    if ((*v)->dtor) {
        for (size_t i = p; i < p + c; ++i) {
            (*v)->dtor(_vector_at(v, i));
        }
    }

    char* erase_at = _vector_begin(v) + ((p + 0) * (*v)->size);
    char* erase_to = _vector_begin(v) + ((p + c) * (*v)->size);
    memmove(erase_at, erase_to, _vector_end(v) - erase_to);
    (*v)->count -= c;
    memset(_vector_begin(v) + ((*v)->size * (*v)->count), 0, (*v)->size);
    return (*v)->count;
}

/*! \brief Inserts a new element at the back of the vector and returns its pointer
*/
static inline char* _vector_push_back(_vector_t** v)
{
    size_t new_count = _vector_count(v) + 1;
    if (new_count != _vector_resize(v, new_count)) {
        return 0;
    }

    return _vector_back(v);
}

/*! \brief Inserts a new element at the back of the vector
*/
static inline char* _vector_emplace_back(_vector_t** v, char* d)
{
    char* n = _vector_push_back(v);
    if (n) {
        memcpy(n, d, (*v)->size);
    }
    return n;
}

/*! \brief Removes element at the back of the vector
*/
static inline void _vector_pop_back(_vector_t** v)
{
    size_t count;
    if (0 < (count = _vector_count(v))) {
        _vector_resize(v, count - 1);
    }
}

/*! \brief
*/
static inline void _vector_sort(_vector_t** v, int o)
{

    if (!(*v)->cmp || o == 0 || 2 > (*v)->count) {
        return;
    }

    // TODO better sort
    char* c = _vector_push_back(v);
again:
    for (size_t i = 1; i < (*v)->count; ++i) {
        char* a = _vector_at(v, i - 0);
        char* b = _vector_at(v, i - 1);
        if (0 > o * (*v)->cmp(a, b)) {
            memcpy(c, a, (*v)->size);
            memcpy(a, b, (*v)->size);
            memcpy(b, c, (*v)->size);
            goto again;
        }
    }
    _vector_pop_back(v);
}

/*! \brief
*/
static inline void _vector_sort_ascending(_vector_t** v) { _vector_sort(v, 1); }
static inline void _vector_sort_descending(_vector_t** v) { _vector_sort(v, -1); }

/*! \brief Removes all element from the vector and free memory
*/
static inline void _vector_del(_vector_t** v)
{
    _vector_resize(v, 0);
    free(*v), *v = 0;
}

#define MAKE_VECTOR(TYPE, NAME, CTOR, DTOR, CMP)                                                                                                                               \
    typedef _vector_t NAME##_vector_t;                                                                                                                                         \
    typedef void (*NAME##_vector_ctor_t)(TYPE*);                                                                                                                               \
    typedef void (*NAME##_vector_dtor_t)(TYPE*);                                                                                                                               \
    typedef int (*NAME##_vector_cmp_t)(TYPE*, TYPE*);                                                                                                                          \
    static inline NAME##_vector_t* NAME##_vector_new() { return (NAME##_vector_t*)_vector_new(sizeof(TYPE), (_vector_ctor_t)CTOR, (_vector_dtor_t)DTOR, (_vector_cmp_t)CMP); } \
    static inline TYPE* NAME##_vector_begin(NAME##_vector_t** v) { return (TYPE*)_vector_begin((_vector_t**)v); }                                                              \
    static inline TYPE* NAME##_vector_end(NAME##_vector_t** v) { return (TYPE*)_vector_end((_vector_t**)v); }                                                                  \
    static inline size_t NAME##_vector_count(NAME##_vector_t** v) { return _vector_count((_vector_t**)v); }                                                                    \
    static inline TYPE* NAME##_vector_at(NAME##_vector_t** v, size_t c) { return (TYPE*)_vector_at((_vector_t**)v, c); }                                                       \
    static inline TYPE* NAME##_vector_front(NAME##_vector_t** v) { return (TYPE*)_vector_front((_vector_t**)v); }                                                              \
    static inline TYPE* NAME##_vector_back(NAME##_vector_t** v) { return (TYPE*)_vector_back((_vector_t**)v); }                                                                \
    static inline size_t NAME##_vector_reserve(NAME##_vector_t** v, size_t c) { return _vector_reserve((_vector_t**)v, c); }                                                   \
    static inline size_t NAME##_vector_resize(NAME##_vector_t** v, size_t c) { return _vector_resize((_vector_t**)v, c); }                                                     \
    static inline size_t NAME##_vector_insert(NAME##_vector_t** v, size_t p, size_t c, const TYPE* d) { return _vector_insert((_vector_t**)v, p, c, (const char*)d); }         \
    static inline size_t NAME##_vector_append(NAME##_vector_t** v, size_t c, const TYPE* d) { return _vector_append((_vector_t**)v, c, (const char*)d); }                      \
    static inline size_t NAME##_vector_erase(NAME##_vector_t** v, size_t p, size_t c) { return _vector_erase((_vector_t**)v, p, c); }                                          \
    static inline TYPE* NAME##_vector_push_back(_vector_t** v) { return (TYPE*)_vector_push_back((_vector_t**)v); }                                                            \
    static inline TYPE* NAME##_vector_emplace_back(_vector_t** v, TYPE* d) { return (TYPE*)_vector_emplace_back((_vector_t**)v, (char*)d); }                                   \
    static inline void NAME##_vector_pop_back(NAME##_vector_t** v) { return _vector_pop_back((_vector_t**)v); }                                                                \
    static inline void NAME##_vector_sort(_vector_t** v, int o) { _vector_sort((_vector_t**)v, o); }                                                                           \
    static inline void NAME##_vector_sort_ascending(_vector_t** v) { _vector_sort_ascending((_vector_t**)v); }                                                                 \
    static inline void NAME##_vector_sort_descending(_vector_t** v) { _vector_sort_descending((_vector_t**)v); }                                                               \
    static inline void NAME##_vector_del(NAME##_vector_t** v) { return _vector_del((_vector_t**)v); }

// Common types
#define MAKE_VECTOR_SIMPLE(TYPE, NAME)                                                                        \
    static inline void NAME##_vector_ctor(TYPE* a) { (*a) = 0; }                                              \
    static inline int NAME##_vector_cmp(TYPE* a, TYPE* b) { return (*a) == (*b) ? 0 : (*a) < (*b) ? -1 : 1; } \
    MAKE_VECTOR(TYPE, NAME, NAME##_vector_ctor, 0, NAME##_vector_cmp)

MAKE_VECTOR_SIMPLE(int8_t, int8);
MAKE_VECTOR_SIMPLE(uint8_t, uint8);
MAKE_VECTOR_SIMPLE(int16_t, int16);
MAKE_VECTOR_SIMPLE(uint16_t, uint16);
MAKE_VECTOR_SIMPLE(int32_t, int32);
MAKE_VECTOR_SIMPLE(uint32_t, uint32);
MAKE_VECTOR_SIMPLE(int64_t, int64);
MAKE_VECTOR_SIMPLE(uint64_t, uint64);
MAKE_VECTOR_SIMPLE(char, char);
MAKE_VECTOR_SIMPLE(unsigned char, uchar);
MAKE_VECTOR_SIMPLE(float, float);
MAKE_VECTOR_SIMPLE(double, double);

// string type
static inline void str_vector_ctor(char** a) { (*a) = 0; }
static inline void str_vector_dtor(char** a)
{
    if (*a)
        free(*a);
    (*a) = 0;
}
MAKE_VECTOR(char*, str, str_vector_ctor, str_vector_dtor, strcmp);

#ifdef __cplusplus
}
#endif
#endif
