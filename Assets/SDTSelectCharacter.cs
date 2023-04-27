using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using QuickVR;

public class SDTSelectCharacter : QuickStageBase
{
    public GameObject EntAvatar;
    public GameObject RobotAvatar;
    public GameObject MaleAvatar;
    public GameObject FemaleAvatar;

    public GameObject SelectedCharacter;
    
    
    [SerializeField]
    private int TimesEnt = 0;
    [SerializeField]
    private int TimesRobot = 0;
    [SerializeField]
    private int TimesMale = 0;
    [SerializeField]
    private int TimesFemale = 0;

    private int selected = 0;
    private int lastSelected = 0;

    private bool validSelected = false;

    protected override void Start()
    {
        base.Start();
    }

    protected override IEnumerator CoUpdate()
    {
        while (!validSelected)
        {
           selected = Random.Range(1, 5); //Max Excluisve
           if((selected == 1) && (TimesEnt < 3) && (lastSelected != 1))
           {
                lastSelected = 1;
                validSelected = true;
           }
           if ((selected == 2) && (TimesRobot < 3) && (lastSelected != 2))
           {
               lastSelected = 2;
               validSelected = true;
           }
           if ((selected == 3) && (TimesMale < 3) && (lastSelected != 3))
           {
               lastSelected = 3;
               validSelected = true;
           }
            if ((selected == 4) && (TimesFemale < 3) && (lastSelected != 4))
            {
                lastSelected = 4;
                validSelected = true;
            }
        }
            

        if (validSelected) {
            switch (selected)
            {
                case 1:
                    TimesEnt++;
                    SelectedCharacter = EntAvatar;
                    break;
                case 2:
                    TimesRobot++;
                    SelectedCharacter = RobotAvatar;
                    break;
                case 3:
                    TimesMale++;
                    SelectedCharacter = MaleAvatar;
                    break;
                case 4:
                    TimesFemale++;
                    SelectedCharacter = FemaleAvatar;
                    break;
                default:
                    break;
            }
            validSelected = false;
        }
        SelectedCharacter.SetActive(true);
        return base.CoUpdate();
    }
}
