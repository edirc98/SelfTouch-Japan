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
public class Monitoring : MonoBehaviour
{
    
    public MonitoringData data = new MonitoringData();
    public SDT_Data SDT_data = new SDT_Data();

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

    public void SaveData(string stage)
    {
        setInitialTime(stage);
        string directory = Application.dataPath + "/MonitoringData/";
        string name = getInitTime() + ".json";
        writeDataToJson(directory, name, data);
    }
    public void SaveSDTData(string EmbodiedGender, int iter, string Character, float distance) {
        SDT_data.SDT_EmbodiedGender = EmbodiedGender;
        SDT_data.SDT_Iteration = iter;
        SDT_data.SDT_Character = Character;
        SDT_data.SDT_Distance = distance;

        string directory = Application.dataPath + "/MonitoringData/";
        string name = SettingsBase.GetGender().ToString() + SettingsBase.GetSubjectID().ToString() + ".json";
        writeDataToJson(directory, name, SDT_data);
    }
}
