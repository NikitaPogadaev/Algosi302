
#include <iostream>
#include <cstdarg>
#include <array>



template<std::size_t N, typename T = double>
struct Point{
    
    template<typename... Coords> requires(sizeof...(Coords) == N && 
    std::conjunction<std::is_same<T, Coords>...>::value)
    Point(Coords... coords) : data_({coords...})
    {}
    
    T& getICoord(std::size_t i){
        data_[i];
    }

    std::conditional_t<N <= 2, T, Point<N, T>&> VectorProduct(const Point<N, T>& p) 
    requires(N < 4)
    {
        if constexpr (N <= 1){
            return 0;
        }
        else if constexpr (N == 2){
            return data_[0] * p.data_[1] - data_[1] * p.data_[0];
        }
        else {
            T x1 = data_[1] * p.data_[2] - data_[2] * p.data_[1];
            T x2 = data_[2] * p.data_[0] - data_[0] * p.data_[2];
            T x3 = data_[0] * p.data_[0] - data_[1] * p.data_[0];
            data_[0] = x1;
            data_[1] = x2;
            data_[2] = x3;
            return *this;
        }
    }

    
private:
    std::array<T, N> data_;
};


int main()
{
    std::cout<<"Hello World\n";
    Point<4, int> p{1, 2, 3, 4};
    Point<4, int> p4{1, 2, 3, 4};
    Point<3> p3(1., 2., 3.);
    Point<2, int> p1(1, 2);
    Point<2, int> p2(1, -2);
    std::cout << p1.VectorProduct(p2);
    p.VectorProduct(p4);

    return 0;
}
