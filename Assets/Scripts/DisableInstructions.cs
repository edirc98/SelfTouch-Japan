using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class DisableInstructions : QuickStageBase
{
    public GameObject SDT_Intructions;
    public GameObject WalkButton;
    public GameObject ConfirmButton;
    protected override IEnumerator CoUpdate()
    {
        WalkButton.SetActive(false);
        ConfirmButton.SetActive(false);
        SDT_Intructions.SetActive(false);
        return base.CoUpdate();
    }
}
