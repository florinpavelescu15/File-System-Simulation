Pavelescu Florin, grupa 324CC

Am implementat toate functiile necesare rezolvarii cerintelor temei, respectand, in mare parte, scheletul pus la dispozitie. 
Pentru unele functii, am modificat tipul returnat; de asemenea, am adaugat doua functii noi, dupa cum urmeaza:
- Functia `free_file_list` goleste lista de fisiere a unui director dat ca argument.
- Functia `free_dir` sterge recursiv un director cu toate fisierele si subdirectoarele acestuia. 
Ideea pe care am avut-o pentru functia `rmdir` se baza pe o implementare folosind recursivitatea,
dar antetul functiei `rmdir` nu era foarte potrivit pentru o astfel de implementare, din punctul meu de vedere.
Asadar, am implementat aceasta functie auxiliara, dandu-i o signatura potrivita pentru o implementare recursiva.
- Am modificat tipul returnat de functia `pwd`, din `char*` in `void`, pentru a putea folosi mai usor recursivitatea.
Detalii mai amanuntite despre implementare se gasesc in comentariile din fisierul sursa `tema1.c`.
