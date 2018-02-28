/* foobar.mata
 *
 * this machine should attempt to accept the words foo, bar, and bash
 * in any number or combination, seperated by whitespace.
 *
 * note: this version is case sensitive, and only matches all lower
 */

start: //this is the general start case, can match any of the words from here
	[f]			foo_1
	[b]			bar_bash_1
	[\s\t\n]	start //ignore whitespace
	~			fail

/* match the word foo! */
foo_1:
	[o]			foo_2 //matches the first o in foo ... i.e. fOo
	~			fail
foo_2:
	[o]			foo_3 //matchs ths last o .. foO
	~			fail
foo_3:
	[\s\t\n]	start //makes sure to not match compound words i.e. foobar
	~			fail

/* bar or bash? 
 *
 * this is a set of state transitions that check to 
 * mak sure that the word matches BAr or BAsh, then
 * branches when reaching baR or baSh, or ba[^sr]
 */
bar_bash_1:
	[a]			bar_bash_2
	~			fail
bar_bash_2:
	[r]			bar //found a 'bar'! ensure that white space follows
	[s]			bash_1 //found a 'bas', is it a 'bash'?
	~			fail

/* bar ... ensure word is delimited by whitewpace */
bar:
	[\s\t\n]	start
	~			fail

/* finally, lets find out if we have a bash */
bash_1:
	[h]			bash_2 //yay!!! found a bash, ensure delimited
	~			fail
bash_2:
	[\s\t\n]	start
	~			fail

/* fail is a sink, never leave */
fail: ~ fail
