using Dagger.AssetPipeline.Models;
using Dagger.AssetPipeline.Processors;
using Dagger.AssetPipeline.Utils;

namespace Dagger.AssetPipeline.Tests.Processors;

public class PrefabBakerTests : IDisposable
{
    private readonly string _tempDir;

    public PrefabBakerTests()
    {
        // Vytvoříme unikátní dočasnou složku pro každý běh testu
        _tempDir = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString());
        Directory.CreateDirectory(_tempDir);
    }

    [Fact]
    public async Task BakeAsync_GeneratesCorrectByteSizes_ForCppMemoryLayout()
    {
        // 1. PŘÍPRAVA (Arrange)
        string schemaPath = Path.Combine(_tempDir, "schema.json");
        string prefabPath = Path.Combine(_tempDir, "player.json");

        // Schéma, na kterém jsme se dohodli
        string schemaContent = @"
        {
            ""AnimationSequence"": [
            { ""name"": ""name"", ""type"": ""hash"" },
            { ""name"": ""texture"", ""type"": ""handle"" },
            { ""name"": ""startFrame"", ""type"": ""int32"" },
            { ""name"": ""numFrames"", ""type"": ""int32"" },
            { ""name"": ""frameDuration"", ""type"": ""float"" },
            { ""name"": ""isLooping"", ""type"": ""bool"" }
            ],
            ""AnimationComponent"": [
            { ""name"": ""sequences"", ""type"": ""AnimationSequence"", ""isArray"": true, ""fixedSize"": 8 },
            { ""name"": ""sequenceCount"", ""type"": ""int32"" },
            { ""name"": ""currentSequenceHash"", ""type"": ""hash"" },
            { ""name"": ""currentFrameIndex"", ""type"": ""int32"" },
            { ""name"": ""timer"", ""type"": ""float"" },
            { ""name"": ""isPlaying"", ""type"": ""bool"" }
            ],
            ""SpriteComponent"": [
            { ""name"": ""texture"", ""type"": ""handle"" },
            { ""name"": ""width"", ""type"": ""float"" },
            { ""name"": ""height"", ""type"": ""float"" },
            { ""name"": ""zIndex"", ""type"": ""int32"" },
            { ""name"": ""srcX"", ""type"": ""float"" },
            { ""name"": ""srcY"", ""type"": ""float"" },
            { ""name"": ""srcWidth"", ""type"": ""float"" },
            { ""name"": ""srcHeight"", ""type"": ""float"" },
            { ""name"": ""hasSrcRect"", ""type"": ""bool"" },
            { ""name"": ""flipX"", ""type"": ""bool"" }
            ]
        }";

        // Vstupní data entity
        string prefabContent = @"
        {
            ""components"": {
            ""SpriteComponent"": { ""texture"": ""player_atlas"" },
            ""AnimationComponent"": { ""sequenceCount"": 0 }
            }
        }";

        await File.WriteAllTextAsync(schemaPath, schemaContent);
        await File.WriteAllTextAsync(prefabPath, prefabContent);

        var schemaRegistry = SchemaRegistry.Load(schemaPath);
        var baker = new PrefabBaker(schemaRegistry);
        
        var job = new AssetJob { SourcePath = prefabPath, Type = AssetType.Prefab };

        // 2. AKCE (Act)
        await baker.BakeAsync(job, _tempDir);

        // 3. KONTROLA (Assert)
        string outPath = Path.Combine(_tempDir, "player.dbin");
        Assert.True(File.Exists(outPath), "Output .dbin file wasn't created.");

        await using var fs = File.OpenRead(outPath);
        using var reader = new BinaryReader(fs);

        // Kolik komponent se upeklo?
        int compCount = reader.ReadInt32();
        Assert.Equal(2, compCount);

        // Uložíme si velikosti nalezených komponent
        // (Dictionary v JSONu negarantuje pořadí, takže musíme číst bez ohledu na to, co je první)
        var componentSizes = new Dictionary<uint, uint>();

        for (int i = 0; i < compCount; i++)
        {
            uint hash = reader.ReadUInt32();
            uint size = reader.ReadUInt32();
            
            componentSizes[hash] = size;

            // Přeskočíme samotná syrová data, abychom se dostali k hlavičce další komponenty
            reader.BaseStream.Seek(size, SeekOrigin.Current);
        }

        uint spriteHash = HashUtils.HashString("SpriteComponent");
        uint animHash = HashUtils.HashString("AnimationComponent");

        // Ověříme SpriteComponent (Očekáváme 38 bajtů)
        Assert.True(componentSizes.ContainsKey(spriteHash), "SpriteComponent missing in the binary!");
        Assert.Equal(38u, componentSizes[spriteHash]);

        // Ověříme AnimationComponent (Očekáváme 217 bajtů)
        Assert.True(componentSizes.ContainsKey(animHash), "AnimationComponent missing in the binary!");
        Assert.Equal(217u, componentSizes[animHash]);
    }

    // Ukliď po sobě dočasné soubory, ať neplníme disk
    public void Dispose()
    {
        if (Directory.Exists(_tempDir))
        {
            Directory.Delete(_tempDir, true);
        }
    }
}