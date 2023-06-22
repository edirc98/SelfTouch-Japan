using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class CountDownTimer : MonoBehaviour
{
    public int InitTime = 3;
    public Text TimeText;
    public float time;
    public bool timerOn = false; 

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (timerOn) { 
            if(time > 0)
            {
                time -= Time.deltaTime;
                updateTimer((int)time);
            }
            else
            {
                timerOn = false; 
            }
        }
    }

    private void updateTimer(int currentTime)
    {
        TimeText.text = currentTime.ToString();
    }
}
