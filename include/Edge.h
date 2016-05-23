#ifndef _EDGE_H
#define _EDGE_H
#include <vector>

class Edge{
	public:
		Edge() : index(-1), time_window(-1), bandwidth(-1), src_index(-1), dst_index(-1) {}
		Edge( int i, int w, int b, int s, int d );

		int getIndex();
		int getTimeWindow();
		
		int getBandwidth();
		int getSrcIndex();
		int getDstIndex();
		int getRemainBand( int );

		void setIndex( int );
		void setTimeWindow( int );
		
		void setBandwidth( int );
		void setSrcIndex( int );
		void setDstIndex( int );
		void setRemainBand( int, int b=0 );

		int getConnection( int );

	private:
		int index;
		int time_window;
		
		int bandwidth;
		int src_index;
		int dst_index;
		
		std::vector<int> remain_band;
};

#endif
