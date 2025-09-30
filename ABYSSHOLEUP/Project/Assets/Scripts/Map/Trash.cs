using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;
using UnityEngine.EventSystems;

public class Trash : MonoBehaviour, IPointerDownHandler
{
    [SerializeField] GameObject selectHand;
    SelectHand sh;
    private bool trashing;
    public bool Trashing {  get { return trashing; }}

    [SerializeField]private List<int> trashList = new List<int>();
    public List<int> TrashList { get { return trashList; } set { trashList=value; } }
    // Start is called before the first frame update
    void Start()
    {
        sh=selectHand.GetComponent<SelectHand>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void OnPointerUp()
    {
        //if(!sh.PutedMap&&PlayerManager.state==PlayerManager.PlayerState.MapCreate)
        //{
        //    if(sh.HandNumber.Count>0&&sh.SelectNumber!=-1)
        //    {
        //        sh.HandNumber.RemoveAt(sh.SelectNumber);
        //    }

        //    sh.SelectNumber = -1;
        //    sh.PutedMap = true;
        //    sh.HandMake();
        //    Destroy(sh.SelectedMap);
        //}
        trashList.Sort();
        trashList.Reverse();            int trashListCount=trashList.Count; 

        for(int i=0;i<trashListCount; i++) 
        {
            sh.HandNumber.RemoveAt(trashList[i]);
        }
        trashList.Clear();
        sh.DestroyLastHands();
        sh.HandMake();

    }

    public void OnPointerDown(PointerEventData eventData)
    {
        trashing = true;
    }

}
