//
// Created by Vicente González on 22-06-23.
//

#ifndef NBODYCUDA_BUFFER_H_
#define NBODYCUDA_BUFFER_H_

#include <iostream>
#include <vector>

#include "shader.h"

struct Vec3
{
	float x, y, z;
};

struct Point
{
	Vec3 position;
	Vec3 acceleration;
};


class Buffer
{
 private:
	unsigned int _ebo, _vbo, _vao;
	std::vector<Point> _data;
	float _ratio;
	Vec3 _acceleration;
	unsigned int _size;

 public:
	friend std::ostream& operator<<(std::ostream& os, const Buffer& buffer);

	Buffer();
	~Buffer();

	void bind() const;
	void unbind() const;

	inline void setSize(unsigned int size) { _size = size; }
	inline void setAcceleration(float x, float y, float z) { _acceleration = {x, y, z}; }
	inline void setRatio(float ratio) { _ratio = ratio; }
	inline unsigned int size() const { return _size; }

	void build();

	void setLayout(const Shader& shader) const;
};


#endif //NBODYCUDA_BUFFER_H_
