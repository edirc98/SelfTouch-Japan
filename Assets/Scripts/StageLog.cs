using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class StageLog : QuickStageBase
{
    public string Log; 
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
        DataMonitoring.SaveLogData(Log);

        return base.CoUpdate();
    }
}
