using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class SDTCountDown : QuickStageBase 
{
    public GameObject TimerText;
    public CountDownTimer Timer;
    public MoveCharacter MoveCharacter; 
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        TimerText.SetActive(true);
        Timer.time = Timer.InitTime;
        Timer.timerOn = true;
        return base.CoUpdate();
    }

    public override void Finish()
    {
        Timer.timerOn = false;
        TimerText.SetActive(false);
        MoveCharacter.MoveCharacterForward();
        base.Finish();
    }
}
