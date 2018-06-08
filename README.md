# Scheduler-Simulator-Project
A project for the exam of Operating Systems.
Se serve cominciamo a scrivere qui quello che metteremo nel report! :D

i file eseguibili li mettiamo con estensione .out

Cose del forum da ricordare

Q: Supponiamo che un task abbia un'istruzione bloccante come ultima istruzione. Una volta aspettato che si sblocchi, possiamo direttamente mandarlo in "exit"?
A: No, bisogna loggare e scrivere sul file output che il task, seppur terminato, deve prima passare allo stato "ready" e allo stato "running"
 
Q: Supponiamo che nella ready list (coda dei task ready) abbiamo N processi di cui i primi 10 con la caratteristica di avere la prossima istruzione, quella puntata dal PC, di tipo bloccante (type 1). Possiamo, ALL'INTERNO DELLO STESSO CLOCK, mettere in "running", e poi "blocked", tutti i 10 task uno dopo l'altro, finché non ne trovo uno con istruzione non bloccante (tipo 0)? Per capirci, si può fare come l'esempio seguente (?) :
Core ,Clock ,Task_id, Task_state
CoreX,ClockY,10,running
CoreX,ClockY,10,blocked
CoreX,ClockY,11,running
CoreX,ClockY,11,blocked
CoreX,ClockY,12,running
CoreX,ClockY,12,blocked
CoreX,ClockY,13,running
A: Sì, si può fare anche ALL'INTERNO DELLO STESSO CLOCK, a patto che vengano loggati i passaggi di stato

https://dibris.aulaweb.unige.it/mod/forum/discuss.php?d=14595 questa se riceve risposta
