using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NoEntry : MonoBehaviour
{
    private GameObject WitchSpeaker;
    WitchSpeak ws;
    private float timer=1;
    // Start is called before the first frame update
    void Start()
    {
        WitchSpeaker=GameObject.Find("WitchSpeaker").gameObject;
        ws=WitchSpeaker.GetComponent<WitchSpeak>();
        Debug.Log(ws);
    }

    // Update is called once per frame
    void Update()
    {
      timer-=Time.deltaTime;
    }

    void OnTriggerEnter2D(Collider2D coll)
    {
        if(timer<0)
        {
            ws.NoEntryCount = ws.NoEntryCount + 1;
            ws.NewNoEntry = true;
            Debug.Log("N“ü‹ÖŽ~ƒGƒŠƒA‚ÉN“ü‚µ‚Ü‚µ‚½");
        }
       
        
    }
}
