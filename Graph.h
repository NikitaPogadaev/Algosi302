#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <queue>
#include <span>
#include <unordered_map>

#include <unordered_set>
#include <vector>

template <typename K>
concept KeyForVertex = std::default_initializable<K> && std::copy_constructible<K> && requires(K &a, K &b) { a = b; };

const auto kDefaultDfsFoo = [](std::size_t a) { return a != 0; };

template <class Edge, class Vertex, class GraphPayLoad>
class IGraph;
template <class K, class VertexPayload>
  requires KeyForVertex<K>
class DefaultVertex;
template <class Vertex, class EdgePayload>
class Edge;
class DefaultGraph;
class WeightedGraph;
class OrientedWeightedGraph;

struct Nil {
  Nil() = default;
  explicit operator bool() const {
    return true;
  }
};

struct NotNil {
  NotNil() = default;
  explicit operator bool() const {
    return false;
  }
};

struct TinToutParent {
  friend class DefaultGraph;
  friend class NonOrientedDefaultGraph;
  friend class WeightedGraph;
  friend class OrientedWeightedGraph;

  int64_t tin_{-1};
  int64_t tout_{-1};
  std::optional<std::size_t> parent_{std::nullopt};

  int64_t GetTin() {
    return tin_;
  }

  int64_t GetTout() {
    return tout_;
  }

  std::optional<std::size_t> GetParent() {
    return parent_;
  }

 protected:
  void SetTin(int64_t x) {
    tin_ = x;
  }

  void SetTout(int64_t x) {
    tout_ = x;
  }

  void SetParent(std::size_t x) {
    parent_ = std::optional<std::size_t>(x);
  }
};

template <class Edge, class Vertex, class GraphPayLoad = Nil>
class IGraph : public GraphPayLoad {
 public:
  using VertexType = Vertex;
  using EdgeType = Edge;
  using VertexRef = Vertex &;
  using EdgeRef = Edge &;
  using PayLoad = GraphPayLoad;
  virtual void InsertEdge(const Vertex &, const Vertex &) = 0;
  virtual void InsertEdge(const typename Vertex::Key &, const typename Vertex::Key &) = 0;
  virtual void InsertVertex(const typename Vertex::Key &key) = 0;
  virtual bool CheckEdge(const typename Vertex::Key &, const typename Vertex::Key &) const = 0;
  virtual bool CheckVertex(const typename Vertex::Key &) const = 0;
  virtual const EdgeType &FindEdge(const typename Vertex::Key &, const typename Vertex::Key &) const = 0;
  virtual const VertexType &FindVertex(const typename Vertex::Key &) const = 0;
  virtual EdgeType &FindEdge(const typename Vertex::Key &, const typename Vertex::Key &) = 0;
  virtual VertexType &FindVertex(const typename Vertex::Key &) = 0;
  virtual size_t EdgesAmount() const = 0;
  virtual size_t VertexesAmount() const = 0;
  virtual ~IGraph() = default;
};

template <class K = std::size_t, class VertexPayload = Nil>
  requires KeyForVertex<K>
class DefaultVertex : public VertexPayload {
 public:
  using Key = K;
  using PayLoad = VertexPayload;
  DefaultVertex() = delete;

  template <class... Args>
  explicit DefaultVertex(const K &key, const Args &...args) : VertexPayload(args...), key_(key) {
  }
  DefaultVertex(const DefaultVertex &other) = default;
  DefaultVertex &operator=(const DefaultVertex &other) {
    key_ = other.key_;
    return *this;
  }
  K GetKey() const {
    return key_;
  }

 private:
  K key_;
};

template <class Vertex, class EdgePayload = Nil>
class Edge : public EdgePayload {
 public:
  using Key = typename Vertex::Key;
  using VertexType = Vertex;
  using PayLoad = EdgePayload;
  Edge() = delete;
  template <class... Args>
  Edge(const Vertex &begin, const Vertex &end, const Args &...args) : EdgePayload(args...), begin_(begin), end_(end) {
  }
  Edge(const Edge &other) = default;
  ~Edge() = default;

  Edge &operator=(const Edge &other) {
    if (this == &other) {
      return *this;
    }
    begin_ = other.begin_;
    end_ = other.end_;
    return *this;
  }

  const Vertex &GetBegin() const {
    return begin_;
  }

  const Vertex &GetEnd() const {
    return end_;
  }

 private:
  Vertex begin_;
  Vertex end_;
};

struct NilNil {
  NilNil() = default;
  explicit operator bool() const {
    return true;
  }
  Nil gag_;
  NotNil n_gag_;
  DefaultVertex<std::size_t, TinToutParent> NullVertex{std::dynamic_extent};
};

class DefaultGraph : public IGraph<Nil, DefaultVertex<std::size_t, TinToutParent>, NilNil> {
 public:
  DefaultGraph() = default;
  ~DefaultGraph() override = default;
  DefaultGraph &operator=(const DefaultGraph &) = default;

  bool IsNull(const VertexType &vert) const {
    return vert.GetKey() == std::dynamic_extent;
  }

  const VertexType &GetNull() const {
    return NullVertex;
  }

  bool CheckVertex(const size_t &key) const override {
    return vertexes_.size() > key;
  }

  bool CheckEdge(const size_t &first, const size_t &second) const override {
    if ((CheckVertex(first)) && (CheckVertex(second))) {
      if (edges_[first].find(second) != edges_[first].end()) {
        return true;
      }
    }
    return false;
  }

  const VertexType &FindVertex(const size_t &key) const override {
    if (vertexes_.size() > key) {
      return vertexes_[key];
    }
    return NullVertex;
  }

  const EdgeType &FindEdge(const size_t &first, const size_t &second) const override {
    if (first == second) {  // codestyle error: unused parameter
    }
    return gag_;
  }

  VertexType &FindVertex(const size_t &key) override {
    if (vertexes_.size() > key) {
      return vertexes_[key];
    }
    return NullVertex;
  }

  EdgeType &FindEdge(const size_t &first, const size_t &second) override {
    if (first == second) {  // codestyle error: unused parameter
    }
    return gag_;
  }

  void InsertVertex(const std::size_t &key) override {
    if (key != vertexes_.size()) {
      return;
    }
    ++vert_amount;
    vertexes_.emplace_back(vertexes_.size());
    auto empty_set = std::unordered_set<size_t>();
    edges_.push_back(empty_set);
  }

  void InsertVertex() {
    ++vert_amount;
    vertexes_.emplace_back(vertexes_.size());
    auto empty_set = std::unordered_set<size_t>();
    edges_.push_back(empty_set);
  }

  explicit DefaultGraph(std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
      InsertVertex();
    }
  }

  void InsertEdge(const VertexType &first, const VertexType &second) override {
    if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) && (first.GetKey() != second.GetKey()) &&
        !(CheckEdge(first.GetKey(), second.GetKey()))) {
      edges_[first.GetKey()].insert(second.GetKey());
      edges_[second.GetKey()].insert(first.GetKey());
    }
    ++edge_amount;
  }

  void InsertEdge(const size_t &first, const size_t &second) override {
    if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) && !(CheckEdge(first, second))) {
      edges_[first].insert(second);
      edges_[second].insert(first);
    }
    ++edge_amount;
  }

  virtual void EraseEdge(const VertexType &first, const VertexType &second) {
    if ((first.GetKey() != second.GetKey()) && CheckEdge(first.GetKey(), second.GetKey())) {
      edges_[first.GetKey()].erase(second.GetKey());
      edges_[second.GetKey()].erase(first.GetKey());
    }
    --edge_amount;
  }

  virtual void EraseEdge(const size_t &first, const size_t &second) {
    if ((first != second) && CheckEdge(first, second)) {
      edges_[first].erase(second);
      edges_[second].erase(first);
    }
    --edge_amount;
  }

  virtual void IsolateVertex(const VertexType &first) {
    if (CheckVertex(first.GetKey())) {
      for (auto i : edges_[first.GetKey()]) {
        edges_[i].erase(first.GetKey());
        --edge_amount;
      }
      edges_[first.GetKey()].clear();
    }
  }

  size_t EdgesAmount() const override {
    return edge_amount;
  }

  size_t VertexesAmount() const override {
    return vert_amount;
  }

  class TopSortFoo {
   public:
    explicit TopSortFoo(std::vector<VertexType::Key> &buf) : buf_(buf) {
    }
    void operator()(std::size_t vert) {
      buf_.push_back(vert);
    }
    std::vector<VertexType::Key> &buf_;
  };

  void BFS() {
    std::vector<bool> used(vert_amount, false);
    for (std::size_t i = 0; i < vert_amount; ++i) {
      if (!used[i]) {
        std::queue<std::size_t> qs;
        qs.push(i);
        used[i] = true;
        while (!qs.empty()) {
          std::size_t tmp = qs.front();
          qs.pop();
          for (auto ind : edges_[tmp]) {
            if (!used[ind]) {
              used[ind] = true;
              vertexes_[ind].SetParent(tmp);
              qs.push(ind);
            }
          }
        }
      }
    }
  }

  void DFS() {
    std::size_t co = 0;
    std::vector<bool> used(vert_amount, false);
    for (std::size_t i = 0; i < vert_amount; ++i) {
      if (used[i]) {
        continue;
      }
      RecDfs(i, used, co);
    }
  }

 protected:
  template <class OutFoo = decltype(kDefaultDfsFoo)>
  void RecDfs(std::size_t vert, std::vector<bool> &used, std::size_t &co, OutFoo out_foo = kDefaultDfsFoo) {
    used[vert] = true;
    vertexes_[vert].SetTin(static_cast<int64_t>(co));
    for (auto ind : edges_[vert]) {
      if (used[ind]) {
        continue;
      }
      ++co;
      vertexes_[ind].SetParent(vert);
      RecDfs(ind, used, co, out_foo);
    }
    vertexes_[vert].SetTout(static_cast<int64_t>(co));
    out_foo(vert);
  }

  std::vector<VertexType> vertexes_;
  std::vector<std::unordered_set<size_t>> edges_;
  std::size_t edge_amount{0};
  std::size_t vert_amount{0};
};

class OrientedDefaultGraph : public DefaultGraph {
 public:
  OrientedDefaultGraph() = default;
  ~OrientedDefaultGraph() override = default;
  OrientedDefaultGraph &operator=(const OrientedDefaultGraph &) = default;

  explicit OrientedDefaultGraph(std::size_t n) : DefaultGraph(n) {
  }

  std::vector<VertexType::Key> TopSort() {
    std::size_t co = 0;
    std::vector<bool> used(vert_amount, false);
    std::vector<VertexType::Key> sorted;
    for (std::size_t i = 0; i < vert_amount; ++i) {
      if (used[i]) {
        continue;
      }
      RecDfs(i, used, co, TopSortFoo(sorted));
    }
    std::reverse(sorted.begin(), sorted.end());
    return sorted;
  }

  void InsertEdge(const VertexType &first, const VertexType &second) override {
    if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) && (first.GetKey() != second.GetKey()) &&
        !(CheckEdge(first.GetKey(), second.GetKey()))) {
      edges_[first.GetKey()].insert(second.GetKey());
    }
    ++edge_amount;
  }

  void InsertEdge(const size_t &first, const size_t &second) override {
    if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) && !(CheckEdge(first, second))) {
      edges_[first].insert(second);
    }
    ++edge_amount;
  }

  void EraseEdge(const VertexType &first, const VertexType &second) override {
    if ((first.GetKey() != second.GetKey()) && CheckEdge(first.GetKey(), second.GetKey())) {
      edges_[first.GetKey()].erase(second.GetKey());
    }
    --edge_amount;
  }

  void EraseEdge(const size_t &first, const size_t &second) override {
    if ((first != second) && CheckEdge(first, second)) {
      edges_[first].erase(second);
    }
    --edge_amount;
  }

  void IsolateVertex(const VertexType &first) override {
    if (CheckVertex(first.GetKey())) {
      edge_amount -= edges_[first.GetKey()].size();
      edges_[first.GetKey()].clear();
    }
  }

  OrientedDefaultGraph InvertedGraph() {
    OrientedDefaultGraph graph(vert_amount);
    for (std::size_t i = 0; i < vert_amount; ++i) {
      for (auto &j : edges_[i]) {
        graph.InsertEdge(j, i);
      }
    }
    return graph;
  }
};

class NonOrientedDefaultGraph : public DefaultGraph {

 public:
  NonOrientedDefaultGraph() = default;
  ~NonOrientedDefaultGraph() override = default;
  NonOrientedDefaultGraph &operator=(const NonOrientedDefaultGraph &) = default;

  explicit NonOrientedDefaultGraph(std::size_t n) : DefaultGraph(n) {
  }

  virtual std::vector<std::size_t> FindDist(std::size_t first, std::size_t last) {
    std::vector<bool> used(vert_amount, false);
    std::vector<std::size_t> way;

    std::queue<std::size_t> qs;
    qs.push(first);
    used[first] = true;
    bool found = false;
    while (!qs.empty()) {
      std::size_t tmp = qs.front();
      qs.pop();
      if (tmp == last) {
        found = true;
        break;
      }
      for (auto ind : edges_[tmp]) {
        if (!used[ind]) {
          used[ind] = true;
          vertexes_[ind].SetParent(tmp);
          if (ind == last) {
            found = true;
            break;
          }
          qs.push(ind);
        }
      }
    }

    if (!found) {
      return way;
    }
    std::size_t cur_vert = last;
    while (cur_vert != first) {
      way.push_back(cur_vert);
      cur_vert = vertexes_[cur_vert].GetParent().value();
    }
    way.push_back(first);
    std::reverse(way.begin(), way.end());
    return way;
  }
};





/////////////////////////////////////////////////////////////////



class WeightedGraph : public IGraph<Nil, DefaultVertex<size_t, TinToutParent>, NilNil> {
 public:
  WeightedGraph() = default;
  ~WeightedGraph() override = default;
  WeightedGraph& operator=(const WeightedGraph&) = default;

  bool IsNull(const VertexType& vert) const {
    return vert.GetKey() == std::dynamic_extent;
  }

  const VertexType& GetNull() const {
    return NullVertex;
  }

  bool CheckVertex(const size_t& key) const override {
    return (vertexes_.size() > key);
  }

  bool CheckEdge(const size_t& first, const size_t& second) const override {
    if ((CheckVertex(first)) && (CheckVertex(second))) {
      return (edges_[first].find(second) != edges_[first].end());
    }
    return false;
  }

  bool CheckEdge(const size_t& first, const size_t& second, int weight) const {
    if ((CheckVertex(first)) && (CheckVertex(second))) {
      return ((edges_[first].find(second) != edges_[first].end()) && ((edges_[first].find(second))->second == weight));
    }
    return false;
  }

  const VertexType& FindVertex(const size_t& key) const override {
    if (vertexes_.size() > key) {
      return vertexes_[key];
    }
    return NullVertex;
  }

  const EdgeType& FindEdge(const size_t&, const size_t&) const override {
    return gag_;
  }

  VertexType& FindVertex(const size_t& key) override {
    if (vertexes_.size() > key) {
      return vertexes_[key];
    }
    return NullVertex;
  }

  EdgeType& FindEdge(const size_t&, const size_t&) override {
    return gag_;
  }

  void InsertVertex(const size_t& key) override {
    if (key != vertexes_.size()) {
      return;
    }
    ++vert_amount;
    vertexes_.emplace_back(vertexes_.size());
    edges_.emplace_back();
  }

  void InsertVertex() {
    ++vert_amount;
    vertexes_.emplace_back(vertexes_.size());
    edges_.emplace_back();
  }

  explicit WeightedGraph(size_t n) : edges_() {
    for (size_t i = 0; i < n; ++i) {
      InsertVertex();
    }
  }

  void InsertEdge(const VertexType& first, const VertexType& second) override {
    if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) && (first.GetKey() != second.GetKey()) &&
        !(CheckEdge(first.GetKey(), second.GetKey()))) {
      edges_[first.GetKey()].emplace(second.GetKey(), 1);
      edges_[second.GetKey()].emplace(first.GetKey(), 1);
    }
    ++edge_amount;
  }

  virtual void InsertEdge(const VertexType& first, const VertexType& second, int weight) {
    if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) && (first.GetKey() != second.GetKey()) &&
        !(CheckEdge(first.GetKey(), second.GetKey()))) {
      edges_[first.GetKey()].emplace(second.GetKey(), weight);
      edges_[second.GetKey()].emplace(first.GetKey(), weight);
    }
    ++edge_amount;
  }

  void InsertEdge(const size_t& first, const size_t& second) override {
    if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) && !(CheckEdge(first, second))) {
      edges_[first].emplace(second, 1);
      edges_[second].emplace(first, 1);
    }
    ++edge_amount;
  }

  virtual void InsertEdge(const size_t& first, const size_t& second, int weight) {
    if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) && !(CheckEdge(first, second))) {
      edges_[first].emplace(second, weight);
      edges_[second].emplace(first, weight);
    }
    ++edge_amount;
  }

  virtual void EraseEdge(const VertexType& first, const VertexType& second) {
    if ((first.GetKey() != second.GetKey()) && CheckEdge(first.GetKey(), second.GetKey())) {
      edges_[first.GetKey()].erase(second.GetKey());
      edges_[second.GetKey()].erase(first.GetKey());
    }
    --edge_amount;
  }

  virtual void EraseEdge(const size_t& first, const size_t& second) {
    if ((first != second) && CheckEdge(first, second)) {
      edges_[first].erase(second);
      edges_[second].erase(first);
    }
    --edge_amount;
  }

  virtual void IsolateVertex(const VertexType& first) {
    if (CheckVertex(first.GetKey())) {
      for (auto i : edges_[first.GetKey()]) {
        edges_[i.first].erase(first.GetKey());
        --edge_amount;
      }
      edges_[first.GetKey()].clear();
    }
  }

  size_t EdgesAmount() const override {
    return edge_amount;
  }
  size_t VertexesAmount() const override {
    return vert_amount;
  }

  class TopSortFoo {
   public:
    explicit TopSortFoo(std::vector<VertexType::Key>& buf) : buf_(buf) {
    }
    void operator()(size_t vert) {
      buf_.push_back(vert);
    }
    std::vector<VertexType::Key>& buf_;
  };

  void BFS() {
    std::vector<bool> used(vert_amount, false);
    for (size_t i = 0; i < vert_amount; ++i) {
      if (!used[i]) {
        std::queue<size_t> qs;
        qs.push(i);
        used[i] = true;
        while (!qs.empty()) {
          size_t tmp = qs.front();
          qs.pop();
          for (auto ind : edges_[tmp]) {
            if (!used[ind.first]) {
              used[ind.first] = true;
              vertexes_[ind.first].SetParent(tmp);
              qs.push(ind.first);
            }
          }
        }
      }
    }
  }

  void DFS() {
    size_t co = 0;
    std::vector<bool> used(vert_amount, false);
    for (size_t i = 0; i < vert_amount; ++i) {
      if (used[i]) {
        continue;
      }
      RecDfs(i, used, co);
    }
  }

 protected:
  template <class OutFoo = decltype(kDefaultDfsFoo)>
  void RecDfs(size_t vert, std::vector<bool>& used, size_t& co, OutFoo out_foo = kDefaultDfsFoo) {
    used[vert] = true;
    vertexes_[vert].SetTin(static_cast<int>(co));
    for (auto ind : edges_[vert]) {
      if (used[ind.first]) {
        continue;
      }
      ++co;
      vertexes_[ind.first].SetParent(vert);
      RecDfs(ind.first, used, co, out_foo);
    }
    vertexes_[vert].SetTout(static_cast<int>(co));
    out_foo(vert);
  }

  std::vector<VertexType> vertexes_;
  std::vector<std::unordered_map<size_t, int>> edges_;
  size_t edge_amount{0};
  size_t vert_amount{0};
};

class OrientedWeightedGraph : public WeightedGraph {

 public:
  OrientedWeightedGraph() = default;
  ~OrientedWeightedGraph() override = default;
  OrientedWeightedGraph& operator=(const OrientedWeightedGraph&) = default;

  explicit OrientedWeightedGraph(size_t n) : WeightedGraph(n) {
  }

  void InsertEdge(const VertexType& first, const VertexType& second) override {
    if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) && (first.GetKey() != second.GetKey()) &&
        !(CheckEdge(first.GetKey(), second.GetKey()))) {
      edges_[first.GetKey()].emplace(second.GetKey(), 1);
    }
    ++edge_amount;
  }

  void InsertEdge(const VertexType& first, const VertexType& second, int weight) override {
    if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) && (first.GetKey() != second.GetKey()) &&
        !(CheckEdge(first.GetKey(), second.GetKey()))) {
      edges_[first.GetKey()].emplace(second.GetKey(), weight);
    }
    ++edge_amount;
  }

  void InsertEdge(const size_t& first, const size_t& second) override {
    if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) && !(CheckEdge(first, second))) {
      edges_[first].emplace(second, 1);
    }
    ++edge_amount;
  }

  void InsertEdge(const size_t& first, const size_t& second, int weight) override {
    if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) && !(CheckEdge(first, second))) {
      edges_[first].emplace(second, weight);
    }
    ++edge_amount;
  }

  void EraseEdge(const VertexType& first, const VertexType& second) override {
    if ((first.GetKey() != second.GetKey()) && CheckEdge(first.GetKey(), second.GetKey())) {
      edges_[first.GetKey()].erase(second.GetKey());
    }
    --edge_amount;
  }

  void EraseEdge(const size_t& first, const size_t& second) override {
    if ((first != second) && CheckEdge(first, second)) {
      edges_[first].erase(second);
    }
    --edge_amount;
  }

};
