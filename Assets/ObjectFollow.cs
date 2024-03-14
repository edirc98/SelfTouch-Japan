using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectFollow : MonoBehaviour
{
    [SerializeField] private GameObject FollowerObject;
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
        FollowerObject.transform.position = transform.position + PosOffset;
        Quaternion rotOffsetQuat = Quaternion.Euler(RotOffset);
        FollowerObject.transform.rotation = new Quaternion(transform.rotation.x, 0, 0, 1);
    }

    public void SetPositionOffset(Vector3 newPosOffset)
    {
        PosOffset = newPosOffset; 
    }

}
