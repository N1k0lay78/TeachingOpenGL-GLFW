#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "stb_image.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool is_filling_polugon = true;
bool is_mode_changed = false;

int main()
{
	// glfw: инициализация и конфигурирование
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif

	// glfw: создание окна
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rjkz application", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: загрузка всех указателей на OpenGL-функции
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	// Компилирование нашей шейдерной программы
	Shader myShader("ver_1.vs", "frag_1.fs");

	unsigned int textureHome;
	glGenTextures(1, &textureHome);
	glBindTexture(GL_TEXTURE_2D, textureHome);

	// Устанавливаем параметры наложения и фильтрации текстур (для текущего связанного объекта текстуры)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		// Загрузка и генерация текстуры
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

	// Устанавливаем параметры наложения и фильтрации текстур (для текущего связанного объекта текстуры)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		int width, height, nrChannels;
		// Загрузка и генерация текстуры
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

	// Устанавливаем параметры наложения и фильтрации текстур (для текущего связанного объекта текстуры)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		int width, height, nrChannels;
		// Загрузка и генерация текстуры
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

	// Устанавливаем параметры наложения и фильтрации текстур (для текущего связанного объекта текстуры)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		int width, height, nrChannels;
		// Загрузка и генерация текстуры
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

	// Указывание вершин (и буферов) и настройка вершинных атрибутов
	float vertices[] = {
	0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  // верхняя правая                   0-
	0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  // нижняя правая                    1
	-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // нижняя левая                    2
	-0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  // верхняя левая                   3
	0.0f, 0.85f, 1.0f, 0.0f, 0.5f,   // верхняя посередине              4-
	0.35f, 0.35f, 1.001f, 1.0f, 1.0f,    // правая верхняя часть окн    5
	0.35f, -0.35f, 1.001f, 1.0f, 0.0f,   // правая нижняя часть окна    6
	-0.35f, -0.35f, 1.001f, 0.0f, 0.0f,  // левая нижняя часть окна     7 
	-0.35f, 0.35f, 1.001f, 0.0f, 1.0f,   // левая верхняя часть окна    8
	0.5f,  0.5f, -1.0f, 0.0f, 1.0f,  // верхняя правая                  9-
	0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // нижняя правая                   10
	-0.5f, -0.5f, -1.0f, 1.0f, 0.0f,  // нижняя левая                   11
	-0.5f,  0.5f, -1.0f, 1.0f, 1.0f,  // верхняя левая                  12
	0.0f, 0.85f, -1.0f, 1.0f, 0.5f,   // верхняя посередине            13-
	};

	// Как объединять вершины
	unsigned int homeIndices[] = {  // дом
		0, 1, 3, // первый треугольник
		1, 2, 3,  // второй треугольник
		0, 1, 9,
		9, 10, 1,
		9, 10, 12,
		10, 11, 12,
		2, 3, 11,
		3, 11, 12,
	};

	unsigned int roofIndices[] = { // крыша
		0, 3, 4,  // крыша
		9, 12, 13,
	};

	unsigned int roofRoofIndices[] = { // крыша
		0, 4, 9,
		9, 4, 13,
		3, 4, 12,
		12, 4, 13,
	};

	unsigned int windowIndices[] = { // окно
		5, 6, 7,  // пловина окна
		5, 7, 8
	};

	unsigned int floorIndices[] = { // окно
		1, 2, 10,
		10, 11, 2,
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как привязанный вершинный буферный объект для вершинного атрибута, так что после этого мы можем спокойно выполнить отвязку
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Вы можете отменить привязку VАО после этого, чтобы другие вызовы VАО случайно не изменили этот VAO (но подобное довольно редко случается).
	// Модификация других VAO требует вызов glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO (или VBO), когда это не требуется напрямую
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

	// Цикл рендеринга
	while (!glfwWindowShouldClose(window))
	{
		// Обработка ввода
		processInput(window);

		// Рендеринг
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// смена режима отрисовки
		if (is_filling_polugon) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		// Рисуем наш первый треугольник
		myShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view = glm::mat4(1.0f);
		// Обратите внимание, что мы перемещаем сцену в направлении, обратном направлению предполагаемого движения
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(myShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(myShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(myShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		myShader.setInt("texture1", 0);
		glBindVertexArray(VAO); // поскольку у нас есть только один VАО, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
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


		// glBindVertexArray(0); // не нужно каждый раз его отвязывать

		// glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода\вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: завершение, освобождение всех ранее задействованных GLFW-ресурсов
	glfwTerminate();
	return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
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

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Убеждаемся, что окно просмотра соответствует новым размерам окна.
	// Обратите внимание, что высота будет значительно больше, чем указано, на Retina-дисплеях
	glViewport(0, 0, width, height);
}