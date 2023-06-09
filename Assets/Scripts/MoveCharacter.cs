using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveCharacter : MonoBehaviour
{
    public SDTSelectCharacter CharacterSelection;
    public SDTMoveCharacter moveCharacter;

    public bool isMoving = false;

    public void MoveCharacterForward()
    {
        isMoving = true;
        CharacterSelection.SelectedCharacterAnimator.SetTrigger("StartWalk");
    }

    public void StopCharacter()
    {
        isMoving = false;
        CharacterSelection.SelectedCharacterAnimator.SetTrigger("EndWalk");
    }

    public void ConfirmDistance()
    {
        moveCharacter.Finish();
        CharacterSelection.SelectedCharacterAnimator = null;
    }
}
