using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectFollow : MonoBehaviour
{
    [Header("Follower Object Control")]
    [SerializeField] private GameObject FollowerObject;
    public bool applyPosition = true;
    public bool applyRotation = false; 
    [Header("Position and rotation adjustment")]
    [SerializeField] private Vector3 PosOffset;
    [SerializeField] private Vector3 RotOffset;


    // Start is called before the first frame update
    void Start()
    {
        FollowerObject = GameObject.FindGameObjectWithTag("Follower"); 
    }

    // Update is called once per frame
    private void FixedUpdate()
    {
        if (applyPosition)
        {
            FollowerObject.transform.position = transform.position + PosOffset;
        }
        if (applyRotation)
        {
            Quaternion rotOffsetQuat = Quaternion.Euler(RotOffset);
            FollowerObject.transform.rotation = new Quaternion(transform.rotation.x, 0, 0, 1);
        }
        
    }

    public void SetPositionOffset(Vector3 newPosOffset)
    {
        PosOffset = newPosOffset; 
    }
    public void SetRotationOffset(Vector3 newRotOffset)
    {
        RotOffset = newRotOffset;
    }

}
