//担当者：畠山駿佑

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerTitleMotioner : MonoBehaviour
{
    private enum MotionState
    {
        FLOATING = 0,
        MOVE,
        HOP,
        SLEEP,
        SIZE
    }
    private MotionState motionState;
    private bool isMotionStarted;

    [SerializeField]
    private float floatHeight;
    [SerializeField]
    private float hoppingHeight;

    private GameObject parent;

    private int lastMotionNumber;

    private BackGroundScroller bgs;
    private float fallSpeed;

    Quaternion direntionRight = Quaternion.Euler(new Vector3(0, 180, 0));
    Quaternion directionLeft = Quaternion.Euler(new Vector3(0, 0, 0));

    [SerializeField]
    private GameObject[] eyes;

    [SerializeField]
    private GameObject[] ears;

    [SerializeField]
    private GameObject[] handParents;

    [SerializeField]
    private GameObject[] hands;

    [SerializeField]
    private GameObject[] wingParents;
    [SerializeField]
    private GameObject[] wings;

    [SerializeField]
    private GameObject[] tail;

    private float flappingTimer;
    private float flappingMagnitude;
    private float flappingSpeed;
    private float sleptTimer;
    [SerializeField]
    private float sleptInterval;
    [SerializeField]
    private float movementHeight;

    // Start is called before the first frame update
    void Start()
    {
        parent = transform.parent.gameObject;
        motionState = MotionState.FLOATING;
        lastMotionNumber = -1;
        isMotionStarted = false;
        bgs = GameObject.Find("BackGroundObject").GetComponent<BackGroundScroller>();
        fallSpeed = bgs.ScrollSpeed;
        flappingTimer = 0.0f;
        flappingMagnitude = 0.0f;
        flappingSpeed = 0.0f;
        sleptTimer = 0.0f;
    }

    //Update is called once per frame
    void Update()
    {
        Motion_Flapping(flappingMagnitude,flappingSpeed);
        parent.transform.localPosition =
            parent.transform.localPosition + new Vector3(0, fallSpeed) * Time.deltaTime;
        sleptTimer += Time.deltaTime;

        if (!isMotionStarted)
        {
            NextMotion();
            flappingMagnitude = 1.0f;
            flappingSpeed = 1.0f;
            switch (motionState)
            {
                case MotionState.FLOATING:
                    {
                        float rand = Random.Range(1, 5);
                        StartCoroutine(Motion_Floating(rand));
                    }
                    break;

                case MotionState.MOVE:
                    {
                        const float movementTimeMin = 1.0f;
                        const float movementTimeMax = 4.0f;
                        float randTime = Random.Range(movementTimeMin, movementTimeMax);

                        const float xMin = -800.0f;
                        const float xMax = 30.0f;
                        const float yMin = -600.0f;
                        const float yMax = 150.0f;
                        Vector3 randTarget = new Vector3(
                            Random.Range(xMin, xMax),
                            Random.Range(yMin, yMax));
                        StartCoroutine(Motion_Move(randTime,randTarget));
                    }
                    break;

                case MotionState.HOP:
                    {
                        const int hopsMin = 1;
                        const int hopsMax = 4;
                        int randHops = Random.Range(hopsMin, hopsMax);

                        const float hoppingTimeMin = 0.3f;
                        const float hoppingTimeMax = 0.6f;
                        float randTime = Random.Range(hoppingTimeMin, hoppingTimeMax);
                        StartCoroutine(Motion_Hop(randHops, randTime));
                    }
                    break;

                case MotionState.SLEEP:
                    {
                        //眠いので羽ばたきは弱くなります
                        flappingMagnitude = 0.75f;
                        flappingSpeed = 0.75f;
                        const float sleepTimeMin = 4.0f;
                        const float sleepTImeMax = 9.0f;
                        float rand = Random.Range(sleepTimeMin, sleepTImeMax);
                        StartCoroutine(Motion_Sleep(rand));
                    }
                    break;
            }
        }
    }

    /// <summary>
    /// 次のモーションを決定する関数。
    /// </summary>
    private void NextMotion()
    {

        if (parent.transform.localPosition.y >= movementHeight)
        {
            //高さが規定よりも上だったら次は必ず移動モーション
            motionState = MotionState.MOVE;
            lastMotionNumber = (int)MotionState.MOVE;
            return;
        }

        bool isEnd = false;
        int rand = 0;
        while (!isEnd)//想定の範囲内の結果になるまで繰り返す
        {
            rand = Random.Range(0, (int)MotionState.SIZE);

            //前回と同じモーションはキャンセル
            if (rand == lastMotionNumber) continue;
            //跳ねるモーションは頻度を減らしたいので1/3の確率でキャンセル
            if (rand == (int)MotionState.HOP && Random.Range(0, 100) >= 33) continue;
            //寝るモーションは前回が漂うモーション以外で、かつ規定の時間経過していなかったらキャンセル
            if (rand == (int)MotionState.SLEEP && (sleptTimer <= sleptInterval || lastMotionNumber != (int)MotionState.FLOATING)) continue;

            //全ての条件に引っかからなかったら終了
            isEnd = true;
        }
        motionState = (MotionState)rand;
        lastMotionNumber = rand;
    }

    /// <summary>
    /// 羽ばたきをする関数。
    /// 羽ばたきをさせたい間はずっとこの関数を呼び続ける。
    /// </summary>
    /// <param name="speed">羽ばたきの速度に掛かる値</param>
    /// <param name="magnitude">羽ばたきの幅に掛かる値</param>
    private void Motion_Flapping(float magnitude,float speed)
    {
        flappingTimer = 
            flappingTimer > 1.0f ?                   //タイマーを1.0と比較
            flappingTimer -= 1.0f :　　　　　　　　　//1.0より大きかったら1.0を引く
            flappingTimer += Time.deltaTime * speed;　//1.0より小さかったらdeltaTime*speedを足す

        //基本になる翼の角度を決定
        List<float> defRotZ = new List<float>
        {
            25.0f,
            -30.0f
        };
        //羽ばたきをした時の角度を決定
        List<float> motionRotZ = new List<float>
        {
            -15.0f,
            15.0f
        };

        for (int i = 0; i < defRotZ.Count; i++)
        {
            //基本の角度に対して最大でどれくらい動かすのか計算
            float z = motionRotZ[i] - defRotZ[i];
            //計算結果に揺動運動の時間軸を掛けて、基本の角度に足す
            wings[i].transform.localRotation =
                Quaternion.Euler(new Vector3(0, 0, defRotZ[i]) +
                new Vector3(0, 0, z) * (1.0f - EaseInReturn(flappingTimer)) * magnitude);
        }
    }

    /// <summary>
    /// 寝るモーションを実行するコルーチン。
    /// </summary>
    /// <param name="time">睡眠時間。モーション全体の時間はこれに2.25秒足した秒数。</param>
    /// <returns></returns>
    private IEnumerator Motion_Sleep(float time)
    {
        isMotionStarted = true;
        float t = 0.0f;
        bool isEnd = false;
        Vector3 defScale = eyes[0].transform.localScale;

        List<GameObject[]> bodyParts = new List<GameObject[]>
        {
            ears,
            handParents,
            hands,
            wingParents,
            tail
        };
        List<float> motionRotZ = new List<float>
        {
            //左右耳
            50.0f,
            -35.0f,
            //両肘
            16.0f,
            -30.0f,
            //両手
            12.5f,
            -30.0f,
            //両翼根本
            30.0f,
            -60.0f,
            //しっぽ
            -65.0f
        };

        List<float> defRotZ = new List<float>();
        for (int i = 0; i < bodyParts.Count; i++)
        {
            for (int j = 0; j < bodyParts[i].Length; j++)
            {
                defRotZ.Add(bodyParts[i][j].transform.localRotation.z);
            }
        }

        //頭の角度に使う変数
        Vector3 leaning = new Vector3(0, 0, 35.0f);

        //こくこくするところ
        for (int n = 0; n < 2; n++)
        {
            yield return new WaitForSeconds(0.5f);
            t = 0.0f;
            isEnd = false;

            //モーションの幅を小さくするのに使う変数
            float weakenRate = 0.25f;

            while (!isEnd)
            {
                if (t >= 1.0f) isEnd = true;

                t += Time.deltaTime;
                transform.localRotation = Quaternion.Euler(leaning * weakenRate * EaseInReturn(t));
                eyes[0].transform.localScale = new Vector3(defScale.x, defScale.y * (1 - EaseInReturn(t)));
                eyes[1].transform.localScale = new Vector3(defScale.x, defScale.y * (1 - EaseInReturn(t)));

                int count = 0;
                for (int i = 0; i < bodyParts.Count; i++)
                {
                    for (int j = 0; j < bodyParts[i].Length; j++)
                    {
                        bodyParts[i][j].transform.localRotation =
                            Quaternion.Euler(new Vector3(0, 0, motionRotZ[count] * weakenRate) * EaseInReturn(t));
                        count++;
                    }
                }
                yield return null;
            }
        }

        yield return new WaitForSeconds(0.5f);

        //寝る瞬間
        t = 0.0f;
        isEnd = false;

        List<Quaternion> defRots = new List<Quaternion>();
        for (int i = 0; i < bodyParts.Count; i++)
        {
            for (int j = 0; j < bodyParts[i].Length; j++)
            {
                defRots.Add(bodyParts[i][j].transform.localRotation);
            }
        }

        while (!isEnd)
        {
            if (t >= 1.0f) isEnd = true;

            t += Time.deltaTime;
            transform.localRotation = Quaternion.Euler(leaning * EaseInExpo(t));

            eyes[0].transform.localScale = new Vector3(defScale.x, defScale.y * (1 - EaseInExpo(t)));
            eyes[1].transform.localScale = new Vector3(defScale.x, defScale.y * (1 - EaseInExpo(t)));

            int count = 0;
            for (int i = 0; i < bodyParts.Count; i++)
            {
                for (int j = 0; j < bodyParts[i].Length; j++)
                {
                    bodyParts[i][j].transform.localRotation =
                        Quaternion.Euler(new Vector3(0, 0, motionRotZ[count]) * EaseInExpo(t));
                    count++;
                }
            }

            yield return null;
        }

        //寝てから落ちて行くモーション
        t = 0.0f;
        isEnd = false;

        while (!isEnd)
        {
            if (t >= 1.0f) isEnd = true;
            t += Time.deltaTime / time;

            parent.transform.localScale = new Vector3(1.0f + t / 3, 1.0f - t / 2, 1);
            fallSpeed = -bgs.ScrollSpeed * 1.5f * t;
            //magnitudeとspeedに使いたい値が同じ
            float ft = 0.5f - (0.4f * t);
            flappingMagnitude = ft;
            flappingSpeed = ft;
            yield return null;
        }
        fallSpeed = bgs.ScrollSpeed;

        //飛び起きて跳ねるところ
        t = 0.0f;
        isEnd = false;

        //飛び起きた時の短いジャンプ
        StartCoroutine(Motion_Hop(1, 0.2f));
        eyes[0].transform.localScale = new Vector3(defScale.x, defScale.y);
        eyes[1].transform.localScale = new Vector3(defScale.x, defScale.y);
        while (!isEnd)
        {
            if (t >= 1.0f) isEnd = true;
            t += Time.deltaTime / 0.25f;

            transform.localRotation = Quaternion.Euler(leaning * (1.0f - EaseInOutBack(t)));
            int count = 0;
            for (int i = 0; i < bodyParts.Count; i++)
            {
                for (int j = 0; j < bodyParts[i].Length; j++)
                {
                    bodyParts[i][j].transform.localRotation =
                        Quaternion.Euler(new Vector3(0, 0, defRotZ[count] * 0.5f) * EaseInOutBack(t));
                    count++;
                }
            }
            yield return null;
        }
        yield return new WaitForSeconds(0.5f);
        sleptTimer = 0.0f;
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="time"></param>
    /// <returns></returns>
    private IEnumerator Motion_Floating(float time)
    {
        isMotionStarted = true;

        float t = 0.0f;
        bool isEnd = false;
        while (!isEnd)
        {
            if (t >= time) isEnd = true;

            t += Time.deltaTime;
            float x = transform.position.x;
            float y = parent.transform.position.y + Mathf.Sin(t * Mathf.PI) * floatHeight;
            transform.position = new Vector2(x, y);

            yield return null;
        }
        transform.position = parent.transform.position;
        isMotionStarted = false;
    }

    /// <summary>
    /// 移動のモーションを実行するコルーチン
    /// </summary>
    /// <param name="time">移動にかかる時間</param>
    /// <returns></returns>
    private IEnumerator Motion_Move(float time,Vector3 movePos)
    {
        isMotionStarted = true;

        float t = 0.0f;
        bool isEnd = false;

        Vector3 defPos = parent.transform.localPosition;
        Vector3 targetVec = movePos - parent.transform.localPosition;

        if (targetVec.x < 0) parent.transform.rotation = directionLeft;
        else parent.transform.rotation = direntionRight;

        StartCoroutine(Motion_Floating(time));
        while (!isEnd)
        {
            if (t >= time) isEnd = true;
            t += Time.deltaTime;

            parent.transform.localPosition = defPos + targetVec * t / time;

            yield return null;
        }
        parent.transform.localPosition = movePos;
        isMotionStarted = false;
    }

    /// <summary>
    /// 跳ねるモーションを実行するコルーチン。モーションの全体時間はhops*time+0.1秒。
    /// </summary>
    /// <param name="hops">跳ねる回数</param>
    /// <param name="time">一回の跳ねに掛かる時間</param>
    /// <returns></returns>
    private IEnumerator Motion_Hop(int hops, float time)
    {
        isMotionStarted = true;

        for(int i = 0; i < hops; i++)
        {
            float t = 0.0f;
            bool isEnd = false;
            Vector3 defScale = parent.transform.localScale;
            Vector3 motionScale = new Vector3(1.15f, 0.9f, 1) - defScale;

            while (!isEnd)
            {
                if (t > 1.0f) isEnd = true;

                t += Time.deltaTime / 0.05f;
                parent.transform.localScale = defScale + motionScale * t;

                yield return null;
            }

            t = 0.0f;
            isEnd = false;
            defScale = parent.transform.localScale;
            motionScale = new Vector3(0.85f, 1.2f, 1) - defScale;

            while (!isEnd)
            {
                if (t > 1.0f) isEnd = true;

                parent.transform.localScale = defScale + motionScale * EaseOutCubic(t);
                t += Time.deltaTime / time;
                float x = transform.position.x;
                float y = parent.transform.position.y +
                    Mathf.Abs(Mathf.Sin(t * Mathf.PI)) * hoppingHeight;
                transform.position = new Vector2(x, y);

                yield return null;
            }

            t = 0.0f;
            isEnd = false;
            defScale = parent.transform.localScale;
            motionScale = new Vector3(1, 1, 1) - defScale;

            while (!isEnd)
            {
                if (t > 1.0f) isEnd = true;

                t += Time.deltaTime / 0.05f;
                parent.transform.localScale = defScale + motionScale * t;

                yield return null;
            }
            parent.transform.localScale = defScale + motionScale;
        }
        isMotionStarted = false;
    }

    /// <summary>
    /// 自作のイージング関数。二次関数を折り返しただけのグラフ。
    /// </summary>
    /// <param name="t">グラフに対する時間軸。戻り値は0.5で1、1.0で0に戻る。</param>
    /// <returns></returns>
    private float EaseInReturn(float t)
    {
        float constValue1 = 2 * t;
        float constValue2 = 2 * (t - 1.0f);

        return t < 0.5f ?
            constValue1 * constValue1 :
            constValue2 * constValue2;
    }

    /// <summary>
    /// イージング関数。なだらかに上昇し後半にかけて急激に上昇するグラフ。
    /// </summary>
    /// <param name="t">グラフに対する時間軸。1.0で1が返ってくる。</param>
    /// <returns></returns>
    private float EaseInExpo(float t)
    {
        return t == 0 ?
            0 :
            t < 1.0f ?
            Mathf.Pow(2, 10 * t - 10) :
            1.0f;
    }

    /// <summary>
    /// イージング関数。急激に上昇し後半にかけてなだらかになっていくグラフ。
    /// </summary>
    /// <param name="t">グラフに対する時間軸。1.0で1が返ってくる。</param>
    /// <returns></returns>
    private float EaseOutCubic(float t)
    {
        float c = 1 - t;
        return t < 1.0 ? 1 - c * c * c : 1.0f;
    }

    /// <summary>
    /// イージング関数。上下に飛び出すＳ字みたいなグラフ。
    /// </summary>
    /// <param name="t">グラフに対する時間軸。1.0で1が返ってくる。</param>
    /// <returns></returns>
    private float EaseInOutBack(float t)
    {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;

        float x1 = 2 * t;
        float x2 = 2 * t - 2;

        return t < 0.5f ?
            (x1 * x1 * ((c2 + 1) * 2 * t - c2)) / 2 :
            t < 1.0f ?
            (x2 * x2 * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2 :
            1.0f;
    }
}
