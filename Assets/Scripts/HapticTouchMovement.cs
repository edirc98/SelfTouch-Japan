using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HapticTouchMovement : Haptic
{
    
    public Transform objectFollower;
    public Vector3 positionOffset;
    
    public bool Follow {set;get;}

    
    protected override void StylusButtonEvent(StylusButtonEventType e)
    {
        Debug.Log("StylusButtonEvent");
    }

    private void Awake()
    {
        Follow = true;
    }
    
    private void FixedUpdate()
    {
        if (Follow)
        {
            FollowCursor();
        }
    }

    private void FollowCursor()
    {
        objectFollower.position = cursor.transform.position + positionOffset; 
    }

    public void SetPositionOffset(Vector3 newPositionOffset)
    {
        positionOffset = newPositionOffset;
    }
}
