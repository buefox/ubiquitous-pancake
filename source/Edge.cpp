#include <stdio.h>
#include "Edge.h"

Edge::Edge( int i, int b, int s, int d ) {
	index = i;
	bandwidth = b;
	src_index = s;
	dst_index = d;
}

int Edge::getIndex() {
	return index;
}
int Edge::getBandwidth() {
	return bandwidth;
}
int Edge::getSrcIndex() {
	return src_index;
}
int Edge::getDstIndex() {
	return dst_index;
}

void Edge::setIndex( int i ) {
	index = i;
}
void Edge::setBandwidth( int b ) {
	bandwidth = b;
}
void Edge::setSrcIndex( int s ) {
	src_index = s;
}
void Edge::setDstIndex( int d ) {
	dst_index = d;
}

int Edge::getConnection( int s ) {
	return s==src_index? dst_index : s==dst_index? src_index : -1;
}

