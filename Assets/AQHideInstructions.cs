using System.Collections;
using System.Collections.Generic;
using QuickVR;
using UnityEngine;

public class AQHideInstructions : QuickStageBase
{
    public GameObject AQ_Scale;
    public AQShowAvatar AQ_Avatar;

    protected override IEnumerator CoUpdate()
    {
        if(AQ_Scale) AQ_Scale.SetActive(false);
        if(AQ_Avatar) AQ_Avatar.currentAvatar.gameObject.SetActive(false);
        return base.CoUpdate();
    }
}
