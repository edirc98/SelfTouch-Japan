using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveOneCharacter : MonoBehaviour
{
    public SDTOneCharacter CharacterSelection;
    public SDTMoveOneCharacter moveCharacter;


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
        moveCharacter.Finish();
        CharacterSelection.SelectedCharacterAnimator = null;
    }
    
    private void Update()
    {
        if(CharacterSelection.SelectedCharacter != null && CharacterSelection.canSkipByDistance)
        {
            if (CharacterSelection.SelectedCharacter.transform.position.z < 1.0f)
            {
                CharacterSelection.canSkipByDistance = false; 
                StopCharacter();
                ConfirmDistance();
            }
        }
        
    }
}
