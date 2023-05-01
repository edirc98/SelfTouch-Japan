using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class FeedbackDestroy : MonoBehaviour
{
    public float destroySecs = 0.2f;
    public AudioClip _destructionClip;
    private AudioSource _destructionAudioSource;
    private MeshRenderer _feedbackObjectMeshRenderer;
    private SphereCollider _feedbackObjectCollider;
    void Start()
    {
        _destructionAudioSource = gameObject.GetComponent<AudioSource>();
        if (_destructionAudioSource == null)
        {
            Debug.LogError("No audio source found on: " + gameObject.name);
        }
        _feedbackObjectMeshRenderer = GetComponent<MeshRenderer>();
        _feedbackObjectCollider = GetComponent<SphereCollider>();
    }

    private void OnTriggerEnter(Collider other)
    {
        if(other.CompareTag("VibrationObject") != true && other.CompareTag("Hand") == true)
        {
            _feedbackObjectCollider.enabled = false;
            _feedbackObjectMeshRenderer.enabled = false;
            _destructionAudioSource.PlayOneShot(_destructionClip);
            Destroy(gameObject, destroySecs);
        }
    }
}
