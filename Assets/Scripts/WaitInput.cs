using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class WaitInput : QuickStageBase
{
    public bool EndWalk = false;
    public string ButtonString;
    public SDTSelectCharacter CharacterSelection;
    
    protected override void Start()
    {
        base.Start();
    }
    protected override IEnumerator CoUpdate()
    {
        while (!InputManager.GetButtonDown(ButtonString)) yield return null;


        if (EndWalk)
        {
            Animator selectedCharacterAnimator = CharacterSelection.SelectedCharacter.GetComponent<Animator>();
            selectedCharacterAnimator.SetTrigger("EndWalk");
            yield return new WaitForSeconds(3.0f);
        }
        

    }

}
