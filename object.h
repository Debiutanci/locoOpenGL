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
	extern Object komin1;
	extern Object komin2;


}

