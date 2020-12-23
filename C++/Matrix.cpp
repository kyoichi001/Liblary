#include <iostream>
#include <vector>
#include <numeric>//iota
#include <algorithm>//next_permutation
#include <cmath>//回転行列

class Matrix {
public:
	using mType = double;
	std::vector<std::vector<mType>> A;
	//nrow 行の数 ncol 列の数 value 初期値
	Matrix(int nrow, int ncol, mType value = 0) :A(std::vector<std::vector<mType>>(nrow, std::vector<mType>(ncol, value))) {}
	Matrix(const std::vector<std::vector<mType>>& a) :A(a) {}
	//行の数
	int nrow()const { return A.size(); }
	//列の数
	int ncol()const { return A[0].size(); }
	Matrix operator+()const { return *this; }
	Matrix operator-()const {
		Matrix ans(A);
		for (int i = 0; i < nrow(); i++) 
			for (int j = 0; j < ncol(); j++) 
				ans.A[i][j] = -ans.A[i][j];
		return ans;
	}
	Matrix operator+(const Matrix& rhs) const {
		if (!isSameSize(rhs))return *this;
		Matrix ans(ncol(),nrow());
		for (int i = 0; i < nrow(); i++) 
			for (int j = 0; j < ncol(); j++) 
				ans.A[i][j] = A[i][j] + rhs.A[i][j];
		return ans;
	}
	Matrix operator-(const Matrix& rhs) const {
		if (!isSameSize(rhs))return *this;
		Matrix ans(ncol(), nrow());
		for (int i = 0; i < nrow(); i++) 
			for (int j = 0; j < ncol(); j++) 
				ans.A[i][j] = A[i][j] - rhs.A[i][j];
		return ans;
	}
	Matrix operator*(const Matrix& rhs) const {
		if (ncol() != rhs.nrow())return *this;
		Matrix ans(nrow(), rhs.ncol());
		for (int i = 0; i < nrow(); i++) {
			for (int j = 0; j < rhs.ncol(); j++) {
				mType sum = 0;
				for (int k = 0; k < ncol(); k++) {
					sum += A[i][k] * rhs.A[k][j];
				}
				ans.A[i][j] = sum;
			}
		}
		return ans;
	}
	//スカラー倍
	Matrix operator*(mType a) const {
		Matrix ans(nrow(), ncol());
		for (int i = 0; i < nrow(); i++) 
			for (int j = 0; j < ncol(); j++) 
				ans.A[i][j] = A[i][j] * a;
		return ans;
	}
	Matrix operator/(mType a) const {
		Matrix ans(nrow(), ncol());
		for (int i = 0; i < nrow(); i++)
			for (int j = 0; j < ncol(); j++)
				ans.A[i][j] = A[i][j] / a;
		return ans;
	}
	void operator+=(const Matrix& rhs) { *this = *this + rhs; }
	void operator-=(const Matrix& rhs) { *this = *this - rhs; }
	void operator*=(const Matrix& rhs) { *this = *this * rhs; }
	void operator*=(mType a) { *this = *this * a; }
	void operator/=(mType a) { *this = *this / a; }

	bool operator==(const Matrix& rhs) const { return A == rhs.A; }
	bool operator!=(const Matrix& rhs) const { return !(*this == rhs); }

	mType operator()(int row, int col)const { return A[row][col]; }

	bool isSameSize(const Matrix& a)const { return nrow() == a.nrow() && ncol() == a.ncol(); }
	//正方行列か
	bool isSquare()const { return nrow() == ncol(); }

	//転置
	Matrix t()const {
		Matrix ans(ncol(), nrow());
		for (int i = 0; i < nrow(); i++) 
			for (int j = i; j < ncol(); j++) 
				ans.A[i][j] = A[j][i];
		return ans;
	}

	//正則かどうか
	bool isInvertible()const {
		if (!isSquare())return false;
		Matrix cp(A);
		Matrix id = Identity(nrow());

		for (int i = 0; i < ncol(); i++) {
			//上三角行列をつくる
			if (cp.A[i][i] == 0) {//対角が0なら入れ替える
				for (int j = i + 1; j < nrow(); j++) {
					if (cp.A[j][i] != 0) {
						cp.A[i].swap(cp.A[j]);
						id.A[i].swap(id.A[j]);
						break;
					}
				}
				if (cp.A[i][i] == 0)return false; //入れ替えられなかったら正則じゃない
			}
			for (int j = 0; j < ncol(); j++) {//今見てる列の要素を対角の要素で割る
				if (i != j)cp.A[i][j] /= cp.A[i][i];
				id.A[i][j] /= cp.A[i][i];
			}
			cp.A[i][i] = 1;

			for (int j = i + 1; j < nrow(); j++) {
				if (cp.A[j][i] == 0)continue;//対角の要素の下の要素が0ならスキップ
				//対角の要素より下にあるベクトルを対角の要素で割る
				for (int k = 0; k < ncol(); k++) {
					if (k != i)cp.A[j][k] /= cp.A[j][i];
					id.A[j][k] /= cp.A[j][i];
				}
				cp.A[j][i] = 1;

				//対角の要素より下にあるベクトルを対角の要素で引く
				for (int k = 0; k < ncol(); k++) {
					cp.A[j][k] -= cp.A[i][k];
					id.A[j][k] -= id.A[i][k];
				}
			}
		}
		return true;
	}

	Matrix inv()const {//掃き出し法
		if (!isSquare())return *this;
		Matrix cp(A);
		Matrix id = Identity(nrow());
		for (int i = 0; i < ncol(); i++) {
			//上三角行列をつくる
			if (cp.A[i][i] == 0) {//対角が0なら入れ替える
				for (int j = i + 1; j < nrow(); j++) {
					if (cp.A[j][i] != 0) {
						cp.A[i].swap(cp.A[j]);
						id.A[i].swap(id.A[j]);
						break;
					}
				}
				if (cp.A[i][i] == 0)return *this; //入れ替えられなかったら正則じゃないのでエラー
			}
			mType r = cp.A[i][i];
			for (int j = 0; j < ncol(); j++) {//今見てる列の要素を対角の要素で割る
				cp.A[i][j] /= r;
				id.A[i][j] /= r;
			}

			for (int j = i + 1; j < nrow(); j++) {
				if (cp.A[j][i] == 0)continue;//対角の要素の下の要素が0ならスキップ
				mType r2 = cp.A[j][i];
				//対角の要素より下にあるベクトルを対角の要素で割る
				for (int k = 0; k < ncol(); k++) {
					cp.A[j][k] /= r2;
					id.A[j][k] /= r2;
				}

				//対角の要素より下にあるベクトルを対角の要素で引く
				for (int k = 0; k < ncol(); k++) {
					cp.A[j][k] -= cp.A[i][k];
					id.A[j][k] -= id.A[i][k];
				}
			}

			//上三角をあーだこーだ
			for (int j = i - 1; j >= 0; j--) {
				for (int k = 0; k < ncol(); k++) {
					if(k!=i)cp.A[j][k] -= cp.A[i][k] * cp.A[j][i];
					id.A[j][k] -= id.A[i][k] * cp.A[j][i];
				}
				cp.A[j][i] = 0;
			}
		}
		return id;
	}

	//行ベクトル
	std::vector<mType> row(int n)const {
		if (n<0 || n>nrow())return {};
		return A[n];
	}
	//列ベクトル
	std::vector<mType> col(int n)const {
		if (n<0 || n>ncol())return {};
		std::vector<mType> ans(ncol());
		for (int i = 0; i < ncol(); i++) ans[i] = A[i][n];
		return ans;
	}


	//行列式
	//https://hikalium.com/lectures/c/0004.html
	mType det()const {
		if (!isSquare())return 0;
		mType det = 1;
		auto mat = A;

		// 上三角行列に変換しつつ、対角成分の積を計算する。
		for (int i = 0; i < nrow(); i++) {
			if (mat[i][i] == 0) {
				// 対角成分が0だった場合は、その列の値が0でない行と交換する
				for (int j = i + 1; j < nrow(); j++) {
					if (mat[j][i] != 0) {
						mat[i].swap(mat[j]);
						// 列を交換したので行列式の値の符号は反転する。
						det = -det;
						break;
					}
				}
			}
			for (int j = i + 1; j < nrow(); j++) {
				mType r = mat[j][i] / mat[i][i];
				for (int k = i; k < ncol(); k++) {
					mat[j][k] -= r * mat[i][k];
				}
			}
			det *= mat[i][i];
		}
		return det;
	}

	//トレース
	mType tr() {
		if (!isSquare())return 0;
		mType ans = 0;
		for (int i = 0; i < nrow(); i++) ans += A[i][i];
		return ans;
	}

	static Matrix Zero(int nrow, int ncol) { return Matrix(nrow, ncol, 0); }
	static Matrix One(int nrow, int ncol) { return Matrix(nrow, ncol, 1); }
	static Matrix Identity(int size) {
		Matrix ans(size, size);
		for (int i = 0; i < size; i++) ans.A[i][i] = 1;
		return ans;
	}
	static Matrix Rotate2D(double theta) {
		return Matrix({
			{std::cos(theta),-std::sin(theta)},
			{ std::sin(theta), std::cos(theta)}
			});
	}
};

//縦ベクトル
class Vector {
public:
	using vType = double;
	std::vector<vType> A;
	Vector(int size, vType value = 0) :A(size, value) {}
	Vector(const std::vector<vType>& a) :A(a) {}

	int size()const { return A.size(); }

	Vector operator+()const { return *this; }
	Vector operator-()const {
		Vector ans(A);
		for (int i = 0; i < size(); i++) ans.A[i] = -A[i];
		return ans;
	}

	Vector operator+(const Vector& rhs)const {
		if (!isSameSize(rhs))return *this;
		Vector ans(A);
		for (int i = 0; i < size(); i++) ans.A[i] = A[i] + rhs.A[i];
		return ans;
	}
	Vector operator-(const Vector& rhs)const {
		if (!isSameSize(rhs))return *this;
		Vector ans(A);
		for (int i = 0; i < size(); i++) ans.A[i] = A[i] - rhs.A[i];
		return ans;
	}
	Vector operator*(vType a)const {
		Vector ans(A);
		for (int i = 0; i < size(); i++) ans.A[i] = A[i] * a;
		return ans;
	}
	Vector operator/(vType a)const {
		Vector ans(A);
		for (int i = 0; i < size(); i++) ans.A[i] = A[i] / a;
		return ans;
	}
	void operator+=(const Vector& rhs) { *this = *this + rhs; }
	void operator-=(const Vector& rhs) { *this = *this - rhs; }
	void operator*=(vType a) { *this = *this * a; }
	void operator/=(vType a) { *this = *this / a; }

	bool operator==(const Vector& rhs) const { return A == rhs.A; }
	bool operator!=(const Vector& rhs) const { return !(*this == rhs); }
	
	Matrix t()const { return Matrix({ A }); }

	vType dot(const Vector& rhs)const {
		if (!isSameSize(rhs))return 0;
		vType ans = 0;
		for (int i = 0; i < size(); i++) ans += A[i] * rhs.A[i];
		return ans;
	}
	vType magniudeSqr()const { return dot(*this); }
	vType magniude()const { return std::sqrt(magniudeSqr()); }

	bool isSameSize(const Vector& rhs)const { return size() == rhs.size(); }

	static Vector Zero(int size) { return Vector(size, 0); }
	static Vector One(int size) { return Vector(size, 1); }
};


Matrix operator*(Matrix::mType a, const Matrix& n) {
	Matrix ans = n;
	for (int i = 0; i < n.nrow(); i++) 
		for (int j = 0; j < n.ncol(); j++) 
			ans.A[i][j] = a * ans.A[i][j];
	return ans;
}
Vector operator*(Vector::vType a, const Vector& n) {
	Vector ans = n;
	for (int i = 0; i < n.size(); i++)ans.A[i] = a * ans.A[i];
	return ans;
}
Vector operator*(const Matrix& lhs,const Vector& rhs) {
	if (lhs.ncol() != rhs.size())return rhs;
	Vector ans(lhs.nrow());
	for (int i = 0; i < lhs.nrow(); i++) {
		Vector::vType sum = 0;
		for (int j = 0; j < lhs.ncol(); j++) sum += lhs.A[i][j] * rhs.A[j];
		ans.A[i] = sum;
	}
	return ans;
}

std::ostream& operator<<(std::ostream& ost, const Matrix& n) {
	for (int i = 0; i < n.nrow(); i++) {
		for (int j = 0; j < n.ncol(); j++) {
			std::cout << n(i, j) << " ";
		}
		std::cout << std::endl;
	}
	return ost;
}
std::ostream& operator<<(std::ostream& ost, const Vector& n) {
	std::cout << "(" << n.A[0];
	for (int i = 1; i < n.size(); i++) {
		std::cout << "," << n.A[i];
	}
	std::cout << ")";
	return ost;
}