Phase 1 :

Am folosit Gemnini ca si AI ajutator.



Promptul pe care i l-am dat pentru a genera functia de pare a fost : Am o structura în C numita report care contine campurile: report\_ID, inspector\_Name, x, y, issue\_Category, severity\_Level, timestamp și description\_Text. Scrie o functie int parse\_condition(const char \*input, char \*field, char \*op, char \*value) care primeste un string de tipul field:operator:value si il imparte in cele trei componente.



Promptul pe care i l-am dat pentru a genera functia de match a fost : Avand structura report definită anterior, scrie o functie int match\_condition(report \*r, const char \*field, const char \*op, const char \*value) care verifica daca recordul r indeplineste conditia data prin field, operator si valoare. Suporta campurile: severity, category, inspector, timestamp. Operatorii suportati sunt: ==, !=, <, <=, >, >=.



Initial, AI-ul a incercat sa foloseasca operatorii > si < direct pe string-uri. A trebuit sa il corectez si sa inlocuiesc operatorii < si > cu strcmp.



Am inteles cum functioneaza seturile de caractere în sscanf pentru a parsa string-uri complexe.

Am intels importanta conversiei datelor dintr-un argument din linia de comanda, in tipul de date necesar.



Phase 2 :

Am folosit Gemini ca si AI ajutator.



Am folosit AI-ul pentru a ma ajuta sa inteleg mai bine cum functioneaza semnalele. Am inteles scopul si folosirea variabilei de tip volatile sig\_atomic\_t, a structului sigaction, dar si a functiilor pause() si getpid(). Totodata acesta m-a ajutat la intelegerea functiei kill().



Phase 3 :

Am folosit Gemini ca si AI ajutator.



Am folosit AI-ul pentru a ma ajuta la intelegerea si aprofundarea functiei fork(). Totodata acesta m-a ajutat la intelegerea folosirii folosirii pipe-urilor, dar si a functiilor execlp() si dup2().



