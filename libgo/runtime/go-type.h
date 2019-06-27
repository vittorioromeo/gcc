/* go-type.h -- basic information for a Go type.

   Copyright 2009 The Go Authors. All rights reserved.
   Use of this source code is governed by a BSD-style
   license that can be found in the LICENSE file.  */

#ifndef LIBGO_GO_TYPE_H
#define LIBGO_GO_TYPE_H

#include <stddef.h>
#include <stdint.h>

#include "array.h"

struct String;

/* Many of the types in this file must match the data structures
   generated by the compiler, and must also match the Go types which
   appear in go/runtime/type.go and go/reflect/type.go.  */

/* Type kinds.  These are used to get the type descriptor to use for
   the type itself, when using unsafe.Typeof or unsafe.Reflect.  The
   values here must match the values generated by the compiler (the
   RUNTIME_TYPE_KIND_xxx values in gcc/go/types.h).  These are macros
   rather than an enum to make it easy to change values in the future
   and hard to get confused about it.

   These correspond to the kind values used by the gc compiler.  */

#define GO_BOOL 1
#define GO_INT 2
#define GO_INT8 3
#define GO_INT16 4
#define GO_INT32 5
#define GO_INT64 6
#define GO_UINT 7
#define GO_UINT8 8
#define GO_UINT16 9
#define GO_UINT32 10
#define GO_UINT64 11
#define GO_UINTPTR 12
#define GO_FLOAT32 13
#define GO_FLOAT64 14
#define GO_COMPLEX64 15
#define GO_COMPLEX128 16
#define GO_ARRAY 17
#define GO_CHAN 18
#define GO_FUNC 19
#define GO_INTERFACE 20
#define GO_MAP 21
#define GO_PTR 22
#define GO_SLICE 23
#define GO_STRING 24
#define GO_STRUCT 25
#define GO_UNSAFE_POINTER 26

#define GO_DIRECT_IFACE (1 << 5)
#define GO_GC_PROG (1 << 6)
#define GO_NO_POINTERS (1 << 7)

#define GO_CODE_MASK 0x1f

/* For each Go type the compiler constructs one of these structures.
   This is used for type reflection, interfaces, maps, and reference
   counting.  */

struct __go_type_descriptor
{
  /* The size in bytes of a value of this type.  Note that all types
     in Go have a fixed size.  */
  uintptr_t __size;

  /* The size of the memory prefix of a value of this type that holds
     all pointers.  */
  uintptr_t __ptrdata;

  /* The type's hash code.  */
  uint32_t __hash;

  /* The type code for this type, one of the type kind values above.
     This is used by unsafe.Reflect and unsafe.Typeof to determine the
     type descriptor to return for this type itself.  It is also used
     by reflect.toType when mapping to a reflect Type structure.  */
  unsigned char __code;

  /* The alignment in bytes of a variable with this type.  */
  unsigned char __align;

  /* The alignment in bytes of a struct field with this type.  */
  unsigned char __field_align;

  /* This function takes a pointer to a value of this type, and the
     size of this type, and returns a hash code.  We pass the size
     explicitly becaues it means that we can share a single instance
     of this function for various different types.  */
  const FuncVal *__hashfn;

  /* This function takes two pointers to values of this type, and the
     size of this type, and returns whether the values are equal.  */
  const FuncVal *__equalfn;

  /* The garbage collection data. */
  const byte *__gcdata;

  /* A string describing this type.  This is only used for
     debugging.  */
  const struct String *__reflection;

  /* A pointer to fields which are only used for some types.  */
  const struct __go_uncommon_type *__uncommon;

  /* The descriptor for the type which is a pointer to this type.
     This may be NULL.  */
  const struct __go_type_descriptor *__pointer_to_this;
};

/* The information we store for each method of a type.  */

struct __go_method
{
  /* The name of the method.  */
  const struct String *__name;

  /* This is NULL for an exported method, or the name of the package
     where it lives.  */
  const struct String *__pkg_path;

  /* The type of the method, without the receiver.  This will be a
     function type.  */
  const struct __go_type_descriptor *__mtype;

  /* The type of the method, with the receiver.  This will be a
     function type.  */
  const struct __go_type_descriptor *__type;

  /* A pointer to the code which implements the method.  This is
     really a function pointer.  */
  const void *__function;
};

/* Additional information that we keep for named types and for types
   with methods.  */

struct __go_uncommon_type
{
  /* The name of the type.  */
  const struct String *__name;

  /* The type's package.  This is NULL for builtin types.  */
  const struct String *__pkg_path;

  /* The type's methods.  This is an array of struct __go_method.  */
  struct __go_open_array __methods;
};

/* The type descriptor for a function.  */

struct __go_func_type
{
  /* Starts like all other type descriptors.  */
  struct __go_type_descriptor __common;

  /* Whether this is a varargs function.  If this is true, there will
     be at least one parameter.  For "..." the last parameter type is
     "interface{}".  For "... T" the last parameter type is "[]T".  */
  _Bool __dotdotdot;

  /* The input parameter types.  This is an array of pointers to
     struct __go_type_descriptor.  */
  struct __go_open_array __in;

  /* The output parameter types.  This is an array of pointers to
     struct __go_type_descriptor.  */
  struct __go_open_array __out;
};

/* A map type.  */

struct __go_map_type
{
  /* Starts like all other type descriptors.  */
  struct __go_type_descriptor __common;

  /* The map key type.  */
  const struct __go_type_descriptor *__key_type;

  /* The map value type.  */
  const struct __go_type_descriptor *__val_type;

  /* The map bucket type.  */
  const struct __go_type_descriptor *__bucket_type;

  /* The map header type.  */
  const struct __go_type_descriptor *__hmap_type;

  /* The size of the key slot.  */
  uint8_t __key_size;

  /* Whether to store a pointer to key rather than the key itself.  */
  uint8_t __indirect_key;

  /* The size of the value slot.  */
  uint8_t __value_size;

  /* Whether to store a pointer to value rather than the value itself.  */
  uint8_t __indirect_value;

  /* The size of a bucket.  */
  uint16_t __bucket_size;

  /* Whether the key type is reflexive--whether k==k for all keys.  */
  _Bool __reflexive_key;

  /* Whether we should update the key when overwriting an entry.  */
  _Bool __need_key_update;
};

/* A pointer type.  */

struct __go_ptr_type
{
  /* Starts like all other type descriptors.  */
  struct __go_type_descriptor __common;

  /* The type to which this points.  */
  const struct __go_type_descriptor *__element_type;
};

#endif /* !defined(LIBGO_GO_TYPE_H) */
