using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR; 

public class SDTResetCharacterPos : QuickStageBase
{
    public bool isTouchScene = false;
    public SDTSelectCharacter CharacterSelection;
    public SDTOneCharacter CharacterSelectedInTouch; 
    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        if (isTouchScene)
        {
            CharacterSelectedInTouch.SelectedCharacter.SetActive(false);
            CharacterSelectedInTouch.SelectedCharacter.transform.position = new Vector3(0, 0, 10);
            CharacterSelectedInTouch.SelectedCharacter.transform.rotation = Quaternion.Euler(new Vector3(0, -180, 0));
        }
        else
        {
            CharacterSelection.SelectedCharacter.SetActive(false);
            CharacterSelection.SelectedCharacter.transform.position = new Vector3(0, 0, 10);
            CharacterSelection.SelectedCharacter.transform.rotation = Quaternion.Euler(new Vector3(0, -180, 0));
        }

        return base.CoUpdate();
    }
}
