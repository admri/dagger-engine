using CommandLine;

namespace Dagger.AssetPipeline;

public class CliOptions
{
    [Option('s', "source", Required = true, HelpText = "Path to directory with raw assets (JSON, PNG, WAV).")]
    public string Source { get; set; } = string.Empty;

    [Option('o', "output", Required = true, HelpText = "Path to output directory with compiled binaries and atlases.")]
    public string Output { get; set; } = string.Empty;

    [Option('m', "max-atlas-size", Required = false, Default = 4096, HelpText = "Maximální povolená velikost Texture Atlasu v pixelech.")]
    public int MaxAtlasSize { get; set; }
}