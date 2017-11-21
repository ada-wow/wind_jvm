/*
 * jni.cpp
 *
 *  Created on: 2017年11月19日
 *      Author: zhengxiaolin
 */

#include "native/native.hpp"
#include "native/java_lang_Object.hpp"
#include "native/java_lang_Thread.hpp"
#include "native/java_lang_System.hpp"

static unordered_map<wstring, function<void*(const wstring &)>> native_map;		// such as: {L"java/lang/Object", search [native method]'s method lambda for java/lang/Object}

void init_native()		// the same as "registerNatives" method.
{
	native_map[L"java/lang/Object"] = java_lang_object_search_method;
	native_map[L"java/lang/System"] = java_lang_system_search_method;
	native_map[L"java/lang/Thread"] = java_lang_thread_search_method;
}

// find a native method <$signature> in a klass <$klass_name>, return the method in (void *). if didn't find, abort().
void *find_native(const wstring & klass_name, const wstring & signature)	// such as: find_native(L"java/lang/Object", L"notify:()V")
{
	auto iter = native_map.find(klass_name);
	if (iter != native_map.end()) {
		return (*iter).second(signature);		// call the klass's find native method.	// maybe will get nullptr.
	} else {
		std::cerr << "didn't find this klass in native !! it didn't do registerNatives() function!!" << std::endl;
		assert(false);
	}
}