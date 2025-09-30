using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class CircularGauge : MonoBehaviour
{
    public Image gaugeImage; // ゲージに使用するImageコンポーネント
    [Range(0, 1)] public float fillAmount = 1.0f; // 塗りつぶし率

    void Update()
    {
        if (gaugeImage != null)
        {
            gaugeImage.fillAmount = fillAmount;
        }
    }

    // 任意でゲージを設定するメソッド
    public void SetGauge(float value)
    {
        fillAmount = Mathf.Clamp01(value); // 0～1にクランプ
    }
}
