#define _CRT_SECURE_NO_WARNINGS

using namespace std;
#include <stdio.h>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>

#include "ObjLoader.hpp"

#include "include/stb_image.h"

char pathToMtl[128];

bool loadOBJ(
	const char * path,
	std::vector < glm::vec3 > & out_vertices,
	std::vector < glm::vec2 > & out_uvs,
	std::vector < glm::vec3 > & out_normals
)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader


		else if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9) {
				printf("Parser can't load");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else if (strcmp(lineHeader, "mtllib") == 0)
		{
			char pathTemporario[128];
			fscanf(file, "%s", pathTemporario);
			strcpy(pathToMtl, "Model/");
			strcat(pathToMtl, pathTemporario);
		}
		///MTL LOAD
		/*else if (strcmp(lineHeader, "mtllib") == 0)
		{
		fscanf(file, "%s\n", &mtlFile);
		loadMtlFile();
		}
		*/


		// For each vertex of each triangle
		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			unsigned int vertexIndex = vertexIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			out_vertices.push_back(vertex);
		}

		// For each uv of each triangle
		for (unsigned int i = 0; i < uvIndices.size(); i++) {
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			out_uvs.push_back(uv);
		}

		// For each normal of each triangle
		for (unsigned int i = 0; i < normalIndices.size(); i++) {
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = temp_normals[normalIndex - 1];
			out_normals.push_back(normal);
		}
	}
}

bool loadMtl(Material& material)
{
	FILE * ficheiro = fopen(pathToMtl, "r");
	if (ficheiro == NULL) {
		printf("Can't Open File\n");
		return false;
	}

	while (1) {
		char lineHeader[128];
		int res = fscanf(ficheiro, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "Ns") == 0) {
			GLfloat Ns_temp = 0.0f;
			fscanf(ficheiro, "%f\n", Ns_temp);
			material.Ns = Ns_temp;
		}
		else if (strcmp(lineHeader, "Ni") == 0) {
			GLfloat Ni_temp = 0.0f;;
			fscanf(ficheiro, "%f\n", Ni_temp);
			material.Ni = Ni_temp;
		}
		else if (strcmp(lineHeader, "d") == 0) {
			GLfloat d_temp = 0.0f;;
			fscanf(ficheiro, "%f\n", d_temp);
			material.d = d_temp;
		}

		else if (strcmp(lineHeader, "Ka") == 0) {
			glm::vec3 Ka_temp = glm::vec3(0.0f, 0.0f, 0.0f);
			fscanf(ficheiro, "%f %f %f\n", Ka_temp.x, Ka_temp.y, Ka_temp.z);
			material.Ka = Ka_temp;
		}
		else if (strcmp(lineHeader, "Kd") == 0) {
			glm::vec3 Kd_temp = glm::vec3(0.0f, 0.0f, 0.0f);
			fscanf(ficheiro, "%f %f %f\n", Kd_temp.x, Kd_temp.y, Kd_temp.z);
			material.Kd = Kd_temp;
		}
		else if (strcmp(lineHeader, "Ks") == 0) {
			glm::vec3 Ks_temp = glm::vec3(0.0f, 0.0f, 0.0f);
			fscanf(ficheiro, "%f %f %f\n", Ks_temp.x, Ks_temp.y, Ks_temp.z);
			material.Ks = Ks_temp;
		}
		
		
		else if (strcmp(lineHeader, "illum") == 0) {
			GLint illum = 0;
			fscanf(ficheiro, "%f\n", illum);
			material.illum = illum;
		}
		else if (strcmp(lineHeader, "map_Kd") == 0) {
			char map_Kd_temp[128];
			fscanf(ficheiro, "%s\n", map_Kd_temp);
			strcpy(material.pathToTga, "Model/");
			strcat(material.pathToTga, map_Kd_temp);
		}
	}
}
//void loadMtlFile() {
//
//	mtlFile = "Model/" + mtlFile;
//	const char * c = mtlFile.c_str();
//	FILE * file = fopen(c, "r");
//	
//
//	while (1) {
//
//		char lineHeader[128];
//		// read the first word of the line
//		int res = fscanf(file, "%s", lineHeader);
//		if (res == EOF)
//			break; // EOF = End Of File. Quit the loop.
//
//				   // else : parse lineHeader
//
//		
//		if (strcmp(lineHeader, "map_Kd") == 0)
//		{
//			fscanf(file, "%s\n", &textureFile);
//			loadTexture();
//		}
//
//		
//	}
//}


//void loadTexture() {
//
//	textureFile = "Model/" + textureFile;
//	glGenTextures(1, &textureID);
//
//	// Bind the newly created texture
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	glUniform1i(textureID, 0);
//
//	// Nice trilinear filtering.
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	// Activates image inversion
//	stbi_set_flip_vertically_on_load(true);
//
//	int width, height, nChannels;
//	unsigned char *imageData = stbi_load(textureFile.c_str(), &width, &height, &nChannels, 0);
//
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);
//
//
//	// Frees the image from memory
//	stbi_image_free(imageData);
//
//}