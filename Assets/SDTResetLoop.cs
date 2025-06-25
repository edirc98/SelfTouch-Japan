using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class SDTResetLoop : QuickStageBase
{
    public QuickStageLoop SDTLoop;
    protected override IEnumerator CoUpdate()
    {
        SDTLoop.ResetCurrentIteration();
        return base.CoUpdate();
    }
}
