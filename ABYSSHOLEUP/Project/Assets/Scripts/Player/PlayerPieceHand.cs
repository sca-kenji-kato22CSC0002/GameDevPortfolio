using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerPieceHand : MonoBehaviour
{
    [SerializeField] private List<int> hand;
    [SerializeField] private GameObject mapCreator;
    // Start is called before the first frame update
    void Start()
    {
        hand = new List<int>();
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Space))
        {
            
        }
    }

    void GetPiece()
    {
        MapCreate MC=mapCreator.GetComponent<MapCreate>();
        if(MC.HandSize>hand.Count)
        {
            return;       
        }
    }

}
