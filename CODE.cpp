#include<fstream>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
using namespace std;

ofstream prcs;
int totalnodes;
int n;

// NODE STRUCTURE
struct node
{
       int number;
       bool visited;
       bool boundary;
       int boundary_case;
       struct edge *edges[4];
}**nodes=0;

// EDGE STRUCTURE
struct edge
{
       bool wall;
       struct node *to;
       struct node *from;
};

// STACK ELEMENT
struct stack
{
       struct node *item;
       struct stack *next;
}*top=0;


// PUSHES NODE TO STACK
void push(struct node *newnode)
{
     struct stack *t=new stack[sizeof(stack)];
     t->item=newnode;
     t->next=0;
     if(top==0)
     {
               top=t;
     }
     else
     {
         t->next=top;
         top=t;
     }
}

// POPS A NODE FROM STACK
struct node * pop()
{
       struct node *temp=top->item;
       top=top->next;
       return temp;
}
       
// ALLOCATING MEMORY FOR EVERY NODE
void create_nodes()
{
     for(int i=0;i<totalnodes;i++)
     {
             nodes[i]=new node[sizeof(node)];
             nodes[i]->number=i+1;
             for(int j=0;j<4;j++)
             {
                     nodes[i]->edges[j]=new edge[sizeof(edge)];
                     nodes[i]->edges[j]->wall=true;
                     nodes[i]->edges[j]->to=0;
             }
     }
}

// CREATES REQUIRED LINK BETWEEN ALL NODES
void create_links()
{
     int nodenumber;
     for(int i=0;i<totalnodes;i++)
     {
             nodenumber=i+1;
             nodes[i]->number=nodenumber;
             nodes[i]->visited=false;
             if( i==0)
             {
                 nodes[i]->boundary=true;
                 nodes[i]->boundary_case=4;
                 nodes[i]->edges[1]->from=nodes[i];
                 nodes[i]->edges[1]->to=nodes[i+1];
                 nodes[i]->edges[2]->from=nodes[i];
                 nodes[i]->edges[2]->to=nodes[i+n];
             }
             else if( i==n-1) 
             {
                  nodes[i]->boundary=true;
                  nodes[i]->boundary_case=5;
                  nodes[i]->edges[3]->from=nodes[i];
                  nodes[i]->edges[3]->to=nodes[i-1];
                  nodes[i]->edges[2]->from=nodes[i];
                  nodes[i]->edges[2]->to=nodes[i+n];
             }
             else if( i==(n*n)-1) 
             {
                  nodes[i]->boundary=true;
                  nodes[i]->boundary_case=6;
                  nodes[i]->edges[0]->from=nodes[i];
                  nodes[i]->edges[0]->to=nodes[i-n];
                  nodes[i]->edges[3]->from=nodes[i];
                  nodes[i]->edges[3]->to=nodes[i-1];
             }
             else if( i==n*(n-1)) 
             {
                  nodes[i]->boundary=true;
                  nodes[i]->boundary_case=7;
                  nodes[i]->edges[0]->from=nodes[i];
                  nodes[i]->edges[0]->to=nodes[i-n];
                  nodes[i]->edges[1]->from=nodes[i];
                  nodes[i]->edges[1]->to=nodes[i+1];
             }        
             else if( i<n) 
             {
                 nodes[i]->boundary=true;
                 nodes[i]->boundary_case=0;
                 nodes[i]->edges[1]->from=nodes[i];
                 nodes[i]->edges[1]->to=nodes[i+1];
                 nodes[i]->edges[2]->from=nodes[i];
                 nodes[i]->edges[2]->to=nodes[i+n];
                 nodes[i]->edges[3]->from=nodes[i];
                 nodes[i]->edges[3]->to=nodes[i-1];
             }
             else if( i%n==0)
             {
                  nodes[i]->boundary=true;
                  nodes[i]->boundary_case=3;
                  nodes[i]->edges[0]->from=nodes[i];
                  nodes[i]->edges[0]->to=nodes[i-n];
                  nodes[i]->edges[1]->from=nodes[i];
                  nodes[i]->edges[1]->to=nodes[i+1];
                  nodes[i]->edges[2]->from=nodes[i];
                  nodes[i]->edges[2]->to=nodes[i+n];
             }
             else if( i%n==n-1) 
             {
                  nodes[i]->boundary=true;
                  nodes[i]->boundary_case=1;
                  nodes[i]->edges[1]->wall=true;
                  nodes[i]->edges[0]->from=nodes[i];
                  nodes[i]->edges[0]->to=nodes[i-n];
                  nodes[i]->edges[2]->from=nodes[i];
                  nodes[i]->edges[2]->to=nodes[i+n];
                  nodes[i]->edges[3]->from=nodes[i];
                  nodes[i]->edges[3]->to=nodes[i-1];
             }
             else if( i%(n*(n-1)) >=1 && i%(n*(n-1)) <=n-1 ) 
             {
                  nodes[i]->boundary=true;
                  nodes[i]->boundary_case=2;
                  nodes[i]->edges[2]->wall=true;
                  nodes[i]->edges[0]->from=nodes[i];
                  nodes[i]->edges[0]->to=nodes[i-n];
                  nodes[i]->edges[1]->from=nodes[i];
                  nodes[i]->edges[1]->to=nodes[i+1];
                  nodes[i]->edges[3]->from=nodes[i];
                  nodes[i]->edges[3]->to=nodes[i-1];
             }
             else
             {
                 nodes[i]->boundary=false;
                 nodes[i]->boundary_case=99;
                 nodes[i]->edges[0]->from=nodes[i];
                 nodes[i]->edges[0]->to=nodes[i-n];
                 nodes[i]->edges[1]->from=nodes[i];
                 nodes[i]->edges[1]->to=nodes[i+1];
                 nodes[i]->edges[2]->from=nodes[i];
                 nodes[i]->edges[2]->to=nodes[i+n];
                 nodes[i]->edges[3]->from=nodes[i];
                 nodes[i]->edges[3]->to=nodes[i-1];
             }
     }
}

// CHECKS IF A DEADEND HAS BEEN REACHED
bool check_deadend(struct node *newnode)
{
     for(int i=0;i<4;i++)								//CHECKS IF THE 'TO' NODE IN EACH DIRECTION IS VISITED
     {
             if(newnode->edges[i]->to!=0)
             if(newnode->edges[i]->to->visited==false)
             {
                 return false;
             }
     }
     return true;
}


// CREATION OF THE MAZE
void create_maze()
{
     int c;
     bool deadend;
     srand (time(NULL));								//SETTING UP THE RANDOM FUNCTION
     struct node *exit=nodes[0],*curr,*next;			//SETTING THE START NODE AS THE END POINT
     int k=1;
     nodes[0]->visited=true;
     nodes[0]->edges[1]->wall=false;
     nodes[0]->edges[1]->to->edges[3]->wall=false;
     push(nodes[0]);									
     curr=nodes[0]->edges[k]->to;						//DFS STARTS FROM THE SECOND NODE
     while(curr!=exit)									
     {
                      curr->visited=true;
                      prcs<<"\ncurr="<<curr->number<<endl; 	//WRITE CURR NODE TO PROCESS.txt FILE
                      switch(curr->boundary_case)
                      {
                                                 case 0:							//NODES IN THE FIRST ROW
                                                      deadend=check_deadend(curr);	//CHECK IF A DEADEND HAS BEEN REACHED
                                                      if( !deadend)
                                                      {
                                                          c=0;						//kill switch variable
                                                          do
                                                          {
                                                            c++;
                                                            k=rand()%3+1;			//CHOSE A RANDOM EDGE BASED ON THE INDEX OF EDGES ARRAY
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                       struct node *next=new node[sizeof(node)];
                                                                       next->visited=true;
                                                            }
                                                            if(c>10)				//KILL SWITCH IN CASE k keeps GETTING AN EDGE THAT HAS BEEN VISITED
                                                            for(int i=0;i<4;i++)	//CHOOSES THE NEXT UNVISITED EDGE IN CLOCKWISE POSITION
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }
                                                          }while(next->visited==true);
                                                          push(curr);
                                                          curr->edges[k]->wall=false;	//REMOVE WALL BETWEEN CURRENT AND NEXT NODE 'TO'->'FROM'
                                                          if( k==1)						//REMOVE WALL BETWEEN NEXT NODE AND CURRENT NODE 'TO'<-'FROM'
                                                          {
                                                              curr->edges[k]->to->edges[3]->wall=false;

                                                              //WRITE TO PROCESS FILE THE CHOSEN EDGE INDEX, NEXT NODE NUMBER AND PRESENT NODE NUMBER

                                                              prcs<<"k="<<k<<endl;				
                                                              prcs<<"next="<<curr->edges[k]->to->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[3]->to->number<<endl;
                                                          }    
                                                          else if(k==2)
                                                          {
                                                               curr->edges[k]->to->edges[0]->wall=false;
                                                               prcs<<"k="<<k<<endl;
                                                               prcs<<"next="<<next->number<<endl;
                                                               prcs<<"from="<<curr->edges[k]->to->edges[0]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                              curr->edges[k]->to->edges[1]->wall=false;
                                                              prcs<<"k="<<k<<endl;
                                                              prcs<<"next="<<next->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[1]->to->number<<endl;
                                                           }
                                                           curr=next;
                                                      }
                                                      else
                                                      curr=pop();		//IF A DEADEND IS REACHED POP NODE FROM STACK
                                                      break;
                                                 case 1:								//NODES IN THE LAST COLUMN
                                                      deadend=check_deadend(curr);
                                                      if( !deadend)
                                                      {
                                                          c=0;
                                                          do
                                                          {
                                                              c++;
                                                            k=(rand()%4);
                                                            if(k==1) k=2;
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                       struct node *next=new node[sizeof(node)];
                                                                       next->visited=true;
                                                            }
                                                            if(c>10)
                                                            for(int i=0;i<4;i++)
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }
                                                          }while(next==0 || next->visited==true );
                                                          push(curr);
                                                          curr->edges[k]->wall=false;
                                                          if(k==0)
                                                          {
                                                                  curr->edges[k]->to->edges[2]->wall=false;
                                                                  prcs<<"k="<<k<<endl;
                                                                  prcs<<"next="<<next->number<<endl;
                                                                  prcs<<"from="<<curr->edges[k]->to->edges[2]->to->number<<endl;
                                                          }
                                                          else if(k==2)
                                                          {
                                                               curr->edges[k]->to->edges[0]->wall=false;
                                                               prcs<<"k="<<k<<endl;
                                                               prcs<<"next="<<next->number<<endl;
                                                               prcs<<"from="<<curr->edges[k]->to->edges[0]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                              curr->edges[k]->to->edges[1]->wall=false;
                                                              prcs<<"k="<<k<<endl;
                                                              prcs<<"next="<<next->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[1]->to->number<<endl;
                                                          }
                                                          curr=next;
                                                      }
                                                      else
                                                      curr=pop();
                                                      break;
                                                 case 2:									//NODES IN THE LAST ROW
                                                      deadend=check_deadend(curr);
                                                      if( !deadend)
                                                      {
                                                          c=0;
                                                          do
                                                          {
                                                              c++;
                                                            k=rand()%4;
                                                            if(k==2) k=1;
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                        struct node *next=new node[sizeof(node)];
                                                                       next->visited=true;
                                                            }
                                                            if(c>10)
                                                            for(int i=0;i<4;i++)
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }
                                                          }while(next->visited==true);
                                                          push(curr);
                                                          curr->edges[k]->wall=false;
                                                          if(k==0)
                                                          {
                                                                  curr->edges[k]->to->edges[2]->wall=false;
                                                                  prcs<<"k="<<k<<endl;
                                                                  prcs<<"next="<<next->number<<endl;
                                                                  prcs<<"from="<<curr->edges[k]->to->edges[2]->to->number<<endl;
                                                          }
                                                          else if(k==1)
                                                          {
                                                               curr->edges[k]->to->edges[3]->wall=false;
                                                               prcs<<"k="<<k<<endl;
                                                               prcs<<"next="<<next->number<<endl;
                                                               prcs<<"from="<<curr->edges[k]->to->edges[3]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                              curr->edges[k]->to->edges[1]->wall=false;
                                                              prcs<<"k="<<k<<endl;
                                                              prcs<<"next="<<next->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[1]->to->number<<endl;
                                                          }
                                                          curr=next;
                                                      }
                                                      else
                                                      curr=pop();
                                                      break;
                                                 case 3:									//NODES IN THE FIRST COLUMN
                                                      deadend=check_deadend(curr);
                                                      if( !deadend)
                                                      {
                                                          c=0;
                                                          do
                                                          {
                                                            c++;
                                                            k=rand()%3;
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                        struct node *next=new node[sizeof(node)];
                                                                       next->visited=true;
                                                            }
                                                            if(c>10)
                                                            for(int i=0;i<4;i++)
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }
                                                          }while(next->visited==true);
                                                          push(curr);
                                                          curr->edges[k]->wall=false;
                                                          if(k==0)
                                                          {
                                                                  curr->edges[k]->to->edges[2]->wall=false;
                                                                  prcs<<"k="<<k<<endl;
                                                                  prcs<<"next="<<next->number<<endl;
                                                                  prcs<<"from="<<curr->edges[k]->to->edges[2]->to->number<<endl;
                                                          }
                                                          else if(k==2)
                                                          {
                                                               curr->edges[k]->to->edges[0]->wall=false;
                                                               prcs<<"k="<<k<<endl;
                                                               prcs<<"next="<<next->number<<endl;
                                                               prcs<<"from="<<curr->edges[k]->to->edges[0]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                           curr->edges[k]->to->edges[3]->wall=false;
                                                           prcs<<"k="<<k<<endl;
                                                           prcs<<"next="<<next->number<<endl;
                                                           prcs<<"from="<<curr->edges[k]->to->edges[3]->to->number<<endl;
                                                          }
                                                          curr=next;
                                                      }
                                                      else
                                                      curr=pop();
                                                      break;
                                                 case 4:							//TOP LEFTMOST NODE
                                                      deadend=check_deadend(curr);
                                                      if( !deadend)
                                                      {
                                                          c=0;
                                                          do
                                                          {
                                                                   c++;
                                                            k=rand()%2+1;
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                        struct node *next=new node[sizeof(node)];
                                                                       next->visited=true;
                                                            }
                                                            if(c>10)
                                                            for(int i=0;i<4;i++)
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }
                                                          }while(next->visited==true);
                                                          push(curr);
                                                          curr->edges[k]->wall=false;
                                                          if(k==1)
                                                          {
                                                                  curr->edges[k]->to->edges[3]->wall=false;
                                                                  prcs<<"k="<<k<<endl;
                                                                  prcs<<"next="<<next->number<<endl;
                                                                  prcs<<"from="<<curr->edges[k]->to->edges[3]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                              curr->edges[k]->to->edges[0]->wall=false;
                                                              prcs<<"k="<<k<<endl;
                                                              prcs<<"next="<<next->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[0]->to->number<<endl;
                                                          }
                                                          curr=next;
                                                      }
                                                      else
                                                      curr=pop();
                                                      break;
                                                 case 5:						//TOP RIGHTMOST NODE
                                                      deadend=check_deadend(curr);
                                                      if( !deadend)
                                                      {
                                                          c=0;
                                                          do
                                                          {
                                                              c++;
                                                            k=rand()%2+2;
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                        struct node *next=new node[sizeof(node)];
                                                                       next->visited=true;
                                                            }
                                                            if(c>10)
                                                            for(int i=0;i<4;i++)
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }
                                                          }while(next->visited==true);
                                                          push(curr);
                                                          curr->edges[k]->wall=false;
                                                          if(k==3)
                                                          {
                                                                  curr->edges[k]->to->edges[1]->wall=false;
                                                                  prcs<<"k="<<k<<endl;
                                                                  prcs<<"next="<<next->number<<endl;
                                                                  prcs<<"from="<<curr->edges[k]->to->edges[1]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                              curr->edges[k]->to->edges[0]->wall=false;
                                                              prcs<<"k="<<k<<endl;
                                                              prcs<<"next="<<next->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[0]->to->number<<endl;
                                                          }
                                                          curr=next;
                                                      }
                                                      else
                                                      curr=pop();
                                                      break;
                                                 case 6:								//BOTTOM RIGHTMOST NODE
                                                      deadend=check_deadend(curr);
                                                      if( !deadend)
                                                      {
                                                          c=0;
                                                          do
                                                          {
                                                              c++;
                                                            k=rand()%2;
                                                            if(k==1) k=3;
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                        struct node *next=new node[sizeof(node)];
                                                                       next->visited=true;
                                                            }
                                                            if(c>10)
                                                            for(int i=0;i<4;i++)
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }
                                                          }while(next->visited==true);
                                                          push(curr);
                                                          curr->edges[k]->wall=false;
                                                          if(k==0)
                                                          {
                                                                  curr->edges[k]->to->edges[2]->wall=false;
                                                                  prcs<<"k="<<k<<endl;
                                                                  prcs<<"next="<<next->number<<endl;
                                                                  prcs<<"from="<<curr->edges[k]->to->edges[2]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                              curr->edges[k]->to->edges[1]->wall=false;
                                                              prcs<<"k="<<k<<endl;
                                                              prcs<<"next="<<next->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[1]->to->number<<endl;
                                                          }
                                                          curr=next;
                                                      }
                                                      else
                                                      curr=pop();
                                                      break;
                                                 case 7:								//BOTTOM LEFTMOST NODE
                                                      deadend=check_deadend(curr);
                                                      if( !deadend)
                                                      {
                                                          c=0;
                                                          do
                                                          {
                                                              c++;
                                                            k=rand()%2;
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                        struct node *next=new node[sizeof(node)];
                                                                       next->visited=true;
                                                            }
                                                            if(c>10)
                                                            for(int i=0;i<4;i++)
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }
                                                          }while(next->visited==true);
                                                          push(curr);
                                                          curr->edges[k]->wall=false;
                                                          if(k==0)
                                                          {
                                                                  curr->edges[k]->to->edges[2]->wall=false;
                                                                  prcs<<"k="<<k<<endl;
                                                                  prcs<<"next="<<next->number<<endl;
                                                                  prcs<<"from="<<curr->edges[k]->to->edges[2]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                              curr->edges[k]->to->edges[3]->wall=false;
                                                              prcs<<"k="<<k<<endl;
                                                              prcs<<"next="<<next->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[3]->to->number<<endl;
                                                          }
                                                          curr=next;
                                                      }
                                                      else
                                                      curr=pop();
                                                      break;
                                              default:										//INNER NODES
                                                      deadend=check_deadend(curr);
                                                      if( !deadend)
                                                      {
                                                          c=0;
                                                          do
                                                          {
                                                            c++;
                                                            k=rand()%4;
                                                            next=curr->edges[k]->to;
                                                            if(next==0)
                                                            {
                                                                        struct node *next=new node[sizeof(node)];
                                                                        next->visited=true;
                                                            }
                                                            if(c>10)
                                                            for(int i=0;i<4;i++)
                                                            {
                                                                    if( curr->edges[i]->to->visited==false)
                                                                    {
                                                                        next=curr->edges[i]->to;
                                                                        k=i;
                                                                        break;
                                                                    }
                                                            }                                                                                                          
                                                          }while(next->visited==true);   
                                                          
                                                          push(curr);
                                                          curr->edges[k]->wall=false;
                                                          if(k==0)
                                                          {
                                                                  curr->edges[k]->to->edges[2]->wall=false;
                                                                  prcs<<"k="<<k<<endl;
                                                                  prcs<<"next="<<next->number<<endl;
                                                                  prcs<<"from="<<curr->edges[k]->to->edges[2]->to->number<<endl;
                                                          }
                                                          else if(k==1)
                                                          {
                                                               curr->edges[k]->to->edges[3]->wall=false;
                                                               prcs<<"k="<<k<<endl;
                                                               prcs<<"next="<<next->number<<endl;
                                                               prcs<<"from="<<curr->edges[k]->to->edges[3]->to->number<<endl;
                                                          }
                                                          else if(k==2)
                                                          {
                                                               curr->edges[k]->to->edges[0]->wall=false;
                                                               prcs<<"k="<<k<<endl;
                                                               prcs<<"next="<<next->number<<endl;
                                                               prcs<<"from="<<curr->edges[k]->to->edges[0]->to->number<<endl;
                                                          }
                                                          else
                                                          {
                                                              curr->edges[k]->to->edges[1]->wall=false;
                                                              prcs<<"k="<<k<<endl;
                                                              prcs<<"next="<<next->number<<endl;
                                                              prcs<<"from="<<curr->edges[k]->to->edges[1]->to->number<<endl;
                                                          }
                                                          curr=next;
                                                      }
                                                      else
                                                      curr=pop();
                                                      break;
                      }
     }
}
       

//  GENERATES THE COMMAND.txt FILE FOR GNUPLOT
void generate_plot()
{
     ofstream file;
     file.open("commands.txt");						
     file<<"set xrange[0:"<<n+1<<"]\n";											
     file<<"set yrange[0:"<<n+1<<"]\n";
     file<<"set arrow from 1,2 to 1,"<<n<<" nohead\n";
     file<<"set arrow from 1,"<<n+1<<" to "<<n+1<<","<<n+1<<" nohead\n";
     file<<"set arrow from "<<n+1<<","<<n+1<<" to "<<n+1<<",1 nohead\n";
     file<<"set arrow from "<<n+1<<",1 to 1,1 nohead\n";
     int x=1,y=n;
     for( int i=0;i<totalnodes;i++)
     {
          if( nodes[i]->edges[1]->wall==true)
              file<<"set arrow from "<<x+1<<","<<y+1<<" to "<<x+1<<","<<y<<" nohead\n";
          if( nodes[i]->edges[2]->wall==true)
              file<<"set arrow from "<<x<<","<<y<<" to "<<x+1<<","<<y<<" nohead\n";
          x++;
          if( x%n==1)
          {
              x=1;
              y--;
          }
     }
     file<<"unset tics\n";
     file<<"unset border\n";
     file<<"set term jpeg size 1000,1000\n";
     file<<"set output \"maze.jpg\"\n";
     file<<"plot 0 lc -1 notitle\n";
     file<<"set output\n";
     file.close();
}
     
                                                 
int main()
{
    
    cout<<"Enter maze size : ";
    cin>>n;
    totalnodes=n*n;
       
    /*ALLOCATING MEMORY FOR POINTER TO ALL NODES*/
    nodes=new node*[totalnodes];
    
    /*ALLOCATE MEMORY TO EVERY NODE*/
    create_nodes();
    cout<<"NODES CREATED\n";
    
    /*FORM REQUIRED LINKS BETWEEN EVERY NODE*/
    create_links();
    cout<<"LINKS FORMED\n";
    
    prcs.open("process.txt");    
    /*CREATE THE MAZE*/
    create_maze();
    cout<<"MAZE CREATED\n";
    
    
    /*CREATES NODES FILE WHICH STORES EVERY NODE INFORMATION*/
    cout<<endl;
    ofstream file;
    file.open("nodes.txt");
    for( int i=0;i<totalnodes;i++)
    {    
         file<<endl<<"node "<<i+1<<endl;
         for( int j=0;j<4;j++)
         {
              if(nodes[i]->edges[j]->wall==true)
              file<<"wall "<<j<<endl;
         }
    }
    cout<<endl;
    cout<<"NODES.TXT MADE\n";
    
    /*GENERATE GNU PLOT FILE*/
    generate_plot();
    cout<<"COMMANDS MADE\n";
    
    // system("pause");
    return 0;
    
}


             
