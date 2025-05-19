using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowerObject : MonoBehaviour
{
    #region VARIABLES

    public enum FollowType
    {
        SYNC, 
        ASYNC
    }
    
    [Header("Follower Object")]
    public GameObject follower;
    
    [Header("Follower Object Config")]
    public Vector3 followerOffset;
    public FollowType followType = FollowType.SYNC;
    public float asyncTime = 3.0f;

    
    private List<Vector3> _positions = new List<Vector3>();

    private float _startTime = 0.0f;
    #endregion

    #region UNITY METHODS
    // Start is called before the first frame update
    void Start()
    {
        follower.transform.position = transform.position + followerOffset;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        _positions.Add(transform.position);

        switch (followType)
        {
            case FollowType.SYNC:
                follower.transform.position = _positions[0] + followerOffset;
                RemoveFirstPosition();
                break;
            case FollowType.ASYNC:
                if (_startTime == 0.0f)
                {
                    _startTime = Time.time;
                }

                if (Time.time > _startTime + asyncTime)
                {
                    follower.transform.position = _positions[0] + followerOffset;
                    RemoveFirstPosition();
                }
                break;
            default:
                break;
        }

        
    }
    #endregion

    #region METHODS

    private void RemoveFirstPosition()
    {
        if (_positions.Count > 0)
        {
            _positions.RemoveAt(0);
        }
    }

    #endregion
}
