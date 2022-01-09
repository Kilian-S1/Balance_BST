/****************************************************************/
/* Hausarbeit Elektrotechnik, Labor Informatik II Hr. Kandziora	*/
/* Matrikelnummer: 9026481										*/
/* Ausbalancierender Binärer Baum								*/
/****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //für Funktion: strcat(), in ausgabe_mit_pfad()

struct Baum //Struktur für eine Element im Binären Baum
{
    int wert;
    struct Baum *links, *rechts;
};
typedef struct Baum baum; //einfacherer Aufruf: struct Baum vs. baum

void eingabe(baum **wurzel); //Vorlage
void ausgabe(baum *zgr); //Vorlage
/*Ausgabe des baumes mit Pfad zu jedem Element*/
void ausgabe_mit_pfad(baum *zgr, char *path);
/*Funktion zum balacieren des Baumes. Rechtsdrehung zweier Knoten*/
baum* spin_right(baum* y, baum* x);
/*Funktion zum balacieren des Baumes. Linksdrehung zweier Knoten */
baum* spin_left(baum* y, baum* x);
/*Funktion welche das ausbalcieren vornimmt */
baum* balance(baum *wurzel);
/* Funktion um die Höhe eines Elementes zu bestimmen */
int hoehe(baum *lb);
int max(int a, int b); //Rückgabe des größeren Wertes

int main()
{
    struct Baum *wurzel = NULL; //Baum erstellen
    char *path; //String anlegen für Fkt. ausgabe_mit_pfad()
    path = (char*)malloc(sizeof(char)); 
    path[0] = '\0';
    eingabe(&wurzel); //Eingabe von Werten des Benutzers in den Baum.
    printf("vor balancing:\nAusgabe Baum: ");
    ausgabe(wurzel); //Ausgabe der Elemente des Baumes
    printf("\n");
    ausgabe_mit_pfad(wurzel, path); //Ausgabe der Elemente mit Pfad
    wurzel = balance(wurzel); //Ausbalancieren des Baumes
    printf("\nnach balancing:\nAusgabe Baum: ");
    path = (char*)malloc(sizeof(char)); //reinitialisierung vom path 
//string da dieser bei der letzten ausgabe mit pfad gelöscht worden ist
    path[0] = '\0';
    ausgabe(wurzel);//Ausgabe der Elemente des ausbalancierten Baumes
    printf("\n");
    ausgabe_mit_pfad(wurzel, path); //Ausgabe der Elemente mit Pfad
    return 0;
}

void eingabe(baum **wurzel) //Eingabefunktion original Vorlage
{
    int wert;
    baum *zgr, *lz;

    do
    {
        printf("Eingabe: ");
        scanf("%d", &wert);
        if (wert != 0)
        {
            zgr = (baum*)malloc(sizeof(baum));
            zgr->wert = wert;
            zgr->links = zgr->rechts = NULL;
            if (*wurzel == NULL) *wurzel = zgr;
            else
            {
                lz = *wurzel;
                while (lz != NULL)
                {
                    if(wert < lz->wert)
                    {
                    	if(lz->links == NULL)
                    	{
                    		lz->links = zgr;
                    		lz = NULL;
                    	}
                    	else lz = lz ->links;
                    }
                    else
                    {
                    	if(lz->rechts == NULL)
                    	{
                    		lz->rechts = zgr;
                    		lz = NULL;
                    	}
                    	else lz = lz-> rechts;
                    }
                }
            }
         }
    } while (wert != 0);
}

void ausgabe_mit_pfad(baum *zgr, char *path){
	if(zgr->links != NULL){
/*Der Pfad zum Element wird in einem String gespeichert. Dafür wird für
 * jeden rekursiven Funktionsaufruf ein neuer String erstellt mit +1
 * Platz für ein neuen Wegbeschreibenden Character (L/R) */
		char *path_cpy = (char*)malloc(sizeof(char)*(strlen(path)+1));
		strcpy(path_cpy, path);
/*Anhängen von "L" an die Kopie des strings */
		strcat(path_cpy, "L");
/* Rekursiver Aufruf der Funktion mit aktualisiertem, kopiertem Pfad*/
		ausgabe_mit_pfad(zgr->links, path_cpy);
	}
	if(path[0] == '\0'){ //Wurzel finden und mit (w) markieren.
		printf("%d (w)   ", zgr->wert);
	}
	else{ //Ausgabe von Wert und Pfad des Elements
		printf("%d (%s)   ", zgr->wert, path);
	}
	if(zgr->rechts != NULL){//Analog zum obigen vorgehen, nun für Rechts
		char *path_cpy = (char*)malloc(sizeof(char)*(strlen(path)+1));
		strcpy(path_cpy, path);
		strcat(path_cpy, "R");
		ausgabe_mit_pfad(zgr->rechts, path_cpy);
	}
	free(path); //Strings am Ende wieder freigeben
}

baum* spin_right(baum* y, baum* x){ //Rechtsdrehung von y und x
/* Vertauschen der Zeiger um Rangfolge im Baum zu ändern */
	x->links = y->rechts;
	y->rechts = x; //Zuvor war x über y. Jetzt ist y über x.
	return y;
}

baum* spin_left(baum* y, baum* x){ //Linksdrehung von y und x
	x->rechts = y->links; //Funktionsweise analog zur Rechtsdrehung
	y->links = x;
	return y;
}

baum* balance(baum *wurzel){
	int hoehe_links;
	int hoehe_rechts;	
	int aktuelle_diff;//Speicher der Differenz von Höhe Links und Rechts
	do{
		hoehe_links = hoehe(wurzel->links);
		hoehe_rechts = hoehe(wurzel->rechts);
		aktuelle_diff = hoehe_links - hoehe_rechts;
/*Wenn aktuelle diff kleiner als -1 ist, ist der Baum rechts höher */
		if(aktuelle_diff < -1){
/*Ein Baum der rechts höher ist, wird durch linksdrehung balanciert
 *ggf. ist vorher jedoch eine Linksdrehung notwendig (Links-Rechts)*/
			if(hoehe(wurzel->rechts->links) >
			hoehe(wurzel->rechts->rechts)){
				wurzel->rechts =
				spin_right(wurzel->rechts->links, wurzel->rechts);
			}
			wurzel = spin_left(wurzel->rechts, wurzel);
		}
/* Analoges Vorgehen wie zuvor, jetzt für den Fall das Links höher ist*/
		else if(aktuelle_diff > 1){
			if(hoehe(wurzel->links->rechts) >
			hoehe(wurzel->links->links)){
				wurzel->links =
				spin_left(wurzel->links->rechts, wurzel->links);
			}
			wurzel = spin_right(wurzel->links, wurzel);
		}
/*Solange wiederholen wie die aktuelle Wurzel nicht balaciert ist*/
	}while(aktuelle_diff > 1 || aktuelle_diff < -1);
/*Wenn die Wurzel ausbalaciert ist werden rekursiv die Teilbäume links-
 * und rechts ausbalciert. Die neue Wurzel wird dabei gespeichert */
	if(wurzel->links != NULL) wurzel->links = balance(wurzel->links);
	if(wurzel->rechts != NULL) wurzel->rechts = balance(wurzel->rechts);
	return wurzel;
}

int hoehe(baum *lb){
/* Höhe bestimmen durch suchen des längsten Wegs zum "Boden", rekursiv*/
	if (lb != NULL) return 1 + max(hoehe(lb->links), hoehe(lb->rechts));
	else return 0;
}

int max(int a, int b){ //Rückgabe des größeren Elementes
	return a > b ? a : b;
}

void ausgabe(baum *zgr) //Ausgabefunktion Original Vorlage
{
	if(zgr->links != NULL) ausgabe(zgr->links);
	printf("%d   ", zgr->wert);
	if(zgr->rechts !=NULL) ausgabe(zgr->rechts);
}
