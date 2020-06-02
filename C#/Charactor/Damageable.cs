using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

using NaughtyAttributes;

/// <summary>
/// ダメージを受けることができる(HPのある)オブジェクト
/// </summary>
public class Damageable : MonoBehaviour
{
    [System.Serializable]public class DamageableEvent : UnityEvent<int> { }

    [Header("Health Option")]
    public int MaxHealth;
    public int InitialHealth;//デフォルトのHP
    public float coolTime;//ダメージを受けてからの一定の無敵時間

    public int Health
    {
        get {return m_health; }
        set { m_health = Mathf.Clamp(value, 0, MaxHealth); }
    }
    public float NormalizedHealth
    {
        get { return (float)m_health / (float)InitialHealth; }
    }
    [ShowNonSerializedField]
    int m_health;

    [NaughtyAttributes.ShowNonSerializedField]
    bool m_damageFlag;//ダメージを受け付けるかどうか

    [NaughtyAttributes.ShowNonSerializedField]
    float m_coolTime=0f;

    [Header("Callback functions"), SerializeField]
    public UnityEvent OnDead;
    [SerializeField]
    public DamageableEvent OnTakeDamage;//そのとき受けたダメージを引数に入れる
    [SerializeField]
    public DamageableEvent OnTakeHeal;//そのとき受けたダメージを引数に入れる


    [System.NonSerialized]
    public bool activeFlag=true;
    private void Awake()
    {
        if (OnDead == null) OnDead = new UnityEvent();
        if (OnTakeDamage == null) OnTakeDamage = new DamageableEvent();
        if (OnTakeHeal == null) OnTakeHeal = new DamageableEvent();
        m_coolTime = 0f;
        m_damageFlag = true;
        m_health = InitialHealth;
    }

    public void Initialize(int initialHealth,int maxHealth,float coolTime_)
    {
        MaxHealth = maxHealth;
        InitialHealth = initialHealth;
        Health = initialHealth;
        coolTime = coolTime_;
    }

    // Update is called once per frame
    void Update()
    {
        if (!m_damageFlag)
        {
            m_coolTime += Time.deltaTime;
            if (m_coolTime > coolTime)
            {
                m_damageFlag = true;
                m_coolTime = 0f;
            }
        }
    }

    public void TakeDamage(Damager damager)//ダメージを受ける
    {
        if (!activeFlag || !m_damageFlag || IsDead()) return;
        Debug.Log("take damage:" + damager.damage, gameObject);
        Health -= damager.damage;
        OnTakeDamage.Invoke(damager.damage);
        if (IsDead())//もしHPが0以下になったら死んだときのイベントを呼び出す
        {
            OnDead.Invoke();
        }
        m_damageFlag = false;
    }

    public void TakeHeal(int health)
    {
        if (!activeFlag || IsDead()) return;
        Health += health;
        OnTakeHeal.Invoke((int)health);
    }

    public bool IsDead() { return m_health == 0; }
    public bool CanTakeDamage() { return m_damageFlag; }


}
