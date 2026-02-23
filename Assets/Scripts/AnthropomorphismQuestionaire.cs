using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;
using UnityEngine.UI;
using TMPro; 

public class AnthropomorphismQuestionaire : QuickStageBase
{
    public List<string> AnthropomorphismQuestions;
    public TMP_Text QuestionStartText;
    public TMP_Text QuestionText;
    public TMP_Text PositiveClueText;
    public TMP_Text NegativeClueText;
    public QuickStageLoop QuestionsLoop;
    public string currentQuestion; 
    
    public bool canConfirm = false;
    private bool canRespond = false; 
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        QuestionStartText.text = "You see this character...";
        PositiveClueText.text = "Very much so";
        NegativeClueText.text = "Not at all";
        canRespond = true;
        if (QuestionsLoop.GetCurrentInteration() == QuestionsLoop._numIterations)
        {
            QuestionsLoop.ResetCurrentIteration(); 
        }
        currentQuestion = AnthropomorphismQuestions[QuestionsLoop.GetCurrentInteration()];
        QuestionText.text = currentQuestion; 
        return base.CoUpdate(); 
    }

    public void Confirm()
    {
        if (canConfirm && canRespond)
        {
            base.Finish();
            canConfirm = false;
            canRespond = false; 
        }  
    }
}
