using System.Collections.Concurrent;
using System.Text.Json;

namespace Dagger.AssetPipeline.Core
{
    public class BuildCache(string cacheFilePath)
    {
        private ConcurrentDictionary<string, string> _hashes = new();

        private readonly JsonSerializerOptions _saveOptions = new()
        {
            WriteIndented = true
        };

        public bool HasChanged(string path, string currentHash)
        {
            if (_hashes.TryGetValue(path, out string? storedHash))
            {
                return storedHash != currentHash;
            }

            return true;
        }

        public void UpdateHash(string path, string hash)
        {
            _hashes[path] = hash;
        }

        public async Task LoadAsync()
        {
            if (!File.Exists(cacheFilePath)) return;

            try
            {
                await using var stream = File.OpenRead(cacheFilePath);
                var loaded = await JsonSerializer.DeserializeAsync<ConcurrentDictionary<string, string>>(stream);
                if (loaded != null) _hashes = loaded;
            }
            catch
            {
                _hashes = new ConcurrentDictionary<string, string>();
            }
        }

        public async Task SaveAsync()
        {
            await using var stream = File.Create(cacheFilePath);
            await JsonSerializer.SerializeAsync(stream, _hashes, _saveOptions);
        }
    }
}