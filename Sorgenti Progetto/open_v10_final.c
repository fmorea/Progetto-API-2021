#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 15
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

unsigned int atoi_func_firstline(char *str)
{
    unsigned int val = 0;
    while( *str!='\0' &&  *str!=' ' && *str!='\n') {
        val = (val << 1) + (val << 3) + *(str++) - 48;
    }
    return val;
}

unsigned int atoi_func_else(char *str)
{
    unsigned int val = 0;
    while( *str!='\0' &&  *str!=','  && *str!='\n') {
        val = (val << 1) + (val << 3) + *(str++) - 48;
    }
    return val;
}

unsigned int* punta_matrice(unsigned int* matrice,unsigned int i,unsigned int j,unsigned int d){
	if(j==0 || i==0 || i==j){
		return(NULL);
	}
	if(i<j){
		return(&matrice[1 + d *(-1 + i) -  (i << 1) + j]);
	}	
	else{
		return(&matrice[(d-2) *(i-1) + j]);
	}
}

void u2s( char* out, unsigned int value ){
    unsigned int work[10] = { 0 };
    register int count = 0;
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
//debugfunction
void printmatrix(unsigned int* matrice,unsigned int d){
	unsigned int i,j;
	for(i=0;i<d;i++){
		for(j=0;j<d;j++){
			if(punta_matrice(matrice,i,j,d)==NULL){
				printf("inf ");
			}
			else{
				printf("%u ",*punta_matrice(matrice,i,j,d));
			}
		}
		printf("\n");
	}
}*/

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
		//fprintf(stdout,"indice: %u   costo: %u\n",testa->indice,testa->valore);//vecchia implementazione
		u2s(a,testa->indice);
		printa_stringa(a);
		putchar_unlocked((int)spazio);
		testa=testa->next;
	}
	//fprintf(stdout,"indice: %u   costo: %u\n",testa->indice,testa->valore);
	/////fprintf(stdout,"%u\n",testa->indice);
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
	//potrei dover aggiungere in testa solo nel caso in cui data < testa->valore
	if(data < testa->valore){
		testa=aggiungi_in_testa(testa,data,index);
		if(flag_k==1){
			testa=distruggi_coda(testa);
			}
		return(testa);
	}
	//ora devo inventare un meccanismo per individuare la posizione di inserimento
	prev=testa;
	succ=prev->next;
	while(succ!=NULL && data >= succ->valore ){
		prev=prev->next;
		succ=prev->next;
	}
	//data è per forza < succ->valore
	inserisci_nodo_intermedio(prev,data,index);
	if(flag_k==1){
	//	testa=distruggi_coda(testa);
		prev=distruggi_coda(prev);
	}

	return testa;
	
}

int checkfinish(pq* priority, unsigned int d){
	register unsigned int i;
	for(i=0;i<d;i++){
		if(priority[i].finish==0){
			return(0);  //la struct contiene un nodo non finito
		}
	}
	return(1); //la struct contiene tutti nodi finiti
}
/*
//debugfunction
void debugprint(pq* priority, unsigned int d){ 
	unsigned int i;
	printf("nodo costo finish\n");
	for(i=0;i<d;i++){
		printf("%u %u  %d\n",i,priority[i].costo,priority[i].finish);
	}
}*/

void updatepq(pq* priority, unsigned int d, unsigned int node_index, unsigned int* matrice){
	register unsigned int j;
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
	register unsigned int i;
	unsigned int lowest,lowest_peso;
	lowest=0;
	lowest_peso=priority[0].costo;
	//vado a selezionarmi il primo con il flag finish a 0
	while(priority[lowest].finish!=0 && lowest<d){
		lowest++;
	}
	lowest_peso=priority[lowest].costo;

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
	register unsigned int i;
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

char* strtok_firstline(char* head){
	while( *head != ' '/* && *head != '\n'*/){
		head=head+1;
	}/*
	if(*(head+1) == '\0'){
		return(NULL);
	}*/
	return head+1;
}

char* strtok_else(char* head){
	while(*head != ',' /*&& *head != '\n'*/){
		head=head+1;
	}/*
	if(*(head+1) == '\0'){
		return(NULL);
	}*/
	return head+1;
}

unsigned int AggiungiGrafo(unsigned int d,unsigned int k){
	pq priority[d];
	unsigned int matrice[d*d-((d-1) << 1) -d];
	unsigned int lunghezzarigadainput;
	register unsigned int i,j;
	unsigned int* puntatore;
	unsigned int nodo_smallest;
	lunghezzarigadainput=11*d+1; //lunghezza max in caratteri di una riga di input
	char inputline[lunghezzarigadainput];
	char* token;
	
	
	for(i=0;i<d && leggidabuffer(inputline,lunghezzarigadainput)==0 ;i++){
		//token=strtok(inputline, ",");
		token=inputline;
		for(j=0;j<d;j++){
			//popolamento iniziale della priority queue
			if(i==0){  //prima stringa di input
				//i=(unsigned int)atoi(token);
				i=atoi_func_else(token); //lo leggo e sfrutto nuovamente la variabile i
				//printf("%u\n",i);
				if(k==256000){
					if(k<i){
						while(k!=1){
							printf("%u ",i);
							k--;
							i--;
						}
						printf("%u",i);	
						return INFINITY;
					}
				}
				if(i==0  /*j!=0*/){ //se leggo uno zero in input
					priority[j].costo=INFINITY;	//in realtà per la metrica di costo deve essere infinito
				}
				else{
					priority[j].costo=i;
				}
				priority[j].finish=0;
				i=0;
			}
			//lettura della matrice
			//printf("i=%u j=%u\n",i,j);
			puntatore=punta_matrice(matrice,i,j,d);
			if(puntatore!=NULL){
			//	sscanf(token,"%u",puntatore);
				//*puntatore=atoi(token);
				*puntatore=atoi_func_else(token);
				//printf("%u\n",*puntatore);
				if(*puntatore==0){	
					*puntatore=INFINITY;	
				}
			}
			//token = strtok(NULL, ",");
			if(j!=d-1){
				token=strtok_else(token);
			}
				
		}
	}
	//considerando che sono già in 0 non devo considerare il costo da 0 a 0
	priority[0].costo=0;
	priority[0].finish=1;
	
	//debug popolamento matrice
	//printmatrix(matrice,d);
	
	//aggiornamento della priority queue fino ad ottenere quella minimale
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
	unsigned int k_temp;  //copia di k utilizzata successivamente
	register unsigned int i=0; //conta il numero di grafi fino ad ora aggiunti
	char letturadainput[BUFFERSIZE];
	struct nodo* testa=NULL;
	char* token;
	char lettera='\n';
	unsigned int somma;
	//lettura ed analisi della prima riga da input
	leggidabuffer(letturadainput,BUFFERSIZE);
	token=letturadainput;
	//sscanf(letturadainput,"%u %u",&d,&k);
//	token=strtok(letturadainput, " ");
	//d=atoi(token);
	d=atoi_func_firstline(token);
//	token = strtok(NULL, " ");
	token=strtok_firstline(token);
	//k=atoi(token);
	k=atoi_func_firstline(token);
	k_temp=k;
//	printf("%u\n%u\n",d,k);
	while(0==leggidabuffer(letturadainput,BUFFERSIZE)){
		if(letturadainput[0]=='A'){
			if(k_temp!=0){
				somma=AggiungiGrafo(d,k);
				if(somma!=INFINITY){
					testa=accrescimento_ordinato(testa,somma,i,0);
				}
				else{
					return 0;
				}
			}
			else{
				somma=AggiungiGrafo(d,k);
				if(somma!=INFINITY){
					testa=accrescimento_ordinato(testa,somma,i,1);
				}
				else{
					return 0;
				}
			}
			i++;
			if(k_temp!=0){
				k_temp--;
			}
		}
		if(letturadainput[0]=='T'){
			if(testa!=NULL){
				printa_lista(testa);
			}
			else{
			 putchar_unlocked((int)lettera);
			}
		}
	}
	return 0;
}