using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;
using UnityEngine.SceneManagement;

public class ChangeScene : QuickStageBase
{
    public int sceneBuildIndex;
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        SceneManager.LoadScene(sceneBuildIndex);
        return base.CoUpdate();
    }
}
