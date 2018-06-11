#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <vector>
#include <glm\common.hpp>

typedef struct Material {
	GLfloat Ns, Ni, d;
	glm::vec3 Ka,Kd,Ks ;
	GLint illum;
	char pathToTga[128];
} Material;

bool loadOBJ(
	const char * path,
	std::vector < glm::vec3 > & out_vertices,
	std::vector < glm::vec2 > & out_uvs,
	std::vector < glm::vec3 > & out_normals
);

bool loadMtl(Material &material);