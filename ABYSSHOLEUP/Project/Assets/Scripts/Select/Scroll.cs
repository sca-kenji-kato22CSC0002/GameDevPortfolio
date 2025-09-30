using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Scroll : MonoBehaviour
{
    enum State
    {
        FALL,
        DEFAULT,
        SELECT
    }

    private State state;
    [SerializeField] private float floatingInterval;
    [SerializeField] private float waitTime;
    [SerializeField] private float floatingCenter;
    [SerializeField] private float floatingHeight;
    [SerializeField] private float scaleLate;
    private float timer;
    // Start is called before the first frame update
    void Start()
    {
        timer = 0.0f;
        state = State.FALL;
        StartCoroutine(StartFall());
    }

    // Update is called once per frame
    void Update()
    {
        if (state != State.FALL)
        {
            timer += Time.deltaTime;
            if (state == State.DEFAULT)
            {
                transform.localPosition = new Vector3(transform.localPosition.x, Floating() + floatingCenter, 0);
                transform.localScale = Vector3.one;
            }
            else
            {
                transform.localPosition = new Vector3(transform.localPosition.x, floatingCenter, 0);
                transform.localScale = Vector3.one * scaleLate;
            }
        }
    }

    private IEnumerator StartFall()
    {
        bool isEnd = false;
        float t = 0.0f;
        float defPosX = transform.localPosition.x;
        float defPosY = transform.localPosition.y;
        yield return new WaitForSeconds(waitTime);
        while (!isEnd)
        {
            transform.localPosition = new Vector3(defPosX, defPosY + (floatingCenter - defPosY) * EaseOutCubic(t), 0);
            t += Time.deltaTime;
            yield return null;
            if (t >= 1.0f) isEnd = true;
        }
        state = State.DEFAULT;
    }

    private float EaseOutCubic(float t)
    {
        float c = 1.0f - t;

        return t <= 1.0f ? 1 - c * c * c : 1.0f;
    }

    private float Floating()
    {
        float ret = Mathf.Sin(2 * Mathf.PI * timer / floatingInterval) * floatingHeight;
        return ret;
    }

    public void OnStateSelect()
    {
        if(state==State.DEFAULT)
        state = State.SELECT;
    }

    public void OnStateDefault()
    {
        if (state == State.SELECT)
            state = State.DEFAULT;
    }
}
