using System.Collections;
using System.Collections.Generic;
using UnityEditor.SearchService;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

/// <summary>
/// リザルト画面の処理全般
/// </summary>
public class Result : MonoBehaviour
{

    [SerializeField] Text timeText;
    [SerializeField] Text oreText;
    [SerializeField] Text totalText;

    [SerializeField] GameObject meterMoveObj;
    Vector3 meterStartPos = new Vector3(25, 90, 0);
    Vector3 meterEndPos = new Vector3(25, 990, 0);
    bool meterStart = false;
    int meterCount = 0;
    int meterTargetCount;
    [SerializeField]int meterMoveSpeed = 10;

    float timeScore;
    float timeDisplayScore;
    float oreScore;
    float oreDisplayScore;
    float totalScore;
    float totalDisplayScore;
    float countUpTime = 1;

    float timer;
    enum ResultState
    {
        time,
        ore,
        tortal,
        end
    }
    ResultState resultState;

    void Start()
    {
        resultState = ResultState.time;
        
        timeDisplayScore = 0;
        oreDisplayScore = 0;
        totalDisplayScore = 0;
        timer = 0;
        meterMoveObj.transform.position = meterStartPos;

        //スコア受け取り
        totalScore = PlayerManager.totalScore;
        timeScore = PlayerManager.time;
        oreScore = PlayerManager.oreCount;
        meterTargetCount = (int)totalScore / 20000;
        if (meterTargetCount >= 100)
        {
            meterTargetCount = 100;
        }

#if false     //デバッグ用
        timeScore = 300.0f;
        oreScore = 1234567;
        totalScore = 1234567;
        meterTargetCount = (int)totalScore / 20000;
        if(meterTargetCount >= 100)
        {
            meterTargetCount = 100;
        }
#endif
    }

    void Update()
    {
        switch (resultState)
        {
            case ResultState.time:
                if(timeScore > timeDisplayScore && !Input.GetMouseButtonDown(0))
                {
                    timeDisplayScore += timeScore * Time.deltaTime / countUpTime;
                    timeText.text = "  " + timeDisplayScore.ToString("f1") + "  秒";
                }
                else
                {
                    timeDisplayScore = timeScore;
                    timeText.text = "  " + timeDisplayScore.ToString("f1") + "  秒";
                    resultState = ResultState.ore;
                }
                break;
            case ResultState.ore:
                if (oreScore > oreDisplayScore && !Input.GetMouseButtonDown(0))
                {
                    oreDisplayScore += oreScore * Time.deltaTime / countUpTime;
                    oreText.text = "  " + oreDisplayScore.ToString("f0") + "  個";
                }
                else
                {
                    oreDisplayScore = oreScore;
                    oreText.text = "  " + oreDisplayScore.ToString("f0") + "  個";
                    resultState = ResultState.tortal;

                    meterStart = true;
                }
                break;
            case ResultState.tortal:
                if (totalScore > totalDisplayScore && !Input.GetMouseButtonDown(0))
                {
                    totalDisplayScore += totalScore * Time.deltaTime / countUpTime;
                    totalText.text = "" + totalDisplayScore.ToString("f0");
                }
                else
                {
                    totalDisplayScore = totalScore;
                    totalText.text = "" + totalDisplayScore.ToString("f0");
                    resultState = ResultState.end;
                }
                break;
            case ResultState.end:
                timer += Time.deltaTime;
                if (Input.GetMouseButtonDown(0) || timer > 5)
                {
                    SceneManager.LoadScene("Title");
                }
                break;
            default:
                resultState = ResultState.end;
                break;
        }

        MeterMove();
    }

    /// <summary>
    /// メーターの動き
    /// </summary>
    void MeterMove()
    {
        if(meterCount != meterTargetCount * meterMoveSpeed && meterStart)
        {
            meterMoveObj.transform.position = new Vector3(meterMoveObj.transform.position.x,
                                                          meterMoveObj.transform.position.y + (((meterEndPos.y - meterStartPos.y) / 100) / meterMoveSpeed),
                                                          0) ;
            meterCount++;
        }
    }
}
