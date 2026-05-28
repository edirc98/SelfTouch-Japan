using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class SageAvatarTouch : QuickStageBase
{
    [SerializeField] private HapticTouchMovement hapticMovement;
    
    protected override IEnumerator CoUpdate()
    {
        hapticMovement.Follow = true;
        return base.CoUpdate();
    }

    public override void Finish()
    {
        hapticMovement.Follow = false;
        base.Finish();
    }
}
