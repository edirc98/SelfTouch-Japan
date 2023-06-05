using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class DisableObjects : QuickStageBase
{
    public List<GameObject> gameObjects;
    protected override IEnumerator CoUpdate()
    {
        foreach( GameObject go in gameObjects)
        {
            go.SetActive(false);
        }
            
        
        return base.CoUpdate();
    }
}
