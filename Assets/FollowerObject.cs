using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class FollowerObject : MonoBehaviour
{
    #region VARIABLES

    public enum FollowType
    {
        Sync, 
        Async
    }
    
    [Header("Follower Object")]
    public GameObject follower;

    [Header("Follower Object Config")] 
    public bool activeFollower = false;
    private FollowType _followType = FollowType.Sync;
    public Vector3 followerOffset;
    public float asyncTime = 3.0f;

    
    private List<Vector3> _positions = new List<Vector3>();

    private float _startTime = 0.0f;
    #endregion

    #region PROPERTIES

    public FollowType CurrentFollowType
    {
        get => _followType;
        set
        {
            if (_followType != value)
            {
                _followType = value;
                //Reset Async Start Time
                _startTime = 0.0f;
                _positions.Clear();
            }
        }
    }


    #endregion
    
    #region UNITY METHODS
    void Start()
    {
        
        follower.transform.position = transform.position + followerOffset;
    }

    private void Update()
    {
        if (Keyboard.current.spaceKey.wasPressedThisFrame)
        {
            ChangeFollowingType(CurrentFollowType == FollowType.Sync ? FollowType.Async : FollowType.Sync);
        }
    }

    void FixedUpdate()
    {
        if(activeFollower)_positions.Add(transform.position);

        if (_positions.Count > 0)
        {
            switch (_followType)
            {
                case FollowType.Sync:
                    follower.transform.position = _positions[0] + followerOffset;
                    RemoveFirstPosition();
                    break;
                case FollowType.Async:
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
            }
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

    private void ChangeFollowingType(FollowType newFollowType)
    {
        CurrentFollowType = newFollowType;
    }
    #endregion
}
