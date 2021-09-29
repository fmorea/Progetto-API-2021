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




