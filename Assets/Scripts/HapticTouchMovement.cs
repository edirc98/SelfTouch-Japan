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

    private Vector3 CalibratedPos;
    private Vector3 calibrationVector; 
    
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
        CalibrationOffset = Vector3.zero;
        tip    = ChildFinder.FindChild(avatarRoot, TipName);
        target = ChildFinder.FindChild(avatarRoot, TargetName);

        if (tip == null || target == null)
        {
            Debug.LogError("[AvatarCalibrator] Calibration aborted: one or both transforms not found.");
            IsCalibrated = false;
        }
        else
        {
            
            Vector3 mainOffset = target.position - cursor.transform.position;
            IsCalibrated      = true;

            Vector3 tipOffset = objectFollower.transform.position - tip.position;
            
            
            CalibrationOffset = mainOffset + tipOffset;
            CalibratedPos = cursor.transform.position + CalibrationOffset;
            
        }
        

        Debug.Log($"[AvatarCalibrator] Calibrated. Offset: {CalibrationOffset}");
    }

    private void OnDrawGizmos()
    {
        if (IsCalibrated)
        {
            Gizmos.color = Color.magenta;
            Gizmos.DrawLine(tip.position, target.position);

            Gizmos.color = Color.yellow; 
            Gizmos.DrawWireSphere(cursor.transform.position, 0.05f);
            
            Gizmos.DrawLine(cursor.transform.position, CalibratedPos);
            Gizmos.color = Color.red;
            Gizmos.DrawWireSphere(CalibratedPos, 0.05f);
        }
        
    }
}

