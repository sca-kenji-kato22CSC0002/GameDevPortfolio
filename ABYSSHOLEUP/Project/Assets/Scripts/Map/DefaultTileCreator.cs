using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using UnityEngine;
/// <summary>
/// ’S“–:ŒF’J
/// </summary>
public class DefaultTileCreator : MonoBehaviour 
{
    private const int mapWidth = 11;
    private const int mapHeight = 8;
    [SerializeField] private GameObject defaultTile;
    [SerializeField] private GameObject MapCreator;
    private MapCreate mc;
    private void Start()
    {
        mc=MapCreator.GetComponent<MapCreate>();
        for(int x=0;x<mapWidth; x++)
        {
            for (int y=0;y<mapHeight; y++)
            {
                if(x!=5||y!=0)
                {
                    GameObject obj = Instantiate(defaultTile, new Vector3(x * 13, y * 13, 0), Quaternion.identity);
                    obj.transform.name = "DefaultTile";
                    obj.transform.parent = transform;
                }
                else
                {
                    mc.PieceCreator(mc.ResPawnPiece,0,1, new Vector3(x * 13, y * 13, 0));
                }
                
            }

        }
    }
    

}
