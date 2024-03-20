using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

[System.Serializable]
public struct AvatarOffsets{
    public string AvatarName; 
    public Vector3 LeftHandPosOffset;
    public Vector3 LefthHandRotOffset;
    public Vector3 RightHandPosOffset;
    public Vector3 RightHandRotOffset; 
}
public class QuickStageSelectEmbodiedAvatar: QuickStageBase
{
    [Header("Avatar Source Animator")]
    public Animator MasterAvatarAnimator;
    public Transform MasterAvatarLeftIKTarget; 
    public Transform MasterAvatarRightIKTarget;
    public ObjectFollow RightHandFollower; 

    [Header("Target Avatars Animators")]
    public Animator HumanAvatar;
    public Animator RobotAvatar;
    public Animator TreeentAvatar;

    [Header("Loop & Conditions")]
    public QuickStageLoop MainLoop;
    public StageSetConditionsOrder Conditions;
    [Header("Avatar Offsets")]
    public List<AvatarOffsets> avatarOffsets; 
    private QuickVRManager _vrManager
    {
        get
        {
            return QuickSingletonManager.GetInstance<QuickVRManager>();
        }
    }
    protected override void Start()
    {
        //Conditions = QuickSingletonManager.GetInstance<StageSetConditionsOrder>(); 
        base.Start();
    }
    protected override IEnumerator CoUpdate()
    {
        int currentIteration = MainLoop.GetCurrentInteration();
        Debug.Log("Current iter: " + currentIteration); 
        if (Conditions.CurrentConditions[currentIteration].AvatarBodyType == Condition.BodyType.Human)
        {
            SetLeftHandOffset(avatarOffsets[0].LeftHandPosOffset, avatarOffsets[0].LefthHandRotOffset); 
            SetRightHandOffset(avatarOffsets[0].RightHandPosOffset, avatarOffsets[0].RightHandRotOffset); 

            HumanAvatar.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(HumanAvatar);
            RobotAvatar.gameObject.SetActive(false);
            TreeentAvatar.gameObject.SetActive(false);

        }
        else if (Conditions.CurrentConditions[currentIteration].AvatarBodyType == Condition.BodyType.Treent)
        {
            SetLeftHandOffset(avatarOffsets[2].LeftHandPosOffset, avatarOffsets[0].LefthHandRotOffset);
            SetRightHandOffset(avatarOffsets[2].RightHandPosOffset, avatarOffsets[0].RightHandRotOffset);

            TreeentAvatar.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(TreeentAvatar);
            HumanAvatar.gameObject.SetActive(false);
            RobotAvatar.gameObject.SetActive(false);

        }
        else if (Conditions.CurrentConditions[currentIteration].AvatarBodyType == Condition.BodyType.Robot)
        {
            SetLeftHandOffset(avatarOffsets[1].LeftHandPosOffset, avatarOffsets[0].LefthHandRotOffset);
            SetRightHandOffset(avatarOffsets[1].RightHandPosOffset, avatarOffsets[0].RightHandRotOffset);

            RobotAvatar.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(RobotAvatar);
            HumanAvatar.gameObject.SetActive(false);
            TreeentAvatar.gameObject.SetActive(false);
        }
        return base.CoUpdate();
    }


    private void SetLeftHandOffset(Vector3 PosOffset, Vector3 RotOffset)
    {
        MasterAvatarLeftIKTarget.transform.position = PosOffset;
        MasterAvatarLeftIKTarget.transform.eulerAngles = RotOffset; 
    }

    private void SetRightHandOffset(Vector3 PosOffset, Vector3 RotOffset)
    {
        RightHandFollower.SetPositionOffset(PosOffset);
        RightHandFollower.SetRotationOffset(RotOffset); 
    }
}
