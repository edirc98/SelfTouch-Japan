using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class ChangeAvatarArmsDriveMode : QuickStageBase
{
    public enum ArmDriveMode
    {
        TRACKING,
        IK
    }
    
    
    public QuickUnityVR MasterAvatarController;
    public ArmDriveMode armDriveMode;
    
    
    protected override void Start()
    {
        MasterAvatarController.SetIKControl(IKBone.LeftHand, QuickUnityVR.ControlType.IK);
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        switch (armDriveMode)
        {
            case ArmDriveMode.TRACKING:
                MasterAvatarController.SetIKControl(IKBone.LeftHand, QuickUnityVR.ControlType.Tracking);
                MasterAvatarController.SetIKControl(IKBone.RightHand, QuickUnityVR.ControlType.Tracking);
                SetRightHandToIK(); //Set the hand in IK pos when hand is in Tracking
                break;
            case ArmDriveMode.IK:
                MasterAvatarController.SetIKControl(IKBone.LeftHand, QuickUnityVR.ControlType.IK);
                MasterAvatarController.SetIKControl(IKBone.RightHand, QuickUnityVR.ControlType.IK);
                break;
            default:
                MasterAvatarController.SetIKControl(IKBone.LeftHand, QuickUnityVR.ControlType.Tracking);
                MasterAvatarController.SetIKControl(IKBone.RightHand, QuickUnityVR.ControlType.Tracking);
                break;
        }
        return base.CoUpdate();
    }

    private void SetRightHandToIK()
    {
        MasterAvatarController.SetIKControl(IKBone.RightThumbDistal, QuickUnityVR.ControlType.IK);
        MasterAvatarController.SetIKControl(IKBone.RightIndexDistal, QuickUnityVR.ControlType.IK);
        MasterAvatarController.SetIKControl(IKBone.RightRingDistal, QuickUnityVR.ControlType.IK);
        MasterAvatarController.SetIKControl(IKBone.RightMiddleDistal, QuickUnityVR.ControlType.IK);
        MasterAvatarController.SetIKControl(IKBone.RightLittleDistal, QuickUnityVR.ControlType.IK);
    }
}
