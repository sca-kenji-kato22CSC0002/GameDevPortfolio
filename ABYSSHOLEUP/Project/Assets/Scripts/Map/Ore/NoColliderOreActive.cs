using UnityEngine;

public class NoColliderOreActive : MonoBehaviour
{
    private GameObject myParent;
    private SpriteRenderer myRenderer;
    private SpriteRenderer parentRendere;
    private void Awake()
    {
        myParent = this.gameObject.transform.parent.gameObject;
        parentRendere = myParent.GetComponent<SpriteRenderer>();
        myRenderer = this.GetComponent<SpriteRenderer>();
    }


    private void Update()
    {
        myRenderer.color = new Color(1,1,1, parentRendere.color.a);
    }
}
