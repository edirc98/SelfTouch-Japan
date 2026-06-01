using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EmbodimentTimeDetector : MonoBehaviour
{
    public GameObject embodimentInstructions;

    public bool canDetectPedal = false; 

    // Update is called once per frame
    void Update()
    {
        if (canDetectPedal && Input.GetKeyDown(KeyCode.K))
        {
            embodimentInstructions.SetActive(false);
        }
    }
}
