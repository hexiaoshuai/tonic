// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_TONIC_TYPED_DATA_UINT16_LIST_H_
#define LIB_TONIC_TYPED_DATA_UINT16_LIST_H_

#include "third_party/dart/runtime/include/dart_api.h"
#include "tonic/converter/dart_converter.h"

namespace tonic {

// A simple wrapper around a Dart Uint16List. It uses Dart_TypedDataAcquireData
// to obtain a raw pointer to the data, which is released when this object is
// destroyed.
//
// This is designed to be used with DartConverter only.
class Uint16List {
 public:
  explicit Uint16List(Dart_Handle list);
  Uint16List(Uint16List&& other);
  Uint16List();
  ~Uint16List();

  uint16_t& at(intptr_t i) {
    TONIC_CHECK(i < num_elements_);
    return data_[i];
  }
  const uint16_t& at(intptr_t i) const {
    TONIC_CHECK(i < num_elements_);
    return data_[i];
  }

  uint16_t& operator[](intptr_t i) { return at(i); }
  const uint16_t& operator[](intptr_t i) const { return at(i); }

  const uint16_t* data() const { return data_; }
  intptr_t num_elements() const { return num_elements_; }
  Dart_Handle dart_handle() const { return dart_handle_; }

  void Release();

 private:
  uint16_t* data_;
  intptr_t num_elements_;
  Dart_Handle dart_handle_;

  Uint16List(const Uint16List& other) = delete;
};

template <>
struct DartConverter<Uint16List> {
  static void SetReturnValue(Dart_NativeArguments args, Uint16List val);
  static Uint16List FromArguments(Dart_NativeArguments args,
                                 int index,
                                 Dart_Handle& exception);
  static Dart_Handle ToDart(const uint16_t* buffer, unsigned int length);
};

}  // namespace tonic

#endif  // LIB_TONIC_TYPED_DATA_UINT16_LIST_H_
