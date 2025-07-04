﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;


[System.Serializable]
public class MonitoringData
{
    public string Gender;
    public string Stage;
    public string Time;
    public float SesionDuration;
    
}

public class LogData
{
    public string Stage;
    public string Time;
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
    public LogData log_data = new LogData();


    private static string FileDirectory;
    private static string FileName;

    public void Start()
    {
        FileDirectory = Application.persistentDataPath + "/MonitoringData/";
        FileName = GetDate() + "_" + SettingsBase.GetSubjectID() + ".json";
    }

    public static string GetDirectory()
    {
        return FileDirectory; 
    }
    public static string GetFileName()
    {
        return FileName; 
    }

    public void SetInitialTime(string stage)
    {
        data.Stage = stage;
        data.Time = System.DateTime.Now.ToString("HH-mm-ss");
    }


    public string GetDate()
    {
        return System.DateTime.Now.ToString("dd-MM-yyyy");
    }
    
    public void WriteDataToJson(string directory, string name, MonitoringData data)
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
    public void WriteDataToJson(string directory, string name, LogData data)
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
    public void WriteDataToJson(string directory, string name, SDT_Data data)
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
    public void WriteDataToJson(string directory, string name, Question_Data data)
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
    public void WriteDataToJson(string directory, string name, AQ_Data data)
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


    public void SaveLogData(string Log)
    {
        log_data.Stage = Log; 
        log_data.Time = System.DateTime.Now.ToString("HH-mm-ss");
        
        string dir = GetDirectory();
        string name = GetFileName(); 
        WriteDataToJson(dir, name, log_data);
    }

    public void SaveSdtData(string EmbodiedGender, int iter, string Character, float distance) {
        SDT_data.SDT_EmbodiedGender = EmbodiedGender;
        SDT_data.SDT_Iteration = iter;
        SDT_data.SDT_Character = Character;
        SDT_data.SDT_Distance = distance;

        string dir = GetDirectory();
        string name = GetFileName(); 
        WriteDataToJson(dir, name, SDT_data);
    }
    public void SaveQuestionData(string questionString, int response)
    {
        question_data.QuestionString = questionString;
        question_data.Response = response;

        string dir = GetDirectory();
        string name = GetFileName();
        WriteDataToJson(dir, name, question_data);
    }
    public void SaveAqData(string ShownAvatar, string question, int response)
    {
        aq_data.AvatarShown = ShownAvatar;
        aq_data.Question = question;
        aq_data.Response = response;

        string dir = GetDirectory();
        string name = GetFileName();
        WriteDataToJson(dir, name, aq_data);

    }
}
