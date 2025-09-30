using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class PlayerMove : MonoBehaviour
{
    public enum Difficulty
    {
        Hop = 1,
        Standard = 2,
        Technical = 3
    }
    public Difficulty dif;
    private float Speed;
    public float SpeedNeutralHop;
    public float SpeedNeutralStandard;
    public float SpeedNeutralTechnical;
    public float SpeedUpHop;
    public float SpeedUpStandard;
    public float SpeedUpTechnical;
    public float maxSpeed;
    public float torqueSpeed;
    private PhysicsMaterial2D dynamicMaterial;
    private Collider2D col2D;
    public float bouncinessNeutral;
    public float bouncinessDown;
    public float bouncinessUp;
    public float friction;
    public GameObject PlayerSkin;
    private float skinSize=0.5f;
    private float skinRotateMax = 0.7f;
    [SerializeField] Rigidbody2D rb = new Rigidbody2D();

    public float PowerMaxTime;
    public float PowerCoolTimeNeutral;
    public float PowerCoolTimeDown;
    public float PowerCoolTimeUp;
    private float PowerCoolTimeRight;
    float PowerTimerRight;
    private float PowerCoolTimeLeft;
    float PowerTimerLeft;

    private string TitleSceneName="Title";
    private string GoalTagName="Goal";

    public Image gaugeImageRight; // ゲージに使用するImageコンポーネント
    public Image gaugeImageLeft; // ゲージに使用するImageコンポーネント
    public GameObject gaugeLeft;
    [Range(0, 1)] public float fillAmountRight = 1.0f; // 塗りつぶし率
    [Range(0, 1)] public float fillAmountLeft = 1.0f; // 塗りつぶし率
    bool moveRight;
    bool moveLeft;
    bool move;
    [SerializeField] private Color fullHealthColor = Color.white;   // HP最大時の色
    [SerializeField] private Color lowHealthColor = Color.red;     // HP最小時の色
    [SerializeField] private float forceMultiplier = 10f; // 力の大きさを調整
    [SerializeField] private GameObject gameManger;
    private PlayerManager pm;
    [SerializeField] private GameObject playerCamera;
    private Camera cam;
    private float camSize;
    private float camSizeNotZero=0.01f;
    private float camZNeutral=10.0f;
    public float camSizeSpeed;
    public float camSizeNeutral;
    public float camSizeUp;
    public float camSizeDown;
    public GameObject Back;
    public float BackPos;

    //鉱石関連
    //Plus
    public static bool coolTimeDown; //移動クールタイム減少
    public bool knockbackDown; //ノックバック軽減
    public bool scoreUp; //短期間獲得スコア上昇
    public bool visibilityUp; //視界範囲上昇
    public bool lavaSpeedDown; //溶岩速度低下
    //Neutral
    public static bool chargeMax; //常時チャージ移動
    public bool destroyWithOneHit; //鉱石一撃破壊
    public static  bool speedUp; //移動距離UP
    //Minus
    public bool lavaSpeedUp; //溶岩速度上昇
    public static bool coolTimeUp; //移動クールタイム増加
    public bool scoreDown; //3回獲得スコア減少
    public bool mapAmountDown; //所持可能マップ数低下
    public bool knockbackUp; //ノックバック増加
    public bool mapDropDown; //ピースドロップ率低下
    public bool mapBlind; //暗黙構想ピース化
    public bool visibilityDown; //視界範囲減少

    // Start is called before the first frame update
    void Start()
    {
        pm = this.gameObject.GetComponent<PlayerManager>();
        pm.AddFunction(PlayerUpdate);
        cam= playerCamera.GetComponent<Camera>();
        moveRight = false;
        moveLeft = false;
        move = false;
        PowerTimerRight = 0;
        PowerTimerLeft = 0;
        rb = GetComponent<Rigidbody2D>();
        dynamicMaterial = new PhysicsMaterial2D();
        PowerCoolTimeRight = PowerCoolTimeNeutral;
        PowerCoolTimeLeft = PowerCoolTimeNeutral;
        
        camSize = camSizeNeutral;

        col2D = GetComponent<Collider2D>();
        dynamicMaterial.bounciness = bouncinessNeutral; // 初期値
        dynamicMaterial.friction = friction;   // 任意
        col2D.sharedMaterial = dynamicMaterial;

        //鉱石関連
        coolTimeDown = false;
        knockbackDown = false;
        scoreUp = false;
        visibilityUp = false;
        lavaSpeedDown = false;
        chargeMax = false;
        destroyWithOneHit = false;
        speedUp = false;
        lavaSpeedUp = false;
        coolTimeUp = false;
        scoreDown = false;
        mapAmountDown = false;
        knockbackUp = false;
        mapDropDown = false;
        mapBlind = false;
        visibilityDown = false;
        if (dif == Difficulty.Hop)
        {
            Speed = SpeedNeutralHop;
            rb.constraints = RigidbodyConstraints2D.FreezeRotation;
            gaugeLeft.gameObject.SetActive(false);
        }
        if (dif == Difficulty.Standard)
        {
            Speed = SpeedNeutralStandard;
            rb.constraints = RigidbodyConstraints2D.FreezeRotation;
            gaugeLeft.gameObject.SetActive(true);
        }
        if (dif == Difficulty.Technical)
        {
            Speed = SpeedNeutralTechnical;
            rb.constraints = RigidbodyConstraints2D.None;
            gaugeLeft.gameObject.SetActive(true);
        }
    }

    // 任意でゲージを設定するメソッド
    public void SetGaugeRight(float value)
    {
        fillAmountRight = Mathf.Clamp01(value); // 0～1にクランプ
    }
    public void SetGaugeLeft(float value)
    {
        fillAmountLeft = Mathf.Clamp01(value); // 0～1にクランプ
    }

    private void PlayerUpdate()
    {
        cam.orthographicSize = camSize;
        playerCamera.transform.position = new Vector3(transform.position.x, transform.position.y, -camZNeutral);
        if (Back != null&&BackPos!=0)
        {
            Back.transform.position=new Vector3(playerCamera.transform.position.x/BackPos,this.transform.position.y/BackPos,0);
        }

        dynamicMaterial.bounciness = Mathf.PingPong(Time.time, 1.0f);

        // 現在の速度を取得
        Vector2 velocity = rb.velocity;

        // 速度が上限を超えた場合、制限をかける
        if (velocity.magnitude > maxSpeed)
        {
            rb.velocity = velocity.normalized * maxSpeed;
        }

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            SceneManager.LoadScene(TitleSceneName);
        }
        if (gaugeImageRight != null && gaugeImageLeft != null)
        {
            gaugeImageRight.fillAmount = fillAmountRight;
            gaugeImageLeft.fillAmount = fillAmountLeft;
        }
        if (dif == Difficulty.Hop)
        {
            gaugeImageRight.fillAmount = fillAmountRight;
            if (Input.GetMouseButton(0) && PowerTimerRight < PowerMaxTime && !move)
            {
                PowerTimerRight += Time.deltaTime;
            }
            if (Input.GetMouseButtonUp(0))
            {
                Vector3 vel = rb.velocity;
                vel.x = 0;
                vel.y = 0;
                rb.velocity = vel;
                move = true;
                // マウスのワールド座標を取得
                Vector3 mouseP = Camera.main.ScreenToWorldPoint(Input.mousePosition);
                mouseP.z = 0; // 2DのためZ軸を0に

                // クリックした方向を計算
                Vector2 dir = (mouseP - transform.position).normalized * Speed;

                // Rigidbody2Dに力を加える
                rb.AddForce(dir * PowerTimerRight, ForceMode2D.Impulse);
            }
            if (move)
            {
                PowerTimerRight -= PowerCoolTimeRight * Time.deltaTime;
                gaugeImageRight.color = lowHealthColor;
            }
            if (PowerTimerRight <= 0)
            {
                PowerTimerRight = 0;
                move = false;
                gaugeImageRight.color = fullHealthColor;
            }
            SetGaugeRight(PowerTimerRight / PowerMaxTime);

            // マウスのスクリーン座標を取得
            Vector3 mousePosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            mousePosition.z = 0f; // 2DなのでZ座標は無視

            // オブジェクトの位置からマウスの位置までのベクトルを計算
            Vector2 direction = mousePosition - transform.position;

            // ベクトルの角度を取得して回転させる
            float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg;

            if (!move && PlayerSkin != null)
            {
                PlayerSkin.transform.rotation = Quaternion.Euler(new Vector3(0, 0, angle));
                if (PlayerSkin.transform.rotation.z > skinRotateMax || PlayerSkin.transform.rotation.z < -skinRotateMax)
                {
                    PlayerSkin.transform.localScale = new Vector3(-skinSize, -skinSize, 1);
                }
                else
                {
                    PlayerSkin.transform.localScale = new Vector3(-skinSize, skinSize, 1);
                }
            }
        }

        if (dif==Difficulty.Standard)
        {
            if (gaugeImageRight != null && gaugeImageLeft != null)
            {
                gaugeImageRight.fillAmount = fillAmountRight;
                gaugeImageLeft.fillAmount = fillAmountLeft;
            }

            if (Input.GetMouseButton(1) && PowerTimerRight < PowerMaxTime && !moveRight)
            {
                PowerTimerRight += Time.deltaTime;
            }

            if (Input.GetMouseButtonUp(1) && !moveRight)
            {
                Vector3 vel = rb.velocity;
                vel.x = 0;
                vel.y = 0;
                rb.velocity = vel;
                moveRight = true;
                Vector2 dir = new Vector2(1, 1).normalized * Speed;
                PlayerSkin.transform.localScale = new Vector3(-skinSize, skinSize, 1);

                // Rigidbody2Dに力を加える
                rb.AddForce(dir * PowerTimerRight, ForceMode2D.Impulse);
            }

            if (Input.GetMouseButton(0) && PowerTimerLeft < PowerMaxTime && !moveLeft)
            {
                PowerTimerLeft += Time.deltaTime;
            }
            if (Input.GetMouseButtonUp(0) && !moveLeft)
            {
                Vector3 vel = rb.velocity;
                vel.x = 0;
                vel.y = 0;
                rb.velocity = vel;
                moveLeft = true;
                Vector2 dir = new Vector2(-1, 1).normalized * Speed;
                PlayerSkin.transform.localScale = new Vector3(skinSize, skinSize, 1);

                // Rigidbody2Dに力を加える
                rb.AddForce(dir * PowerTimerLeft, ForceMode2D.Impulse);
            }
            if (moveLeft && moveRight)
            {
                Vector3 vel = rb.velocity;
                vel.x = 0;
                rb.velocity = vel;
            }
            if (moveRight)
            {
                PowerTimerRight -= PowerCoolTimeRight * Time.deltaTime;
                gaugeImageRight.color = lowHealthColor;
            }
            if (PowerTimerRight <= 0)
            {
                PowerTimerRight = 0;
                moveRight = false;
                gaugeImageRight.color = fullHealthColor;
            }
            SetGaugeRight(PowerTimerRight / PowerMaxTime);

            if (moveLeft)
            {
                PowerTimerLeft -= PowerCoolTimeLeft * Time.deltaTime;
                gaugeImageLeft.color = lowHealthColor;
            }
            if (PowerTimerLeft <= 0)
            {
                PowerTimerLeft = 0;
                moveLeft = false;
                gaugeImageLeft.color = fullHealthColor;
            }
            SetGaugeLeft(PowerTimerLeft / PowerMaxTime);
        }
        if (dif == Difficulty.Technical)
        {
            //右翼
            if (Input.GetMouseButton(1) && PowerTimerRight < PowerMaxTime && !moveRight)
            {
                PowerTimerRight += Time.deltaTime;
            }

            if (Input.GetMouseButtonUp(1) && !moveRight)
            {
                moveRight = true;
                rb.angularVelocity = 0f;
                rb.AddTorque(-torqueSpeed * PowerTimerRight);
            }
            //左翼
            if (Input.GetMouseButton(0) && PowerTimerLeft < PowerMaxTime && !moveLeft)
            {
                PowerTimerLeft += Time.deltaTime;
            }
            if (Input.GetMouseButtonUp(0) && !moveLeft)
            {
                moveLeft = true;
                rb.angularVelocity = 0f;
                rb.AddTorque(torqueSpeed * PowerTimerLeft);
            }
            if (moveLeft && moveRight && !move)
            {
                move = true;
                float Power = PowerTimerLeft + PowerTimerRight;
                rb.angularVelocity = 0f;
                rb.AddForce(transform.up * Power, ForceMode2D.Impulse);
            }
            if (moveRight)
            {
                PowerTimerRight -= PowerCoolTimeRight * Time.deltaTime;
                gaugeImageRight.color = lowHealthColor;
            }
            if (PowerTimerRight <= 0)
            {
                PowerTimerRight = 0;
                moveRight = false;
                move = false;
                gaugeImageRight.color = fullHealthColor;
            }
            SetGaugeRight(PowerTimerRight / PowerMaxTime);

            if (moveLeft)
            {
                PowerTimerLeft -= PowerCoolTimeLeft * Time.deltaTime;
                gaugeImageLeft.color = lowHealthColor;
            }
            if (PowerTimerLeft <= 0)
            {
                PowerTimerLeft = 0;
                moveLeft = false;
                move = false;
                gaugeImageLeft.color = fullHealthColor;
            }
            SetGaugeLeft(PowerTimerLeft / PowerMaxTime);
        }
        

        //鉱石関連
        //Plus
        if (coolTimeDown)
        {
            if (PowerCoolTimeRight != PowerCoolTimeDown)
            {
                PowerCoolTimeRight = PowerCoolTimeDown;
                PowerCoolTimeLeft = PowerCoolTimeDown;
            }
        }
        if (knockbackDown)
        {
            if(dynamicMaterial.bounciness != bouncinessDown)
            {
                col2D = GetComponent<Collider2D>();
                dynamicMaterial.bounciness = bouncinessDown;
                col2D.sharedMaterial = dynamicMaterial;
            }
        }
        if (scoreUp)
        {

        }
        if (visibilityUp)
        {
            if (camSize >= camSizeUp)
            {
                camSize = camSizeUp;

            }else
            {
                camSize += Time.deltaTime* camSizeSpeed;
            }
        }
        if (lavaSpeedDown)
        {

        }
        //Neutral
        if (chargeMax)
        {
            if (dif == Difficulty.Standard || dif == Difficulty.Technical)
            {
                if (Input.GetKey(KeyCode.D) && PowerTimerRight < PowerMaxTime && !moveRight)
                {
                    PowerTimerRight = PowerMaxTime;
                }
                if (Input.GetKey(KeyCode.A) && PowerTimerLeft < PowerMaxTime && !moveLeft)
                {
                    PowerTimerLeft = PowerMaxTime;
                }
            }
            else
            {
                if (Input.GetMouseButton(0) && PowerTimerRight < PowerMaxTime && !move)
                {
                    PowerTimerRight = PowerMaxTime;
                }
            }
        }
        if (destroyWithOneHit)
        {

        }
        if (speedUp)
        {
            if (dif == Difficulty.Hop)
            {
                if (Speed != SpeedUpHop)
                {
                    Speed = SpeedUpHop;
                }
            }
            if (dif == Difficulty.Standard)
            {
                if (Speed != SpeedUpStandard)
                {
                    Speed = SpeedUpStandard;
                }
            }
            if (dif == Difficulty.Technical)
            {
                if (Speed != SpeedUpTechnical)
                {
                    Speed = SpeedUpTechnical;
                }
            }
            
        }
        //Minus
        if (lavaSpeedUp)
        {

        }
        if (coolTimeUp)
        {
            if (PowerCoolTimeRight != PowerCoolTimeUp)
            {
                PowerCoolTimeRight = PowerCoolTimeUp;
                PowerCoolTimeLeft = PowerCoolTimeUp;
            }
        }
        if (scoreDown)
        {

        }
        if (mapAmountDown)
        {

        }
        if (knockbackUp)
        {
            if (dynamicMaterial.bounciness != bouncinessUp)
            {
                col2D = GetComponent<Collider2D>();
                dynamicMaterial.bounciness = bouncinessUp;
                col2D.sharedMaterial = dynamicMaterial;
            }
        }
        if (mapDropDown)
        {

        }
        if (mapBlind)
        {

        }
        if (visibilityDown)
        {
            if (camSize <= camSizeDown)
            {
                camSize = camSizeDown;
            }
            else
            {
                camSize -= Time.deltaTime* camSizeSpeed;
            }
        }

        //戻す処理
        if (!coolTimeDown&&!coolTimeUp)
        {
            if (PowerCoolTimeRight != PowerCoolTimeNeutral)
            {
                PowerCoolTimeRight = PowerCoolTimeNeutral;
                PowerCoolTimeLeft = PowerCoolTimeNeutral;
            }
        }
        if (!knockbackUp && !knockbackDown)
        {
            if (dynamicMaterial.bounciness != bouncinessNeutral)
            {
                col2D = GetComponent<Collider2D>();
                dynamicMaterial.bounciness = bouncinessNeutral;
                col2D.sharedMaterial = dynamicMaterial;
            }
        }
        if (!lavaSpeedDown&&lavaSpeedUp)
        {

        }
        if (!visibilityDown && !visibilityUp)
        {
            if(camSize<camSizeNeutral)
            {
                camSize += Time.deltaTime * camSizeSpeed;
            }
            if (camSize > camSizeNeutral)
            {
                camSize -= Time.deltaTime * camSizeSpeed;
            }
            if (camSizeNotZero < camSize - camSizeNeutral && -camSizeNotZero > camSize-camSizeNeutral)
            {
                camSize = camSizeNeutral;
            }
        }
        if (!speedUp)
        {
            if (dif == Difficulty.Hop)
            {
                Speed = SpeedNeutralHop;
            }
            if (dif == Difficulty.Standard)
            {
                Speed = SpeedNeutralStandard;
            }
            if (dif == Difficulty.Technical)
            {
                Speed = SpeedNeutralTechnical;
            }
        }
    }

    private void OnTriggerEnter2D(Collider2D coll)
    {
        if(coll.gameObject.CompareTag(GoalTagName))
        {
            SceneManager.LoadScene(TitleSceneName);
        }
    }
}
