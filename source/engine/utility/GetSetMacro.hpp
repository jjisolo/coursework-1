#pragma once

#define makeGetter(memberName, functionName) \
		auto inline get##functionName() { return(this->memberName); }

#define makeSetter(memberName, functionName) \
		void set##functionName(GLuint newVal) { ##memberName = newVal; }

#define makeGetterAndSetter(memberName, functionName) \
		makeGetter(memberName, functionName) \
		makeSetter(memberName, functionName)