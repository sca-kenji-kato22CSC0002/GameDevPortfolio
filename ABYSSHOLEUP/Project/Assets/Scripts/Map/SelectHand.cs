using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

/// <summary>
/// ’S“–:ŒF’J
/// </summary>

public class SelectHand : MonoBehaviour
{
    [SerializeField] private int selectNumber;//‘I‘ğ‚µ‚½èD@èD‚Ì‚Ç‚ê‚ğ¶¬‚·‚é‚©
    [SerializeField] private List<int> handsNumber = new List<int>();//‚Á‚Ä‚¢‚éèD@‘I‘ğ‚µ‚½èD‚ÍCSV‚Ì‰½”Ô–Ú‚Ìƒf[ƒ^‚È‚Ì‚©
    [SerializeField] private GameObject[] hands = new GameObject[8];
    [SerializeField] private GameObject MapCreator;
    [SerializeField] private GameObject handsWindow;
    [SerializeField] private GameObject[] handObjects;
    public List<int> HandNumber { get { return handsNumber; } set { handsNumber = value; } }
    private const float size = 1f;
    MapCreate MC;
    SpriteRenderer sr;
    public int SelectNumber {  get { return selectNumber; } set {  selectNumber = value; } }

    private bool onemore;
    public bool Onemore { get { return onemore; } set {  onemore = value; } }

    private GameObject selectedMap;
    public GameObject SelectedMap { get {  return selectedMap; } set {  selectedMap = value; } }

    private bool putedMap;
    public bool PutedMap { get { return putedMap; } set { putedMap = value; } } 
    private void OnEnable()
    {
        for (int i = 0; i < handObjects.Length; i++)
        {
            sr = handObjects[i].GetComponent<SpriteRenderer>();
            sr.color = new Color(sr.color.r, sr.color.g, sr.color.b, 1.0f);
        }
        onemore = true;
        DestroyLastHands();
        MC = MapCreator.GetComponent<MapCreate>();
        selectNumber = -1;
        handsWindow.transform.localScale = new Vector3(1.0f, 1.0f, 1.0f);
        HandMake();
        handsWindow.transform.localScale = new Vector3(0.4f, 0.4f, 0.4f);
    }
    private void Update()
    {
        if(Input.GetMouseButtonDown(1))
        {
            selectNumber=-1;
            putedMap=true;
            Destroy(selectedMap);
        }
        if(selectNumber>=0&&!putedMap)
        {
            if (handsNumber.Count == 0)
            {
                return;
            }
            for (int i = 0; i < handObjects.Length; i++)
            {
                sr = handObjects[i].GetComponent<SpriteRenderer>();
                sr.color = new Color(sr.color.r, sr.color.g, sr.color.b, 0.0f);
            }
            for (int i = 0; i < handObjects.Length; i++)
            {
                sr = handObjects[i].GetComponent<SpriteRenderer>();
                DestroyLastHands();
            }
            onemore = true;
        }
        else if(onemore)
        {
            for (int i = 0; i < handObjects.Length; i++)
            {
                sr = handObjects[i].GetComponent<SpriteRenderer>();
                sr.color = new Color(sr.color.r, sr.color.g, sr.color.b, 1.0f);
            }
            if(putedMap)
            {
                if (handsNumber.Count>selectNumber&&selectNumber>=0)
                {
                    handsNumber.RemoveAt(selectNumber);
                }
              
                DestroyLastHands();
                MC = MapCreator.GetComponent<MapCreate>();
               
                HandMake();
                
                selectNumber = -1;
            }
           
        }
    }

    public void HandMake()
    {
        handsWindow.transform.localScale = new Vector3(1.0f, 1.0f, 1.0f);
        for (int i=0;i<handsNumber.Count;i++)
        {
            MC.PieceCreator(MC.PieceData, handsNumber[i], size,hands[i].transform.position, hands[i],false);
        }
        handsWindow.transform.localScale = new Vector3(0.4f, 0.4f, 0.4f);
    }

    public void DestroyLastHands()
    {
        for(int i=0;i<hands.Length;i++)
        {
            for(int j =0;j<hands[i].transform.childCount;j++)
            {
                Destroy(hands[i].transform.GetChild(j).gameObject);
            }
        }
    }
}
