using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.PlayerLoop;

public class LikertScaleController : MonoBehaviour
{
    public List<String> questions = new List<String>();
    public List<CanvasGroup> scaleButtons;
    public TMPro.TMP_Text questionText;
    
    public bool startQuestionaire = false;

    
    [Header("Selection Control")]
    [SerializeField] private KeyCode key = KeyCode.K;
    [SerializeField] private float holdThreshold = 2.0f;

    private float _pressTime;
    private bool _isHolding;
    private float _holdTimer;
    
    private int _currentOption = 0;
    private int _currentQuestion = 0;
    private float _selectionProgress = 0.0f;
    
    private void Start()
    {
        ResetQuestionaire();
        NextQuestion(_currentQuestion);
        SelectButton(_currentOption);
    }


    private void Update()
    {
        if (startQuestionaire)
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
                scaleButtons[_currentOption].alpha = Mathf.Lerp(0.3f, 1f, _selectionProgress);
                if (_selectionProgress >= 1.0f)
                {
                    NextQuestion(++_currentQuestion);
                    _isHolding = false;
                }
            }
            
            if (Input.GetKeyUp(key) && _isHolding)
            {
                float heldTime = Time.time - _pressTime;
                _isHolding = false;
                if (heldTime < holdThreshold)
                {
                    //Next Option
                    DeselectButton(_currentOption);
                    _currentOption = (_currentOption + 1) % scaleButtons.Count;
                    SelectButton(_currentOption);
                }
            }
        }
    }
    public void SelectButton(int i)
    {
        scaleButtons[i].alpha = 0.3f;
    }

    public void DeselectButton(int i)
    {
        scaleButtons[i].alpha = 0.0f;
    }

    private void NextQuestion(int i)
    {
        questionText.text = questions[i];
        _selectionProgress = 0.0f;
        _holdTimer = 0.0f;
        DeselectButton(_currentOption);
        
        _currentOption = 0;
        SelectButton(_currentOption);
        
    }
    
    public void ResetQuestionaire()
    {
        _currentOption = 0;
        _currentQuestion = 0;
    }
}
