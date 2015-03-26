#pragma once
#include "Base.h"

namespace DepthUI{

class DEPTHUI_API Shader
{
public:
	bool is_ready_;

	std::string name_;

	enum ShaderStageType{
		SHADER_VERTEX = 0,
		SHADER_GEOMETRY,
		SHADER_FRAGMENT,
		NUM_SHADER_STAGES = SHADER_FRAGMENT + 1
	};

	GLuint shaderTarget[NUM_SHADER_STAGES];

	GLuint guid_program_;
	GLuint guid_shader_[NUM_SHADER_STAGES];

	std::string filename_shader_[NUM_SHADER_STAGES];

	char * source_shader_[NUM_SHADER_STAGES];

	std::string compileLog_;

	Shader(std::string name)
	{
		name_ = name;

		is_ready_ = false;

		guid_program_ = 0;

		guid_shader_[SHADER_VERTEX] = 0;
		guid_shader_[SHADER_GEOMETRY] = 0;
		guid_shader_[SHADER_FRAGMENT] = 0;

		compileLog_ = "";

		filename_shader_[SHADER_VERTEX] = "";
		filename_shader_[SHADER_GEOMETRY] = "";
		filename_shader_[SHADER_FRAGMENT] = "";

		source_shader_[SHADER_VERTEX] = NULL;
		source_shader_[SHADER_GEOMETRY] = NULL;
		source_shader_[SHADER_FRAGMENT] = NULL;

		shaderTarget[SHADER_VERTEX] = GL_VERTEX_SHADER;
		shaderTarget[SHADER_GEOMETRY] = GL_GEOMETRY_SHADER;
		shaderTarget[SHADER_FRAGMENT] = GL_FRAGMENT_SHADER;
	}

	~Shader()
	{
	}

	char* File2CharArray(const char *path)
	{
		FILE *fd;
		long len,
			r;
		char *str;

		if (!fopen_s(&fd, path, "r"))
		{
			fprintf(stderr, "Can't open file '%s' for reading\n", path);
			fprintf(stdout, "Can't open file '%s' for reading\n", path);
			return NULL;
		}

		fseek(fd, 0, SEEK_END);
		len = ftell(fd);

		fseek(fd, 0, SEEK_SET);

		if (!(str = (char*)malloc(len * sizeof(char))))
		{
			fprintf(stderr, "Can't malloc space for '%s'\n", path);
			fprintf(stdout, "Can't malloc space for '%s'\n", path);
			return NULL;
		}

		r = fread(str, sizeof(char), len, fd);

		str[r - 1] = '\0'; /* Shader sources have to term with null */

		fclose(fd);

		return str;
	}

	void SetShaderFiles(int shaderStage, std::string filename)
	{
		is_ready_ = false;
		filename_shader_[shaderStage] = filename;
		source_shader_[shaderStage] = File2CharArray(filename.c_str());
	}

	void SetShaderSource(int shaderStage, const char * source)
	{
		is_ready_ = false;
		source_shader_[shaderStage] = _strdup(source);
	}

	void SetShaderSource(int shaderStage, std::string source)
	{
		is_ready_ = false;
		source_shader_[shaderStage] = _strdup(source.c_str());
	}

	bool AttachShader(int shaderStage)
	{
		int success = true;
		guid_shader_[shaderStage] = DEPTHGL(glCreateShader(shaderTarget[shaderStage]));
		if (guid_shader_[shaderStage] == 0)
		{
			printf("%s shader creation error at stage %d\n", shaderStage);
			success = false;
		}
		else
		{
			DEPTHGL(glShaderSource(guid_shader_[shaderStage], 1, (const GLchar**)&source_shader_[shaderStage], 0));
			DEPTHGL(glCompileShader(guid_shader_[shaderStage]));

			GLint isCompiled = 0;
			DEPTHGL(glGetShaderiv(guid_shader_[shaderStage], GL_COMPILE_STATUS, &isCompiled));
			if (isCompiled == GL_FALSE)
			{
				// compile log
				int maxLength;
				DEPTHGL(glGetShaderiv(guid_shader_[shaderStage], GL_INFO_LOG_LENGTH, &maxLength));
				char* compilerErrors = new char[maxLength];
				DEPTHGL(glGetShaderInfoLog(guid_shader_[shaderStage], maxLength, &maxLength, compilerErrors));
				compileLog_.assign(compilerErrors);
				printf("%s %d Log:\n %s\n", name_.c_str(), shaderStage, compileLog_.c_str());
			}
			else
			{
				DEPTHGL(glAttachShader(guid_program_, guid_shader_[shaderStage]));
			}
		}
		return success;
	}

	bool LoadShaders()
	{
		bool success = true;
		guid_program_ = DEPTHGL(glCreateProgram());
		if (guid_program_ == 0)
		{
			printf("shader program creation error for %s\n", name_);
			success = false;
		}
		else
		{
			for (int i = 0; i < NUM_SHADER_STAGES; i++)
			{
				if (source_shader_[i] != NULL) success = AttachShader(i);
			}
		}

		if (success)
		{
			DEPTHGL(glLinkProgram(guid_program_));
			GLint linked;
			DEPTHGL(glGetProgramiv(guid_program_, GL_LINK_STATUS, &linked));
			if (!linked)
			{
				printf("Program not linked.\n");
				printProgramInfoLog(guid_program_);
			}
		}
		if (success) is_ready_ = true;
		return success;
	}

	void printShaderInfoLog(GLuint shader)
	{
		int infoLogLen = 0;
		int charsWritten = 0;
		GLchar *infoLog;

		DEPTHGL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen));

		if (infoLogLen > 0)
		{
			infoLog = new GLchar[infoLogLen];

			DEPTHGL(glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog));
			printf("InfoLog:\n%s\n", infoLog);
			delete[] infoLog;
		}
	}

	// printProgramInfoLog
	void printProgramInfoLog(GLint Program)
	{
		int infoLogLen = 0;
		int charsWritten = 0;
		GLchar *infoLog;

		DEPTHGL(glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &infoLogLen));

		if (infoLogLen > 0)
		{
			infoLog = new GLchar[infoLogLen];

			DEPTHGL(glGetProgramInfoLog(Program, infoLogLen, &charsWritten, infoLog));
			printf("InfoLog:\n%s\n", infoLog);
			delete[] infoLog;
		}
	}

	virtual void Enable()
	{
		if (!is_ready_)
		{
			LoadShaders();
		}
		if (is_ready_)
		{
			bool isProgram = DEPTHGL(glIsProgram(guid_program_));
			if (!isProgram)
			{
				printf("%s %d is not a gl program. reloading...\n", name_, guid_program_);
			}
			DEPTHGL(glUseProgram(guid_program_));
		}
	}

	virtual void Disable()
	{
		glUseProgram(0);
	}

	GLint GetAttributeLocation(const std::string & name)
	{
		return glGetAttribLocation(guid_program_, name.c_str());
	}

	GLint GetUniformLocation(const std::string & name)
	{
		return glGetUniformLocation(guid_program_, name.c_str());
	}
};

class DEPTHUI_API DefaultShader:
	public Shader
{
public:
	DefaultShader() :Shader("Default")
	{
		// vertex shader
		SetShaderSource(SHADER_VERTEX,
			"#version 420 compatibility					\n"
			"											\n"
			"struct Matrices{							\n"
			"	mat4 Projection;						\n"
			"	mat4 ModelView;							\n"
			"};											\n"
			"uniform Matrices matrices;					\n"
			"											\n"
			"// in										\n"
			"in vec3 VPosition;							\n"
			"//in vec3 VNormal;							\n"
			"in vec4 VColor;							\n"
			"											\n"
			"// out										\n"
			"out vec4 FColor;							\n"
			"											\n"
			"void main()								\n"
			"{											\n"
			"	gl_Position = matrices.Projection *		\n"
			"	matrices.ModelView						\n"
			"	* vec4(VPosition, 1);					\n"
			"	FColor = VColor;						\n"
			"}											\n"
		);


		// fragment shader
		SetShaderSource(SHADER_FRAGMENT,
			"#version 420 compatibility				\n"
			"										\n"
			"uniform uint ID;						\n"
			"										\n"
			"in vec4 FColor;						\n"
			"										\n"
			"out vec4 FragColor;					\n"
			"										\n"
			"void main(void)						\n"
			"{										\n"
			"	FragColor = FColor;					\n"
			"}										\n"
		);
	}

	~DefaultShader()
	{
	}

	void SetTransformationMatrices(const Mat4 & mat_projection, const Mat4 & mat_modelview)
	{
		int loc = GetUniformLocation("matrices.Projection");
		if (loc != -1) DEPTHGL(glProgramUniformMatrix4fv(guid_program_, loc, 1, GL_FALSE, mat_projection.data()));
		loc = GetUniformLocation("matrices.ModelView");
		if (loc != -1) DEPTHGL(glProgramUniformMatrix4fv(guid_program_, loc, 1, GL_FALSE, mat_modelview.data()));
	}
};



class DEPTHUI_API Quad2DTextureShader :
	public Shader
{
private:
	uint texSlot_;
	uint tex_quad2D_;
	uint vao_quad_;
	uint vbo_quad_position_;
	uint vbo_quad_element_;
	uint viewport_width_;
	uint viewport_height_;

	bool is_initialized_;
public:
	Quad2DTextureShader() :Shader("Quad")
	{
		is_initialized_ = false;

		vao_quad_ = 0;
		vbo_quad_position_ = 0;
		vbo_quad_element_ = 0;

		// vertex shader
		SetShaderSource(SHADER_VERTEX,
			"#version 420 compatibility					\n"
			"											\n"
			"// in										\n"
			"in vec3 VPosition;							\n"
			"											\n"
			"// out										\n"
			"out vec2 TexCoord;							\n"
			"											\n"
			"void main()								\n"
			"{											\n"
			"	gl_Position = vec4(VPosition, 1);		\n"
			"	TexCoord = VPosition.xy * 0.5			\n"
			"	+ vec2(0.5,0.5);						\n"
			"}											\n"
			);


		// fragment shader
		SetShaderSource(SHADER_FRAGMENT,
			"#version 420 compatibility				\n"
			"										\n"
			"struct Texture2D{						\n"
			"	sampler2D Sampler;					\n"
			"	ivec2 Dims;							\n"
			"};										\n"
			"uniform Texture2D tex2D_Quad;			\n"
			"uniform ivec2 ViewportDims;			\n"
			"										\n"
			"in vec2 TexCoord;						\n"
			"										\n"
			"out vec4 FragColor;					\n"
			"										\n"
			"void main(void)						\n"
			"{										\n"
			"	FragColor = texture(				\n"
			"	tex2D_Quad.Sampler, TexCoord);		\n"
			"}										\n"
			);
	}

	~Quad2DTextureShader()
	{
	}

	void SetParameters(const uint & tex_quad2D, uint viewport_width, uint viewport_height)
	{
		texSlot_ = GL_TEXTURE0;
		tex_quad2D_ = tex_quad2D;

		viewport_width_ = viewport_width;
		viewport_height_ = viewport_height;
	}

	virtual void DefaultRender()
	{
		Enable();

		if (!is_initialized_)
		{
			DEPTHGL(glGenVertexArrays(1, &vao_quad_));
			DEPTHGL(glBindVertexArray(vao_quad_));

			float data_pos_quad[4 * 3] = { -1.0, -1.0, 0,
				1.0, -1.0, 0,
				1.0, 1.0, 0,
				-1.0, 1.0, 0 };
			glGenBuffers(1, &vbo_quad_position_);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_quad_position_);
			glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), data_pos_quad, GL_STATIC_DRAW);

			const int loc_vertices_quad = GetAttributeLocation("VPosition");
			DEPTHGL(glEnableVertexAttribArray(loc_vertices_quad));
			DEPTHGL(glVertexAttribPointer(loc_vertices_quad, 3, GL_FLOAT, GL_FALSE, 0, 0));

			uint data_quad[1 * 4] = { 0, 1, 2, 3 };
			glGenBuffers(1, &vbo_quad_element_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_quad_element_);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1 * 4 * sizeof(uint), data_quad, GL_STATIC_DRAW);

			DEPTHGL(glBindVertexArray(0));

			DEPTHGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
			DEPTHGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			is_initialized_ = true;
		}

		if (is_initialized_)
		{
			GLboolean depth_test_old;
			glGetBooleanv(GL_DEPTH_TEST, &depth_test_old);
			glDisable(GL_DEPTH_TEST);

			GLboolean depth_write_old;
			glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_write_old);
			glDepthMask(GL_FALSE);

			GLboolean blend_old;
			glGetBooleanv(GL_BLEND, &blend_old);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_TEXTURE_2D);

			glActiveTexture(texSlot_);
			glBindTexture(GL_TEXTURE_2D, tex_quad2D_);

			int loc = GetUniformLocation("tex2D_Quad.Sampler");
			if (loc != -1) DEPTHGL(glProgramUniform1i(guid_program_, loc, (texSlot_ - GL_TEXTURE0)));	// texture slot
			loc = GetUniformLocation("ViewportDims");
			if (loc != -1) DEPTHGL(glProgramUniform2i(guid_program_, loc, viewport_width_, viewport_height_));	// texture size

			DEPTHGL(glBindVertexArray(vao_quad_));
			DEPTHGL(glDrawElements(GL_QUADS, 4 * 1, GL_UNSIGNED_INT, (void*)0));
			DEPTHGL(glBindVertexArray(0));

			glDisable(GL_TEXTURE_2D);

			if (depth_test_old) glEnable(GL_DEPTH_TEST);
			if (!blend_old) glDisable(GL_BLEND);
			glDepthMask(depth_write_old);
		}

		Disable();
	}
};

}