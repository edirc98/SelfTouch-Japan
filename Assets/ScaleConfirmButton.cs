using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScaleConfirmButton : MonoBehaviour
{
    [SerializeField]
    private Animator ButtonAnimator;
    [SerializeField]
    private AudioSource ButtonSound;


    private bool presed = false;

    public AnthropomorphismQuestionaire AntropoQuestion;
    public EmbodimentQuestionaire EmbodimentQuestion;
    public LikertScaleController ScaleController; 

    // Start is called before the first frame update
    void Start()
    {
        ButtonAnimator = GetComponent<Animator>();
        ButtonSound = GetComponent<AudioSource>();
    }

    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log("Entered Button Trigger");
        if (!presed)
        {
            presed = true;
            if (other.CompareTag("Hand"))
            {
                if(ScaleController.selectedOption != -1)
                {
                    if(AntropoQuestion)AntropoQuestion.canConfirm = true; 
                    if(EmbodimentQuestion)EmbodimentQuestion.canConfirm = true;
                    AntropoQuestion.Confirm();
                    ScaleController.ResetButtonsToIdle();
                    ScaleController.ResetSelectedOption(); 
                }
                ButtonAnimator.SetTrigger("PressButton");
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
