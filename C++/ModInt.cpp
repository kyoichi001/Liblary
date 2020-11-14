//https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a

#include<iostream>
template<long N>
class ModInt {
	using mType = long;
	mType a;


	constexpr int fracSize=10000000;
	mType frac[fracSize];
	mType fracInv[fracSize];
	mType inv[fracSize];
	void Calc() {
		frac[0] = frac[1] = 1;
		fracInv[0] = fracInv[1] = 1;
		inv[1] = 1;
		for (int i = 2; i < fracSize; i++) {
			frac[i] = (i * frac[i - 1]) % N;
			inv[i] = N - inv[N % i] * (N / i) % N;
			finv[i] = finv[i - 1] * inv[i] % N;
		}
	}

public:
	ModInt() :a(0) { Calc(); }
	ModInt(mType a_) {
		if (a_ < 0) {
			a = N - ((-a_) % N);
		}
		else {
			a = a_ % N;
		}
		Calc();
	}

	mType get()const { return a; }
	ModInt operator+()const { return *this; }
	ModInt operator-()const { return { N - a }; }
	ModInt operator+(const ModInt& rhs) const { return ModInt(*this) += rhs; }
	ModInt operator-(const ModInt& rhs) const { return ModInt(*this) -= rhs; }
	ModInt operator*(const ModInt& rhs) const { return ModInt(*this) *= rhs; }
	ModInt operator/(const ModInt& rhs) const { return ModInt(*this) /= rhs; }

	bool operator==(const ModInt& rhs)const { return a == rhs.a; }
	bool operator> (const ModInt& rhs)const { return a >  rhs.a; }
	bool operator>=(const ModInt& rhs)const { return a >= rhs.a; }
	bool operator<=(const ModInt& rhs)const { return a <= rhs.a; }
	bool operator< (const ModInt& rhs)const { return a <  rhs.a; }

	ModInt& operator+=(const ModInt& rhs) {
		a += rhs.a;
		if (a >= N) a -= N;
		return *this;
	}
	ModInt& operator-=(const ModInt& rhs) {
		if (a < rhs.a)a += N;
		a -= rhs.a;
		return *this;
	}
	ModInt& operator*=(const ModInt& rhs) {
		a = a * rhs.a % N;
		return *this;
	}
	ModInt& operator/=(ModInt rhs) {
		mType exp = N - 2;
		while (exp) {
			if (exp & 1)*this *= rhs;
			rhs *= rhs;
			exp >>= 1;//exp/=2
		}
		return *this;
	}
	ModInt pow(mType k)const {
		ModInt res(1), x(a);
		while (k) {
			if (k & 1) res *= x;
			x *= x;
			k >>= 1;
		}
		return res;
	}
	ModInt inv()const {
		return pow(N - 2);//inv[a];
	}

	mType nPk(int n, int k) {
		if (n < k) return 0;
		if (n < 0 || k < 0) return 0;
		return (frac[n] * fracInv[n - k]) % N;
	}
	// 二項係数計算
	mType nCk(int n, int k) {
		if (n < k) return 0;
		if (n < 0 || k < 0) return 0;
		return frac[n] * (fracInv[k] * fracInv[n - k] % N) % N;
	}
};

template<long N>
ModInt<N> operator+(long a, const ModInt<N>& n) { return n + a; }
template<long N>
ModInt<N> operator-(long a, const ModInt<N>& n) { return -n + a; }
template<long N>
ModInt<N> operator*(long a, const ModInt<N>& n) { return n * a; }
template<long N>
ModInt<N> operator/(long a, const ModInt<N>& n) { return a * n.inv(); }

template<long N>
std::ostream& operator<<(std::ostream& os, const ModInt<N>& n) {
	os << n.get();
	return os;
}
