using UnityEngine;
using UnityEngine.EventSystems;

/// <summary>
/// ’S“–:ŒF’J
/// </summary>

public class DefaultTileManager : MonoBehaviour
{
    private GameObject mapCreator;
    private GameObject selectedMap;
    private SelectHand sh;
    MapCreate mc;
    private bool onCursor;
    private bool selectMap = false;
    private bool isFirst = false;
    private Sprite[] sprites;
    private SpriteRenderer sr;

    private void OnEnable()
    {
        mapCreator = GameObject.Find("MapCreator");
        mc = mapCreator.GetComponent<MapCreate>();
        sprites = mc.GridSprites;
    }

    private void Start()
    {
        sr=GetComponent<SpriteRenderer>();
        onCursor = false;
       
        sh = GameObject.Find("Main Camera").transform.GetChild(0).gameObject.transform.GetChild(0).gameObject.GetComponent<SelectHand>();
        mc.FirstPieceCreator(mc.PieceData, 100, 1, this.transform.position,this.gameObject);
        sr.sprite = sprites[0];
    }

    private void Update()
    {
        if(PlayerManager.state==PlayerManager.PlayerState.PlayerMove)
        {
            sr.color = new Color(1.0f, 1.0f, 1.0f, 0.4f);
        }
        else
        {
            sr.color = new Color(1.0f, 1.0f, 1.0f, 1.0f);
        }
        if (Input.GetMouseButtonUp(0)&&PlayerManager.state==PlayerManager.PlayerState.MapCreate&&onCursor&&sh.SelectedMap!=null)
        {
            mc.PieceCreator(mc.PieceData, sh.HandNumber[sh.SelectNumber], 1, this.transform.position);
            mc.SetPosition = this.transform.position;
            mc.MakeMap = true;
            selectedMap = sh.SelectedMap;
            var sMS = selectedMap.GetComponent<SpriteRenderer>();
            sMS.color=new Color(sMS.color.r, sMS.color.g, sMS.color.b,0.0f);
            
            Destroy(selectedMap);
            sh.Onemore = true;
            sh.PutedMap = true;
            sh.HandMake();
            selectedMap = null;
            Destroy(this.gameObject);
        }

    }
    public void OnPointerUp(PointerEventData eventData)
    {
        if(eventData.pointerId==-1&&PlayerManager.state==PlayerManager.PlayerState.MapCreate)
        {
            //mc.SetPosition = this.transform.position;
            //mc.MakeMap = true;
            //sh.SelectNumber = -1;
            //sh.Onemore = true;
            //selectedMap = sh.SelectedMap;
            //selectedMap.transform.position = this.transform.position;
            //selectedMap.transform.parent = null;
            //selectedMap.GetComponent<MyNumberSet>().enabled = false;
            //sh.PutedMap = true;
            //sh.HandNumber.Remove(sh.SelectNumber);
            //sh.HandMake();
            //selectedMap = null;
        }
      
    }

    public void OnPointerEnter()
    {
        onCursor = true;
        if(sh.SelectNumber>=0)
        {
            sr.sprite = sprites[1];
        }
       
    }
    public void OnPointerExit()
    {
        onCursor = false;
        sr.sprite = sprites[0];
    }

}
