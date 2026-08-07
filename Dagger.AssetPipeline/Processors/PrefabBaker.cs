using System.Text.Json.Nodes;
using Dagger.AssetPipeline.Models;
using Dagger.AssetPipeline.Utils;

namespace Dagger.AssetPipeline.Processors;

public class PrefabBaker(SchemaRegistry schemaRegistry)
{
    public async Task BakeAsync(AssetJob job, string outputDir)
    {
        var jsonContent = await File.ReadAllTextAsync(job.SourcePath);

        var root = JsonNode.Parse(jsonContent);
        var componentsNode = root?["components"]?.AsObject();

        if (componentsNode == null) return;

        var outPath = Path.Combine(outputDir, Path.GetFileNameWithoutExtension(job.SourcePath) + ".dbin");
        await using var fs = new FileStream(outPath, FileMode.Create, FileAccess.Write, FileShare.None);
        await using var finalWriter = new BinaryWriter(fs);

        finalWriter.Write(componentsNode.Count);

        foreach (var (compName, value) in componentsNode)
        {
            var compData = value?.AsObject();

            if (!schemaRegistry.Components.ContainsKey(compName) || compData == null)
            {
                Console.WriteLine($"[WARNING] Component {compName} not in schema!");
                continue;
            }

            using var ms = new MemoryStream();
            await using var bw = new BinaryWriter(ms);

            WriteStruct(bw, compData, compName);

            finalWriter.Write(HashUtils.HashString(compName));
            finalWriter.Write((uint)ms.Length);
            finalWriter.Write(ms.ToArray());
        }
    }

    private void WriteStruct(BinaryWriter bw, JsonObject? jsonData, string structName)
    {
        var schemaFields = schemaRegistry.GetFields(structName);
        if (schemaFields == null) return;

        foreach (var field in schemaFields)
        {
            var jsonNode = jsonData?[field.Name];

            if (field.IsArray)
            {
                var jsonArray = jsonNode?.AsArray();
                int elementsInJson = jsonArray?.Count ?? 0;

                for (int i = 0; i < field.FixedSize; i++)
                {
                    var elementNode = i < elementsInJson ? jsonArray![i] : null;
                    WriteField(bw, elementNode, field.Type);
                }
            }
            else
            {
                WriteField(bw, jsonNode, field.Type);
            }
        }
    }

    private void WriteField(BinaryWriter bw, JsonNode? jsonNode, string typeName)
    {
        switch (typeName)
        {
            case "float":
                bw.Write(jsonNode != null ? (float)jsonNode : 0.0f);
                break;

            case "int32":
                bw.Write(jsonNode != null ? (int)jsonNode : 0);
                break;

            case "bool":
                bw.Write((byte)(jsonNode != null && (bool)jsonNode ? 1 : 0));
                break;

            case "hash":
                bw.Write(jsonNode != null ? HashUtils.HashString((string)jsonNode!) : 0u);
                break;

            case "handle":
                bw.Write(jsonNode != null ? HashUtils.HashString((string)jsonNode!) : 0u);
                bw.Write(0u);
                break;

            default:
                WriteStruct(bw, jsonNode?.AsObject(), typeName);
                break;
        }
    }
}