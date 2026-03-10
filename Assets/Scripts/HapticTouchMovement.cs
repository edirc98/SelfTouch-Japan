using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HapticTouchMovement : Haptic
{
    protected override void StylusButtonEvent(StylusButtonEventType e)
    {
        Debug.Log("StylusButtonEvent");
    }
}
