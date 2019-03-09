#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXPOS 30
#define MAXNAME 20
#define MAXSUR 30
#define MAXTITLE 30
#define MAXTECH 20
#define MAXFOLD 20
typedef enum {false = 0,true = 1} bool;

typedef struct position
{
    char name[MAXNAME];
    char surname[MAXSUR];
    char title[MAXTITLE];
    char style[MAXTECH];
    int year;
    struct position * next;
    struct position * prev;
    struct folder * parent;
} Position;

typedef struct folder
{
    char title[MAXFOLD];
    struct folder * sibling;
    struct folder * psibling;
    struct folder * fchild;
    struct folder * parent;
    struct picture * child;
} Folder;

typedef struct tree
{
    Folder * root;
    int number;
} Tree;

void AddFolder (Tree *itree);
void AddPicture(Tree * wd);
void AddToTree(Position * wp,Tree * wd);
void Bin(Folder * wf,FILE * pl);
void Binary(Tree * itree);
void DeleteEverything(Tree * itree);
void DeleteF(Folder * wp,Tree *itree);
void DeleteFolder(Tree * itree);
void DeletePicture(Tree * wd);
void DeletePosition(Position * wp, Tree * itree);
bool EmptyTree(const Tree *itree);
bool FullTree (const Tree *itree);
char * load(char *z,int how);
char menu (void);
void ModifyPicture(Tree * wd);
void ModifyFolder (Tree * wd);
void move(Tree * itree);
void ShowP(Tree * itree);
void ShowSurname(Folder * wf,char * str);
void ShowT(Folder * wf);
void ShowTech(Folder * wf,char * str);
void ShowTitle(Folder * wf,char * str);
void ShowY(Folder *wf,int v);
void TexFile(Tree * itree);
void Tex(Folder * wf,FILE * pl);
void TreeInitialisation(Tree *itree);
Position * FindPos(Position * wp,Folder * wf);
Folder * FindFol(Folder * wf,Folder * wd);
Folder* WhereToPut(Tree * itree);

int main()//pokazuje mozliwosci
{
    Tree pict;
    char choice;
    TreeInitialisation(&pict);
    while((choice = menu())!='z')
    {
        switch (choice)
        {
        case 'a':
            AddFolder(&pict);
            break;
        case 'b':
            AddPicture(&pict);
            break;
        case 'c':
            ModifyFolder(&pict);
            break;
        case 'd':
            DeleteFolder(&pict);
            break;
        case 'e':
            DeletePicture(&pict);
            break;
        case 'f':
            ModifyPicture(&pict);
            break;
        case 'g':
            ShowT(pict.root);
            break;
        case 'h':
            ShowP(&pict);
            break;
        case 'i':
            move(&pict);
            break;
        case 'j':
            Binary(&pict);
            break;
        case 'k':
            TexFile(&pict);
            break;
        default :
            puts("There is not such an option");
        }
    }
    DeleteEverything(&pict);
    puts("Bye!");
    return 0;
}
void TreeInitialisation(Tree *itree)//inicjalizuje drzewo
{
    itree->root = NULL;
    itree->number = 0;
}
bool EmptyTree(const Tree *itree)//czy drzewo jest puste
{
    if (itree->root == NULL)
        return true;
    else
        return false;
}
bool FullTree (const Tree *itree)//czy drzewo jest pelne
{
    if (itree->number == MAXPOS)
        return true;
    else
        return false;
}
void AddFolder (Tree *itree)//dodanie folderu
{
    Folder * news;
    Folder * after;
    char choice;
    if (FullTree(itree))
    {
        fprintf(stderr,"Tree is full\n");
        return;
    }
    news= malloc(sizeof(Folder));
    if (EmptyTree(itree))
    {
        itree->root=news;
        news->sibling=NULL;
        news->child=NULL;
        news->fchild=NULL;
        news->parent=NULL;
        news->psibling=NULL;
        puts("Type in the folder's name:");
        load(news->title,MAXFOLD);
        itree->number++;
    }
    else
    {
        puts("Type in the folder's name:");
        load(news->title,MAXFOLD);
        if(FindFol(news,itree->root)!= NULL)
        {
            puts("There is such a folder");
            free(news);
            return;
        }
        else
        {
            after = WhereToPut(itree);
            if(after!=NULL)
            {
                puts("Do you want it to be a son or a sibling?");
                puts("Type s if son or b if sibling");
                choice=getchar();
                switch (choice)
                {
                case 'b':
                    news->sibling=after->sibling;
                    if(after->sibling!=NULL)
                        after->sibling->psibling=news;
                    after->sibling=news;
                    news->psibling=after;
                    news->child=NULL;
                    news->parent=NULL;
                    news->fchild=NULL;
                    break;
                case 's':
                    news->parent=after;
                    if(after->fchild!=NULL)
                        after->fchild->parent=news;
                    news->fchild=after->fchild;
                    after->fchild=news;
                    news->child=NULL;
                    news->sibling=NULL;
                    news->psibling=NULL;
                    break;
                }
                itree->number++;
            }
            else
            {
                puts("Such folder does not exist");
                free(news);
                return;
            }
        }
    }
}
Folder* WhereToPut(Tree * itree)//gdzie wstawic folder
{
    Folder * temp;
    Folder * find;
    temp=malloc(sizeof(Folder));
    puts("After which folder would you like to put a folder?");
    puts("Type in the title of folder:");
    load(temp->title,MAXFOLD);
    find=FindFol(temp,itree->root);
    free (temp);
    return find;
}
char menu (void)//dokonywanie wyboru z menu
{
    int ch;
    puts("a)add folder    b)add position\n");
    puts("c)change folder's name\n");
    puts("d)delete folder e)delete position\n");
    puts("f)modify position\n");
    puts("g)show topology\n");
    puts("h)show by parameters\n");
    puts("i)move to other place\n");
    puts("j)save to binary file\n");
    puts("k)save to text file\n");
    puts("z)exit\n");
    while ((ch=getchar())!= EOF)
    {
        while (getchar()!= '\n')
            continue;
        ch=tolower(ch);
        if(strchr("abcdefghijkz",ch)==NULL)
            puts("Type a,b,c,d,e,f,g,h,i,j,k or z:");
        else
            break;
    }
    if(ch==EOF)
        ch='z';
    return ch;
}
void AddPicture(Tree * itree)//dodanie pozycji obrazu
{
    Position * temp;
    Position * wsk;
    int c;
    if (FullTree(itree))
        puts("There is no space left");
    if (EmptyTree(itree))
        puts("You have to create a folder first");
    else
    {
        temp=(Position*)malloc(sizeof(Position));
        if (temp != NULL)
        {
            puts("Name of the painter:");
            load(temp->name,MAXNAME);
            puts("Surname of the painter:");
            load(temp->surname,MAXSUR);
            puts("Title:");
            load(temp->title,MAXTITLE);
            puts("Painting technique:");
            load(temp->style,MAXTECH);
            puts("Year:");
            scanf("%d",&temp->year);
            while((c = getchar()) != '\n' && c != EOF);
            temp->next=NULL;
            temp->prev=NULL;
            wsk=FindPos(temp,itree->root);
            if (wsk!=NULL)
            {
                fprintf(stderr,"There is such a position\n");
                free(temp);
                return;
            }
        }
        else
        {
            fprintf(stderr,"Can't create a position\n");
            free (temp);
            return;
        }
        itree->number++;
        AddToTree(temp, itree);
    }
}
void AddToTree (Position * wp,Tree * itree)//wstawia pozycje do drzewa
{
    Folder * find;
    Folder * tempr;
    Position * what;
    tempr=malloc(sizeof(Folder));
    puts("In which folder would you like to create a position?");
    puts("Type in the folder's title:");
    load(tempr->title,MAXFOLD);
    find=FindFol(tempr,itree->root);
    free(tempr);
    if (find!=NULL)
    {
        if(find->child!=NULL)
        {
            what=find->child;
            what->parent=NULL;
            what->prev=wp;
            wp->next=what;
        }
        wp->parent=find;
        find->child=wp;
        wp->prev=NULL;
    }
    else
        puts("That folder does not exist");
}
Folder * FindFol(Folder * wf,Folder * start)//wyszukuje dany folder zwraca NULL gdy nie znalazl
{
    Folder * szuk;
    szuk=start;
    if (szuk==NULL)
        return NULL;
    if (strcmp(wf->title,szuk->title)==0)
        return szuk;
    else
    {
        szuk=FindFol(wf,start->fchild);
        if(szuk!=NULL)
            return szuk;
        szuk=FindFol(wf,start->sibling);
        if(szuk!=NULL)
            return szuk;
    }
    return NULL;
}
char * load(char *z,int how)//wczytywanie wyrazow
{
    char * sum;
    char * here;
    sum = fgets(z,how,stdin);
    if (sum)
    {
        here = strchr(z,'\n');
        if(here)
            *here='\0';
        else
            while (getchar()!= '\n')
                continue;
    }
    return sum;
}
void ModifyPicture(Tree * itree)//zmiana zawartosci pozycji
{
    int c;
    if(EmptyTree(itree))
    {
        puts("The tree is empty");
        return;
    }
    Position * find;
    Position * temp;
    temp=malloc(sizeof(Position));
    puts("Which position would you like to modify?");
    puts("Type painter's name:");
    load(temp->name,MAXNAME);
    puts("Type painter's surname:");
    load(temp->surname,MAXSUR);
    puts("Type the title:");
    load(temp->title,MAXTITLE);
    puts("Type the technique:");
    load(temp->style,MAXTECH);
    puts("Type the year:");
    scanf("%d",&temp->year);
    while((c = getchar()) != '\n' && c != EOF);
    find=FindPos(temp,itree->root);
    free(temp);
    if (find!=NULL)
    {
        puts("New painter's name:");
        load(find->name,MAXNAME);
        puts("New author's surname:");
        load(find->surname,MAXSUR);
        puts("New title:");
        load(find->title,MAXTITLE);
        puts("New technique:");
        load(find->style,MAXTECH);
        puts("New year:");
        scanf("%d",&find->year);
        while((c = getchar()) != '\n' && c != EOF);
    }
    else
        puts("There is not such a position");
}
void ModifyFolder (Tree * itree)//zmiana tytulu folderu
{
    if(EmptyTree(itree))
    {
        puts("The tree is empty");
        return;
    }

    Folder * wf;
    Folder * find;
    Folder * second;
    wf=malloc(sizeof(Folder));
    puts("Which folder's name would you like to modify?");
    load(wf->title,MAXFOLD);
    find=FindFol(wf,itree->root);
    if(find!=NULL)
    {
        puts("Type in the new title:");
        load(wf->title,MAXFOLD);
        second=FindFol(wf,itree->root);
        free(wf);
        if (second==NULL)
        {
            puts("Type in the title for the second time:");
            load(find->title,MAXFOLD);
        }
        else
        {
            puts("There is a folder with such title");
            return;
        }
    }
    else
        puts("There is no such a folder");

}
void DeletePicture(Tree * itree)//pyta ktora pozycje usunac
{
    int c;
    Position *temp;
    Position *del;
    temp=malloc(sizeof(Position));
    if(EmptyTree(itree))
    {
        puts("The tree is empty");
        return;
    }
    puts("Name of the painter:");
    load(temp->name,MAXNAME);
    puts("Surname of the painter:");
    load(temp->surname,MAXSUR);
    puts("Title:");
    load(temp->title,MAXTITLE);
    puts("Painting technique:");
    load(temp->style,MAXTECH);
    puts("Year:");
    scanf("%d",&temp->year);
    while((c = getchar()) != '\n' && c != EOF);
    del=FindPos(temp,itree->root);
    free(temp);
    if (del!=NULL)
    {
        DeletePosition(del,itree);
        puts("The position has been deleted");
    }
    else
        puts("The position does not exist");
}
Position * FindPos(Position * wp,Folder * wf)//wyszukuje dana pozycje zwraca NULL gdy nie znalazl
{
    Position * szuk;
    szuk=wf->child;
    if(szuk==NULL)
    {
        return NULL;
    }
    if ((strcmp(szuk->name,wp->name)==0)&&(strcmp(szuk->surname,wp->surname)==0)&&(szuk->year==wp->year)&&(strcmp(szuk->style,wp->style)==0)&&(strcmp(szuk->title,wp->title)==0))
        return szuk;
    else
    {
        for(szuk=szuk->next; szuk!=NULL; szuk=szuk->next)
        {
            if ((strcmp(szuk->name,wp->name)==0)&&(strcmp(szuk->surname,wp->surname)==0)&&(szuk->year==wp->year)&&(strcmp(szuk->style,wp->style)==0)&&(strcmp(szuk->title,wp->title)==0))
                return szuk;
        }
        if(wf->fchild!=NULL)
            szuk=FindPos(wp,wf->fchild);
        if (szuk!=NULL)
            return szuk;
        if(wf->sibling!=NULL)
            szuk=FindPos(wp,wf->sibling);
        if (szuk!=NULL)
            return szuk;
    }
    return NULL;
}
void DeletePosition(Position * wp, Tree * itree)//usuwanie pozycji
{
    if(wp->prev!=NULL)
        wp->prev->next=wp->next;
    if(wp->next!=NULL)
        wp->next->prev=wp->prev;
    if(wp->parent!=NULL)
    {
        wp->parent->child=wp->next;
        if(wp->next!=NULL)
            wp->next->parent=wp->parent;
        puts("tu");
    }
    free(wp);
    itree->number--;
}
void DeleteFolder(Tree * itree)//pyta ktory folder usunac
{
    if(EmptyTree(itree))
    {
        puts("The tree is empty");
        return;
    }
    Folder * wf;
    Folder * del;
    wf=malloc(sizeof(Folder));
    puts("Which folder would you like to delete? Type the name:");
    load(wf->title,MAXFOLD);
    del=FindFol(wf,itree->root);
    free (wf);
    if (del!=NULL)
        DeleteF(del,itree);
    else
        puts("Such Folder does not exist");
}
void DeleteF(Folder * wp,Tree *itree)//usuwanie folderu
{
    Position *bro;
    Position *temp;
    bro=wp->child;
    while(bro!=NULL)
    {
        temp=bro->next;
        DeletePosition(bro,itree);
        bro=temp;
    }
    if(wp->parent!=NULL)
        {
            wp->parent->fchild=wp->sibling;
            if(wp->sibling!=NULL)
            wp->sibling->parent=wp->parent;
        }
    if(wp->sibling!=NULL)
        wp->sibling->psibling=wp->psibling;
    if(wp->psibling!=NULL)
        wp->psibling->sibling=wp->sibling;
    if(wp->fchild!=NULL)
        DeleteF(wp->fchild,itree);
    if(wp->sibling!=NULL)
        DeleteF(wp->sibling,itree);
    if(wp==itree->root)
    itree->root=NULL;
    free(wp);
    itree->number--;
}
void DeleteEverything(Tree * itree)//usuwa wszystko
{
    if(!EmptyTree(itree))
        DeleteF(itree->root,itree);
}
void ShowT(Folder * wf)//pokazuje wszystkie foldery w kolejnosci dzieci, rodzenstwo
{
    if(wf==NULL)
        return;
    printf("%s\n",wf->title);
    ShowT(wf->fchild);
    ShowT(wf->sibling);
}
void ShowP(Tree * itree)//pyta o parametr wyszukiwania
{
    int c;
    Folder * temp;
    Folder * find;
    Position * para;
    char ch;
    para=malloc(sizeof(Position));
    temp=malloc(sizeof(Folder));
    puts("Which folder's inside do you want to see?");
    puts("Type in the title:");
    load(temp->title,MAXFOLD);
    find=FindFol(temp,itree->root);
    free(temp);
    if (find==NULL)
        puts("Such folder does not exist");
    else
    {
        puts("Choose a parameter:");
        puts("a)title     b)technique");
        puts("c)painter   d)year");
        ch=getchar();
        while((c = getchar()) != '\n' && c != EOF);
        switch(ch)
        {
        case'a':
            puts("Type in the title:");
            load(para->title,MAXTITLE);
            ShowTitle(find,para->title);
            break;
        case'b':
            puts("Type in the technique:");
            load(para->style,MAXTECH);
            ShowTech(find,para->style);
            break;
        case'c':
            puts("Type in painter's surname:");
            load(para->surname,MAXSUR);
            ShowSurname(find,para->surname);
            break;
        case'd':
            puts("type in the year:");
            scanf("%d",&(para->year));
            while((c = getchar()) != '\n' && c != EOF);
            ShowY(find,para->year);
            break;
        }
    }
    free(para);
}
void ShowSurname(Folder * wf,char * str)//wyswietla pozycje o danym nazwisku
{
    Position * temp;
    for(temp=wf->child; temp!=NULL; temp=temp->next)
        if(strcmp(temp->surname,str)==0)
        {
            puts("");
            printf("%s\n%s\n%s\n%s\n%d\n",temp->name,temp->surname,temp->title,temp->style,temp->year);
        }
    if(wf->fchild!=NULL)
        ShowSurname(wf->fchild,str);
    if(wf->sibling!=NULL)
        ShowSurname(wf->sibling,str);
}
void ShowTitle(Folder * wf,char * str)//wyswietla pozycje o danym tytule
{
    Position * temp;
    for(temp=wf->child; temp!=NULL; temp=temp->next)
        if(strcmp(temp->title,str)==0)
        {
            puts("");
            printf("%s\n%s\n%s\n%s\n%d\n",temp->name,temp->surname,temp->title,temp->style,temp->year);
        }
    if(wf->fchild!=NULL)
        ShowTitle(wf->fchild,str);
    if(wf->sibling!=NULL)
        ShowTitle(wf->sibling,str);
}
void ShowTech(Folder * wf,char * str)//wyswietla pozycje o danej technice
{
    Position * temp;
    for(temp=wf->child; temp!=NULL; temp=temp->next)
        if(strcmp(temp->style,str)==0)
        {
            puts("");
            printf("%s\n%s\n%s\n%s\n%d\n",temp->name,temp->surname,temp->title,temp->style,temp->year);
        }
    if(wf->fchild!=NULL)
        ShowTech(wf->fchild,str);
    if(wf->sibling!=NULL)
        ShowTech(wf->sibling,str);
}
void ShowY(Folder * wf,int v)//wyswietla pozycje o danym roku
{
    Position * temp;
    for(temp=wf->child; temp!=NULL; temp=temp->next)
        if(temp->year==v)
        {
            puts("");
            printf("%s\n%s\n%s\n%s\n%d\n",temp->name,temp->surname,temp->title,temp->style,temp->year);
        }
    if(wf->fchild!=NULL)
        ShowY(wf->fchild,v);
    if(wf->sibling!=NULL)
        ShowY(wf->sibling,v);
}
void move(Tree * itree)//pozwala na przeniesienie folderu
{
    Folder * temp;
    Folder * find;
    Folder * after;
    Folder * szuk;
    char choice;
    temp=malloc(sizeof(Folder));
    puts("Which folder would you like to move?");
    puts("Type in the title:");
    load(temp->title,MAXFOLD);
    find=FindFol(temp,itree->root);
    free(temp);
    if(find==NULL)
    {
        puts("Such folder does not exist");
        return;
    }
    else
    {
        if(find==itree->root)
        {
            puts("You can't move the root of a tree.");
            return;
        }
        after=WhereToPut(itree);
        if(after!=NULL)
        {
            puts("Dou you want it to be a son or a sibling?");
            puts("Type s if son or b if sibling");
            choice=getchar();
            switch (choice)
            {
            case 'b':
                szuk=find;
                while(szuk->sibling!=NULL)
                {
                    szuk=szuk->sibling;
                }
                if(find->psibling!=NULL)
                    find->psibling->sibling=NULL;
                find->psibling=after;
                szuk->sibling=after->sibling;
                after->sibling=find;
                if(find->parent!=NULL)
                {
                    temp=find->parent;
                    temp->fchild=NULL;
                    find->parent=NULL;
                }
                break;
            case 's':
                szuk=find;
                while(szuk->fchild!=NULL)
                {
                    szuk=szuk->fchild;
                }
                if(find->psibling!=NULL)
                    find->psibling->sibling=NULL;
                find->psibling=NULL;
                szuk->fchild=after->fchild;
                if(after->fchild!=NULL)
                    after->fchild->parent=find;
                after->fchild=find;
                if(find->parent!=NULL)
                {
                    temp=find->parent;
                    temp->fchild=NULL;
                }
                find->parent=after;
                break;
            }
        }
    }
}
void Binary(Tree * itree)
{
    if(EmptyTree(itree))
    {
        puts("Tree is empty");
        return;
    }
    FILE * plik = fopen("binary2.bin", "wb");
    Bin(itree->root,plik);
    fclose(plik);
}
void Bin(Folder * wf,FILE * plik)
{
    Position * s;
    for(s=wf->child; s!=NULL; s=s->next)
        fwrite(s, sizeof(Position), 1, plik);
    if(wf->fchild!=NULL)
        Bin(wf->fchild,plik);
    if(wf->sibling!=NULL)
        Bin(wf->sibling,plik);
}
void TexFile(Tree * itree)
{
    if(EmptyTree(itree))
    {
        puts("Tree is empty");
        return;
    }
    FILE * plikt = fopen("test3.txt", "w");
    Tex(itree->root,plikt);
    fclose(plikt);
}
void Tex(Folder * wf,FILE * plikt)
{
    Position * s;
    int l=1;
    fprintf (plikt, "Folder: %s\n",wf->title);
    for(s=wf->child; s!=NULL; s=s->next)
    {
        fprintf (plikt, "Position %d\n",l++);
        fprintf (plikt, "Title: %s\n",s->title);
        fprintf (plikt, "Name: %s\n",s->name);
        fprintf (plikt, "Surname: %s\n",s->surname);
        fprintf (plikt, "Technique: %s\n",s->style);
        fprintf (plikt, "Year: %d\n",s->year);
    }
    if(wf->fchild!=NULL)
        Tex(wf->fchild,plikt);
    if(wf->sibling!=NULL)
        Tex(wf->sibling,plikt);
}
