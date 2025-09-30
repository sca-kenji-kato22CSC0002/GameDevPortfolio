using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Magma : MonoBehaviour
{
    [SerializeField,Header("‚PƒuƒƒbƒN/Ý’è•b")] private float maxSpeed;
    [SerializeField] private float defSpeed;
    [SerializeField] private float speed;
    [SerializeField] private GameObject player;
    private const int mapHeight = 13;
    private float oreEffectSpeedRate;
    private float timer;
    private bool isMovable;
    [SerializeField, Header("ŠJŽnŒã‚ÉŽw’è•b‘Ò‚Â")] private float waitTime;

    private void Start()
    {
        speed = defSpeed;
        oreEffectSpeedRate = 1.0f;
        timer = 0.0f;
        isMovable = false;
        StartCoroutine(WaitAnySeconds());
    }

    // Update is called once per frame
    void Update()
    {
        if (isMovable)
        {
            Vector3 spd = Vector3.zero;
            float dist = Mathf.Abs(transform.localPosition.y - player.transform.localPosition.y);
            if (dist >= mapHeight)
            {
                speed = maxSpeed;
            }
            else
            {
                speed = defSpeed;
            }
            spd.y = 1.0f / speed * oreEffectSpeedRate <= maxSpeed ?
                1.0f / speed * oreEffectSpeedRate : maxSpeed;

            transform.position += spd * Time.deltaTime;
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        PlayerManager.state = PlayerManager.PlayerState.PlayerMove;
        if (collision.CompareTag("Player")) { SceneManager.LoadScene("Title"); }
       
    }

    private IEnumerator WaitAnySeconds()
    {
        yield return new WaitForSeconds(waitTime);
        isMovable = true;
    }
}
