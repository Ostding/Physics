#ifndef __ATAN2_TABLE_H__
#define __ATAN2_TABLE_H__

namespace physics
{
	class Atan2Table
	{
	public:
		static int BITS;
		static int BITS2;
		static int MASK;
		static int COUNT;
		static int DIM;
		static int table[16384];
	};
}
#endif