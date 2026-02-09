using System;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using UnityEngine;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct SharedMemoryData
{
    public int fromUnity;
    [MarshalAs(UnmanagedType.I1)]
    public bool fromUnityUpdated;

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
    public int[] toUnity;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
    public bool[] toUnityUpdated;
}

public class SharedMemoryClient : MonoBehaviour
{
    private MemoryMappedFile mmf;
    private MemoryMappedViewAccessor accessor;

    private const string MAP_NAME = "Local\\UnitySharedMemory";
    private static readonly int SIZE = Marshal.SizeOf<SharedMemoryData>();

    public GameObject hapticDevice;

    // Define StopWatch
    System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();

    void Start()
    {
        try
        {
            mmf = MemoryMappedFile.OpenExisting(MAP_NAME);
            accessor = mmf.CreateViewAccessor(0, SIZE, MemoryMappedFileAccess.ReadWrite);
            Debug.Log("Connected to Shared Memory");
        }
        catch (Exception e)
        {
            Debug.LogError("Unable to connect to Shared Memory: " + e.Message);
        }
    }

    void FixedUpdate()
    {
        if (accessor == null) return;

        // Show communication time
        Debug.Log("Communication time�F" + sw.ElapsedMilliseconds + " ms");
       
        // Stop measurement of communication time
        sw.Stop();

        // Reset measurement of comuunication time
        sw.Reset();

        // Start measurement of communication time
        sw.Start();

        SharedMemoryData data = ReadData();

        // Receive the data from C++ program
        for (int i = 0; i < 6; i++)
        {
            if (data.toUnityUpdated[i])
            {
                data.toUnityUpdated[i] = false;
            }
        }
        //Debug.Log("Position data: " + data.toUnity[0] + "," + data.toUnity[1] + "," + data.toUnity[2]);
        Debug.Log("Orientation data: " + data.toUnity[3] + "," + data.toUnity[4] + "," + data.toUnity[5]);

        hapticDevice.transform.position = new Vector3(
            (float)data.toUnity[0] / 100000.0f, (float)data.toUnity[2] / 100000.0f, (float)data.toUnity[3] / 100000.0f);

        hapticDevice.transform.eulerAngles = new Vector3(
            Mathf.Rad2Deg * (float)data.toUnity[3] / 100.0f, Mathf.Rad2Deg * (float)data.toUnity[4] / 100.0f, Mathf.Rad2Deg * (float)data.toUnity[5] / 100.0f);

        //// Send the data to C++ program
        //int newValue = Time.frameCount;  // for example, send the frame count

        //if (data.fromUnity != newValue)
        //{
        //    data.fromUnity = newValue;
        //    data.fromUnityUpdated = true;
        //}

        //WriteData(data);
    }

    void OnApplicationQuit()
    {
        accessor?.Dispose();
        mmf?.Dispose();
    }

    private SharedMemoryData ReadData()
    {
        byte[] buffer = new byte[SIZE];
        accessor.ReadArray(0, buffer, 0, SIZE);

        GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
        SharedMemoryData data = Marshal.PtrToStructure<SharedMemoryData>(handle.AddrOfPinnedObject());
        handle.Free();

        return data;
    }

    private void WriteData(SharedMemoryData data)
    {
        byte[] buffer = new byte[SIZE];
        IntPtr ptr = Marshal.AllocHGlobal(SIZE);
        Marshal.StructureToPtr(data, ptr, false);
        Marshal.Copy(ptr, buffer, 0, SIZE);
        Marshal.FreeHGlobal(ptr);

        accessor.WriteArray(0, buffer, 0, SIZE);
    }
}