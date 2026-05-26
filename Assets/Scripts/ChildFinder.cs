using UnityEngine; 

public static class ChildFinder
{
    public static Transform FindChild(Transform avatarRoot, string childName)
    {
        Transform result = avatarRoot.Find(childName);

        if (result == null)
            result = FindChildRecursive(avatarRoot, childName);

        if (result == null)
            Debug.LogWarning($"[AvatarChildFinder] '{childName}' not found under '{avatarRoot.name}'.");

        return result;
    }

    private static Transform FindChildRecursive(Transform parent, string targetName)
    {
        foreach (Transform child in parent)
        {
            if (child.name == targetName)
                return child;

            Transform found = FindChildRecursive(child, targetName);
            if (found != null)
                return found;
        }

        return null;
    }
}