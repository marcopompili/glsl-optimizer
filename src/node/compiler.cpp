#include "compiler.h"

using namespace v8;
using namespace node;


Persistent<Function> Compiler::constructor;

//----------------------------------------------------------------------

Compiler::Compiler(glslopt_target target)
{
	_binding = glslopt_initialize(target);
}

//----------------------------------------------------------------------

Compiler::~Compiler()
{
	release();
}

//----------------------------------------------------------------------

void Compiler::release()
{
	if (_binding)
	{
		glslopt_cleanup(_binding);

		_binding = 0;
	}
}

//----------------------------------------------------------------------

void Compiler::Init(Handle<Object> exports)
{
	Isolate *isolate = exports->GetIsolate();

	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Compiler"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "dispose", Dispose);

	// Export the class
	constructor.Reset(isolate, tpl->GetFunction());
	exports->Set(String::NewFromUtf8(isolate, "Compiler"), tpl->GetFunction());
}

//----------------------------------------------------------------------

void Compiler::New(const FunctionCallbackInfo<Value>& args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall())
	{
		glslopt_target target = kGlslTargetOpenGL;

		if (args[0]->IsInt32())
			target = (glslopt_target)args[0]->Int32Value();

		else if (args[0]->IsBoolean())
			target = (glslopt_target)( (int)args[0]->BooleanValue() );

		Compiler* obj = new Compiler(target);
		obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
	} else {
		const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
	}
}

//----------------------------------------------------------------------

void Compiler::Dispose(const FunctionCallbackInfo<Value>& args)
{
	Compiler* obj = ObjectWrap::Unwrap<Compiler>(args.This());
	obj->release();

	args.GetReturnValue().SetUndefined();
}
