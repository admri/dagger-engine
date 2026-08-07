using System.Threading.Channels;
using Dagger.AssetPipeline.Models;

namespace Dagger.AssetPipeline.Core
{
    public class AssetScanner(string sourceDir)
    {
        public async Task ScanAsync(ChannelWriter<AssetJob> writer)
        {
            Console.WriteLine($"[Scanner] Starting scanning: {sourceDir}");
            int count = 0;

            var jsonFiles = Directory.EnumerateFiles(sourceDir, "*.json", SearchOption.AllDirectories);
            foreach (var file in jsonFiles)
            {
                // Ignore schema
                if (Path.GetFileName(file).Equals("schema.json", StringComparison.OrdinalIgnoreCase)) 
                    continue;

                await writer.WriteAsync(new AssetJob { SourcePath = file, Type = AssetType.Prefab });
                count++;
            }

            var atlasDirs = Directory.EnumerateDirectories(sourceDir, "*.atlas", SearchOption.AllDirectories);
            foreach (var dir in atlasDirs)
            {
                await writer.WriteAsync(new AssetJob { SourcePath = dir, Type = AssetType.TextureAtlas });
                count++;
            }

            writer.Complete(); 
            Console.WriteLine($"[Scanner] Done. Found {count} assets for processing.");
        }
    }
}