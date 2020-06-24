// lesson002.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
void processInput(GLFWwindow*window);
void framebuffer_size_callback(GLFWwindow*window, int w, int h);

//!顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"//out vec4 vertexColor; // 为片段着色器指定一个颜色输出\n"
			"out vec3 ourColor;//向片段着色器输出一个颜色\n"
			"void main() \n"
			"{\n"
			" gl_Position = vec4(aPos.x, aPos.y,aPos.z, 1.0);\n"
			" //vertexColor = vec4(0.5,0.0,0.0,1.0);\n"
			" ourColor = aColor;\n"
			"}\0";
const char *fragmentShaderSoure = "#version 330 core \n"
"in vec3 ourColor;"
"out vec4 FragColor;\n"
"//uniform vec4 ourColor;//在openGL程序代码中设定这个变量\n"
"void main()\n"
"{\n"
"//  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"FragColor = vec4(ourColor, 1.0);\n"
"}\0";



int main()
{
	glfwInit();//初始化 glfw
	//！设置gl 版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//MAC 
#ifdef __APPLE__ 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif // __APPLE__ 


	GLFWwindow *window = glfwCreateWindow(800, 600, "lesson002", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Glfw create failed!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//!制造 上下文
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	//GLAD 管理opengl函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		
		std::cout<<"failed to initialize GLAD"<<std::endl;
		return -1;
	}

	//构建和编译我们的着色程序

	///////////////////////// 尝试获取 着色器可用的顶点属性
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << std::endl;

	//顶点着色器
	//指定当前着色器为顶点着色器
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//设定顶点着色器代码
	glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
	//编译着色器
	glCompileShader(vertexShader);

	//检查着色器编译错误
	int success;//成功
	char infoLog[512];
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
	if (!success) {
		//得到 着色器错误信息
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::VERTEX::COMPILATLON_FAILED\n" << infoLog << std::endl;		
	}

	//!--------------片段着色器----------------
	//创建一个片段着色器
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//设置当前片段着色器代码
	glShaderSource(fragmentShader,1,&fragmentShaderSoure,NULL);
	//！编译当前片段着色器
	glCompileShader(fragmentShader);

	//检查当前片段着色器是否有错误
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS,&success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//*********链接着色器********************
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);

	//检查链接错误
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram,sizeof(infoLog),NULL,infoLog);
	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//!********删除着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[] = {
	-0.5f,-0.5f,0.0f,  1.0f, 0.0f, 0.0f,//右下
	0.5f, -0.5f,0.0f,  0.0f, 1.0f, 0.0f,//左下
	0.0f,  0.5f,0.0f,  0.0f, 0.0f, 1.0f //顶部
	};
	//!TODO:顶点缓冲对象
	unsigned int VBO, VAO;//vbo 顶点缓冲对象  vao 顶点数组对象
	glGenVertexArrays(1,&VAO);/// 顶点数组对象设置
	glGenBuffers(1,&VBO);/// 顶点缓冲对象设置
	glBindVertexArray(VAO);///顶点数组对象链接
	glBindBuffer(GL_ARRAY_BUFFER,VBO);///链接顶点缓冲对象
	//!设定缓冲数据类型
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	//!设置 顶点属性(位置)指针？？？？？
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FLOAT,6*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);//启动顶点属性s数组
	
	//！设置顶点属性指针（颜色）
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FLOAT,6 *sizeof(float),(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//***********************解除顶点缓冲对象绑定************
	glBindBuffer(GL_ARRAY_BUFFER,0);

	//！解除顶点数组对象绑定
	glBindVertexArray(0);




	while (!glfwWindowShouldClose(window))
	{
		//!处理输入事件
		processInput(window);
		//!渲染指令

		//设置清空颜色
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//根据时间更新三角形颜色
		//获取当前时间秒数
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f + 0.5f);
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//画第一个三角形
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,3);


		//！交换缓冲区
		glfwSwapBuffers(window);
		glfwPollEvents();//!检测有没有出发事件（键盘输入，鼠标移动等）

	}

	//释放、删除之前分配的所有资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
    std::cout << "Hello World!\n";
}

//！出来输入事件
void processInput(GLFWwindow*window) {
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
//框架缓冲区大小改变
void framebuffer_size_callback(GLFWwindow*window, int w, int h) {
	
	glViewport(0,0,w,h);
};
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
