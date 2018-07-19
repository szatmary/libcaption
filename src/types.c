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

#include "types.h"

typedef struct {
    size_t count;
    size_t alloc;
    size_t size;
    ctor_t ctor;
    dtor_t dtor;
    cmp_t cmp;
} vector_t;

char* _vector_new(size_t s, ctor_t ctor, dtor_t dtor, cmp_t cmp)
{
    vector_t* v = (vector_t*)calloc(sizeof(vector_t) + s, 1);
    v->size = s, v->ctor = ctor, v->dtor = dtor, v->cmp = cmp;
    return ((char*)v) + sizeof(vector_t);
}

// Internal use only
static inline vector_t* dref(char** v)
{
    return (vector_t*)(((char*)(*v)) - sizeof(vector_t));
}

/*! \brief Returns pointer to vectory memory
*/
char* _vector_begin(char** v)
{
    if (!v || !*v) {
        return 0;
    }

    return (*v);
}

/*! \brief Returns pointer to vectory memory
*/
char* _vector_end(char** v)
{
    if (!v || !*v) {
        return 0;
    }

    return (*v) + (dref(v)->size * dref(v)->count);
}

/*! \brief Returns count of elements in vector
*/
size_t _vector_count(char** v)
{
    if (!v || !*v) {
        return 0;
    }

    return dref(v)->count;
}

/*! \brief Returns a pointer to the element at position c
*/
char* _vector_at(char** v, size_t c)
{
    if (!v || !*v) {
        return 0;
    }

    return (*v) + (c * dref(v)->size);
}

/*! \brief Returns a pointer to the element at the front of the vector, or 0 if vector is empty
*/
char* _vector_front(char** v)
{
    if (!v || !*v) {
        return 0;
    }

    return (*v);
}

/*! \brief Returns a pointer to the element at the front of the vector, or 0 if vector is empty
*/
char* _vector_back(char** v)
{
    if (!v || !*v) {
        return 0;
    }

    return _vector_at(v, _vector_count(v) - 1);
}

/*! \brief Reserves enough memry to store c elements
*/
#define PAGE_SIZE ((size_t)1024)
size_t _vector_reserve(char** v, size_t c)
{
    if (!v || !*v) {
        return 0;
    }

    if (c > dref(v)->alloc) {
        size_t bytes = sizeof(vector_t) + ((1 + c) * dref(v)->size);
        bytes = (bytes + PAGE_SIZE) & ~(PAGE_SIZE - 1); // Round to page size
        size_t new_alloc = (bytes - (dref(v)->size + sizeof(vector_t))) / dref(v)->size;
        char* new_ptr = (char*)realloc(dref(v), bytes);

        if (new_ptr) {
            (*v) = new_ptr + sizeof(vector_t);
            dref(v)->alloc = new_alloc;
        }
    }
    return dref(v)->alloc;
}

/*! \brief Resizes vector by inserting or removine elements as necessary
*/
size_t _vector_resize(char** v, size_t c)
{
    if (!v || !*v) {
        return 0;
    }

    if (c > _vector_reserve(v, c)) {
        return 0; // could not alocate storage
    }

    while (dref(v)->ctor && dref(v)->count < c) {
        ++dref(v)->count, dref(v)->ctor(_vector_back(v));
    }

    while (dref(v)->dtor && dref(v)->count > c) {
        dref(v)->dtor(_vector_back(v)), --dref(v)->count;
    }

    dref(v)->count = c;
    // _vector_reserve always alocates an extra element, so this is safe
    memset(_vector_begin(v) + (dref(v)->size * dref(v)->count), 0, dref(v)->size);
    return dref(v)->count;
}

/*! \brief 
*/
void _vector_clear(char** v)
{
    _vector_resize(v, 0);
}

/*! \brief
*/
size_t _vector_insert(char** v, size_t p, size_t c, char* d)
{
    if (!v || !*v) {
        return 0;
    }

    if (dref(v)->count < p) {
        return 0;
    }

    size_t new_count = dref(v)->count + c;
    if (c > _vector_reserve(v, new_count)) {
        return 0;
    }

    char* insert_at = _vector_begin(v) + ((p + 0) * dref(v)->size);
    char* insert_to = _vector_begin(v) + ((p + c) * dref(v)->size);
    memmove(insert_to, insert_at, _vector_end(v) - insert_at);
    memmove(insert_at, d, c * dref(v)->size);
    dref(v)->count = new_count;
    // null out last element
    memset(_vector_begin(v) + (dref(v)->size * dref(v)->count), 0, dref(v)->size);
    return dref(v)->count;
}

/*! \brief
*/
size_t _vector_append(char** v, size_t c, char* d)
{
    return _vector_insert(v, dref(v)->count, c, d);
}

/*! \brief
*/
size_t _vector_erase(char** v, size_t p, size_t c)
{
    if (!v || !*v) {
        return 0;
    }

    if (dref(v)->count < p + c) {
        return 0;
    }

    if (dref(v)->dtor) {
        for (size_t i = p; i < p + c; ++i) {
            dref(v)->dtor(_vector_at(v, i));
        }
    }

    char* buffer = _vector_begin(v);
    char* erase_at = buffer + (p * dref(v)->size);
    char* erase_to = erase_at + (c * dref(v)->size);
    dref(v)->count = _vector_end(v) - erase_to;
    memmove(erase_at, erase_to, dref(v)->count);
    memset(buffer + (dref(v)->size * dref(v)->count), 0, dref(v)->size);
    return dref(v)->count;
}

/*! \brief Inserts a new element at the back of the vector and returns its pointer
*/
char* _vector_push_back(char** v)
{
    if (!v || !*v) {
        return 0;
    }

    size_t new_count = _vector_count(v) + 1;
    if (new_count != _vector_resize(v, new_count)) {
        return 0;
    }

    return _vector_back(v);
}

/*! \brief Inserts a new element at the back of the vector
*/
char* _vector_emplace_back(char** v, char* d)
{
    if (!v || !*v) {
        return 0;
    }

    char* n = _vector_push_back(v);
    if (n) {
        memcpy(n, d, dref(v)->size);
    }
    return n;
}

/*! \brief Removes element at the back of the vector
*/
void _vector_pop_back(char** v)
{
    if (!v || !*v) {
        return;
    }

    size_t count;
    if (0 < (count = _vector_count(v))) {
        _vector_resize(v, count - 1);
    }
}

/*! \brief
*/
void _vector_sort(char** v, int o)
{
    if (!v || !*v) {
        return;
    }

    if (!dref(v)->cmp || o == 0 || 2 > dref(v)->count) {
        return;
    }

    // TODO better sort
    char* c = _vector_push_back(v);
again:
    for (size_t i = 1; i < dref(v)->count; ++i) {
        char* a = _vector_at(v, i - 0);
        char* b = _vector_at(v, i - 1);
        if (0 > o * dref(v)->cmp(a, b)) {
            memcpy(c, a, dref(v)->size);
            memcpy(a, b, dref(v)->size);
            memcpy(b, c, dref(v)->size);
            goto again;
        }
    }
    _vector_pop_back(v);
}

/*! \brief
*/
void _vector_dup(char** t, char** f)
{
    if (!t || !*t | !f || !*f) {
        return;
    }

    size_t count = _vector_count(f);
    _vector_resize(t, 0); // run dtors
    _vector_reserve(t, count);
    memcpy(dref(t), dref(f), (1 + count) * dref(f)->size);
}

/*! \brief Removes all element from the vector and free memory
*/
void _vector_del(char** v)
{
    if (!v || !*v) {
        return;
    }

    _vector_resize(v, 0);
    free(dref(v)), *v = 0;
}
