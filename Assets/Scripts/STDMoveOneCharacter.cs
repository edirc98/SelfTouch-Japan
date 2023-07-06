using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class STDMoveOneCharacter : QuickStageBase
{
    public GameObject walkButton;
    public GameObject confirmButton;

    public bool DistanceConfirmed;

    protected override IEnumerator CoUpdate()
    {
        walkButton.SetActive(true);
        confirmButton.SetActive(true);
        //CharacterSelection.SelectedCharacter.SetActive(true);
        while (!DistanceConfirmed) yield return null;

        walkButton.SetActive(false);
        confirmButton.SetActive(false);
        base.Finish();
    }
}
