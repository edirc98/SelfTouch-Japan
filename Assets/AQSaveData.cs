using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class AQSaveData : QuickStageBase
{
    public AnthropomorphismQuestionaire AntropoQuestion;
    public LikertScaleController ScaleController; 
    public AQShowAvatar ShowAvatar; 
    public AQShowAvatarCondition ShowAvatarCondition;
    public bool isCondition; 
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
        if (isCondition)
        {
            DataMonitoring.SaveAqData(ShowAvatarCondition.currentAvatar.ToString(), AntropoQuestion.currentQuestion, ScaleController.lastSelectedOption); 

        }
        else
        {
            DataMonitoring.SaveAqData(ShowAvatar.currentAvatar.ToString(), AntropoQuestion.currentQuestion, ScaleController.lastSelectedOption); 
        }

        return base.CoUpdate();
    }
}
