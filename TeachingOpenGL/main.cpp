#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ���������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

bool is_filling_polugon = true;
bool is_mode_changed = false;

int main()
{
	// glfw: ������������� � ����������������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // ���������������� ��� ������, ���� ����������� macOS
#endif

	// glfw: �������� ����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rjkz application", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: �������� ���� ���������� �� OpenGL-�������
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// �������������� ����� ��������� ���������

	// ��������� ������
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// �������� �� ������� ������ �������������� ���������� �������
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ����������� ������
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// �������� �� ������� ������ �������������� ������������ �������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ���������� ��������
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// �������� �� ������� ������ �������������� ���������� ��������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// ���������� ������ (� �������) � ��������� ��������� ���������
	float vertices[] = {
	0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // ������� ������                   0
	0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // ������ ������                    1
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,  // ������ �����                    2
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // ������� �����                   3
	0.0f, 0.85f, 0.0f, 0.0f, 0.0f, 1.0f,   // ������� ����������              4
	0.2f, 0.2f, 0.0f, 0.2f, 0.2f, 1.0f,    // ������ ������� ����� ����       5
	0.2f, -0.2f, 0.0f, 0.2f, 0.2f, 1.0f,   // ������ ������ ����� ����        6
	-0.2f, -0.2f, 0.0f, 0.2f, 0.2f, 1.0f,  // ����� ������ ����� ����         7 
	-0.2f, 0.2f, 0.0f, 0.2f, 0.2f, 1.0f,   // ����� ������� ����� ����        8
	};

	// ��� ���������� �������
	unsigned int homeIndices[] = {  // ���
		0, 1, 3, // ������ �����������
		1, 2, 3  // ������ �����������
	};

	unsigned int roofIndices[] = { // �����
		0, 3, 4  // �����
	};

	unsigned int windowIndices[] = { // ����
		5, 6, 7,  // ������� ����
		5, 7, 8
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// ������� ��������� ������ ���������� �������, ����� ��������� � ������������� ��������� �����(�), � ����� ������������� ��������� �������(�)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// �������� ��������, ��� ������ �������� ���������, ����� glVertexAttribPointer() ��������������� VBO ��� ����������� ��������� �������� ������ ��� ���������� ��������, ��� ��� ����� ����� �� ����� �������� ��������� �������
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// �� ������ �������� �������� V�� ����� �����, ����� ������ ������ V�� �������� �� �������� ���� VAO (�� �������� �������� ����� ���������).
	// ����������� ������ VAO ������� ����� glBindVertexArray(), ������� �� ������ �� ������� �������� VAO (��� VBO), ����� ��� �� ��������� ��������
	glBindVertexArray(0);

	unsigned int homeEBO;
	glGenBuffers(1, &homeEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, homeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(homeIndices), homeIndices, GL_STATIC_DRAW);

	unsigned int roofEBO;
	glGenBuffers(1, &roofEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roofIndices), roofIndices, GL_STATIC_DRAW);

	unsigned int windowEBO;
	glGenBuffers(1, &windowEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(windowIndices), windowIndices, GL_STATIC_DRAW);

	// ���������������� ��������� ������ ��� ��������� ��������� � ������ �������
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ���� ����������
	while (!glfwWindowShouldClose(window))
	{
		// ��������� �����
		processInput(window);

		// ���������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ����� ������ ���������
		if (is_filling_polugon) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		// ������ ��� ������ �����������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // ��������� � ��� ���� ������ ���� V��, �� ��� ������������� ��������� ��� ������ ��� (�� �� ������� ���, ����� �� ���� ������� ��������������)
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//lUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 1.0f, 0.5f, 0.2f, 1.0f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, homeEBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 1.0f, 0.2f, 0.2f, 1.0f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofEBO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 0.2f, 0.2f, 1.0f, 1.0f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowEBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// glBindVertexArray(0); // �� ����� ������ ��� ��� ����������

		// glfw: ����� ���������� front- � back- �������. ������������ ������� �����\������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
	glfwTerminate();
	return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !is_mode_changed) {
		is_filling_polugon = !is_filling_polugon;
		is_mode_changed = true;
	}
	else if (!glfwGetKey(window, GLFW_KEY_SPACE)) {
		is_mode_changed = false;
	}
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// ����������, ��� ���� ��������� ������������� ����� �������� ����.
	// �������� ��������, ��� ������ ����� ����������� ������, ��� �������, �� Retina-��������
	glViewport(0, 0, width, height);
}