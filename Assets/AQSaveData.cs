using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class AQSaveData : QuickStageBase
{

    public AnthropomorphismQuestionaire AntropoQuestion;
    public LikertScaleController ScaleController; 
    public AQShowAvatar ShowAvatar; 
    [HideInInspector]
    public Monitoring DataMonitoring;

    protected override void Start()
    {
        DataMonitoring = QuickSingletonManager.GetInstance<Monitoring>();
        //DataMonitoring.SaveData(MonitoringCaption);

        base.Start();
    }

    // Update is called once per frame
    protected override IEnumerator CoUpdate()
    {
        DataMonitoring.SaveAqData(ShowAvatar.currentAvatar.ToString(), AntropoQuestion.currentQuestion, ScaleController.lastSelectedOption); 

        return base.CoUpdate();
    }
}
