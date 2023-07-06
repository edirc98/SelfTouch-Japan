using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class StageMonitoring : QuickStageBase
{

    public bool isTouchScene = false;
    public SDTSelectCharacter CharacterSelection;
    public SDTOneCharacter CharacterSelectedInTouch;
    public QuickStageLoop MainLoop;
    int iter = 0;

    [HideInInspector]
    public Monitoring DataMonitoring; 

    protected override void Start()
    {
        DataMonitoring = QuickSingletonManager.GetInstance<Monitoring>();
        //DataMonitoring.SaveData(MonitoringCaption);
        
        base.Start();
    }
    protected override IEnumerator CoUpdate()
    {
        iter = MainLoop.GetCurrentInteration();
        if (isTouchScene)
        {
            DataMonitoring.SaveSDTData("Touch SDT; " + SettingsBase.GetGender().ToString(), iter, CharacterSelectedInTouch.SelectedCharacter.name, CharacterSelectedInTouch.SelectedCharacter.transform.position.z);
        }
        else
        {
            DataMonitoring.SaveSDTData(SettingsBase.GetGender().ToString(), iter, CharacterSelection.SelectedCharacter.name, CharacterSelection.SelectedCharacter.transform.position.z);
        }
        
        return base.CoUpdate();
    }


}
