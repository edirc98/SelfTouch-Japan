using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonScaleAnimation : MonoBehaviour
{

    [SerializeField]
    private Animator ButtonAnimator;
    [SerializeField]
    private AudioSource ButtonSound;
    [SerializeField]
    private LikertScaleController ScaleController; 

    private bool presed = false;
    [Header("Button Parameters")]
    public int id;

    [Header("Materials")]
    public Material IdleMat;
    public Material SelectedMat; 

    // Start is called before the first frame update
    void Start()
    {
        ButtonAnimator = GetComponent<Animator>();
        ButtonSound = GetComponent<AudioSource>();
        ScaleController = GetComponentInParent<LikertScaleController>(); 
    }

    private void OnTriggerEnter(Collider other)
    {
        //Debug.Log("Entered Button Trigger");
        if (!presed)
        {
            presed = true;
            if (other.CompareTag("Hand"))
            {
                SelectButton();
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

    public void ChangeToSelectedMaterial()
    {
        transform.GetChild(1).GetComponent<MeshRenderer>().material = SelectedMat; 
    }

    public void ChangeToIdleMaterial()
    {
        transform.GetChild(1).GetComponent<MeshRenderer>().material = IdleMat;
    }

    public void SelectButton()
    {
        foreach(ButtonScaleAnimation button in ScaleController.ScaleButtons)
        {
            if (button.id != id)
            {
                button.ChangeToIdleMaterial();

            }
            else
            {
                button.ChangeToSelectedMaterial();
                ScaleController.selectedOption = id;
                ScaleController.lastSelectedOption = id;
            } 
                
        }
    }
}
