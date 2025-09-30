using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestTimeScale : MonoBehaviour
{
    private string[] worldSpeedUpStr = { "b", "a", "i", "s", "o","k","u" };
    [SerializeField]private int inputed;
    [SerializeField]private int speed;
    private bool setSpeed;
    // Start is called before the first frame update
    void Start()
    {
        inputed = 0;
        speed = 1;
    }

    // Update is called once per frame
    void Update()
    {
        if(!setSpeed)
        {
            if (PlayerManager.state == PlayerManager.PlayerState.MapCreate)
            {
                Time.timeScale = 0.5f;
            }
            else
            {
                Time.timeScale = 1f;
            }
        }
       

        
        if(inputed==worldSpeedUpStr.Length)
        {
            for(int i=1;i<5;i++)
            {
                if (Input.GetKeyDown((KeyCode)(i + 48)))
                {
                    Debug.Log("Debug");
                    speed = i;
                }
            }
        }
        else if(!(inputed==worldSpeedUpStr.Length))
        {
            if(Input.GetKeyDown(worldSpeedUpStr[inputed]))
            {
                inputed++;
            }
            
        }
        if (Input.GetKeyDown(KeyCode.Return))
        {
            setSpeed = true;
            Time.timeScale = speed;
        }
       
    }
}
