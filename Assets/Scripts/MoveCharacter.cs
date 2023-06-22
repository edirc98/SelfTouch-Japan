using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveCharacter : MonoBehaviour
{
    public SDTSelectCharacter CharacterSelection;
    public SDTMoveCharacter moveCharacter;
    private Animator selectedCharacterAnimator;

    public bool isMoving = false;

    [Header("Lerp Params")]
    public float lerpDuration;
    public Transform lerpEndPose;
    
    // Start is called before the first frame update
    void Start()
    {
       

    }

    // Update is called once per frame
    void Update()
    {

    }


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
        selectedCharacterAnimator = null;
    }
}
