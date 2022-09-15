#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "VertexBuffer.h"
#include <execution>
#include "Profile.h"

enum class EBasicGeometry {
    NONE,
    CUBE,
    PYRAMID,
};

namespace GeometryOBJUtils {
static constexpr auto kObjCustomHeader = "#Normalnyi obj 1.0";
static constexpr auto kConvertedNamePostfix = "_a1";
static constexpr auto kVertexCountPrefix = "#vcount";
static constexpr auto kNormalCountPrefix = "#vncount";
static constexpr auto kUVCountPrefix = "#vtcount";
static constexpr auto kFaceCountPrefix = "#fcount";

static void ConvertObjToCustom(const std::string& fileName) {
    std::ifstream input(fileName);
    assert(input.is_open());

    std::string content((std::istreambuf_iterator(input)), std::istreambuf_iterator<char>());

    const auto vertexNormalUVCount = std::count(std::execution::par_unseq, std::begin(content), std::end(content), 'v');
    constexpr auto vertexCountDevisor = 2;
    const auto vertexCount = vertexNormalUVCount / vertexCountDevisor;
    const auto uvCount = vertexCount, normalCount = vertexCount;
    const auto faceCount = std::count(std::execution::par_unseq, std::begin(content), std::end(content), 'f');
    const std::string convertedName = fileName + kConvertedNamePostfix;
    std::ofstream convertedFile(convertedName, std::ofstream::trunc);
    convertedFile.clear();
    convertedFile << kObjCustomHeader << "\n";

    convertedFile << kVertexCountPrefix << " " << vertexCount << '\n';
    convertedFile << kNormalCountPrefix << " " << normalCount << '\n';
    convertedFile << kUVCountPrefix << " " << uvCount << '\n';
    convertedFile << kFaceCountPrefix << " " << faceCount << '\n';

    convertedFile << content;
    convertedFile.close();

    input.close();
}
} // namespace GeometryFormatUtils

template <class Vertex>
class Geometry {
public:
    Geometry() = default;
    Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    Geometry(EBasicGeometry geometryType);


    /**
     * Generate vertices and indices.
     */
    static Geometry GeneratePolygon(unsigned int n, float height = 1.0f, float r = 1.0f);
    static Geometry GeneratePyramid(unsigned int n, float height = 1.0f, float r = 1.0f);
    static Geometry GeneratePrism(unsigned int n, float height = 1.0f, float r = 1.0f);
    static Geometry GenerateSphere(float r = 1.0f, unsigned int sectorCount = 50, unsigned int stackCount = 50);

    static Geometry<Vertex> LoadObj(const std::string& file, bool bUseCustomOBJ = true);
    void AddVertex(const Vertex& vertex) { m_Vertices.push_back(vertex); }
    void AddVertex(const glm::vec3& pos) { m_Vertices.emplace_back(pos); }
    void AddIndex(unsigned int index) { m_Indices.push_back(index); }
    void AddIndices(std::initializer_list<unsigned int> l) { m_Indices.insert(std::end(m_Indices), l); }
    void MergeWith(Geometry& source);

    void ReserveVertices(unsigned int n) { m_Vertices.reserve(n); }
    void ReserveIndices(unsigned int n) { m_Indices.reserve(n); }

    void SetPositions(const std::vector<Vertex>& vertices) { m_Vertices = vertices; };
    void SetIndices(const std::vector<unsigned int>& indices) { m_Indices = indices; };

    void GenerateNormals(bool bFlatShading = false);

    std::vector<Vertex>& GetVertices() { return m_Vertices; };
    std::vector<unsigned int>& GetIndices() { return m_Indices; };
    const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
    const std::vector<unsigned int>& GetIndices() const { return m_Indices; }
    size_t GetNumVertices() const { return m_Vertices.size(); };
    size_t GetNumIndices() const { return m_Indices.size(); };
    bool Empty() const { return GetNumVertices() == 0 && GetNumIndices() == 0; }

    auto begin();
    auto end();
    auto cbegin() const;
    auto cend() const;

private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

};

template <class Vertex>
void Geometry<Vertex>::MergeWith(Geometry& source) {
    unsigned int destVerts = GetNumVertices();
    m_Vertices.insert(std::end(m_Vertices), std::make_move_iterator(std::begin(source.GetVertices())),
        std::make_move_iterator(std::end(source.GetVertices())));
    auto& sourceIndices = source.GetIndices();
    for (auto& i : sourceIndices) {
        i += destVerts;
    }
    m_Indices.insert(std::end(m_Indices), std::begin(sourceIndices), std::end(sourceIndices));
}

template <class Vertex>
void Geometry<Vertex>::GenerateNormals(bool bFlatShading) {
    if (Empty()) {
        return;
    }
    auto& vertices = GetVertices();
    auto& indices = GetIndices();

    for (size_t i = 0; i < indices.size() - 2; i += 3) {
        auto& vertA = vertices[indices[i]];
        auto& vertB = vertices[indices[i + 1]];
        auto& vertC = vertices[indices[i + 2]];
        if (bFlatShading) {
            glm::vec3 flatNormal = glm::normalize(glm::cross(vertB.position - vertA.position, vertC.position - vertA.position));
            vertA.normal = flatNormal;
            vertB.normal = flatNormal;
            vertC.normal = flatNormal;
        } else {
            glm::vec3 perpendicular = glm::cross(vertB.position - vertA.position, vertC.position - vertA.position);
            vertA.normal += perpendicular;
            vertB.normal += perpendicular;
            vertC.normal += perpendicular;
        }
    }
    if (!bFlatShading) {
        for (auto& vert : vertices) {
            vert.normal = glm::normalize(vert.normal);
        }
    }
}

/*http://www.songho.ca/opengl/gl_sphere.html*/
template <class Vertex>
Geometry<Vertex> Geometry<Vertex>::GenerateSphere(float r, unsigned int sectorCount, unsigned int stackCount) {
    Geometry<Vertex> sphereGeometry;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    for (unsigned int i = 0; i <= stackCount; i++) {
        float stackAngle = glm::pi<float>() / 2 - i * stackStep; // starting from pi/2 to -pi/2
        float xy = r * glm::cos(stackAngle);                     // r * cos(u)
        float z = r * glm::sin(stackAngle);                      // r * sin(u)
        for (unsigned int j = 0; j <= sectorCount; j++) {
            float sectorAngle = j * sectorStep; // starting from 0 to 2pi
            // vertex position (x, y, z)
            float x = xy * glm::cos(sectorAngle); // r * cos(u) * cos(v)
            float y = xy * glm::sin(sectorAngle); // r * cos(u) * sin(v)
            sphereGeometry.AddVertex(glm::vec3(x, y, z));
        }
    }

    // generate CCW index list of sphere triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    for (unsigned int i = 0; i < stackCount; i++) {
        unsigned int k1 = i * (sectorCount + 1); // beginning of current stack
        unsigned int k2 = k1 + sectorCount + 1;  // beginning of next stack

        for (unsigned int j = 0; j < sectorCount; j++, k1++, k2++) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                sphereGeometry.AddIndices({k1, k2, k1 + 1});
            }
            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1)) {
                sphereGeometry.AddIndices({k1 + 1, k2, k2 + 1});
            }
        }
    }
    return sphereGeometry;
}


// TODO: Profile.
template <class Vertex>
Geometry<Vertex> Geometry<Vertex>::LoadObj(const std::string& file, bool bUseCustomOBJ) {

    std::ifstream fin(file, std::ifstream::in);
    assert(fin.is_open());
    std::string objHeader;
    std::getline(fin, objHeader);
    bUseCustomOBJ = false;
    if (bUseCustomOBJ) {
        if (objHeader != GeometryOBJUtils::kObjCustomHeader) {
            GeometryOBJUtils::ConvertObjToCustom(file);
            return LoadObj(file + GeometryOBJUtils::kConvertedNamePostfix);
        }
    }
    LOG_DURATION("LoadOBJ");
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> vertexUVs;
    std::vector<glm::vec3> vertexNormals;

    std::string line;

    while (std::getline(fin, line)) {
        std::istringstream istream{line};
        std::string header;

        istream >> header;

        if (header == GeometryOBJUtils::kVertexCountPrefix) {
            size_t vertexCount;
            istream >> vertexCount;
            vertices.reserve(vertexCount);
        } else if (header == GeometryOBJUtils::kNormalCountPrefix) {
            size_t vnCount;
            istream >> vnCount;
            vertexNormals.reserve(vnCount);
        } else if (header == GeometryOBJUtils::kUVCountPrefix) {
            size_t uvCount;
            istream >> uvCount;
            vertexUVs.reserve(uvCount);
        } else if (header == GeometryOBJUtils::kFaceCountPrefix) {
            size_t indexCount;
            istream >> indexCount;
            indices.reserve(indexCount * 2);
        } else if (header == "v") {
            Vertex vertex{};
            istream >> vertex.position.x >> vertex.position.y >> vertex.position.z;
            vertices.push_back(vertex);
        } else if (header == "vn") {
            glm::vec3 normal;
            istream >> normal.x >> normal.y >> normal.z;
            vertexNormals.push_back(normal);
        } else if (header == "vt") {
            glm::vec2 uv;
            istream >> uv.x >> uv.y;
            vertexUVs.push_back(uv);
        } else if (header == "f") {
            const int vertexMaxRank = 4;
            for (int i = 0; i < vertexMaxRank; i++) {
                std::string vertexIndexData;
                std::string delim;

                bool bNormalAbsent = false;
                bool bTextureAbsent = false;
                bool bNormalTexutreAbsent = false;

                if (line.find("//") != std::string::npos) {
                    delim = "//";
                    bTextureAbsent = true;
                } else if (line.find("/") != std::string::npos) {
                    delim = "/";

                } else {
                    delim = " ";
                    bNormalTexutreAbsent = true;
                }
                istream >> vertexIndexData;
                if (vertexIndexData.empty()) {
                    break;
                }
                auto startTokenizing = static_cast<std::string::size_type>(0);
                auto endTokenizing = vertexIndexData.find(delim);
                /*    0   1    2
                 *  Index/UV/Normal */
                std::vector<int> vertexIndexDataTokenised;

                while (endTokenizing != std::string::npos) {
                    vertexIndexDataTokenised.push_back(std::stoi(vertexIndexData.substr(startTokenizing, endTokenizing - startTokenizing)));
                    startTokenizing = endTokenizing + delim.size();
                    endTokenizing = vertexIndexData.find(delim, startTokenizing);
                }
                vertexIndexDataTokenised.push_back(std::stoi(vertexIndexData.substr(startTokenizing, endTokenizing - startTokenizing)));

                for (auto& data : vertexIndexDataTokenised) {
                    if (data < 0) {
                        data += vertices.size() + 1;
                    }
                }

                if (vertexIndexDataTokenised.size() == 2 && delim == "/") {
                    bNormalAbsent = true;
                }

                // Link parsed data to vertex.
                indices.push_back(vertexIndexDataTokenised[0] - 1);
                if (!bNormalTexutreAbsent) {
                    /////////////////////
                    // TODO: Add texture support.
                    if (!bTextureAbsent) {
                        //vertices[vertexIndexDataTokenised[0] - 1].texUV = vertexUVs[vertexIndexDataTokenised[1] - 1];
                    }
                    if (!bNormalAbsent) {
                        vertices[vertexIndexDataTokenised[0] - 1].normal =
                            vertexNormals[vertexIndexDataTokenised[vertexIndexDataTokenised.size() - 1] - 1];
                    }
                }

                // Link quad
                if (i == 3) {
                    indices.push_back(indices[indices.size() - 2]);
                    indices.push_back(indices[indices.size() - 5]);
                }
            }
        }
    }
    constexpr bool bLogStat = true;
    if (bLogStat) {
        std::cout << "verts: " << vertices.size() << " inds " << indices.size() << " norms " << vertexNormals.size() << " uvs "
            << vertexUVs.size() << std::endl;
    }
    return {vertices, indices};
}

template <class Vertex>
Geometry<Vertex> Geometry<Vertex>::GeneratePrism(unsigned int n, float height /*= 1.0f*/, float r /*= 1.0f*/) {
    // generate upper base
    Geometry<Vertex> prism = Geometry<Vertex>::GeneratePolygon(n, 1.0f, r);
    prism.ReserveIndices(12 * n);
    // generate lower body and move data to the prism geometry
    Geometry<Vertex> lowerBody = Geometry<Vertex>::GeneratePolygon(n, -height, r);
    prism.MergeWith(lowerBody);
    for (unsigned int i = 1; i < n; i++) {
        prism.AddIndices({i, i + 1, n + 1 + i});
        prism.AddIndices({n + 2 + i, n + 1 + i, i + 1});
    }
    prism.AddIndices({n, 1, n + n + 1});
    prism.AddIndices({n + n + 1, n + 2, 1});

    return prism;
}

template <class Vertex>
Geometry<Vertex> Geometry<Vertex>::GeneratePyramid(unsigned int n, float height /*= 1.0f*/, float r /*= 1.0f*/) {
    // generate base polygon
    Geometry<Vertex> pyramid = Geometry<Vertex>::GeneratePolygon(n, height, r);

    pyramid.AddVertex(glm::vec3(0.0f, 1, 0.0f)); // apex
    unsigned int apx = n + 1;
    // connect apex with all vertices

    for (unsigned int i = 2; i < n + 1; i++) {
        pyramid.AddIndices({apx, i, i - 1});
    }
    pyramid.AddIndices({apx, n, 1});
    return pyramid;

}

template <class Vertex>
Geometry<Vertex>::Geometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : m_Vertices(vertices),
      m_Indices(indices) {
}

template <class Vertex>
Geometry<Vertex> Geometry<Vertex>::GeneratePolygon(unsigned int n, float height /*= 0.0f*/, float r /*= 1.0f*/) {
    std::vector<Vertex> vertices;
    vertices.reserve(n + 1);
    std::vector<unsigned int> indices;
    indices.reserve(n * 3);

    float angle = 2 * glm::pi<float>() / n;
    float normalHeight = 1 - glm::clamp<float>(height, 0.0f, 2.0f);

    vertices.emplace_back(glm::vec3(0.0f, normalHeight, 0.0f)); // center; 0 index
    for (unsigned int i = 0; i < n; i++) {
        float x = r * glm::cos(i * angle);
        float y = r * glm::sin(i * angle);

        vertices.emplace_back(glm::vec3(x, normalHeight, y));
        if (i > 0) {
            // custom triangle strip
            indices.insert(std::end(indices), {0, i, i + 1});
        }
    }
    indices.insert(std::end(indices), {0, n, 1}); // close
    return Geometry<Vertex>{vertices, indices};
}

template <class Vertex>
auto Geometry<Vertex>::cbegin() const {
    return std::cbegin(m_Vertices);
}

template <class Vertex>
auto Geometry<Vertex>::cend() const {
    return std::cend(m_Vertices);
}

template <class Vertex>
auto Geometry<Vertex>::begin() {
    return std::begin(m_Vertices);
}

template <class Vertex>
auto Geometry<Vertex>::end() {
    return std::end(m_Vertices);
}

template <class Vertex>
Geometry<Vertex>::Geometry(EBasicGeometry geometryType) {
    switch (geometryType) {
        case EBasicGeometry::PYRAMID: m_Vertices.emplace_back(glm::vec3(-0.5f, 0.0f, 0.5f));
            m_Vertices.emplace_back(glm::vec3(-0.5f, 0.0f, -0.5f));
            m_Vertices.emplace_back(glm::vec3(0.5f, 0.0f, -0.5f));
            m_Vertices.emplace_back(glm::vec3(0.5f, 0.0f, 0.5f));
            m_Vertices.emplace_back(glm::vec3(0.0f, 0.8f, 0.0f));

            m_Indices = {0, 1, 2, 0, 2, 3, 0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4};
            break;

        case EBasicGeometry::CUBE: m_Vertices.emplace_back(glm::vec3{-1.0, -1.0, 1.0});
            m_Vertices.emplace_back(glm::vec3{1.0, -1.0, 1.0});
            m_Vertices.emplace_back(glm::vec3{1.0, 1.0, 1.0});
            m_Vertices.emplace_back(glm::vec3{-1.0, 1.0, 1.0});
            m_Vertices.emplace_back(glm::vec3{-1.0, -1.0, -1.0});
            m_Vertices.emplace_back(glm::vec3{1.0, -1.0, -1.0});
            m_Vertices.emplace_back(glm::vec3{1.0, 1.0, -1.0});
            m_Vertices.emplace_back(glm::vec3{-1.0, 1.0, -1.0});

            m_Indices = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4, 4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3};
            break;
    }
}
