#include <stdio.h>
#include "Edge.h"

Edge::Edge( int i, int w, int b, int s, int d ) {
	index = i;
	time_window = w;
	bandwidth = b;
	src_index = s;
	dst_index = d;
	remain_band.resize( time_window+1, bandwidth );
}

int Edge::getIndex() {
	return index;
}
int Edge::getTimeWindow() {
	return time_window;
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
int Edge::getRemainBand( int t ) {
	if ( t <= time_window ) return remain_band[t];
	else fprintf( stderr, "[ERROR] Invalid time slot %d\n", t );
	return -1;
}

void Edge::setIndex( int i ) {
	index = i;
}
void Edge::setTimeWindow( int w ) {
	time_window = w;
	remain_band.resize( time_window+1, bandwidth );
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
void Edge::setRemainBand( int t, int b ) {
	if ( t <= time_window ) remain_band[t] = ( b ? b : bandwidth );
	else fprintf( stderr, "[ERROR] Invalid time slot %d\n", t );
}

int Edge::getConnection( int s ) {
	return s==src_index? dst_index : s==dst_index? src_index : -1;
}