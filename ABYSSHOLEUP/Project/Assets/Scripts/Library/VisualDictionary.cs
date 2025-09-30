using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class VisualDictionary : MonoBehaviour
{
    public GameObject pageCanvas;
    public GameObject detailsCanvas;
    public GameObject backButton;
    public GameObject nextButton;
    public GameObject[] oreSelectButton;
    public Image[] oreImage;
    public Sprite[] oreSprite;
    public Text[] oreNameText;
    public Text[] oreNumberText;
    public Text NowPageText;

    const int page_Max = 2;
    const int page_Min = 1;
    int page_Now = 1;

    int oreMax = 5;

    public GameObject yakiimo;

    void Start()
    {
        pageCanvas.SetActive(true);
        detailsCanvas.SetActive(false);

        page_Now = 1;
        pageUpdate();
    }

    void Update()
    {
        float sin = Mathf.Sin(Time.time);
        yakiimo.transform.position += new Vector3(0, sin/8, 0); 
    }

    /// <summary>
    /// 次のページに移動
    /// </summary>
    public void goNextPage()
    {
        if(page_Now == page_Max)
        {
            page_Now = page_Min;
        }
        else
        {
            page_Now++;
        }
        pageUpdate();
    }
    /// <summary>
    /// 前のページに移動
    /// </summary>
    public void goBackPage()
    {
        if(page_Now == page_Min)
        {
            page_Now = page_Max;
        }
        else
        {
            page_Now--;
        }
        pageUpdate();
    }

    /// <summary>
    /// 鉱石ボタンを押したときの処理
    /// </summary>
    public void pushOreButton_1()
    {
        detailsCanvas.SetActive(true);
        Details.openDetails((page_Now * 3) - 2);
    }
    public void pushOreButton_2()
    {
        Details.openDetails((page_Now * 3) - 1);
        detailsCanvas.SetActive(true);
    }
    public void pushOreButton_3()
    {
        Details.openDetails(page_Now * 3);
        detailsCanvas.SetActive(true);
    }

    /// <summary>
    /// ページの更新
    /// </summary>
    public void pageUpdate()
    {
        NowPageText.text = page_Now + " / " + page_Max;

        if(page_Now == page_Min)
        {
            oreSelectButton[2].SetActive(true);
            //backButton.SetActive(false);
           // nextButton.SetActive(true);

            oreNumberText[0].text = "No." + ((page_Now * 3) - 2).ToString("D3");
            oreNumberText[1].text = "No." + ((page_Now * 3) - 1).ToString("D3");
            oreNumberText[2].text = "No." + (page_Now * 3).ToString("D3");
        }
        else if(page_Now == page_Max)
        {
            oreSelectButton[2].SetActive(false);
            // backButton.SetActive(true);
            //  nextButton.SetActive(false);

            oreNumberText[0].text = "No." + ((page_Now * 3) - 2).ToString("D3");
            oreNumberText[1].text = "No." + ((page_Now * 3) - 1).ToString("D3");
            oreNumberText[2].text = "No." + (page_Now * 3).ToString("D3");
        }
        else
        {
            oreSelectButton[2].SetActive(true);
            //  backButton.SetActive(true);
            // nextButton.SetActive(true);

            oreNumberText[0].text = "No." + ((page_Now * 3) - 2).ToString("D3");
            oreNumberText[1].text = "No." + ((page_Now * 3) - 1).ToString("D3");
            oreNumberText[2].text = "No." + (page_Now * 3).ToString("D3");
        }
    }

    public void GoTitle()
    {
        SceneManager.LoadScene("Title");
    }
}
