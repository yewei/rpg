#ifndef RPG_FIND_PATH_H
#define RPG_FIND_PATH_H

#include <queue>
#include <map>
#include <vector>
#include "RPG_Map.h"
#include "RPG_NPC.h"
using RPG_NS_Map::RPG_Map;
using std::priority_queue;
using std::map;
using std::vector;
using namespace RPG_NS_NPC;

namespace RPG_NS_FindPath
{
	class PQNode{
	public:	
		int x, y; //当前位置
		int w, f; //评估值
		int father; //先驱结点
		int moveStatus; //前面一个结点是怎么过来的

		bool operator< (const PQNode& a) const
		{
			return w+f > a.w+a.f;
		}
	};

	/*bool operator< (const PQNode& pq1, const PQNode& pq2){
		return pq1.w+pq1.f > pq2.w+pq2.f;
	}*/

	class FindPath
	{
		private:
			static int m_eX;
			static int m_eY; //目标点
			static RPG_Map* m_pMap;
			static map<int , bool> used;
			//static priority_queue<PQNode, vector<PQNode>, less<vector<PQNode>::value_type>> pq;
			static priority_queue<PQNode> pq;
			static vector<PQNode> pathList; //已经扩展过的结点队列


		private:
			static int calculateF(int x, int y); //计算评估函数
			static bool reachGoal(int x, int y) { return x==m_eX&&y==m_eY;}
			static void setMap(RPG_Map* pMap) {m_pMap = pMap;}
			static bool canMove(int x, int y) //判断(x,y)是否能走
				{return m_pMap->canMov(x, y) && !used[m_pMap->getMapSize().width*y+x];}
			static void setUsed(int x, int y) {used[m_pMap->getMapSize().width*y+x]=true;}
			static void expandNode(int father, PQNode& node); //扩展node结点,father指明其放在pathList的位置

		public:
			static bool findPath(RPG_Map* pMap, int sx, int sy, int ex, int ey, std::vector<int>* command);
			//在地图上找到一个路径,路径命令结果存放在command中, A* 算法

			
	};
}


#endif