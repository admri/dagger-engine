using System;
using System.IO;
using System.Threading.Channels;
using System.Threading.Tasks;
using Dagger.AssetPipeline.Models;

namespace Dagger.AssetPipeline.Core
{
    public class AssetScanner(string sourceDir)
    {
        public async Task ScanAsync(ChannelWriter<AssetJob> writer)
        {
            Console.WriteLine($"[Scanner] Starting scanning: {sourceDir}");

            var files = Directory.EnumerateFiles(sourceDir, "*.*", SearchOption.AllDirectories);

            int count = 0;
            foreach (var file in files)
            {
                var extension = Path.GetExtension(file).ToLower();
                var type = AssetType.Unknown;

                if (extension == ".png") type = AssetType.Texture;
                else if (extension == ".json") type = AssetType.Prefab;
                
                if (type != AssetType.Unknown)
                {
                    var job = new AssetJob
                    {
                        SourcePath = file,
                        Type = type
                    };

                    await writer.WriteAsync(job);
                    count++;
                }
            }

            writer.Complete(); 
            Console.WriteLine($"[Scanner] Done. Found {count} assets for processing.");
        }
    }
}