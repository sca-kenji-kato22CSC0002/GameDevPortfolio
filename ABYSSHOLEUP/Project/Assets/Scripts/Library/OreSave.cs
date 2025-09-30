using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class OreSave : MonoBehaviour
{

    public static void Save(int oreNumber)
    {
        string saveName = "Ore_" + oreNumber.ToString();
        PlayerPrefs.SetInt(saveName, 1);
    }

    public static bool Load(int oreNumber)
    {
        string saveName = "Ore_" + oreNumber.ToString();
        bool isGet = Convert.ToBoolean(PlayerPrefs.GetInt(saveName,0));

        return isGet;
    }
}
