using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

/// <summary>
/// 効果音を鳴らすクラス
/// SingletonMonoBehaviourはシングルトンクラス
/// </summary>
public class SEManager : SingletonMonoBehaviour<SEManager>
{
    public AudioMixerGroup seOutput;
    Dictionary<AudioClip,AudioSource> m_se = new Dictionary<AudioClip, AudioSource>();

    public void Play(AudioClip clip)
    {
        Instance.play(clip);
    }
     void play(AudioClip clip)
    {
        //もしその音声を鳴らすAudioSourceがなければ作成
        if (!m_se.ContainsKey(clip))
        {
            var child = new GameObject();
            child.transform.parent = transform;
            var audioS = child.AddComponent<AudioSource>();
            audioS.playOnAwake = false;
            audioS.clip = clip;
            audioS.outputAudioMixerGroup = seOutput;
            m_se.Add(clip, audioS);
        }
        m_se[clip].Play();
    }
}
