namespace Dagger.AssetPipeline.Models
{
    public enum AssetType
    {
        Texture,
        Prefab,
        Level,
        Unknown
    }

    public class AssetJob
    {
        public string SourcePath { get; set; } = string.Empty;
        public AssetType Type { get; set; }
    }
}