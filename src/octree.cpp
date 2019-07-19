#include "octree.h"

namespace physics
{
  int OCTreeNode::MAX_LEVEL = 10; 
	int OCTreeNode::MAX_OBJECTS = 10;

	OCTreeNode::OCTreeNode(AABB aabb, int level, int index, OCTreeNode *parent)
		:level(level)
		,index(index)
		,aabb(aabb)
		,parent(parent)
		,isAllStatic(true)
	{}

	OCTreeNode::~OCTreeNode()
	{}

	void OCTreeNode::destroy(OCTreeNode *root)
	{
		for (VectorNodes::iterator it = root->children.begin(); it != root->children.end(); it++)
		{
			OCTreeNode *p = *it;
			OCTreeNode::destroy(p);
		}
		delete root;
		root = 0;
	}

	void OCTreeNode::getSplitCorners(Vector3 *corners)
	{
		Vector3 center = aabb.getCenter();
		Vector3 halfSize = (aabb.max - aabb.min) * ffhalf;
		Vector3 extendX(halfSize.x, ffzero, ffzero);
		Vector3 extendY(ffzero, halfSize.y, ffzero);
		Vector3 extendZ(ffzero, ffzero, halfSize.z);

		// left up front
		corners[0] = center - extendX;
		corners[1] = center + extendY + extendZ;

		// right up front
		corners[2] = center;
		corners[3] = center + extendX + extendY + extendZ;

		// left bottom front
		corners[4] = center - extendX - extendY;
		corners[5] = center + extendZ;

		// right bottom front
		corners[6] = center - extendY;
		corners[7] = center + extendX + extendZ;

		// left up back
		corners[8] = center - extendX - extendZ;
		corners[9] = center + extendY;

		// right up back
		corners[10] = center - extendZ;
		corners[11] = center + extendX + extendY;

		// left bottom back
		corners[12] = center - extendX - extendY - extendZ;
		corners[13] = center;

		// right bottom back
		corners[14] = center - extendY - extendZ;
		corners[15] = center + extendX;
	}

	bool OCTreeNode::split()
	{
		if (!children.empty())
			return false;

		int level = level + 1;
		Vector3 corners[16];
		getSplitCorners(corners);

		for (int i = 0; i < 8; i++)
		{
			Vector3 min = corners[i * 2];
			Vector3 max = corners[i * 2 + 1];
			OCTreeNode *child = new OCTreeNode(AABB(min, max), level, i, this);
			children.push_back(child);
		}
		return true;
	}

	void OCTreeNode::getIndex(std::vector<int> &indices, Primitive * cp, bool ckBound, bool ckMore)
	{
		if (ckBound)
		{
			if (!aabb.intersect(cp->aabb))
			{
				return;
			}
		}

		if (children.empty())
		{
			return;
		}

		for (size_t i = 0; i < children.size(); i++)
		{
			OCTreeNode *v = children[i];
			if (v->aabb.intersect(cp->aabb))
			{
				indices.push_back(i);
			}
			if (!ckMore && !indices.empty())
			{
				cp->interCount = indices.size();
				return;
			}

			//this is optimize
			//if intersection more than two, it means that primitive should be hold in parent node
			if (indices.size() > 1)
			{
				cp->interCount = indices.size();
				return;
			}
		}
	}

	void OCTreeNode::remove(Primitive *cp)
	{
		for (VectorPrimitives::iterator it = primitives.begin(); it != primitives.end(); it++)
		{
			if (*it == cp)
			{
				cp->node = 0;
				primitives.erase(it);
				break;
			}
		}
	}

	void OCTreeNode::insert(Primitive * cp)
	{
		if (!children.empty())
		{
			std::vector<int> indices;
			getIndex(indices, cp, false, true);
			if (indices.size() == 1)
			{
				int index = indices[0];
				OCTreeNode *child = children[index];
				child->insert(cp);
				return;
			}
		}

		cp->node = this;
		primitives.push_back(cp);

		if (primitives.size() > OCTreeNode::MAX_OBJECTS && level < OCTreeNode::MAX_LEVEL)
		{
			bool suc = split();
			//If real split space, should insert all primitives to sub space nodes
			if (suc) {
				std::vector<int> indices;
				for (int i = primitives.size() - 1; i >= 0; i--)
				{
					Primitive *v = primitives[i];
					indices.clear();
					getIndex(indices, v, false, true);

					if (indices.size() == 1)
					{
						int index = indices[0];
						children[index]->insert(v);
						primitives.erase(primitives.begin() + i);
					}
				}
			}//Other wise just insert last one into sub space node
			else
			{
				Primitive *v = primitives.back();
				std::vector<int> indices;
				getIndex(indices, v, false, true);
				
				if (indices.size() == 1)
				{
					primitives.pop_back();
					int index = indices[0];
					children[index]->insert(v);
				}
			}
		}
		else
		{
			if(!cp->isStatic)
				isAllStatic = false;
		}
	}

	OCTreeNode * OCTreeNode::GetParent()
	{
		return parent;
	}

	void OCTreeNode::refresh(OCTreeNode *root)
	{
		if (root == 0)
			root = this;

		isAllStatic = true;

		VectorPrimitives toInsert;
		std::vector<int> indices;
		for (int i = primitives.size()-1; i >= 0; i--)
		{
			
			Primitive *p = primitives[i];
			//plane does not contain a rigidbody
			if (p->body && !p->body->isAwake) continue;
			//static primitive which intersect with more than one sub space node 
			//or this is leaf node
			if(p->isStatic && (p->interCount > 1 || children.empty())) continue;

			if (!p->isStatic)
			{
				p->refreshAABB();

				//check is no longer belong to this node
				if (!p->aabb.intersect(aabb))
				{
					primitives.erase(primitives.begin() + i);
					toInsert.push_back(p);
					continue;
				}
				else
				{
					isAllStatic = false;
				}
			}

			indices.clear();
			getIndex(indices, p, true, true);
			
			if (indices.empty())
			{//if primitive nolonger belongs to this node, we should reinsert it;
				primitives.erase(primitives.begin() + i);
				toInsert.push_back(p);
			}
			else if (indices.size() == 1)
			{//if primitive belongs to this node, and intersects with only one child, then move it to the child
				int index = indices[0];
				OCTreeNode *node = children[index];
				if (p->node != node)
				{
					primitives.erase(primitives.begin() + i);
					node->insert(p);
				}
			}
			else//if primitive intersects with more than one node, we hold it in this node;
			{
				if (!p->isStatic)
					isAllStatic = false;
			}
		}

		//reinsert
		for (VectorPrimitives::iterator it = toInsert.begin(); it != toInsert.end(); it++)
		{
			root->insert(*it);
		}

		for (VectorNodes::iterator it = children.begin(); it != children.end(); it ++)
		{
			(*it)->refresh(root);
		}
	}

	void OCTreeNode::getPotentialContacts(PotentialContacts &contacts, MapContBool &mapIgnore)
	{
		for (int i = 0; i < primitives.size(); i++)
		{
			Primitive *a = primitives[i];
			//check intersection with each other
			if(!isAllStatic)
			{
				for (int j = i + 1; j < primitives.size(); j++)
				{
					Primitive *b = primitives[j];
					//static primitives no need to check intersection
					if (a->isStatic && b->isStatic)
						continue;

					unsigned int t = (a->tContact | b->tContact);
					MapContBool::iterator it = mapIgnore.find(t);
					if(it == mapIgnore.end() || (it != mapIgnore.end() && !it->second))
					{
						if (a->aabb.intersect(b->aabb))
							contacts.push_back(std::make_pair(a, b));
					}
				}
			}
			//check intersection with children's primitives
			for (VectorNodes::iterator it = children.begin(); it != children.end(); it++)
			{
				OCTreeNode *p = *it;
				if (a->aabb.intersect(p->aabb))
				{
					p->getPotentialContacts(a, contacts, mapIgnore);
				}
			}
		}
		//check children
		for (VectorNodes::iterator it = children.begin(); it != children.end(); it++)
		{
			OCTreeNode *p = *it;
			p->getPotentialContacts(contacts, mapIgnore);
		}
	}

	void OCTreeNode::getPotentialContacts(Primitive *cp, PotentialContacts &contacts, MapContBool &mapIgnore)
	{
		//check intersection with local primitives
		if(!cp->isStatic || !isAllStatic)
		{
			for (VectorPrimitives::iterator it = primitives.begin(); it != primitives.end(); it++)
			{
				Primitive *p = *it;
				if (cp->isStatic && p->isStatic)
					continue;

				unsigned int t = cp->tContact | p->tContact;
				MapContBool::iterator itor = mapIgnore.find(t);
				if (itor == mapIgnore.end() || (itor != mapIgnore.end() && !itor->second))
				{
					if (cp->aabb.intersect(p->aabb))
						contacts.push_back(std::make_pair(cp, p));
				}
			}
		}
		//check intersection with children's primitives
		for (VectorNodes::iterator it = children.begin(); it != children.end(); it++)
		{
			OCTreeNode *p = *it;
			if (cp->aabb.intersect(p->aabb))
			{
				p->getPotentialContacts(cp, contacts, mapIgnore);
			}
		}
	}

	void OCTreeNode::render()
	{
#ifdef DEBUG_RENDER
		if (children.size() == 0)
		{
			Renderer::renderAABB(aabb);
		}
		else
		{
			for (VectorNodes::iterator it = children.begin(); it != children.end(); it++)
			{
				OCTreeNode *p = *it;
				p->render();
			}
		}
#endif
	}

	OCTreeNode::VectorNodes &OCTreeNode::getChildren()
	{
		return children;
	}

	AABB OCTreeNode::getAABB()
	{
		return aabb;
	}
}