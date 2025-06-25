using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;


[System.Serializable]
public class Condition
{
    public enum BodyType
    {
        HumanMale,
        HumanFemale,
        Treent, 
        Robot
    }; 
    public enum TouchType
    {
        Async, 
        Sync
    }

    public BodyType AvatarBodyType;
    public TouchType ConditionTouchType; 
}
public class StageSetConditionsOrder : QuickStageBase
{
    public List<Condition> CurrentConditions;

    protected override void Start()
    {
        //can be added to read a csv file and add to the CurrentConditions the corresponding random order. 
        base.Start();
    }
}
