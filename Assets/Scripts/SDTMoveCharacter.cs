using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class SDTMoveCharacter : QuickStageBase
{

    public GameObject WalkButton;
    public GameObject ConfirmButton;
    public SDTSelectCharacter CharacterSelection; 
    public bool DistanceConfirmed;
    protected override void Start()
    {
        base.Start();
    }
    protected override IEnumerator CoUpdate()
    {
        WalkButton.SetActive(true);
        ConfirmButton.SetActive(true);
        WalkButton.GetComponent<ButtonAnimation>().CanWalk = true; 
        ConfirmButton.GetComponent<ButtonAnimation>().CanConfirm = true; 

        while (!DistanceConfirmed) yield return null;

        //WalkButton.SetActive(false);
        //ConfirmButton.SetActive(false);
        WalkButton.GetComponent<ButtonAnimation>().CanWalk = false;
        ConfirmButton.GetComponent<ButtonAnimation>().CanConfirm = false;
        base.Finish();
    }


}
