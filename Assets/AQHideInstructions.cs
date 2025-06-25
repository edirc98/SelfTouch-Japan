using System.Collections;
using System.Collections.Generic;
using QuickVR;
using UnityEngine;

public class AQHideInstructions : QuickStageBase
{
    public GameObject AQ_Scale;
    public AQShowAvatar AQ_Avatar;
    public AQShowAvatarCondition AQ_AvatarCondition;

    protected override IEnumerator CoUpdate()
    {
        if(AQ_Scale) AQ_Scale.SetActive(false);
        if(AQ_Avatar) AQ_Avatar.currentAvatar.gameObject.SetActive(false);
        if(AQ_AvatarCondition) AQ_AvatarCondition.currentAvatar.gameObject.SetActive(false);
        return base.CoUpdate();
    }
}
