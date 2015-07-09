#include<iostream>
#include<string>
#include<vector>
using namespace std;

struct state_node {
	string stateRps;
	  int cost_g;
	  int cost_h;
	  int cost_f;
	  state_node *parent;
	  char action;

};

struct MatrixIndex
{
	int XIndex;
	int YIndex;
};


/*                      Function            */

void fetch(char [],char []);
void traverseNode(state_node *node);
bool isSolvable(char input[]);
int getInvCount(char input[]);

void Print_str(char []);

void change_To_num(int *,char []);    //Turn String to NUM matrix

void change_To_Str(int matrix[4][4],char []);  //Turn NUM matrix to String 

void swap(int *a,int *b);

void decide_can_go(int XIndex,int YIndex,bool b[4]); //Return UDLR that can go 

int Successor_Function(state_node node,string nextState[],char nextAction[]); 

class BinaryMinHeap {
private:
      //int *data;
	  
	  state_node *data;


      int heapSize;
      int arraySize;
 
      int getLeftChildIndex(int nodeIndex) {
            return 2 * nodeIndex + 1;
      }
 
      int getRightChildIndex(int nodeIndex) {
            return 2 * nodeIndex + 2;
      }
 
      int getParentIndex(int nodeIndex) {
            return (nodeIndex - 1) / 2;
      }
 
public:
      BinaryMinHeap(int size) {
            data = new state_node[size];
            heapSize = 0;
            arraySize = size;
      }    
 


      state_node getMinimum() 
	  {
           if (isEmpty()==true)
			 {
            cout<<"!!Empty!!";
			}
            else
			{
                  return data[0];
			}
      }
 
      bool isEmpty() {
		  if(heapSize==0)
			return (true);
		  else
			  return(false);
          
      }
 
     
 
      ~BinaryMinHeap() {
            delete[] data;
      }




void BinaryMinHeap::siftUp(int nodeIndex) {        ////bottom_up to heapify
      int parentIndex; 
	  state_node tmp;
      if (nodeIndex != 0) {
            parentIndex = getParentIndex(nodeIndex);
			if (data[parentIndex].cost_f > data[nodeIndex].cost_f) {
                  tmp = data[parentIndex];
                  data[parentIndex] = data[nodeIndex];
                  data[nodeIndex] = tmp;
                  siftUp(parentIndex);
            }
      }
}
 
void BinaryMinHeap::insert(state_node newnode) {  //insert element to last and then bottom up heapify
      if (heapSize == arraySize)
            throw string("Heap's underlying storage is overflow");
      else {

            heapSize++;
			data[heapSize - 1]=newnode;
			
            siftUp(heapSize - 1);
      }
}





void BinaryMinHeap::siftDown(int nodeIndex) {          //Top-down to adjust

      int leftChildIndex, rightChildIndex, minIndex;
	  state_node tmp;
      leftChildIndex = getLeftChildIndex(nodeIndex);
      rightChildIndex = getRightChildIndex(nodeIndex);
      if (rightChildIndex >= heapSize) {
            if (leftChildIndex >= heapSize)
                  return;
            else
                  minIndex = leftChildIndex;
      } 

	  else {
		  if (data[leftChildIndex].cost_f <= data[rightChildIndex].cost_f)
                  minIndex = leftChildIndex;
            else
                  minIndex = rightChildIndex;
      }
	  if (data[nodeIndex].cost_f > data[minIndex].cost_f) {
            tmp = data[minIndex];
            data[minIndex] = data[nodeIndex];
            data[nodeIndex] = tmp;
            siftDown(minIndex);
      }
}
 
void BinaryMinHeap::removeMin() {    //remove min and then the last element instead first , do heapify

      if (isEmpty()==true)
	  {
		    
            cout<<"!!Empty!!"<<endl;
	  }
      else {
            data[0] = data[heapSize - 1];
            heapSize--;
            if (heapSize > 0)
                  siftDown(0);
      }
}

void BinaryMinHeap::refresh(state_node newdata) {          //decide input data action whether it is repeat,if repeat smaller will keep and covered
	
	int theSame=0;
	int thesameIndex=0;
	for(int i=0;i<heapSize;i++)
	{
		if(newdata.stateRps.compare(data[i].stateRps)==0)
		{
			if(newdata.cost_f < data[i].cost_f)
			{
				thesameIndex=i;
				theSame=1;
			}
			else
			{
				theSame=2;
			}
		}
	}

	if(theSame==0)
	{
		insert(newdata);
	}
	else if(theSame==1)
	{
		data[thesameIndex]=newdata;
		siftUp(thesameIndex);
		
	}
	else
	{}


}
};


/*           Function            */

/*Possible Algorithm*/



  //Priority queue

int manh_distance(string state);



int main(void)
{
	
	state_node *myNode,*newNode,*final;

	int nextStateCount;

	string goal="0123456789ABCDEF";
	string nextState[4];
	char nextAction[5];
	//int u;
	//final=NULL;
	

	myNode=new state_node;
	final=new state_node;

	char *iniState=new char[17];
	
	cout<<"Initial state: ";
		cin>>iniState;
	

	while(!cin.eof())
	{

		BinaryMinHeap pqueue(10000);

		

		myNode->stateRps=iniState;

		char temp[17];
		strcpy(temp,myNode->stateRps.c_str());

		if(myNode->stateRps=="0123456789ABCDEF")
		{
			cout<<"It is the goal state."<<endl;

		}
		
		else if(isSolvable(temp)==false)
		{
			cout<<"No Solution!!!!"<<endl;
			
		}
		else
		{
			myNode->cost_g=0;
			myNode->cost_h = manh_distance(myNode->stateRps);
			myNode->cost_f= myNode->cost_g + myNode->cost_h;

			myNode->parent=NULL;
			myNode->action=' ';
			pqueue.refresh(*myNode);

			delete myNode;

			while(pqueue.isEmpty()!=true)
			{


				

				myNode=new state_node;

				*myNode=pqueue.getMinimum();

				pqueue.removeMin();
				
				
				if(myNode->stateRps==goal)
				{
					final=myNode;
					break;
				}
				

				nextStateCount=Successor_Function(*myNode,nextState,nextAction);
				
				int k=0;
				for(int i=0;i<nextStateCount;i++)
				{
					
					newNode =new state_node;

					newNode->stateRps=nextState[i];
					newNode->action=nextAction[i];
					newNode->cost_g=myNode->cost_g+1;
					newNode->cost_h=manh_distance(nextState[i]);
					newNode->cost_f=newNode->cost_g+newNode->cost_h;
					newNode->parent=myNode;
					
					
					
					pqueue.refresh(*newNode);

					delete newNode;
					

				}
				
				

				
				
			}

				

				if(final==NULL)
				{
					cout<<"No solution!!";
				}

				else
				{
					
					traverseNode(final);
					
					
					//cout<<(*final).parent->action<<endl;

				}
			
				
				


		}

		

		cout<<"Initial state: ";
		cin>>iniState;
		
	}


	


	system("pause");
	return(0);

}
void swap(int *a,int *b)
{
	int temp;

	temp=*a;

	*a=*b;

	*b=temp;
}


void change_To_num(int *matrix ,char input[])
{


	for(int i=0;i<16;i++)
	{
		switch(input[i])
		{	case 'A':
				matrix[i]=10;
				break;
			case 'B':
				matrix[i]=11;
				break;
			case 'C':
				matrix[i]=12;
				break;
			case 'D':
				matrix[i]=13;
				break;
			case 'E':
				matrix[i]=14;
				break;
			case 'F':
				matrix[i]=15;
				break;
			default:
				matrix[i]=input[i]-'0';
		}

	}




}


void change_To_Str(int matrix[4][4],char output[])
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			switch(matrix[i][j])
			{	
			case 10:
				output[i*4+j]='A';
				break;
			case 11:
				output[i*4+j]='B';
				break;
			case 12:
				output[i*4+j]='C';
				break;
			case 13:
				output[i*4+j]='D';
				break;
			case 14:
				output[i*4+j]='E';
				break;
			case 15:
				output[i*4+j]='F';
				break;
			default:
				output[i*4+j]=matrix[i][j] + '0';
				
			}
		}

	}

}


int getInvCount(char input[])
{
	int matrix[16];
	int matrixWithoutZero[15];
	int PairCount=0;
	int ZeroRow;

	for(int i=0;i<16;i++)
	{
		switch(input[i])
		{	case 'A':
				matrix[i]=10;
				break;
			case 'B':
				matrix[i]=11;
				break;
			case 'C':
				matrix[i]=12;
				break;
			case 'D':
				matrix[i]=13;
				break;
			case 'E':
				matrix[i]=14;
				break;
			case 'F':
				matrix[i]=15;
				break;
			default:
				matrix[i]=input[i]-'0';
		}

	}

	int k=0;
	for(int i=0;i<16;i++)
	{
		if(matrix[i]!=0)
		{
			matrixWithoutZero[k]=matrix[i];
			k++;
		}
		else
		{
			ZeroRow=i/4;
		}


	}




	for(int i=0;i<15;i++)
	{
		for(int j=i;j<15;j++)
		{
			if(matrixWithoutZero[i] > matrixWithoutZero[j])
			{
				PairCount++;
			}
		}
	}

	return(PairCount+ZeroRow);

	
}


bool isSolvable(char input[])
{
	return(getInvCount(input)%2==0);
}

void decide_can_go(int XIndex,int YIndex,bool b[4])
{
	if(XIndex==0)
		b[0]=false;
	else
		b[0]=true;


	if(XIndex==3)
		b[1]=false;
	else
		b[1]=true;


	if(YIndex==0)
		b[2]=false;
	else
		b[2]=true;


	if(YIndex==3)
		b[3]=false;
	else
		b[3]=true;


}

void fetch(char input[],char output[])
{

	for(int i=0;i<16;i++)
	{
		output[i]=input[i];

	}
	output[16]='\0';
	
}
int Successor_Function(state_node node,string nextState[4],char nextAction[5])
{

	


	int *matrix=new int[16];

	int XIndex,YIndex;

	bool b[4]={true,true,true,true};

	int PossibleMove=0;


	char *Umove=new char[17];
	char *Dmove=new char[17];
	char *Lmove=new char[17];
	char *Rmove=new char[17];

	vector<char> action;

	char **output=new char*[4];

	for(int i=0;i<4;i++){
		output[i] = new char[17];
	}
	
	char *newoutput=new char[17];

	char *state=new char[17];

	strcpy(state,node.stateRps.c_str());


	change_To_num(matrix ,state);
	

	
	int fourXmatrix[4][4]={{matrix[0],matrix[1],matrix[2],matrix[3]},
						   {matrix[4],matrix[5],matrix[6],matrix[7]},
						   {matrix[8],matrix[9],matrix[10],matrix[11]},
						   {matrix[12],matrix[13],matrix[14],matrix[15]}};

	int UfourXmatrix[4][4]={{matrix[0],matrix[1],matrix[2],matrix[3]},
						   {matrix[4],matrix[5],matrix[6],matrix[7]},
						   {matrix[8],matrix[9],matrix[10],matrix[11]},
						   {matrix[12],matrix[13],matrix[14],matrix[15]}};
	int DfourXmatrix[4][4]={{matrix[0],matrix[1],matrix[2],matrix[3]},
						   {matrix[4],matrix[5],matrix[6],matrix[7]},
						   {matrix[8],matrix[9],matrix[10],matrix[11]},
						   {matrix[12],matrix[13],matrix[14],matrix[15]}};
	int LfourXmatrix[4][4]={{matrix[0],matrix[1],matrix[2],matrix[3]},
						   {matrix[4],matrix[5],matrix[6],matrix[7]},
						   {matrix[8],matrix[9],matrix[10],matrix[11]},
						   {matrix[12],matrix[13],matrix[14],matrix[15]}};
	int RfourXmatrix[4][4]={{matrix[0],matrix[1],matrix[2],matrix[3]},
						   {matrix[4],matrix[5],matrix[6],matrix[7]},
						   {matrix[8],matrix[9],matrix[10],matrix[11]},
						   {matrix[12],matrix[13],matrix[14],matrix[15]}};

	 

		for(int x=0;x<4;x++)
		{
			for(int y=0;y<4;y++)
			{
				if(fourXmatrix[x][y]==0)
				{
					XIndex=x;
					YIndex=y;
				}
			}
		}

		

		decide_can_go(XIndex,YIndex,b);
		

		for(int i=0;i<4;i++)
		{
			if(b[i]==1)
			{
				PossibleMove++;
			}

		}

		int n=0;

		if(b[0])
		{
			swap(UfourXmatrix[XIndex][YIndex],UfourXmatrix[XIndex-1][YIndex]);
			change_To_Str(UfourXmatrix,output[0]);
			
			fetch(output[0],newoutput);
			nextAction[n]='u';
			nextState[n].assign(newoutput);
			n++;
		}
		

		if(b[1])
		{
			
			swap(DfourXmatrix[XIndex][YIndex],DfourXmatrix[XIndex+1][YIndex]);
			change_To_Str(DfourXmatrix,output[1]);
			
			fetch(output[1],newoutput);
			nextAction[n]='d';
			nextState[n].assign(newoutput);
			n++;
			
		}
		
		if(b[2])
		{
			swap(LfourXmatrix[XIndex][YIndex],LfourXmatrix[XIndex][YIndex-1]);
			change_To_Str(LfourXmatrix,output[2]);
			
			
			fetch(output[2],newoutput);
			nextAction[n]='l';
			nextState[n].assign(newoutput);
			n++;
			
		}
		
		if(b[3])
		{
			swap(RfourXmatrix[XIndex][YIndex],RfourXmatrix[XIndex][YIndex+1]);
			change_To_Str(RfourXmatrix,output[3]);
			
			fetch(output[3],newoutput);
			nextAction[n]='r';
			nextState[n].assign(newoutput);
			n++;
			
		}
		

		nextAction[4]='\0';

		
		/*

		cout<<PossibleMove<<endl;

		for(int i=0;i<4;i++)
		{

			if(nextAction[i]!='n')
			{
				cout<<nextAction[i]<<",";
				Print_str(output[i]);
			}

		}

		*/

		return(PossibleMove);


}


int manh_distance(string state)
{
	char *goalState = "0123456789ABCDEF";
	char *nowState =new char[17];

	int *goalMatrix=new int[16];

	int *nowMatrix=new int[16];

	MatrixIndex goalIndex[16];

	int Distance=0;

	strcpy(nowState,state.c_str());



	change_To_num(goalMatrix,goalState);
	change_To_num(nowMatrix,nowState);
	

	for(int i=0;i<4;i++)							//°O¿ý¤Gºû°}¦CªºINDEX //record all Index of 2 dimension matrix 
	{
		for(int j=0;j<4;j++)
		{
			goalIndex[goalMatrix[i*4+j]].XIndex=i;
			goalIndex[goalMatrix[i*4+j]].YIndex=j;

		}


	}

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			Distance += abs(goalIndex[nowMatrix[i*4+j]].XIndex - i	) + abs(goalIndex[nowMatrix[i*4+j]].YIndex - j	);
			


		}
	}


	return(Distance);


}


void traverseNode(state_node *node)
{
	state_node *p = node;

	

	if(p!=NULL)
	{
		
		traverseNode(p->parent);
		/*
		if(p->action=='u')
			cout <<"Move 0 to up"<<endl;
		else if(p->action=='d')
			cout <<"Move 0 to down"<<endl;
		else if(p->action=='l')
			cout <<"Move 0 to left"<<endl;
		else
		{
			cout <<"Move 0 to right"<<endl;
		}
		*/

		switch(p->action)
		{
		case 'u':
			cout <<"Move 0 to up"<<endl;
			break;
		case 'd':
			cout <<"Move 0 to down"<<endl;
			break;
		case 'l':
			cout <<"Move 0 to left"<<endl;
			break;
		case 'r':
			cout <<"Move 0 to right"<<endl;
			break;
		default:
			break;
		}

	}
	else
	{
		cout <<"Solution:"<<endl;
	}




	




	/*
	
	if ( p== NULL) {
        cout << "The list is empty" << endl;
        return;
    }
	cout << "LinkedList: ";

	while (p != NULL) { 
		
		if(p->action=='u')
			cout <<"Move to Up"<<endl;
		else if(p->action=='d')
			cout <<"Move to Down"<<endl;
		else if(p->action=='l')
			cout <<"Move to Left"<<endl;
		else if(p->action=='r')
			cout <<"Move to Right"<<endl;
		else
		{}

		p = p->parent;
    }


	
    cout << endl;
	
	*/
}

