using System.Collections.Generic;
using UnityEngine;


public class LikertQuestionnaire
{
    public List<LikertQuestion> Questions;
}
[System.Serializable]
public class LikertQuestion  {
    public string questionText;
    public string positiveText;
    public string negativeText;
}

public static class QuestionnaireLoader
{
    public static List<LikertQuestion> LoadQuestionnaire(string questionnaireName)
    {
        TextAsset json = Resources.Load<TextAsset>("Questionnaires/" + questionnaireName);
        
        if(json != null) Debug.Log("Loaded Questionnaire " + questionnaireName);
        else Debug.Log("No Questionnaire found");
        
        LikertQuestionnaire list = JsonUtility.FromJson<LikertQuestionnaire>(json.text);
        if(list.Questions != null) Debug.Log("Questions loaded " + list.Questions.Count);
        return list.Questions;
    }
}