using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;
using UnityEngine.UI;


[System.Serializable]
public class Question
{
    public string questionString;
    public int response;
}

public class StageQuestionaire : QuickStageBase
{
    

    public List<Question> Questions;

    public GameObject QuestionsCanvas;
    public Text QuestionText;
    public Slider ResponseSlider;


    private int currentValue;
    private bool _valueValidated = false;
    private float activationThreshold = 0.05f;
    private Monitoring DataMonitoring;

    protected override void Start()
    {
        DataMonitoring = QuickSingletonManager.GetInstance<Monitoring>();

        base.Start();
    }
    protected override IEnumerator CoUpdate()
    {
        //Activate canvas
        QuestionsCanvas.SetActive(true);
        //For each question wait until the user presses the selection button
        for(int i =0; i < Questions.Count; i++)
        {
            // Start slider and current value at 0
            ResponseSlider.value = 0;
            currentValue = (int)ResponseSlider.value;
            // Update Question text with the corresponding question string
            QuestionText.text = Questions[i].questionString;

            //Start the corrutine that alow us to move the slider 
            StartCoroutine("UpdateSlider");
            yield return StartCoroutine("WaitUserInput");
            //Once value has been selected, save the value in the corresponding question.
            Questions[i].response = (int)ResponseSlider.value; 
            _valueValidated = false;
            DataMonitoring.SaveQuestionData(Questions[i].questionString, Questions[i].response);
            yield return new WaitForSeconds(1);
        }

        //Once the stage is finished deactivate canvas
        QuestionsCanvas.SetActive(false);
        base.Finish();
    }



    protected IEnumerator WaitUserInput()
    {
        while (!InputManager.GetButtonDown("ValueValidation"))
        {
            yield return null;
        }
        _valueValidated = true;

    }
    protected IEnumerator UpdateSlider()
    {
        while (!_valueValidated)
        {
            if (System.Math.Abs(InputManager.GetAxis("SliderRightSelector")) > activationThreshold || System.Math.Abs(InputManager.GetAxis("SliderLeftSelector")) > activationThreshold)
            {
                if (InputManager.GetAxis("SliderRightSelector") > 0.0f || InputManager.GetAxis("SliderLeftSelector") > 0.0f)
                {
                    UpdateSliderValue(1);
                }
                else UpdateSliderValue(-1);
            }
            yield return new WaitForSeconds(0.05f);
        }
    }

    private void UpdateSliderValue(int value)
    {
        currentValue += value;
        //Control the slider value do not go more than the lower and upper limit
        if (currentValue > ResponseSlider.maxValue) currentValue = (int)ResponseSlider.maxValue;
        if (currentValue < ResponseSlider.minValue) currentValue = (int)ResponseSlider.minValue;

        ResponseSlider.value = currentValue;
    }
}
