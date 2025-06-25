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
}
