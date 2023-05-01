using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class BallsSpawner : MonoBehaviour
{
    [Header("Obejct Prefab")]
    public GameObject BallPrefab;
    [Header("Spawn Config")]
    public Transform SpawnCenter; 
    public float SpawnRadius;
    public int minInstances;
    public List<Color> spawnColors; 
    // Start is called before the first frame update
    void Start()
    {
        if(SpawnCenter == null)
        {
            SpawnCenter = transform; 
        }
    }

    // Update is called once per frame
    void Update()
    {
        if(transform.childCount < minInstances)
        {
            GameObject instance = Instantiate(BallPrefab, transform);
            instance.GetComponent<MeshRenderer>().material.color = spawnColors[Random.Range(0, spawnColors.Count - 1)];
            instance.transform.localPosition = new Vector3(Random.Range(-SpawnRadius,SpawnRadius), Random.Range(-SpawnRadius, SpawnRadius),0);
        }
    }



    private void OnDrawGizmos()
    {
        Gizmos.color = Color.yellow;
        Gizmos.DrawWireSphere(transform.position, SpawnRadius);
        
    }
}
