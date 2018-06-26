# Scheduler-Simulator-Project
A project for the exam of Operating Systems.
Se serve cominciamo a scrivere qui quello che metteremo nel report! :D

I file eseguibili li mettiamo con estensione .out

* Algoritmo not preemptive: SPN
* Algoritmo preemptive: SRT

## TO-DO

* Inserimenti ed estrazioni da coda READY (heap o inserimenti ordinati)
* Inserimenti coda BLOCKED (in coda/in testa) -> moveTask riceve anche il core
* Estrazione coda BLOCKED (coda/testa)
* Prossimo processo (scheduling: messa in RUNNING, controllo altre code -> estrarre quelli pronti)
* Esecuzione istruzioni (incremento clock)
* Mettere in BLOCKED/EXIT
* Gestione fine programma (liberare memoria, restituire controllo, chiusura file)
* Mutex
* Fine task

## Domande/dubbi

* bisogna verificare se si possono fare tutti gli spostamenti nello stesso ciclo di ck o se non va bene
* se vogliamo mettere il min heap dobbiamo utilizzare al posto di prev e next figlio dx e figlio sx
* corner case: un processo ha come ultima istruzione un'istruzione bloccante, controllare com'è gestito
* in un singolo ciclo di ck uno stato può passare in running e andare direttamente in blocked, prima di prendere il
prossimo processo è meglio aumentare di uno il ck

## Log delle cose fatte su firstbranch
    
* Apertura fw_np in processo
* Passaggio struct a thread e dichiarazione di alcune variabili utili al thread per scheduling
* enum core_t, in task_t il campo core e wait_time per indicare da quale processo è stato bloccato e per quanto tempo
* modificata la funzione moveTask aggiungendo da quale coda a quale altra viene spostato
* utilizzo mutex nella funzione del thread
* avanzamento di task, manca la funzione random per attesa
* dopo esecuzione il task è mandato in blocked o exit
* funzione log per stampare transizioni

## _Cose del forum da ricordare_

Q: Supponiamo che un task abbia un'istruzione bloccante come ultima istruzione. Una volta aspettato che si sblocchi, possiamo direttamente mandarlo in "exit"?

A: No, bisogna loggare e scrivere sul file output che il task, seppur terminato, deve prima passare allo stato "ready" e allo stato "running"
 
Q: Supponiamo che nella ready list (coda dei task ready) abbiamo N processi di cui i primi 10 con la caratteristica di avere la prossima istruzione, quella puntata dal PC, di tipo bloccante (type 1). Possiamo, ALL'INTERNO DELLO STESSO CLOCK, mettere in "running", e poi "blocked", tutti i 10 task uno dopo l'altro, finché non ne trovo uno con istruzione non bloccante (tipo 0)? Per capirci, si può fare come l'esempio seguente (?) :

Core ,Clock ,Task_id, Task_state\
CoreX,ClockY,10,running\
CoreX,ClockY,10,blocked\
CoreX,ClockY,11,running\
CoreX,ClockY,11,blocked\
CoreX,ClockY,12,running\
CoreX,ClockY,12,blocked\
CoreX,ClockY,13,running

A: Sì, si può fare anche ALL'INTERNO DELLO STESSO CLOCK, a patto che vengano loggati i passaggi di stato

https://dibris.aulaweb.unige.it/mod/forum/discuss.php?d=14595 questa se riceve risposta
