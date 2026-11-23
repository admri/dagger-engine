using System.Security.Cryptography;

namespace Dagger.AssetPipeline.Utils;

public static class FileHasher
{
    /// <summary>
    /// Asynchronously computes SHA-256 hash of a given file and returns it as a Base64 string.
    /// </summary>
    public static async Task<string> ComputeHashAsync(string filePath)
    {
        if (!File.Exists(filePath))
            return string.Empty;

        using var sha256 = SHA256.Create();
        await using var stream = File.OpenRead(filePath);
        
        byte[] hashBytes = await sha256.ComputeHashAsync(stream);
        
        return Convert.ToBase64String(hashBytes);
    }

    public static async Task<string> ComputeDirectoryHashAsync(string directoryPath)
    {
        if (!Directory.Exists(directoryPath))
            return string.Empty;

        var files = Directory.GetFiles(directoryPath, "*.*", SearchOption.AllDirectories);
        
        // Sort files by name alphabetically to not influence final hash by random ordering
        Array.Sort(files); 

        using var sha256 = SHA256.Create();
        
        foreach (var file in files)
        {
            await using var stream = File.OpenRead(file);
            byte[] buffer = new byte[8192];
            int bytesRead;
            while ((bytesRead = await stream.ReadAsync(buffer)) > 0)
            {
                sha256.TransformBlock(buffer, 0, bytesRead, buffer, 0);
            }
        }

        sha256.TransformFinalBlock([], 0, 0);
        return Convert.ToBase64String(sha256.Hash!);
    }
}