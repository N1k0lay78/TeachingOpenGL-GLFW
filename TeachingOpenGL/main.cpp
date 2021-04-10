#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "stb_image.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// inputs
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



// ���������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool is_filling_polugon = true;
bool is_mode_changed = false;

// camera
float yaw = -90.0f;
float pitch = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view;


// mouse
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;
float fov = 60;

// delatime
float deltaTime = 0.0f;	// ����� ����� ������� � ��������� �������
float lastFrame = 0.0f; // ����� ���������� �����

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
	glEnable(GL_DEPTH_TEST);

	// �������������� ����� ��������� ���������
	Shader myShader("ver_1.vs", "frag_1.fs");

	unsigned int textureHome;
	glGenTextures(1, &textureHome);
	glBindTexture(GL_TEXTURE_2D, textureHome);

	// ������������� ��������� ��������� � ���������� ������� (��� �������� ���������� ������� ��������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		// �������� � ��������� ��������
		int width, height, nrChannels;
		unsigned char *data = stbi_load("../Source/texture/brick.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	unsigned int textureWindow;
	glGenTextures(1, &textureWindow);
	glBindTexture(GL_TEXTURE_2D, textureWindow);

	// ������������� ��������� ��������� � ���������� ������� (��� �������� ���������� ������� ��������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		int width, height, nrChannels;
		// �������� � ��������� ��������
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load("../Source/texture/window.jpeg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	unsigned int textureRoof;
	glGenTextures(1, &textureRoof);
	glBindTexture(GL_TEXTURE_2D, textureRoof);

	// ������������� ��������� ��������� � ���������� ������� (��� �������� ���������� ������� ��������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		int width, height, nrChannels;
		// �������� � ��������� ��������
		// stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load("../Source/texture/wood.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	unsigned int textureRoofRoof;
	glGenTextures(1, &textureRoofRoof);
	glBindTexture(GL_TEXTURE_2D, textureRoofRoof);

	// ������������� ��������� ��������� � ���������� ������� (��� �������� ���������� ������� ��������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		int width, height, nrChannels;
		// �������� � ��������� ��������
		// stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load("../Source/texture/roof.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	};

	// ���������� ������ (� �������) � ��������� ��������� ���������
	float vertices[] = {
	0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  // ������� ������                   0-
	0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  // ������ ������                    1
	-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // ������ �����                    2
	-0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  // ������� �����                   3
	0.0f, 0.85f, 1.0f, 0.0f, 0.5f,   // ������� ����������              4-
	0.35f, 0.35f, 1.001f, 1.0f, 1.0f,    // ������ ������� ����� ���    5
	0.35f, -0.35f, 1.001f, 1.0f, 0.0f,   // ������ ������ ����� ����    6
	-0.35f, -0.35f, 1.001f, 0.0f, 0.0f,  // ����� ������ ����� ����     7 
	-0.35f, 0.35f, 1.001f, 0.0f, 1.0f,   // ����� ������� ����� ����    8
	0.5f,  0.5f, -1.0f, 0.0f, 1.0f,  // ������� ������                  9-
	0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // ������ ������                   10
	-0.5f, -0.5f, -1.0f, 1.0f, 0.0f,  // ������ �����                   11
	-0.5f,  0.5f, -1.0f, 1.0f, 1.0f,  // ������� �����                  12
	0.0f, 0.85f, -1.0f, 1.0f, 0.5f,   // ������� ����������            13-
	};

	// ��� ���������� �������
	unsigned int homeIndices[] = {  // ���
		0, 1, 3, // ������ �����������
		1, 2, 3,  // ������ �����������
		0, 1, 9,
		9, 10, 1,
		9, 10, 12,
		10, 11, 12,
		2, 3, 11,
		3, 11, 12,
	};

	unsigned int roofIndices[] = { // �����
		0, 3, 4,  // �����
		9, 12, 13,
	};

	unsigned int roofRoofIndices[] = { // �����
		0, 4, 9,
		9, 4, 13,
		3, 4, 12,
		12, 4, 13,
	};

	unsigned int windowIndices[] = { // ����
		5, 6, 7,  // ������� ����
		5, 7, 8
	};

	unsigned int floorIndices[] = { // ����
		1, 2, 10,
		10, 11, 2,
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// ������� ��������� ������ ���������� �������, ����� ��������� � ������������� ��������� �����(�), � ����� ������������� ��������� �������(�)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

	unsigned int roofRoofEBO;
	glGenBuffers(1, &roofRoofEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofRoofEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roofRoofIndices), roofRoofIndices, GL_STATIC_DRAW);

	unsigned int windowEBO;
	glGenBuffers(1, &windowEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(windowIndices), windowIndices, GL_STATIC_DRAW);

	unsigned int floorEBO;
	glGenBuffers(1, &floorEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHome);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureWindow);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureRoof);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureRoofRoof);

	glm::vec3 cubePositions[] = {
	    glm::vec3(0.0f,  0.0f,  0.0f),
	    glm::vec3(2.0f,  5.0f, -15.0f),
	    glm::vec3(-1.5f, -2.2f, -2.5f),
	    glm::vec3(-3.8f, -2.0f, -12.3f),
	    glm::vec3(2.4f, -0.4f, -3.5f),
	    glm::vec3(-1.7f,  3.0f, -7.5f),
	    glm::vec3(1.3f, -2.0f, -2.5f),
	    glm::vec3(1.5f,  2.0f, -2.5f),
	    glm::vec3(1.5f,  0.2f, -1.5f),
	    glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	// mouse settings
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	// ���� ����������
	while (!glfwWindowShouldClose(window))
	{
		// eval delta
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// ��������� �����
		processInput(window);

		// ���������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ����� ������ ���������
		if (is_filling_polugon) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		for (unsigned int i = 0; i < 10; i++)
		{
			// ������ ��� ������ �����������
			myShader.use();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

			glm::mat4 view = glm::mat4(1.0f);
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			glm::mat4 projection;
			projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

			int modelLoc = glGetUniformLocation(myShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			int viewLoc = glGetUniformLocation(myShader.ID, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			myShader.setInt("texture1", 0);
			glBindVertexArray(VAO); // ��������� � ��� ���� ������ ���� V��, �� ��� ������������� ��������� ��� ������ ��� (�� �� ������� ���, ����� �� ���� ������� ��������������)
			//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
			//lUseProgram(shaderProgram);
			//glUniform4f(vertexColorLocation, 1.0f, 0.5f, 0.2f, 1.0f);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, homeEBO);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			myShader.use();
			myShader.setInt("texture1", 2);
			//glUniform4f(vertexColorLocation, 1.0f, 0.2f, 0.2f, 1.0f);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofEBO);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			myShader.use();
			//glUniform4f(vertexColorLocation, 1.0f, 0.2f, 0.2f, 1.0f);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			myShader.use();
			myShader.setInt("texture1", 1);
			//glUniform4f(vertexColorLocation, 0.2f, 0.2f, 1.0f, 1.0f);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowEBO);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			myShader.use();
			myShader.setInt("texture1", 3);
			//glUniform4f(vertexColorLocation, 0.2f, 0.2f, 1.0f, 1.0f);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofRoofEBO);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

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
	// ������� ����
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// �������� ����� ���������
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !is_mode_changed) {
		is_filling_polugon = !is_filling_polugon;
		is_mode_changed = true;
	}
	else if (!glfwGetKey(window, GLFW_KEY_SPACE)) {
		is_mode_changed = false;
	}

	float cameraSpeed = 4.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// ����������, ��� ���� ��������� ������������� ����� �������� ����.
	// �������� ��������, ��� ������ ����� ����������� ������, ��� �������, �� Retina-��������
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov > 1.0f && fov < 45.0f)
		fov -= yoffset;
	else if (fov <= 1.0f)
		fov = 1.0f;
	else if (fov >= 45.0f)
		fov = 45.0f;
}