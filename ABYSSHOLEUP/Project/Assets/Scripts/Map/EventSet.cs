using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

/// <summary>
/// 担当:熊谷
/// </summary>
public class EventSet : MonoBehaviour
{
    public delegate void SetEvent();
    private string corectEventID;
    protected const string enter = "Enter";
    protected const string exit = "Exit";
    protected const string down = "Down";
    protected const string up = "Up";
    protected const string click  = "Click";
    public virtual void PointerEnter()
    {
        Debug.Log("基底クラス側の関数が呼ばれています\n継承先の関数をオーバーライドしてください");//基底クラス側の関数が呼び出されていることを知らせる
    }

    public virtual void PointerDown()
    {
        Debug.Log("基底クラス側の関数が呼ばれています\\n継承先の関数をオーバーライドしてください\"");
    }

    public virtual void PointerExit()
    {
        Debug.Log("基底クラス側の関数が呼ばれています\\n継承先の関数をオーバーライドしてください\"");
    }

    public virtual void SetEventType(string eventID, SetEvent e)
    {
        EventTrigger trigger = GetComponent<EventTrigger>();
        EventTrigger.Entry entry = new EventTrigger.Entry();

        switch (eventID)
        {
            case (enter):
                {
                    entry.eventID = EventTriggerType.PointerEnter;
                }
                break;
            case (down):
                {
                    entry.eventID = EventTriggerType.PointerDown;
                }
                break;
            case (exit):
                {
                    entry.eventID = EventTriggerType.PointerExit;
                }
                break;
            case (up):
                {
                    entry.eventID = EventTriggerType.PointerUp;
                }
                break;
        }

        entry.callback.AddListener((data) => { e(); });
        trigger.triggers.Add(entry);
    }
}
