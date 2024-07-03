using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LikertScaleController : MonoBehaviour
{
    public List<ButtonScaleAnimation> ScaleButtons;

    public int selectedOption = -1; 

    private void Awake()
    {
        gameObject.GetComponentsInChildren<ButtonScaleAnimation>(ScaleButtons); 
    }

    public void ResetButtonsToIdle()
    {
        foreach (ButtonScaleAnimation button in ScaleButtons)
        {
            button.ChangeToIdleMaterial(); 
        }
    }

    public void ResetSelectedOption()
    {
        selectedOption = -1;
    }
}
