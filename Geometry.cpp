
#include <iostream>
#include <cstdarg>
#include <array>



template<std::size_t N, typename T = double>
struct Point{
    
    template<typename... Coords>
    Point(Coords... coords) requires(sizeof...(Coords) == N && 
    std::conjunction<std::is_same<T, Coords>...>::value) : data_({coords...})
    {}
    
    T& getICoord(std::size_t i){
        data_[i];
    }
    
private:
    std::array<T, N> data_;
};


int main()
{
    std::cout<<"Hello World\n";
    Point<4, int> p(1, 2, 3, 4);
    return 0;
}
