using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class ButtonEffects : MonoBehaviour
{
    private enum ButtonMode
    {
        NONE=0,
        START,
        LIBRARY,
        OPTION,
        EXIT
    }
    private enum ButtonState
    {
        OFF_CURSOR,
        ON_CURSOR,
    }
    private Image buttonBackGroundImage;
    [SerializeField] private GameObject buttonDecoration;
    [SerializeField] private Sprite[] buttonSprites;
    [SerializeField] private GameObject offContents;
    [SerializeField] private GameObject onContents;
    private ButtonMode mode;
    private ButtonState state;

    private const float scalingSpeed_Button = 10.0f;
    private Vector3 onCursorScale_Button;
    private Vector3 offCursorScale_Button;

    private const float scalingSpeed_Deco = 5.0f;
    private Vector3 onCursorScale_Deco;
    private Vector3 offCursorScale_Deco;
    // Start is called before the first frame update
    void Awake()
    {
        switch (gameObject.name)
        {
            case "Start":
                mode = ButtonMode.START;
                break;


            case "Library":
                mode = ButtonMode.LIBRARY;
                break;

            case "Option":
                mode = ButtonMode.OPTION;
                break;

            case "Exit":
                mode = ButtonMode.EXIT;
                break;

        }
        buttonBackGroundImage = transform.GetChild(0).gameObject.GetComponent<Image>();
        buttonBackGroundImage.sprite = buttonSprites[0];
        state = ButtonState.OFF_CURSOR;

        offCursorScale_Button = Vector3.one;
        onCursorScale_Button = Vector3.one * 1.25f;

        offCursorScale_Deco = Vector3.one;
        onCursorScale_Deco = Vector3.one * 1.1f;
    }

    // Update is called once per frame
    void Update()
    {
        if(state == ButtonState.OFF_CURSOR)
        {
            buttonBackGroundImage.sprite = buttonSprites[0];
            Scaling(gameObject, offCursorScale_Button,scalingSpeed_Button);
            Scaling(buttonDecoration, offCursorScale_Deco,scalingSpeed_Deco);
            buttonDecoration.SetActive(false);
            buttonDecoration.transform.localScale = offCursorScale_Deco;
            onContents.SetActive(false);
            offContents.SetActive(true);
        }
        else
        {
            buttonBackGroundImage.sprite = buttonSprites[1];
            buttonDecoration.SetActive(true);
            Scaling(gameObject, onCursorScale_Button,scalingSpeed_Button);
            Scaling(buttonDecoration, onCursorScale_Deco,scalingSpeed_Deco);
            onContents.SetActive(true);
            offContents.SetActive(false);

        }
    }

    public void OnClick()
    {
        string sceneName = null;
        switch (mode)
        {
            case ButtonMode.START:
                sceneName = "MapTest";
                break;

            case ButtonMode.LIBRARY:
                sceneName = "Library";
                break;

                case ButtonMode.OPTION:
                sceneName = "Option";
                break;

                case ButtonMode.EXIT:
                Application.Quit();
                break;
        }

        if (sceneName != null)
            SceneManager.LoadScene(sceneName);
    }

   public void PointerEnter()
    {
        state = ButtonState.ON_CURSOR;
    }

    public void PointerExit()
    {
        state= ButtonState.OFF_CURSOR;
    }

    private void Scaling(GameObject go,Vector3 scaleLate,float scalingSpeed)
    {
        Vector3 scl = scaleLate - go.transform.localScale;
        if (scl.x < 0 ? go.transform.localScale.x > scaleLate.x : go.transform.localScale.x < scaleLate.x)
            go.transform.localScale += scl * Time.deltaTime * scalingSpeed;
        else if (scl.x > 0 ? go.transform.localScale.x > scaleLate.x : go.transform.localScale.x < scaleLate.x)
            go.transform.localScale = scaleLate;
    }
}
