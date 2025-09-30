using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

/// <summary>
/// プレイヤーがギミックに当たった時の処理
/// </summary>
public class PlayerHitGimmick : MonoBehaviour
{
    [SerializeField] float ripplePower = 0.5f;
    Rigidbody2D rg;

    void Start()
    {
        rg = this.gameObject.GetComponent<Rigidbody2D>();
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if(collision.gameObject.tag == "Adsorption")
        {
            rg.velocity = Vector2.zero;
        }
    }

    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "RippleRed")
        {
            Vector2 v = (collision.gameObject.transform.position - this.transform.position) / 3.5f;
            rg.AddForce(v * ripplePower);
        }
        if (collision.gameObject.tag == "RippleBlue")
        {
            float power = 0.5f;
            Vector2 v = (this.transform.position - collision.gameObject.transform.position) / 3.5f;
            if(v.magnitude < 0.5f)
            {
                power = 2f;
            }
            else
            {
                power = 0.5f;
            }
            rg.AddForce(v * ripplePower * power);
        }
    }

}
