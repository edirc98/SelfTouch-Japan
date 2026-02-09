using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class AQShowAvatarCondition : QuickStageBase
{
    public List<GameObject> AQAvatars;
    public GameObject currentAvatar; 
    public StageSetConditionsOrder ConditionsOrder;
    public QuickStageLoop MainLoop;

    protected override IEnumerator CoUpdate()
    {
        switch (ConditionsOrder.currentConditions[MainLoop.GetCurrentInteration()].avatarBodyType)
        {
            case Condition.BodyType.Human:
                currentAvatar = AQAvatars[0];
                break;
            case Condition.BodyType.Treent:
                currentAvatar = AQAvatars[2];
                break;
            case Condition.BodyType.Robot:
                currentAvatar = AQAvatars[3];
                break;
            default:
                break;
        }
        currentAvatar.gameObject.SetActive(true);
        return base.CoUpdate();
    }
}
