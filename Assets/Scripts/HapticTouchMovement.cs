using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HapticTouchMovement : Haptic
{
    
    private const string TipName    = "Tip";
    private const string TargetName = "Target";
    
    public GameObject objectFollower;
    public Vector3 CalibrationOffset { get; private set; }
    public bool    IsCalibrated      { get; private set; }
    
    [SerializeField] private QuickStageSelectEmbodiedAvatar avatar;


    [SerializeField] private Transform tip;
    [SerializeField] private Transform target;
    
    public bool Follow {set;get;}

    
    protected override void StylusButtonEvent(StylusButtonEventType e)
    {
        if (e == StylusButtonEventType.Grey_Down)
        {
            Debug.Log("Calibration:");
       
            
            Calibrate(avatar.CurrentAvatarTransform);
        }
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
        objectFollower.transform.position = cursor.transform.position + CalibrationOffset;
    }

    public void Calibrate(Transform avatarRoot)
    {
        //CalibrationOffset = Vector3.zero;
        tip    = ChildFinder.FindChild(avatarRoot, TipName);
        target = ChildFinder.FindChild(avatarRoot, TargetName);

        if (tip == null || target == null)
        {
            Debug.LogError("[AvatarCalibrator] Calibration aborted: one or both transforms not found.");
            IsCalibrated = false;
        }
        else
        {
            CalibrationOffset = target.TransformPoint(target.localPosition) - tip.TransformPoint(tip.localPosition);
            IsCalibrated      = true; 
        }
        

        Debug.Log($"[AvatarCalibrator] Calibrated. Offset: {CalibrationOffset}");
    }

    private void OnDrawGizmos()
    {
        if (IsCalibrated)
        {
            Gizmos.color = Color.magenta;
            Gizmos.DrawLine(tip.position, target.position);
        }
        
    }
}
