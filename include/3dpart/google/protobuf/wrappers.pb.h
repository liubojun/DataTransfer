﻿// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/wrappers.proto

#ifndef PROTOBUF_google_2fprotobuf_2fwrappers_2eproto__INCLUDED
#define PROTOBUF_google_2fprotobuf_2fwrappers_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace google {
namespace protobuf {

// Internal implementation detail -- do not call these.
void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

class BoolValue;
class BytesValue;
class DoubleValue;
class FloatValue;
class Int32Value;
class Int64Value;
class StringValue;
class UInt32Value;
class UInt64Value;

// ===================================================================

class LIBPROTOBUF_EXPORT DoubleValue : public ::google::protobuf::Message {
 public:
  DoubleValue();
  virtual ~DoubleValue();

  DoubleValue(const DoubleValue& from);

  inline DoubleValue& operator=(const DoubleValue& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const DoubleValue& default_instance();

  void Swap(DoubleValue* other);

  // implements Message ----------------------------------------------

  inline DoubleValue* New() const { return New(NULL); }

  DoubleValue* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DoubleValue& from);
  void MergeFrom(const DoubleValue& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(DoubleValue* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional double value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  double value() const;
  void set_value(double value);

  // @@protoc_insertion_point(class_scope:google.protobuf.DoubleValue)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  double value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static DoubleValue* default_instance_;
};
// -------------------------------------------------------------------

class LIBPROTOBUF_EXPORT FloatValue : public ::google::protobuf::Message {
 public:
  FloatValue();
  virtual ~FloatValue();

  FloatValue(const FloatValue& from);

  inline FloatValue& operator=(const FloatValue& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const FloatValue& default_instance();

  void Swap(FloatValue* other);

  // implements Message ----------------------------------------------

  inline FloatValue* New() const { return New(NULL); }

  FloatValue* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FloatValue& from);
  void MergeFrom(const FloatValue& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(FloatValue* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional float value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  float value() const;
  void set_value(float value);

  // @@protoc_insertion_point(class_scope:google.protobuf.FloatValue)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  float value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static FloatValue* default_instance_;
};
// -------------------------------------------------------------------

class LIBPROTOBUF_EXPORT Int64Value : public ::google::protobuf::Message {
 public:
  Int64Value();
  virtual ~Int64Value();

  Int64Value(const Int64Value& from);

  inline Int64Value& operator=(const Int64Value& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Int64Value& default_instance();

  void Swap(Int64Value* other);

  // implements Message ----------------------------------------------

  inline Int64Value* New() const { return New(NULL); }

  Int64Value* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Int64Value& from);
  void MergeFrom(const Int64Value& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Int64Value* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int64 value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  ::google::protobuf::int64 value() const;
  void set_value(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:google.protobuf.Int64Value)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::int64 value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static Int64Value* default_instance_;
};
// -------------------------------------------------------------------

class LIBPROTOBUF_EXPORT UInt64Value : public ::google::protobuf::Message {
 public:
  UInt64Value();
  virtual ~UInt64Value();

  UInt64Value(const UInt64Value& from);

  inline UInt64Value& operator=(const UInt64Value& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const UInt64Value& default_instance();

  void Swap(UInt64Value* other);

  // implements Message ----------------------------------------------

  inline UInt64Value* New() const { return New(NULL); }

  UInt64Value* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const UInt64Value& from);
  void MergeFrom(const UInt64Value& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(UInt64Value* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint64 value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  ::google::protobuf::uint64 value() const;
  void set_value(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:google.protobuf.UInt64Value)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::uint64 value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static UInt64Value* default_instance_;
};
// -------------------------------------------------------------------

class LIBPROTOBUF_EXPORT Int32Value : public ::google::protobuf::Message {
 public:
  Int32Value();
  virtual ~Int32Value();

  Int32Value(const Int32Value& from);

  inline Int32Value& operator=(const Int32Value& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Int32Value& default_instance();

  void Swap(Int32Value* other);

  // implements Message ----------------------------------------------

  inline Int32Value* New() const { return New(NULL); }

  Int32Value* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Int32Value& from);
  void MergeFrom(const Int32Value& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Int32Value* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  ::google::protobuf::int32 value() const;
  void set_value(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:google.protobuf.Int32Value)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::int32 value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static Int32Value* default_instance_;
};
// -------------------------------------------------------------------

class LIBPROTOBUF_EXPORT UInt32Value : public ::google::protobuf::Message {
 public:
  UInt32Value();
  virtual ~UInt32Value();

  UInt32Value(const UInt32Value& from);

  inline UInt32Value& operator=(const UInt32Value& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const UInt32Value& default_instance();

  void Swap(UInt32Value* other);

  // implements Message ----------------------------------------------

  inline UInt32Value* New() const { return New(NULL); }

  UInt32Value* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const UInt32Value& from);
  void MergeFrom(const UInt32Value& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(UInt32Value* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint32 value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  ::google::protobuf::uint32 value() const;
  void set_value(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:google.protobuf.UInt32Value)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::uint32 value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static UInt32Value* default_instance_;
};
// -------------------------------------------------------------------

class LIBPROTOBUF_EXPORT BoolValue : public ::google::protobuf::Message {
 public:
  BoolValue();
  virtual ~BoolValue();

  BoolValue(const BoolValue& from);

  inline BoolValue& operator=(const BoolValue& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const BoolValue& default_instance();

  void Swap(BoolValue* other);

  // implements Message ----------------------------------------------

  inline BoolValue* New() const { return New(NULL); }

  BoolValue* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const BoolValue& from);
  void MergeFrom(const BoolValue& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(BoolValue* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional bool value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  bool value() const;
  void set_value(bool value);

  // @@protoc_insertion_point(class_scope:google.protobuf.BoolValue)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  bool value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static BoolValue* default_instance_;
};
// -------------------------------------------------------------------

class LIBPROTOBUF_EXPORT StringValue : public ::google::protobuf::Message {
 public:
  StringValue();
  virtual ~StringValue();

  StringValue(const StringValue& from);

  inline StringValue& operator=(const StringValue& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const StringValue& default_instance();

  void Swap(StringValue* other);

  // implements Message ----------------------------------------------

  inline StringValue* New() const { return New(NULL); }

  StringValue* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const StringValue& from);
  void MergeFrom(const StringValue& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(StringValue* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  const ::std::string& value() const;
  void set_value(const ::std::string& value);
  void set_value(const char* value);
  void set_value(const char* value, size_t size);
  ::std::string* mutable_value();
  ::std::string* release_value();
  void set_allocated_value(::std::string* value);

  // @@protoc_insertion_point(class_scope:google.protobuf.StringValue)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static StringValue* default_instance_;
};
// -------------------------------------------------------------------

class LIBPROTOBUF_EXPORT BytesValue : public ::google::protobuf::Message {
 public:
  BytesValue();
  virtual ~BytesValue();

  BytesValue(const BytesValue& from);

  inline BytesValue& operator=(const BytesValue& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const BytesValue& default_instance();

  void Swap(BytesValue* other);

  // implements Message ----------------------------------------------

  inline BytesValue* New() const { return New(NULL); }

  BytesValue* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const BytesValue& from);
  void MergeFrom(const BytesValue& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(BytesValue* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional bytes value = 1;
  void clear_value();
  static const int kValueFieldNumber = 1;
  const ::std::string& value() const;
  void set_value(const ::std::string& value);
  void set_value(const char* value);
  void set_value(const void* value, size_t size);
  ::std::string* mutable_value();
  ::std::string* release_value();
  void set_allocated_value(::std::string* value);

  // @@protoc_insertion_point(class_scope:google.protobuf.BytesValue)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr value_;
  mutable int _cached_size_;
  friend void LIBPROTOBUF_EXPORT protobuf_AddDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_AssignDesc_google_2fprotobuf_2fwrappers_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2fwrappers_2eproto();

  void InitAsDefaultInstance();
  static BytesValue* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// DoubleValue

// optional double value = 1;
inline void DoubleValue::clear_value() {
  value_ = 0;
}
inline double DoubleValue::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.DoubleValue.value)
  return value_;
}
inline void DoubleValue::set_value(double value) {
  
  value_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.DoubleValue.value)
}

// -------------------------------------------------------------------

// FloatValue

// optional float value = 1;
inline void FloatValue::clear_value() {
  value_ = 0;
}
inline float FloatValue::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.FloatValue.value)
  return value_;
}
inline void FloatValue::set_value(float value) {
  
  value_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.FloatValue.value)
}

// -------------------------------------------------------------------

// Int64Value

// optional int64 value = 1;
inline void Int64Value::clear_value() {
  value_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 Int64Value::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.Int64Value.value)
  return value_;
}
inline void Int64Value::set_value(::google::protobuf::int64 value) {
  
  value_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.Int64Value.value)
}

// -------------------------------------------------------------------

// UInt64Value

// optional uint64 value = 1;
inline void UInt64Value::clear_value() {
  value_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 UInt64Value::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.UInt64Value.value)
  return value_;
}
inline void UInt64Value::set_value(::google::protobuf::uint64 value) {
  
  value_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.UInt64Value.value)
}

// -------------------------------------------------------------------

// Int32Value

// optional int32 value = 1;
inline void Int32Value::clear_value() {
  value_ = 0;
}
inline ::google::protobuf::int32 Int32Value::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.Int32Value.value)
  return value_;
}
inline void Int32Value::set_value(::google::protobuf::int32 value) {
  
  value_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.Int32Value.value)
}

// -------------------------------------------------------------------

// UInt32Value

// optional uint32 value = 1;
inline void UInt32Value::clear_value() {
  value_ = 0u;
}
inline ::google::protobuf::uint32 UInt32Value::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.UInt32Value.value)
  return value_;
}
inline void UInt32Value::set_value(::google::protobuf::uint32 value) {
  
  value_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.UInt32Value.value)
}

// -------------------------------------------------------------------

// BoolValue

// optional bool value = 1;
inline void BoolValue::clear_value() {
  value_ = false;
}
inline bool BoolValue::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.BoolValue.value)
  return value_;
}
inline void BoolValue::set_value(bool value) {
  
  value_ = value;
  // @@protoc_insertion_point(field_set:google.protobuf.BoolValue.value)
}

// -------------------------------------------------------------------

// StringValue

// optional string value = 1;
inline void StringValue::clear_value() {
  value_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& StringValue::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.StringValue.value)
  return value_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void StringValue::set_value(const ::std::string& value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:google.protobuf.StringValue.value)
}
inline void StringValue::set_value(const char* value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:google.protobuf.StringValue.value)
}
inline void StringValue::set_value(const char* value, size_t size) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:google.protobuf.StringValue.value)
}
inline ::std::string* StringValue::mutable_value() {
  
  // @@protoc_insertion_point(field_mutable:google.protobuf.StringValue.value)
  return value_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* StringValue::release_value() {
  
  return value_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void StringValue::set_allocated_value(::std::string* value) {
  if (value != NULL) {
    
  } else {
    
  }
  value_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set_allocated:google.protobuf.StringValue.value)
}

// -------------------------------------------------------------------

// BytesValue

// optional bytes value = 1;
inline void BytesValue::clear_value() {
  value_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& BytesValue::value() const {
  // @@protoc_insertion_point(field_get:google.protobuf.BytesValue.value)
  return value_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void BytesValue::set_value(const ::std::string& value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:google.protobuf.BytesValue.value)
}
inline void BytesValue::set_value(const char* value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:google.protobuf.BytesValue.value)
}
inline void BytesValue::set_value(const void* value, size_t size) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:google.protobuf.BytesValue.value)
}
inline ::std::string* BytesValue::mutable_value() {
  
  // @@protoc_insertion_point(field_mutable:google.protobuf.BytesValue.value)
  return value_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* BytesValue::release_value() {
  
  return value_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void BytesValue::set_allocated_value(::std::string* value) {
  if (value != NULL) {
    
  } else {
    
  }
  value_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set_allocated:google.protobuf.BytesValue.value)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_google_2fprotobuf_2fwrappers_2eproto__INCLUDED
