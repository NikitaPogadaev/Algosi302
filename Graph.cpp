#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <cstddef>
#include <optional>
#include <span>
#include <concepts>
#include <iostream>
#include <queue>

template <typename K>
concept KeyForVertex = 
std::default_initializable<K> &&
std::copy_constructible<K>
&& requires(K& a, K& b) {
  a = b;
};

template <class Edge, class Vertex, class GraphPayLoad>
class IGraph;

template <class K, class VertexPayload>
requires KeyForVertex<K>
class DefaultVertex;

template <class Vertex, class EdgePayload>
class Edge;

class DefaultGraph;


struct Nil {
  Nil() = default;
  operator bool() const {
    return true;
  }
};

struct NotNil {
  NotNil() = default;
  operator bool() const {
    return false;
  }
};


struct TinToutParent {
  friend class DefaultGraph;

  int tin_{-1};
  int tout_{-1};
  std::optional<std::size_t> parent_{std::nullopt};
  int getTin() {
    return tin_;
  }
  int getTout() {
    return tout_;
  }
  std::optional<std::size_t> getParent() {
    return parent_;
  }

  protected:
  void setTin(int x) {
    tin_ = x;
  }
  void setTout(int x) {
    tout_ = x;
  }
  void setParent(std::size_t x) {
    parent_ = std::optional<std::size_t>(x);
  }
};



template <class Edge, class Vertex, class GraphPayLoad = Nil>
class IGraph : public GraphPayLoad {
 public:
  using VertexType = Vertex;
  using EdgeType = Edge;
  using VertexRef = Vertex&;
  using EdgeRef = Edge&;
  using PayLoad = GraphPayLoad;
  virtual void InsertEdge(const Vertex&, const Vertex&) = 0;
  virtual void InsertEdge(const typename Vertex::Key&, const typename Vertex::Key&) = 0;
  virtual void InsertVertex(const typename Vertex::Key& key) = 0;
  virtual bool CheckEdge(const typename Vertex::Key&, const typename Vertex::Key&) const = 0;
  virtual bool CheckVertex(const typename Vertex::Key&) const = 0;
  virtual const EdgeType& FindEdge(const typename Vertex::Key&, const typename Vertex::Key&) const = 0;
  virtual const VertexType& FindVertex(const typename Vertex::Key&) const = 0;
  virtual EdgeType& FindEdge(const typename Vertex::Key&, const typename Vertex::Key&) = 0;
  virtual VertexType& FindVertex(const typename Vertex::Key&)  = 0;
  virtual size_t EdgesAmount() const = 0;
  virtual size_t VertexesAmount() const = 0;
  virtual ~IGraph() {};
};

template <class K = std::size_t, class VertexPayload = Nil>
requires KeyForVertex<K>
class DefaultVertex : public VertexPayload {
 public:
  using Key = K;
  using PayLoad = VertexPayload;
  DefaultVertex() = delete;
  
  template <class... Args>
  DefaultVertex(const K& key, const Args&... args) : VertexPayload(args...), key_(key) {
  }
  DefaultVertex(const DefaultVertex& other) = default;
  DefaultVertex& operator=(const DefaultVertex& other) {
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
  Edge(const Vertex& begin, const Vertex& end, const Args&... args) : EdgePayload(args...), begin_(begin), end_(end) {
  }
  explicit Edge(const Edge& other) = default;
  ~Edge() = default;
  Edge& operator=(const Edge& other) {
    if (this == &other) {
      return *this;
    }
    begin_ = other.begin_;
    end_ = other.end_;
    return *this;
  }
  const Vertex& GetBegin() const {
    return begin_;
  }
  const Vertex& GetEnd() const {
    return end_;
  }
 private:
  Vertex begin_;
  Vertex end_;
};


struct NilNil {
  NilNil() = default;
  operator bool() const {
    return true;
  }
  Nil gag_;
  NotNil n_gag_;
  DefaultVertex<std::size_t, TinToutParent> NullVertex{std::dynamic_extent};
};



class DefaultGraph : public IGraph<Nil, DefaultVertex<std::size_t, TinToutParent>, NilNil> {
 public:

  bool isNull(const VertexType& vert) const {
    return vert.GetKey() == std::dynamic_extent;
  }

  const VertexType& getNull() const {
    return NullVertex;
  }

  bool CheckVertex(const size_t& key) const override {
    if (vertexes_.size() > key) {
      return true;
    }
    return false;
  }

  bool CheckEdge(const size_t& first, const size_t& second) const override {
    if ((CheckVertex(first)) && (CheckVertex(second))) {
      if (edges_[first].find(second) != edges_[first].end()) {
        return true;
      }
    }
      return false;
  }

  const VertexType& FindVertex(const size_t& key) const override {
    if (vertexes_.size() > key) {
      return vertexes_[key];
    }
    return NullVertex;
  }

  const EdgeType& FindEdge(const size_t& first, const size_t& second) const override {
    return gag_;
  }

  VertexType& FindVertex(const size_t& key) override {
    if (vertexes_.size() > key) {
      return vertexes_[key];
    }
    return NullVertex;
  }

  EdgeType& FindEdge(const size_t& first, const size_t& second) override {
    return gag_;
  }

  void InsertVertex(const std::size_t& key) override {
    if (key != vertexes_.size()) return;
    ++vert_amount;
    vertexes_.emplace_back(vertexes_.size());
    edges_.push_back(std::unordered_set<size_t>());
  }  
  
  void InsertVertex() {
    ++vert_amount;
    vertexes_.emplace_back(vertexes_.size());
    edges_.push_back(std::unordered_set<size_t>());
  } 

 void InsertEdge(const VertexType& first, const VertexType& second) override {
  if ((CheckVertex(first.GetKey())) && (CheckVertex(second.GetKey())) && (first.GetKey() != second.GetKey()) && !(CheckEdge(first.GetKey(), second.GetKey()))) {
    edges_[first.GetKey()].insert(second.GetKey());
    edges_[second.GetKey()].insert(first.GetKey());
  }
  ++edge_amount;
 }
 
 void InsertEdge(const size_t& first, const size_t& second) override {
  if ((CheckVertex(first)) && (CheckVertex(second)) && (first != second) && !(CheckEdge(first, second))) {
    edges_[first].insert(second);
    edges_[second].insert(first);
  }
  ++edge_amount;
 }

  size_t EdgesAmount() const override {
    return edge_amount;
  }
  size_t VertexesAmount() const override {
    return vert_amount;
  }

  void BFS(){
    std::vector<bool> used(vert_amount, false);
    for(std::size_t i = 0; i < vert_amount; ++i) {
      if (!used[i]) {
        std::queue<std::size_t> qs;
        qs.push(i);
        used[i] = true;
        while (!qs.empty()) {
          std::size_t tmp = qs.front();
          qs.pop();
          for(auto ind: edges_[tmp]){
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

  void DFS(){
    std::size_t co = 0;
    std::vector<bool> used (vert_amount, false);
    for (std::size_t i = 0; i < vert_amount; ++i) {
      if (used[i]) continue;
      recDFS(i, used, co);
    }
  }

 private:
  
  void recDFS(std::size_t vert, std::vector<bool>& used, std::size_t& co){
    used[vert] = true;
    vertexes_[vert].setTin(co);
    for(auto ind: edges_[vert]) {
      if (used[ind]) continue;
      ++co;
      vertexes_[ind].setParent(vert);
      recDFS(ind, used, co);
    }
    vertexes_[vert].setTin(co);
  }
  
  std::vector<VertexType> vertexes_;
  std::vector<std::unordered_set<size_t>> edges_;
  std::size_t edge_amount{0};
  std::size_t vert_amount{0};
};

int main() {
  DefaultGraph dg;

  dg.InsertVertex();
  dg.InsertVertex();
  dg.InsertEdge(0, 1);
  std::cout << dg.CheckEdge(0, 1);
  dg.BFS();
  dg.DFS();
  return 0;
}