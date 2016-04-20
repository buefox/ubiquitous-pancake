#include "Instance.h"

Instance::Instance( int i, int c, int m ) {
	index = i;
	comp = c;
	memo = m;
}

int Instance::getIndex() {
	return index;
}
int Instance::getComp() {
	return comp;
}
int Instance::getMemo() {
	return memo;
}

void Instance::setIndex( int i ) {
	index = i;
}
void Instance::setComp( int c ) {
	comp = c;
}
void Instance::setMemo( int m ) {
	memo = m;
}
