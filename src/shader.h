//
// Created by Vicente González on 18-06-23.
//

#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>

#include <string>


class Shader
{
 private:
	unsigned int _id;

 public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
	~Shader();

	void bind() const;
	void unbind() const;

 private:
	static unsigned int loadSources(unsigned int type, const std::string& path);
	void linkSources(unsigned int vertex, unsigned int fragment) const;
	void linkSources(unsigned int vertex, unsigned int geometry, unsigned int fragment) const;

};

#endif //_SHADER_H_
