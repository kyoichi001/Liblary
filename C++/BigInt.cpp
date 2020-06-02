
#include<string>
#include<iostream>

//多倍長整数クラス
class BigInt {
	std::string a;
	bool sgn;
public:
	BigInt() :a("0"), sgn(true) {}
	template<class T>
	BigInt(T a_) : sgn(true) {
		if (std::is_integral<T>::value) {
			a = std::to_string(a_);
		}
		if (a[0] == '-') {
			a.erase(a.begin());
			sgn = false;
		}
	}
	BigInt(std::string a_) :a(a_), sgn(true) {
		if (a[0] == '-') {
			a.erase(a.begin());
			sgn = false;
		}
		adjust();
	}
	BigInt(const char* a_) :a(a_), sgn(true) {
		if (a[0] == '-') {
			a.erase(a.begin());
			sgn = false;
		}
		adjust();
	}
	BigInt(const BigInt& num, bool sgn_) {
		a = num.a;
		sgn = sgn_;
	}
	BigInt(const std::string& num, bool sgn_) {
		a = num;
		sgn = sgn_;
	}
	void operator++() { (*this) += 1; }
	void operator--() { (*this) -= 1; }

	BigInt operator+() const { return BigInt(*this); }
	BigInt operator-() const { return BigInt(a, isnegative()); }
	BigInt operator+(const BigInt& rhs) const { return BigInt(*this) += rhs; }
	BigInt operator-(const BigInt& rhs) const { return BigInt(*this) -= rhs; }
	BigInt operator*(const BigInt& rhs) const { return BigInt(*this) *= rhs; }
	BigInt operator/(const BigInt& rhs) const { return BigInt(*this) /= rhs; }
	BigInt operator%(const BigInt& rhs) const { return BigInt(*this) %= rhs; }

	BigInt& operator+=(const BigInt& rhs) {
		if (isnegative() && !rhs.isnegative()) {
			*this = rhs - abs((*this));
			return *this;
		}
		else if (!isnegative() && rhs.isnegative()) {
			*this = (*this) - abs(rhs);
			return *this;
		}
		else if (isnegative() && rhs.isnegative()) {
			*this = -(abs((*this)) + abs(rhs));
			return (*this);
		}
		int k, m;
		minmax(digit(), rhs.digit(), k, m);

		std::string ans(digit() >= rhs.digit() ? a : rhs.a);
		std::string lef(digit() >= rhs.digit() ? rhs.a : a);
		char kr = '0';
		//下一桁がans[k-1](小さい桁のほう)なので、そこから0まで見ていく
		//A = 123 , B = 12 :: k=2 m=3
		//ans = "123"
		for (int i = 1; i <= k; i++) {
			char tmp = ans[m - i] + lef[k - i] + kr;
			tmp -= '0' * 3;
			kr = '0';
			if (tmp > 9) {
				tmp -= 10;
				kr = '1';
			}
			ans[m - i] = tmp + '0';
		}
		if (m != k) {
			//桁が違えば、
			for (int i = m - k - 1; i >= 0; i--) {
				char tmp = ans[i] + kr;
				tmp -= '0' * 2;
				kr = '0';
				if (tmp > 9) {
					tmp -= 10;
					kr = '1';
				}
				ans[i] = tmp + '0';
			}
		}
		if (kr == '1') {
			ans.insert(ans.begin(), '1');
		}
		a = ans;
		adjust();
		return *this;
	}
	BigInt& operator-=(const BigInt& rhs) {
		if (isnegative() && !rhs.isnegative()) {
			*this = -(rhs + abs(*this));
			return *this;
		}
		else if (!isnegative() && rhs.isnegative()) {
			*this = (*this) + abs(rhs);
			return *this;
		}
		else if (isnegative() && rhs.isnegative()) {
			*this = abs(rhs) - abs(*this);
			return (*this);
		}

		int k, m;
		minmax(digit(), rhs.digit(), k, m);
		bool reverseFlag = (*this) < rhs;
		std::string ans(reverseFlag ? rhs.a : a);
		std::string lef(reverseFlag ? a : rhs.a);

		char kr = '0';
		for (int i = 1; i <= k; i++) {
			char tmp = ans[m - i] - lef[k - i] - kr;
			tmp += '0';
			kr = '0';
			if (tmp < 0) {
				tmp += 10;
				kr = '1';
			}
			ans[m - i] = tmp + '0';
		}
		if (m != k) {
			for (int i = m - k - 1; i >= 0; i--) {
				char tmp = ans[i] - kr;
				kr = '0';
				if (tmp < 0) {
					tmp += 10;
					kr = '1';
				}
				ans[i] = tmp + '0';
			}
		}
		a = ans;
		sgn = !reverseFlag;//if(reverseFlag)sgn=false;
		adjust();
		return *this;
	}
	BigInt& operator*=(const BigInt& rhs) {
		bool nsgn = sgn ^ rhs.sgn;
		karatsuba(rhs);
		sgn = !nsgn;
		adjust();
		return *this;
	}
	BigInt& operator/=(const BigInt& rhs) {
		if (iszero()) return *this;
		if (rhs.iszero()) {
			std::cerr << "error: cant devide by zero." << std::endl;
			return *this;
		}
		BigInt rhs_(rhs, true);
		if ((*this) < rhs_) {//|*this|<|rhs| ==> 0
			a = "0";
			sgn = true;
			return (*this);
		}
		bool nsgn = sgn ^ rhs.sgn;
		sgn = true;
		BigInt M("1"), X("1"), pre("1");
		M.leftshift(digit() + 3);
		while (true) {
			BigInt tmp(X);
			tmp.karatsuba(tmp);
			tmp.karatsuba(rhs_);
			tmp.rightshift(M.digit());
			X += X;
			X -= tmp;
			if (X == pre) break;
			pre = X;
		}
		X.karatsuba(*this);
		X.rightshift(M.digit());
		a = X.a;
		sgn = !nsgn;
		adjust();
		return *this;
	}
	BigInt& operator%=(const BigInt& rhs) {
		if (rhs.iszero()) {
			a = "0";
			sgn = true;
			return *this;
		}
		BigInt tmp(*this);
		tmp /= rhs;
		tmp *= rhs;
		(*this) -= tmp;
		adjust();
		return *this;
	}

	bool operator<(const BigInt& rhs) const {
		if (isnegative() && rhs.isnegative()) {
			return BigInt(rhs.a, true) < BigInt(a, true);
		}
		if (isnegative())return true;
		if (rhs.isnegative())return false;
		if (digit() < rhs.digit())return true;
		if (digit() > rhs.digit())return false;
		for (int i = 0, N = a.size(); i < N; i++) {
			if (a[i] < rhs.a[i]) 	return true;
			if (a[i] > rhs.a[i]) 	return false;
		}
		return false;
	}
	bool operator<=(const BigInt& rhs) const {
		return (*this) < rhs || (*this) == rhs;
	}
	bool operator>(const BigInt& rhs)const {
		if (isnegative() && rhs.isnegative()) {
			return BigInt(rhs.a, true) > BigInt(a, true);
		}
		if (isnegative())return false;
		if (rhs.isnegative())return true;
		if (digit() < rhs.digit())return false;
		if (digit() > rhs.digit())return true;
		for (int i = 0, N = a.size(); i < N; i++) {
			if (a[i] > rhs.a[i]) 	return true;
			if (a[i] < rhs.a[i]) 	return false;
		}
		return false;
	}
	bool operator>=(const BigInt& rhs) const {
		return (*this) > rhs || (*this) == rhs;
	}
	bool operator==(const BigInt& rhs)const { return a == rhs.a; }
	bool operator!=(const BigInt& rhs)const { return a != rhs.a; }

	//[begin,end)までの数を返す
	BigInt copy(int begin, int end)const {
		if (begin < 0)begin = 0;
		end = min(end, digit());
		if (begin >= end) {
			return BigInt();
		}
		return  a.substr(begin, end - begin);
	}


	//10進で桁をずらす
	void leftshift(int k) {
		if (iszero())return;
		a += std::string(k, '0');
	}
	void rightshift(int k) {
		if (a.size() <= k) {//桁数を超える数だけ右にずらすなら0にする
			a = "0";
			sgn = true;
			return;
		}
		a = a.substr(0, a.size() - k);
	}
	//桁数
	inline size_t digit()const { return a.size(); }
	void adjust() {
		if (a.empty()) { a = "0"; sgn = true; return; }
		for (size_t i = 0; i < a.size();) {
			if (a[i] == '0')a.erase(a.begin());
			else return;
		}
		if (a.empty()) { a = "0"; sgn = true; }
	}
	inline bool ispositive()const { return sgn && a != "0"; }
	inline bool iszero()const { return a == "0"; }
	inline bool isnegative()const { return !sgn; }
	friend std::ostream& operator<<(std::ostream& os, const BigInt& dt);
	friend BigInt abs(const BigInt& a);



private:

	BigInt GetLeft(int size2n)const {
		if (size2n / 2 >= a.size()) return BigInt("0");
		return BigInt(a.substr(0, a.size() - size2n / 2));
	}
	BigInt GetRight(int size2n)const {
		if (size2n / 2 >= a.size()) return BigInt(a);
		return BigInt(a.substr(a.size() - size2n / 2));
	}

	std::string mul(BigInt& a, char b) {
		std::string ans = a.a;
		char kr = 0;
		for (int i = a.digit() - 1; i >= 0; i--) {
			char tmp = (a.a[i] - '0') * (b - '0') + kr;
			if (tmp > 9) kr = tmp / 10;
			tmp %= 10;
			ans[i] = tmp + '0';
		}
		if (kr != 0) {
			ans.insert(ans.begin(), kr + '0');
		}
		return ans;
	}

	// O(N^1.58) operator*=
	void karatsuba(const BigInt& o) {
		static int krtb = 0;
		krtb++;
		int len_ = max(digit(), o.digit());
		if (len_ <= 1) {
			if (len_ == 0)std::cerr << len_ << std::endl;
			if (o.digit() != 1)std::cerr << "???" << std::endl;
			(*this) = mul((*this), o.a[0]);
			return;
		}
		int Len = 1;
		while (Len < len_)Len *= 2;
		BigInt ret;
		BigInt b = GetRight(Len), a = GetLeft(Len);
		BigInt d = o.GetRight(Len), c = o.GetLeft(Len);
		BigInt X(a + b), Y(c + d);
		X.karatsuba(Y);
		X.leftshift(Len / 2);
		ret += X;
		BigInt Z(a);
		Z.karatsuba(c);
		BigInt W(b);
		W.karatsuba(d);
		ret += W;
		BigInt tmp(Z);
		tmp.leftshift(Len);
		ret += tmp;
		Z += W;
		Z.leftshift(Len / 2);
		ret -= Z;
		(*this) = ret;
	}

	static int max(int a, int b) { return a > b ? a : b; }
	static int min(int a, int b) { return a < b ? a : b; }
	static void minmax(int a, int b, int& min, int& max) {
		if (a > b) {
			min = b; max = a;
		}
		else {
			min = a; max = b;
		}
	}

};
inline BigInt abs(const BigInt& a) { return BigInt(a.a); }

std::ostream& operator<<(std::ostream& os, const BigInt& dt)
{
	os << (dt.sgn ? "" : "-") << dt.a;
	return os;
}