namespace Dagger.AssetPipeline.Models
{
    public enum AssetType
    {
        Unknown,
        Prefab,
        TextureAtlas
    }

    public class AssetJob
    {
        public string SourcePath { get; set; } = string.Empty;
        public AssetType Type { get; set; }
    }
}