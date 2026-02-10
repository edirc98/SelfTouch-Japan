using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class IOSScaleController : MonoBehaviour
{
    [Header("Sliders")]
    [SerializeField] private Slider selfSlider;
    [SerializeField] private Slider otherSlider;
    
    [Header("Selection Control")]
    [SerializeField] private KeyCode key = KeyCode.K;
    [SerializeField] private float holdThreshold = 2.0f;
    [SerializeField] private float totalMovingTime = 10.0f;
    [SerializeField] private bool startQuestionnaire = false;
    
    private bool _slidersMoving;
    private float _pressTime;
    private bool _isHolding;
    private float _holdTimer;
    private float _selectionProgress = 0.0f;

    [SerializeField] private int _totalTrials = 4;
    private int _currentTrial = 0;

    
    // Start is called before the first frame update
    void Awake()
    {
        _slidersMoving = false;
    }

    public void OnEnable()
    {
        ResetSliders();
        _currentTrial = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if (startQuestionnaire)
        {
            if (Input.GetKeyDown(key))
            {
                _pressTime = Time.time;
                _isHolding = true;
            }

            if (_isHolding)
            {
                _holdTimer += Time.deltaTime;

                _selectionProgress = Mathf.Clamp01(_holdTimer / holdThreshold);
                if (_selectionProgress >= 1.0f)
                {
                    _currentTrial++;
                    if (_currentTrial >= _totalTrials)
                    {
                        gameObject.SetActive(false);
                        return;
                    }
                    NextTrial();
                    ResetSliders();
                    _isHolding = false;
                }
            }
            
            if (Input.GetKeyUp(key) && _isHolding)
            {
                float heldTime = Time.time - _pressTime;
                _isHolding = false;
                if (heldTime < holdThreshold)
                {
                    _slidersMoving = !_slidersMoving;
                }
            }
            if(_slidersMoving) MoveSliders();
        }
    }

    private void ResetSliders()
    {
        selfSlider.value = 0.0f;
        otherSlider.value = 0.0f;   
    }
    private void NextTrial()
    {
        _slidersMoving = false;
        _selectionProgress = 0.0f;
        _holdTimer = 0.0f;
    }

    private void MoveSliders()
    {
        float range = selfSlider.maxValue - selfSlider.minValue;
        float speed = range / totalMovingTime;

        selfSlider.value += speed * Time.deltaTime;
        otherSlider.value += speed * Time.deltaTime;

        if (selfSlider.value >= selfSlider.maxValue)
        {
            selfSlider.value = selfSlider.maxValue;
            _slidersMoving = false;
        }
    }
}
