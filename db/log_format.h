// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Log format information shared by reader and writer.
// See ../doc/log_format.md for more detail.

#ifndef STORAGE_LEVELDB_DB_LOG_FORMAT_H_
#define STORAGE_LEVELDB_DB_LOG_FORMAT_H_

namespace leveldb {
namespace log {

enum RecordType {
  // Zero is reserved for preallocated files
  kZeroType = 0,

  // ��ʾ���recordû�п�Խblock
  kFullType = 1,

  // For fragments
  // ��ʾrecord��ͷ�Ĳ���
  kFirstType = 2,
  // ��ʾrecord�м�Ĳ���
  kMiddleType = 3,
  // ��ʾrecordβ�Ͳ���
  kLastType = 4
};
static const int kMaxRecordType = kLastType;

// block��С32KB
static const int kBlockSize = 32768;

// Header is checksum (4 bytes), length (2 bytes), type (1 byte).
// ����λ4�ֽ� ����2�ֽ� ����1�ֽ�
static const int kHeaderSize = 4 + 2 + 1;

}  // namespace log
}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_LOG_FORMAT_H_
