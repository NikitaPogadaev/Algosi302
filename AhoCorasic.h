
#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

template <class Iterator> class IteratorRange {
  public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {}
    Iterator begin() const { return begin_; }
    Iterator end() const { return end_; }

  private:
    Iterator begin_, end_;
};

namespace traverses {

template <class Vertex, class Graph, class Visitor>
void BreadthFirstSearch(Vertex origin_vertex, const Graph &graph,
                        Visitor visitor) {
    std::queue<Vertex> queue;
    std::unordered_set<Vertex> discovered;

    queue.push(origin_vertex);
    discovered.insert(origin_vertex);
    visitor.DiscoverVertex(origin_vertex);

    while (!queue.empty()) {
        Vertex cur = queue.front();
        queue.pop();
        visitor.ExamineVertex(cur);

        for (const auto &edge : OutgoingEdges(graph, cur)) {
            visitor.ExamineEdge(edge);
            Vertex target = GetTarget(graph, edge);
            if (discovered.find(target) == discovered.end()) {
                discovered.insert(target);
                visitor.DiscoverVertex(target);
                queue.push(target);
            }
        }
    }
}

template <class Vertex, class Edge> class BfsVisitor {
  public:
    virtual void DiscoverVertex(Vertex) {}
    virtual void ExamineEdge(const Edge &) {}
    virtual void ExamineVertex(Vertex) {}
    virtual ~BfsVisitor() = default;
};

} // namespace traverses

namespace aho_corasick {

struct AutomatonNode {
    AutomatonNode() : suffix_link(nullptr), terminal_link(nullptr) {}
    std::vector<size_t> terminated_string_ids;
    std::map<char, AutomatonNode> trie_transitions;
    std::map<char, AutomatonNode *> automaton_transitions;
    AutomatonNode *suffix_link;
    AutomatonNode *terminal_link;
};

AutomatonNode *GetTrieTransition(AutomatonNode *node, char character) {
    auto it = node->trie_transitions.find(character);
    return it != node->trie_transitions.end() ? &it->second : nullptr;
}

AutomatonNode *GetAutomatonTransition(AutomatonNode *node, AutomatonNode *root,
                                      char character) {
    auto &cache = node->automaton_transitions;
    auto it = cache.find(character);
    if (it != cache.end()) {
        return it->second;
    }
    if (AutomatonNode *transition = GetTrieTransition(node, character)) {
        cache[character] = transition;
        return transition;
    }
    if (node == root) {
        cache[character] = root;
        return root;
    }
    AutomatonNode *suffix_transition =
        GetAutomatonTransition(node->suffix_link, root, character);
    cache[character] = suffix_transition;
    return suffix_transition;
}

namespace internal {

class AutomatonGraph {
  public:
    struct Edge {
        Edge(AutomatonNode *source, AutomatonNode *target, char character)
            : source(source), target(target), character(character) {}
        AutomatonNode *source;
        AutomatonNode *target;
        char character;
    };
};

std::vector<typename AutomatonGraph::Edge>
OutgoingEdges(const AutomatonGraph &, AutomatonNode *vertex) {
    std::vector<typename AutomatonGraph::Edge> edges;
    for (auto &transition : vertex->trie_transitions) {
        edges.emplace_back(vertex, &transition.second, transition.first);
    }
    return edges;
}

AutomatonNode *GetTarget(const AutomatonGraph &,
                         const AutomatonGraph::Edge &edge) {
    return edge.target;
}

class SuffixLinkCalculator
    : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
  public:
    explicit SuffixLinkCalculator(AutomatonNode *root) : root_(root) {}
    void ExamineVertex(AutomatonNode *node) override {
        if (!node->suffix_link) {
            node->suffix_link = root_;
        }
    }
    void ExamineEdge(const AutomatonGraph::Edge &edge) override {
        edge.target->suffix_link =
            edge.source == root_
                ? root_
                : GetAutomatonTransition(edge.source->suffix_link, root_,
                                         edge.character);
    }

  private:
    AutomatonNode *root_;
};

class TerminalLinkCalculator
    : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
  public:
    explicit TerminalLinkCalculator(AutomatonNode *root) : root_(root) {}
    void DiscoverVertex(AutomatonNode *node) override {
        if (node == root_) {
            node->terminal_link = nullptr;
            return;
        }
        if (!node->suffix_link->terminated_string_ids.empty()) {
            node->terminal_link = node->suffix_link;
        } else {
            node->terminal_link = node->suffix_link->terminal_link;
        }
    }

  private:
    AutomatonNode *root_;
};

} // namespace internal

class NodeReference {
  public:
    NodeReference() : node_(nullptr), root_(nullptr) {}
    NodeReference(AutomatonNode *node, AutomatonNode *root)
        : node_(node), root_(root) {}

    NodeReference Next(char character) const {
        return NodeReference(GetAutomatonTransition(node_, root_, character),
                             root_);
    }

    template <class Callback> void GenerateMatches(Callback on_match) const {
        for (NodeReference node = *this; node; node = node.TerminalLink()) {
            for (size_t id : node.TerminatedStringIds()) {
                on_match(id);
            }
        }
    }

    bool IsTerminal() const {
        if (!node_) {
            return false;
        }
        return !node_->terminated_string_ids.empty();
    }

    explicit operator bool() const { return node_ != nullptr; }
    bool operator==(NodeReference other) const {
        return node_ == other.node_ && root_ == other.root_;
    }

  private:
    using TerminatedStringIterator = std::vector<size_t>::const_iterator;
    using TerminatedStringIteratorRange =
        IteratorRange<TerminatedStringIterator>;

    NodeReference TerminalLink() const {
        return NodeReference(node_->terminal_link, root_);
    }

    TerminatedStringIteratorRange TerminatedStringIds() const {
        return TerminatedStringIteratorRange(
            node_->terminated_string_ids.begin(),
            node_->terminated_string_ids.end());
    }

    AutomatonNode *node_;
    AutomatonNode *root_;
};

class AutomatonBuilder;

class Automaton {
  public:
    Automaton() = default;
    Automaton(const Automaton &) = delete;
    Automaton &operator=(const Automaton &) = delete;

    NodeReference Root() { return NodeReference(&root_, &root_); }

  private:
    AutomatonNode root_;
    friend class AutomatonBuilder;
};

class AutomatonBuilder {
  public:
    void Add(const std::string &string, size_t id) {
        words_.push_back(string);
        ids_.push_back(id);
    }

    std::unique_ptr<Automaton> Build() {
        auto automaton = std::make_unique<Automaton>();
        BuildTrie(words_, ids_, automaton.get());
        BuildSuffixLinks(automaton.get());
        BuildTerminalLinks(automaton.get());
        return automaton;
    }

  private:
    static void BuildTrie(const std::vector<std::string> &words,
                          const std::vector<size_t> &ids,
                          Automaton *automaton) {
        for (size_t i = 0; i < words.size(); ++i) {
            AddString(&automaton->root_, ids[i], words[i]);
        }
    }

    static void AddString(AutomatonNode *root, size_t string_id,
                          const std::string &string) {
        AutomatonNode *cur = root;
        for (char ch : string) {
            cur = &cur->trie_transitions[ch];
        }
        cur->terminated_string_ids.push_back(string_id);
    }

    static void BuildSuffixLinks(Automaton *automaton) {
        internal::SuffixLinkCalculator visitor(&automaton->root_);
        traverses::BreadthFirstSearch(&automaton->root_,
                                      internal::AutomatonGraph(), visitor);
    }

    static void BuildTerminalLinks(Automaton *automaton) {
        internal::TerminalLinkCalculator visitor(&automaton->root_);
        traverses::BreadthFirstSearch(&automaton->root_,
                                      internal::AutomatonGraph(), visitor);
    }

    std::vector<std::string> words_;
    std::vector<size_t> ids_;
};

} // namespace aho_corasick
