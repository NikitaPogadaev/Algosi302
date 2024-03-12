#include <iostream>
#include <cstdarg>
#include <array>
#include <map>
#include <climits>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <vector>


template<class T>
struct Point;

template<class T>
struct Vector;

template<class T = double>
struct Point {
    T x;
    T y;


    T SetX(const T& new_x) {
        return (x = new_x);
    }

    T SetY(const T& new_y) {
        return (y = new_y);
    }

    T& GetX() {
        return x;
    }

    T& GetY() {
        return y;
    }

    T GetX() const {
        return x;
    }

    T GetY() const {
        return y;
    }

    Point(T x, T y) : x(x), y(y) {
    }

    Point(const Vector<T>& v) : x(v.x), y(v.y) {
    }

    Vector<T> operator-(const Point<T>& other) {
        return {x - other.x, y - other.y};
    }

    bool operator==(const Point<T>& other) const {
        return ((x == other.x) && (y == other.y));
    }


    bool operator<(const Point<T>& other) const {
        return sqrt(GetX() * GetX() + GetY() * GetY()) < sqrt(other.GetX() * other.GetX() + other.GetY() + other.GetY());
    }

};

template <class T>
std::ostream& operator<<(std::ostream& os, const Point<T> point) {
    os << "x: " << point.x << ", y: " << point.y;
    return os;
}

template <class T>
std::istream& operator<<(std::istream& is, Point<T>& point) {
    is >> point.x >> point.y;
    return is;
}

template<class T = double>
Vector<T> operator+(const Vector<T>& vector, const Point<T>& point) {
    return {vector.x + point.x, vector.y + point.y};
}

template<class T = double>
Vector<T> operator+(const Point<T>& point, const Vector<T>& vector) {
    return {vector.x + point.x, vector.y + point.y};
}

// template<class T = double>
// bool operator<(const Point<T>& point1, const Point<T>& point2) {
//     if (point1.x < point2.x)
//         return true;
//     else {
//         point1.y < point2.y;
//     }
// }

///////////////////////////////////////

///////////////////////////////////////
template <class T = double>
struct Vector : public Point<T> {
    // T x;
    // T y;

    Vector(const Point<T>& first, const Point<T>& second) : Point<T>((second.x - first.x), (second.y - first.y)){
    }

    Vector(const Point<T>& first) : Point<T>((first.x), (first.y)){
    }

    Vector(const Vector<T>& other) : Point<T>(other.x, other.y) {
    }


    inline Vector<T> operator+(const Vector<T>& other) const {
        return{this->GetX() + other.GetX(), this->GetY() + other.GetY()};
    }

    inline Vector<T> operator-(const Vector<T>& other) const {
        return{this->GetX() - other.GetX(), this->GetY() - other.GetY()};
    }

    Vector<T>& operator+=(const Vector<T>& other) {
        *this = *this + other;
        return *this;
    }

    Vector<T>& operator-=(const Vector<T>& other) {
        *this = *this - other;
        return *this;
    }

};

///////////////////////////////////////
/////////////////////////////////////// LINE
template <class T>
struct Line {
    Point<T> a;
    Point<T> b;

    
    Line(Point<T> first, Point<T> second) : a(first), b(second) {
    }

    Line(T A, T B, T C) {
        if (A == 0) {
            Point<T> a(0, (-C) / B);
            Point<T> b(1, (-C) / B);
        } else if (B == 0) {
            Point<T> a((-C) / A, 0);
            Point<T> b((-C) / A, 1);
        }
    }

    Line(Point<T> point, Vector<T> vector) {
        a = point;
        Point<T> second_point(a.x + vector.x, a.y + vector.y);
        b = second_point;
    }

    // template<class EQ>
    // bool operator ==(const Line<T>& l){
    //     return EQ(l, *this);
    // }

    // template<class CMP>
    // bool operator < (const Line<T>& l){
    //     return CMP(l, *this);
    // }

    // template<class CMP>
    bool compareWithLine(const Line<T>& l, auto& CMP) const {
        return CMP(*this, l);
    }



};

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


int main() {
    Point x1(2., 3.);
    Point x2(20., 30.);


    Point x3(-2., -3.);
    Point x4(-20., -30.);
    // Point x4(x2);
    std::cout << (x1 < x2) << '\n';

    Line l1 (x1, x2);


    Line l2 (x3, x4);


    auto cmp1 = [](const Line<double>& l1, const Line<double>& l2){
        if(l1.a != l2.a) return l1.a < l2.a;
        else return l1.b < l2.b;
    };


    std::cout << l1.compareWithLine(l2, cmp1) << '\n';

    std::vector<Line<double>> lines(3, l1);
    std::sort(lines.begin(), lines.end(), cmp1);
    // std::cout << cmp1(l1,l2);

    return 0;
};
