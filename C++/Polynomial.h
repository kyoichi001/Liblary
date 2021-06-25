
#include <vector>
#include<iostream>
class Polynomial {
    using mType = ModInt<2>;
public:
    //[0]ならx^0 [1]ならx^1 ...
    vector<mType> a;
    Polynomial() {}
    Polynomial(const vector<mType>& a_) :a(a_) {}
    Polynomial(const vector<pair<int, mType>>& a_) {
        for (auto& i : a_) {
            while (i.first >= a.size())  a.push_back(0);
            a[i.first] = i.second;
        }
    }

    Polynomial operator +()const { return *this; }
    Polynomial operator -() const {
        auto res = a;
        for (auto& i : res) i = -i;
        return res;
    }
    Polynomial operator +(const Polynomial& rhs)const {
        if (a.size() >= rhs.a.size()) {
            auto res = rhs.a;
            for (int i = 0; i < rhs.a.size(); i++) res[i] = a[i] + rhs.a[i];
            for (int i = rhs.a.size(); i < a.size(); i++)   res.push_back(a[i]);
            return res;
        }
        auto res = a;
        for (int i = 0; i < a.size(); i++) res[i] = a[i] + rhs.a[i];
        for (int i = a.size(); i < rhs.a.size(); i++)  res.push_back(rhs.a[i]);
        return res;
    }
    Polynomial operator -(const Polynomial& rhs)const { return *this + (-rhs); }
    Polynomial operator *(const Polynomial& rhs)const {
        Polynomial res;
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < rhs.a.size(); j++) {
                res[i + j] += a[i] * rhs.a[j];
            }
        }
        return res;
    }
    Polynomial operator /(const Polynomial& rhs)const {
        Polynomial res;
        Polynomial b = *this;//割られる数
        auto s = b.max();
        auto t = rhs.max();
        while (s.first >= t.first) {
            mType x = s.second / t.second;//係数
            for (int i = 0; i < rhs.a.size(); i++) {
                b[s.first - i] -= rhs.a[t.first - i] * x;
            }
            res[s.first - t.first] = x;
            s = b.max();
            t = rhs.max();
        }
        return res;
    }
    Polynomial operator %(const Polynomial& rhs)const {
        Polynomial res;
        Polynomial b = *this;//割られる数
        auto s = b.max();
        auto t = rhs.max();
        while (s.first >= t.first) {
            mType x = s.second / t.second;//係数
            for (int i = 0; i < rhs.a.size(); i++) {
                b[s.first - i] -= rhs.a[t.first - i] * x;
            }
            res[s.first - t.first] = x;
            s = b.max();
            t = rhs.max();
        }
        return b;
    }

    Polynomial operator *(mType t)const {
        Polynomial res = *this;
        for (int i = 0; i < a.size(); i++)  res.a[i] = a[i] * t;
        return res;
    }
    Polynomial operator /(mType t)const {
        Polynomial res = *this;
        for (int i = 0; i < a.size(); i++)  res[i] = a[i] / t;
        return res;
    }

    void operator +=(const Polynomial& rhs) { *this = *this + rhs; }
    void operator -=(const Polynomial& rhs) { *this = *this - rhs; }
    void operator *=(const Polynomial& rhs) { *this = *this * rhs; }
    void operator /=(const Polynomial& rhs) { *this = *this / rhs; }
    void operator %=(const Polynomial& rhs) { *this = *this % rhs; }
    void operator *=(mType t) { *this = *this * t; }
    void operator /=(mType t) { *this = *this / t; }

    bool operator ==(const Polynomial& rhs)const {
        auto s = max();
        auto t = rhs.max();
        if (s.first != t.first)return false;
        for (int i = s.first - 1; i >= 0; i--) {
            if (a[i] != rhs.a[i])return false;
        }
        return true;
    }
    bool operator !=(const Polynomial& rhs)const { return !(*this == rhs); }
    bool operator >(const Polynomial& rhs)const {
        auto s = max();
        auto t = rhs.max();
        if (s.first > t.first)return true;
        else if (s.first < t.first)return false;
        for (int i = s.first - 1; i >= 0; i--) {
            if (a[i] > rhs.a[i])return true;
            else if (a[i] < rhs.a[i])return false;
        }
        return false;
    }
    bool operator >=(const Polynomial& rhs)const { return *this > rhs || *this == rhs; }
    bool operator <=(const Polynomial& rhs)const { return !(*this > rhs); }
    bool operator <(const Polynomial& rhs)const { return !(*this >= rhs); }

    Polynomial operator <<(int t)const {
        Polynomial res = *this;
        for (int i = 0; i < a.size(); i++) {
            res[(i - t) % a.size()] = a[i];
        }
        return res;
    }

    //高い次数にずらす
    Polynomial operator >>(int t)const {
        vector<mType> res(a.size() + t);
        for (int i = 0; i < a.size() + t; i++) {
            res[i] = (i < t ? mType(0) : a[i - t]);
        }
        return res;
    }

    mType& operator[](int t) {
        while (a.size() <= t)a.push_back(0);
        return a[t];
    }
    bool isZero()const {
        for (auto& i : a)if (i != 0)return false;
        return true;
    };

    //最大の項の次数と係数を返す
    std::pair<int, mType> max()const {
        for (int i = a.size() - 1; i >= 0; i--) {
            if (a[i] != 0)return { i,a[i] };
        }
        return { 0,0 };
    }
    void clean() {
        if (a.size() == 1)return;
        for (int i = a.size() - 1; i >= 1; i--) {
            if (a[i] != 0)return;
            a.erase(a.begin() + i);
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Polynomial& a)
{
    os << "(";
    if (a.a.back() != 0) {
        os  << a.a.back();
        if (a.a.size() - 1 > 0)os << "x";
        if (a.a.size() - 1 > 1)os << "^" << a.a.size() - 1;
    }
    else {
        os << "0";
    }
    for (int i = a.a.size() - 2; i >= 0; i--) {
        if (a.a[i] == 0)continue;
        os << " + " << a.a[i];
        if (i > 0)os << "x";
        if (i > 1)os << "^" << i;
    }
    os << ")";
    return os;
}