using CommandLine;
using Dagger.AssetPipeline.Core;
using Dagger.AssetPipeline.Models;

namespace Dagger.AssetPipeline
{
    internal class Program
    {
        public static async Task<int> Main(string[] args)
        {
            return await Parser.Default.ParseArguments<CliOptions>(args)
                .MapResult(
                    RunPipelineAsync,
                    errs => Task.FromResult(1)
                );
        }

        private static async Task<int> RunPipelineAsync(CliOptions opts)
        {
            Console.WriteLine("=== Dagger Asset Pipeline ===");

            // Source directory validation
            if (!Directory.Exists(opts.Source))
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"Error: Source directory '{opts.Source}' doesn't exist.");
                Console.ResetColor();
                return 1;
            }

            // Output directory preparation
            if (!Directory.Exists(opts.Output))
            {
                Directory.CreateDirectory(opts.Output);
            }

            // Pipeline
            try
            {
                Console.WriteLine($"Scanner set to: {opts.Source}");
                Console.WriteLine($"Output set to: {opts.Output}");

                string cachePath = Path.Combine(opts.Output, ".daggercache.json");
                var cache = new BuildCache(cachePath);

                Console.WriteLine("[System] Loading build cache...");
                await cache.LoadAsync();

                var schemaPath = Path.Combine(opts.Source, "schema.json");
                Console.WriteLine($"Loading component definitions from: {schemaPath}");
                var schemaRegistry = SchemaRegistry.Load(schemaPath);

                var runner = new PipelineRunner(opts.Source, opts.Output, schemaRegistry, opts.MaxAtlasSize, cache);
                await runner.RunAsync();

                Console.WriteLine("[System] Saving build cache...");
                await cache.SaveAsync();

                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine("Pipeline completed successfully.");
                Console.ResetColor();

                return 0;
            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"FATAL ERROR: {ex.Message}");
                Console.ResetColor();
                return 1;
            }
        }
    }
}
