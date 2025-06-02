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
        AQ_Scale.SetActive(false);
        AQ_Avatar.currentAvatar.gameObject.SetActive(false);
        return base.CoUpdate();
    }
}
