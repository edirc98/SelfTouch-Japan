using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LikertScaleController : MonoBehaviour
{
    public List<ButtonScaleAnimation> ScaleButtons;

    public int selectedOption = 0; 

    private void Awake()
    {
        gameObject.GetComponentsInChildren<ButtonScaleAnimation>(ScaleButtons); 
    }
}
