#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "schism/System/FileIO.h"

namespace Schism::Gl
{
	Shader::Shader()
	{
	}
	
	Shader::Shader(const std::string& vert, const std::string& frag)
	{
		Compile(vert, frag);
	}

	Ref<Shader> Shader::Create()
	{
		return MakeRef<Shader>();
	}

	Ref<Shader> Shader::Create(const std::string& vert, const std::string& frag)
	{
		auto vertCode = System::ReadFile(vert);
		auto fragCode = System::ReadFile(frag);

		return MakeRef<Shader>(vertCode, fragCode);
	}

	Shader::~Shader()
	{
		Delete();
	}

	void Shader::Compile(const std::string& vert, const std::string& frag)
	{
		m_VertexShader = __CompileShader(GL_VERTEX_SHADER, vert.c_str());
		m_FragmentShader = __CompileShader(GL_FRAGMENT_SHADER, frag.c_str());

		m_Program = glCreateProgram();
		glAttachShader(m_Program, m_VertexShader);
		glAttachShader(m_Program, m_FragmentShader);

		glLinkProgram(m_Program);

		GLint isLinked = 0;
		glGetProgramiv(m_Program, GL_LINK_STATUS, &isLinked);
		
		if (!isLinked)
		{
			GLint maxLen = 0;

			glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<GLchar> infoLog(maxLen);
			glGetProgramInfoLog(m_Program, maxLen, &maxLen, &infoLog[0]);
			
			glDeleteProgram(m_Program);
			glDeleteShader(m_VertexShader);
			glDeleteShader(m_FragmentShader);

			SC_CORE_ERROR("{0}", infoLog.data());
			SC_CORE_TRACE("Error linking program");
		}
		
		glDetachShader(m_Program, m_VertexShader);
		glDetachShader(m_Program, m_FragmentShader);
		glDeleteShader(m_VertexShader);
		glDeleteShader(m_FragmentShader);
	}


	void Shader::SetInt(const std::string& name, int v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name.c_str());
		glUniform1i(loc, v);
	}
	
	void Shader::SetFloat(const std::string& name, float v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name.c_str());
		glUniform1f(loc, v);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name.c_str());
		glUniform2f(loc, v.x, v.y);
	}
	
	void Shader::SetFloat3(const std::string& name, const glm::vec3& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name.c_str());
		glUniform3f(loc, v.x, v.y, v.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name.c_str());
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name.c_str());
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(v));
	}

	
	void Shader::SetMat4(const std::string& name, const glm::mat4& v) const
	{
		GLint loc = glGetUniformLocation(m_Program, name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(v));
	}

	void Shader::Bind()
	{
		glUseProgram(m_Program);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}
	
	void Shader::Delete()
	{
		glDeleteProgram(m_Program);
	}
}
