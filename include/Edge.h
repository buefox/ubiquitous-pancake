#ifndef _EDGE_H
#define _EDGE_H

class Edge{
	public:
		Edge() : index(-1), bandwidth(-1), src_index(-1), dst_index(-1) {}
		Edge( int i, int b, int s, int d );

		int getIndex();
		int getBandwidth();
		int getSrcIndex();
		int getDstIndex();

		void setIndex( int );
		void setBandwidth( int );
		void setSrcIndex( int );
		void setDstIndex( int );
		
		int getConnection( int );

	private:
		int index;
		int bandwidth;
		int src_index;
		int dst_index;
};

#endif
