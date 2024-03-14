using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class QuickStageSelectEmbodiedAvatar: QuickStageBase
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
            HumanAvatar.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(HumanAvatar);
            RobotAvatar.gameObject.SetActive(false);
            TreeentAvatar.gameObject.SetActive(false);

        }
        else if (Conditions.CurrentConditions[currentIteration].AvatarBodyType == Condition.BodyType.Treent)
        {
            TreeentAvatar.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(TreeentAvatar);
            HumanAvatar.gameObject.SetActive(false);
            RobotAvatar.gameObject.SetActive(false);

        }
        else if (Conditions.CurrentConditions[currentIteration].AvatarBodyType == Condition.BodyType.Robot)
        {
            RobotAvatar.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(RobotAvatar);
            HumanAvatar.gameObject.SetActive(false);
            TreeentAvatar.gameObject.SetActive(false);



        }
        return base.CoUpdate();
    }
}
