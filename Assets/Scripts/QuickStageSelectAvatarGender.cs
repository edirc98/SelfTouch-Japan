using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class QuickStageSelectAvatarGender : QuickStageBase
{
    [Header("Avatar Source Animator")]
    public Animator masterAvatarAnimator;
    [Header("Target Avatars Animators")]
    public Animator maleAvatarAnimator;
    public Animator femaleAvatarAnimator;


    private QuickVRManager _vrManager
    {
        get
        {
            return QuickSingletonManager.GetInstance<QuickVRManager>();
        }
    }
    protected override IEnumerator CoUpdate()
    {
        if (SettingsBase.GetGender() == SettingsBase.Genders.Male)
        {
            if (maleAvatarAnimator.gameObject.activeSelf == false) maleAvatarAnimator.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(maleAvatarAnimator);
            femaleAvatarAnimator.gameObject.SetActive(false);
            Debug.Log("MALE selected as Target Avatar");
        }
        else
        {
            if (femaleAvatarAnimator.gameObject.activeSelf == false) femaleAvatarAnimator.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(femaleAvatarAnimator);
            maleAvatarAnimator.gameObject.SetActive(false);
            Debug.Log("FEMALE selected as Target Avatar");
        }

        return base.CoUpdate();
    }
}
