using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;


[System.Serializable]
public class MonitoringData
{
    public string Gender;
    public string Stage;
    public string Time;
    public System.DateTime InitTime;
    public float SesionDuration;

}
[System.Serializable]
public class SDT_Data
{
    public string SDT_EmbodiedGender;
    public float SDT_Distance;
    public string SDT_Character;
    public int SDT_Iteration;

}
[System.Serializable]
public class Question_Data
{
    public string QuestionString;
    public int Response;

}

[System.Serializable]
public class AQ_Data
{
    public string AvatarShown;
    public string Question;
    public int Response; 
}
public class Monitoring : MonoBehaviour
{
    
    public MonitoringData data = new MonitoringData();
    public SDT_Data SDT_data = new SDT_Data();
    public Question_Data question_data = new Question_Data();
    public AQ_Data aq_data = new AQ_Data();


    // Start is called before the first frame update
    public void setInitialTime(string stage)
    {
        data.Stage = stage;
        data.InitTime = System.DateTime.Now;
        data.Time = data.InitTime.ToString("dd-MM-yyyy-HH-mm-ss");
    }
    public string getInitTime()
    {
        return data.InitTime.ToString("dd-MM-yyyy");
    }
    public void setDuration()
    {
        var now = System.DateTime.Now;
        var diff = now - data.InitTime;
        data.SesionDuration = diff.Seconds;
    }
    public void writeDataToJson(string directory, string name, MonitoringData data)
    {
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }
        string outputData = JsonUtility.ToJson(data);
        if (!File.Exists(directory + name))
        {
            File.WriteAllText(directory + name, outputData);
        }
        else
        {
            File.AppendAllText(directory + name, outputData);
        }
        File.AppendAllText(directory + name, "\n");
    }
    public void writeDataToJson(string directory, string name, SDT_Data data)
    {
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }
        string outputData = JsonUtility.ToJson(data);
        if (!File.Exists(directory + name))
        {
            File.WriteAllText(directory + name, outputData);
        }
        else
        {
            File.AppendAllText(directory + name, outputData);
        }
        File.AppendAllText(directory + name, "\n");
    }
    public void writeDataToJson(string directory, string name, Question_Data data)
    {
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }
        string outputData = JsonUtility.ToJson(data);
        if (!File.Exists(directory + name))
        {
            File.WriteAllText(directory + name, outputData);
        }
        else
        {
            File.AppendAllText(directory + name, outputData);
        }
        File.AppendAllText(directory + name, "\n");
    }
    public void writeDataToJson(string directory, string name, AQ_Data data)
    {
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }
        string outputData = JsonUtility.ToJson(data);
        if (!File.Exists(directory + name))
        {
            File.WriteAllText(directory + name, outputData);
        }
        else
        {
            File.AppendAllText(directory + name, outputData);
        }
        File.AppendAllText(directory + name, "\n");
    }

    public void SaveData(string stage)
    {
        setInitialTime(stage);
#if UNITY_ANDROID
        string directory = Application.persistentDataPath + "/MonitoringData/";
#else
        string directory = Application.dataPath + "/MonitoringData/";

#endif 
        string FileName = SettingsBase.GetSubjectID().ToString() + ".json";
        writeDataToJson(directory, FileName, data);
    }

    public void SaveSDTData(string EmbodiedGender, int iter, string Character, float distance) {
        SDT_data.SDT_EmbodiedGender = EmbodiedGender;
        SDT_data.SDT_Iteration = iter;
        SDT_data.SDT_Character = Character;
        SDT_data.SDT_Distance = distance;

#if UNITY_ANDROID
        string directory = Application.persistentDataPath + "/MonitoringData/";
#else
                string directory = Application.dataPath + "/MonitoringData/";

#endif
        string FileName = SettingsBase.GetSubjectID().ToString() + ".json";
        writeDataToJson(directory, FileName, SDT_data);
    }
    public void SaveQuestionData(string questionString, int response)
    {
        question_data.QuestionString = questionString;
        question_data.Response = response; 

#if UNITY_ANDROID
        string directory = Application.persistentDataPath + "/MonitoringData/";
#else
        string directory = Application.dataPath + "/MonitoringData/";

#endif
        string FileName = SettingsBase.GetSubjectID().ToString() + ".json";
        writeDataToJson(directory, FileName, question_data);
    }
    public void SaveAQData(string ShownAvatar, string question, int response)
    {
        aq_data.AvatarShown = ShownAvatar;
        aq_data.Question = question;
        aq_data.Response = response;
#if UNITY_ANDROID
        string directory = Application.persistentDataPath + "/MonitoringData/";
#else
        string directory = Application.persistentDataPath + "/MonitoringData/";

#endif
        string FileName = SettingsBase.GetSubjectID().ToString() + ".json";
        writeDataToJson(directory, FileName, aq_data);

    }
}
