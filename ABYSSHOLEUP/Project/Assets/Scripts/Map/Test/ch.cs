using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ch : MonoBehaviour
{
    public static bool a;
    // Start is called before the first frame update
    void Start()
    {
        a = false;
    }

    // Update is called once per frame
    void Update()
    {
     if(Input.GetMouseButtonDown(1))
        {
            a = !a;
        }   
    }
}
