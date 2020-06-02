using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

//Damageableに衝突したとき、healthを減らすクラス
//alwaysAttackがtrueでないとAttack()が実行している最中でないとあたってもダメージが入らないので注意！
public class Damager : MonoBehaviour
{
    [Serializable]
    public class DamagerEvent : UnityEvent<Damager, Damageable> { }//DamagerがDamageableと衝突したとき実行するイベント
    public int damage;//与えるダメージ
    public bool alwaysAttack;//衝突した瞬間Damageを入るようにするか
    public bool ignoreSameTag = true;//自身と同じタグのDamageableにダメージを与えない


    [SerializeField]
    DamagerEvent OnDamageableHit;//Damageableにあたったとき
    public Collider2D damagerCollider;//Damagerの有効範囲

    private void Awake()
    {
        if (OnDamageableHit == null) OnDamageableHit = new DamagerEvent();
    }


    // Update is called once per frame
    void Update()
    {
        if (alwaysAttack)
            Attack();
    }

    public void AddEvent(UnityAction<Damager, Damageable> func)
    {
        OnDamageableHit.AddListener(func);
    }

    Collider2D[] hits = new Collider2D[15];
    public void Attack()
    {
        int count = damagerCollider.OverlapCollider(new ContactFilter2D(), hits);

        for (int i = 0; i < count; i++)
        {
            // m_LastHit = m_AttackOverlapResults[i];
            Damageable damageable = hits[i].GetComponent<Damageable>();

            if (
                damageable == null || damageable.gameObject == gameObject ||//自分自身にダメージを与えるのを防止
                 (ignoreSameTag && damageable.gameObject.tag == tag)//フラグがtrueかつ自身とタグが同じなら無視
                 ) continue;
            if (!damageable.CanTakeDamage()) continue;
            OnDamageableHit.Invoke(this, damageable);
            damageable.TakeDamage(this);//ダメージをdamageableに与える
        }
    }
}
