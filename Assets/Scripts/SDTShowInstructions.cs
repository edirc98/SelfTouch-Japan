using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class SDTShowInstructions : QuickStageBase
{
    public GameObject SDT_Instructions;
    public GameObject WalkButton;
    public GameObject ConfirmButton; 

    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        WalkButton.SetActive(true);
        ConfirmButton.SetActive(true);
        SDT_Instructions.SetActive(true);
        return base.CoUpdate();
    }

    public override void Finish()
    {
        SDT_Instructions.SetActive(false);
        base.Finish();
    }
}
