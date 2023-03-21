#include "Window.hxx"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"

namespace gl {
    void frameBufferCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    bool Window::initialize() {
        if(!m_bInitialized) {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            
            #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    
            #endif

            if((m_pWindow = glfwCreateWindow(650, 480, "CourseWork", NULL, NULL)) == NULL) {
                fprintf(stderr, "Error: Unable to create GLFW window context");
                glfwTerminate();
                return false;
            }

            glfwMakeContextCurrent(m_pWindow);
            glfwSetFramebufferSizeCallback(m_pWindow, frameBufferCallback);

	        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
                fprintf(stderr, "Error: Unable to load gladLoadGLLoader")
	   	        glfwTerminate();
    	        return false;
            }

            m_uVertexBufferArray   = gl::util::createVertexBufferArray();
            m_uVertexBufferObjects = gl::util::createVertexBufferObjects();

            m_bInitialized = true;
        }

        return true;
    }

    void Window::render() const {
        if(!m_bInitialized) {
            return;
        }

        while (!glfwWindowShouldClose(m_pWindow)) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear     (GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(m_pWindow);
            glfwPollEvents ();
        }
    }

    void Window::release() const {
        if(m_bInitialized) {
            glfwTerminate();
        }
    }

}