// #include "GraphAlgo.h"
// #include "Graph.h"

#include <iostream>
#include <vector>
#include <set>

template<typename T>
concept Iterable = requires(T t) {
    // Проверка наличия методов begin() и end()
    { t.begin() } -> std::same_as<typename T::iterator>;
    { t.end() } -> std::same_as<typename T::iterator>;
};

void foo(auto& cont){
    for(auto& i:cont){
        std::cout << i << ' ';
    }
}

template<Iterable T>
std::ostream& operator<<(std::ostream& os, T& cont){
for (auto& v : cont) {
    os << v << "\n";
  }
  return os;
}

int main() {
    std::set<int> v{1, 4, 2, 4};
    std::cout << v;
    foo(v);
    
}