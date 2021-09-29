#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 15			//massima dimensione del buffer di input per i comandi AggiungiGrafo e TopK
#define INFINITY 4294967295		//massimo intero senza segno su 32 bit
#define BIG_K 100000			//soglia necessaria successivamente che definisce arbitrariamente un "K grande"

//Struttura per contenere la lista di priorità, priority queue == pq
typedef struct {
	unsigned int costo;		//Peso di una freccia uscente dal nodo
	int finish; 			//servirà per terminare l'algoritmo quando tutti i nodi saranno visitati
}pq; 

struct nodo{
	unsigned int valore;	 //valore == costo
	unsigned int indice; 	//indice == ordine casuale di arrivo del grafo
	struct nodo* next;		//puntatore al prossimo nodo
};


//conversione da stringa ascii ad unsigned int per la prima riga di input
unsigned int atoi_func_firstline(char *str)
{
    unsigned int val = 0;
    while( *str!='\0' &&  *str!=' ' && *str!='\n') {
        val = (val << 1) + (val << 3) + *(str++) - 48;
    }
    return val;
}

//conversione da stringa ascii ad unsigned int per le altre righe di input
unsigned int atoi_func_else(char *str)
{
    unsigned int val = 0;
    while( *str!='\0' &&  *str!=','  && *str!='\n') {
        val = (val << 1) + (val << 3) + *(str++) - 48;
    }
    return val;
}

//l'idea di base è che non serve memorizzare tutta la matrice, ad esempio tutti gli autoanelli non
//mi danno informazione per raggiugere altri nodi, come la prima riga che viene usata solo in
//fase di inizializzazione della priority queue, e la prima colonna che non mi serve perché sono già
//in zero, questo permette di diminuire la dimensione della matrice

//i=indice di riga della matrice	j=indice di colonna della matrice	d=dimensione matrice
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

//funzione che converte da unsigned int a striga ascii
void u2s( char* output, unsigned int valore ){
    unsigned int array[10] = { 0 };
    register int contatore = 0;
    do{
    	array[contatore++] = valore % 10;
    	valore /= 10;
    } while ( valore );
    while ( contatore > 0 ) *output++ = '0' + array[--contatore];
    *output = 0;
}

//semplice reimplementazione di printf per le stringhe
void printa_stringa(char* lettera){
	while(*lettera =='0' || *lettera =='1' || *lettera =='2'||*lettera =='3'||*lettera =='4'||*lettera =='5'||*lettera =='6'||*lettera =='7'||*lettera =='8'||*lettera =='9'){
		putchar_unlocked((int)*lettera);
		lettera=lettera+1;
	}
}

//Semplici funzioni di gestione della linked list, self-explanatory
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

//Semplici funzioni di gestione della linked list, self-explanatory
struct nodo* aggiungi_in_testa(struct nodo* vecchia_testa,unsigned int i, unsigned int index){
	struct nodo* nuova_testa;
	nuova_testa=malloc(sizeof(struct nodo));
	nuova_testa->next=vecchia_testa;
	nuova_testa->valore=i;
	nuova_testa->indice=index;
	return nuova_testa;
}

//Semplici funzioni di gestione della linked list, self-explanatory
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

//Funzione usata per stampare la lista quando viene impartito il comando TopK
//per l'uso del flag_big fare riferimento alle funzioni AggiungiGrafo e Main
void printa_lista(struct nodo* testa,int flag_big, unsigned int k){
	char a_capo='\n';
	char spazio=' ';
	char a[10];
	
	if(flag_big==0){
		while(testa->next != NULL){
			u2s(a,testa->indice);
			printa_stringa(a);
			putchar_unlocked((int)spazio);
			testa=testa->next;
		}
	}
	if(flag_big==1){	//se flag_big è posto ad 1, stampa solo le prime k, perché la lista sarà più lunga
		while(k!=1){
			u2s(a,testa->indice);
			printa_stringa(a);
			putchar_unlocked((int)spazio);
			testa=testa->next;
			k--;
		}
	}
	u2s(a,testa->indice);
	printa_stringa(a);
	putchar_unlocked((int)a_capo);
}

//Semplici funzioni di gestione della linked list, self-explanatory
void cancella_nodo_intermedio(struct nodo* prev){
	struct nodo* todelete;
	todelete=prev->next;
	prev->next=todelete->next;
	free(todelete);
}

//Semplici funzioni di gestione della linked list, self-explanatory
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

//Funzione che posiziona nel punto giusto della linked list un nuovo nodo,
//mantenendo un ordine di costo crescente, cosicché i nodi con costo più basso
//si troveranno all'inizio

//data=costo  index=indice temporale di aggiunta del grafo
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
		prev=distruggi_coda(prev);
	}
	return testa;
}


//Funzione che scansiona i flag finish e restituisce 1 se tutti i nodi sono stati visitati
int checkfinish(pq* priority, unsigned int d){
	register unsigned int i;
	for(i=0;i<d;i++){
		if(priority[i].finish==0){
			return(0);  //la struct contiene un nodo non finito
		}
	}
	return(1); //la struct contiene tutti nodi finiti
}

//Funzione di aggiornamento della priority queue con i dati forniti dalla matrice, riga i=node_index
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

//Funzione che permette di trovare il nodo a minore peso della priority queue
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

//funzione che restituisce la somma dei costi minimi
//secondo le specifiche del progetto
unsigned int sommacosti(pq* priority, unsigned int d){
	register unsigned int i;
	unsigned int sum=0;
	for(i=0;i<d;i++){
		if(priority[i].costo!=INFINITY){
			sum=sum+priority[i].costo; //overflow potenziale, ma da specifiche del progetto non può accadere
		}
	}
	return(sum);
}


//funzione che legge una riga da input
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


//	Implementazione molto unsafe di strtok, per la prima riga di input
//	unsafe perché potrebbe restituire un puntatore in una zona di memoria non accessibile se non usato bene
char* strtok_firstline(char* head){
	while( *head != ' '){
		head=head+1;
	}
	return head+1;
}

//	Implementazione molto unsafe di strtok
char* strtok_else(char* head){
	while(*head != ','){
		head=head+1;
	}
	return head+1;
}


//	Una delle funzioni più importanti del progetto, tra i suoi ruoli:
//	-Leggere la prima riga della matrice ed inizializzare la priority queue
//	-Leggere e memorizzare il resto della matrice
//	-Chiamare più volte updatepq fino a che la matrice non è stata completamente analizzata
//	-Chiamare sommacosti e farsi restituire la somma dei cammini minimi
//	-Restituire al chiamante la somma dei cammini minimi

//	Il ruolo di flag_big è il seguente:
// Se AggiungiGrafo si accorge che K è molto alto (arbitrariamente definito dalla #define BIG_K)
// Si attiva un meccanismo di segnalazione tale per cui la funzione accrescimento_ordinato
//	non si metterà a perdere tempo a cancellare la coda, vedere l'implementazione di accrescimento_ordinato
unsigned int AggiungiGrafo(unsigned int d,unsigned int k, unsigned int index,int* flag_big){
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
		token=inputline;
		for(j=0;j<d;j++){
			//popolamento iniziale della priority queue
			if(i==0){  //prima stringa di input
				i=atoi_func_else(token); //lo leggo e sfrutto nuovamente la variabile i
				if(index==0 && i > k && k > BIG_K){
					*flag_big=1;
				}
				if(i==0){ //se leggo uno zero in input
					priority[j].costo=INFINITY;	//in realtà per la metrica di costo deve essere infinito
				}
				else{
					priority[j].costo=i;
				}
				priority[j].finish=0;
				i=0;
			}
			//lettura della matrice
			puntatore=punta_matrice(matrice,i,j,d);
			if(puntatore!=NULL){
				*puntatore=atoi_func_else(token);
				if(*puntatore==0){	
					*puntatore=INFINITY;	
				}
			}
			if(j!=d-1){
				token=strtok_else(token);
			}	
		}
	}
	//considerando che sono già in 0 non devo considerare il costo da 0 a 0
	priority[0].costo=0;
	priority[0].finish=1;
	
	//aggiornamento della priority queue fino ad ottenere quella minimale
	while(checkfinish(priority,d)==0){
		nodo_smallest=lowestnodo(priority,d);
		if (priority[nodo_smallest].finish!=-1){
			updatepq(priority,d,nodo_smallest,matrice);
		}
	}
	return(sommacosti(priority,d)); 
}


//	Il main si limita a ricevere i comandi "AggiungiGrafo" e "TopK" e passare la palla
//	alle altre funzioni di interesse.
int main(void)
{
	unsigned int d;					//numero nodi dei grafi
	unsigned int k; 				//numero di grafi da selezionare
	unsigned int k_temp; 		 	//copia di k utilizzata successivamente
	register unsigned int i=0; 		//conta il numero di grafi fino ad ora aggiunti
	char letturadainput[BUFFERSIZE];
	struct nodo* testa=NULL;
	char* token;
	char lettera='\n';
	int flag_big=0;
	//lettura ed analisi della prima riga da input
	leggidabuffer(letturadainput,BUFFERSIZE);
	token=letturadainput;
	d=atoi_func_firstline(token);
	token=strtok_firstline(token);
	k=atoi_func_firstline(token);
	k_temp=k;
	while(0==leggidabuffer(letturadainput,BUFFERSIZE)){
		if(letturadainput[0]=='A'){
			if(flag_big==1){	//se il flag big è a 1, fai crescere la lista indefinitamente, perché
								//con buona probabilità aggiungerai più in testa che in coda
				testa=accrescimento_ordinato(testa,AggiungiGrafo(d,k,i,&flag_big),i,0);
			}
			if(k_temp!=0 && flag_big==0){
				testa=accrescimento_ordinato(testa,AggiungiGrafo(d,k,i,&flag_big),i,0);
			}
			if(k_temp==0 && flag_big==0){
				testa=accrescimento_ordinato(testa,AggiungiGrafo(d,k,i,&flag_big),i,1);
			}
			i++;
			if(k_temp!=0){
				k_temp--;
			}
		}
		if(letturadainput[0]=='T'){
			if(testa!=NULL){
				printa_lista(testa,flag_big,k);
			}
			else{
			 putchar_unlocked((int)lettera);
			}
		}
	}
	return 0;
}
