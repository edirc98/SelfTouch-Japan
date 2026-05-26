using System.Collections.Generic;
using QuickVR;
using UnityEngine;

public class YesNoQuestionController : MonoBehaviour
{
    [Header("Questionnaire Name")]
    public string questionnaireName;
    
    [SerializeField] private List<LikertQuestion> questions;
    public LikertResponses results;
    public List<CanvasGroup> scaleButtons;
    
    [Header("Instructions")]
    public GameObject instructionsGO;
    
    [Header("Question Texts")]
    public TMPro.TMP_Text questionText;
    
    public bool startQuestionnaire = false;

    
    [Header("Selection Control")]
    [SerializeField] private KeyCode key = KeyCode.K;
    [SerializeField] private float holdThreshold = 2.0f;
    
    
    [Header("Loop to modify")]
    [SerializeField] private QuickStageLoop loop;
    
    
    
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
            ResetQuestionnaire();
            ResetSelection();
            NextQuestion(_currentQuestion);
            SelectButton(_currentOption);
            startQuestionnaire = true;
            _isHolding = false; 
        }
    }

    public void OnDisable()
    {
        startQuestionnaire = false;
        _isHolding = false;
        ResetQuestionnaire();
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
                    if (_currentOption == 0)
                    {
                        Debug.Log("Answer: YES");
                        YesResponse();
                    }
                    else if (_currentOption == 1)
                    {
                        Debug.Log("Answer: NO");
                        NoResponse();
                    }
                    //Show instructions only for the first X questions
                    if (_currentQuestion > (int)(questions.Count / 5.0f))
                    {
                        HideInstructions();
                    }
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
        ShowInstructions();
    }

    private void ResetSelection()
    {
        _selectionProgress = 0.0f;
        _holdTimer = 0.0f;
        DeselectButton(_currentOption);
    }

    private void ShowInstructions()
    {
        instructionsGO.SetActive(true);
    }

    public void HideInstructions()
    {
        instructionsGO.SetActive(false);   
    }
    
    private void FinishQuestionnaire()
    {
        gameObject.SetActive(false);
    }

    private void YesResponse()
    {
        //All continues the same
    }

    private void NoResponse()
    {
        //Touch Loop finishes
        loop.FinishLoop();
    }
}
