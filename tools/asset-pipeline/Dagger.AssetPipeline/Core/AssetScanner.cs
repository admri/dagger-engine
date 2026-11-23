using System.Threading.Channels;
using Dagger.AssetPipeline.Models;
using Dagger.AssetPipeline.Utils;

namespace Dagger.AssetPipeline.Core
{
    public class AssetScanner(string sourceDir, BuildCache cache)
    {
        public async Task ScanAsync(ChannelWriter<AssetJob> writer)
        {
            Console.WriteLine($"[Scanner] Starting scanning: {sourceDir}");
            int queuedCount = 0;
            int skippedCount = 0;

            var jsonFiles = Directory.EnumerateFiles(sourceDir, "*.json", SearchOption.AllDirectories);
            foreach (var file in jsonFiles)
            {
                // Ignore schema
                if (Path.GetFileName(file).Equals("schema.json", StringComparison.OrdinalIgnoreCase)) 
                    continue;

                string hash = await FileHasher.ComputeHashAsync(file);
                if (cache.HasChanged(file, hash))
                {
                    await writer.WriteAsync(new AssetJob { SourcePath = file, Type = AssetType.Prefab, Hash = hash });
                    queuedCount++;
                }
                else skippedCount++;
            }

            var atlasDirs = Directory.EnumerateDirectories(sourceDir, "*.atlas", SearchOption.AllDirectories);
            foreach (var dir in atlasDirs)
            {
                string hash = await FileHasher.ComputeDirectoryHashAsync(dir);
                if (cache.HasChanged(dir, hash))
                {
                    await writer.WriteAsync(new AssetJob
                        { SourcePath = dir, Type = AssetType.TextureAtlas, Hash = hash });
                    queuedCount++;
                }
                else skippedCount++;
            }

            writer.Complete();
            Console.WriteLine($"[Scanner] Done. Queued: {queuedCount}, Skipped: {skippedCount}");
        }
    }
}