using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ScoreCounter : MonoBehaviour
{
    [SerializeField]
    private GameObject[] scoreDigits;
    [SerializeField]
    private float rotationTime;
    [SerializeField]
    private int nowScore;
    private float scoreCounter;
    private int previousScore;

    public Text score;
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (nowScore == (int)scoreCounter)
        {
            previousScore=nowScore;
        }
        else
        {
            int difference = nowScore - previousScore;
            scoreCounter += difference * Time.deltaTime / rotationTime;

            if(difference > 0)
            {
                if(scoreCounter > nowScore)
                {
                    scoreCounter=nowScore;
                }
            }
            else
            {
                if (scoreCounter < nowScore)
                {
                    scoreCounter = nowScore;
                }
            }

            score.text = ((int)scoreCounter).ToString();
        }
    }
}
