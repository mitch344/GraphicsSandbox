#include <vector>
#include <DirectXMath.h>
#include <cstdlib>

#include "../Layouts/VertexLayout.h"

struct Torus
{
    unsigned int IndicesCount = 0U;
    std::vector<Vertex> Vertices;
    std::vector<int> Indices;

    void Generate(float majorRadius, float minorRadius, int numMajorSegments, int numMinorSegments)
    {
        for (int i = 0; i <= numMajorSegments; ++i)
        {
            float majorAngle = 2.0f * DirectX::XM_PI * static_cast<float>(i) / static_cast<float>(numMajorSegments);
            float cosMajor = static_cast<float>(cos(majorAngle));
            float sinMajor = static_cast<float>(sin(majorAngle));

            for (int j = 0; j <= numMinorSegments; ++j)
            {
                float minorAngle = 2.0f * DirectX::XM_PI * static_cast<float>(j) / static_cast<float>(numMinorSegments);
                float cosMinor = static_cast<float>(cos(minorAngle));
                float sinMinor = static_cast<float>(sin(minorAngle));

                float x = (majorRadius + minorRadius * cosMinor) * cosMajor;
                float y = (majorRadius + minorRadius * cosMinor) * sinMajor;
                float z = minorRadius * sinMinor;

                Vertex vs;
                vs.Position = DirectX::XMFLOAT3(x, y, z);
                vs.Normal = DirectX::XMFLOAT3(cosMajor * cosMinor, sinMajor * cosMinor, sinMinor);

                vs.Tangent = DirectX::XMFLOAT3(-sinMajor, cosMajor, 0.0f);
                vs.Bitangent = DirectX::XMFLOAT3(-cosMajor * sinMinor, -sinMajor * sinMinor, cosMinor);

                vs.TexCoord.x = 1.0f - static_cast<float>(i) / static_cast<float>(numMajorSegments);
                vs.TexCoord.y = 1.0f - static_cast<float>(j) / static_cast<float>(numMinorSegments);

                vs.Color.x = static_cast<float>(rand()) / RAND_MAX;
                vs.Color.y = static_cast<float>(rand()) / RAND_MAX;
                vs.Color.z = static_cast<float>(rand()) / RAND_MAX;
                vs.Color.w = 1.0f;

                Vertices.push_back(vs);
            }
        }

        for (int i = 0; i < numMajorSegments; ++i)
        {
            for (int j = 0; j < numMinorSegments; ++j)
            {
                int a = i * (numMinorSegments + 1) + j;
                int b = a + numMinorSegments + 1;

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