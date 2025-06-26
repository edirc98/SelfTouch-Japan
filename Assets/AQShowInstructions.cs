using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 
using TMPro; 

public class AQShowInstructions : QuickStageBase
{
    public GameObject AQ_Scale;
    public TMP_Text QuestionText;
    public GameObject AQ_Instructions;
    
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        
        
        if (AQ_Scale)AQ_Scale.SetActive(true);
        if (QuestionText) QuestionText.text = "";
        if (AQ_Instructions)AQ_Instructions.SetActive(true);
        return base.CoUpdate();
    }

    public override void Finish()
    {
        if(AQ_Instructions) AQ_Instructions.SetActive(false);
        base.Finish();
    }
}
