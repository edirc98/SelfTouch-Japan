using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class QuickStageSelectAvatarGender : QuickStageBase
{
    [Header("Avatar Source Animator")]
    public Animator MasterAvatarAnimator;
    [Header("Target Avatars Animators")]
    public Animator HumanAvatar;
    public Animator RobotAvatar;
    public Animator TreeentAvatar;

    [Header("Loop & Conditions")]
    public QuickStageLoop MainLoop;
    public StageSetConditionsOrder Conditions; 
    private QuickVRManager _vrManager
    {
        get
        {
            return QuickSingletonManager.GetInstance<QuickVRManager>();
        }
    }
    protected override IEnumerator CoUpdate()
    {
        int currentIteration = MainLoop.GetCurrentInteration();
        if (Conditions.CurrentConditions[currentIteration].AvatarBodyType == Condition.BodyType.Human)
        {
            _vrManager.SetAnimatorTarget(HumanAvatar);
        }
        else if (Conditions.CurrentConditions[currentIteration].AvatarBodyType == Condition.BodyType.Treent)
        {
            _vrManager.SetAnimatorTarget(TreeentAvatar);

        }
        else if (Conditions.CurrentConditions[currentIteration].AvatarBodyType == Condition.BodyType.Robot)
        {
            _vrManager.SetAnimatorTarget(RobotAvatar);

        }
        return base.CoUpdate();
    }
}
