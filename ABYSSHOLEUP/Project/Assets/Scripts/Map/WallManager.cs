using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using UnityEngine;

public class WallManager : MonoBehaviour
{
    Collider2D collider;
    GameObject player;
    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.Find("Player");
        collider=this.GetComponent<Collider2D>();
    }

    // Update is called once per frame
    void Update()
    {
        float dist = (player.transform.position - this.transform.position).magnitude;
        collider.enabled = (dist < 1);
    }
}
