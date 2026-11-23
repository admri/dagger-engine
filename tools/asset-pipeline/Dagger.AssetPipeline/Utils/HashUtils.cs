using System.Text;

namespace Dagger.AssetPipeline.Utils;

public static class HashUtils
{
    public const uint InvalidValue = 0;
    private const uint FnvOffsetBasis = 2166136261;
    private const uint FnvPrime = 16777619;

    public static uint HashString(string text)
    {
        if (string.IsNullOrEmpty(text))
        {
            return InvalidValue;
        }

        var hash = FnvOffsetBasis;
        
        foreach (var b in Encoding.UTF8.GetBytes(text))
        {
            hash ^= b;
            hash *= FnvPrime;
        }
        return hash;
    }
}