if exists('b:current_syn') | finish| endif

syn region charclass start=/\v\[\^?/hs=e+1 skip=/\v([^]]|\\])/ end=/\v]/he=s-1 contains=PescapeChars,ccops,int
hi def link charclass String

syn match PescapeChars /\v\\./ contained containedin=charclass
hi def link PescapeChars Error
syn match escapeChars /\v\\[strn<{[^\\-]/ contained containedin=PescapeChars
hi def link escapeChars PreProc

syn match Pint /\v\{[^}]*}/ contained containedin=charclass contains=int
syn match int /\v\{([0-9]+(,|\.\.))*([0-9]+(\.\.)?)?}/ contains=ints,delim contained containedin=Pint
syn match ints /\v[0-9]+(,|\.\.)?/ contained containedin=int
syn match delim /[{}]/ contained containedin=int
syn match intops /\v(,|\.\.|\{|})/ contained containedin=ints
hi def link Pint Error
hi def link int Error
hi def link ints Type
hi def link delim Function
hi def link intops Function

syn match ccops /\v-/ contained containedin=charclass
hi def link ccops Function

syn match Pstate /\v[a-zA-Z_][a-zA-Z_0-9]*[ \t\n\r]*\<?:\>?/ contains=startstate,finalstate,op
hi def link Pstate Statement

syn match startstate /\v[a-zA-Z_][a-zA-Z_0-9]*[ \t\n\r]*\<?:\>/ contained contains=op containedin=Pstate
hi def link startstate Type

syn match finalstate /\v[a-zA-Z_][a-zA-Z_0-9]*[ \t\n\r]*\<:([^>]|\n)/ contained contains=op containedin=Pstate
hi def link finalstate PreProc

syn match deststate /\v\[(\\]|[^]])+\][ \t\n\r]*[a-zA-Z_][a-zA-Z_0-9]*/ contains=op,dest,charclass
syn match dest /\v[a-zA-Z_][a-zA-Z_0-9]*/ contained containedin=deststate
hi def link dest Normal

syn match dfltstate /\v\~[ \t\n\r]*[a-zA-Z_][a-zA-Z_0-9]*/ contains=op
hi def link dfltstate Constant

syn match op /\v(\<?:\>?|\~)/ containedin=state,dfltstate,Pstate,startstate contained
hi def link op Operator




"Comments:
syn region comments start=/\v\/\*/ end=/\v\*\//
syn match comments "\v//.*$" 
hi def link comments Comment
