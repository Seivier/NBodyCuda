//
// Created by Vicente González on 18-06-23.
//

#ifndef NBODYCUDA_SHADER_H_
#define NBODYCUDA_SHADER_H_

#include <string>
#include <unordered_map>

class Shader
{
 private:
	unsigned int _id;
	mutable std::unordered_map<std::string, int> _uniformLocationCache;

 public:
	Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1f(const std::string& name, float value) const;
	void setUniform1i(const std::string& name, int value) const;
	int getAttribLocation(const std::string& name) const;

 private:
	int getUniformLocation(const std::string& name) const;
};

#endif // NBODYCUDA_SHADER_H_
