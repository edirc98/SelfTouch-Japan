using System.Collections;
using System.Collections.Generic;
using QuickVR;
using UnityEngine;

public class StageAvatarTouch : QuickStageBase
{
    public GameObject TouchInstructions;
    private float envodimentTime = 0.0f; 
    protected override IEnumerator CoUpdate()
    {
        TouchInstructions.SetActive(true);
        //Save time at the start of the stage
        
        //Hide instructions when pedal is pressed and save time
        if (Input.GetKeyDown(KeyCode.K))
        {
            TouchInstructions.SetActive(false);
            //SaveTime //TODO
        }
        return base.CoUpdate();
    }

    public override void Finish()
    {
        //Save no time and hide instructions if pedal has not been pressed during the stage
        TouchInstructions.SetActive(false);
        base.Finish();
    }
}
