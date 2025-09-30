using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using UnityEngine.EventSystems;

/// <summary>
/// ’S“–:ŒF’J
/// </summary>

public class MyNumberSet : EventSet, IPointerDownHandler ,IPointerUpHandler,IPointerEnterHandler
{
    SelectHand sh;
    GameObject backGround;
    SpriteRenderer sr;
    Collider2D collider;
    GameObject selectedMap;
    [SerializeField] private GameObject trash;
    Trash tr;
    public GameObject SelectedMap { get; } 
    
    void Awake()
    {
        backGround = GameObject.Find("BackGround");
        sh = backGround.GetComponent<SelectHand>();
        collider = GetComponent<Collider2D>();
        tr=trash.GetComponent<Trash>();
    }
    void OnEnable()
    {
        
        collider.enabled = true;
        sh.SelectNumber = -1;
        selectedMap = null;
    }
    private void Update()
    {
       
        if(selectedMap!=null&&!sh.PutedMap) {
           Vector3 mousePosition = Input.mousePosition; 
            mousePosition = Camera.main.ScreenToWorldPoint(mousePosition);
            selectedMap.transform.position = new Vector3(mousePosition.x, mousePosition.y, 0);

        }
    
        if(sh.SelectNumber<0)
        {
            collider.enabled = true;
        }
        else
        {
            collider.enabled = false;
        }
        if(selectedMap!=null&&sh.SelectNumber<0)
        {
            selectedMap = null;
            sh.PutedMap = false;
        }
        this.gameObject.SetActive(PlayerManager.state == PlayerManager.PlayerState.MapCreate);
    }
    

    public void OnPointerDown(PointerEventData eventData)
    {
        if(PlayerManager.state==PlayerManager.PlayerState.PlayerMove)
        {
            sh.SelectedMap = null;  
            return;
        }
        if (eventData.pointerId==-1&&selectedMap==null&&this.gameObject.transform.childCount>0)
        {
            sh.PutedMap = false;

            selectedMap = Instantiate(this.gameObject,transform.position,Quaternion.identity);
            selectedMap.transform.parent = transform.parent.transform;
            sh.SelectedMap= selectedMap;
            sh.SelectNumber = int.Parse(this.name);
            selectedMap.GetComponent<Collider2D>().enabled = false; 
            selectedMap.layer = 2;
        }
        if (eventData.pointerId == -2)
        {
            if (sh.HandNumber.Count > int.Parse(this.gameObject.name))
            {
                sh.HandNumber.RemoveAt(int.Parse(this.gameObject.name));
                Debug.Log(int.Parse(this.gameObject.name));
                sh.SelectNumber = -1;
                sh.PutedMap = true;
                sh.HandMake();
                Destroy(sh.SelectedMap);
            }
        }
    }
        

    private IEnumerator delay()
    {
        yield return new WaitForSeconds(0.1f);
        collider.enabled = false;

    }

    public void OnPointerUp(PointerEventData eventData)
    {
        if(eventData.pointerId==-1)
        {
            //sh.PutedMap = true;
        }
         
    //    selectedMap = null;
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        if (sh.HandNumber.Count<=int.Parse(this.gameObject.name))
        {
            return;
        }
        List<int> tmpTrList = tr.TrashList;
        for (int i=0;i<tr.TrashList.Count;i++)
        {
            if (tmpTrList[i] == int.Parse(this.gameObject.name))
            {
                return;
            }
        }
        if(tr.Trashing)
        {
            tmpTrList.Add(int.Parse(this.gameObject.name));
            tr.TrashList=tmpTrList;
        }
         
    }
}
