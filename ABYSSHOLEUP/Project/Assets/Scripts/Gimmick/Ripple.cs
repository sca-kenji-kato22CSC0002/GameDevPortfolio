using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// 波紋のプログラム
/// 波紋　青は内側から外側に　赤は外側から内側に
/// stateはinspectorで設定する
/// </summary>
public class Ripple : MonoBehaviour
{
    enum State
    {
        Blue,
        Red,
    }
    [SerializeField] State state;

    SpriteRenderer spriteRenderer;
    float alpha;

    GameObject spriteObj;
    float scale;
    float scaleMax = 1.1f;
    float scaleMin = 0.0f;

    [SerializeField]float time;

    void Start()
    {
        spriteObj = this.gameObject;
        spriteRenderer = this.gameObject.GetComponent<SpriteRenderer>();
        if(state == State.Blue) 
        {
            alpha = 1.0f;
            scale = scaleMin;
        }
        else if (state == State.Red)
        {
            alpha = 0.0f;
            scale = scaleMax;
        }
        spriteObj.transform.localScale = new Vector3 (scale, scale, scale);
        spriteRenderer.color = new Color(1,1,1,alpha);

        time = 1.5f;
    }

    void Update()
    {
        if (state == State.Blue)
        {
            alpha -= Time.deltaTime / time;
            scale += (Time.deltaTime * scaleMax) / time;

            if(spriteObj.transform.localScale.x >= scaleMax)
            {
                Destroy(this.gameObject);
            }
        }
        else if (state == State.Red)
        {
            alpha += Time.deltaTime / time;
            scale -= (Time.deltaTime * scaleMax) / time;

            if (spriteObj.transform.localScale.x <= scaleMin)
            {
                Destroy(this.gameObject);
            }
        }

        spriteObj.transform.localScale = new Vector3(scale, scale, scale);
        spriteRenderer.color = new Color(1, 1, 1, alpha);
    }
}
