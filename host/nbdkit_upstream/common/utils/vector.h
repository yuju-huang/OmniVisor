/* nbdkit
 * Copyright (C) 2020 Red Hat Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of Red Hat nor the names of its contributors may be
 * used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RED HAT AND CONTRIBUTORS ''AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL RED HAT OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* Simple implementation of a vector.  It can be cheaply appended, and
 * more expensively inserted.  There are two main use-cases we
 * consider: lists of strings (either with a defined length, or
 * NULL-terminated), and lists of numbers.  It is generic so could be
 * used for lists of anything (eg. structs) where being able to append
 * easily is important.
 */

#ifndef NBDKIT_VECTOR_H
#define NBDKIT_VECTOR_H

#include <assert.h>
#include <string.h>

/* Use of this macro defines a new type called ‘name’ containing an
 * extensible vector of ‘type’ elements.  For example:
 *
 *   DEFINE_VECTOR_TYPE(string_vector, char *)
 *
 * defines a new type called ‘string_vector’ as a vector of ‘char *’.
 * You can create variables of this type:
 *
 *   string_vector names = empty_vector;
 *
 * where ‘names.ptr[]’ will be an array of strings and ‘names.size’
 * will be the number of strings.  There are no get/set accessors.  To
 * iterate over the strings you can use the ‘.ptr’ field directly:
 *
 *   for (size_t i = 0; i < names.size; ++i)
 *     printf ("%s\n", names.ptr[i]);
 *
 * Initializing with ‘empty_vector’ sets ‘.ptr = NULL’ and ‘.size = 0’.
 *
 * DEFINE_VECTOR_TYPE also defines utility functions.  For the full
 * list see the definition below, but useful functions include:
 *
 * ‘name’_append  (eg. ‘string_vector_append’)
 *   - Append a new element at the end.  This operation is cheap.
 *
 * ‘name’_insert  (eg. ‘string_vector_insert’)
 *   - Insert a new element at the beginning, middle or end.  This
 *     operation is more expensive because existing elements may need
 *     to be copied around.
 *
 * Both functions extend the vector if required, and so both may fail
 * (returning -1) which you must check for.
 */
#define DEFINE_VECTOR_TYPE(name, type)                                  \
  struct name {                                                         \
    type *ptr;                 /* Pointer to array of items. */         \
    size_t size;               /* Number of valid items in the array. */ \
    size_t alloc;              /* Number of items allocated. */         \
  };                                                                    \
  typedef struct name name;                                             \
                                                                        \
  /* Reserve n elements at the end of the vector.  Note space is        \
   * allocated but the vector size is not increased and the new         \
   * elements are not initialized.                                      \
   */                                                                   \
  static inline int                                                     \
  name##_reserve (name *v, size_t n)                                    \
  {                                                                     \
    return generic_vector_reserve ((struct generic_vector *)v, n,       \
                                   sizeof (type));                      \
  }                                                                     \
                                                                        \
  /* Insert at i'th element.  i=0 => beginning  i=size => append */     \
  static inline int                                                     \
  name##_insert (name *v, type elem, size_t i)                          \
  {                                                                     \
    if (v->size >= v->alloc) {                                          \
      if (name##_reserve (v, 1) == -1) return -1;                       \
    }                                                                   \
    memmove (&v->ptr[i+1], &v->ptr[i], (v->size-i) * sizeof (elem));    \
    v->ptr[i] = elem;                                                   \
    v->size++;                                                          \
    return 0;                                                           \
  }                                                                     \
                                                                        \
  /* Append a new element to the end of the vector. */                  \
  static inline int                                                     \
  name##_append (name *v, type elem)                                    \
  {                                                                     \
    return name##_insert (v, elem, v->size);                            \
  }                                                                     \
                                                                        \
  /* Iterate over the vector, calling f() on each element. */           \
  static inline void                                                    \
  name##_iter (name *v, void (*f) (type elem))                          \
  {                                                                     \
    size_t i;                                                           \
    for (i = 0; i < v->size; ++i)                                       \
      f (v->ptr[i]);                                                    \
  }                                                                     \
                                                                        \
  /* Sort the elements of the vector. */                                \
  static inline void                                                    \
  name##_sort (name *v,                                                 \
               int (*compare) (const type *p1, const type *p2))         \
  {                                                                     \
    qsort (v->ptr, v->size, sizeof (type), (void *) compare);           \
  }                                                                     \
                                                                        \
  /* Search for an exactly matching element in the vector using a       \
   * binary search.  Returns a pointer to the element or NULL.          \
   */                                                                   \
  static inline type *                                                  \
  name##_search (const name *v, const void *key,                        \
                 int (*compare) (const void *key, const type *v))       \
  {                                                                     \
    return bsearch (key, v->ptr, v->size, sizeof (type),                \
                    (void *) compare);                                  \
  }

#define empty_vector { .ptr = NULL, .size = 0, .alloc = 0 }

struct generic_vector {
  void *ptr;
  size_t size;
  size_t alloc;
};

extern int generic_vector_reserve (struct generic_vector *v,
                                   size_t n, size_t itemsize);

#endif /* NBDKIT_VECTOR_H */
