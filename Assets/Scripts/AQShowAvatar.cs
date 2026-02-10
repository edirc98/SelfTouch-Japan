using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class AQShowAvatar : QuickStageBase
{
    public QuickStageLoop AvatarsLoop; 
    public List<GameObject> AQAvatars;
    public GameObject currentAvatar; 
    
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        if (currentAvatar == null)
        {
            currentAvatar = AQAvatars[AvatarsLoop.GetCurrentInteration()];
        }
        else
        {
            currentAvatar.SetActive(false); 
            currentAvatar = AQAvatars[AvatarsLoop.GetCurrentInteration()];
        }
        
        currentAvatar.SetActive(true); 

        return base.CoUpdate();
    }
}
