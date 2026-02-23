using System;
using System.Collections.Generic;
using UnityEditor.AddressableAssets.Build.AnalyzeRules;
using UnityEngine;

[System.Serializable]
public class LikertResponse
{
    public string questionText;
    public int response; 
    
    public LikertResponse(){}

    public LikertResponse(string questionText, int response)
    {
        this.questionText = questionText;
        this.response = response;
    }
}
[System.Serializable]
public class LikertResponses
{
    public string date;
    public string time;
    public List<LikertResponse> responses;

    public LikertResponses()
    {
        date = DateTime.Now.ToString("dd/MM/yy");
        time = DateTime.Now.ToString("HH:mm");
        responses = new List<LikertResponse>();
    }
}


public class LikertScaleController : MonoBehaviour
{
    [Header("Questionnaire Name")]
    public string questionnaireName;
    
    [SerializeField] private List<LikertQuestion> questions;
    public LikertResponses results;
    public List<CanvasGroup> scaleButtons;
    
    [Header("Question Texts")]
    public TMPro.TMP_Text questionText;
    public TMPro.TMP_Text negativeText;
    public TMPro.TMP_Text positiveText;
    
    public bool startQuestionnaire = false;

    
    [Header("Selection Control")]
    [SerializeField] private KeyCode key = KeyCode.K;
    [SerializeField] private float holdThreshold = 2.0f;
    
    private float _pressTime;
    private bool _isHolding;
    private float _holdTimer;
    
    private int _currentOption = 0;
    [SerializeField] private int _currentQuestion = 0;
    private float _selectionProgress = 0.0f;

    public void Awake()
    {
        results = new LikertResponses();
        LoadQuestionnaire();
    }

    public void OnEnable()
    {
        if (questions.Count > 0)
        {
            startQuestionnaire = true;
            ResetQuestionnaire();
            NextQuestion(_currentQuestion);
            SelectButton(_currentOption);
        }
    }

    public void OnDisable()
    {
        startQuestionnaire = false;
        //questions.Clear();
        results.responses.Clear();
    }


    private void Update()
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
                scaleButtons[_currentOption].alpha = Mathf.Lerp(0.3f, 1f, _selectionProgress);
                if (_selectionProgress >= 1.0f)
                {
                    
                    SaveQuestion(_currentQuestion,_currentOption);
                    
                    _currentQuestion++;
                    if (_currentQuestion >= questions.Count)
                    {
                        startQuestionnaire = false;
                        QuestionnaireDataSaver.SaveToJson(results,questionnaireName);
                        Invoke(nameof(FinishQuestionnaire),0.5f);
                        DeselectButton(_currentOption);
                        return;
                    }
                    NextQuestion(_currentQuestion);
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
                    ResetSelection();
                    _currentOption = (_currentOption + 1) % scaleButtons.Count;
                    SelectButton(_currentOption);
                }
            }
        }
    }
    
    private void LoadQuestionnaire()
    {
        questions = QuestionnaireLoader.LoadQuestionnaire(questionnaireName);
        
    }
    
    private void SelectButton(int i)
    {
        scaleButtons[i].alpha = 0.3f;
    }

    private void DeselectButton(int i)
    {
        scaleButtons[i].alpha = 0.0f;
    }

    private void NextQuestion(int i)
    {
        LikertQuestion q = questions[i];
        questionText.text = q.questionText;
        negativeText.text = q.negativeText;
        positiveText.text = q.positiveText;

        ResetSelection();
        
        _currentOption = 0;
        SelectButton(_currentOption);
    }

    private void SaveQuestion(int question,int option)
    {
        //Save response
        var result = new LikertResponse(questions[question].questionText,option);
        results.responses.Add(result);
    }
    
    private void ResetQuestionnaire()
    {
        _currentOption = 0;
        _currentQuestion = 0;
        _selectionProgress = 0.0f;
        _holdTimer = 0.0f;
    }

    private void ResetSelection()
    {
        _selectionProgress = 0.0f;
        _holdTimer = 0.0f;
        DeselectButton(_currentOption);
    }
    
    private void FinishQuestionnaire()
    {
        gameObject.SetActive(false);
    }
}
