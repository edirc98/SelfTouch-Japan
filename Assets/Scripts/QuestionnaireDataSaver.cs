using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public static class QuestionnaireDataSaver
{
    private static string _fileDirectory;
    private static readonly string Extension = "_Results.json";
    
    public static void SaveToJson<T>(T data, string fileName)
    {
        _fileDirectory = Application.persistentDataPath + "/Results/" + SettingsBase.GetSubjectID() + "/";
        string path = _fileDirectory + fileName + Extension;
        string json = JsonUtility.ToJson(data, true);

        if (!Directory.Exists(_fileDirectory))
        {
            Directory.CreateDirectory(_fileDirectory);
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
