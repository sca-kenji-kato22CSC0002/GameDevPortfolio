using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlueRippleParent : MonoBehaviour
{
    [SerializeField] GameObject prefab;
    float timer;
    [SerializeField] float intervalTime = 1.0f;

    void Start()
    {
        timer = 0.0f;
    }

    void Update()
    {
        timer -= Time.deltaTime;

        if(timer <= 0.0f)
        {
            Instantiate(prefab, this.gameObject.transform.position, Quaternion.identity, this.gameObject.transform);
            timer = intervalTime;
        }
    }
}
