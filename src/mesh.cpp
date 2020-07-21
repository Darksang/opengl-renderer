#include "mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <unordered_map>

void Mesh::LoadOBJ(const std::string & File) {
    std::unordered_map<Vertex, unsigned int> UniqueVertices;

    tinyobj::attrib_t Attributes;
    std::vector<tinyobj::shape_t> Shapes;
    std::vector<tinyobj::material_t> Materials;
    std::string Warning, Error;

    if (tinyobj::LoadObj(&Attributes, &Shapes, &Materials, &Warning, &Error, File.c_str())) {
        for (const auto & Shape : Shapes) {
            for (const auto & Index : Shape.mesh.indices) {
                Vertex V{};

                V.Position = {
                    Attributes.vertices[3 * Index.vertex_index],
                    Attributes.vertices[3 * Index.vertex_index + 1],
                    Attributes.vertices[3 * Index.vertex_index + 2]
                };

                V.Normal = {
                    Attributes.normals[3 * Index.normal_index],
                    Attributes.normals[3 * Index.normal_index + 1],
                    Attributes.normals[3 * Index.normal_index + 2],
                };

                V.TexCoords = {
                    Attributes.texcoords[2 * Index.texcoord_index],
                    Attributes.texcoords[2 * Index.texcoord_index + 1]
                };

                // If this vertex isn't duplicate, add it
                if (UniqueVertices.count(V) == 0) {
                    UniqueVertices[V] = static_cast<unsigned int>(VertexBuffer.size());
                    VertexBuffer.push_back(V); 
                } 

                IndexBuffer.push_back(UniqueVertices[V]);
            }
        }

        std::cout << "\n#### OBJ Mesh: " << File << " ####" << std::endl;
        std::cout << "Vertices = " << VertexBuffer.size() << std::endl;
        std::cout << "Indices = " << IndexBuffer.size() << std::endl;
        std::cout << "Triangles = " << IndexBuffer.size() / 3 << std::endl;
        std::cout << "####" << std::endl;

        SetupMesh();
    }
}

void Mesh::LoadTexture(const std::string & File) {
    glGenTextures(1, &MeshTexture.ID);

    stbi_set_flip_vertically_on_load(true);

    int Width, Height, NumChannels;
    stbi_uc * Data = stbi_load(File.c_str(), &Width, &Height, &NumChannels, 0);

    if (Data) {
        GLenum Format;

        if (NumChannels == 3)
            Format = GL_RGB;
        else if (NumChannels == 4)
            Format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, MeshTexture.ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "\n#### Texture: " << File << " ####" << std::endl;
        std::cout << "Width: " << Width << std::endl;
        std::cout << "Height: " << Height << std::endl;
        std::cout << "Channels: " << NumChannels << std::endl;
        std::cout << "####" << std::endl;

        stbi_image_free(Data);
    }
}

void Mesh::Draw(Shader & Shader) {
    glActiveTexture(GL_TEXTURE0);
    Shader.SetInt("TextureDiffuse", 0);
    glBindTexture(GL_TEXTURE_2D, MeshTexture.ID);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, IndexBuffer.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh() {
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Load data into buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, VertexBuffer.size() * sizeof(Vertex), &VertexBuffer[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer.size() * sizeof(unsigned int), &IndexBuffer[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}