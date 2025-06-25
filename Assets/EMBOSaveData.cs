using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class EMBOSaveData : QuickStageBase
{
    public EmbodimentQuestionaire embodimentQuestion ;
    public LikertScaleController ScaleController; 
    [HideInInspector]
    public Monitoring DataMonitoring;

    protected override void Start()
    {
        DataMonitoring = QuickSingletonManager.GetInstance<Monitoring>();
        base.Start();
    }

    // Update is called once per frame
    protected override IEnumerator CoUpdate()
    {
        DataMonitoring.SaveQuestionData(embodimentQuestion.currentQuestion.Question, ScaleController.lastSelectedOption); 

        return base.CoUpdate();
    }
}
