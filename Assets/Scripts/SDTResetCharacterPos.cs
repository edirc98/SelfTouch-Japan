using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class SDTResetCharacterPos : QuickStageBase
{
    public SDTSelectCharacter CharacterSelection;
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        CharacterSelection.SelectedCharacter.SetActive(false);
        CharacterSelection.SelectedCharacter.transform.position = new Vector3(0, 0, 10);
        CharacterSelection.SelectedCharacter.transform.rotation = Quaternion.Euler(new Vector3(0, -180, 0));
        return base.CoUpdate();
    }
}
