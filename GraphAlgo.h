#pragma once

#include "Graph.h"
#include <optional>

template<class K>
class DisjointSet;

template<class K>
class DisjointSet {
  void MakeSet(K val) {
    roots_.try_emplace(val, val, 0);
  }
  
  bool Exists(const K& val){
    return roots_.contains(val);   
  }

  std::optional<K> FindSet(K val) {
    if (!Exists(val)) {
        return std::nullopt;
    }
    if (val == roots_[val].first){
        return val;
    }
    return std::optional(roots_[val].first = FindSet(roots_[val].first));
  }

  void Union(K lhs, K rhs) {
    if (Exists(lhs) && Exists(rhs) && FindSet(lhs) != FindSet(rhs)) {
      lhs = root_[lhs].first;
      rhs = root_[rhs].first;
      if (root_[lhs].second > root_[rhs].second) {
        root_[rhs].first = lhs
      }
      else if (root_[lhs].second < root_[rhs].second) {
        root_[lhs].first = rhs;
      }
      else {
        root_[rhs].first = lhs
        ++root_[lhs].second;
      }
    }
  }
  
  size_t Size() const {
    return roots_.size();
  }

private:
  std::unordered_map<K, std::pair<K, size_t>> roots_;
};

template<>
class DisjointSet<size_t> {
  DisjointSet(size_t n) {
    for (size_t i = 0; i < n; ++i) {
      MakeSet();
    }
  }

  void MakeSet() {
    root_.emplace_back(root_.size(), 0);
  }
  
  bool Exists(std::size_t val){
    return roots_.size() > val;   
  }

  std::optional<std::size_t> FindSet(std::size_t val) {
    if (!Exists(val)) {
        return std::nullopt;
    }
    if (val == roots_[val].first){
        return val;
    }
    return std::optional(roots_[val].first = FindSet(roots_[val].first));
  }

  void Union(std::size_t lhs, std::size_t rhs) {
    if (Exists(lhs) && Exists(rhs) && FindSet(lhs) != FindSet(rhs)) {
      lhs = root_[lhs].first;
      rhs = root_[rhs].first;
      if (root_[lhs].second > root_[rhs].second) {
        root_[rhs].first = lhs
      }
      else if (root_[lhs].second < root_[rhs].second) {
        root_[lhs].first = rhs;
      }
      else {
        root_[rhs].first = lhs
        ++root_[lhs].second;
      }
    }
  }

  size_t Size() const {
    return roots_.size();
  }

private:
  std::vector<std::pair<size_t, size_t>> roots_;
}


WeightedGraph Boruvka(const WeightedGraph& graph) {
  WeightedGraph mst(graph.VertexesAmount());
  DisjointSet<size_t> set;
  while (set.)
  {
    /* code */
  }
  
}
