static struct{char member[32];}next__TO__next={{
 0x08, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

static struct{char member[32];}start__TO__next={{
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};

enum{STATE_NUMBER_start, STATE_NUMBER_fail, STATE_NUMBER_next, UNKNOWN_STATE=-1};

int STATE_isfinal(int STATE)
{
	switch(STATE){
		case STATE_NUMBER_start:
			return 0;
		default:
			return STATE;
	}
}

const int INITIAL_FSM_STATE = STATE_NUMBER_start;

#define STATE_init(X)	(X=INITIAL_FSM_STATE)

char STATE_TEXT_start [] = "start";
char STATE_TEXT_fail [] = "fail";
char STATE_TEXT_next [] = "next";
char STATE_TEXT_UNKNOWN [] = "UKNOWN";

char* STATE_text(int STATE)
{
	switch(STATE)
	{
		case STATE_NUMBER_start:
			return STATE_TEXT_start;
		case STATE_NUMBER_fail:
			return STATE_TEXT_fail;
		case STATE_NUMBER_next:
			return STATE_TEXT_next;
		default:
			return STATE_TEXT_UNKNOWN;
	}
}

int FSM(int STATE, char input)
{
	switch(STATE){
		case STATE_NUMBER_start:
			goto STATE_LABEL_start;
		case STATE_NUMBER_fail:
			goto STATE_LABEL_fail;
		case STATE_NUMBER_next:
			goto STATE_LABEL_next;
		default: return -1; //bad state
	}

	STATE_LABEL_start:
		if(cc_bit((charclass*)&start__TO__next, input))
			return STATE_NUMBER_next;
		//Default Transition:
			return STATE_NUMBER_fail;
	STATE_LABEL_next:
		if(cc_bit((charclass*)&next__TO__next, input))
			return STATE_NUMBER_next;
		//Default Transition:
			return STATE_NUMBER_fail;
	STATE_LABEL_fail:
		//Default Transition:
			return STATE_NUMBER_fail;
}

int FSM_finite_control(char* input)
{
	char *cur;
	int state;
	if(!input)
		return -1;

	for(cur=input, STATE_init(state);*cur;++cur)
	{
		state = FSM(state,*cur);
	}
	return STATE_isfinal(state);
}

