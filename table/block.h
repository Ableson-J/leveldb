// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_TABLE_BLOCK_H_
#define STORAGE_LEVELDB_TABLE_BLOCK_H_

#include <cstddef>
#include <cstdint>

#include "leveldb/iterator.h"

namespace leveldb {

struct BlockContents;
class Comparator;

class Block {
 public:
  // Initialize the block with the specified contents.
  explicit Block(const BlockContents& contents);

  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;

  ~Block();

  size_t size() const { return size_; }
  Iterator* NewIterator(const Comparator* comparator);

 private:
  class Iter;

  uint32_t NumRestarts() const;

  /*
    const char* data_: 这是一个指向Block数据的指针，即Block的内容。它指向Block的起始位置。
    size_t size_: 这个变量存储了Block的大小，即数据的字节数。它表示Block的长度。
    uint32_t restart_offset_: 这个变量表示重启点数组(restart array)在data_中的偏移量。重启点数组是用于高效解析Block的索引结构。
    bool owned_: 这个变量表示Block是否拥有data_指针所指向的数据。当owned_为true时，
      表示Block拥有数据并负责释放它；当owned_为false时，表示Block只是引用了外部数据，并不负责释放。
   */

  const char* data_;
  size_t size_;
  uint32_t restart_offset_;  // Offset in data_ of restart array
  bool owned_;               // Block owns data_[]
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_BLOCK_H_
