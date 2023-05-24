using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveCharacter : MonoBehaviour
{
    public SDTSelectCharacter CharacterSelection;
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
        if(selectedCharacterAnimator == null && CharacterSelection.SelectedCharacter !=null) selectedCharacterAnimator = CharacterSelection.SelectedCharacter.GetComponent<Animator>();
       
        //if(CharacterSelection.SelectedCharacter.activeSelf == false) CharacterSelection.SelectedCharacter.SetActive(true);
        

    }


    public void MoveCharacterForward()
    {
        isMoving = true;
        //StartCoroutine(Lerp());
    }

    public void StopCharacter()
    {
        isMoving = false;
        //StopCoroutine(Lerp());
    }

    IEnumerator Lerp()
    {
        float timeElapsed = 0;
        while (timeElapsed < lerpDuration)
        {
            CharacterSelection.SelectedCharacter.transform.position = Vector3.Lerp(CharacterSelection.SelectedCharacter.transform.position, lerpEndPose.position, timeElapsed / lerpDuration);
            timeElapsed += Time.deltaTime;
            yield return null;
        }
        CharacterSelection.SelectedCharacter.transform.position = lerpEndPose.position;
    }
}
