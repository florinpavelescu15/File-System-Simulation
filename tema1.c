#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir
{
	char *name;
	struct Dir *parent;
	struct File *head_children_files;
	struct Dir *head_children_dirs;
	struct Dir *next;
} Dir;

typedef struct File
{
	char *name;
	struct Dir *parent;
	struct File *next;
} File;

// creez in directorul parent fisierul name
void touch(Dir *parent, char *name)
{
	File *p = parent->head_children_files;

	// daca parent contine fisire
	if (p != NULL)
	{
		// caut fisierul name
		while (p != NULL)
		{
			// daca il gasesc, afisez mesajul
			if (strcmp(p->name, name) == 0)
			{
				printf("File already exists\n");
				return;
			}
			p = p->next;
		}

		// daca nu, adaug fisierul name la finalul listei
		p = parent->head_children_files;
		while (p->next != NULL)
			p = p->next;

		File *new = malloc(sizeof(File));
		new->name = name;
		new->parent = parent;
		new->next = NULL;

		p->next = new;
	}
	else
	// daca parent nu contine fisiere
	{
		// pun noul fisier in varful listei de fisiere
		File *new = malloc(sizeof(File));
		new->name = name;
		new->parent = parent;
		new->next = NULL;

		parent->head_children_files = new;
	}
}

// creez in directorul parent directorul name
void mkdir(Dir *parent, char *name)
{
	Dir *p = parent->head_children_dirs;

	// daca parent contine directoare
	if (p != NULL)
	{
		// caut directorul name
		while (p != NULL)
		{
			// daca il gasesc, afisez mesajul
			if (strcmp(p->name, name) == 0)
			{
				printf("Directory already exists\n");
				return;
			}
			p = p->next;
		}

		// daca nu, adaug directorul la finalul listei
		p = parent->head_children_dirs;
		while (p->next != NULL)
			p = p->next;

		Dir *new = malloc(sizeof(Dir));
		new->name = name;
		new->parent = parent;
		new->head_children_files = NULL;
		new->head_children_dirs = NULL;
		new->next = NULL;

		p->next = new;
	}
	else
	// daca parent nu contine directoare
	{
		// pun noul director in varful listei de directoare
		Dir *new = malloc(sizeof(Dir));
		new->name = name;
		new->parent = parent;
		new->head_children_files = NULL;
		new->head_children_dirs = NULL;
		new->next = NULL;

		parent->head_children_dirs = new;
	}
}

// afisez continutul directorului parent
void ls(Dir *parent)
{
	File *p = parent->head_children_files;
	Dir *q = parent->head_children_dirs;

	if (p == NULL && q == NULL)
		printf("\n");

	// afisez numele directoarelor
	while (q != NULL)
	{
		printf("%s\n", q->name);
		q = q->next;
	}

	// afisez numele fisierelor
	while (p != NULL)
	{
		printf("%s\n", p->name);
		p = p->next;
	}
}

// sterg din directorul parent fisierul name
void rm(Dir *parent, char *name)
{
	File *p = parent->head_children_files, *t;
	int found = 0;

	// caut fisierul name
	while (p != NULL)
	{
		if (strcmp(p->name, name) == 0)
		{
			found = 1;
			break;
		}
		p = p->next;
	}

	// daca nu il gasesc, afisez mesajul
	if (found == 0)
	{
		printf("Could not find the file\n");
		return;
	}

	// daca il gasesc, il elimin din lista si eliberez memoria
	p = parent->head_children_files;

	// daca fisierul name se afla in capul listei de fisiere
	if (strcmp(p->name, name) == 0)
	{
		parent->head_children_files = parent->head_children_files->next;
		free(p);
	}
	else
	// daca fisierul name nu este in capul listei de fisiere
	{
		while (p != NULL)
		{
			if (strcmp(p->next->name, name) == 0)
			{
				t = p;
				p = p->next;
				break;
			}
			p = p->next;
		}

		t->next = p->next;
		free(p);
	}
}

// sterg toate fisierele din directorul parent
void free_file_list(Dir **parent)
{
	File *p = (*parent)->head_children_files;
	if (p == NULL)
		return;
	else
	{
		(*parent)->head_children_files = (*parent)->head_children_files->next;
		free(p);
		free_file_list(parent);
	}
}

// sterg directorul target cu toare subdirectoarele lui
void free_dir(Dir **target)
{
	if (*target == NULL)
		return;
	else
	{
		// sterg toate fisierele din target
		free_file_list(target);
		Dir *p = (*target)->head_children_dirs;

		// daca nu exista alte directoare in target, eliberez memoria
		if (p == NULL)
		{
			free(*target);
			return;
		}
		else
		/*daca exista directoare in target, sterg fiecare
		  director in parte apeland, recursiv, functia free_dir*/
		{
			(*target)->head_children_dirs = (*target)->head_children_dirs->next;
			free_dir(&p);
			free_dir(target);
		}
	}
}

// sterg din directorul parent directorul name
void rmdir(Dir *parent, char *name)
{
	Dir *p = parent->head_children_dirs, *t;
	int found = 0;

	// caut directorul name
	while (p != NULL)
	{
		if (strcmp(p->name, name) == 0)
		{
			found = 1;
			break;
		}
		p = p->next;
	}

	// daca nu il gasesc, afisez mesajul
	if (found == 0)
	{
		printf("Could not find the dir\n");
		return;
	}

	/*daca il gasesc, il elimin din lista; il sterg impreuna
	  cu toate subdirectoarele lui, flosind functia free_dir*/
	p = parent->head_children_dirs;

	// directorul name se afla in capul listei de directoare
	if (strcmp(p->name, name) == 0)
	{
		parent->head_children_dirs = parent->head_children_dirs->next;
		free_dir(&p);
	}
	else
	// directorul name nu se afla in capul listei de directoare
	{
		while (p != NULL)
		{
			if (strcmp(p->next->name, name) == 0)
			{
				t = p;
				p = p->next;
				break;
			}
			p = p->next;
		}

		t->next = p->next;
		free_dir(&p);
	}
}

// modific directorul target in name
void cd(Dir **target, char *name)
{
	// daca name este "..", merg in directorul parinte
	if (strcmp(name, "..") == 0)
	{
		if ((*target)->parent != NULL)
			(*target) = (*target)->parent;
	}
	else
	// altfel, caut directorul name
	{
		Dir *p = (*target)->head_children_dirs;
		while (p != NULL)
		{
			// daca il gasesc, schimb target in name
			if (strcmp(p->name, name) == 0)
			{
				(*target) = p;
				return;
			}
			p = p->next;
		}
		// daca nu il gasesc, afisez mesajul
		printf("No directories found!\n");
	}
}

// afisez calea absoluta spre directorul curent
void pwd(Dir *target)
{
	if (target == NULL)
		return;
	else
	{
		// apelez recursiv functia pwd
		pwd(target->parent);
		printf("/%s", target->name);
	}
}

// sterg directorul target
void stop(Dir *target)
{
	free_dir(&target);
}

/*afisez ierarhia de fisiere si directoare din target;
  afisarea se face sub forma de arbore*/
void tree(Dir *target, int level)
{
	// afisez numele directorului curent, cu spatierea ceruta
	for (int i = 0; i < level; i++)
		printf("    ");
	if (level >= 0)
		printf("%s\n", target->name);

	// afisez, recursiv, fiecare director din target
	Dir *q = target->head_children_dirs;
	while (q != NULL)
	{
		tree(q, level + 1);
		q = q->next;
	}

	// afisez numele fisierelor din target, cu spatierea ceruta
	File *p = target->head_children_files;
	while (p != NULL)
	{
		for (int i = 0; i <= level; i++)
			printf("    ");
		printf("%s\n", p->name);
		p = p->next;
	}
}

// schimb numele fisierului/directorului oldname in newname
void mv(Dir *parent, char *oldname, char *newname)
{
	File *p = parent->head_children_files;
	Dir *q = parent->head_children_dirs, *r;
	int found_file = 0, found_dir = 0;

	// caut fisierul/directorul oldname
	while (p != NULL)
	{
		if (strcmp(p->name, oldname) == 0)
		{
			found_file = 1;
			break;
		}
		p = p->next;
	}

	while (q != NULL)
	{
		if (strcmp(q->name, oldname) == 0)
		{
			found_dir = 1;
			break;
		}
		q = q->next;
	}

	// daca nu il gasesc, afisez mesajul
	if (found_file == 0 && found_dir == 0)
	{
		printf("File/Director not found\n");
		return;
	}

	/*daca il gasesc, caut fisierul/directorul newname;
	  daca exista deja in parent, afisez mesajul*/
	p = parent->head_children_files;
	while (p != NULL)
	{
		if (strcmp(p->name, newname) == 0)
		{
			printf("File/Director already exists\n");
			return;
		}
		p = p->next;
	}

	q = parent->head_children_dirs;
	while (q != NULL)
	{
		if (strcmp(q->name, newname) == 0)
		{
			printf("File/Director already exists\n");
			return;
		}
		q = q->next;
	}

	/*daca nu gasesc newname si oldname corespunde unui fisier,
	  sterg fisierul respectiv si creez fisierul newname*/
	if (found_file == 1)
	{
		rm(parent, oldname);
		touch(parent, newname);
	}

	/*daca nu gasesc newname si oldname corespunde unui director,
	  creez directorul newname cu toate atributele directorului oldname;
	  sterg directorul oldname*/
	if (found_dir == 1)
	{
		mkdir(parent, newname);
		q = parent->head_children_dirs;
		while (q->next != NULL)
		{
			if (strcmp(q->name, oldname) == 0)
				r = q;
			q = q->next;
		}
		q->head_children_files = q->head_children_files;
		q->head_children_dirs = r->head_children_dirs;
		rmdir(parent, oldname);
	}
}

int main()
{
	Dir *home = malloc(sizeof(Dir)), *curr;
	int i;
	char *cmd = malloc(10 * sizeof(char));
	char **args = malloc(100 * sizeof(char *));
	for (i = 0; i < 100; i++)
		args[i] = malloc(10 * sizeof(char));

	// creez directorul home (director radacina)
	home->name = "home";
	home->parent = NULL;
	home->head_children_files = NULL;
	home->head_children_dirs = NULL;
	home->next = NULL;

	// setez directorul home ca director curent
	curr = home;
	i = 0;

	do
	{
		// citesc comanda curenta
		scanf("%s", cmd);

		/*in functie de tipul comenzii, citesc argumentele
		  si apelez functiile corespunzatoare*/
		if (strcmp(cmd, "touch") == 0)
		{
			scanf("%s", args[i]);
			touch(curr, args[i]);
			i++;
		}

		if (strcmp(cmd, "mkdir") == 0)
		{
			scanf("%s", args[i]);
			mkdir(curr, args[i]);
			i++;
		}

		if (strcmp(cmd, "ls") == 0)
			ls(curr);

		if (strcmp(cmd, "rm") == 0)
		{
			scanf("%s", args[i]);
			rm(curr, args[i]);
			i++;
		}

		if (strcmp(cmd, "rmdir") == 0)
		{
			scanf("%s", args[i]);
			rmdir(curr, args[i]);
			i++;
		}

		if (strcmp(cmd, "cd") == 0)
		{
			scanf("%s", args[i]);
			cd(&curr, args[i]);
			i++;
		}

		if (strcmp(cmd, "tree") == 0)
			tree(curr, -1);

		if (strcmp(cmd, "pwd") == 0)
			pwd(curr);

		if (strcmp(cmd, "mv") == 0)
		{
			scanf("%s", args[i]);
			i++;
			scanf("%s", args[i]);
			mv(curr, args[i - 1], args[i]);
			i++;
		}

	} while (strcmp(cmd, "stop") != 0);
	// repet citirea pana la intalnirea comenzii stop

	// sterg directorul radacina
	stop(home);

	// eliberez memoria alocata vectorilor folositi la citirea comenzilor
	free(cmd);
	for (i = 0; i < 100; i++)
		free(args[i]);
	free(args);

	return 0;
}
