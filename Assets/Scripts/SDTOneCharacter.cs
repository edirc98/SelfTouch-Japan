using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class SDTOneCharacter : QuickStageBase
{
    public GameObject TreentAvatar;
    public GameObject RobotAvatar;
    public GameObject MaleAvatar;

    public GameObject SelectedCharacter;
    public Animator SelectedCharacterAnimator;

    public QuickStageLoop MainLoop;
    public StageSetConditionsOrder Conditions;

    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {

        switch (Conditions.CurrentConditions[MainLoop.GetCurrentInteration()].AvatarBodyType)
        {
            case Condition.BodyType.Human:
                SelectedCharacter = MaleAvatar;
                break;
            case Condition.BodyType.Treent:
                SelectedCharacter = TreentAvatar;
                break;
            case Condition.BodyType.Robot:
                SelectedCharacter = RobotAvatar;
                break;
            default:
                break;
        }
        SelectedCharacterAnimator = SelectedCharacter.GetComponent<Animator>();
        SelectedCharacter.SetActive(true);
        return base.CoUpdate();
    }
}
