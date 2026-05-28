using System.Collections;
using System.Collections.Generic;
using QuickVR;
using UnityEngine;

public class StageLoadQuestionnaireConditional : StageLoadQuestionnaire
{
    public QuickStageLoop loop; 
    
    
    protected override IEnumerator CoUpdate()
    {
		if(loop.numIterations == 4){
			loop.numIterations = 3; 
		}
        if (loop.currentIteration == loop.numIterations-1)
        {
			loop.numIterations = loop.numIterations+1; 
            questionnaireGo.SetActive(true);
            while (questionnaireGo.activeSelf == true)
            {
                yield return null;
            }
        }
        base.Finish();
    }
    
}
