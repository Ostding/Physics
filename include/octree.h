#ifndef __OCTREENODE_H__
#define __OCTREENODE_H__
#include <vector>
#include <map>
using namespace std;

#include "types.h"
#include "aabb.h"
#include "primitive.h"
#ifndef DISABLE_RENDER
#include "renderer.h"
#endif

namespace physics
{
	class OCTreeNode
	{
	public:
		int level;
		int index;
		OCTreeNode *parent;
		AABB aabb;

		typedef vector<OCTreeNode *> VectorNodes;
		typedef vector<Primitive *> VectorPrimitives;
		VectorPrimitives primitives;
		VectorNodes children;

	public:
		OCTreeNode(AABB aabb, int level, int index, OCTreeNode *parent);
		~OCTreeNode();

		static int MAX_LEVEL;
		static int MAX_OBJECTS;
		static void destroy(OCTreeNode *root);

	public:
		void insert(Primitive *cp);
		void remove(Primitive *cp);
		void refresh(OCTreeNode *root = NULL);

		typedef pair<Primitive *, Primitive *> ContactPair;
		typedef vector<ContactPair> PotentialContacts;
		typedef map<unsigned int, bool> MapContBool;
		void getPotentialContacts(PotentialContacts &contacts, MapContBool &mapIgnore);
		void getPotentialContacts(Primitive *cp, PotentialContacts &contacts, MapContBool &mapIgnore);

		VectorNodes &getChildren();
		AABB getAABB();
		bool isAllStatic;
		OCTreeNode *GetParent();

		// for debug
		void render();

	private:
		void getSplitCorners(Vector3 *corners);
		bool split();
		void getIndex(vector<int> &indices, Primitive *cp, bool ckBound = false, bool ckMore = false);
	};
}

#endif