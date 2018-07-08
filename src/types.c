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

_vector_t* _vector_new(size_t s, _ctor_t ctor, _dtor_t dtor, _cmp_t cmp)
{
    _vector_t* v = (_vector_t*)calloc(sizeof(_vector_t) + s, 1);
    v->size = s, v->ctor = ctor, v->dtor = dtor, v->cmp = cmp;
    return v;
}

/*! \brief Returns pointer to vectory memory
*/
char* _vector_begin(_vector_t** v)
{
    return ((char*)(*v)) + sizeof(_vector_t);
}

/*! \brief Returns pointer to vectory memory
*/
char* _vector_end(_vector_t** v)
{
    return ((char*)(*v)) + sizeof(_vector_t) + ((*v)->count * (*v)->size);
}

/*! \brief Returns count of elements in vector
*/
size_t _vector_count(_vector_t** v)
{
    return (*v)->count;
}

/*! \brief Returns a pointer to the element at position c
*/
char* _vector_at(_vector_t** v, size_t c)
{
    if ((*v)->count <= c) {
        return 0;
    }

    return _vector_begin(v) + (c * (*v)->size);
}

/*! \brief Returns a pointer to the element at the front of the vector, or 0 if vector is empty
*/
char* _vector_front(_vector_t** v)
{
    return _vector_at(v, 0);
}

/*! \brief Returns a pointer to the element at the front of the vector, or 0 if vector is empty
*/
char* _vector_back(_vector_t** v)
{
    return _vector_at(v, _vector_count(v) - 1);
}

/*! \brief Reserves enough memry to store c elements
*/
size_t _vector_reserve(_vector_t** v, size_t c)
{
    if (c > (*v)->alloc) {
        // TODO alocate full pages
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
size_t _vector_resize(_vector_t** v, size_t c)
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
size_t _vector_insert(_vector_t** v, size_t p, size_t c, const char* d)
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
size_t _vector_append(_vector_t** v, size_t c, const char* d)
{
    return _vector_insert(v, (*v)->count, c, d);
}

/*! \brief
*/
size_t _vector_erase(_vector_t** v, size_t p, size_t c)
{
    if ((*v)->count < p + c) {
        return 0;
    }

    if ((*v)->dtor) {
        for (size_t i = p; i < p + c; ++i) {
            (*v)->dtor(_vector_at(v, i));
        }
    }

    char* buffer = _vector_begin(v);
    char* erase_at = buffer + (p * (*v)->size);
    char* erase_to = erase_at + (c * (*v)->size);
    (*v)->count = _vector_end(v) - erase_to;
    memmove(erase_at, erase_to, (*v)->count);
    memset(buffer + ((*v)->size * (*v)->count), 0, (*v)->size);
    return (*v)->count;
}

/*! \brief Inserts a new element at the back of the vector and returns its pointer
*/
char* _vector_push_back(_vector_t** v)
{
    size_t new_count = _vector_count(v) + 1;
    if (new_count != _vector_resize(v, new_count)) {
        return 0;
    }

    return _vector_back(v);
}

/*! \brief Inserts a new element at the back of the vector
*/
char* _vector_emplace_back(_vector_t** v, char* d)
{
    char* n = _vector_push_back(v);
    if (n) {
        memcpy(n, d, (*v)->size);
    }
    return n;
}

/*! \brief Removes element at the back of the vector
*/
void _vector_pop_back(_vector_t** v)
{
    size_t count;
    if (0 < (count = _vector_count(v))) {
        _vector_resize(v, count - 1);
    }
}

/*! \brief
*/
void _vector_sort(_vector_t** v, int o)
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
void _vector_dup(_vector_t** t, _vector_t** f)
{
    size_t count = _vector_count(f);
    _vector_resize(t, 0); // run dtors
    _vector_reserve(t, count);
    memcpy((*t), (*f), count * (*f)->size);
}

/*! \brief Removes all element from the vector and free memory
*/
void _vector_del(_vector_t** v)
{
    _vector_resize(v, 0);
    free(*v), *v = 0;
}
