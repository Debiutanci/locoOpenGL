#include "object.h"
#include "shaderprogram.h"
#include <string>
// assimp imports
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>


namespace Models {

	Object object;
	Object walec;
	Object belka;
	Object kolo3;
	Object tory;
	Object podwozie;
	Object kratka;

	Object::Object() {
		//init("./model/cube.obj");
	}

	Object::Object(float R, float r, float mainDivs, float tubeDivs) {
		buildObject(R, r, mainDivs, tubeDivs);
	}

	Object::Object(std::string filename) {
		init(filename);
	}

	Object::~Object() {
	}

	inline float Object::d2r(float deg) {
		return PI * deg / 180.0f;
	}

	vec4 Object::generateObjectPoint(float R, float r, float alpha, float beta) {
		alpha = d2r(alpha);
		beta = d2r(beta);
		return vec4((R + r * cos(alpha)) * cos(beta), (R + r * cos(alpha)) * sin(beta), r * sin(alpha), 1.0f);
	}

	void Object::init(std::string filename)
	{
			// ASSIMP CORE
			//std::string filename = "./model/Cube_obj.obj";

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				fprintf(stderr, "Nie mo¿na wczytaæ pliku OBJ.\n");
				exit(EXIT_FAILURE);
			}


			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[i];
				vertices = new float[mesh->mNumVertices*4];
				normals = new float[mesh->mNumVertices * 4];
				texCoords = new float[mesh->mNumVertices * 4];
				vertexCount = mesh->mNumVertices;
				
				for (unsigned int j = 0; j < mesh->mNumVertices; j++)
				{
					aiVector3D pos = mesh->mVertices[j];
					glm::vec4 vertex(pos.x, pos.y, pos.z, 1.0f);

					//vertex do tablicy
					vertices[4*j] = mesh->mVertices[j].x;
					vertices[4*j+1] = mesh->mVertices[j].y;
					vertices[4*j+2] = mesh->mVertices[j].z;
					vertices[4*j+3] = 1.0;

					if (mesh->HasNormals())
					{
						aiVector3D normal = mesh->mNormals[j];
						glm::vec4 normalVec(normal.x, normal.y, normal.z, 0.0f);
						//normalVec do tablicy z normals

						normals[4 * j] = mesh->mNormals[j].x;
						normals[4 * j + 1] = mesh->mNormals[j].y;
						normals[4 * j + 2] = mesh->mNormals[j].z;
						normals[4 * j + 3] = 0.0;
					}

					if (mesh->HasTextureCoords(0))
					{
						aiVector3D texCoord = mesh->mTextureCoords[0][j];
						glm::vec2 texCoordVec(texCoord.x, texCoord.y);
						//texCoordVec do tablicy z texCoords

						texCoords[4 * j] = mesh->mTextureCoords[0][j].x;
						texCoords[4 * j + 1] = mesh->mTextureCoords[0][j].y;
						texCoords[4 * j + 2] = mesh->mTextureCoords[0][j].z;
						texCoords[4 * j + 3] = 1.0;
					}
				}
			}
	}

	vec4 Object::computeVertexNormal(float alpha, float beta) {
		alpha = d2r(alpha);
		beta = d2r(beta);
		return vec4(cos(alpha) * cos(beta), cos(alpha) * sin(beta), sin(alpha), 0.0f);
	}

	vec4 Object::computeFaceNormal(vector<vec4>& face) {
		vec3 a = vec3(face[1] - face[0]);
		vec3 b = vec3(face[2] - face[0]);

		return normalize(vec4(cross(b, a), 0.0f));
	}

	void Object::generateObjectFace(vector<vec4>& vertices, vector<vec4>& vertexNormals, vec4& faceNormal, float R, float r, float alpha, float beta, float step_alpha, float step_beta) {
		vertices.clear();
		vertexNormals.clear();

		vertices.push_back(generateObjectPoint(R, r, alpha, beta));
		vertices.push_back(generateObjectPoint(R, r, alpha + step_alpha, beta));
		vertices.push_back(generateObjectPoint(R, r, alpha + step_alpha, beta + step_beta));
		vertices.push_back(generateObjectPoint(R, r, alpha, beta + step_beta));

		faceNormal = computeFaceNormal(vertices);

		vertexNormals.push_back(computeVertexNormal(alpha, beta));
		vertexNormals.push_back(computeVertexNormal(alpha + step_alpha, beta));
		vertexNormals.push_back(computeVertexNormal(alpha + step_alpha, beta + step_beta));
		vertexNormals.push_back(computeVertexNormal(alpha, beta + step_beta));
	}



	void Object::buildObject(float R, float r, float mainDivs, float tubeDivs) {
		vector<vec4> face;
		vector<vec4> faceVertexNormals;
		vec4 normal;

		internalVertices.clear();
		internalFaceNormals.clear();
		internalVertexNormals.clear();

		float mult_alpha = 360.0f / tubeDivs;
		float mult_beta = 360.0f / mainDivs;

		for (int alpha = 0; alpha < round(tubeDivs); alpha++) {
			for (int beta = 0; beta < round(mainDivs); beta++) {

				generateObjectFace(face, faceVertexNormals, normal, R, r, alpha * mult_alpha, beta * mult_beta, mult_alpha, mult_beta);

				internalVertices.push_back(face[0]);
				internalVertices.push_back(face[1]);
				internalVertices.push_back(face[2]);

				internalVertices.push_back(face[0]);
				internalVertices.push_back(face[2]);
				internalVertices.push_back(face[3]);

				internalVertexNormals.push_back(faceVertexNormals[0]);
				internalVertexNormals.push_back(faceVertexNormals[1]);
				internalVertexNormals.push_back(faceVertexNormals[2]);

				internalVertexNormals.push_back(faceVertexNormals[0]);
				internalVertexNormals.push_back(faceVertexNormals[2]);
				internalVertexNormals.push_back(faceVertexNormals[3]);

				for (int i = 0; i < 6; i++) internalFaceNormals.push_back(normal);


			}
		}

		vertices = (float*)internalVertices.data();
		normals = (float*)internalFaceNormals.data();
		vertexNormals = (float*)internalVertexNormals.data();
		texCoords = vertexNormals;
		vertexCount = internalVertices.size();
	}

	void Object::drawSolid(ShaderProgram *sp, string fn) {
		//init(fn);

		glEnableVertexAttribArray(sp->a("vertex"));
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices);

		glEnableVertexAttribArray(sp->a("normal"));
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glDisableVertexAttribArray(sp->a("vertex"));
		glDisableVertexAttribArray(sp->a("normal"));
	}
}
