#include <node.h>
#include <nan.h>
#include "shader.h"
#include <glsl_optimizer.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
	Isolate *isolate = exports->GetIsolate();

	// Export constants
	exports->Set(Symbol::New(isolate, String::NewFromUtf8(isolate, "VERTEX_SHADER")),
																		Int32::New(isolate, kGlslOptShaderVertex));

	exports->Set(Symbol::New(isolate, String::NewFromUtf8(isolate, "FRAGMENT_SHADER")),
																		Int32::New(isolate, kGlslOptShaderFragment));

	exports->Set(Symbol::New(isolate, String::NewFromUtf8(isolate, "TARGET_OPENGL")),
																		Int32::New(isolate, kGlslTargetOpenGL));

	exports->Set(Symbol::New(isolate, String::NewFromUtf8(isolate, "TARGET_OPENGLES20")),
																		Int32::New(isolate, kGlslTargetOpenGLES20));

	exports->Set(Symbol::New(isolate, String::NewFromUtf8(isolate, "TARGET_OPENGLES30")),
																		Int32::New(isolate, kGlslTargetOpenGLES30));

	// Export classes
	Compiler::Init(exports);
	Shader::Init(exports);
}

NODE_MODULE(glslOptimizer, InitAll);
