using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class StageMonitoring : QuickStageBase
{
    public SDTSelectCharacter CharacterSelection;
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
        iter++;
        DataMonitoring.SaveSDTData(SettingsBase.GetGender().ToString(), iter, CharacterSelection.SelectedCharacter.name, CharacterSelection.SelectedCharacter.transform.position.z);
        return base.CoUpdate();
    }


}
