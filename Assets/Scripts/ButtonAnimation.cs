using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonAnimation : MonoBehaviour
{
    [SerializeField]
    private Animator ButtonAnimator;
    [SerializeField]
    private AudioSource ButtonSound;

    public MoveCharacter moveCharacter; 

    public bool WalkButton = false;
    public bool ConfirmButton = false;

    private bool presed = false;
    // Start is called before the first frame update
    void Start()
    {
        ButtonAnimator = GetComponent<Animator>();
        ButtonSound = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log("Entered Button Trigger");
        if (!presed)
        {
            presed = true;
            if (other.CompareTag("Hand"))
            {
                ButtonAnimator.SetTrigger("PressButton");
                if (WalkButton)
                {
                    if (moveCharacter.isMoving == false)
                    {
                        Debug.Log("START Moving Character");
                        moveCharacter.MoveCharacterForward();
                    }
                    else if (moveCharacter.isMoving == true)
                    {
                        Debug.Log("STOP Moving Character");
                        moveCharacter.StopCharacter();
                    }
                }
                else if (ConfirmButton)
                {
                    Debug.Log("CONFIRM Distance");
                    //TODO
                }

                
                ButtonSound.Play();
            }
        }
        
    }

    private void OnTriggerExit(Collider other)
    {
        if (presed)
        {
            if (other.CompareTag("Hand"))
            {
                presed = false;
                ButtonAnimator.SetTrigger("ReleaseButton");
            }
        }
    }
}
