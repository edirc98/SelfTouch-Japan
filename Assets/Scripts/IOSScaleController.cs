using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

[System.Serializable]
public class IOSResponse
{
    public int trial;
    public int value;

    public IOSResponse() {}
    public IOSResponse(int trial, int value)
    {
        this.trial = trial;
        this.value = value;
    }
}

[System.Serializable]
public class IOSResponses
{
    public string date;
    public string time;
    public List<IOSResponse> responses;

    public IOSResponses()
    {
        date = DateTime.Now.ToString("dd/MM/yy");
        time = DateTime.Now.ToString("HH:mm");
        responses = new List<IOSResponse>();
    }
}

public class IOSScaleController : MonoBehaviour
{
    [Header("Sliders")]
    [SerializeField] private Slider selfSlider;
    [SerializeField] private Slider otherSlider;
    [SerializeField] private Slider progressSlider;

    [Header("Circle Texts")] 
    [SerializeField] private TMPro.TMP_Text selfText; 
    [SerializeField] private TMPro.TMP_Text otherText;
    
    [Header("Selection Control")]
    [SerializeField] private KeyCode key = KeyCode.K;
    [SerializeField] private float holdThreshold = 2.0f;
    [SerializeField] private float totalMovingTime = 10.0f;
    [SerializeField] private bool startQuestionnaire = false;
    
    private IOSResponses _responses;
    
    private bool _slidersMoving;
    private bool _changeWay = false; 
    private float _pressTime;
    private bool _isHolding;
    private float _holdTimer;
    private float _selectionProgress = 0.0f;

    [SerializeField] private int _totalTrials = 4;
    private int _currentTrial = 0;

    
    // Start is called before the first frame update
    void Awake()
    {
        _responses = new IOSResponses();
    }

    public void OnEnable()
    {
        ResetSliders(0);
        startQuestionnaire = true;
        _currentTrial = 0;
        _isHolding = false;
        _holdTimer = 0.0f; 
    }

    public void OnDisable()
    {
        startQuestionnaire = false;
        _responses.responses.Clear();
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
                UpdateProgressSlider(_selectionProgress);
                if (_selectionProgress >= 1.0f)
                {
                    //Save response
                    var response = new IOSResponse(_currentTrial, (int)selfSlider.value);
                    _responses.responses.Add(response);
                    
                    _currentTrial++;
                    if (_currentTrial >= _totalTrials)
                    {
                        QuestionnaireDataSaver.SaveToJson(_responses,"IOS_Questionnaire");
                        Invoke(nameof(FinishQuestionnaire),0.5f);
                        startQuestionnaire = false;
                        return;
                    }
                    NextTrial();
                    if (_currentTrial < _totalTrials / 2)
                    {
                        ResetSliders(0);
                    }
                    else {ResetSliders(1);}
                    
                }
            }
            
            if (Input.GetKeyUp(key) && _isHolding)
            {
                float heldTime = Time.time - _pressTime;
                _selectionProgress = 0.0f;
                _holdTimer = 0.0f;
                _isHolding = false;
                if (heldTime < holdThreshold)
                {
                    _slidersMoving = !_slidersMoving;
                    _selectionProgress = 0.0f; 
                    UpdateProgressSlider(_selectionProgress);
                }
            }
            if(_slidersMoving) MoveSliders();
        }
    }

    private void ResetSliders(int startPos)
    {
        if (startPos == 0)
        {
            selfSlider.value = 0.0f;
            otherSlider.value = 0.0f;
        }
        else if (startPos == 1)
        {
            selfSlider.value = selfSlider.maxValue;
            otherSlider.value = otherSlider.maxValue;
        }

        progressSlider.value = 0.0f; 
        _selectionProgress = 0.0f; 


    }
    private void NextTrial()
    {
        _slidersMoving = false;
        _selectionProgress = 0.0f;
        _holdTimer = 0.0f;
        if (_currentTrial != 0)
        {
            selfText.text = "Self \n (My Body)";
            otherText.text = "Other \n (My Avatar)";
        }
    }

    private void MoveSliders()
    {
        float range = selfSlider.maxValue - selfSlider.minValue;
        float speed = range / totalMovingTime;

        if (!_changeWay)
        {
            selfSlider.value += speed * Time.deltaTime;
            otherSlider.value += speed * Time.deltaTime;
        }
        else
        {
            selfSlider.value -= speed * Time.deltaTime;
            otherSlider.value -= speed * Time.deltaTime;
        }
        
        if (selfSlider.value >= selfSlider.maxValue || selfSlider.value <= selfSlider.minValue)
        {
            _changeWay = !_changeWay;
            //selfSlider.value = selfSlider.maxValue;
            //slidersMoving = false;
        }
    }

    private void UpdateProgressSlider(float value)
    {
        progressSlider.value = value;
    }

    private void FinishQuestionnaire()
    {
        gameObject.SetActive(false);
    }
}
