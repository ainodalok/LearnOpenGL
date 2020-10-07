#include "Mesh.h"
#include "Model.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int> &indices, const std::vector<Texture>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::draw(bool noTex, GLuint materialUBOId)
{
    if (!noTex)
    {
        Model::MaterialUBO materialUBO = { 0, 0, 0, 0 };
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            switch (textures[i].type)
            {
            case DIFFUSE:
                materialUBO.diffuseUsed = 1;
                break;
            case SPECULAR:
                materialUBO.specularUsed = 1;
                break;
            case NORMAL:
                materialUBO.normalMapUsed = 1;
                break;
            case HEIGHT:
                materialUBO.heightMapUsed = 1;
                break;
            }
            // Supports only one texture per type
            // DIFFUSE, SPECULAR, NORMAL, HEIGHT
            glActiveTexture(GL_TEXTURE0 + textures[i].type);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, materialUBOId);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(materialUBO), &materialUBO);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, bitangent));
}
