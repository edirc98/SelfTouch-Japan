using System.Collections.Generic;
using QuickVR;


[System.Serializable]
public class Condition
{
    public enum BodyType
    {
        Human,
        Treent, 
        Robot
    }; 
    public enum TouchType
    {
        Async, 
        Sync
    }

    public BodyType avatarBodyType;
    public TouchType conditionTouchType; 
}
public class StageSetConditionsOrder : QuickStageBase
{
    public List<Condition> currentConditions;
}
