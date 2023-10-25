#include "primer/trie.h"
#include <string_view>
#include "common/exception.h"
// #include <iostream>

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  std::shared_ptr<const TrieNode> node = this->root_;

  if (node == nullptr) { return nullptr; }

  for (auto &c : key) {
    if (node->children_.count(c)) {
      node = node->children_.at(c);
    }
    else {
      return nullptr;
    }
  }

  auto node_with_value = std::dynamic_pointer_cast<const TrieNodeWithValue<T>>(node);

  if (node_with_value == nullptr) {
    return nullptr;
  }

  return node_with_value->value_.get();

  // throw NotImplementedException("Trie::Get is not implemented.");

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  std::shared_ptr<const TrieNode> new_root = nullptr;
  std::shared_ptr<TrieNode> new_node = nullptr;
  std::shared_ptr<TrieNode> new_node_prev = nullptr;
  std::shared_ptr<const TrieNode> old_node = nullptr;

  for (auto it = key.begin() - 1; it < key.end(); it++) {
    if (it == key.begin() - 1) {
      old_node = this->root_;
    }
    else if (old_node) {
      old_node = old_node->children_.count(*it) > 0 ?
        old_node->children_.at(*it) : nullptr;
    }
    
    if (it == key.end() - 1) {
      auto value_ptr = std::make_shared<T>(std::move(value));
      new_node = old_node ?
        new_node = std::make_shared<TrieNodeWithValue<T>>(old_node->children_, value_ptr) :
        new_node = std::make_shared<TrieNodeWithValue<T>>(value_ptr);
    }
    else {
      new_node = old_node ?
        old_node->Clone() :
        std::make_shared<TrieNode>();
    }

    if (it == key.begin() - 1) {
      new_root = new_node;
      new_node_prev = new_node;
    }
    else {
      new_node_prev->children_[*it] = new_node;
      new_node_prev = new_node;
    }
  }

  return Trie(new_root);

  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
}

auto Trie::Remove(std::string_view key) const -> Trie {
  std::shared_ptr<const TrieNode> root = this->root_;
  auto it_cut = key.begin() - 1;

  if (root == nullptr) { return *this; }

  for (auto it = key.begin(); it < key.end(); it++) {
    if (root->children_.count(*it) > 0) {
      root = root->children_.at(*it);
    }
    else {
      return *this;
    }

    if (it != key.end() - 1) {
      if (!root->is_value_node_ && root->children_.size() == 1) {
        if (it_cut == key.end()) {
          it_cut = it;
        }
      }
      else {
        it_cut = key.end();
      }
    }
    else {
      if (root->children_.empty()) {
        if (it_cut == key.end()) {
          it_cut = it;
        }
      }
      else {
        it_cut = key.end();
      }
    }
  }

  std::shared_ptr<const TrieNode> new_root = nullptr;
  std::shared_ptr<TrieNode> new_node = nullptr;
  std::shared_ptr<TrieNode> new_node_prev = nullptr;
  std::shared_ptr<const TrieNode> old_node = nullptr;

  for (auto it = key.begin() - 1; it < key.end(); it++) {
    if (it_cut == it) {
      if (it_cut != key.begin() - 1) {
        new_node_prev->children_.erase(*it);
      }
      break;
    }

    if (it == key.begin() - 1) {
      old_node = this->root_;
    }
    else if (old_node) {
      old_node = old_node->children_.count(*it) > 0 ?
        old_node->children_.at(*it) : nullptr;
    }
    
    if (it == key.end() - 1) {
      new_node = old_node ?
        new_node = std::make_shared<TrieNode>(old_node->children_) :
        std::make_shared<TrieNode>();
    }
    else {
      new_node = old_node ?
        old_node->Clone() :
        std::make_shared<TrieNode>();
    }

    if (it == key.begin() - 1) {
      new_root = new_node;
      new_node_prev = new_node;
    }
    else {
      new_node_prev->children_[*it] = new_node;
      new_node_prev = new_node;
    }
  }

  return Trie(new_root);


  // std::shared_ptr<const TrieNode> root = this->root_;

  // if (root == nullptr) { return *this; }

  // auto it_cut = key.begin();
  // for (auto it = key.begin() ; it < key.end(); it++) {
  //   if (root->children_.count(*it) > 0) {
  //     if (root->children_.size() == 1) {
  //       it_cut = it_cut == nullptr ? it_cut : it;
  //     }
  //     else {
  //       it_cut = nullptr;
  //     }
  //     root = root->children_.at(*it);
  //   }
  //   else {
  //     return *this;
  //   }
  // }

  // if (it_cut == key.begin()) {
  //   return Trie(nullptr);
  // }
  
  // std::shared_ptr<const TrieNode> new_root = nullptr;
  // std::shared_ptr<TrieNode> new_node = nullptr;
  // std::shared_ptr<TrieNode> new_node_prev = nullptr;
  // std::shared_ptr<const TrieNode> old_node = nullptr;

  // for (auto it = key.begin() - 1; it < key.end(); it++) {
  //   if (it == key.begin() - 1) {
  //     old_node = this->root_;
  //   }
  //   else {
  //     old_node = old_node->children_.at(*it);
  //   }

  //   if (old_node->is_value_node_) {
  //     auto old_node_with_value = std::static_pointer_cast<const TrieNodeWithValue<T>>(old_node);
  //     new_node = std::make_shared<TrieNodeWithValue<T>>(old_node_with_value->value_);
  //   }
  //   else {
  //     new_node = std::make_shared<TrieNode>();
  //   }

  //   if (old_node) {
  //     for (auto [k, v] : old_node->children_) {
  //       auto child = v->is_value_node_ ?
  //         std::static_pointer_cast<const TrieNodeWithValue<T>>(v)->Clone() : v->Clone();
  //       new_node->children_[k] = std::move(child);
  //     }
  //   }

  //   if (it == key.begin() - 1) {
  //     new_root = new_node;
  //     new_node_prev = new_node;
  //   }
  //   else {
  //     new_node_prev->children_[*it] = new_node;
  //     new_node_prev = new_node;
  //   }
  // }

  // return Trie(new_root);

  // return root->is_value_node_ ? 
  //   std::static_pointer_cast<const TrieNodeWithValue<T>>(root)->value_.get() : nullptr;

  throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
