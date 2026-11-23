using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using Dagger.AssetPipeline.Utils;

namespace Dagger.AssetPipeline.Processors;

public class SpritePacker
{
    private class SpriteInfo
    {
        public string Name { get; init; } = string.Empty;
        public Image<Rgba32> Image { get; init; } = null!;
        public int X { get; set; }
        public int Y { get; set; }
    }

    public async Task PackAsync(string inputDirectory, string outputDirectory, string atlasName, int maxAtlasSize)
    {
        var files = Directory.GetFiles(inputDirectory, "*.png");
        if (files.Length == 0) return;

        var sprites = new List<SpriteInfo>();

        try
        {
            // Load all images into memory
            foreach (var file in files)
            {
                var img = await Image.LoadAsync<Rgba32>(file);
                
                if (img.Width > maxAtlasSize || img.Height > maxAtlasSize)
                {
                    throw new InvalidOperationException($"Image '{Path.GetFileName(file)}' ({img.Width}x{img.Height}px) exceeds maximum atlas size {maxAtlasSize}px!");
                }

                sprites.Add(new SpriteInfo
                {
                    Name = Path.GetFileNameWithoutExtension(file),
                    Image = img
                });
            }

            // Sort images by height, descending
            sprites = [.. sprites.OrderByDescending(s => s.Image.Height)];

            int currentX = 0;
            int currentY = 0;
            int currentRowHeight = 0;
            int finalAtlasWidth = 0;
            int finalAtlasHeight = 0;

            // Calculate positions
            foreach (var sprite in sprites)
            {
                if (currentX + sprite.Image.Width > maxAtlasSize)
                {
                    // Doesn't fit, move to next row
                    currentX = 0;
                    currentY += currentRowHeight;
                    currentRowHeight = 0;
                }

                // Check atlas height overflow
                if (currentY + sprite.Image.Height > maxAtlasSize)
                {
                    throw new InvalidOperationException($"Atlas '{atlasName}' exceeded maximum size {maxAtlasSize}x{maxAtlasSize}px! Split directory into multiple .atlas directories.");
                }

                sprite.X = currentX;
                sprite.Y = currentY;

                currentX += sprite.Image.Width;
                
                if (sprite.Image.Height > currentRowHeight)
                {
                    currentRowHeight = sprite.Image.Height;
                }

                if (currentX > finalAtlasWidth) finalAtlasWidth = currentX;
                if (currentY + currentRowHeight > finalAtlasHeight) finalAtlasHeight = currentY + currentRowHeight;
            }

            // Create atlas
            using var atlasImage = new Image<Rgba32>(finalAtlasWidth, finalAtlasHeight);

            atlasImage.Mutate(ctx =>
            {
                foreach (var sprite in sprites)
                {
                    ctx.DrawImage(sprite.Image, new Point(sprite.X, sprite.Y), 1f);
                }
            });

            // Save PNG
            string pngPath = Path.Combine(outputDirectory, $"{atlasName}.png");
            await atlasImage.SaveAsPngAsync(pngPath);

            // Save binary metadata
            string binPath = Path.Combine(outputDirectory, $"{atlasName}.dbin");
            await using var fs = new FileStream(binPath, FileMode.Create, FileAccess.Write, FileShare.None);
            await using var bw = new BinaryWriter(fs);

            bw.Write(sprites.Count);

            foreach (var sprite in sprites)
            {
                bw.Write(HashUtils.HashString(sprite.Name));
                bw.Write((short)sprite.X);
                bw.Write((short)sprite.Y);
                bw.Write((short)sprite.Image.Width);
                bw.Write((short)sprite.Image.Height);
            }

            Console.WriteLine($"[SpritePacker] Created atlas: {atlasName}.png ({finalAtlasWidth}x{finalAtlasHeight} px, {sprites.Count} textures)");
        }
        finally
        {
            foreach (var sprite in sprites)
            {
                sprite.Image.Dispose();
            }
        }
    }
}