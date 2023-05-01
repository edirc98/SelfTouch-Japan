using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class QuickStageVisuolotactileBalls : QuickStageBase
{
    public GameObject BallsSpawner; 
    protected override IEnumerator CoUpdate()
    {
        BallsSpawner.SetActive(true);
        return base.CoUpdate();
    }
    public override void Finish()
    {
        BallsSpawner.SetActive(false);
        base.Finish();
    }
}
