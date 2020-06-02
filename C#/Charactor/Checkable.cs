using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

/// <summary>
/// Checkerで起動できるオブジェクト
/// </summary>
public class Checkable : MonoBehaviour
{
    //static PlayerController player;

    [System.Serializable]public class CheckableEvent : UnityEvent<Checker> { }

    public bool m_DestroyAfterChecked = false;
    public bool m_MoveAfterChecked = false;

    [SerializeField]
   public  CheckableEvent OnChecked;



    // Start is called before the first frame update
    void Awake()
    {
        if (OnChecked == null) OnChecked = new CheckableEvent();
    }


    public void TakeCheck(Checker checker)
    {
        OnChecked.Invoke(checker);
        if (m_DestroyAfterChecked)
            Destroy(gameObject);
    }

    //static public void SetPlayer(PlayerController player_) { player = player_; }
}
