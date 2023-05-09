using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class QuickStageVisuolotactileBalls : QuickStageBase
{
    public GameObject EmbodimentInstructions;
    public GameObject BallsSpawner; 
    protected override IEnumerator CoUpdate()
    {
        EmbodimentInstructions.SetActive(true);
        BallsSpawner.SetActive(true);
        return base.CoUpdate();
    }
    public override void Finish()
    {
        EmbodimentInstructions.SetActive(false);
        BallsSpawner.SetActive(false);
        base.Finish();
    }
}
