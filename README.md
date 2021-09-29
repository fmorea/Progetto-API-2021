# A dijkstra's algorithm reimplementation
# Prova Finale di Algoritmi e principi dell'informatica - a.a. 2020-2021 - Politecnico di Milano 
# Fernando Morea 

Nel contesto della prova d'esame finale dell'esame di algoritmi e principi dell'informatica si chiedeva 
di reimplementare il noto algoritmo di shortest path finding dijkstra per stilare una classifica dei
migliori grafi tra quelli dati.

I grafi da classificare erano forniti in un file di testo, sotto forma di matrice di adiacenza.
Il programma doveva leggere il file di testo da stdin, calcolare il coefficiente di costo della matrice ed
inserire in una classifica temporanea (la cui struttura dati era da definire), infine una volta aver calcolato 
il costo per tutte le matrici doveva stampare su stdout tutti gli indici (ordine di inserimento) delle k matrici
con coefficiente di costo più piccolo.

Per visionare gli altri dettagli progettuali richiesti, viene fornito il pdf con i requisiti.

# Dettagli implementativi
Come struttura dati per memorizzare la classifica ho scelto una lista dinamica.
Seppure una lista non sia particolarmente efficiente in termini di complessità temporale, ho preferito
utilizzare una struttura dati semplice sia dal punto di vista implementativo che di manutenzione (rispetto ad
altre soluzioni come alberi o heap).

# Versioni ed ottimizzazioni
Per superare i test automatizzati erogati dalla piattaforma, si sono rese necessarie molte ottimizzazioni successive,
tra cui la sostituzione di molte funzioni della libreria standard del C, tra le quali atoi e sscanf.
In sostanza la parte da ottimizzare riguardava il parsing dell'input.
Si consiglia di fare riverimento alla versione contenuta nel file sorgente Open_v4.c per capire il funzionamento del programma,
siccome dalla versione 4 alla versione 11 strutturalmente il programma non è cambiato, ed anzi risulta meno leggibile.

# Qualche riflessione
Dover sacrificare leggibilità del codice, semplicità delle strutture dati per ottimizzare la gestione della memoria e delle 
risorse è stata una scelta quasi obbligata da questo tipo di competizione e dal tempo a disposizione.




