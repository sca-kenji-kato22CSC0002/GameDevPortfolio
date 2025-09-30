using System.Collections;
using System.Collections.Generic;
using Unity.IO.LowLevel.Unsafe;
using UnityEngine;

/// <summary>
/// ’S“–:ŒF’J
/// </summary>

public class OreData : MonoBehaviour
{
    [SerializeField] private string[] names;
    [SerializeField] private Sprite[] sprite;
    public struct EventPercentage
    {
        public  List<int> oreOne;
        [SerializeField] private int[] oreTwo;
        [SerializeField] private int[] oreThree;
        [SerializeField] private int[] oreFour;
       
    }
    EventPercentage percentage;
    public string[] GetNames { get { return names; } }
    public Sprite[] GetSprite { get { return sprite;} }
    public EventPercentage GetEventPercentages { get { return percentage; } }


    private void Awake()
    {
        percentage = new EventPercentage();
        percentage.oreOne = new List<int> { 10, 0, 0 };
    }
}
