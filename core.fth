: cells    8 * ;
: true     -1 ;
: false    0 ;

: '        immediate bl find ;
: [']      immediate bl find ' push lit , , ' , lit , ;

: begin    immediate here 8 -  ;
: until    immediate ['] branch , ; 
: while    immediate ['] branch here 0 , ;
: repeat   immediate here 8 + swap ! ['] goto , ;
	   
: \        immediate begin key 10 = until ; 
: (        immediate begin key 41 = until ; ( and mid-line comments too! )
: _?       immediate ['] push key , ;

\ More stack manipulators
: 2drop    drop drop ;
: 3drop    2drop drop ;
: rot      >r swap r> swap ;
: -rot     rot rot ;
: nip      >r drop r> ;
: over     >r dup r> swap ;
: 2dup     over over ;
: 2swap    rot >r rot r> ;
: 2over    >r >r over over r> r> 2swap ;
: 2nip     >r >r 2drop r> r> ;
: 2rot     >r >r 2swap r> r> 2swap ;
: -2rot    2rot 2rot ;

\ some more memory managers
: incr     dup @ 1 + swap ! ;
: decr     dup @ 1 - swap ! ;
: allot    begin dup while 0 c, 1 - repeat drop ;

\ These are handy for testing things
: cr       10 emit ;
: space    32 emit ;
: spaces   begin dup while space 1 - repeat drop ;

\ Conditionals are just tracking and writing gotos
\ Note: '8 -' is required because the do_colon, et al, 
\ increment pip automatically -- jump targets need to land
\ one full cell prior to where we want execution to happen.

: if       immediate ['] branch here 0 , ;
: then     immediate here 8 - swap ! ;
: else     immediate ['] goto here 0 , swap here 8 - swap ! ;
: 0=       if false else true then ;

\ String handling functions
: s"       immediate ['] goto here 0 , here 0 begin
	     key dup _? " = 
	     if drop true else c, 1 + false then 
	   until rot here 8 -  swap ! swap 
	   ['] push , ['] push , ;
: scan     swap 1 + swap 1 - ;
: type     begin dup while over c@ emit scan repeat 2drop ;
: ."c      ' s" [ lit ] execute ['] type ;
: ."i      begin key dup 34 = 0= while emit repeat drop ;
: ."       immediate mode if ."c else ."i then ;
: esc      27 emit ;
: green    esc ." [32;1m" ;
: magenta  esc ." [35;1m" ;
: red      esc ." [31;1m" ;
: rst      esc ." [0m" ;

\ Working with definitions
: create   immediate bl header dovar [reveal] ;

: do       immediate
	   ['] swap here ['] >r ['] >r ;

: loop     immediate 
	   ['] r> ['] push 1 , ['] + 
	   ['] r> ['] 2dup ['] = 
	   ['] branch 8 - , ['] 2drop ;

: i        immediate ['] r> ['] dup ['] >r ;
	   
\ ------------------------------------------------------------------------
\ Now for some test code, to see how all of that works
\ ------------------------------------------------------------------------

: hello    ." Hello world!" cr ;
: test     if ." true" else ." false" then space ;

green ." Testing conditionals" cr rst
false test true test cr

cr green ." Testing string words" cr rst
hello 

cr green ." Testing stack manipulators" cr rst
." Start: " 1 2 3 4 .s
." 2over: " 2over .s 
." 2rot:  " 2rot .s
." 2nip:  " 2nip .s
." 3drop: " 3drop .s
." drop:  " drop .s

cr green ." Testing " magenta ." create " green ." functionality (squares) " cr rst
create array 4 cells allot
0 array 0 cells + !
1 array 1 cells + !
4 array 2 cells + !
9 array 3 cells + !
: .array 4 0 do array i cells + @ . loop ;
.array cr

cr green ." Testing do loops (three amigos)" cr rst
: test      3 0 do i 1 + . ." Amigo!" cr loop ;
test

cr green ." Testing recursion" cr rst
: factorial reveal dup if dup 1 - factorial * else drop 1 then ;
: test      0 do i factorial . loop ;
10 test cr

cr green ." Testing calling C functions (printf)" cr rst
: fmt      s" %d * 2 = %d " ;
: printf   [ cbifs @ ] lit 0 rot cbif ;
fmt drop 2 4 3 .s 
fmt 1 - ." FMT is " type cr 
printf cr

cr green ." Testing malloc and free" cr rst
: malloc   [ cbifs  8 + @ ] lit 1 1 cbif ;
: free     [ cbifs 16 + @ ] lit 0 1 cbif ;
1000000 malloc .s free .s

cr green ." Stack check: " red .s cr

