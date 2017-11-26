/*
 * java_security_AccessController.cpp
 *
 *  Created on: 2017年11月25日
 *      Author: zhengxiaolin
 */

#include "native/java_security_AccessController.hpp"
#include <vector>
#include <algorithm>
#include <cassert>
#include "wind_jvm.hpp"
#include "native/native.hpp"

static unordered_map<wstring, void*> methods = {
    {L"doPrivileged:(" PA ")" OBJ,				(void *)&JVM_DoPrivileged},
};

// this method, I simplified it to just run the `run()` method.
void JVM_DoPrivileged(list<Oop *> & _stack){		// static
	Oop *pa = (Oop *)_stack.front();	_stack.pop_front();
	wind_jvm & vm = (*(wind_jvm *)_stack.back());	_stack.pop_back();
	assert(pa != nullptr);
	shared_ptr<Method> method = std::static_pointer_cast<InstanceKlass>(pa->get_klass())->get_this_class_method(L"run:()" VOD);
	assert(method != nullptr);
	Oop *result = vm.add_frame_and_execute(method, {pa});		// load the `this` obj
	_stack.push_back(result);
}

// 返回 fnPtr.
void *java_security_accesscontroller_search_method(const wstring & signature)
{
	auto iter = methods.find(signature);
	if (iter != methods.end()) {
		return (*iter).second;
	}
	return nullptr;
}



