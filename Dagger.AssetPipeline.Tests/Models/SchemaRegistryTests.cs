using Dagger.AssetPipeline.Models;

namespace Dagger.AssetPipeline.Tests.Models;

public class SchemaRegistryTests
{
    [Fact]
    public void Load_FileNotFound_ThrowsFileNotFoundException()
    {
        Assert.Throws<FileNotFoundException>(() => 
            SchemaRegistry.Load("non_existent_file.json")
        );
    }
}