using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class DisableInstructions : QuickStageBase
{
    public GameObject SDT_Intructions;
    protected override IEnumerator CoUpdate()
    {
        SDT_Intructions.SetActive(false);
        return base.CoUpdate();
    }
}
