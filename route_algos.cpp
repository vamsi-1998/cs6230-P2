#include "utils.h"
using namespace std;

struct NodeIdVCIdPair{
	string nodeId;
	int VCId;
};

NodeIdVCIdPair* nextStop(Node* currentNode,string destNodeId);

//Here the current node as we destination node will be a head node in their respective tiles
string nextStopIdHeadToHead(Node* currentNode,string destNodeId){
	//Move from this head node to the head node of another tile depending on the
	//type of L1 network.
	Node* L1Node = currentNode->L1Node;
	//destination node id in the L1 network
	string L1Id = L1Node->nodeId;
	string L1DestId = L1Id.substr(0,L1Id.find('N')+1) + getTileId(destNodeId);
	//Use the algo for the type of network that L1 is.
	NodeIdVCIdPair* pL1 = nextStop(L1Node,L1DestId);
	return (L1Node->L1L2IdMap)[pL1->nodeId];

}

NodeIdVCIdPair* nextStopChain(Node* currentNode,string destNodeId){
	NodeIdVCIdPair* p = new NodeIdVCIdPair();

	string currId = currentNode -> nodeId ;
	if(getTileId(currId) == getTileId(destNodeId))
	{

		//cout << "Tile ids are " << getTileId(currId) << " " << getTileId(destNodeId) << endl ;
		string currid = currId.substr(currId.find('N')+1);
		string destNId = destNodeId.substr(destNodeId.find('N')+1 );
		string pref = currId.substr(0,currId.find('N')+1);
		//cout << "currId is " << currid << " " << destNId << " " << pref << endl ;
		int curr = std::stoi(currid) ;
		int dest = std::stoi(destNId) ;
		// Finding the shortest path  between the two nodes 
		if( curr < dest )
		{
				int fin = curr+1 ;
				p->nodeId = pref + to_string(fin) ;

		}
		else
		{
			int fin = curr-1 ;
			p->nodeId = pref + to_string(fin) ;

		}
	}
	else
	{
		if(currentNode->isHead)
		{
				p->nodeId = nextStopIdHeadToHead(currentNode,destNodeId);
		}
		else
		{
				string currid = currId.substr(currId.find('N')+1);
				string headNodeId = currentNode -> headNodeId ;
				string headId = headNodeId.substr(destNodeId.find('N')+1 );
				string pref = currId.substr(0,currId.find('N')+1);
				//cout << "currId is " << currid << " " << destNId << " " << pref << endl ;
				int curr = std::stoi(currid) ;
				int head = std::stoi(headId) ;
				if( curr < head )
				{
					int fin = curr+1 ;
					p->nodeId = pref + to_string(fin) ;
				}
				else
				{
					int fin = curr-1 ;
					p->nodeId = pref + to_string(fin) ;

				}
		}
	}
	return p;
}

NodeIdVCIdPair* nextStopRing(Node* currentNode,string destNodeId){

	NodeIdVCIdPair* p = new NodeIdVCIdPair();
	string currId = currentNode -> nodeId ;

	if(getTileId(currId) == getTileId(destNodeId))
	{
 
		string currid = currId.substr(currId.find('N')+1);
		string destNId = destNodeId.substr(destNodeId.find('N')+1 );
		string pref = currId.substr(0,currId.find('N')+1);
		//cout << "currId is " << currid << " " << destNId << " " << pref << endl ;
		int curr = std::stoi(currid) ;
		int dest = std::stoi(destNId) ;
		if( curr < dest )
		{
				int fin = curr+1 ;
				p->nodeId = pref + to_string(fin) ;

		}
		else
		{
			int fin = curr-1 ;
			p->nodeId = pref + to_string(fin) ;

		}
	}
	else
	{
		if(currentNode->isHead)
		{
				p->nodeId = nextStopIdHeadToHead(currentNode,destNodeId);
		}
		else
		{
				string currid = currId.substr(currId.find('N')+1);
				string headNodeId = currentNode -> headNodeId ;
				string headId = headNodeId.substr(destNodeId.find('N')+1 );
				string pref = currId.substr(0,currId.find('N')+1);
				//cout << "currId is " << currid << " " << destNId << " " << pref << endl ;
				int curr = std::stoi(currid) ;
				int head = std::stoi(headId) ;
				if( curr < head )
				{
					int fin = curr+1 ;
					p->nodeId = pref + to_string(fin) ;
				}
				else
				{
					int fin = curr-1 ;
					p->nodeId = pref + to_string(fin) ;

				}
		}
	}
	return p;
}

NodeIdVCIdPair* nextStopMesh(Node* currentNode,string destNodeId){
	NodeIdVCIdPair* p = new NodeIdVCIdPair();


	string currId = currentNode->nodeId;


	if(getTileId(currId) == getTileId(destNodeId)){
		//This block of code will be executed in two cases :
		//when destination node is in the same L2 tile AND
		//when we are using this function for finding the route over the L1 network.


		//The content of the node ids after 'N'
		string currNId  = currId.substr(currId.find('N')+1);

		string destNId = destNodeId.substr(destNodeId.find('N')+1);

		string pref = currId.substr(0,currId.find('N')+1);
		int currX = std::stoi(currNId.substr(0,currNId.find("_")));
		int currY = std::stoi(currNId.substr(currNId.find("_")+1));

		int destX = std::stoi(destNId.substr(0,destNId.find("_")));
		int destY = std::stoi(destNId.substr(destNId.find("_")+1));

		if(currX != destX){
			int newX = currX > destX ? currX - 1 : currX + 1;
			p->nodeId = pref + to_string(newX) + "_" + to_string(currY);
		}
		else{
			int newY = currY > destY ? currY - 1 : currY + 1;
			p->nodeId = pref + to_string(currX) + "_" + to_string(newY);
		}

		//Since there is just 1 VC per physical link
		p->VCId = 0;


	}
	else
	{

		//In this case the current rounting will happen
		//from the head node of one type to the head node of
		//another type
		//The flit can be going from a tile node to a tile's head node OR
		//from the current tile's head node to another tile's head node.
		//Only these cases are possible.
		//
		if(currentNode->isHead){
			p->nodeId = nextStopIdHeadToHead(currentNode,destNodeId);
		}
		else{
			//Move from the current node towards the head node of this
			//tile
			string currNId  = currId.substr(currId.find('N')+1);
		        string headNodeId = currentNode->headNodeId;
			string headNId = headNodeId.substr(headNodeId.find('N')+1);


			int currX = std::stoi(currNId.substr(0,currNId.find("_")));
			int currY = std::stoi(currNId.substr(currNId.find("_")+1));

			int headX = std::stoi(headNId.substr(0,headNId.find("_")));
			int headY = std::stoi(headNId.substr(headNId.find("_")+1));

			string pref = currId.substr(0,currId.find('N')+1);
			if(currX != headX){
				int newX = currX > headX ? currX - 1 : currX + 1;
				p->nodeId = pref + to_string(newX) + "_" + to_string(currY);
			}
			else{
				int newY = currY > headY ? currY - 1 : currY + 1;
				p->nodeId = pref + to_string(currX) + "_" + to_string(newY);
			}



		}

		p->VCId = 0;

	}

	return p;

}


NodeIdVCIdPair* nextStopFoldedTorus(Node* currentNode,string destNodeId){
NodeIdVCIdPair* p = new NodeIdVCIdPair();

	return p;

}

//Utility function used in 2 places within the hypercube function
string nextStopIdWithinHypercube(string currNodeId,string destNodeId){
	//both the current and destination nodes are of type H in the same tile
	int n = currNodeId.size();
	//We will just flit 1 bit in the node level id
	string newNodeId = currNodeId;
	//Will never reach the char 'N' as it is compulsory that the current and destination
	//nodeId is not the same.
	while(currNodeId[n-1] != 'N'){
		if(currNodeId[n-1] != destNodeId[n-1]){
			break;
		}
		n--;
	}
	//Flip the first different bit from the back
	newNodeId[n-1] = newNodeId[n-1] == '0' ? '1' : '0';
	return newNodeId;

}

NodeIdVCIdPair* nextStopHypercube(Node* currentNode,string destNodeId){
	NodeIdVCIdPair* p = new NodeIdVCIdPair();

	string currNodeId = currentNode->nodeId;

	if(getTileId(currNodeId) == getTileId(destNodeId)){
		p->nodeId = nextStopIdWithinHypercube(currNodeId,destNodeId);
	}
	else{
		if(currentNode->isHead){
		        p->nodeId = nextStopIdHeadToHead(currentNode,destNodeId);
		}
		else{
			//Route in the direction of head node
			p->nodeId = nextStopIdWithinHypercube(currNodeId,currentNode->headNodeId);
		}

	}

	p->VCId = 0;

	return p;

}

NodeIdVCIdPair* nextStopButterfly(Node* currentNode,string destNodeId){

	NodeIdVCIdPair* p = new NodeIdVCIdPair();

	return p;

}

NodeIdVCIdPair* nextStop(Node* currentNode,string destNodeId){
	if(currentNode->type == 'C'){
		return nextStopChain(currentNode,destNodeId);
	}
	else if(currentNode->type == 'C'){
		return nextStopRing(currentNode,destNodeId);
	}
	else if(currentNode->type == 'R'){
		return nextStopRing(currentNode,destNodeId);
	}
	else if(currentNode->type == 'M'){
		return nextStopMesh(currentNode,destNodeId);
	}
	else if(currentNode->type == 'F'){
		return nextStopFoldedTorus(currentNode,destNodeId);
	}
	else if(currentNode->type == 'H'){
		return nextStopHypercube(currentNode,destNodeId);
	}
	else if(currentNode->type == 'B'){
		return nextStopButterfly(currentNode,destNodeId);
	}
	return NULL;

}
