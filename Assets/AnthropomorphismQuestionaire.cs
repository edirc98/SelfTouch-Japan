using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;
using UnityEngine.UI;
using TMPro; 

public class AnthropomorphismQuestionaire : QuickStageBase
{
    public List<string> AnthropomorphismQuestions;
    public TMP_Text QuestionText;
    public QuickStageLoop QuestionsLoop;
    // Start is called before the first frame update
    public bool canConfirm = false;
    private bool canRespond = false; 
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        canRespond = true; 
        if (QuestionsLoop.GetCurrentInteration() == QuestionsLoop._numIterations) QuestionsLoop.ResetCurrentIteration(); 
        QuestionText.text = AnthropomorphismQuestions[QuestionsLoop.GetCurrentInteration()];
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