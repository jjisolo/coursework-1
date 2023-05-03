#pragma once

#define __gettertype GLuint

#define makeGetter(memberName, functionName) \
		__gettertype inline get##functionName() { return(this->memberName); }

#define makeSetter(memberName, functionName) \
		void set##functionName(__gettertype newVal) { ##memberName = newVal; }

#define makeGetterAndSetter(memberName, functionName) \
		makeGetter(memberName, functionName) \
		makeSetter(memberName, functionName)