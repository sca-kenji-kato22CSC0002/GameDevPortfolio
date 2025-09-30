using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

public class ButtonContentsMotion : MonoBehaviour
{
    [SerializeField] private GameObject[] contents;
    private enum ButtonMode
    {
        NONE=0,
        START,
        LIBRARY,
        OPTION,
        EXIT
    }
    [SerializeField] ButtonMode mode;
    private void OnEnable()
    {
        switch (mode)
        {
            case ButtonMode.START:
                StartCoroutine(StartMotion());
                break;

            case ButtonMode.LIBRARY:
                StartCoroutine(LibraryMotion());
                break;

            case ButtonMode.OPTION:
                StartCoroutine(OptionMotion());
                break;

            case ButtonMode.EXIT:
                StartCoroutine(ExitMotion());
                break;
        }
    }

    private IEnumerator BrinkStar()
    {
        float motionSpeed = 0.75f;
        float[] scalengTimer = new float[3] { 0.0f, 1.0f / 3, 2.0f / 3 };
        while (true)
        {
            for (int i = 0; i < 3; i++)
            {
                scalengTimer[i] += Time.deltaTime * motionSpeed;
                contents[i + 2].transform.localScale = Vector3.one * Mathf.Sin(Mathf.PI * scalengTimer[i]);
                if (scalengTimer[i] >= 1.0f)
                {
                    float x = Random.Range(-35.0f, 45.0f);
                    float y = Random.Range(-35.0f, 45.0f);
                    contents[i + 2].transform.localPosition = new Vector3(x, y, 0);
                    scalengTimer[i] -= 1.0f;
                }
            }
            yield return null;
        }
    }

    private IEnumerator StartMotion()
    {
        float t = 0.0f;
        float motionSpeed = 0.5f;

        Vector3 defRot = new Vector3(0, 0, 5.0f);
        Vector3 motRot = new Vector3(0, 0, 10.0f);
        Vector3 addRot = motRot - defRot;

        contents[0].transform.localRotation = Quaternion.Euler(defRot);

        yield return new WaitForSeconds(0.25f);

        while (true)
        {
            t += Time.deltaTime * motionSpeed;
            contents[0].transform.localRotation = Quaternion.Euler(defRot + addRot * Mathf.Sin(t * Mathf.PI));

            yield return null;
        }
    }

    private IEnumerator LibraryMotion()
    {
        bool isEnd = false;
        float t = 0.0f;

        Vector3 defPos = new Vector3(-375.0f, -40.0f, 0);
        Vector3 motPos = new Vector3(-390.0f, 0, 0);
        Vector3 addDist = motPos - defPos;
        float motionSpeed = 3.0f;

        Vector3 defScl = Vector3.zero;
        Vector3 motScl = Vector3.one;
        Vector3 addScl = motScl - defScl;
        contents[1].transform.localScale = Vector3.zero;

        Vector3 defRot = Vector3.zero;
        Vector3 motRot = new Vector3(0, 0, 8.0f);
        Vector3 addRot = motRot - defRot;

        float[] scalengTimer = new float[3] { 0.0f, 1.0f / 3, 2.0f / 3 };
        for (int i = 0; i < scalengTimer.Length; i++)
            contents[i + 2].transform.localScale = Vector3.one * scalengTimer[i];

        while (!isEnd)
        {
            t += Time.deltaTime * motionSpeed;
            float x = addDist.x * EaseOutCirc(t);
            float y = addDist.y * EaseOutBack(t);
            contents[0].transform.localPosition= defPos+ new Vector3(x,y);

            if (t >= 1.0f) isEnd = true;
            yield return null;
        }

        isEnd = false;
        t = 0.0f;
        motionSpeed = 3.0f;

        while (!isEnd)
        {
            t += Time.deltaTime * motionSpeed;
            float x = addScl.x * EaseInCubic(t);
            float y = addScl.y * EaseInOutCubic(t);
            contents[1].transform.localScale = defScl + new Vector3(x, y);

            if (t >= 1.0f) isEnd = true;
            yield return null;
        }

        StartCoroutine(BrinkStar());

        isEnd = false;
        t = 0.0f;
        motionSpeed = 4.0f;

        while (!isEnd)
        {
            t += Time.deltaTime * motionSpeed;
            contents[1].transform.localRotation = 
                Quaternion.Euler(defRot + addRot * Mathf.Sin(t * 2.0f * Mathf.PI));

            if (t >= 1.0f) isEnd = true;
            yield return null;
        }
        contents[1].transform.localRotation = Quaternion.Euler(defRot);

        yield return new WaitForSeconds(0.25f);

        t = 0.0f;
        defPos = new Vector3(-390.0f,0,0);
        motPos = new Vector3(-390.0f, 5.0f, 0);
        addDist = motPos - defPos;
        motionSpeed = 0.1f;

        while (true)
        {
            contents[5].transform.localPosition = defPos + addDist * Mathf.Sin(Mathf.Abs(t * 2.0f * Mathf.PI));
            t += Time.deltaTime * motionSpeed;
            yield return null;
        }
    }

    private IEnumerator OptionMotion()
    {
        bool isEnd = false;
        float t = 0.0f;

        Canvas handLayer = contents[2].transform.GetComponent<Canvas>();
        Vector3 defPos = new Vector3(-345, 0, 0);
        Vector3 motPos = new Vector3(-335, 0, 0);
        Vector3 addDist = motPos - defPos;
        const float motionSpeed = 3.0f;

        Vector3 defRot = new Vector3(0, 0, 30.0f);
        Vector3 motRot = new Vector3(0, 0, -5.0f);
        Vector3 addRot = motRot - defRot;

        contents[0].transform.localRotation = Quaternion.Euler(defRot);

        while (!isEnd)
        {
            contents[2].transform.localPosition = defPos + addDist * Mathf.Sin(t *  Mathf.PI);
            t += Time.deltaTime * motionSpeed;
            handLayer.sortingOrder = t <= 0.5f ? 1 : 3;
            if (t >= 1.0f) isEnd = true;
            yield return null;
        }

        isEnd = false;
        t = 0.0f;

        while (!isEnd)
        {
            contents[0].transform.localRotation = Quaternion.Euler(defRot + addRot * EaseInOutCubic(t));
            t += Time.deltaTime;
            if (t >= 1.0f) isEnd = true;
            yield return null;
        }

        while (true)
        {
            contents[0].transform.localRotation = Quaternion.Euler(motRot + Vector3.one * Mathf.Sin(t * Mathf.PI));
            t += Time.deltaTime;
            yield return null;
        }
    }

    private IEnumerator ExitMotion()
    {
        bool isEnd = false;
        float t = 0.0f;

        Vector3 defPos = new Vector3(-130, -70.0f, 0);
        Vector3 motPos = new Vector3(-130, -20.0f, 0);
        Vector3 addDist = motPos - defPos;
        Vector3 defRot_Body = new Vector3(0,0,-10.0f);
        Vector3 motRot_Body = Vector3.zero;
        Vector3 addRot_Body = motRot_Body - defRot_Body;

        Vector3 defRot_Hand =Vector3.zero;
        Vector3 motRot_Hand = new Vector3(0.0f, 0.0f, 15.0f);
        Vector3 addRot_Hand = motRot_Hand - defRot_Hand;

        contents[1].transform.localRotation = Quaternion.Euler(defRot_Hand);
        while (!isEnd)
        {
            contents[0].transform.localPosition = defPos + addDist * EaseOutBack(t);
            contents[0].transform.localRotation= Quaternion.Euler(defRot_Body+addRot_Body*EaseOutBack(t));
            if (t >= 1.0f) isEnd = true;
            yield return t <= 0.0f ? new WaitForSeconds(0.25f) : null;
            t += Time.deltaTime * 2;
        }

        isEnd = false;
        t = 0.0f;
        while (!isEnd)
        {
            contents[1].transform.localRotation = Quaternion.Euler(defRot_Hand + addRot_Hand * Mathf.Sin(t * Mathf.PI*3));
            t += Time.deltaTime;
            yield return null;
        }
    }

    /// <summary>
    /// èƒàÇ™èoÇƒÇ≠ÇÈÇ‚Ç¬
    /// </summary>
    /// <returns></returns>
    private float EaseOutBack(float t)
    {
        const float c1 = 1.70158f;
        float c2 = c1 + 1;
        float c3 = t - 1;

        return t <= 1.0f ? 1 + c2 * c3 * c3 * c3 + c1 * c3 * c3 : 1.0f;
    }

    /// <summary>
    /// ñÇèóÇ™èoÇƒÇ≠ÇÈÇ‚Ç¬Ç∆çzêŒÇÃägèkÅFÇô
    /// </summary>
    /// <returns></returns>
    private float EaseInOutCubic(float t)
    {
        float c = -2 * t + 2;

        return t > 1.0f ? 1.0f 
            : t < 0.5f ? 4 * t * t * t : 1 - (c * c * c) / 2;
    }

    /// <summary>
    /// ñ{ÇÃâ°à⁄ìÆ
    /// </summary>
    /// <returns></returns>
    private float EaseOutCirc(float t)
    {
        float c = t - 1;
        return t <= 1.0f ? Mathf.Sqrt(1 - c * c) : 1.0f;
    }

    /// <summary>
    /// çzêŒÇÃägèkÅFÇô
    /// </summary>
    /// <returns></returns>
    private float EaseOutQuad(float t)
    {
        float c = 1 - t;
        return t <= 1.0f ? 1 - c * c : 1.0f;
    }

    /// <summary>
    /// çzêŒÇÃägèkÅFÇò
    /// </summary>
    /// <returns></returns>
    private float EaseInCubic(float t)
    {
        return t <= 1.0f ? t * t : 1.0f;
    }

    private float EaseInQuint(float t)
    {
        return t * t * t * t * t;
    }

    private float EaseOutBounce(float t)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1.0f / d1)
        {
            return n1 * t * t;
        }
        else if (t < 2.0f / d1)
        {
            return n1 * (t -= 1.5f / d1) * t + 0.75f;
        }
        else if (t < 2.5f / d1)
        {
            return n1 * (t -= 2.25f / d1) * t + 0.9375f;
        }
        else
        {
            return t <= 1.0f ? n1 * (t -= 2.625f / d1) * t + 0.984375f : 1.0f;
        }
    }

    private float EaseInBounce(float t)
    {
        return t<= 1.0f ? 1 - EaseOutBounce(1 - t) : 1.0f;
    }
}
