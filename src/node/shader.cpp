#include "shader.h"
#include <nan.h>

using namespace v8;
using namespace node;

Persistent<Function> Shader::constructor;

//----------------------------------------------------------------------

Shader::Shader(Compiler* compiler, int type, const char* source)
{
	if (compiler)
	{
		_binding = glslopt_optimize(compiler->getBinding(), (glslopt_shader_type)type, source, 0);
		_compiled = glslopt_get_status(_binding);
	}
	else
	{
		_binding = 0;
		_compiled = false;
	}
}

//----------------------------------------------------------------------

Shader::~Shader()
{
	release();
}

//----------------------------------------------------------------------

void Shader::release()
{
	if (_binding)
	{
		glslopt_shader_delete(_binding);
		_binding = 0;
		_compiled = false;
	}
}

//----------------------------------------------------------------------

const char* Shader::getOutput() const
{
	return (_compiled) ? glslopt_get_output(_binding) : "";
}

//----------------------------------------------------------------------

const char* Shader::getRawOutput() const
{
	return (_compiled) ? glslopt_get_raw_output(_binding) : "";
}

//----------------------------------------------------------------------

const char* Shader::getLog() const
{
	return (_compiled) ? glslopt_get_log(_binding) : "";
}

//----------------------------------------------------------------------

void Shader::Init(Handle<Object> exports)
{
	Isolate *isolate = exports->GetIsolate();

	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Shader"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "dispose", Dispose);
  NODE_SET_PROTOTYPE_METHOD(tpl, "compiled", Compiled);
  NODE_SET_PROTOTYPE_METHOD(tpl, "output", Output);
  NODE_SET_PROTOTYPE_METHOD(tpl, "rawOutput", RawOutput);
	NODE_SET_PROTOTYPE_METHOD(tpl, "log", Log);

	// Export the class
	constructor.Reset(isolate, tpl->GetFunction());
	exports->Set(String::NewFromUtf8(isolate, "Shader"), tpl->GetFunction());
}

//----------------------------------------------------------------------

void Shader::New(const FunctionCallbackInfo<Value>& args)
{
	Isolate *isolate = args.GetIsolate();

	// Checking arguments
	if(args.Length() != 3)
	{
	  isolate->ThrowException(Exception::TypeError(
	    String::NewFromUtf8(isolate, "Need three arguments")));

	  return;
	}

	// Checking compiler
	if(!args[0]->IsObject())
	{
		isolate->ThrowException(Exception::TypeError(
	    String::NewFromUtf8(isolate, "The compiler is not a valid Object")));

		return;
	}

	// Checking shader type
	if (!args[1]->IsInt32())
	{
		isolate->ThrowException(Exception::TypeError(
	    String::NewFromUtf8(isolate, "The shader type is not a valid Integer")));

		return;
	}

	// Checking the shader source code
	if (args[2]->IsString())
	{
		isolate->ThrowException(Exception::TypeError(
	    String::NewFromUtf8(isolate, "The source code is not a valid String")));

		return;
	}

	Compiler* compiler = ObjectWrap::Unwrap<Compiler>(args[0]->ToObject());
	int type = args[1]->Int32Value();
	String::Utf8Value sourceCode(args[2]->ToString());

	Shader* obj = new Shader(compiler, type, *sourceCode);

	obj->Wrap(args.This());

	const int argc = 1;
	Local<Value> argv[argc] = { args[0] };
	Local<Function> cons = Local<Function>::New(isolate, constructor);
	args.GetReturnValue().Set(cons->NewInstance(argc, argv));
}

//----------------------------------------------------------------------

void Shader::Dispose(const FunctionCallbackInfo<Value>& args)
{
	Shader* obj = ObjectWrap::Unwrap<Shader>(args.This());
	obj->release();

	args.GetReturnValue().SetUndefined();
}

//----------------------------------------------------------------------

void Shader::Compiled(const FunctionCallbackInfo<Value>& args)
{
	Isolate *isolate = args.GetIsolate();

	Shader* obj = ObjectWrap::Unwrap<Shader>(args.This());

	args.GetReturnValue().Set(Boolean::New(isolate, obj->isCompiled()));
}

//----------------------------------------------------------------------

void Shader::Output(const FunctionCallbackInfo<Value>& args)
{
	Isolate *isolate = args.GetIsolate();

	Shader* obj = ObjectWrap::Unwrap<Shader>(args.This());

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, obj->getOutput()));
}

//----------------------------------------------------------------------

void Shader::RawOutput(const FunctionCallbackInfo<Value>& args)
{
	Isolate *isolate = args.GetIsolate();

	Shader* obj = ObjectWrap::Unwrap<Shader>(args.This());

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, obj->getRawOutput()));
}

//----------------------------------------------------------------------

void Shader::Log(const FunctionCallbackInfo<Value>& args)
{
	Isolate *isolate = args.GetIsolate();

	Shader* obj = ObjectWrap::Unwrap<Shader>(args.This());

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, obj->getLog()));
}
