using System.Collections;
using System.Collections.Generic;
using QuickVR;
using TMPro;

[System.Serializable]
public struct EmboQuestion
{
    public string Question;
    public string PosClue;
    public string NegClue;
}
public class EmbodimentQuestionaire : QuickStageBase
{
    public List<EmboQuestion> EmbodimentQuestions;
    public TMP_Text QuestionStartText;
    public TMP_Text QuestionText;
    public TMP_Text NegativeClueText;
    public TMP_Text PositiveClueText;
    public QuickStageLoop QuestionsLoop;
    public EmboQuestion currentQuestion; 
    
    public bool canConfirm = false;
    private bool canRespond = false; 
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        QuestionStartText.text = "Respond the following question:";
        canRespond = true;
        if (QuestionsLoop.GetCurrentInteration() == QuestionsLoop._numIterations)
        {
            QuestionsLoop.ResetCurrentIteration(); 
        }
        currentQuestion = EmbodimentQuestions[QuestionsLoop.GetCurrentInteration()];
        QuestionText.text = currentQuestion.Question; 
        NegativeClueText.text = currentQuestion.NegClue;
        PositiveClueText.text = currentQuestion.PosClue;
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
