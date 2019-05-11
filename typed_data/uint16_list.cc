// Copyright 2015 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "tonic/typed_data/uint16_list.h"

#include <string.h>

#include "tonic/logging/dart_error.h"

namespace tonic {

Uint16List::Uint16List()
    : data_(nullptr), num_elements_(0), dart_handle_(nullptr) {}

Uint16List::Uint16List(Dart_Handle list)
    : data_(nullptr), num_elements_(0), dart_handle_(list) {
  if (Dart_IsNull(list))
    return;

  Dart_TypedData_Type type;
  Dart_TypedDataAcquireData(list, &type, reinterpret_cast<void**>(&data_),
                            &num_elements_);
  TONIC_DCHECK(!LogIfError(list));
  if (type != Dart_TypedData_kUint16)
    Dart_ThrowException(ToDart("Non-genuine Uint16List passed to engine."));
}

Uint16List::Uint16List(Uint16List&& other)
    : data_(other.data_),
      num_elements_(other.num_elements_),
      dart_handle_(other.dart_handle_) {
  other.data_ = nullptr;
  other.dart_handle_ = nullptr;
}

Uint16List::~Uint16List() {
  Release();
}

void Uint16List::Release() {
  if (data_) {
    Dart_TypedDataReleaseData(dart_handle_);
    data_ = nullptr;
    num_elements_ = 0;
    dart_handle_ = nullptr;
  }
}

Uint16List DartConverter<Uint16List>::FromArguments(Dart_NativeArguments args,
                                                    int index,
                                                    Dart_Handle& exception) {
  Dart_Handle list = Dart_GetNativeArgument(args, index);
  TONIC_DCHECK(!LogIfError(list));
  return Uint16List(list);
}

void DartConverter<Uint16List>::SetReturnValue(Dart_NativeArguments args,
                                               Uint16List val) {
  Dart_SetReturnValue(args, val.dart_handle());
}

Dart_Handle DartConverter<Uint16List>::ToDart(const uint16_t* buffer,
                                              unsigned int length) {
  const intptr_t buffer_length = static_cast<intptr_t>(length);
  Dart_Handle array = Dart_NewTypedData(Dart_TypedData_kUint16, buffer_length);
  TONIC_DCHECK(!LogIfError(array));
  {
    Dart_TypedData_Type type;
    void* data = nullptr;
    intptr_t data_length = 0;
    Dart_TypedDataAcquireData(array, &type, &data, &data_length);
    TONIC_CHECK(type == Dart_TypedData_kUint16);
    TONIC_CHECK(data);
    TONIC_CHECK(data_length == buffer_length);
    memmove(data, buffer, data_length * sizeof(uint16_t));
    Dart_TypedDataReleaseData(array);
  }
  return array;
}

}  // namespace tonic
