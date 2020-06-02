using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

/// <summary>
/// BGMを管理するクラス
/// </summary>
public class BGMManager : SingletonMonoBehaviour<BGMManager>,ISaveable,ILoadable
{

    protected AudioSource m_MusicAudioSource;
    protected AudioSource m_AmbientAudioSource;

    [Header("Music Settings")]
    public AudioClip musicAudioClip;
    public AudioMixerGroup musicOutput;
    public bool musicPlayOnAwake = true;
    [Range(0f, 1f)]
    public float musicVolume = 1f;
    [Header("Ambient Settings")]
    public AudioClip ambientAudioClip;
    public AudioMixerGroup ambientOutput;
    public bool ambientPlayOnAwake = true;
    [Range(0f, 1f)]
    public float ambientVolume = 1f;

    float m_BGMMixerVolume=0;
    float m_SEMixerVolume=0;

    protected bool m_TransferMusicTime, m_TransferAmbientTime;

    //every clip pushed on that stack throught "PushClip" function will play until completed, then pop
    //once that stack is empty, it revert to the musicAudioClip
    protected Stack<AudioClip> m_MusicStack = new Stack<AudioClip>();
    new void Awake()
    {
        base.Awake();
        //DontDestroyOnLoad(gameObject);

        m_MusicAudioSource = gameObject.AddComponent<AudioSource>();
        m_MusicAudioSource.clip = musicAudioClip;
        m_MusicAudioSource.outputAudioMixerGroup = musicOutput;
        m_MusicAudioSource.loop = true;
        m_MusicAudioSource.volume = musicVolume;

        if (musicPlayOnAwake)
        {
            m_MusicAudioSource.time = 0f;
            m_MusicAudioSource.Play();
        }

        m_AmbientAudioSource = gameObject.AddComponent<AudioSource>();
        m_AmbientAudioSource.clip = ambientAudioClip;
        m_AmbientAudioSource.outputAudioMixerGroup = ambientOutput;
        m_AmbientAudioSource.loop = true;
        m_AmbientAudioSource.volume = ambientVolume;

        if (ambientPlayOnAwake)
        {
            m_AmbientAudioSource.time = 0f;
            m_AmbientAudioSource.Play();
        }
        SaveLoadManager.Instance.SetLoadable(this);
        SaveLoadManager.Instance.SetSaveable(this);
    }
    private void Update()
    {
        if (m_MusicStack.Count > 0)
        {
            //isPlaying will be false once the current clip end up playing
            if (!m_MusicAudioSource.isPlaying)
            {
                m_MusicStack.Pop();
                if (m_MusicStack.Count > 0)
                {
                    m_MusicAudioSource.clip = m_MusicStack.Peek();
                    m_MusicAudioSource.Play();
                }
                else
                {//Back to looping music clip
                    m_MusicAudioSource.clip = musicAudioClip;
                    m_MusicAudioSource.loop = true;
                    m_MusicAudioSource.Play();
                }
            }
        }
    }

    //今流れているBGMに割り込んで再生する
    public void PushClip(AudioClip clip)
    {
        m_MusicStack.Push(clip);
        m_MusicAudioSource.Stop();
        m_MusicAudioSource.loop = false;
        m_MusicAudioSource.time = 0;
        m_MusicAudioSource.clip = clip;
        m_MusicAudioSource.Play();
    }

    public void ChangeMusic(AudioClip clip)
    {
        musicAudioClip = clip;
        m_MusicAudioSource.clip = clip;
    }

    public void ChangeAmbient(AudioClip clip)
    {
        ambientAudioClip = clip;
        m_AmbientAudioSource.clip = clip;
    }

    public void Play()
    {
        PlayJustAmbient();
        PlayJustMusic();
    }
    //BGMだけを再生
    public void PlayJustMusic()
    {
        m_MusicAudioSource.time = 0f;
        m_MusicAudioSource.Play();
    }
    //環境音だけ再生
    public void PlayJustAmbient()
    {
        m_AmbientAudioSource.Play();
    }

    public void Stop()
    {
        StopJustAmbient();
        StopJustMusic();
    }

    public void StopJustMusic() { m_MusicAudioSource.Stop(); }

    public void StopJustAmbient() { m_AmbientAudioSource.Stop(); }

    //0～5までの整数値で音量を指定する
    public void SetVolumeBy5(int value)
    {
        Debug.Log(value);
        ambientVolume = (float)value / 5f;
        musicVolume = (float)value / 5f;
        ambientOutput.audioMixer.SetFloat("BGM", -80f + musicVolume * (80f));
        ambientOutput.audioMixer.SetFloat("Ambient", -80f + ambientVolume * (80f));
    }

    public void Mute()
    {
        MuteJustAmbient();
        MuteJustMusic();
    }

    public void MuteJustMusic() { m_MusicAudioSource.volume = 0f; }
    public void MuteJustAmbient() { m_AmbientAudioSource.volume = 0f; }

    public void Unmute()
    {
        UnmuteJustAmbient();
        UnmuteJustMustic();
    }

    public void UnmuteJustMustic(){m_MusicAudioSource.volume = musicVolume;}
    public void UnmuteJustAmbient() { m_AmbientAudioSource.volume = ambientVolume; }

    public void Mute(float fadeTime)
    {
        MuteJustAmbient(fadeTime);
        MuteJustMusic(fadeTime);
    }

    public void MuteJustMusic(float fadeTime)
    {
        StartCoroutine(VolumeFade(m_MusicAudioSource, 0f, fadeTime));
    }

    public void MuteJustAmbient(float fadeTime)
    {
        StartCoroutine(VolumeFade(m_AmbientAudioSource, 0f, fadeTime));
    }

    public void Unmute(float fadeTime)
    {
        UnmuteJustAmbient(fadeTime);
        UnmuteJustMusic(fadeTime);
    }

    //BGMの音量をfadeTimeかけてもとに戻す
    public void UnmuteJustMusic(float fadeTime)
    {
        StartCoroutine(VolumeFade(m_MusicAudioSource, musicVolume, fadeTime));
    }

    //環境音の音量をfadeTimeかけてもとに戻す
    public void UnmuteJustAmbient(float fadeTime)
    {
        StartCoroutine(VolumeFade(m_AmbientAudioSource, ambientVolume, fadeTime));
    }

    protected IEnumerator VolumeFade(AudioSource source, float finalVolume, float fadeTime)
    {
        float volumeDifference = Mathf.Abs(source.volume - finalVolume);
        float inverseFadeTime = 1f / fadeTime;

        while (!Mathf.Approximately(source.volume, finalVolume))
        {
            float delta = Time.deltaTime * volumeDifference * inverseFadeTime;
            source.volume = Mathf.MoveTowards(source.volume, finalVolume, delta);
            yield return null;
        }
        source.volume = finalVolume;
    }

    public IEnumerator Save(PlayerDataTable data)
    {
        data.BGMVolume = (int)((m_BGMMixerVolume + 80f) / 80f * 5f);
        data.SEVolume = (int)((m_SEMixerVolume + 80f) / 80f * 5f);
        yield return null;
    }

    public IEnumerator Load(PlayerDataTable data)
    {
        m_BGMMixerVolume =-80f+ data.BGMVolume/5f*80f;
        m_SEMixerVolume = -80f + data.SEVolume / 5f * 80f;
        m_optionUI.Initialize(data.BGMVolume, data.SEVolume);
        yield return null;
    }
}

