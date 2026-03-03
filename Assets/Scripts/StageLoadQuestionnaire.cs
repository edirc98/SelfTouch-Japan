using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;
public class StageLoadQuestionnaire : QuickStageBase
{
    public GameObject questionnaireGo;
    
    
    protected override IEnumerator CoUpdate()
    {
        questionnaireGo.SetActive(true);
        while (questionnaireGo.activeSelf == true)
        {
            yield return null;
        }
        base.Finish();
    }
}
