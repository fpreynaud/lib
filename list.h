typedef struct Element
{
	int empty;
	void* e;
	struct Element* prev;
	struct Element* next;
} Element ;

typedef Element* list;
typedef struct List
{
	list elements;
	Element* current;
	int empty;
	int length;
	Element* last;
} List;

Element Element_new();

Element Element_init(void* e, list prev, list next);

int Element_isEmpty(Element* e);

void* List_get(List* l);

List List_new();

List EltToLst(Element* e);

void List_del(List* l);

List List_copy(List* src);

void List_push(List* l, void* elt);

/* remove first element from the list and return it */
char* List_pop(List* l);

Element List_prev(List* l);

Element List_next(List* l);

void List_rewind(List* l);

int List_hasPrev(List* l);

int List_hasNext(List* l);

void List_toStringGeneric(List* l);

char* List_charListToString(List* l);

void List_printChar(List* l);

void List_toStringInt(List* l);

Element* List_last(List* l);

void List_toLast(List* l);

int List_isEmpty(List* l);

void List_add(List* l, void* e);

void List_addInt(List* l, int e);

void List_addFloat(List* l, float e);

void List_addDouble(List* l, double e);
