# TODO List

~~Move into Puffer~~
- finish c_step, c_reset, c_init
- write cy_gridscape.pyx
- add Log and LogBuffer
- finish GameState struct 
	- this should cdef in cy and init in python to pass up the chain
- unfuck rendering for local testing
	- this is why they declare AND define in the header... yuck
	- dynamically link ncurses and wrap the project in a .c and .h for training?

# Controls
- Keys 1-9: Move player
- 5: Check object details
- E: Interact with object
- A: Attack object
- Q: Quit
