//
// Created by Vicente González on 22-06-23.
//

#ifndef NBODYCUDA_BUFFER_H_
#define NBODYCUDA_BUFFER_H_

#include <glm/vec3.hpp>

#include <iostream>
#include <vector>

#include "shader.h"


struct Point
{
	glm::vec3 position;
	glm::vec3 velocity;
	float mass;
};


class Buffer
{
 private:
	unsigned int _ebo, _vbo, _vao;
	float _ratio;
	float _s_mass;
	unsigned int _size;

	struct cudaGraphicsResource* _cuda_resource;
	float* _cuda_points;

 public:
	std::vector<Point> data;

 public:
	friend std::ostream& operator<<(std::ostream& os, const Buffer& buffer);

	Buffer();
	~Buffer();

	void bind() const;
	void unbind() const;

	void map();
	void unmap();

	inline void setSize(unsigned int size) { _size = size; }
	inline void setSpecialMass(float mass) { _s_mass = mass; }
	inline void setRatio(float ratio) { _ratio = ratio; }

	void build();

	void setLayout(const Shader& shader) const;

	float* getCudaPoints() const;
};


#endif //NBODYCUDA_BUFFER_H_
