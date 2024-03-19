using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.IO;

public class SerialController : MonoBehaviour
{
    public SerialHandler serialHandler;
    public GameObject targetObject;
    public Vector3 initialPosition;

    [Header("I/O Controller")]
    //[SerializeField, NonEditable]
    //public float[] analogInput;
    //[SerializeField, NonEditable]
    //public int[] digitalInput;
    [SerializeField, NonEditable]
    public int[] receivedData;

    // Start is called before the first frame update
    void Start()
    {
        // Message process when signal is received
        serialHandler.OnDataReceived += OnDataReceived;
    }

    // Update is called once per frame
    void Update()
    {

    }

    // FixedUpdate is called once per frame
    void FixedUpdate()
    {
        // Move target object based on received data
        targetObject.transform.position = new Vector3((float)-receivedData[1] / 1000.0f + initialPosition.x, (float)receivedData[2] / 1000.0f + initialPosition.y, (float)receivedData[0] / 1000.0f + initialPosition.z);
        targetObject.transform.eulerAngles = new Vector3(receivedData[4], -receivedData[5], receivedData[3]);
    }

    // Process when serial data is received  
    void OnDataReceived(string message)
    {
        try
        {
            // Split message by comma
            string[] split = message.Split(',');

            // Receive data
            for (int i = 0; i < receivedData.Length; i++)
            {
                receivedData[i] = int.Parse(split[i]);
            }
        }
        catch (System.Exception e)
        {
            Debug.LogWarning(e.Message);
        }
    }
}
