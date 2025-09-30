using System.Collections;
using System.Collections.Generic;
using System.Linq.Expressions;
using UnityEngine;
/// <summary>
/// 担当:熊谷
/// </summary>

public class Ore : MonoBehaviour 
{
    private GameObject player;
    private GameObject comboManager;
    private ComboManager cm;
    private Rigidbody2D playerRb;
    private GameObject oreManager;
    private SelectHand sh;
    private MapCreate mc;
    private PlayerManager pm;
    private OreData oreData =new OreData();
    public delegate void OreEvent();
    private const string playerTag = "Player";
    private short hitCount;
    private float hitTimer;
    private bool hitPlayer;
    private const float validVelocity=3.0f;
    private float playerPower = 0;
    private WitchSpeak ws;

    [SerializeField] private GameObject[] gradationObjects;
    [SerializeField]private GameObject magma;
     private float[] eventTimer=new float[4];
    //鉱石の持つ情報
    public struct OreInfo
    {
        public string name;//名前
        public int number;//番号
        public int durability;
        public Sprite sprite;//見た目
        public List<OreEvent> events;//割り当てられる効果
        public List<int> eventPercentage;//各効果の発生割合
        public int addScore;

    }
    private const int score = 500;
    private const float scoreRate = 10;
    private int totalScore;
    private float colorRate;

    public int TotalScore { get { return totalScore; } }
    OreInfo info = new OreInfo();
    public OreInfo Info { get { return info; } set { info=value; } }

    private void Awake()
    {
        player = GameObject.Find(playerTag);
        GameObject witchSpeaker = GameObject.Find("WitchSpeaker");
        ws=witchSpeaker.GetComponent<WitchSpeak>();
        pm=player.GetComponent<PlayerManager>();    
        comboManager=GameObject.Find("ComboManager");
        cm=comboManager.GetComponent<ComboManager>();
        Debug.Log(GameObject.Find("Main Camera"));
        sh=GameObject.Find("Main Camera").transform.GetChild(0).gameObject.transform.GetChild(0).GetComponent<SelectHand>();
        playerRb=player.GetComponent<Rigidbody2D>();
        oreManager = GameObject.Find("OreManager");
        oreData=oreManager.GetComponent<OreData>();
        info.sprite = oreData.GetSprite[info.number];
        info.name = oreData.GetNames[0];
        OreData.EventPercentage ep = oreData.GetEventPercentages;
        info.eventPercentage = ep.oreOne;
        hitCount = 0;
        hitTimer = 0;
        hitPlayer = false;
        info.durability = 1;
        mc = GameObject.Find("MapCreator").GetComponent<MapCreate>();
        magma = GameObject.Find("Magma");
    }
    // Start is called before the first frame update
    void Start()
    {
        for(int i=0;i<gradationObjects.Length;i++)
        {
            gradationObjects[i].transform.parent.transform.rotation =
            Quaternion.Euler(new Vector3(0, 0, -transform.rotation.z));  
        }
    }

    // Update is called once per frame
    void Update()
    {
        if(hitPlayer){ hitTimer += Time.deltaTime;}
        if (hitTimer > 0.5f) { hitPlayer = false; }
        DestroyMe();
        playerPower= Mathf.Abs(playerRb.velocity.x) + Mathf.Abs(playerRb.velocity.y);
        RedHot();
    }

    private void EventSet(OreEvent e)
    {
        info.events.Add(e);
    }

    private void RedHot()
    {
        float diff = Mathf.Abs(magma.transform.position.y - transform.position.y);
        const int pieceBlocks = 13;
        Vector3 gradationScrollPos = new Vector3(0, 60, 0);

        if (diff < pieceBlocks)
        {
            colorRate = 2.0f*(0.5f - diff / pieceBlocks);
            for(int i = 0; i < gradationObjects.Length; i++)
            {
                gradationObjects[i].transform.transform.localPosition = gradationScrollPos * colorRate;
            }
        }

    }

    private void DestroyMe()
    {
        if(hitCount>=info.durability)
        {
            GetHand();
            PlayerManager.totalScore += (int)((1 + (colorRate * colorRate)) * scoreRate)*score;
            PlayerManager.oreCount++;
            int rand = Random.Range(0, 10);
            if (rand < 4)
            {
                TmpEventStart();
            }
            if(PlayerManager.oreCount>=10&&PlayerManager.oreCount%5==0&&PlayerManager.oreCount<36)
            {
                ws.AddString = ws.FixedLine[ws.CallFixedLineCount];
                ws.CallFixedLineCount++;
            }
            if(Random.Range(1,101)<=20&&ws.CanAddDendrogram)
            {
                if (Random.Range(1, 101) <= 3 && !ws.SpesialLineFlag)
                {
                    ws.SpesialLineFlag = true;
                    ws.AddString = ws.SpesialLine;
                }
                else if (ws.Dendrogram.Count > ws.NowLine)
                {
                    Debug.Log(ws.NowLine * 2);
                    ws.AddString = ws.Dendrogram[ws.NowLine];
                    ws.NowLineCount = ws.NowLineCount + 1;
                    if (ws.NowLine < 3)
                    {
                        ws.NowLine += 2;
                    }
                    else
                    {
                        ws.NowLine = ws.NowLine * 2 - Random.Range(1, 3);
                    }
                }
            }
           
           
           
            cm.ComboCount++;
            cm.ComboTimer = 0.0f;
            cm.ComboFlag = true;
            Destroy(this.gameObject);
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if(!collision.gameObject.CompareTag(playerTag)||hitPlayer)
        {
            return;
        }
       
        if(playerPower>=validVelocity)
        {
            hitPlayer = true;
            hitTimer=0;
            hitCount++;
        }
    }

    private void GetHand()//鉱石をはかいしたらマップを獲得
    {
        int size = sh.HandNumber.Count;
        const int maxHand = 8;
        if(size<maxHand)//現在所持しているマップの数が手札上限より少なければ
        {
            int getHandNumber = Random.Range(0, 62);//仮。ここの数字は後でマップ総量に変更
            sh.HandNumber.Add(getHandNumber);
        }
    }

    void TmpEventStart()
    {
        int rand = Random.Range(0, 4);

        switch (rand)
        {
            case 0:
                {
                    PlayerMove.coolTimeDown = true;
                    PlayerMove.coolTimeUp = false;
                    PlayerMove.chargeMax = false;
                    PlayerMove.speedUp = false;
                }
                break;
            case 1:
                {
                    PlayerMove.coolTimeDown = false;
                    PlayerMove.coolTimeUp = false;
                    PlayerMove.chargeMax = false;
                    PlayerMove.speedUp = false;
                }
                break;
            case 2:
                {
                    PlayerMove.coolTimeDown = false;
                    PlayerMove.coolTimeUp = false;
                    PlayerMove.chargeMax = true;
                    PlayerMove.speedUp = false;
                }
                break;
            case 3:
                {
                    PlayerMove.coolTimeDown = false;
                    PlayerMove.coolTimeUp = false;
                    PlayerMove.chargeMax = false;
                    PlayerMove.speedUp = true;
                }
                break;
        }
    }
    
}
