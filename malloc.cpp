#include <iostream>
#include <list>
#include <stdio.h>

using namespace std;

struct node 
{
	int id;
	int size;
	int indexOfStart;
	int indexOfEnd;
	bool allocated;
};

class MemoryManager
{
	list<node> List;
	list<node>::iterator pos;
	list<node>::reverse_iterator reverse_pos;
	int countCoalescing;
	int countMemoryAllocationFailure;

	public:
	MemoryManager() {
		addNode(0, 7937, 256, 8192, false);
		countCoalescing = 0;
		countMemoryAllocationFailure = 0;
	}
	bool allocateManager(int id, int size) {
		int sumOfFreeSpace = calculateSumOfFreeSpace();
		int maxAvailableFreeSpace = findMaxAvailableFreeSpace ();
		bool successOfAllocation = false;

		if ( sumOfFreeSpace == 0 ) {
			return false;
		}
		else if ( size <= sumOfFreeSpace ) {
			if ( size <= maxAvailableFreeSpace ) {
				successOfAllocation = allocate (id, size);
				return true;
			} else if ( size > maxAvailableFreeSpace ) {
				coalescing();
				countCoalescing++;
				maxAvailableFreeSpace = findMaxAvailableFreeSpace ();
				if ( size <= maxAvailableFreeSpace ) {
					allocate (id, size );
					return true;
				} else {
					countMemoryAllocationFailure++;
					return false;
				}

			} else
				;
		} else
			;
	}	

	bool free(int id) {
		for ( pos = List.begin(); pos != List.end(); pos++ ){
			if ( (*pos).allocated == true && (*pos).id == id ) {
				(*pos).allocated = false;
				break;
			}
		}

	}

	void status(FILE* file) {
		int sumOfFreeSpace = calculateSumOfFreeSpace();
		int sumOfAllocatedSpace = calculateSumOfAllocatedSpace();
	
		fprintf (file, "Current Memory Allocation statue\n");
		fprintf (file, "-------------------------------------------\n");
		fprintf (file, "Available memory space : %d\n", sumOfFreeSpace);
		int i = 1;
		for ( pos = List.begin(); pos != List.end(); pos++ ){
			if ( (*pos).allocated == false ) {
				fprintf (file, "%d ", i);
				fprintf (file, "%d-", (*pos).indexOfStart );
				fprintf (file, "%d\n", (*pos).indexOfEnd );
				i++;
			}
		}
		fprintf (file, "Allocated memory space : %d\n", sumOfAllocatedSpace);
		i = 1;
		for ( pos = List.begin(); pos != List.end(); pos++ ){
			if ( (*pos).allocated == true ) {
				fprintf (file, "%d ", i);
				fprintf (file, "%d-", (*pos).indexOfStart );
				fprintf (file, "%d\n", (*pos).indexOfEnd );
				i++;
			}
		}
		fprintf (file, "coalescing count : %d\n", countCoalescing);
		fprintf (file, "memory allocation failure count : %d\n\n", countMemoryAllocationFailure);


		//print();
	}

	void print() {
		for ( pos = List.begin(); pos != List.end(); pos++ ){
			cout << (*pos).id << " ";
			cout << (*pos).size << " ";
			cout << "[" << (*pos).indexOfStart << "-";
			cout << (*pos).indexOfEnd << "]" << endl;
		}
		cout << "List.size(): " << List.size() << endl;
		cout << endl << endl;
	}

	private:
	bool allocate(int id, int size) {
		int maxAvailableFreeSpace = findMaxAvailableFreeSpace();
		if ( maxAvailableFreeSpace == 0 )
			return false;
		else {
			for ( pos = List.begin(); pos != List.end(); pos++ ){
				if ( (*pos).allocated == false && maxAvailableFreeSpace == (*pos).size ) {
					node myAllocatedNode = makeNode(id, size, (*pos).indexOfStart, (*pos).indexOfStart + size-1, true );
					node myFreeNode = makeNode(0, (*pos).size - size, (*pos).indexOfStart+size, (*pos).indexOfEnd, false );
					pos = List.erase(pos);
					if ( myFreeNode.indexOfStart >= 8192) {
						;
					} else {
						pos = List.insert(pos, myFreeNode );
					}
					pos = List.insert(pos, myAllocatedNode );
					return true;
				}
			}
			return false;
		}
	}


	int findMaxAvailableFreeSpace () {
		int maxAvailableFreeSpace = 0;
		for ( pos = List.begin(); pos != List.end(); pos++ ){
			if ( (*pos).allocated == false && maxAvailableFreeSpace <= (*pos).size )
				maxAvailableFreeSpace = (*pos).size;
		}
		return maxAvailableFreeSpace;
	}
	int calculateSumOfFreeSpace () {
		int sumOfFreeSpace = 0;
		for ( pos = List.begin(); pos != List.end(); pos++ ){
			if ( (*pos).allocated == false )
				sumOfFreeSpace += (*pos).size;
		}
		return sumOfFreeSpace;

	}
	int calculateSumOfAllocatedSpace () {
		int sumOfAllocatedSpace = 0;
		for ( pos = List.begin(); pos != List.end(); pos++ ){
			if ( (*pos).allocated == true )
				sumOfAllocatedSpace += (*pos).size;
		}
		return sumOfAllocatedSpace;

	}

	void addNode (int id, int size, int indexOfStart, int indexOfEnd, bool allocated ) {
		node myNode;
		myNode.id = id;
		myNode.size = size;
		myNode.indexOfStart = indexOfStart;
		myNode.indexOfEnd = indexOfEnd;
		myNode.allocated = allocated;
		List.push_back(myNode);
	}

	node makeNode (int id, int size, int indexOfStart, int indexOfEnd, bool allocated ) {
		node myNode;
		myNode.id = id;
		myNode.size = size;
		myNode.indexOfStart = indexOfStart;
		myNode.indexOfEnd = indexOfEnd;
		myNode.allocated = allocated;
		return myNode;
	}

	bool coalescing() {
		node myNodePrevious;
		for ( pos = List.begin(); pos != List.end(); pos++ ){
			if ( (*pos).allocated == false ) {
				myNodePrevious.id = (*pos).id;
				myNodePrevious.size = (*pos).size;
				myNodePrevious.indexOfStart = (*pos).indexOfStart;
				myNodePrevious.indexOfEnd = (*pos).indexOfEnd;
				myNodePrevious.allocated = (*pos).allocated;
				pos = List.erase(pos);
				if ( (*pos).allocated == true || pos == List.end() ) {
					pos = List.insert(pos, myNodePrevious);
					//pos++;
				}
				else {
					(*pos).id = 0;
					(*pos).size = (*pos).size + myNodePrevious.size;
					(*pos).indexOfStart = myNodePrevious.indexOfStart;
					(*pos).indexOfEnd = (*pos).indexOfEnd;
					(*pos).allocated = false;
				}
			}
		}
	}

};

int main()
{
	MemoryManager myMemoryManager;
	FILE *inFile = fopen("scenario.in", "r");
	FILE *outFile = fopen ("result.out", "w");
	while(1) {
		char command;
		int id, size;
		int result = fscanf(inFile, "%c", &command);
		if ( result == EOF )
			break;
		switch (command) {
			case 'A':
				fscanf(inFile, "%d", &id);
				fscanf(inFile, "%d", &size);
				myMemoryManager.allocateManager(id, size);
				break;
			case 'F':
				fscanf(inFile, "%d", &id);
				myMemoryManager.free(id);
				break;
			case 'S':
				myMemoryManager.status(outFile);
				//myMemoryManager.print();
				break;
			default:
				break;
		}
	}	
	fclose(outFile);
	fclose(inFile);
}
