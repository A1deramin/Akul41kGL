#pragma once
#include "Mesh.h"
#include "MeshSolidColor.h"
#include "MeshSolidColorWireframe.h"
#include "MeshMaterial.h"

class MeshUtils {
public:
    /** Get appropriate shader type based on mesh and vertex type.
     *  @param meshType predefined mesh type
     */
    template <class Vertex>
    static EDefaultShader GetDefaultShaderBasedOnMeshType(EMeshType meshType);

    /**
     * Construct Mesh object based on geometry representation
     * @param meshType predefined mesh type.
     * @param basicGeometry predefined geometry representation. If specified NONE the geometry param will be used.
     * @param shader predefined shader type. If specified NONE the shader will be constructed based on mesh and vertex type. @see GetDefaultShaderBasedOnMeshType
     * @param geometry underlying geometry. Will be used iff basicGeometry is NONE.
     */
    template <class Vertex>
    static MeshPtr<Vertex> GetMeshByType(
        EMeshType meshType,
        EBasicGeometry basicGeometry = EBasicGeometry::NONE,
        EDefaultShader shader = EDefaultShader::NONE,
        const Geometry<Vertex>& geometry = {});


};

template <class Vertex>
EDefaultShader MeshUtils::GetDefaultShaderBasedOnMeshType(EMeshType meshType) {
    switch (meshType) {
        case EMeshType::MESH: if (std::is_base_of_v<VertexColor, Vertex>) {
                return EDefaultShader::COLOR;
            }

            return EDefaultShader::DEFAULT;
            break;

        case EMeshType::MESH_SOLID_COLOR: return EDefaultShader::SOLID_COLOR;
            break;
        case EMeshType::MESH_SOLID_COLOR_WIREFRAME: return EDefaultShader::SOLID_COLOR_WIREFRAME;
            break;
        case EMeshType::MESH_MATERIAL: return EDefaultShader::LIGHTING;
            break;
    }
}


template <class Vertex>
static MeshPtr<Vertex> MeshUtils::GetMeshByType(
    EMeshType meshType,
    EBasicGeometry basicGeometry,
    EDefaultShader shader,
    const Geometry<Vertex>& geometry) {
    if (shader == EDefaultShader::NONE) {
        shader = GetDefaultShaderBasedOnMeshType<Vertex>(meshType);
    }
    switch (meshType) {
        case EMeshType::MESH: if (basicGeometry != EBasicGeometry::NONE) {
                return std::make_shared<Mesh<Vertex>>(basicGeometry, shader);
            }
            return std::make_shared<Mesh<Vertex>>(geometry, shader);

        case EMeshType::MESH_SOLID_COLOR: if (basicGeometry != EBasicGeometry::NONE) {
                return std::make_shared<MeshSolidColor<Vertex>>(basicGeometry, shader);
            }
            return std::make_shared<MeshSolidColor<Vertex>>(geometry, shader);

        case EMeshType::MESH_SOLID_COLOR_WIREFRAME: if (basicGeometry != EBasicGeometry::NONE) {
                return std::make_shared<MeshSolidColorWireframe<Vertex>>(basicGeometry,
                    shader);
            }
            return std::make_shared<MeshSolidColorWireframe<Vertex>>(geometry, shader);

        case EMeshType::MESH_MATERIAL: {
            if (basicGeometry != EBasicGeometry::NONE) {
                return std::make_shared<MeshMaterial<Vertex>>(basicGeometry, shader);
            }
            return std::make_shared<MeshMaterial<Vertex>>(geometry, shader);
        }

    }
}
