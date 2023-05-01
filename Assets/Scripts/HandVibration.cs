using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;
public enum HAND
{
    LEFT_HAND,
    RIGHT_HAND,
}
public class HandVibration : MonoBehaviour
{
    
    public HAND _hand;
    public float VibrationDuration = 0.1f; 
    private QuickVibratorManager _vibratorManager; 
    // Start is called before the first frame update
    void Start()
    {
        _vibratorManager = QuickSingletonManager.GetInstance<QuickVibratorManager>(); 
    }

    private void OnTriggerEnter(Collider other)
    {
        if(other.CompareTag("Hand") == false)
        {
            if (other.CompareTag("VibrationObject") == true)
            {
                if (_hand == HAND.LEFT_HAND)
                {
                    QuickVibratorManager.Vibrate(_vibratorManager.GetVirtualVibrator(0), VibrationDuration);
                }
                else
                {
                    QuickVibratorManager.Vibrate(_vibratorManager.GetVirtualVibrator(1), VibrationDuration);
                }
            }
        }
    }
}
