using System.Text.Json;

namespace Dagger.AssetPipeline.Models
{
    public class SchemaField
    {
        public string Name { get; set; } = string.Empty;
        public string Type { get; set; } = string.Empty;

        public bool IsArray { get; set; } = false;
        public int FixedSize { get; set; } = 0;
    }

    public class SchemaRegistry
    {
        public Dictionary<string, List<SchemaField>> Components { get; set; } = [];

        private SchemaRegistry() { }

        private static readonly JsonSerializerOptions JsonOptions = new()
        { 
            PropertyNameCaseInsensitive = true 
        };

        public static SchemaRegistry Load(string schemaPath)
        {
            if (!File.Exists(schemaPath))
            {
                throw new FileNotFoundException($"Schema not found: {schemaPath}");
            }

            var json = File.ReadAllText(schemaPath);

            var registry = new SchemaRegistry
            {
                Components = JsonSerializer.Deserialize<Dictionary<string, List<SchemaField>>>(json, JsonOptions) 
                             ?? []
            };

            return registry;
        }

        public List<SchemaField>? GetFields(string compName)
        {
            return Components.GetValueOrDefault(compName);
        }
    }
}