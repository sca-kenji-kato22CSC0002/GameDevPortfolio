using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Gauge : MonoBehaviour
{
    [SerializeField] private GameObject player;
    [SerializeField] private GameObject magma;
    [SerializeField] private Text altitudeText;
    private const float revision = -6.5f;
    private const float goalHeight = 95.0f;
    private const float gaugeHeight = 800;
    private float playerProgress;
    private float magmaProgress;

    [SerializeField] private GameObject playerIcon;
    [SerializeField] private GameObject magmaIcon;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float pPosY = player.transform.position.y - revision >= 0.0f ? player.transform.position.y - revision : 0.0f;
        playerProgress = pPosY / goalHeight;
        altitudeText.text = "___" + (goalHeight - playerProgress * goalHeight) + "m";
        playerIcon.transform.localPosition = new Vector3(0, gaugeHeight * playerProgress);

        float mPosY = magma.transform.position.y - revision >= 0.0f ? magma.transform.position.y - revision : 0.0f;
        magmaProgress = mPosY / goalHeight;
        magmaIcon.transform.localPosition = new Vector3(20.0f, magmaProgress * gaugeHeight + 400.0f, 0.0f);
    }
}
