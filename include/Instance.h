#ifndef _INSTANCE_H
#define _INSTANCE_H

class Instance {
	public:
		Instance() : comp(-1), memo(-1), index(-1) {}
		Instance( int, int, int );

		int getIndex();
		int getComp();
		int getMemo();

		void setIndex( int );
		void setComp( int );
		void setMemo( int );

	protected:
		int comp;
		int memo;

	private:
		int index;
};

#endif
