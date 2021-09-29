#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 22
#define INFINITY 4294967295

typedef struct {
	unsigned int costo;
	int finish; //vale 1 se ho già analizzato le frecce uscenti del nodo
}pq; //priority queue

struct nodo{
	unsigned int valore; //valore == costo
	unsigned int indice; //indice == ordine casuale di arrivo del grafo
	struct nodo* next;
};

unsigned int* punta_matrice(unsigned int* matrice,unsigned int i,unsigned int j,unsigned int d){
	unsigned int coeff;
	if(i<j){
		coeff=i+1;
	}
	else{
		coeff=i;
	}
	
	if(i==j){
		return(NULL);
	}
	
	return(&matrice[i*d+j-coeff]);
}

void u2s( char* out, unsigned int value ){
    unsigned int work[10] = { 0 };
    int count = 0;
    do{
       work[count++] = value % 10;
      value /= 10;
    } while ( value );
    while ( count > 0 )
      *out++ = '0' + work[--count];
    *out = 0;
}

void printa_stringa(char* lettera){
	while(*lettera =='0' || *lettera =='1' || *lettera =='2'||*lettera =='3'||*lettera =='4'||*lettera =='5'||*lettera =='6'||*lettera =='7'||*lettera =='8'||*lettera =='9'){
		putchar_unlocked((int)*lettera);
		lettera=lettera+1;
	}
}

/*
//debug function
void printmatrix(unsigned int* matrice,unsigned int d){
	unsigned int i,j;
	for(i=0;i<d;i++){
		for(j=0;j<d;j++){
			if(i!=j){
				printf("%u ",*punta_matrice(matrice,i,j,d));
			}
			if(i==j){
				printf("inf ");
			}
		}
		printf("\n");
	}
}
*/
struct nodo* distruggi_coda(struct nodo* testa){
	struct nodo* backup;
	backup=testa;
	if(testa==NULL){
		return NULL;
	}
	if(testa->next==NULL){
		free(testa);
		return NULL;
	}
	while(testa->next->next != NULL){
		testa=testa->next;
	}
	free(testa->next);
	testa->next = NULL;
	return(backup);
}

struct nodo* aggiungi_in_testa(struct nodo* vecchia_testa,unsigned int i, unsigned int index){
	struct nodo* nuova_testa;
	nuova_testa=malloc(sizeof(struct nodo));
	nuova_testa->next=vecchia_testa;
	nuova_testa->valore=i;
	nuova_testa->indice=index;
	return nuova_testa;
}

struct nodo* aggiungi_in_coda(struct nodo* vecchia_testa,unsigned int i,unsigned int index){
	struct nodo* punto_di_aggancio;
	struct nodo* nuovo_nodo;
	punto_di_aggancio=vecchia_testa;
	if(punto_di_aggancio==NULL){
		punto_di_aggancio=aggiungi_in_testa(NULL,i,index);
		return punto_di_aggancio;
	}
	while(punto_di_aggancio->next!=NULL){
		punto_di_aggancio=punto_di_aggancio->next;
	}
	nuovo_nodo=malloc(sizeof(struct nodo));
	nuovo_nodo->valore=i;
	nuovo_nodo->indice=index;
	nuovo_nodo->next=NULL;
	punto_di_aggancio->next=nuovo_nodo;
	return vecchia_testa;
}

void printa_lista(struct nodo* testa){
	char a_capo='\n';
	char spazio=' ';
	char a[10];
	
	while(testa->next != NULL){
		//fprintf(stdout,"%u ",testa->indice);//vecchia implementazione
		u2s(a,testa->indice);
		printa_stringa(a);
		putchar_unlocked((int)spazio);
		testa=testa->next;
	}
	//fprintf(stdout,"%u\n",testa->indice);
	u2s(a,testa->indice);
		printa_stringa(a);
		putchar_unlocked((int)a_capo);
}

void cancella_nodo_intermedio(struct nodo* prev){
	struct nodo* todelete;
	todelete=prev->next;
	prev->next=todelete->next;
	free(todelete);
}

void inserisci_nodo_intermedio(struct nodo* prev,unsigned int data,unsigned int index){
	struct nodo* succ;
	struct nodo* new;
	succ=prev->next;
	new=malloc(sizeof(struct nodo));
	prev->next=new;
	new->next=succ;
	new->valore=data;
	new->indice=index;
}

struct nodo* accrescimento_ordinato(struct nodo* testa,unsigned int data, unsigned int index, unsigned int flag_k){
	struct nodo* prev;
	struct nodo* succ;
	if(testa==NULL && flag_k==0){
		testa=malloc(sizeof(struct nodo));
		testa->next=NULL;
		testa->valore=data;
		testa->indice=index;
		return testa;
	}
	if(testa->next==NULL){
		if(testa->valore >= data){
			testa=aggiungi_in_testa(testa,data,index);
			if(flag_k==1){
				testa=distruggi_coda(testa);
				}
			}
		if(testa->valore < data && flag_k==0){
			testa=aggiungi_in_coda(testa,data,index);
		}
		return testa;
	}	
	if(testa->valore >= data && flag_k==0){
		testa=aggiungi_in_testa(testa,data,index);
		if(flag_k==1){
			testa=distruggi_coda(testa);
		}
		return testa;
	}
	prev=testa;
	succ=prev->next;
	while(succ->next!=NULL && succ->valore <= data){
		prev=prev->next;
		succ=prev->next;
	}
	if(succ->next==NULL && flag_k==1){
		return testa;
	}
	inserisci_nodo_intermedio(prev,data,index);
	if(flag_k==1){
		testa=distruggi_coda(testa);
	}
	return testa;
}

int checkfinish(pq* priority, unsigned int d){
	unsigned int i;
	for(i=0;i<d;i++){
		if(priority[i].finish==0){
			return(0);  //la struct contiene un nodo non finito
		}
	}
	return(1); //la struct contiene tutti nodi finiti
}
/*
void debugprint(pq* priority, unsigned int d){ //funzione per il solo debug, da rimuovere successivamente
	unsigned int i;
	printf("nodo costo finish\n");
	for(i=0;i<d;i++){
		printf("%u %u  %d\n",i,priority[i].costo,priority[i].finish);
	}
}
*/
void updatepq(pq* priority, unsigned int d, unsigned int node_index, unsigned int* matrice){
	unsigned int j;
	unsigned int costo_nuovo;
	unsigned int* peso_matrice;
	for(j=0;j<d;j++){
		peso_matrice=punta_matrice(matrice,node_index,j,d);
		if(peso_matrice!=NULL && *peso_matrice!=INFINITY ){
			costo_nuovo=*peso_matrice + priority[node_index].costo; //overflow potenziale
			if(priority[j].costo > costo_nuovo){
				priority[j].costo=costo_nuovo;
			}
		}
	}
	priority[node_index].finish=1;	
}


unsigned int lowestnodo(pq* priority, unsigned int d){
	unsigned int i;
	unsigned int lowest,lowest_peso;
	int flag=1;
	lowest=0;
	lowest_peso=priority[0].costo;
	while(flag==1){
		while(priority[lowest].finish!=0 && lowest<d){
			lowest++;
			lowest_peso=priority[lowest].costo;
		}
		flag=0; //arrivo quì se una delle due condizioni non viene rispettata
	}
	for(i=lowest;i<d;i++){
		if(priority[i].costo < lowest_peso && priority[i].finish==0){
			lowest=i;
			lowest_peso=priority[i].costo;
		}
	}
	if(lowest_peso==INFINITY){
		priority[lowest].finish=-1;//nodo non raggiungibile
	}
	return(lowest);
}

unsigned int sommacosti(pq* priority, unsigned int d){
	unsigned int i;
	unsigned int sum=0;
	for(i=0;i<d;i++){
		if(priority[i].costo!=INFINITY){
			sum=sum+priority[i].costo; //overflow potenziale
		}
	}
	return(sum);
}

int leggidabuffer(char* buffer,int buffersize){
	int flag;
    flag=0;
	while(flag==0){
    	if(fgets(buffer, buffersize, stdin) == NULL){
    		return(1); //ERROR CODE 1: si è raggiunto EOF
		}
        if(buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
            flag=1;
        } 
    }
    return(0); //ERROR CODE 0: successo lettura riga da input 
}



unsigned int AggiungiGrafo(unsigned int d){
	pq priority[d];
	unsigned int matrice[d*d-d];
	unsigned int lunghezzarigadainput;
	unsigned int i,j;
	unsigned int* puntatore;
	unsigned int nodo_smallest;
	lunghezzarigadainput=11*d+1; //lunghezza max in caratteri di una riga di input
	char inputline[lunghezzarigadainput];
	char* token;

	for(i=0;i<d && leggidabuffer(inputline,lunghezzarigadainput)==0 ;i++){
		token=strtok(inputline, ",");
		for(j=0;j<d;j++){
			//printf("i=%u j=%u\n",i,j);
			puntatore=punta_matrice(matrice,i,j,d);
			if(puntatore!=NULL){
			//	sscanf(token,"%u",puntatore);
				*puntatore=(unsigned int)atoi(token);
				//printf("%u\n",*puntatore);
				if(*puntatore==0){	
					*puntatore=INFINITY;	
				}
			}
			token = strtok(NULL, ",");	
		}
	}
	
	//debug
	//printmatrix(matrice,d);
	
	//popolamento iniziale della priority queue
	priority[0].costo=0;
	//priority[0].da=0;
	priority[0].finish=1;
	for(j=1;j<d;j++){
		priority[j].costo=*punta_matrice(matrice,0,j,d);
	//	priority[j].da=0;
		priority[j].finish=0;
	}
	while(checkfinish(priority,d)==0){
		//debugprint(priority,d);
		nodo_smallest=lowestnodo(priority,d);
		if (priority[nodo_smallest].finish!=-1){
			updatepq(priority,d,nodo_smallest,matrice);
		}
	}
	//debugprint(priority,d);
	return(sommacosti(priority,d));
}


int main(void)
{
	unsigned int d; //numero nodi dei grafi
	unsigned int k; //numero di grafi da selezionare
	unsigned int k_temp; 
	unsigned int i=0;
	char letturadainput[BUFFERSIZE];
	struct nodo* testa=NULL;
	char* token;
	//lettura ed analisi della prima riga da input
	leggidabuffer(letturadainput,BUFFERSIZE);
	//sscanf(letturadainput,"%u %u",&d,&k);
	token=strtok(letturadainput, " ");
	d=(unsigned int)atoi(token);
	token = strtok(NULL, " ");
	k=(unsigned int)atoi(token);
	k_temp=k;
//	printf("%u\n%u\n",d,k);
	while(0==leggidabuffer(letturadainput,BUFFERSIZE)){
		if(strcmp(letturadainput,"AggiungiGrafo")==0){
			if(k_temp!=0){
				testa=accrescimento_ordinato(testa,AggiungiGrafo(d),i,0);
			}
			else{
				testa=accrescimento_ordinato(testa,AggiungiGrafo(d),i,1);
			}
			i++;
			if(k_temp!=0){
				k_temp--;
			}
		}
		if(strcmp(letturadainput,"TopK")==0){
			if(testa!=NULL){
				printa_lista(testa);
			}
			else{
			 printf("\n");
			}
		}
	}
	return 0;
}
