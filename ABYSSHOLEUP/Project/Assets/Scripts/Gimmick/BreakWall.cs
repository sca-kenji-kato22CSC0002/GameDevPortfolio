using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BreakWall : MonoBehaviour
{
    [SerializeField]Collider2D col;
    SpriteRenderer spriteRenderer;

    GameObject player;
    Rigidbody2D rb;
    float playerPower;


    void Start()
    {
        col = this.gameObject.GetComponent<Collider2D>();
        spriteRenderer = this.gameObject.GetComponent<SpriteRenderer>();

        player = GameObject.Find("Player");
        rb = player.GetComponent<Rigidbody2D>();
    }

    void Update()
    {
        playerPower = Mathf.Abs(rb.velocity.x) + Mathf.Abs(rb.velocity.y);
    }

    public void OnCollisionEnter2D(Collision2D collision)
    {
        if(collision.gameObject.tag == "Player")
        {
            if(playerPower >= 3.0f)
            {
                col.enabled = false;
                spriteRenderer.color = new Color(0, 0, 0, 0);
                Destroy(this.gameObject);
            }
        }
    }
}
