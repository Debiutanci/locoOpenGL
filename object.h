#pragma once

#include "model.h"
#include "shaderprogram.h"
#include <string>


namespace Models {

	using namespace std;
	using namespace glm;

	class Object : public Model {

	public:
		Object();
		Object(float R, float r, float mainDivs, float tubeDivs);
		Object(std::string filename);
		virtual ~Object();
		virtual void drawSolid(ShaderProgram* sp, string f);
		void drawWithTex(ShaderProgram* sp, GLuint texture, glm::mat4 P, glm::mat4 V, glm::mat4 M);

		//glm::mat4 M;
  //      glm::mat4 V;
  //      glm::mat4 P;
		GLuint texture_map;

	private:
		vector<vec4> internalVertices;
		vector<vec4> internalFaceNormals;
		vector<vec4> internalVertexNormals;

		void init(std::string filename);

		inline float d2r(float deg);
		vec4 generateObjectPoint(float R, float r, float alpha, float beta);
		vec4 computeVertexNormal(float alpha, float beta);
		vec4 computeFaceNormal(vector<vec4>& face);
		void generateObjectFace(vector<vec4>& vertices, vector<vec4>& vertexNormals, vec4& faceNormal, float R, float r, float alpha, float beta, float step_alpha, float step_beta);
		void buildObject(float R, float r, float mainDivs, float tubeDivs);
		
	};

	extern Object object;
	extern Object walec;
	extern Object belka;
	extern Object kolo3;
	extern Object tory;
	extern Object podwozie;
	extern Object kratka;
	extern Object kabina;
	extern Object dach;

}

