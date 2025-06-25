using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class StageDisembodieAvatar : QuickStageBase
{
    private QuickVRManager _vrManager
    {
        get
        {
            return QuickSingletonManager.GetInstance<QuickVRManager>();
        }
    }

    protected override IEnumerator CoUpdate()
    {
        _vrManager.GetAnimatorTarget().gameObject.SetActive(false);
        return base.CoUpdate();
    }
}
