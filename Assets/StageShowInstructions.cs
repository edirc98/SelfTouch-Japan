using System.Collections;
using System.Collections.Generic;
using QuickVR;
using UnityEngine;

public class StageShowInstructions : QuickStageBase
{
    public GameObject instructionsGO;
    protected override IEnumerator CoUpdate()
    {
        instructionsGO.SetActive(true);
        return base.CoUpdate();
    }

    public override void Finish()
    {
        instructionsGO.SetActive(false);
        base.Finish();
    }
}
