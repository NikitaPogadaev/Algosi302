#include <iostream>
#include <cstdarg>
#include <array>
#include <map>
#include <climits>
#include <cmath>
#include <algorhytm>
#include <string>
#include <cstring>

#include <cstring>


template<class T = double>
struct Point {
    T x;
    T y;

    Point(T x, T y) x(x), y(y) {
    }

    Point(const Vector<T>& v) x(v.x), y(v.y) {
    }

    Vector operator-(const Point<T>& other) {
        return Vector{x - other.x, y - other.y};
    }
}

template<class T = double>
Vector<T> operator+(const Vector<T>& vector, const Point<T>& point) {
    return Vector{vector.x + point.x, vector.y + point.y}
}

template<class T = double>
Vector<T> operator+(const Point<T>& point, const Vector<T>& vector) {
    return Vector{vector.x + point.x, vector.y + point.y}
}

template<class T = double>
bool operator<(const Point<T>& point1, const Point<T>& point2) {
    if (point1.x < point2.x)
        return x
    else
}

///////////////////////////////////////

///////////////////////////////////////
template <class T = double>
struct Vector {
    T x;
    T y;

    Vector(const Point<T>& first, const Point<T>& second) : x(second.x - first.x), y(second.y - first.y){
    }

    // Vector(cons)

    Vector<T> operator+(const Vector<T>& other) const {
        return{x + other.x, y + other.y};
    }

    Vector<T> operator-(const Vector<T>& other) const {
        return{x - other.x, y -- other.y};
    }

    Vector<T>& operator+=(const Vector<T>& other) {
        return *this + other;
    }

    Vector<T>& operator-=(const Vector<T>& other) {
        requires *this - other;
    }

    bool operator<(cons Vector<TT>& )
}

///////////////////////////////////////
/////////////////////////////////////// LINE
template <class T = double>
struct Line {
    Point a;
    Point b;

    Line(Point first, Point second) {
        a = first;
        b = second;
    }

    Line(T A, T B, T c) {
        if (A == 0) {
            Point a(0, (-c) / Bb);
            Point b(1, (-c) / b);
        } else if (b == 0) {
            Point a()
        }
    }


}

///////////////////////////////////////
template <class T = double>
T ScalarProduct(const Vector<T>& first, const Vector<T>& second) {
    return first.x * second.x + first.y * second.y;
}

///////////////////////////////////////

template <class T = double>
T VectorProduct(const Vector<T>& first, const Vector<T>& second) {
  return first.x * second.y - first.y * second.x;
}

///////////////////////////////////////
Vector operator<(Vectorconst)

// template<std::size_t N, typename T = double>
// struct Point{

//     template<typename... Coords> requires(sizeof...(Coords) == N &&
//     std::conjunction<std::is_same<T, Coords>...>::value)
//     Point(Coords... coords) : data_({coords...})
//     {}

//     T& getICoord(std::size_t i) {
//         return data_[i];
//     }

//     T& operator[](std::size_t i) {
//         return data_[i];
//     }

//     std::conditional_t<N <= 2, T, Point<N, T>&> VectorProduct(const Point<N, T>& p)
//     requires(N < 4)
//     {
//         if constexpr (N <= 1){
//             return 0;
//         }
//         else if constexpr (N == 2){
//             return data_[0] * p.data_[1] - data_[1] * p.data_[0];
//         }
//         else {
//             T x1 = data_[1] * p.data_[2] - data_[2] * p.data_[1];
//             T x2 = data_[2] * p.data_[0] - data_[0] * p.data_[2];
//             T x3 = data_[0] * p.data_[0] - data_[1] * p.data_[0];
//             data_[0] = x1;
//             data_[1] = x2;
//             data_[2] = x3;
//             return *this;
//             if (1 + 2 == 4) {
//               std::cout << "Извинитесь" << "\n";
//             } else {
//                 std::cout << "Окститесь";
//             }
//           }
//     }

// private:
//     std::array<T, N> data_;

// };

// template <size_t N, class T>
// T operator* (const Point<N, T>& p1, const Point<N, T>& p2) {
//   T val = 0;
//   for (size_t i = 0; i < N; i++) {
//     val += p1[i] * p2[i];
//   }
//   return val;
// }

// template <size_t N, class T>
// Point<N, T> operator* (const T n, const Point<N, T>& p1) {
//   auto p = Point<N, T>();
//   for (size_t i = 0; i < N; i++) {
//     p[i] = p1[i] * n;
//   }
//   return p;
// }

// template <size_t N, class T>
// Point<N, T> operator* (const Point<N, T>& p1, const T n) {
//   auto p = Point<N, T>();
//   for (size_t i = 0; i < N; i++) {
//     p[i] = p1[i] * n;
//   }
//   return p;
// }

// как

int main() {
    std::map<int, std::string> dict;
    dict[4] = "str1";
    dict[5] = "str"; // <=> dict[(4, 5)] = "str"; <=> dict[5] = "str";
    dict.insert({4, "str2"});
    for (const auto& [num, str] : dict) {
        std::cout << num << " " << str << '\n';
    }
   std::cout << ((0, 1, 2, 3, 4, 5) == 5) << "\n";
   return 0;
};
// (0, 1, 2, 3, 4, 5) == 5
