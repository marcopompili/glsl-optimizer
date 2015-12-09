#ifndef SHADER_H
#define SHADER_H

#include "compiler.h"
#include <node.h>
#include <nan.h>
#include <glsl_optimizer.h>

class Shader : public node::ObjectWrap {
public:
	static void Init(v8::Handle<v8::Object> exports);

	inline bool isCompiled() const { return _compiled; }
	const char* getOutput() const;
	const char* getRawOutput() const;
	const char* getLog() const;

	void release();

private:
	Shader(Compiler* compiler, int type, const char* source);
	~Shader();

	glslopt_shader* _binding;
	bool _compiled;

	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Dispose(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void Compiled(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Output(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void RawOutput(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Log(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Persistent<v8::Function> constructor;
};

#endif
