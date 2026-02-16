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
    //public ObjectFollow RightHandFollower; 

    [Header("Target Avatars Animators")]
    public Animator HumanMaleAvatar;
    public Animator HumanFemaleAvatar;
    public Animator RobotAvatar;
    public Animator TreeentAvatar;

    [Header("Loop & Conditions")]
    public QuickStageLoop MainLoop;
    public StageSetConditionsOrder Conditions;
    [Header("Avatar Offsets")]
    public List<AvatarOffsets> avatarOffsets; 
    
    private Monitoring _dataMonitoring;
    private QuickVRManager _vrManager
    {
        get
        {
            return QuickSingletonManager.GetInstance<QuickVRManager>();
        }
    }

    protected override void Start()
    {
        _dataMonitoring = QuickSingletonManager.GetInstance<Monitoring>();
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        int currentIteration = MainLoop.GetCurrentInteration();
        Debug.Log("Current iter: " + currentIteration); 
        switch (Conditions.currentConditions[currentIteration].avatarBodyType)
        { 
            case Condition.BodyType.Human:
                if (SettingsBase.GetGender() == SettingsBase.Genders.Male)
                {
                    SetLeftHandOffset(avatarOffsets[0].LeftHandPosOffset, avatarOffsets[0].LefthHandRotOffset); 
                    SetRightHandOffset(avatarOffsets[0].RightHandPosOffset, avatarOffsets[0].RightHandRotOffset); 

                    HumanMaleAvatar.gameObject.SetActive(true);
                    _vrManager.SetAnimatorTarget(HumanMaleAvatar);
                    RobotAvatar.gameObject.SetActive(false);
                    TreeentAvatar.gameObject.SetActive(false);
                    HumanFemaleAvatar.gameObject.SetActive(false);
                    
                    _dataMonitoring.SaveLogData("Iter: " + currentIteration + " Embodied: " + nameof(Condition.BodyType.Human) + " Male");
                    
                    break;
                }
                else
                {
                    SetLeftHandOffset(avatarOffsets[1].LeftHandPosOffset, avatarOffsets[1].LefthHandRotOffset);
                    SetRightHandOffset(avatarOffsets[1].RightHandPosOffset, avatarOffsets[1].RightHandRotOffset);

                    HumanFemaleAvatar.gameObject.SetActive(true);
                    _vrManager.SetAnimatorTarget(HumanFemaleAvatar);
                    RobotAvatar.gameObject.SetActive(false);
                    TreeentAvatar.gameObject.SetActive(false);
                    HumanMaleAvatar.gameObject.SetActive(false);
                    
                    _dataMonitoring.SaveLogData("Iter: " + currentIteration + " Embodied: " + nameof(Condition.BodyType.Human) + " Female");

                    
                    break;
                }
            case Condition.BodyType.Robot:
                SetLeftHandOffset(avatarOffsets[2].LeftHandPosOffset, avatarOffsets[2].LefthHandRotOffset);
                SetRightHandOffset(avatarOffsets[2].RightHandPosOffset, avatarOffsets[2].RightHandRotOffset);

                RobotAvatar.gameObject.SetActive(true);
                _vrManager.SetAnimatorTarget(RobotAvatar);
                HumanMaleAvatar.gameObject.SetActive(false);
                HumanFemaleAvatar.gameObject.SetActive(false);
                TreeentAvatar.gameObject.SetActive(false);
                
                _dataMonitoring.SaveLogData("Iter: " + currentIteration + " Embodied: " + nameof(Condition.BodyType.Human));

                
                break;
            case Condition.BodyType.Treent:
                SetLeftHandOffset(avatarOffsets[3].LeftHandPosOffset, avatarOffsets[3].LefthHandRotOffset);
                SetRightHandOffset(avatarOffsets[3].RightHandPosOffset, avatarOffsets[3].RightHandRotOffset);

                TreeentAvatar.gameObject.SetActive(true);
                _vrManager.SetAnimatorTarget(TreeentAvatar);
                HumanMaleAvatar.gameObject.SetActive(false);
                HumanFemaleAvatar.gameObject.SetActive(false);
                RobotAvatar.gameObject.SetActive(false);
                
                _dataMonitoring.SaveLogData("Iter: " + currentIteration + " Embodied: " + nameof(Condition.BodyType.Human));
                
                break;
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
        //RightHandFollower.SetPositionOffset(PosOffset);
        //RightHandFollower.SetRotationOffset(RotOffset); 
    }
}
