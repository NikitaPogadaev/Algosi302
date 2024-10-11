#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <set>
#include <cstring>
#include <fstream>
#include <span>
#include <concepts>

template<typename T>
concept Hashable = requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

struct Nil{};

struct UsefullPayLoad {
    std::size_t term_count;
};

template<Hashable T, typename PayLoad = Nil>
struct SuffAutomat {
public:
    void add(const T& chadd) {
        line.emplace_back(chadd);
        dka.push_back(Node());
        std::size_t curr = dka.size() - 1;
        std::size_t tmpptr = last;
        dka[curr].len = dka[tmpptr].len + 1;
        while ((tmpptr != std::dynamic_extent)
                && (dka[tmpptr].to.find(chadd) == dka[tmpptr].to.end())) {
            dka[tmpptr].to[chadd] = curr;
            tmpptr = dka[tmpptr].link;
        }
        if (tmpptr == std::dynamic_extent) {
            dka[curr].link = 0;
            last = curr;
            return;
        }
        std::size_t q = dka[tmpptr].to[chadd];
        if (dka[q].len == dka[tmpptr].len + 1) {
            dka[curr].link = q;
            last = curr;
            return;
        }
        dka.push_back(Node());
        std::size_t clone = dka.size() - 1;
        dka[clone].to = dka[q].to;
        dka[clone].len = dka[tmpptr].len + 1;
        dka[clone].link = dka[q].link;
        dka[q].link = clone;
        dka[curr].link = clone;
        while ((tmpptr != std::dynamic_extent)
                && (dka[tmpptr].to.find(chadd) != dka[tmpptr].to.end())
                && (dka[tmpptr].to[chadd] == q)) {
            dka[tmpptr].to[chadd] = clone;
            tmpptr = dka[tmpptr].link;
        }
        last = curr;
        return;
    }

    void term_set() {
        int64_t ptr = last;
        while (ptr != std::dynamic_extent) {
            dka[ptr].term = true;
            ptr = dka[ptr].link;
        }
    }

    void term_clear() {
        for (auto& i : dka) {
            i.term = false;
        }
    }

    SuffAutomat() {
        line = std::vector<T>();
        dka.push_back(Node());
        last = 0;
    }

    SuffAutomat(const std::vector<T>& s, bool flag_unset = true) :SuffAutomat() {
        for (auto& i : s) {
            add(i);
        }
        if (flag_unset) {
            term_set();
        }
    }

    std::size_t get_last() {
        return last;
    }

    const std::vector<T>& get_line() {
        return line;
    }

private:
    std::size_t last;
    std::vector<T> line;

    struct Node : PayLoad {
        std::size_t len;
        std::size_t link;
        bool term;
        std::unordered_map<T, std::size_t>to;
        Node() {
            link = std::dynamic_extent;
            len = 0;
            term = false;
        }
        Node(const Node&) = default;
        Node(Node&&) = default;
        Node& operator=(const Node&) = default;
        Node& operator=(Node&&) = default;
        ~Node() = default;
    };

    std::vector<Node> dka;

};




template<typename PayLoad>
struct SuffAutomat<char, PayLoad> {
public:
    void add(char chadd) {
        line.push_back(chadd);
        dka.push_back(Node());
        std::size_t curr = dka.size() - 1;
        std::size_t tmpptr = last;
        dka[curr].len = dka[tmpptr].len + 1;
        while ((tmpptr != std::dynamic_extent)
                && (dka[tmpptr].to.find(chadd) == dka[tmpptr].to.end())) {
            dka[tmpptr].to[chadd] = curr;
            tmpptr = dka[tmpptr].link;
        }
        if (tmpptr == std::dynamic_extent) {
            dka[curr].link = 0;
            last = curr;
            return;
        }
        int64_t q = dka[tmpptr].to[chadd];
        if (dka[q].len == dka[tmpptr].len + 1) {
            dka[curr].link = q;
            last = curr;
            return;
        }
        dka.push_back(Node());
        std::size_t clone = dka.size() - 1;
        dka[clone].to = dka[q].to;
        dka[clone].len = dka[tmpptr].len + 1;
        dka[clone].link = dka[q].link;
        dka[q].link = clone;
        dka[curr].link = clone;
        while ((tmpptr != std::dynamic_extent)
                && (dka[tmpptr].to.find(chadd) != dka[tmpptr].to.end())
                && (dka[tmpptr].to[chadd] == q)) {
            dka[tmpptr].to[chadd] = clone;
            tmpptr = dka[tmpptr].link;
        }
        last = curr;
        return;
    }

    void term_set() {
        int64_t ptr = last;
        while (ptr != std::dynamic_extent) {
            dka[ptr].term = true;
            ptr = dka[ptr].link;
        }
    }

    void term_clear() {
        for (auto& i : dka) {
            i.term = false;
        }
    }

    SuffAutomat() {
        line = "";
        dka.push_back(Node());
        last = 0;
    }

    SuffAutomat(const std::string& s, bool flag_unset = true) : SuffAutomat() {
        for (auto& i : s) {
            add(i);
        }
        if (flag_unset) {
            term_set();
        }
    }

    bool is_substr(const std::string& s) {
        std::size_t ver = 0, x = 0;
        while (x < s.length()) {
            if (dka[ver].to.find(s[x]) != dka[ver].to.end()) {
                ver = dka[ver].to[s[x]];
                ++x;
            }
            else {
                break;
            }
        }
        if (x == s.length())return true;
        return false;
    }

    bool is_suff(const std::string& s) {
        std::size_t ver = 0, x = 0;
        while (x < s.length()) {
            if (dka[ver].to.find(s[x]) != dka[ver].to.end()) {
                ver = dka[ver].to[s[x]];
                ++x;
            }
            else {
                break;
            }
        }
        if (x == s.length())return dka[ver].term;
        return false;
    }

    std::size_t get_last() {
        return last;
    }

    const std::string& get_line() {
        return line;
    }

private:
    std::size_t last;
    std::string line;

    struct Node : PayLoad {
        std::size_t len;
        std::size_t link;
        bool term;
        std::unordered_map<char, std::size_t>to;
        Node() {
            link = std::dynamic_extent;
            len = 0;
            term = false;
        }
        Node(const Node&) = default;
        Node(Node&&) = default;
        Node& operator=(const Node&) = default;
        Node& operator=(Node&&) = default;
        ~Node() = default;
    };

    std::vector<Node> dka;
};


struct MySuffAutomat : SuffAutomat<char, UsefullPayLoad>{};


int main () {
    SuffAutomat<char, UsefullPayLoad> suff_auto(std::string("dgdfg"));

    suff_auto.add('g');

    SuffAutomat<std::string> suff_auto_str;
    suff_auto_str.add("fghgfyh");

    MySuffAutomat my_suff;
    my_suff.add('g');

    return 0;
}