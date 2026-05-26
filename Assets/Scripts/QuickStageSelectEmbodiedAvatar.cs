using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

[System.Serializable]
public struct AvatarOffsets{
    public string avatarName; 
    public Vector3 leftHandPosOffset;
    public Vector3 leftHandRotOffset;
    public Vector3 rightHandPosOffset;
    public Vector3 rightHandRotOffset; 
}
public class QuickStageSelectEmbodiedAvatar: QuickStageBase
{
    [Header("Avatar Source Animator")]
    public Animator masterAvatarAnimator;
    public Transform masterAvatarLeftIKTarget; 
    public Transform masterAvatarRightIKTarget;
    public HapticTouchMovement rightHandFollower;

    public Transform CurrentAvatarTransform; 

    [Header("Target Avatars Animators")]
    public Animator humanMaleAvatar;
    public Animator humanFemaleAvatar;
    public Animator robotAvatar;
    public Animator treeentAvatar;

    [Header("Loop & Conditions")]
    public QuickStageLoop mainLoop;
    public QuickStageLoop touchloop;
    public StageSetConditionsOrder conditions;
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
        int currentIteration = mainLoop.GetCurrentInteration();
        Debug.Log("Current iter: " + currentIteration); 
        switch (conditions.currentConditions[currentIteration].avatarBodyType)
        { 
            case Condition.BodyType.Human:
                if (SettingsBase.GetGender() == SettingsBase.Genders.Male)
                {
                    SetLeftHandOffset(avatarOffsets[0].leftHandPosOffset, avatarOffsets[0].leftHandRotOffset); 
                    //SetRightHandOffset(avatarOffsets[0].rightHandPosOffset); 

                    humanMaleAvatar.gameObject.SetActive(true);
                    CurrentAvatarTransform = humanMaleAvatar.transform;
                    _vrManager.SetAnimatorTarget(humanMaleAvatar);
                    robotAvatar.gameObject.SetActive(false);
                    treeentAvatar.gameObject.SetActive(false);
                    humanFemaleAvatar.gameObject.SetActive(false);
                    
                    _dataMonitoring.SaveLogData("Iter: " + currentIteration + " Embodied: " + nameof(Condition.BodyType.Human) + " Male");
                    
                    break;
                }
                else
                {
                    SetLeftHandOffset(avatarOffsets[1].leftHandPosOffset, avatarOffsets[1].leftHandRotOffset);
                    //SetRightHandOffset(avatarOffsets[1].rightHandPosOffset);

                    humanFemaleAvatar.gameObject.SetActive(true);
                    CurrentAvatarTransform = humanFemaleAvatar.transform;
                    _vrManager.SetAnimatorTarget(humanFemaleAvatar);
                    robotAvatar.gameObject.SetActive(false);
                    treeentAvatar.gameObject.SetActive(false);
                    humanMaleAvatar.gameObject.SetActive(false);
                    
                    _dataMonitoring.SaveLogData("Iter: " + currentIteration + " Embodied: " + nameof(Condition.BodyType.Human) + " Female");

                    
                    break;
                }
            case Condition.BodyType.Robot:
                SetLeftHandOffset(avatarOffsets[2].leftHandPosOffset, avatarOffsets[2].leftHandRotOffset);
                //SetRightHandOffset(avatarOffsets[2].rightHandPosOffset);

                robotAvatar.gameObject.SetActive(true);
                CurrentAvatarTransform = robotAvatar.transform;
                _vrManager.SetAnimatorTarget(robotAvatar);
                humanMaleAvatar.gameObject.SetActive(false);
                humanFemaleAvatar.gameObject.SetActive(false);
                treeentAvatar.gameObject.SetActive(false);
                
                _dataMonitoring.SaveLogData("Iter: " + currentIteration + " Embodied: " + nameof(Condition.BodyType.Human));

                
                break;
            case Condition.BodyType.Treent:
                SetLeftHandOffset(avatarOffsets[3].leftHandPosOffset, avatarOffsets[3].leftHandRotOffset);
                //SetRightHandOffset(avatarOffsets[3].rightHandPosOffset);

                treeentAvatar.gameObject.SetActive(true);
                CurrentAvatarTransform = treeentAvatar.transform;
                _vrManager.SetAnimatorTarget(treeentAvatar);
                humanMaleAvatar.gameObject.SetActive(false);
                humanFemaleAvatar.gameObject.SetActive(false);
                robotAvatar.gameObject.SetActive(false);
                
                _dataMonitoring.SaveLogData("Iter: " + currentIteration + " Embodied: " + nameof(Condition.BodyType.Human));
                
                break;
        }
        touchloop.ResetCurrentIteration();
        return base.CoUpdate();
    }


    private void SetLeftHandOffset(Vector3 posOffset, Vector3 rotOffset)
    {
        masterAvatarLeftIKTarget.transform.position = posOffset;
        masterAvatarLeftIKTarget.transform.eulerAngles = rotOffset; 
    }

    private void SetRightHandOffset(Vector3 posOffset)
    {
        //rightHandFollower.SetPositionOffset(posOffset);
    }
}
