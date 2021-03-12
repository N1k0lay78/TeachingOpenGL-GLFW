#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

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

	// Компилирование нашей шейдерной программы
	Shader myShader("ver_1.vs", "frag_1.fs");

	// Указывание вершин (и буферов) и настройка вершинных атрибутов
	float vertices[] = {
	0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // верхняя правая                   0
	0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // нижняя правая                    1
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,  // нижняя левая                    2
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // верхняя левая                   3
	0.0f, 0.85f, 0.0f, 0.0f, 0.0f, 1.0f,   // верхняя посередине              4
	0.2f, 0.2f, 0.0f, 0.2f, 0.2f, 1.0f,    // правая верхняя часть окна       5
	0.2f, -0.2f, 0.0f, 0.2f, 0.2f, 1.0f,   // правая нижняя часть окна        6
	-0.2f, -0.2f, 0.0f, 0.2f, 0.2f, 1.0f,  // левая нижняя часть окна         7 
	-0.2f, 0.2f, 0.0f, 0.2f, 0.2f, 1.0f,   // левая верхняя часть окна        8
	};

	// Как объединять вершины
	unsigned int homeIndices[] = {  // дом
		0, 1, 3, // первый треугольник
		1, 2, 3  // второй треугольник
	};

	unsigned int roofIndices[] = { // крыша
		0, 3, 4  // крыша
	};

	unsigned int windowIndices[] = { // окно
		5, 6, 7,  // пловина окна
		5, 7, 8
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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

	unsigned int windowEBO;
	glGenBuffers(1, &windowEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(windowIndices), windowIndices, GL_STATIC_DRAW);

	// Раскомментируйте следующую строку для отрисовки полигонов в режиме каркаса
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Цикл рендеринга
	while (!glfwWindowShouldClose(window))
	{
		// Обработка ввода
		processInput(window);

		// Рендеринг
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// смена режима отрисовки
		if (is_filling_polugon) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		// Рисуем наш первый треугольник
		myShader.use();
		glBindVertexArray(VAO); // поскольку у нас есть только один VАО, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//lUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 1.0f, 0.5f, 0.2f, 1.0f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, homeEBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		myShader.use();
		//glUniform4f(vertexColorLocation, 1.0f, 0.2f, 0.2f, 1.0f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofEBO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		myShader.use();
		//glUniform4f(vertexColorLocation, 0.2f, 0.2f, 1.0f, 1.0f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowEBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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