using System.Threading.Channels;
using Dagger.AssetPipeline.Models;
using Dagger.AssetPipeline.Processors;

namespace Dagger.AssetPipeline.Core
{
    public class WorkerPool(string outputDir, SchemaRegistry schemaRegistry)
    {
        public async Task StartConsumingAsync(ChannelReader<AssetJob> reader, int workerId)
        {
            var prefabBaker = new PrefabBaker(schemaRegistry);

            await foreach (var job in reader.ReadAllAsync())
            {
                Console.WriteLine($"[Worker {workerId}] Processing: {Path.GetFileName(job.SourcePath)}");
                
                try
                {
                    if (job.Type == AssetType.Prefab)
                    {
                        await prefabBaker.BakeAsync(job, outputDir);
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