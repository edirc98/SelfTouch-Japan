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
        CharacterSelection.SelectedCharacterAnimator.SetBool("IsWalking", true);
    }

    public void StopCharacter()
    {
        isMoving = false;
        CharacterSelection.SelectedCharacterAnimator.SetBool("IsWalking", false);
    }

    public void ConfirmDistance()
    {
        CharacterSelection.SelectedCharacterAnimator = null;
        moveCharacter.Finish();
    }

    private void Update()
    {
        if(CharacterSelection.SelectedCharacter != null)
        {
            if (CharacterSelection.SelectedCharacter.transform.position.z < 1.0f)
            {
                StopCharacter();
                ConfirmDistance();
            }
        }
        
    }
}
