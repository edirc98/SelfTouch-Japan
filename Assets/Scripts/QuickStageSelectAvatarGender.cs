using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class QuickStageSelectAvatarGender : QuickStageBase
{
    [Header("Avatar Source Animator")]
    public Animator MasterAvatarAnimator;
    [Header("Target Avatars Animators")]
    public Animator MaleAvatarAnimator;
    public Animator FemaleAvatarAnimator;


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
            if (MaleAvatarAnimator.gameObject.activeSelf == false) MaleAvatarAnimator.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(MaleAvatarAnimator);
            FemaleAvatarAnimator.gameObject.SetActive(false);
            Debug.Log("MALE selected as Target Avatar");
        }
        else
        {
            if (FemaleAvatarAnimator.gameObject.activeSelf == false) FemaleAvatarAnimator.gameObject.SetActive(true);
            _vrManager.SetAnimatorTarget(FemaleAvatarAnimator);
            MaleAvatarAnimator.gameObject.SetActive(false);
            Debug.Log("FEMALE selected as Target Avatar");
        }

        return base.CoUpdate();
    }
}
