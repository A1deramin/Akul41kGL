#define GL_CHECK_ERRORS GLCheckErrors UNIQ_ID(__LINE__)

extern "C" {
	__declspec(dllexport) int NvOptimusEnablement = 0x00000001;
}
static void glClearError() {
	while (glGetError())
		;
}
static void glCheckError() {
	while (auto error = glGetError()) {
		std::cout << "[OpenGL Error] ("
			<< "0x" << std::hex << error << " )" << std::endl;
	}
}
class GLCheckErrors {
public:
	GLCheckErrors() { glClearError(); }
	~GLCheckErrors() { glCheckError(); }
};