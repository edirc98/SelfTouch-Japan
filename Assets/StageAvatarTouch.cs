using System.Collections;
using System.Collections.Generic;
using QuickVR;
using UnityEngine;
using UnityEngine.Serialization;

public class StageAvatarTouch : QuickStageBase
{
    public GameObject EmbodimentWhenInstructions;
    public GameObject TouchInstructions2;
    private float embodimentTime = 0.0f; 
    
    private EmbodimentTimeDetector embodimentTimeDetector;

    protected override void Awake()
    {
        embodimentTimeDetector = GetComponent<EmbodimentTimeDetector>(); 
        base.Awake();
    }

    protected override IEnumerator CoUpdate()
    {
        embodimentTimeDetector.canDetectPedal = true; 
        EmbodimentWhenInstructions.SetActive(true);
        TouchInstructions2.SetActive(true);
        return base.CoUpdate();
    }

    public override void Finish()
    {
        //Save no time and hide instructions if pedal has not been pressed during the stage
        embodimentTimeDetector.canDetectPedal = false; 
        EmbodimentWhenInstructions.SetActive(false);
        TouchInstructions2.SetActive(false);

        base.Finish();
    }
}
