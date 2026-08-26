using System.Threading.Channels;
using Dagger.AssetPipeline.Models;

namespace Dagger.AssetPipeline.Core
{
    public class PipelineRunner(string sourceDir, string outputDir, SchemaRegistry schemaRegistry, int maxAtlasSize, BuildCache cache)
    {
        public async Task RunAsync()
        {
            var channel = Channel.CreateUnbounded<AssetJob>();

            // Producer
            var scanner = new AssetScanner(sourceDir, cache);
            var producerTask = scanner.ScanAsync(channel.Writer);

            // Consumer
            var workerPool = new WorkerPool(outputDir, schemaRegistry, maxAtlasSize, cache);
            int coreCount = Environment.ProcessorCount;
            Console.WriteLine($"[Pipeline] Starting {coreCount} working threads...");

            var consumerTasks = new Task[coreCount];
            for (int i = 0; i < coreCount; i++)
            {
                consumerTasks[i] = workerPool.StartConsumingAsync(channel.Reader, i);
            }

            // Wait for all to end
            await producerTask;
            await Task.WhenAll(consumerTasks);
        }
    }
}