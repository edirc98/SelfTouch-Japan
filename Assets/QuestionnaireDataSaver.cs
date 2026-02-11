using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public static class QuestionnaireDataSaver
{
    private static readonly string FileDirectory = Application.persistentDataPath + "/Results/";
    private static readonly string Extension = "_Results.json";
    
    public static void SaveToJson<T>(T data, string fileName)
    {
       
        string path = FileDirectory + fileName +"_" +SettingsBase.GetSubjectID() + Extension;
        string json = JsonUtility.ToJson(data, true);

        if (!Directory.Exists(FileDirectory))
        {
            Directory.CreateDirectory(FileDirectory);
        }
        
        if (!File.Exists(path))
        {
            File.WriteAllText(path, json);
        }
        else
        {
            File.AppendAllText(path, json);
        }
        File.AppendAllText(path, "\n");
        
        Debug.Log("Saved at: " + path);
    }
}
