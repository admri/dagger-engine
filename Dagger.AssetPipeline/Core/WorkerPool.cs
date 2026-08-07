using System.Threading.Channels;
using Dagger.AssetPipeline.Models;
using Dagger.AssetPipeline.Processors;

namespace Dagger.AssetPipeline.Core
{
    public class WorkerPool(string outputDir, SchemaRegistry schemaRegistry, int maxAtlasSize)
    {
        public async Task StartConsumingAsync(ChannelReader<AssetJob> reader, int workerId)
        {
            var prefabBaker = new PrefabBaker(schemaRegistry);
            var spritePacker = new SpritePacker();

            await foreach (var job in reader.ReadAllAsync())
            {
                Console.WriteLine($"[Worker {workerId}] Processing: {Path.GetFileName(job.SourcePath)}");
                
                try
                {
                    if (job.Type == AssetType.Prefab)
                    {
                        Console.WriteLine($"[Worker {workerId}] Baking Prefab: {Path.GetFileName(job.SourcePath)}");
                        await prefabBaker.BakeAsync(job, outputDir);
                    }
                    else if (job.Type == AssetType.TextureAtlas)
                    {
                        string atlasName = Path.GetFileNameWithoutExtension(job.SourcePath);
                        Console.WriteLine($"[Worker {workerId}] Packing Atlas: {atlasName}");
                        await spritePacker.PackAsync(job.SourcePath, outputDir, atlasName, maxAtlasSize);
                    }
                }
                catch (Exception ex)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine($"[Worker {workerId}] ERROR in {job.SourcePath}: {ex.Message}");
                    Console.ResetColor();
                }
            }
        }
    }
}