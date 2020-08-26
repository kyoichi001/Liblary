# 自作ライブラリ保管
自分で制作したライブラリを保管しています。主にC++かUnity(C#)になると思います。


## C++
### 数学
- [剰余環 ModInt](https://github.com/kyoichi001/Liblary/blob/master/C++/ModInt.cpp)
- [多倍長整数 BigInt](https://github.com/kyoichi001/Liblary/blob/master/C++/BigInt.cpp)
- [分数 Fraction](https://github.com/kyoichi001/Liblary/blob/master/C++/Fraction.cpp)

### ゲーム用
- [補間クラス Easing](https://github.com/kyoichi001/Liblary/tree/master/C%2B%2B/Easing)
- [ゲーム内オブジェクト管理リスト ObjectList](https://github.com/kyoichi001/Liblary/tree/master/C%2B%2B/ObjectList)
- [衝突判定](https://github.com/kyoichi001/Liblary/blob/master/C++/Collision/Collision.h)
- [交差判定](https://github.com/kyoichi001/Liblary/blob/master/C++/Collision/intersectAt.h)

### 空間分割
- [2次元用](https://github.com/kyoichi001/Liblary/blob/master/C++/Collision/Morton2D.h)
モートン符号を利用した2次元空間分割。
四分木にオブジェクトを挿入し、探索を効率化。更新は全要素を削除してからまたすべて挿入しなおす。
	- 挿入 O(1)
	- 全要素削除 O(N)
	- 探索 O(logN ? )

### その他競プロ用など
- [UnionFind](https://github.com/kyoichi001/Liblary/blob/master/C++/UnionFind.h)

- [3次元用](https://github.com/kyoichi001/Liblary/blob/master/C++/Collision/Morton3D.cpp)
モートン符号を利用した3次元空間分割。
2次元のものとほとんど同じ。

## Unity(C#)
- [シングルトン Singleton](https://github.com/kyoichi001/Liblary/blob/master/C%23/SingletonMonoBehaviour.cs)
- [エンティティ Checkable](https://github.com/kyoichi001/Liblary/blob/master/C%23/Charactor/Checkable.cs)
- [エンティティチェック Checker](https://github.com/kyoichi001/Liblary/blob/master/C%23/Charactor/Checker.cs)
- [HP管理 Damageable](https://github.com/kyoichi001/Liblary/blob/master/C%23/Charactor/Damageable.cs)
- [攻撃管理 Damager](https://github.com/kyoichi001/Liblary/blob/master/C%23/Charactor/Damager.cs)
- [BGM管理 BGMManager](https://github.com/kyoichi001/Liblary/blob/master/C%23/Audio/BGMManager.cs)
