#include "Shader.h"

void Shader::ReadSourceFile(string& vertexFile,string& fragmentFile, const char* vertexPath, const char* fragmentPath)
{
	ifstream vShaderFile;
	ifstream fShaderFile;
	//Check for exceptions to throw the function
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		//Open Files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		//Put file contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//Close File handlers
		vShaderFile.close();
		fShaderFile.close();
		//Convert stream to string
		vertexFile = vShaderStream.str();
		fragmentFile = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_READ" << endl;
	}
	//convert string to char*
	const char* vShaderCode = vertexFile.c_str();
	const char* fShaderCode = fragmentFile.c_str();

	CompileShaders(vShaderCode, fShaderCode);
}

void Shader::CompileShaders(const char* vertexCode, const char* fragmentCode)
{
	unsigned int vertex, fragment;
	int success;
	GLint logLength; //Specifies the legnth of the log
	string infoLog; //Where output log is stored


	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &logLength); //Get length of output log
		infoLog.resize(logLength); //Resize string to its desired length
		glGetShaderInfoLog(vertex, logLength, &logLength, &infoLog[0]);
		cout << "ERROR::SHADER:VERTEX:COMPILATION::FAILED\n" << infoLog << endl;
		//Empty and reset string to empty ready for next call
		infoLog = infoLog.empty(); 
		infoLog.resize(0);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &logLength); //Get length of output log
		infoLog.resize(logLength);
		glGetShaderInfoLog(fragment, logLength, &logLength, &infoLog[0]);
		cout << "ERROR::SHADER:FRAGMENT:COMPILATION::FAILED\n" << infoLog << endl;
		infoLog = infoLog.empty();
		infoLog.resize(0);
	}

	//Attach shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//Check for linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength); //Get length of output log
		infoLog.resize(logLength);
		glGetProgramInfoLog(ID, logLength, &logLength, &infoLog[0]);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		infoLog = infoLog.empty();
		infoLog.resize(0);
	}

	//Delete shaders upon successful linking
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	string vertexCode;
	string fragmentCode;
	ReadSourceFile(vertexCode, fragmentCode, vertexPath, fragmentPath);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const string& name, mat4& matrix) const
{
	//pas through as transposed 4x4 matrix
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3(const string& name, vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
