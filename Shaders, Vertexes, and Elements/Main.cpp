#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

const GLchar* vertexSource =
	"#version 150 core\n"

	"in vec2 position;"
	"in vec3 color;"
	"out vec3 Color;"

	"void main() {"
	"	Color = color;"
	"	gl_Position = vec4(position, 0.0, 1.0);"
	"}";

const GLchar* fragmentSource =
	"#version 150 core\n"

	"in vec3 Color;"
	"out vec4 outColor;"

	"void main() {"
	"	outColor = vec4(Color, 1.0);"
	"}";

int main()
{
	glfwInit();	//initializes GLFW

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);	//creation of the window
	/*parameters: width, height, title,
		windowmode (nullptr for windowed and glfwGetPrimaryMonitor() for fullscreen),
		idk what the last one does really
	*/

	glfwMakeContextCurrent(window);	//creates a context for the window
	
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	
	printf("%u\n", vertexBuffer);
	/*All of this glew code checks which functions are available at runtime
		and links them dynamically.
	*/

	GLuint vao;	//Vertex Arry Object
	glGenVertexArrays(1, &vao);	//generates a arrays for vertexs
	glBindVertexArray(vao);	//binds the VAO to make it active

	GLuint vbo;	//Vertex Buffer Object
	glGenBuffers(1, &vbo);	//generates a buffer for vertexs

	GLuint ebo;	//Element Buffer Object
	glGenBuffers(1, &ebo);	//generates a buffer for elements

	GLuint elements[] = {
		0,1,2,	//first half of the rect
		2,3,0	//second half
	};

	GLfloat vertices[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left:		red
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right:		green
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right:	blue
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left:		white
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);	//make the buffer active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//writes the data fro the vertices array to the buffer
	//GL_STATIC_DRAW => data uploaded one and drawn many times
	//GL_DYNAMIC_DRAW => data will be changed from time to time, but drawn many times more than that
	//GL_STREAM_DRAW => data will change almost every time it's drawn

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//creation and establishment of a vertex shader

	glCompileShader(vertexShader);	//compiles the shader to code to run on the GPU

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	//creation and establishment of a fragment shader

	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//creates a program for the shaders and adds them to it to work together

	glLinkProgram(shaderProgram);	//links the shader together
	glUseProgram(shaderProgram);	//starts the shaders used

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	//finds the "position" reference within the shader program

	glEnableVertexAttribArray(posAttrib);//enabls the vertex attribute array
	
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
	//parameters: reference, # of values for input, type of value, to normalize or not

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));

	while (!glfwWindowShouldClose(window)) {	//essential "while running"
		glfwSwapBuffers(window);	//swaps buffers after they are done being drawn
		glfwPollEvents();	//Poll for events that fire

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		//checks to see if the escape key is press and will close the window if so

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/*parameters
			 -primative type: point, line, or triangle
			 -# of indicies to draw
			 -type of the element data
			 -# of vertices to skip
		*/
	}

	//Deallocation
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();	//terminates GLFW
	return 0;
}