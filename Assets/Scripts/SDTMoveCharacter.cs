using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class SDTMoveCharacter : QuickStageBase
{

    public GameObject WalkButton;
    public GameObject ConfirmButton;

    public bool DistanceConfirmed;
    protected override void Start()
    {
        base.Start();
    }
    protected override IEnumerator CoUpdate()
    {
        WalkButton.SetActive(true);
        ConfirmButton.SetActive(true);
        //CharacterSelection.SelectedCharacter.SetActive(true);
        while (!DistanceConfirmed) yield return null;

        //WalkButton.SetActive(false);
        //ConfirmButton.SetActive(false);
        base.Finish();
    }


}
