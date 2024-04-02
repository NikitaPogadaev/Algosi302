
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
concept KeyForVertex =
    std::default_initializable<K> && std::copy_constructible<K> &&
    requires(K &a, K &b) { a = b; };

const auto DEFAULT_DFS_FOO = [](std::size_t a) {};

template <class Edge, class Vertex, class GraphPayLoad> class IGraph;
template <class K, class VertexPayload>
  requires KeyForVertex<K>
class DefaultVertex;
template <class Vertex, class EdgePayload> class Edge;
class DefaultGraph;

struct Nil {
  Nil() = default;
  operator bool() const { return true; }
};

struct NotNil {
  NotNil() = default;
  operator bool() const { return false; }
};

struct TinToutParent {
  friend class DefaultGraph;
  friend class NonOrientedDefaultGraph;

  int tin_{-1};
  int tout_{-1};
  std::optional<std::size_t> parent_{std::nullopt};
  int getTin() { return tin_; }
  int getTout() { return tout_; }
  std::optional<std::size_t> getParent() { return parent_; }

protected:
  void setTin(int x) { tin_ = x; }
  void setTout(int x) { tout_ = x; }
  void setParent(std::size_t x) { parent_ = std::optional<std::size_t>(x); }
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
  virtual void InsertEdge(const typename Vertex::Key &,
                          const typename Vertex::Key &) = 0;
  virtual void InsertVertex(const typename Vertex::Key &key) = 0;
  virtual bool CheckEdge(const typename Vertex::Key &,
                         const typename Vertex::Key &) const = 0;
  virtual bool CheckVertex(const typename Vertex::Key &) const = 0;
  virtual const EdgeType &FindEdge(const typename Vertex::Key &,
                                   const typename Vertex::Key &) const = 0;
  virtual const VertexType &FindVertex(const typename Vertex::Key &) const = 0;
  virtual EdgeType &FindEdge(const typename Vertex::Key &,
                             const typename Vertex::Key &) = 0;
  virtual VertexType &FindVertex(const typename Vertex::Key &) = 0;
  virtual size_t EdgesAmount() const = 0;
  virtual size_t VertexesAmount() const = 0;
  virtual ~IGraph(){};
};

template <class K = std::size_t, class VertexPayload = Nil>
  requires KeyForVertex<K>
class DefaultVertex : public VertexPayload {
public:
  using Key = K;
  using PayLoad = VertexPayload;
  DefaultVertex() = delete;

  template <class... Args>
  DefaultVertex(const K &key, const Args &...args)
      : VertexPayload(args...), key_(key) {}
  DefaultVertex(const DefaultVertex &other) = default;
  DefaultVertex &operator=(const DefaultVertex &other) {
    key_ = other.key_;
    return *this;
  }
  K GetKey() const { return key_; }

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
  Edge(const Vertex &begin, const Vertex &end, const Args &...args)
      : EdgePayload(args...), begin_(begin), end_(end) {}
  explicit Edge(const Edge &other) = default;
  ~Edge() = default;
  Edge &operator=(const Edge &other) {
    if (this == &other) {
      return *this;
    }
    begin_ = other.begin_;
    end_ = other.end_;
    return *this;
  }
  const Vertex &GetBegin() const { return begin_; }
  const Vertex &GetEnd() const { return end_; }

private:
  Vertex begin_;
  Vertex end_;
};

struct NilNil {
  NilNil() = default;
  operator bool() const { return true; }
  Nil gag_;
  NotNil n_gag_;
  DefaultVertex<std::size_t, TinToutParent> NullVertex{std::dynamic_extent};
};

class DefaultGraph
    : public IGraph<Nil, DefaultVertex<std::size_t, TinToutParent>, NilNil> {
public:
  DefaultGraph() = default;
  ~DefaultGraph() = default;
  DefaultGraph &operator=(const DefaultGraph &) = default;

  bool isNull(const VertexType &vert) const {
    return vert.GetKey() == std::dynamic_extent;
  }

  const VertexType &getNull() const { return NullVertex; }

  bool CheckVertex(const size_t &key) const override {
    if (vertexes_.size() > key) {
      return true;
    }
    return false;
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

  const EdgeType &FindEdge(const size_t &first,
                           const size_t &second) const override {
    return gag_;
  }

  VertexType &FindVertex(const size_t &key) override {
    if (vertexes_.size() > key) {
      return vertexes_[key];
    }
    return NullVertex;
  }

  EdgeType &FindEdge(const size_t &first, const size_t &second) override {
    return gag_;
  }

  void InsertVertex(const std::size_t &key) override {
    if (key != vertexes_.size())
      return;
    ++vert_amount;
    vertexes_.emplace_back(vertexes_.size());
    edges_.push_back(std::unordered_set<size_t>());
  }

  void InsertVertex() {
    ++vert_amount;
    vertexes_.emplace_back(vertexes_.size());
    edges_.push_back(std::unordered_set<size_t>());
  }

  DefaultGraph(std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
      InsertVertex();
    }
  }

  virtual void InsertEdge(const VertexType &first,
                          const VertexType &second) override {
    if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) &&
        (first.GetKey() != second.GetKey()) &&
        !(CheckEdge(first.GetKey(), second.GetKey()))) {
      edges_[first.GetKey()].insert(second.GetKey());
      edges_[second.GetKey()].insert(first.GetKey());
    }
    ++edge_amount;
  }

  virtual void InsertEdge(const size_t &first, const size_t &second) override {
    if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) &&
        !(CheckEdge(first, second))) {
      edges_[first].insert(second);
      edges_[second].insert(first);
    }
    ++edge_amount;
  }

  virtual void EraseEdge(const VertexType &first, const VertexType &second) {
    if ((first.GetKey() != second.GetKey()) &&
        CheckEdge(first.GetKey(), second.GetKey())) {
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

  size_t EdgesAmount() const override { return edge_amount; }
  size_t VertexesAmount() const override { return vert_amount; }

  class TopSortFoo {
  public:
    TopSortFoo(std::vector<VertexType::Key> &buf) : buf_(buf) {}
    void operator()(std::size_t vert_) { buf_.push_back(vert_); }
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
              vertexes_[ind].setParent(tmp);
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
      if (used[i])
        continue;
      recDFS(i, used, co);
    }
  }

protected:
  template <class OutFoo = decltype(DEFAULT_DFS_FOO)>
  void recDFS(std::size_t vert, std::vector<bool> &used, std::size_t &co,
              OutFoo out_foo = DEFAULT_DFS_FOO) {
    used[vert] = true;
    vertexes_[vert].setTin(co);
    for (auto ind : edges_[vert]) {
      if (used[ind])
        continue;
      ++co;
      vertexes_[ind].setParent(vert);
      recDFS(ind, used, co, out_foo);
    }
    vertexes_[vert].setTout(co);
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
  ~OrientedDefaultGraph() = default;
  OrientedDefaultGraph &operator=(const OrientedDefaultGraph &) = default;

  OrientedDefaultGraph(std::size_t n) : DefaultGraph(n) {}

  std::vector<VertexType::Key> TopSort() {
    std::size_t co = 0;
    std::vector<bool> used(vert_amount, false);
    std::vector<VertexType::Key> sorted;
    for (std::size_t i = 0; i < vert_amount; ++i) {
      if (used[i])
        continue;
      recDFS(i, used, co, TopSortFoo(sorted));
    }
    std::reverse(sorted.begin(), sorted.end());
    return sorted;
  }

  virtual void InsertEdge(const VertexType &first,
                          const VertexType &second) override {
    if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) &&
        (first.GetKey() != second.GetKey()) &&
        !(CheckEdge(first.GetKey(), second.GetKey()))) {
      edges_[first.GetKey()].insert(second.GetKey());
    }
    ++edge_amount;
  }

  virtual void InsertEdge(const size_t &first, const size_t &second) override {
    if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) &&
        !(CheckEdge(first, second))) {
      edges_[first].insert(second);
    }
    ++edge_amount;
  }

  virtual void EraseEdge(const VertexType &first,
                         const VertexType &second) override {
    if ((first.GetKey() != second.GetKey()) &&
        CheckEdge(first.GetKey(), second.GetKey())) {
      edges_[first.GetKey()].erase(second.GetKey());
    }
    --edge_amount;
  }

  virtual void EraseEdge(const size_t &first, const size_t &second) override {
    if ((first != second) && CheckEdge(first, second)) {
      edges_[first].erase(second);
    }
    --edge_amount;
  }

  virtual void IsolateVertex(const VertexType &first) override {
    if (CheckVertex(first.GetKey())) {
      edge_amount -= edges_[first.GetKey()].size();
      edges_[first.GetKey()].clear();
    }
  }

  OrientedDefaultGraph invertedGraph() {
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
  ~NonOrientedDefaultGraph() = default;
  NonOrientedDefaultGraph &operator=(const NonOrientedDefaultGraph &) = default;

  NonOrientedDefaultGraph(std::size_t n) : DefaultGraph(n) {}

  virtual std::vector<std::size_t> FindDist(std::size_t first,
                                            std::size_t last) {
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
          vertexes_[ind].setParent(tmp);
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
      cur_vert = vertexes_[cur_vert].getParent().value();
    }
    way.push_back(first);
    std::reverse(way.begin(), way.end());
    return way;
  }
};

int main() {
  size_t n;
  size_t m;
  size_t a;
  size_t b;
  std::cin >> n >> m >> a >> b;
  NonOrientedDefaultGraph g(n);
  for (size_t i = 0; i < m; i++) {
    size_t vertex_start;
    size_t vertex_end;
    std::cin >> vertex_start >> vertex_end;
    g.InsertEdge(vertex_start - 1, vertex_end - 1);
  }
  auto dist = g.FindDist(a - 1, b - 1);
  if (dist.empty()) {
    std::cout << -1;
  } else {
    std::cout << dist.size() - 1 << "\n";
    for (auto& el : dist) {
      std::cout << el + 1 << " ";
    }
  }
  

  return 0;
}
