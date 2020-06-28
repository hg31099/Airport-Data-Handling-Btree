#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define ts 60
#define sw_size 26
#define MAX 5
//------------------------------------------------------------------
typedef enum{false,true} bool;

//passenger structure
typedef struct passenger_tag
{
    char name[50];
    int age;
    int luggage_num;
    int luggage_wt;
    int fine;
    int sno;
    struct passenger_tag *next;
}passenger;
//ticket structure
//node
typedef struct ticket_tag
{
    char ticket_id[6];
    passenger *p;
    int pass_num;
}tnode;
//tree
typedef struct ticket_tree
{
	tnode keys[MAX];
	struct ticket_tree *a[MAX+1];
	int c;
	bool isleaf;
}ttree;

//flight structure
//node
typedef struct flight_tag
{
    char fid[4];
    ttree *troot;
    int s[ts];
}fnode;
//tree
typedef struct flight_tree
{
	fnode keys[MAX];
	struct flight_tree* a[MAX+1];
	int c;
	bool isleaf;

}ftree;

typedef struct service_window
{
    char sw_name[10];
    ftree *froot;
    struct service_window *next;
}swnode;
//hash table for O(1) complexity
swnode *swtable[sw_size];
//-------------------------------------------------------------------
//hashing function returns index


int hash_swtable(char c)
{
    int index;
    index=(c-'a')%sw_size;
    return index;
}

//intialisation functions
void intialise_swtable(swnode *swtable[])
{
    int i=0;
    for(i=0;i<sw_size;i++)
    {
        swtable[i]=NULL;
    }
}

swnode * intialise_swnode(swnode *swptr)
{
    swptr->sw_name[0]='\0';
    swptr->froot=NULL;
    swptr->next=NULL;
    return swptr;
}

ftree * intialise_ftree(ftree *node)
{
	node->c=0;
	node->isleaf=true;
	int i=0,j=0;
	for(i=0;i<MAX;i++)
	{
		node->a[i]=NULL;
		(node->keys[i]).fid[0]='\0';
		(node->keys[i]).troot=NULL;
		for(j=0;j<ts;j++)
		{
		(node->keys[i]).s[j]=0;
		}
	}
	node->a[i]=NULL;
	return node;	
}

passenger * intialise_passenger(passenger *p)
{
    p->name[0]='\0';
    p->age=0;
    p->luggage_num=0;
    p->luggage_wt=0;
    p->fine=0;
    p->sno=0;
    p->next=NULL;
    return p;
}


ttree * intialise_ttree(ttree *node)
{
	node->c=0;
	node->isleaf=true;
	int i=0;
	for(i=0;i<MAX;i++)
	{
		node->a[i]=NULL;
		(node->keys[i]).ticket_id[0]='\0';
		(node->keys[i]).p=NULL;
		(node->keys[i]).pass_num=0;
	}
	node->a[i]=NULL;
	return node;
}
//______________________________________________________________________________________________________________________________________________



//creates and intialises service window------------------------------------
ftree* split(ftree *root,ftree *ptr);
ftree* add_flight(ftree *fptr,char cid,int id);
ttree* add_ticket(ttree *root,tnode t);
//-------------------------------------------------------------------------
swnode * create_swnode()//checked
{
    swnode *nptr;
    nptr=(swnode *)malloc(sizeof(swnode));
    nptr=intialise_swnode(nptr);
    return nptr;
}


void display(ftree *ptr, int blanks)//checked
{
  if (ptr)
    {
      int i;
      for (i = 1; i <= blanks; i++)
	  printf (" ");
      for (i = 0; i < (ptr->c); i++)
      printf ("%s ", ptr->keys[i].fid);
      printf ("\n");
      for (i = 0; i <= (ptr->c); i++)
      display (ptr->a[i], blanks + 10);
    }				/*End of if */
}				/*End of display() */


void addFlightServiceWindow(swnode* swtable[])//checked
{
    swnode *ptr,*nptr;
    nptr=create_swnode();
    int num=0,i=0;
    printf("Enter service window name:\n");
    scanf("%s",nptr->sw_name);
    int index=0;
    index=hash_swtable(nptr->sw_name[0]);
    if(swtable[index]==NULL)
    {
        swtable[index]=nptr;
    }
    else
    {
        ptr=swtable[index];

        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=nptr;
    }
    printf("\n1)to take input from file\n2)to give input\n");
    
    int in_ch,id=0;
    scanf("%d",&in_ch);
    if(in_ch==1)
    {
    	    FILE *f;
  			f = fopen("flight.txt","r");
  			while(fscanf(f," %d",&id)!=-1)
  			{
        		nptr->froot=add_flight(nptr->froot,nptr->sw_name[0],id);  				
  			}
  			
    }
    if(in_ch==2)
    {
    	printf("Enter number of flights to this window:\n");
    	scanf("%d",&num);
    	for(i=0;i<num;i++)
    	{
    	    printf("Enter flight id:(in specified format)\n");  //in format first letter of service window and then flight number
    		scanf("%d",&id);
        	nptr->froot=add_flight(nptr->froot,nptr->sw_name[0],id);
    	
    	}
    } 
    printf("Successfully added service window for %s \n",nptr->sw_name);

}

ftree * makeflightnode()//checked
{
	ftree *fnptr;
	fnptr=(ftree *)malloc(sizeof(ftree));
	fnptr=intialise_ftree(fnptr);
	return fnptr;

}



int fid_to_key(char a[])//checked
{
	int i=0;
	i=(a[1]-'0')*10+(a[2]-'0');
	return i;
}

void add_array(fnode keys[],char cid,int id,int c)//checked
{
	int i=c-1;
	int j;
	while(i>=0 && fid_to_key(keys[i].fid)>id)
	{
		keys[i+1]=keys[i];
		i--;
	}
	keys[i+1].fid[0]=cid;
	keys[i+1].fid[1]=(id/10)+'0';
	keys[i+1].fid[2]=(id%10)+'0';
	keys[i+1].fid[3]='\0';
	keys[i+1].troot=NULL;
	for(j=0;j<ts;j++)
	{
		keys[i+1].s[j]=0;
	}
}


ftree* search_pos_flight(ftree *root,char cid,int id,int *d)//checked
{
	
	ftree *ptr;
	ptr=root;
	int cnt,i,flag=0;
	ftree *retval;
	*d=1;
	if(ptr->isleaf==true)
	{
		cnt=ptr->c;
		for(i=0;i<cnt && flag==0;i++)
		{
			if(fid_to_key((ptr->keys[i]).fid)>=id)
			{

				flag=1;
				if(fid_to_key((ptr->keys[i]).fid)==id)
				{
					*d=0;
					printf("element found at leaf\n");
				}
			}
		}
		retval=ptr;
	}
	if(ptr->isleaf==false)
	{
		cnt=ptr->c;
		for(i=0;i<cnt && flag==0;i++)
		{
			if(fid_to_key((ptr->keys[i]).fid)>=id)
			{
				flag=1;
				if(fid_to_key((ptr->keys[i]).fid)==id)
				{
					*d=0;
					retval=ptr;
					printf("element found \n");
				}			
			}
		}
		if(flag==1) i--;
		if(*d==1)
		{
			retval=search_pos_flight(ptr->a[i],cid,id,d);
		}
		
	}
	return retval;
	

}

void add_array_ins(fnode keys[],fnode ft,int c)//checked
{
	int i=c-1;
	int j;
	while(i>=0 && fid_to_key(keys[i].fid)>fid_to_key(ft.fid))
	{
		keys[i+1]=keys[i];
		i--;
	}
	keys[i+1]=ft;
}


ftree* insert(ftree *root,fnode ft)//checked
{
	ftree *ptr=root,*parent_ptr;
	ftree *ptr3;
	int i;
	int d=1;
	if(root==NULL)
	{
		root=makeflightnode();
		root->keys[root->c]=ft;
		(root->c)++;
	}
	else
	{
		ptr=search_pos_flight(root,ft.fid[0],fid_to_key(ft.fid),&d);
		if(d==1)
		{
			if(ptr->c<4)
			{
				add_array_ins(ptr->keys,ft,ptr->c);
				ptr->c++;
			}///dont split
			else//split
			{
				add_array_ins(ptr->keys,ft,ptr->c);
				ptr->c++;
				root=split(root,ptr);		
			}
		}
	}
	return root;
}
ftree* findparent(ftree *root,ftree *ptr)//checked
{
	int flag=0,i=0;
	ftree *retval=NULL;
	if(root->isleaf==false)
	{
		for(i=0 ; i<root->c+1 && flag==0; )
		{
	
			if(root->a[i]!=ptr)
			{
				retval=findparent(root->a[i],ptr);
				i++;
			
			}
			else
			{
				flag=1;
				retval=root;
			}
		}
	}
	return retval;
}
ftree* split(ftree *root,ftree *ptr)//checked
{
	int i,j,flag,z,p;
	ftree *ptr2,*pptr,*ptr3,*retval;
	if(ptr==root)
	{
		root=insert(NULL,ptr->keys[MAX/2]);
		ptr2=NULL;
			for(j=0;j<(MAX/2);j++)
			{
				ptr2=insert(ptr2,ptr->keys[j]);
			}
			root->a[0]=ptr2;
			root->isleaf=false;
			if(ptr->isleaf==false)
			{
				ptr2->isleaf=false;	
			}	
			ptr2=NULL;
			j++;
			for(;j<MAX;j++)
			{
				ptr2=insert(ptr2,ptr->keys[j]);
			}
			if(ptr->isleaf==false)
			{
				ptr2->isleaf=false;	
			}
			root->a[1]=ptr2;
			retval=root;
			
		if(ptr->isleaf==false)
		{
			j=0;
			ptr2=root->a[0];
			for(i=0;i<=(MAX/2);i++)	
			{
				ptr2->a[i]=ptr->a[j++];
			}
			ptr2=root->a[1];
			j=0;
			
			for(;i<=MAX;i++)
			{
				ptr2->a[j++]=ptr->a[i];
			}
		}
		free(ptr);
	}
	else
	{
		pptr=findparent(root,ptr);		
		add_array_ins(pptr->keys,ptr->keys[MAX/2],pptr->c);
		pptr->c++;
		flag=0; 
		for(i=0;i<pptr->c && flag==0;)
		{
			if(fid_to_key(pptr->keys[i].fid)==fid_to_key(ptr->keys[MAX/2].fid))
			{
				flag=1;
			}
			else
			{
				i++;
			}
		}
		
		ptr2=NULL;
			for(j=0;j<(MAX/2);j++)
			{
				ptr2=insert(ptr2,ptr->keys[j]);
			}
			pptr->a[i]=ptr2;
			pptr->isleaf=false;
			z=0;
			if(ptr->isleaf==false)
			{
				for(p=0;p<=ptr2->c;p++)
				{
					ptr2->a[p]=ptr->a[z++];
				}
			}
			
		ptr2=NULL;
		j++;
			for(;j<MAX;j++)
			{
				ptr2=insert(ptr2,ptr->keys[j]);
			}
			if(ptr->isleaf==false)
			{
				for(p=0;p<=ptr2->c;p++)
				{
					ptr2->a[p]=ptr->a[z++];
				}
			}
			int k,l;
			l=pptr->c-1;
			k=l+1;
			while(l!=i)
			{
				pptr->a[k]=pptr->a[l];
				k--;
				l--;
			}
			pptr->a[k]=ptr2;

		if(pptr->c<5)
		{
			retval=root;
		}
		else
		{
			retval=split(root,pptr);
		}
		free(ptr);
	}
	return retval;
}

ftree* add_flight(ftree *fptr,char cid,int id)//checked
{
	ftree *fnptr,*ptr;
	int d=1;

	if(fptr==NULL)
	{
		fptr=makeflightnode();
		(fptr->keys[fptr->c]).fid[0]=cid;
		(fptr->keys[fptr->c]).fid[1]=(id/10)+'0';
		(fptr->keys[fptr->c]).fid[2]=(id%10)+'0';
		(fptr->keys[fptr->c]).fid[3]='\0';
		(fptr->c)++;
	}
	else
	{
		ptr=search_pos_flight(fptr,cid,id,&d);
		if(d==1)
		{
			if(ptr->c<4)
			{
				add_array(ptr->keys,cid,id,ptr->c);
				ptr->c++;
			}///dont split
			else//split
			{
				add_array(ptr->keys,cid,id,ptr->c);
				ptr->c++;
				fptr=split(fptr,ptr);		
			}
		}
	}
	return fptr;
}
//------------------------------------------------------------------------------------------


void print_av_seats(fnode f)
{
    printf("\n");
 //   printf("W M A   A M W");
    int i=0,j=0,count=0;
    for(i=0;i<(ts);i++)
    {
       if(f.s[i]==0)
       {
           printf("%d ",(i+1));
           if((i+1)%3==1) printf(" W \n");
           if((i+1)%3==2) printf(" M \n");
           if((i+1)%3==0) printf(" A \n");
           count++;
       }
    }
    printf("Total available seats are %d \n",count);
}

void print_seat_chart(fnode f)
{
	printf("\n");
	int i=0,j=0;
	printf("W  M  A \tA  M  W \n");
	for(i=0;i<(ts)/6;i++)
	{
		for(j=0;j<3;j++)
		{
			if(f.s[3*i+j]==0)
			{
				if((3*i+j)<9) printf("0%d ",(3*i+j+1));
				else  printf("%d ",(3*i+j+1));
			}
			else printf("   ");
		}
		printf("\t");
		for(j=0;j<3;j++)
		{
			if(f.s[3*(i+11)-j-1]==0)
			{
				printf("%d ",(3*(i+11)-j));
			}
			else printf("   ");
		}
		printf("\n");
	}
}
 


void DisplayAvailableSeats(char f_id[])
{
  int index,found=0,i,count=0;
  swnode *ptr;
  ftree *fptr;
  int d=1;
  index=hash_swtable(f_id[0]);
  if(swtable[index]==NULL)   //search for that index in hash table
  {
      printf("No such flight exists \n");
  }
  else   //if found then search for that flight id
  {
      ptr=swtable[index];
      fptr=NULL;
      while(d==1 && ptr!=NULL)
      {
			fptr=search_pos_flight(ptr->froot,f_id[0],fid_to_key(f_id),&d);
			
			if(d==1)
			{
				ptr=ptr->next;
			}
      }
      if(d==0)
      {
      	i=0;
      	while(fid_to_key(fptr->keys[i].fid)!=fid_to_key(f_id))
      	{
      		i++;
      	}
           //  print_av_seats(fptr->keys[i]);
             print_seat_chart(fptr->keys[i]);
      }
		
      if(d==1)
      {
          printf("No such flight exists \n");
      }

  }


}


int num_av_seats(fnode f)
{
     int i=0,j=0,count=0;
    for(i=0;i<(ts);i++)
    {
       if(f.s[i]==0)
       {
           count++;
       }
    }
    return count;
}

//-------------------------------------------------------------------------------
void allocateBoardingPass(char f_id[])
{
    int pnum=0;
    int pos;
    int d=1;
    int index,found=0,i,count=0;
      swnode *ptr;
      ftree *fptr;
      index=hash_swtable(f_id[0]);    //using flight id ,searching for flight
      if(swtable[index]==NULL)
      {
          printf("No such flight exists \n");
      }
	  else   //if found then search for that flight id
	  {
		  ptr=swtable[index];
		  fptr=NULL;
		  while(d==1 && ptr!=NULL)
		  {
				fptr=search_pos_flight(ptr->froot,f_id[0],fid_to_key(f_id),&d);
				
				if(d==1)
				{
					ptr=ptr->next;
				}
		  }
		  if(d==0)
		  {
		  	i=0;
		  	while(fid_to_key(fptr->keys[i].fid)!=fid_to_key(f_id))
		  	{
		  		i++;
		  	}
		  	count=num_av_seats(fptr->keys[i]);
		  	fnode fins;
		  	fins=fptr->keys[i];
		  	pos=i;
		       //  printf("%d \n",count);
		  }
			
		  if(d==1)
		  {
		      printf("No such flight exists \n");
		  }

	  }
  i=0;  //tail pointer used to get O(1) complexity in inserting at end
  passenger *pptr,*tptr,*nptr;
  passenger *ahptr,*whptr,*mhptr,*atptr,*wtptr,*mtptr,*wtravptr,*mtravptr,*atravptr;
  wtravptr=mtravptr=atravptr=ahptr=whptr=mhptr=atptr=wtptr=mtptr=NULL;

  char ch;

  ttree *tickptr;
  tickptr=NULL;
  tnode ticket;
  int win_seat=0;
  int ais_seat=0,fine=0,mid_seat=0;
  //if flight is found then take the input from the passengers and store it in linked lists
  //3 different linked lists are created for each seat preferances
  int choice=1;
  printf("1)to give input \n2)to take default input");
  scanf("%d",&choice);
  if(d==0)
  {
      if(choice==1)
    {
    	    printf("Enter number of passengers \n");
          scanf("%d",&pnum);
    	ticket.pass_num=pnum;
       for(i=0;i<pnum;i++)
      {
		

          pptr=(passenger *)malloc(sizeof(passenger));
          pptr=intialise_passenger(pptr);
          printf("Enter passengers details:\n");
          printf("name: \n");
          scanf("%s",pptr->name);
          printf("age: \n");
          scanf("%d",&pptr->age);
          printf("luggage nos.: \n");
          scanf("%d",&pptr->luggage_num);
          printf("luggage weight: \n");
          scanf("%d",&pptr->luggage_wt);
          fine=((pptr->luggage_wt)-15)*100;
          if(fine<0) fine=0;
          FILE *f2;
          f2=fopen("airport.txt","a");
          fflush(stdin);
          fprintf(f2,"%s\n%d\n%d\n%d\n",pptr->name,pptr->age,pptr->luggage_num,pptr->luggage_wt);
          fclose(f2);
          if((pptr->age)<=15)
          {
              win_seat++;
               nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
             strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(whptr==NULL)
                {
                    whptr=nptr;
                    wtptr=nptr;
                }
                else
                {
                    wtptr->next=nptr;
                    wtptr=nptr;
                }
          }
          if((pptr->age)>=60)
          {
              ais_seat++;
              nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
              strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(ahptr==NULL)
                {
                    ahptr=nptr;
                    atptr=nptr;
                }
                else
                {
                    atptr->next=nptr;
                    atptr=nptr;
                }
          }
        if((pptr->age)<60 &&(pptr->age)>15)
        {
            mid_seat++;
            nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
              strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(mhptr==NULL)
                {
                    mhptr=nptr;
                    mtptr=nptr;
                }
                else
                {
                    mtptr->next=nptr;
                    mtptr=nptr;
                }

        }

      }
  }

      else
       {
       	char str[100];
  int d1=0,d2=0,d3=0;
       		FILE *f;
  			f = fopen("airport.txt","r");
  		//	ch=fgetc(f);
  		i=0;
  			while(fscanf(f,"%s %d %d %d",str,&d1,&d2,&d3)!=-1)
  			{
		///	printf("%d",d1);
		pptr=(passenger *)malloc(sizeof(passenger));
          pptr=intialise_passenger(pptr);
			  strcpy(pptr->name,str);
			  	pptr->age=d1;
	  			pptr->luggage_num=d2;
	  			pptr->luggage_wt=d3;
	  			fine=((pptr->luggage_wt)-15)*100;
	          	if(fine<0) fine=0;
	          	if((pptr->age)<=15)
    	      {
              win_seat++;
               nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
             strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(whptr==NULL)
                {
                    whptr=nptr;
                    wtptr=nptr;
                }
                else
                {
                    wtptr->next=nptr;
                    wtptr=nptr;
                }
          }
          if((pptr->age)>=60)
          {
              ais_seat++;
              nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
              strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(ahptr==NULL)
                {
                    ahptr=nptr;
                    atptr=nptr;
                }
                else
                {
                    atptr->next=nptr;
                    atptr=nptr;
                }
          }
        if((pptr->age)<60 &&(pptr->age)>15)
        {
            mid_seat++;
            nptr=(passenger *)malloc(sizeof(passenger));
               nptr=intialise_passenger(nptr);
              strcpy(nptr->name,pptr->name);
              nptr->age=pptr->age;
              nptr->luggage_num=pptr->luggage_num;
              nptr->luggage_wt=pptr->luggage_wt;
              nptr->fine=fine;
                if(mhptr==NULL)
                {
                    mhptr=nptr;
                    mtptr=nptr;
                }
                else
                {
                    mtptr->next=nptr;
                    mtptr=nptr;
                }

        }

	        }
	      fclose(f);


}
      pnum=win_seat+ais_seat+mid_seat;
      ticket.pass_num=pnum;
    // we have got count of each sseat preferances and corrosponding linke lists
    //seats are alloted accord to prefernances for child and aged people
    //and nearby seats are given to passengers with same ticket id
    wtravptr=whptr;
    mtravptr=mhptr;
    atravptr=ahptr;
    i=0;
    int seat_num=1;
    for(i=0;i<ts && pnum>0;i++)
    {
        seat_num=i+1;
        if(fptr->keys[pos].s[i]==0)
        {
            if(seat_num%3==1)
            {
              if(win_seat>0)
              {
                wtravptr->sno=seat_num;
                win_seat--;
                pnum--;
                fptr->keys[pos].s[i]=1;
                wtravptr=wtravptr->next;
              }
              else
              {
                 if(mid_seat>0)
                 {
                     mtravptr->sno=seat_num;
                     mid_seat--;
                     pnum--;
                     fptr->keys[pos].s[i]=1;
                     mtravptr=mtravptr->next;
                 }
              }
            }
            if(seat_num%3==2)
            {
               if(mid_seat>0)
                 {
                     mtravptr->sno=seat_num;
                     mid_seat--;
                     pnum--;
                     fptr->keys[pos].s[i]=1;
                     mtravptr=mtravptr->next;
                 }

            }
            if(seat_num%3==0)
            {
                if(ais_seat>0)
              {
                atravptr->sno=seat_num;
                ais_seat--;
                pnum--;
                fptr->keys[pos].s[i]=1;
                atravptr=atravptr->next;
              }
              else
              {
                 if(mid_seat>0)
                 {
                     mtravptr->sno=seat_num;
                     mid_seat--;
                     pnum--;
                     fptr->keys[pos].s[i]=1;
                     mtravptr=mtravptr->next;
                 }
              }

            }
        }


    }
    //if prefered  seats are not available,then we allocate them other nearby seats
    if(pnum>0)
    {
        for(i=0;i<ts && pnum>0;i++)
        {
            seat_num=i+1;
            if(fptr->keys[pos].s[i]==0)
            {
                if(win_seat>0)
              {
                wtravptr->sno=seat_num;
                win_seat--;
                pnum--;
               fptr->keys[pos].s[i]=1;
                wtravptr=wtravptr->next;
              }
              else
              {
                  if(mid_seat>0)
                 {
                     mtravptr->sno=seat_num;
                     mid_seat--;
                     pnum--;
                     fptr->keys[pos].s[i]=1;
                     mtravptr=mtravptr->next;
                 }
                 else if(ais_seat>0)
                 {
                    atravptr->sno=seat_num;
                    ais_seat--;
                    pnum--;
                    fptr->keys[pos].s[i]=1;
                    atravptr=atravptr->next;
                 }
              }

            }
        }
           printf("prefered seats can't be alloted.Inconvenience is regreted \n");
     }
     passenger *mainptr;
    //concatenation of 3 linked lists
    //3 linked lists we created contains seat number and we need to concatenate it
    if(ahptr==NULL && mhptr==NULL) mainptr=whptr;
    if(whptr==NULL && mhptr==NULL) mainptr=ahptr;
    if(ahptr==NULL && whptr==NULL) mainptr=mhptr;

    if(whptr==NULL && ahptr!=NULL && mhptr!=NULL)
    {
        mainptr=ahptr;
        atptr->next=mhptr;
    }
    else
    {
        if(mhptr==NULL && ahptr!=NULL && whptr!=NULL)
        {
            mainptr=ahptr;
            atptr->next=whptr;
        }
        if(ahptr==NULL && mhptr!=NULL && whptr!=NULL)
        {
            mainptr=whptr;
            wtptr->next=mhptr;
        }

    }

    if(mhptr!=NULL && ahptr!=NULL && whptr!=NULL)
    {
        mainptr=whptr;
        wtptr->next=mhptr;
        mtptr->next=ahptr;
    }

//----------
	
    strcpy(ticket.ticket_id,f_id); //ticket id has first 3 char as flight-id and next 2 char as seat_num

        ticket.ticket_id[3]=(mainptr->sno)/10+'0';
        ticket.ticket_id[4]=(mainptr->sno)%10+'0';
        ticket.p=mainptr;
        fptr->keys[pos].troot=add_ticket(fptr->keys[pos].troot,ticket);
        

    printf("Passengers welcome aboard\n");
    printf("please note %s ticket id for future reference \n",ticket.ticket_id); //note this ticket id

    }//end of found(if)
      else printf("required number of seats are not available \nInconvenience caused is deeply regreted \n");




}//end of function


ttree * maketicketnode()
{
	ttree *tnptr;
	tnptr=(ttree *)malloc(sizeof(ttree));
	tnptr=intialise_ttree(tnptr);
	return tnptr;

}

int tid_to_key(char id[])
{
	int i;
	i=(id[1]-'0')*1000+(id[2]-'0')*100+(id[3]-'0')*10+(id[4]-'0');
	return i;
}

ttree* search_pos_ticket(ttree *root,tnode t,int *d)
{
	
	ttree *ptr;
	ptr=root;
	int cnt,i,flag=0;
	ttree *retval;
	*d=1;
	if(ptr->isleaf==true)
	{
		cnt=ptr->c;
		for(i=0;i<cnt && flag==0;i++)
		{
			if(tid_to_key((ptr->keys[i]).ticket_id)>=tid_to_key(t.ticket_id))
			{

				flag=1;
				if(tid_to_key((ptr->keys[i]).ticket_id)==tid_to_key(t.ticket_id))
				{
					*d=0;
					printf("element found at leaf\n");
				}
			}
		}
		retval=ptr;
	}
	if(ptr->isleaf==false)
	{
		cnt=ptr->c;
		for(i=0;i<cnt && flag==0;i++)
		{
			if(tid_to_key((ptr->keys[i]).ticket_id)>=tid_to_key(t.ticket_id))
			{
				flag=1;
				if(tid_to_key((ptr->keys[i]).ticket_id)==tid_to_key(t.ticket_id))
				{
					*d=0;
					retval=ptr;
					printf("element found \n");
				}			
			}
		}
		if(flag==1) i--;
		if(*d==1)
		{
			retval=search_pos_ticket(ptr->a[i],t,d);
		}
		
	}
	return retval;
	

}

ttree* findparent_ttree(ttree *root,ttree *ptr)
{
	int flag=0,i=0;
	ttree *retval=NULL;
	if(root->isleaf==false)
	{
		for(i=0 ; i<root->c+1 && flag==0; )
		{
	
			if(root->a[i]!=ptr)
			{
				retval=findparent_ttree(root->a[i],ptr);
				i++;
			
			}
			else
			{
				flag=1;
				retval=root;
			}
		}
	}
	return retval;
}

void add_array_ticket(tnode keys[],tnode t,int c)
{
	int i=c-1;
	int j;
	while(i>=0 && tid_to_key(keys[i].ticket_id)>tid_to_key(t.ticket_id))
	{
		keys[i+1]=keys[i];
		i--;
	}
	strcpy(keys[i+1].ticket_id,t.ticket_id);
	keys[i+1].p=t.p;
	keys[i+1].pass_num=t.pass_num;
	
}

ttree* split_ticket(ttree *root,ttree *ptr)
{
	int i,j,flag,z,p;
	ttree *ptr2,*pptr,*ptr3,*retval;
	if(ptr==root)
	{
		root=add_ticket(NULL,ptr->keys[MAX/2]);
		ptr2=NULL;
			for(j=0;j<(MAX/2);j++)
			{
				ptr2=add_ticket(ptr2,ptr->keys[j]);
			}
			root->a[0]=ptr2;
			root->isleaf=false;
			if(ptr->isleaf==false)
			{
				ptr2->isleaf=false;	
			}	
			ptr2=NULL;
			j++;
			for(;j<MAX;j++)
			{
				ptr2=add_ticket(ptr2,ptr->keys[j]);
			}
			if(ptr->isleaf==false)
			{
				ptr2->isleaf=false;	
			}
			root->a[1]=ptr2;
			retval=root;
			
		if(ptr->isleaf==false)
		{
			j=0;
			ptr2=root->a[0];
			for(i=0;i<=(MAX/2);i++)	
			{
			
				ptr2->a[i]=ptr->a[j++];
			}
			ptr2=root->a[1];
			j=0;
			
			for(;i<=MAX;i++)
			{
				ptr2->a[j++]=ptr->a[i];
			}
		}
		free(ptr);
	}
	else
	{
		pptr=findparent_ttree(root,ptr);		
		add_array_ticket(pptr->keys,ptr->keys[MAX/2],pptr->c);
		pptr->c++;
		flag=0; 
		for(i=0;i<pptr->c && flag==0;)
		{
			if(tid_to_key(pptr->keys[i].ticket_id)==tid_to_key(ptr->keys[MAX/2].ticket_id))
			{
				flag=1;
			}
			else
			{
				i++;
			}
		}
		
		ptr2=NULL;
			for(j=0;j<(MAX/2);j++)
			{
				ptr2=add_ticket(ptr2,ptr->keys[j]);
			}
			pptr->a[i]=ptr2;
			pptr->isleaf=false;
			z=0;
			if(ptr->isleaf==false)
			{
				for(p=0;p<=ptr2->c;p++)
				{
					ptr2->a[p]=ptr->a[z++];
				}
			}
			
		ptr2=NULL;
		j++;
			for(;j<MAX;j++)
			{
				ptr2=add_ticket(ptr2,ptr->keys[j]);
			}
			if(ptr->isleaf==false)
			{
				for(p=0;p<=ptr2->c;p++)
				{
					ptr2->a[p]=ptr->a[z++];
				}
			}
			int k,l;
			l=pptr->c-1;
			k=l+1;
			while(l!=i)
			{
				pptr->a[k]=pptr->a[l];
				k--;
				l--;
			}
			pptr->a[k]=ptr2;

		if(pptr->c<5)
		{
			retval=root;
		}
		else
		{
			retval=split_ticket(root,pptr);
		}
		free(ptr);
	}
	return retval;
}

ttree* add_ticket(ttree *root,tnode t)
{
	ttree *ptr=root,*parent_ptr;
	ttree *ptr3;
	int i;
	int d=1;
	if(root==NULL)
	{
		root=maketicketnode();
		root->keys[root->c]=t;
		(root->c)++;
	}
	else
	{
		ptr=search_pos_ticket(root,t,&d);
		if(d==1)
		{
			if(ptr->c<4)
			{
				add_array_ticket(ptr->keys,t,ptr->c);
				ptr->c++;
			}///dont split
			else//split
			{
				add_array_ticket(ptr->keys,t,ptr->c);
				ptr->c++;
				root=split_ticket(root,ptr);		
			}
		}
	}
	return root;	

}


//Display full data of flight i.e all passengers in that flight
void print(tnode t)
{
	passenger *pptr;
          printf("*********************************************\n");
          printf("Ticket ID: %s \n",t.ticket_id);
          printf("*********************************************\n");
         int i=1;
          pptr=t.p;
          while(pptr!=NULL)
          {
            printf("**passenger %d \n",i);
            printf("**Name:%s \n",pptr->name);
            printf("**Age: %d \n",pptr->age);
            printf("**Seat no.:%d \n",pptr->sno);
            if((pptr->sno)%3==1)printf("**Seat Type: W \n");
            if((pptr->sno)%3==2)printf("**Seat Type: M \n");
            if((pptr->sno)%3==0)printf("**Seat Type: A \n");
			printf("**Luggage nos: %d \n",pptr->luggage_num);
            printf("**Luggage weight: %d \n",pptr->luggage_wt);
            printf("**Fine :%d \n",pptr->fine);
            printf("________________________________________________\n");
            pptr=pptr->next;
            i++;
           }	
}


void traverse(ttree *node)
{
	int i,c;
	if(node->isleaf==true)
	{
		c=node->c;
		for(i=0;i<c;i++)
		{
			print(node->keys[i]);
		}
	}
	else
	{
		c=node->c;
		for (i = 0; i < c+1; ++i)
		{
			traverse(node->a[i]);
			if(i < c)
			{
				print(node->keys[i]);	
			}
		}
	}
}

void DisplayFlightData(char f_id[])
{
     int index,found=0,i,count;
     int d=1,pos=0;
      swnode *ptr;
      ftree *fptr;
      index=hash_swtable(f_id[0]);    //using flight id ,searching for flight
      if(swtable[index]==NULL)
      {
          printf("No such flight exists \n");
      }
	  else   //if found then search for that flight id
	  {
		  ptr=swtable[index];
		  fptr=NULL;
		  while(d==1 && ptr!=NULL)
		  {
				fptr=search_pos_flight(ptr->froot,f_id[0],fid_to_key(f_id),&d);
				
				if(d==1)
				{
					ptr=ptr->next;
				}
		  }
		  if(d==0)
		  {
		  	i=0;
		  	while(fid_to_key(fptr->keys[i].fid)!=fid_to_key(f_id))
		  	{
		  		i++;
		  	}
		  	count=num_av_seats(fptr->keys[i]);
		  	fnode fins;
		  	fins=fptr->keys[i];
		  	pos=i;
		       //  printf("%d \n",count);
		  }
			
		  if(d==1)
		  {
		      printf("No such flight exists \n");
		  }

	  }//else ends
      if(d==0)
      {
		  ttree *tickptr;
		  passenger *pptr;
		  tickptr=NULL;
		  pptr=NULL;
		  tickptr=fptr->keys[pos].troot;
		  traverse(tickptr);

      }

}

void DisplayPassengerDetails(char t_id[])
{
    char f_id[4];
    f_id[0]=t_id[0];
    f_id[1]=t_id[1];
    f_id[2]=t_id[2];
    f_id[3]='\0';
    int index,found=0,i;
    int count=0,pos=0;
    int d=1;
      swnode *ptr;
      ftree *fptr;
      index=hash_swtable(f_id[0]);    //using flight id ,searching for flight
      if(swtable[index]==NULL)
      {
          printf("No such flight exists \n");
      }
	  else   //if found then search for that flight id
	  {
		  ptr=swtable[index];
		  fptr=NULL;
		  while(d==1 && ptr!=NULL)
		  {
				fptr=search_pos_flight(ptr->froot,f_id[0],fid_to_key(f_id),&d);
				
				if(d==1)
				{
					ptr=ptr->next;
				}
		  }
		  if(d==0)
		  {
		  	i=0;
		  	while(fid_to_key(fptr->keys[i].fid)!=fid_to_key(f_id))
		  	{
		  		i++;
		  	}
		  	count=num_av_seats(fptr->keys[i]);
		  	fnode fins;
		  	fins=fptr->keys[i];
		  	pos=i;
		       //  printf("%d \n",count);
		  }
			
		  if(d==1)
		  {
		      printf("No such flight exists \n");
		  }

	  }
      if(d==0)
      {
		  ttree *tickptr;
		  passenger *pptr;
		  tickptr=NULL;
		  pptr=NULL;
		  tnode temp;
		  int k=1;
		  strcpy(temp.ticket_id,t_id);
		  tickptr=fptr->keys[pos].troot;
		  tickptr=search_pos_ticket(tickptr,temp,&k);
		  int i,found=0;
		  for(i=0;i<MAX && found==0 && k==0;i++)
		  {
		  	if(strcmp(tickptr->keys[i].ticket_id,t_id)==0)
		  	{	found=1;
		  	}
		  
		  }
		  i--;
		  tnode t;
		  t=tickptr->keys[i];
		  pptr=t.p;
		  if(found==1)
		  {
                  printf("**************************************************\n");
                  printf("-----------WE WISH YOU A HAPPY JOURNEY-------------\n");
                  printf("Flight id of your flight: %s \n",f_id);
                  printf("Ticket ID: %s \n",t_id);
                  printf("**************************************************\n");
                  i=1;
                  pptr=t.p;
                  while(pptr!=NULL)
                  {
                    printf("*passenger %d \n",i);
                    printf("*Name:%s \n",pptr->name);
                    printf("*Age: %d \n",pptr->age);
                    printf("*Seat no.:%d \n",pptr->sno);
                    if((pptr->sno)%3==1)printf("*Seat Type: W \n");
 	        	    if((pptr->sno)%3==2)printf("*Seat Type: M \n");
    		        if((pptr->sno)%3==0)printf("*Seat Type: A \n");
                    printf("*Luggage nos: %d \n",pptr->luggage_num);
                    printf("*Luggage weight: %d \n",pptr->luggage_wt);
                    printf("*Fine :%d \n",pptr->fine);
                    printf("________________________________________________\n");
                    pptr=pptr->next;
                    i++;
                  }
           }
           if(found==0) printf("Ticket id not found -:please enter correct ticket id\n");
      }


}

void print_range(tnode t,char ftid[],char ttid[])
{
	passenger *pptr;
	if(strcmp(t.ticket_id,ftid)>=0 && strcmp(t.ticket_id,ttid)<=0)
	{
	
          printf("*********************************************\n");
          printf("Ticket ID: %s \n",t.ticket_id);
          printf("*********************************************\n");
         int i=1;
          pptr=t.p;
          while(pptr!=NULL)
          {
            printf("**passenger %d \n",i);
            printf("**Name:%s \n",pptr->name);
            printf("**Age: %d \n",pptr->age);
            printf("**Seat no.:%d \n",pptr->sno);
            if((pptr->sno)%3==1)printf("**Seat Type: W \n");
            if((pptr->sno)%3==2)printf("**Seat Type: M \n");
            if((pptr->sno)%3==0)printf("**Seat Type: A \n");
			printf("**Luggage nos: %d \n",pptr->luggage_num);
            printf("**Luggage weight: %d \n",pptr->luggage_wt);
            printf("**Fine :%d \n",pptr->fine);
            printf("________________________________________________\n");
            pptr=pptr->next;
            i++;
           }
      }	
}


void traverse_range(ttree *node,char ftid[],char ttid[])
{
	int i,c;
	if(node->isleaf==true)
	{
		c=node->c;
		for(i=0;i<c;i++)
		{
			print_range(node->keys[i],ftid,ttid);
		}
	}
	else
	{
		c=node->c;
		for (i = 0; i < c+1; ++i)
		{
			traverse_range(node->a[i],ftid,ttid);
			if(i < c)
			{
				print_range(node->keys[i],ftid,ttid);	
			}
		}
	}
}

void RangeDisplayPassengerDetails(char f_id[],char ftid[],char ttid[])
{
     int index,found=0,i,count;
     int d=1,pos=0;
      swnode *ptr;
      ftree *fptr;
      index=hash_swtable(f_id[0]);    //using flight id ,searching for flight
      if(swtable[index]==NULL)
      {
          printf("No such flight exists \n");
      }
	  else   //if found then search for that flight id
	  {
		  ptr=swtable[index];
		  fptr=NULL;
		  while(d==1 && ptr!=NULL)
		  {
				fptr=search_pos_flight(ptr->froot,f_id[0],fid_to_key(f_id),&d);
				
				if(d==1)
				{
					ptr=ptr->next;
				}
		  }
		  if(d==0)
		  {
		  	i=0;
		  	while(fid_to_key(fptr->keys[i].fid)!=fid_to_key(f_id))
		  	{
		  		i++;
		  	}
		  	fnode fins;
		  	fins=fptr->keys[i];
		  	pos=i;
		       //  printf("%d \n",count);
		  }
			
		  if(d==1)
		  {
		      printf("No such flight exists \n");
		  }

	  }
	  traverse_range(fptr->keys[i].troot,ftid,ttid);
	  
}


void print_menu()
{
    printf("Enter your choice\n");
    printf("1)To add service window \n");
    printf("2)To book a ticket \n");
    printf("3)To Display flight data-:details of checked-in passengers \n");
    printf("4)To Display passenger details \n");
    printf("5)To Display Available Seats in flight \n");
    printf("6)To Display range of tickets\n");
    printf("7)To see this menu again \n");
}
//main driver function---------------------------

void front_end()
{
	printf("\t\t|************************ HELLO USER************************* |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|          A hearty welcome to flight booking system          |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|                                                             |\n");
	printf("\t\t|                              - A project by-:Hardik gandhi  |\n");
	printf("\t\t|************************************************************ |\n");
}

int main()
{
    int choice=1;
    swnode *swptr;
    swptr=NULL;
    intialise_swtable(swtable);
    char flight_id_user[4];
    char ticket_id_user[6];
    char from_ticket_id_user[6],to_ticket_id_user[6];
    front_end();
    print_menu();
    while(choice)
    {
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:	addFlightServiceWindow(swtable);
            break;
            case 2:printf("Enter flight id to book :\n");
            scanf("%s",flight_id_user);
            allocateBoardingPass(flight_id_user);
            break; 
            case 3:printf("Enter flight id to display details :\n");
            scanf("%s",flight_id_user);
            DisplayFlightData(flight_id_user);
            break;
            case 4:printf("Enter ticket id to get details \n");
            scanf("%s",ticket_id_user);
            DisplayPassengerDetails(ticket_id_user);
            break;  
            case 5: printf("Enter flight id :\n");
            scanf("%s",flight_id_user);
            DisplayAvailableSeats(flight_id_user);
            break; 
            case 6: printf("Enter flight id :\n");
            scanf("%s",flight_id_user);
            printf("Enter from ticket id  \n");
            scanf("%s",from_ticket_id_user);
            printf("Enter to ticket id to get details \n");
            scanf("%s",to_ticket_id_user);
            RangeDisplayPassengerDetails(flight_id_user,from_ticket_id_user,to_ticket_id_user);
            break;
            case 7: print_menu();
            break; 
            case 8:display(swtable[0]->froot,0);
            break;

        }

    }
	printf("\n**********Thank you for visiting************\n");
    return 0;
}
