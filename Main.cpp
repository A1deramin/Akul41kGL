// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

// for debug sleep
#include <thread>

#include "Camera.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "MeshSolidColor.h"
#include "MeshSolidColorWireframe.h"
#include "MeshVertexLit.h"
#include "Parallelepiped.h"
#include "OGLRenderer.h"
#include "Scene.h"
#include "Shader.h"
#include "Shape.h"
#include "Shapes/Prism.h"
#include "Shapes/Pyramid.h"
#include "Shapes/Sphere.h"
#include "Texture.h"
#include "Utils/GLError.h"
#include "Utils/Profile.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void AddSphere(Scene& scene) {
    SpherePtr<VertexBase> sphereShape = std::make_shared<Sphere<VertexBase>>(1.0f);
    MeshSolidColorWireframePtr<VertexBase> sphereMesh =
        std::dynamic_pointer_cast<MeshSolidColorWireframe<VertexBase>>(sphereShape->GetBaseMesh());
    sphereMesh->SetColor(glm::vec4(1.0, 0.3, 0.3, 1));
    sphereMesh->SetLineColor(glm::vec4(0.3, 0.5, 0.7, 1));
    scene.AddObject(sphereShape);
}

void AddPyramid(Scene& scene, OGLRenderer* renderer) {
    PyramidPtr<VertexBase> pyramidShape = std::make_shared<Pyramid<VertexBase>>(3, glm::vec3(0.0f, -3.0f, 0.0f));
    auto pyramidMesh = pyramidShape->GetMesh<MeshSolidColorWireframe>();
    pyramidMesh->SetColor(glm::vec4(0.3, 0.7, 0.3, 1.0));
    pyramidMesh->SetLineColor(glm::vec4(0.7, 0.3, 0.3, 1.0));

    pyramidShape->SetUpdateMethod([pyramidShape, renderer]() {
        int n = pyramidShape->GetLateralFaces();
        if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
            n += 1;
            std::cout << n << "\n";
            pyramidShape->Regenerate(n);
        }
        if (glfwGetKey(renderer->GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
            n -= 1;
            std::cout << n << "\n";
            pyramidShape->Regenerate(n);
        }
    });
    scene.AddObject(pyramidShape);
}

void AddDynamicShapes(Scene& scene) {
    Geometry<VertexBase> pyramid(EBasicGeometry::PYRAMID);

    MeshSolidColorPtr<VertexBase> mesh = std::make_shared<MeshSolidColor<VertexBase>>(
        Mesh<VertexBase>(pyramid.GetVertices(), pyramid.GetIndices(), EDefaultShader::SOLID_COLOR), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    ShapePtr<VertexBase> shape1 = std::make_shared<Shape<VertexBase>>(mesh, glm::vec3(3, 0, 0));
    ShapePtr<VertexBase> shape2 = std::make_shared<Shape<VertexBase>>(mesh, glm::vec3(2, 0, 0));

    shape1->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

    shape1->SetUpdateMethod([shape1]() {
        double deltaX = sin(glfwGetTime()) * 2;
        double deltaY = cos(glfwGetTime()) * 2;
        shape1->SetLocation(glm::vec3(deltaX, -deltaY, 0));
    });
    shape2->SetUpdateMethod([shape2]() { shape2->SetScale(glm::vec3(abs(sin(glfwGetTime())))); });

    scene.AddObject(shape1);
    scene.AddObject(shape2);
}

void AddCustomVertexPyramid(Scene& scene) {
    Geometry<VertexColor> pyramidColor(EBasicGeometry::PYRAMID);

    for (auto& vertexColor : pyramidColor) {
        vertexColor.color = glm::vec4((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0);
    }
    MeshPtr<VertexColor> meshColor =
        std::make_shared<Mesh<VertexColor>>(pyramidColor.GetVertices(), pyramidColor.GetIndices(), EDefaultShader::COLOR);

    ShapePtr<VertexColor> shape3 = std::make_shared<Shape<VertexColor>>(meshColor, glm::vec3(-2, 0, 0));

    shape3->SetUpdateMethod([shape3]() { shape3->SetRotation((float)glfwGetTime() * 0.02, glm::vec3(0.5f, 1.0f, 0.5f)); });
    scene.AddObject(shape3);
}

void AddPrisms(Scene& scene) {
    PrismPtr prism1 = std::make_shared<Prism<VertexBase>>(3, glm::vec3(0, -5, 0));
    PrismPtr prism2 = std::make_shared<Prism<VertexBase>>(4, glm::vec3(3, 0, 0));
    PrismPtr prism3 = std::make_shared<Prism<VertexBase>>(5, glm::vec3(-3, 0, 0));
    PrismPtr prism4 = std::make_shared<Prism<VertexBase>>(6, glm::vec3(0, 5, 0));

    prism1->GetMesh<MeshSolidColorWireframe>()->SetLineWidth(4.0f);

    prism3->GetMesh<MeshSolidColorWireframe>()->SetColor({50.0f, 100.0f, 0.0f, 1.0f});
    prism3->GetMesh<MeshSolidColorWireframe>()->SetLineWidth(2.0f);
    prism3->GetMesh<MeshSolidColorWireframe>()->SetLineColor({255.0f, 0.0f, 0.0f, 1.0f});

    prism2->GetMesh<MeshSolidColorWireframe>()->SetLineColor({255, 0.0f, 255.0f, 1.0f});
    prism2->GetMesh<MeshSolidColorWireframe>()->SetLineWidth(2.0f);

    prism4->GetMesh<MeshSolidColorWireframe>()->SetColor({0.0f, 255.0f, 255.0f, 1.0f});
    prism4->GetMesh<MeshSolidColorWireframe>()->SetLineWidth(2.0f);

    scene.AddObject(prism1);
    scene.AddObject(prism2);
    scene.AddObject(prism3);
    scene.AddObject(prism4);
}

void AddParallelepiped(Scene& scene) {
    ParallelepipedPtr parallelepipedShape =
        std::make_shared<Parallelepiped<VertexBase>>(glm::vec3(0.0f, 3.0f, 0.0f), EMeshType::MESH_SOLID_COLOR_WIREFRAME);
    scene.AddObject(parallelepipedShape);
    parallelepipedShape->SetHeight(0.5);
    parallelepipedShape->SetUpdateMethod(
        [parallelepipedShape]() { parallelepipedShape->AddRotation(sin(glfwGetTime()) / 5, glm::vec3(1.0f, 0, 0)); });

}

void TestAll(Scene& scene, OGLRenderer* renderer) {
    AddParallelepiped(scene);
    AddCustomVertexPyramid(scene);
    AddPyramid(scene, renderer);
    AddSphere(scene);
    AddPrisms(scene);
    AddDynamicShapes(scene);
}

void AddModel(Scene& scene, std::string file = "res/models/dennis.obj") {
    auto model = Geometry<VertexNormalTexture>::LoadObj(file);

    auto modelMesh = std::make_shared<MeshSolidColorWireframe<VertexNormalTexture>>(model, EDefaultShader::SOLID_COLOR_WIREFRAME);

    auto modelShape = std::make_shared<Shape<VertexNormalTexture>>(modelMesh);

    scene.AddObject(modelShape);
}

void AddColorVertexModel(Scene& scene, std::string file = "res/models/dennis.obj") {
    auto model = Geometry<VertexColorNormalTexture>::LoadObj(file);

    for (auto& vert : model) {
        vert.color = glm::vec4((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0);
    }

    auto modelMesh = std::make_shared<Mesh<VertexColorNormalTexture>>(model, EDefaultShader::COLOR);

    auto modelShape = std::make_shared<Shape<VertexColorNormalTexture>>(modelMesh);

    scene.AddObject(modelShape);
}

void AddSolidColorModelBreakEncapsulation(Scene& scene, std::string file = "res/models/dennis.obj") {

    auto model = Geometry<VertexNormalTexture>::LoadObj(file);

    auto modelMesh = std::make_shared<MeshSolidColorWireframe<VertexNormalTexture>>(model, EDefaultShader::SOLID_COLOR);

    modelMesh->GetShader()->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    auto modelShape = std::make_shared<Shape<VertexNormalTexture>>(modelMesh);

    scene.AddObject(modelShape);
}

int main() {
    constexpr int width = 800, height = 800;
    constexpr bool bLogFPS = true;
    std::shared_ptr<OGLRenderer> renderer = std::make_shared<OGLRenderer>(width, height);
    CameraPtr camera = std::make_shared<Camera>(width, height, glm::vec3(0.0f, 0.0f, 5.0));
    Scene scene;

    auto model = Geometry<VertexNormalColor>::LoadObj("res/models/dennis.obj");
    model.GenerateNormals(false);
    auto meshVertLit = std::make_shared<MeshVertexLit<VertexNormalColor>>(model, EDefaultShader::VERTEX_LIGHTING);
    meshVertLit->SetLightPosition({0.0f, 0.0f, 0.0f});
    auto shapeVertLit = std::make_shared<Shape<VertexNormalColor>>(meshVertLit);
    scene.AddObject(shapeVertLit);

    double lastTime = glfwGetTime();
    unsigned int frames = 1;

    while (!glfwWindowShouldClose(renderer->GetWindow())) {
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 1.0f) {
            double delta = (currentTime - lastTime) / frames;
            if (bLogFPS) {
                std::cout << "Frame: " << delta * 1000 << "ms / << fps: " << 1 / delta << "\n";
            }
            frames = 0;
            lastTime = glfwGetTime();
        }

        renderer->Clear();

        scene.Draw(camera);

        camera->Inputs(renderer->GetWindow());

        glfwSwapBuffers(renderer->GetWindow());
        glfwPollEvents();

        frames++;
    }
    return 0;
}
