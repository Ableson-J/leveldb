// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_TABLE_BLOCK_BUILDER_H_
#define STORAGE_LEVELDB_TABLE_BLOCK_BUILDER_H_

#include <cstdint>
#include <vector>

#include "leveldb/slice.h"

namespace leveldb {

struct Options;

class BlockBuilder {
 public:
  explicit BlockBuilder(const Options* options);

  BlockBuilder(const BlockBuilder&) = delete;
  BlockBuilder& operator=(const BlockBuilder&) = delete;

  // Reset the contents as if the BlockBuilder was just constructed.
  void Reset();

  // REQUIRES: Finish() has not been called since the last call to Reset().
  // REQUIRES: key is larger than any previously added key
  void Add(const Slice& key, const Slice& value);

  // Finish building the block and return a slice that refers to the
  // block contents.  The returned slice will remain valid for the
  // lifetime of this builder or until Reset() is called.
  Slice Finish();

  // Returns an estimate of the current (uncompressed) size of the block
  // we are building.
  size_t CurrentSizeEstimate() const;

  // Return true iff no entries have been added since the last Reset()
  bool empty() const { return buffer_.empty(); }

  /*
  在LevelDB的buildBlock头文件中，以下是这些变量的作用：
  const Options* options_: 这是一个指向Options结构的指针，用于存储构建Block时的相关选项和配置。
  std::string buffer_: 这是一个目标缓冲区，用于存储构建过程中生成的数据。在构建Block期间，数据会被追加到该缓冲区中。
  std::vector<uint32_t> restarts_: 这是一个存储重启点(restart point)的数组。重启点是Block中一系列条目的分界点，
      用于在读取时进行快速跳转。该数组存储了重启点在buffer_中的偏移量。
  int counter_: 这个变量用于记录从上一个重启点到当前位置已经插入的条目数量。它在构建Block时不断更新，用于计算下一个重启点。
  bool finished_: 这个变量用于标记构建Block的完成状态。当Finish()方法被调用时，它会被设置为true，表示Block已经构建完成。
  std::string last_key_: 这个变量用于存储上一个插入的条目的键。它在构建过程中用于检测是否有序插入条目，并进行一些相关的优化。
*/
 private:
  const Options* options_;
  std::string buffer_;              // Destination buffer
  std::vector<uint32_t> restarts_;  // Restart points
  int counter_;                     // Number of entries emitted since restart
  bool finished_;                   // Has Finish() been called?
  std::string last_key_;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_BLOCK_BUILDER_H_
