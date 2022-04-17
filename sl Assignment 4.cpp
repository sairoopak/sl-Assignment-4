#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct nodea
{	
	char name[10];
	int address;
	int memory_alloc;
	struct nodea *next;
}node_type_alloc;

typedef struct nodef
{	
	int start_address;
	int end_address;
	int memory_alloc;
	struct nodef *next;
}node_type_free;

//making node of alloacted list
node_type_alloc* makingnode_alloc (char name[10],int add,int mem)
{	
	node_type_alloc* ptr;
	ptr = (node_type_alloc*)malloc(sizeof(node_type_alloc));
	strcpy(ptr->name,name);
	ptr->address = add;
	ptr->memory_alloc = mem;
	ptr->next = NULL;
	return ptr;
}

//making node of free list
node_type_free* makingnode_free (int start_add,int end_add,int mem)
{	
	node_type_free* ptr;
	ptr = (node_type_free*)malloc(sizeof(node_type_free));
	ptr->start_address = start_add;
	ptr->end_address = end_add;
	ptr->memory_alloc = mem;
	ptr->next = NULL;
	return ptr;
}

//insert at start of allocated list
node_type_alloc* insertatstart_alloc (node_type_alloc *lptr, int add, int mem, char* name)
{	
	node_type_alloc *ptr;
	ptr = makingnode_alloc(name,add,mem);
	ptr->next = lptr;
	lptr = ptr;
	return lptr;
}

//insert at start of free list
node_type_free* insertatstart_free (node_type_free *lptr, int start_add, int end_add, int mem)
{	
    node_type_free *ptr;
	ptr = makingnode_free(start_add,end_add,mem);
	ptr->next = lptr;
	lptr = ptr;
	return lptr;
}

//print func to print allocated list
void print_alloc (node_type_alloc *lptr)
{	
	printf("This is the allocated list:\n");
	printf("Name\taddr\tmemory\n");
	while(lptr != NULL)
	{	printf("%s\t",lptr->name);
		printf("%d\t",lptr->address);
		printf("%d\t",lptr->memory_alloc);
		lptr = lptr->next;
		printf("\n");
	}
}

//print func to print free list 
void printfr(node_type_free *lptr)
{	
	printf("This is the FreeList:\n");
	printf("start_addr\tend_addr\tmemory\n");
	while(lptr != NULL)
	{	printf("%d\t\t",lptr->start_address);
		printf("%d\t\t",lptr->end_address);
		printf("%d\t\t",lptr->memory_alloc);
		lptr = lptr->next;
		printf("\n");
	}
}
 
//basic merge function
node_type_free* merge(node_type_free *lptr1,node_type_free *lptr2)
{	
	node_type_free *result,*tail,*p,*q;
	p = lptr1;
	q = lptr2;
	if(lptr1 == NULL)
	{	result = lptr2;
	}
	else if(lptr2 == NULL)
	{	result = lptr1;
	}
	else
	{	if((lptr1->memory_alloc) < (lptr2->memory_alloc))
		{	result = lptr1;
			p = p->next;
		}
		else
		{	result = lptr2;
			q = q->next;
		}
		tail = result;
		while((p != NULL)&&(q != NULL))
		{	
			if((p->memory_alloc) < (q->memory_alloc))
			{	tail = tail->next = p;
				p = p->next;
			}
			else
			{	tail = tail->next = q;
				q = q->next;
			}
		}
		if(p != NULL)
		{	tail->next = p;
		}
		else
		{	tail->next = q;
		}
	}
	return result;
}

//divide function
node_type_free* divide(node_type_free *lptr)
{	
	node_type_free *slow,*fast,*result;
	slow = lptr;
	fast = lptr->next->next;
	while(fast != NULL)
	{	
		slow = slow->next;
		fast = fast->next;
		if(fast != NULL)
		{	fast = fast->next;
		}
	}
	result = slow->next;
	slow->next = NULL;
	return result;
}

//sort function
node_type_free* sort(node_type_free *lptr)
{	
	node_type_free *head,*temp;
	head = lptr;
	if((head != NULL) && (head->next != NULL))
	{	temp = divide(lptr);
		lptr = sort(lptr);
		temp = sort(temp);
		head = merge(lptr,temp);
	}
	return head;
}

//modify the free list
node_type_alloc* modifyf(node_type_free *lptr,node_type_alloc *aptr,int memory,char name[10])
{	
	node_type_free *ptr,*sptr,*prev;
	int start_address;
	int flag = 1;
	sptr = ptr = lptr;
	while((ptr != NULL)&&(flag == 1))
	{	
		if(ptr->memory_alloc >= memory)
		{	start_address = ptr->start_address;
			ptr->start_address = (ptr->start_address) + memory;
			ptr->memory_alloc = (ptr->memory_alloc) - (memory);	
			flag = 0;
			aptr = insertatstart_alloc(aptr,start_address,memory,name);
		}
		else 
		{	ptr = ptr->next;
		}
	}
	//if memory not available
	if(ptr == NULL)
	{ 	
		printf("\n Oops,Memory can not be allocated \n");
	}
	//removing duplicate nodes with same start and end addresses
	while(sptr->next != NULL)
	{	
		if(((sptr->next)->start_address) == ((sptr->next)->end_address))
		{	prev = sptr;
			sptr = sptr->next;
			prev->next = sptr->next;
			free(sptr);
		}
		else
		{	sptr = sptr->next;
		}
	}
	return aptr;
}

//free the memory 
node_type_alloc* modifyboth (node_type_alloc *aptr, node_type_free **fptr, char f[10])
{	
	node_type_alloc *p, *sptr, *prev, *nptr;
	node_type_free *q,*optr;
	int start_address, end_address, memory_alloc, found = 0;
	q = *fptr;
	p = aptr;
	prev = NULL;
	
	while((p != NULL) && (found == 0))
	{	
		if(!strcmp(p->name,f))
		{	
			start_address = p->address;
			end_address = (p->address) + (p->memory_alloc);
			memory_alloc = p->memory_alloc;
			found = 1;
			if(p == aptr)
			{	nptr = p;
				p = p->next;
				free(nptr);
				aptr = p;
			}	
			else
			{	prev->next = p->next;
				free(p);
			}
		}
		else
		{	prev = p;
			p = p->next;
		}
	}
	found = 0;
	while((q != NULL) && (found == 0))
	{	
		if(q->start_address == end_address)
		{	
			q->start_address = start_address;
			q->memory_alloc = (q->memory_alloc) + (memory_alloc);
			found = 1;
		}
		else if(q->end_address == start_address)
		{	
			q->end_address = end_address;
			q->memory_alloc = (q->memory_alloc) + (memory_alloc);
			found = 1;
		}
		q = q->next;
	}
	if(found == 0)
	{	
		*fptr = insertatstart_free(*fptr, start_address, end_address, memory_alloc);
	}
	return aptr;
}		
		
//main function 
int main()
{	
	node_type_alloc *aptr = NULL;
	node_type_free *fptr = NULL;
	int stop = 1,opt;
	char first[] = "xyz";
	char second[] = "abc";
	char third[] = "pqr";
	char fourth[] = "hat";
	char fifth[] = "wet";
	
	aptr = insertatstart_alloc(aptr,0,10,first);     //allocated memory before
	aptr = insertatstart_alloc(aptr,20,30,second);
	aptr = insertatstart_alloc(aptr,80,60,third);
	aptr = insertatstart_alloc(aptr,200,90,fourth);
	aptr = insertatstart_alloc(aptr,390,150,fifth);

	fptr = insertatstart_free(fptr,150,180,30);        // freed memory before 
	fptr = insertatstart_free(fptr,300,380,80);
	fptr = insertatstart_free(fptr,550,650,100);
	fptr = insertatstart_free(fptr,720,1000,280);
	
	while(stop == 1)
	{	
		printf("1.Allocation of memory \n");
		printf("2.Free up the memory \n");
		printf("3.Show the Allocated and Free memory \n");
		printf("4.Exit \n");
		printf("Select the option to perform : ");
		scanf("%d",&opt);
		if(opt == 1)
		{ 	
			int memory;
		    char name[10];
		    printf("Enter the size to allocate : ");
			scanf("%d",&memory);
			printf("Enter name of the process : ");
			scanf("%s",name);
			fptr = sort(fptr);
			aptr = modifyf(fptr,aptr,memory,name);
			print_alloc(aptr);
			printfr(fptr);
			printf("\n");
			printf("To continue press 1 or press 0 : ");
			scanf("%d",&stop);
		}
		else if(opt == 2)
		{	
			char free[10];
		   	print_alloc(aptr);
			printf("\n");
			printf("Which one you want to free : ");
			scanf("%s",free);
			aptr = modifyboth(aptr,&fptr,free);
			print_alloc(aptr);
			printfr(fptr);
			printf("\n");
			printf("To continue press 1 or press 0 : ");
			scanf("%d",&stop);
		}
		else if(opt == 3)
		{	
			print_alloc(aptr);
			printfr(fptr);
			printf("\n");
			printf("To continue press 1 or press 0 : ");
			scanf("%d",&stop);
		}
		else if(opt == 4)
		{	stop = 0;
		}
		else
		{	
			printf("Enter the valid input !! ");
			stop = 0;
		}		
	}
	return 0;
}
