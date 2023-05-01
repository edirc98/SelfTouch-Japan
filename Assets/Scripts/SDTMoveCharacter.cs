using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class SDTMoveCharacter : QuickStageBase
{
    public SDTSelectCharacter CharacterSelection;
    protected override void Start()
    {
        base.Start();
    }
    protected override IEnumerator CoUpdate()
    {
        Animator selectedCharacterAnimator = CharacterSelection.SelectedCharacter.GetComponent<Animator>();
        //CharacterSelection.SelectedCharacter.SetActive(true);

        yield return new WaitForSeconds(2.0f);
        selectedCharacterAnimator.SetTrigger("StartWalk");
        base.CoUpdate();
    }


}
