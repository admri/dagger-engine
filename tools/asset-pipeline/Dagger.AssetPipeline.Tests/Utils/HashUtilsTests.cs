using Dagger.AssetPipeline.Utils;

namespace Dagger.AssetPipeline.Tests.Utils;

public class HashUtilsTests
{
    [Fact]
    public void HashString_ReturnsCorrectFnv1aHash()
    {
        Assert.Equal(0x0A4098FBu, HashUtils.HashString("SpriteComponent"));
        Assert.Equal(0xA5A1BF78u, HashUtils.HashString("AnimationComponent"));
    }

    [Fact]
    public void HashString_EmptyOrNull_ReturnsZero()
    {
        Assert.Equal(0u, HashUtils.HashString(""));
        Assert.Equal(0u, HashUtils.HashString(null!));
    }
}