#include <vector>
#include <DirectXMath.h>
#include <cstdlib>

#include "../Layouts/VertexLayout.h"

struct Sphere
{
    unsigned int IndicesCount = 0U;
    std::vector<Vertex> Vertices;
    std::vector<int> Indices;

    void Generate(float Parallels, float Meridians, float Radius)
    {
        for (float i = 0; i <= Parallels; i++)
        {
            float theta = i * DirectX::XM_PI / Parallels;
            float sinTheta =static_cast<float>(sin(theta));
            float cosTheta = static_cast<float>(cos(theta));

            for (float j = 0; j <= Meridians; j++)
            {
                float phi = j * 2 * DirectX::XM_PI / Meridians;
                float sinPhi = static_cast<float>(sin(phi));
                float cosPhi = static_cast<float>(cos(phi));

                Vertex vs;
                vs.Normal.x = cosPhi * sinTheta;
                vs.Normal.y = cosTheta;
                vs.Normal.z = sinPhi * sinTheta;

                vs.Tangent.x = -Radius * sinPhi;
                vs.Tangent.y = 0.0f;
                vs.Tangent.z = Radius * cosPhi;

                vs.Bitangent.x = Radius * cosPhi * cosTheta;
                vs.Bitangent.y = -Radius * sinTheta;
                vs.Bitangent.z = Radius * sinPhi * cosTheta;

                vs.Position.x = Radius * vs.Normal.x;
                vs.Position.y = Radius * vs.Normal.y;
                vs.Position.z = Radius * vs.Normal.z;

                vs.TexCoord.x = 1.0f - (j / Meridians);
                vs.TexCoord.y = 1.0f - (i / Parallels);

                vs.Color.x = static_cast<float>(rand()) / RAND_MAX;
                vs.Color.y = static_cast<float>(rand()) / RAND_MAX;
                vs.Color.z = static_cast<float>(rand()) / RAND_MAX;
                vs.Color.w = 1.0f;

                Vertices.push_back(vs);
            }
        }

        for (int k = 0; k < static_cast<int>(Parallels); k++)
        {
            for (int l = 0; l < static_cast<int>(Meridians); l++)
            {
                int a = static_cast<int>(k * (Meridians + 1)) + l;
                int b = a + static_cast<int>(Meridians) + 1;

                Indices.push_back(a);
                Indices.push_back(b);
                Indices.push_back(a + 1);

                Indices.push_back(b);
                Indices.push_back(b + 1);
                Indices.push_back(a + 1);
            }
        }

        IndicesCount = static_cast<unsigned int>(Indices.size());
    }
};