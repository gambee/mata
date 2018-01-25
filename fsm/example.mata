/* examle.mata
 * This is a sketch of a possible example of the mata language
 *
 * Imagine a passive state machine (i.e. has an internal state, but no side affects 
 * on the outside world. It has read only access in the form of 'dequeue' actions,
 * and no other interaction with the outside world). 
 *
 * What things does it have?
 * 	- a finite set of input symbols : alph no need to declare, 'otherwise' 
 * 		case will solve the need for this
 * 	- a finite set of states : state
 * 	- a subset of accept states : final
 * 	- an initial state : q0 -ish
 * 	- a transition function that maps a state and input symbol to a state
 * 	- a finite control (the CPU + program wrapper) that applies the transition
 * 		function repeated on the initial state and an input string until the string
 * 		is done.
 * final, fail, and error states?
 */

machine MyMachine
{
	state q0:
		a0	->	q1
		a1	->	q2
		a2	->	q3
		a^	->	e0
	state q1:
		a0	->	q1
		a1	->	q2
		a2	->	q3
		a^	->	e0
	state q2:
		a1	->	q2
		a2	->	q3
		a^	->	e0
	state q3:
		a2	->	q3
		a^	->	f0
}
	

