#include <stdio.h>
#include <stdlib.h>
#include "User.h"

User::User( int i, int l, int num, bool apps[], bool reqs[] ) {
	index = i;
	location = l;
	num_apps = num;
	applications.resize( num_apps, false );
	if ( apps != NULL )
		for ( int j=0; j<num_apps; j++ ) applications[j] = apps[j];
	requests.resize( num_apps, false );
	if ( reqs != NULL )
		for ( int j=0; j<num_apps; j++ ) requests[j] = reqs[j];
}

int User::getIndex() {
	return index;
}
int User::getLocation() {
	return location;
}
int User::getNumApps() {
	return num_apps;
}
bool User::getApplication( int i ) {
	if ( i < getNumApps() ) return applications[i];
	else fprintf( stderr, "[ERROR] Invalid App. index %d\n", i );
	return false;
}
bool User::getRequest( int i ) {
	if ( i < getNumApps() ) return requests[i];
	else fprintf( stderr, "[ERROR] Invalid App. index %d\n", i );
	return false;
}

void User::setAll( int i, int l, int num, bool apps[], bool reqs[]=NULL ) {
	index = i;
	location = l;
	num_apps = num;
	applications.resize( num_apps, false );
	if ( apps )
		for ( int j=0; j<num_apps; j++ ) applications[j] = apps[j];
	requests.resize( num_apps, false );
	if ( reqs )
		for ( int j=0; j<num_apps; j++ ) requests[j] = reqs[j];
}
void User::setIndex( int i ) {
	index = i;
}
void User::setLocation( int l ) {
	location = l;
}
void User::setNumApps( int num ) {
	num_apps = num;
	applications.resize( num_apps, false );
	requests.resize( num_apps, false );
}
void User::setApplication( int i, bool b ) {
	if ( i < getNumApps() ) applications[i] = b;
	else fprintf( stderr, "[ERROR] Invalid App. index %d\n", i );
}
void User::setRequest( int i, bool b ) {
	if ( i < getNumApps() ) requests[i] = b;
	else fprintf( stderr, "[ERROR] Invalid App. index %d\n", i );
}

int User::movement( int choice ) {
	return rand() % choice;
}
void User::launchRequests() {
	for ( int i=0; i<getNumApps(); i++ )
		requests[i] = applications[i] & (rand() % 2?true:false);
}
