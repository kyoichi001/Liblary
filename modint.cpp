template<long N>
class ModInt {
	using mType = long;
	mType a;
public:
	ModInt() :a(0) {}
	ModInt(mType a_) { a = a_ % N; }
	operator long() { return a; }
	ModInt operator+(const ModInt rhs) const { return ModInt(*this) += rhs; }
	ModInt operator-(const ModInt rhs) const { return ModInt(*this) -= rhs; }
	ModInt operator*(const ModInt rhs) const { return ModInt(*this) *= rhs; }
	ModInt operator/(const ModInt rhs) const { return ModInt(*this) /= rhs; }

	ModInt& operator+=(const ModInt rhs) {
		a += rhs.a;
		if (a >= N) a -= N;
		return *this;
	}
	ModInt& operator-=(const ModInt rhs) {
		if (a < rhs.a)a += N;
		a -= rhs.a;
		return *this;
	}
	ModInt& operator*=(const ModInt rhs) {
		a = a * rhs.a % N;
		return *this;
	}
	ModInt& operator/=(ModInt rhs) {
		mType exp = N - 2;
		while (exp) {
			if (exp & 1)*this *= rhs;
			rhs *= rhs;
			exp /= 2;
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
		return pow(N - 2);
	}
};


template<long N>
std::ostream& operator<<(std::ostream& os, ModInt<N> n) {
	long num = n;
	os << num;
	return os;
}