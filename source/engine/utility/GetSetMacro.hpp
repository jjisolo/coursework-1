#pragma once

#define __gettersettertype GLuint

#define makeGetter(memberName, functionName) \
		__gettersettertype inline get##functionName() { return(this->memberName); }

#define makeSetter(memberName, functionName) \
		void set##functionName(__gettersettertype newVal) { ##memberName = newVal; }

#define makeGetterAndSetter(memberName, functionName) \
		makeGetter(memberName, functionName) \
		makeSetter(memberName, functionName)