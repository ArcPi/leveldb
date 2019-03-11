// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Slice is a simple structure containing a pointer into some external
// storage and a size.  The user of a Slice must ensure that the slice
// is not used after the corresponding external storage has been
// deallocated.
//
// Multiple threads can invoke const methods on a Slice without
// external synchronization, but if any of the threads may call a
// non-const method, all threads accessing the same Slice must use
// external synchronization.

// slice是leveldb自定义的结构体，对string的封装。能够很方便地跟string进行转化。底层很多操作都是以slice为对象。
// slice包含一个指向外部存储的指针和记录其大小的变量。使用slice需要保证，当外部存储被删除后，与外部存储相关联的slice不能再被使用
// 不需要和外界同步，当多线程调用slice的静态方法。但是，如果多线程调用非静态方法，必须和外界同步

#ifndef STORAGE_LEVELDB_INCLUDE_SLICE_H_
#define STORAGE_LEVELDB_INCLUDE_SLICE_H_

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include "leveldb/export.h"

namespace leveldb {

class LEVELDB_EXPORT Slice {
 public:
  // Create an empty slice.
  // 创建空的slice
  Slice() : data_(""), size_(0) { }

  // Create a slice that refers to d[0,n-1].
  // 创建创建长度为n的slice
  Slice(const char* d, size_t n) : data_(d), size_(n) { }

  // Create a slice that refers to the contents of "s"
  // 从string创建slice
  Slice(const std::string& s) : data_(s.data()), size_(s.size()) { }

  // Create a slice that refers to s[0,strlen(s)-1]
  // 从char[]数组创建slice
  Slice(const char* s) : data_(s), size_(strlen(s)) { }

  // Intentionally copyable.
  // 拷贝构造函数 拷贝赋值运算符
  Slice(const Slice&) = default;
  Slice& operator=(const Slice&) = default;

  // Return a pointer to the beginning of the referenced data
  const char* data() const { return data_; }

  // Return the length (in bytes) of the referenced data
  // 返回slice的长度
  size_t size() const { return size_; }

  // Return true iff the length of the referenced data is zero
  // 查看slice是否为空
  bool empty() const { return size_ == 0; }

  // Return the ith byte in the referenced data.
  // REQUIRES: n < size()
  // 重载下标运算符
  char operator[](size_t n) const {
    assert(n < size());
    return data_[n];
  }

  // Change this slice to refer to an empty array
  // 将slice清空
  void clear() { data_ = ""; size_ = 0; }

  // Drop the first "n" bytes from this slice.
  // 舍弃前n个字节
  void remove_prefix(size_t n) {
    assert(n <= size());
    data_ += n;
    size_ -= n;
  }

  // Return a string that contains the copy of the referenced data.
  // slice转换为string类型
  std::string ToString() const { return std::string(data_, size_); }

  // Three-way comparison.  Returns value:
  //   <  0 iff "*this" <  "b",
  //   == 0 iff "*this" == "b",
  //   >  0 iff "*this" >  "b"
  // 实现slice大小比较
  int compare(const Slice& b) const;

  // Return true iff "x" is a prefix of "*this"
  // 如果x属于"*this"的前缀，返回为true
  bool starts_with(const Slice& x) const {
    return ((size_ >= x.size_) &&
            (memcmp(data_, x.data_, x.size_) == 0));
  }

 private:
  const char* data_;
  size_t size_;
};

inline bool operator==(const Slice& x, const Slice& y) {
  return ((x.size() == y.size()) &&
          (memcmp(x.data(), y.data(), x.size()) == 0));
}

inline bool operator!=(const Slice& x, const Slice& y) {
  return !(x == y);
}

inline int Slice::compare(const Slice& b) const {
  const size_t min_len = (size_ < b.size_) ? size_ : b.size_;
  int r = memcmp(data_, b.data_, min_len);
  if (r == 0) {
    if (size_ < b.size_) r = -1;
    else if (size_ > b.size_) r = +1;
  }
  return r;
}

}  // namespace leveldb


#endif  // STORAGE_LEVELDB_INCLUDE_SLICE_H_
