#include<iostream>
#include<string>

template<typename T>
class Fraction {
	T numerator;//分子
	T denominator;//分母

	T gcd(T x, T y) const {
		if (x < y) {
			T tmp = x;
			x = y;
			y = tmp;
		}
		T r;
		while (y > 0) {
			r = x % y;
			x = y;
			y = r;
		}
		return x;
	}
	T lcm(T x, T y) const {
		return T(x / gcd(x, y)) * y;
	}
public:
	using Real = double;

	Fraction(T numerator_, T denominator_) {
		T g = gcd(numerator_, denominator_);
		numerator = numerator_ / g;
		denominator = denominator_ / g;
	}
	Fraction(T numerator_) :numerator(numerator_), denominator(1) {}
	Fraction(std::string frac) {
		auto last = frac.find_first_of('/');
		if (last != std::string::npos) {
			numerator = std::atoi(frac.substr(0, last).c_str());
			denominator = std::atoi(frac.substr(last + 1, frac.size() - last - 1).c_str());
			return;
		}

		last = frac.find_first_of('.');
		if (last != std::string::npos) {
			frac.erase(last, 1);
			int count = frac.size() - last;
			numerator = std::atoi(frac.c_str());
			denominator = 1;
			for (int i = 0; i < count; ++i)denominator *= 10;
			T g = gcd(numerator, denominator);
			numerator /= g;
			denominator /= g;
			return;
		}
		numerator = 0;
		denominator = 0;
	}

	Real Res() const { return numerator / denominator; }
	Fraction<T> Inv() const { return Fraction<T>(denominator, numerator); }
	bool IsNaN() const { return denominator == 0; }

	Fraction<T> operator +() const { return *this; }
	Fraction<T> operator -()const { return Fraction<T>(-numerator, denominator); }

	Fraction<T> operator +(const Fraction<T>& frac)const {
		T l = lcm(denominator, frac.denominator);
		T a = l / denominator;
		T b = l / frac.denominator;
		return Fraction<T>(numerator * a + frac.numerator * b, l);
	}
	Fraction<T> operator -(const Fraction<T>& frac)const {
		return *this + -*this;
	}
	Fraction<T> operator *(const Fraction<T>& frac)const {
		return Fraction<T>(numerator * frac.numerator, denominator * frac.denominator);
	}
	Fraction<T> operator /(const Fraction<T>& frac) const {
		return Fraction<T>(numerator * frac.denominator, denominator * frac.numerator);
	}

	Fraction<T> operator +=(Fraction<T> frac) { *this = *this + frac; return *this; }
	Fraction<T> operator -=(Fraction<T> frac) { *this = *this - frac; return *this; }
	Fraction<T> operator *=(Fraction<T> frac) { *this = *this * frac; return *this; }
	Fraction<T> operator /=(Fraction<T> frac) { *this = *this / frac; return *this; }


	Fraction<T> operator +(T frac) const { return Fraction<T>(numerator + frac * denominator, denominator); }
	Fraction<T> operator -(T frac) const { return Fraction<T>(numerator - frac * denominator, denominator); }
	Fraction<T> operator *(T frac) const { return Fraction<T>(numerator * frac, denominator); }
	Fraction<T> operator /(T frac)const { return Fraction<T>(numerator, denominator * frac); }

	Fraction<T> operator +=(T frac) { *this = *this + frac; return *this; }
	Fraction<T> operator -=(T frac) { *this = *this - frac; return *this; }
	Fraction<T> operator *=(T frac) { *this = *this * frac; return *this; }
	Fraction<T> operator /=(T frac) { *this = *this / frac; return *this; }

	friend std::ostream& operator<<(std::ostream& os, const Fraction& dt);

};

template<typename T>
std::ostream& operator<<(std::ostream& os,const Fraction<T>& n) {
	os << n.numerator << "/" << n.denominator;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Fraction<int>& n) {
	os << n.numerator << "/" << n.denominator;
	return os;
}
std::ostream& operator<<(std::ostream& os, const Fraction<long>& n) {
	os << n.numerator << "/" << n.denominator;
	return os;
}
