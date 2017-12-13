#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "mpi.h"

struct rBufAssoc { 
  void * addr; 
  void * taddr;
  int length;
  int req;
  struct rBufAssoc* next;
} rBufFirst = {0,0,0,0}; 

struct sBufAssoc { 
  void * addr;
  int length;
  int req;
  struct sBufAssoc* next;
} sBufFirst = {0,0,0}; 

static struct rBufAssoc* rBufAssoc_head=&rBufFirst;
static struct sBufAssoc* sBufAssoc_head=&sBufFirst;

void associateR(void* buf, 
		void* tBuf,
		int count, 
		int req) {
  struct rBufAssoc* thisAssoc=rBufAssoc_head;
  while (1) { 
    if (thisAssoc->req==0)
      break; 
    if (thisAssoc->next==0)
      break;
    thisAssoc=thisAssoc->next;
  }
  if (thisAssoc->req!=0) { 
    thisAssoc->next=(struct rBufAssoc*)malloc(sizeof(struct rBufAssoc));
    thisAssoc=thisAssoc->next;
    thisAssoc->next=0;
  }
  thisAssoc->addr  =buf;
  thisAssoc->taddr =tBuf;
  thisAssoc->length=count;
  thisAssoc->req   =req;
}

void associateS(void* buf, 
		int count, 
		int req) {
  struct sBufAssoc* thisAssoc=sBufAssoc_head;
  while (1) { 
    if (thisAssoc->req==0)
      break; 
    if (thisAssoc->next==0)
      break;
    thisAssoc=thisAssoc->next;
  }
  if (thisAssoc->req!=0) { 
    thisAssoc->next=(struct sBufAssoc*)malloc(sizeof(struct sBufAssoc));
    thisAssoc=thisAssoc->next;
    thisAssoc->next=0;
  }
  thisAssoc->addr  =buf;
  thisAssoc->length=count;
  thisAssoc->req   =req;
}

/* 
 combine temporary buffer allocation 
 and bookkeeping
*/
void oadtirecv_(void *buf,
		int *count, 
		int *datatype, 
		int *src, 
		int *tag, 
		int *comm, 
		int *req, 
		int *ierror) {
  double * tBuf;
  int myId;
  *ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  tBuf=malloc(*count*sizeof(double));
  *ierror = MPI_Irecv( tBuf, 
		       *count, 
		       (MPI_Datatype)(*datatype), 
		       *src, 
		       *tag, 
		       (MPI_Comm)(*comm), 
		       (MPI_Request *)(req));
  /* printf("%i: irecv b:%x t:%x c:%i s:%i r:%i\n",myId, buf,(void*)tBuf,*count, *src, *req);*/
  associateR(buf, tBuf,*count, *req);
} 

/* 
 combine temporary buffer allocation 
 and bookkeeping
*/
void oadtisend_(void *buf,
		int *count, 
		int *datatype, 
		int *dest, 
		int *tag, 
		int *comm, 
		int *req, 
		int *ierror) {
  int myId;
  *ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  *ierror = MPI_Isend( buf, 
		       *count, 
		       (MPI_Datatype)(*datatype), 
		       *dest, 
		       *tag, 
		       (MPI_Comm)(*comm), 
		       (MPI_Request *)(req));
  /* printf("%i: isend b:%x c:%i d:%i r:%i\n",myId, buf,*count, *dest, *req); */
  associateS(buf,*count, *req);
} 

void oadtsend_(void *buf,
	       int *count, 
	       int *datatype, 
	       int *dest, 
	       int *tag, 
	       int *comm, 
	       int *ierror) {
  int myId;
  *ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  /* printf("%i: send b:%x c:%i dt:%i d:%i \n",myId, buf,*count, *datatype, *dest); */
  *ierror = MPI_Send( buf, 
		      *count, 
		      (MPI_Datatype)(*datatype), 
		      *dest, 
		      *tag, 
		      (MPI_Comm)(*comm));
  memset(buf,0,
	 *count*sizeof(double));
} 


void oadhandlerequest_ (int *r) { 
  int done=0;
  double *tBuf;
  double *rBuf;
  int i;
  int myId,ierror;
  struct rBufAssoc* rAssoc=rBufAssoc_head;
  struct sBufAssoc* sAssoc=sBufAssoc_head;
  ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  while(sAssoc) { 
    if (*r==sAssoc->req) { 
	memset(sAssoc->addr,0,
	       sAssoc->length*sizeof(double));
	sAssoc->req=0; 
	done=1;
	/* printf("%i: handle S request r:%i\n",myId, *r); */
	break; 
    }
    sAssoc=sAssoc->next;
  }
  if (done)
    return;
  while(rAssoc) { 
    if (*r==rAssoc->req) { 
      tBuf=(double *)rAssoc->taddr;
      rBuf=(double *)rAssoc->addr;
      for(i=0;i<rAssoc->length;i++) { 
	rBuf[i]+=tBuf[i];
      }
      free(tBuf);
      rAssoc->req=0;
      /* printf("%i: handle R request r:%i\n",myId, *r); */
      done=1;
      break; 
    }
    rAssoc=rAssoc->next;
  }
  if (!done) { 
    /* printf("%i: cannot handle request r:%i\n",myId, *r); */
  }
}
