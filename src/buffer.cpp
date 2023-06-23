//
// Created by Vicente González on 22-06-23.
//
#include <glad/glad.h>

#include <random>

#include "buffer.h"


static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float> dis(-1.0, 1.0);

static const float MASS = 1.0f;

using namespace glm;

Buffer::Buffer()
	: _ebo(0), _vbo(0), _vao(0), data(), _ratio(0.0f), _s_mass(MASS), _size(0)
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
}

Buffer::~Buffer()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void Buffer::bind() const
{
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
}

void Buffer::unbind() const
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Buffer::build()
{
	unsigned int old_size = static_cast<unsigned int>(data.size());

	std::vector<Point> newdata(_size);

	auto n_sp = static_cast<unsigned int>(_size * _ratio);
	unsigned int j = 0;

	for (unsigned int i = 0; i < _size; ++i)
	{
		vec3 position(dis(gen), dis(gen), 0.0f);
		vec3 velocity = {0.0f, 0.0f, 0.0f};
		float mass = MASS;


		if (i < old_size)
		{
			position = data[i].position;
			velocity = data[i].velocity;
		}


		if (j < n_sp)
		{
			mass = _s_mass;
			++j;
		}

		newdata[i] = {position, velocity, mass};
	}

	data = newdata;

//	std::vector<float> vertices;
//	std::vector<unsigned int> indices;
//	vertices.reserve(_size * 6);
//	for (const auto& point : data)
//	{
//		vertices.push_back(point.position.x);
//		vertices.push_back(point.position.y);
//		vertices.push_back(point.position.z);
//		vertices.push_back(point.acceleration.x);
//		vertices.push_back(point.acceleration.y);
//		vertices.push_back(point.acceleration.z);
//		indices.push_back(static_cast<unsigned int>(indices.size()));
//	}

	bind();
	glBufferData(GL_ARRAY_BUFFER, _size * sizeof(Point), data.data(), GL_STATIC_DRAW);
	std::vector<unsigned int> indices(_size);
	for (unsigned int i = 0; i < _size; ++i)
		indices[i] = i;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	unbind();
}

void Buffer::setLayout(const Shader& shader) const
{
	shader.bind();
	bind();

	auto pos = shader.getAttribLocation("position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(Point), nullptr);

//	auto vel = shader.getAttribLocation("velocity");
//	glEnableVertexAttribArray(vel);
//	glVertexAttribPointer(vel, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)(3 * sizeof(float)));
//
//	auto acc = shader.getAttribLocation("acceleration");
//	glEnableVertexAttribArray(acc);
//	glVertexAttribPointer(acc, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)(6 * sizeof(float)));

	auto mass = shader.getAttribLocation("mass");
	glEnableVertexAttribArray(mass);
	glVertexAttribPointer(mass, 1, GL_FLOAT, GL_FALSE, sizeof(Point), (const void*)(6 * sizeof(float)));

	unbind();
	shader.unbind();
}
std::ostream& operator<<(std::ostream& os, const Buffer& buffer)
{
	os << "Buffer: " << std::endl;
	os << "Size: " << buffer._size << std::endl;
	os << "Ratio: " << buffer._ratio << std::endl;
	os << "Data: " << std::endl;
	for (const auto& point : buffer.data)
	{
		os << point.position.x << ", " << point.position.y << ", " << point.position.z << std::endl;
		os << std::endl;
	}
	return os;
}

