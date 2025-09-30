using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackGroundScroller : MonoBehaviour
{
    [SerializeField]
    private float scrollSpeed;
    public float ScrollSpeed { get { return scrollSpeed; } }

    // Update is called once per frame
    void Update()
    {
        transform.position = transform.position + 
            new Vector3(0, scrollSpeed) * Time.deltaTime;
        if (transform.localPosition.y > 1080.0f)
        {
            Vector3 pos = transform.localPosition;
            pos.y -= 1080.0f;
            transform.localPosition = pos;
        }
    }
}
