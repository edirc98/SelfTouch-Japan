//using UnityEngine;
//using System.Collections;
//using System.IO.Ports;
//using System.Threading;

//public class SerialHandler : MonoBehaviour
//{
//    public delegate void SerialDataReceivedEventHandler(string message);
//    public event SerialDataReceivedEventHandler OnDataReceived;

//    public string portName;                         // Port name (e.g., COM1, COM2, ...)
//    public int baudRate;                            // Baud rate (e.g., 9600, 115200, ...)
//    public int dataLength;                          // Data length (8 bits in general)

//    private SerialPort serialPort_;                 // Serial port
//    private Thread thread_;                         // Thread
//    private bool isRunning_ = false;                // Running flag

//    private string message_;                        // String
//    private bool isNewMessageReceived_ = false;     // Message flag

//    // Use this for initialization
//    void Start()
//    {
//        Open();
//    }

//    // Update is called once per frame
//    void Update()
//    {
//        if (isNewMessageReceived_)                  // if message extists...
//        {
//            // Read message
//            OnDataReceived(message_);
//        }
//        isNewMessageReceived_ = false;
//    }

//    // Destroy serial port
//    void OnDestroy()
//    {
//        Close();
//    }

//    // Open function
//    private void Open()
//    {
//        // Configuration of serial port
//        serialPort_ = new SerialPort(portName, baudRate, Parity.None, 8, StopBits.One);
//        //serialPort_ = new SerialPort(portName, baudRate);

//        // Port open
//        serialPort_.Open();

//        // Set timeout [ms]
//        serialPort_.ReadTimeout = 20;

//        //  Running flag ON
//        isRunning_ = true;

//        // Create new thread for read
//        thread_ = new Thread(Read);
//        thread_.Start();
//    }

//    // Close function
//    private void Close()
//    {
//        // Initialize flags
//        isNewMessageReceived_ = false;
//        isRunning_ = false;

//        if (thread_ != null && thread_.IsAlive)
//        {
//            thread_.Join();
//        }

//        if (serialPort_ != null && serialPort_.IsOpen)
//        {
//            // Close serial port
//            serialPort_.Close();

//            // Dispose serial port
//            serialPort_.Dispose();
//        }
//    }

//    // Read function
//    private void Read()
//    {
//        while (isRunning_ && serialPort_ != null && serialPort_.IsOpen)
//        {
//            try
//            {
//                message_ = serialPort_.ReadLine();
//                isNewMessageReceived_ = true;
//            }
//            catch (System.Exception e)
//            {
//                Debug.LogWarning(e.Message);
//            }
//        }
//    }

//    // Write function
//    public void Write(string message)
//    {
//        try
//        {
//            serialPort_.Write(message);
//        }
//        catch (System.Exception e)
//        {
//            Debug.LogWarning(e.Message);
//        }
//    }
//}