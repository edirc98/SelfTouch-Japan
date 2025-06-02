using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class AQShowInstructions : QuickStageBase
{
    public GameObject AQ_Scale;
    public GameObject AQ_Instructions;
    
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        AQ_Scale.SetActive(true);
        AQ_Instructions.SetActive(true);
        return base.CoUpdate();
    }

    public override void Finish()
    {
        AQ_Instructions.SetActive(false);
        base.Finish();
    }
}
