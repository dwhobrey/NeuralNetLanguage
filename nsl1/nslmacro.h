/* Internal def of macros */
typedef struct 
{	char *m_name;
	char *m_text;
} NSLMacInfo;

NSLMacInfo NSLStdMacros[]={
	{"NSLSOpSize","$&%s"},  /* fixed size of array */
	{"NSLSOpNum","$|%s"},   /* cur num elms in array */
	{"NSLSOpHi","$^%s"},    /* largest value in array */
	{"NSLSOpLo","$_%s"},	/* lowest value in array */
	{"NSLSOpIn","$\%%s"},	/* num elms in decl, or num inputs after '.' */
	{"NSLSOpOut","$:%s"},	/* num elms in decl, or num outputs after '.' */
	{"NSLSOpNode","$.%s"},  /* current node num */
	{"NSLSOpPos","$@%s"},   /* position of cur elm in array */
	{"NSLSOpVal","$#%s"},   /* value of cur elm in array */
	{"NSLSOpAbs","$!@%s"},	/* absolute num of next conx */
	{"NSLSOpRel","$!#%s"},	/* relative num of next conx */
	{"NSLSOpEoc","$!!%s"},	/* returns non zero if not end of oelm */
	{"NSLSOpLeaf","$!%s"},  /* num leaves in next oelm at part level p */
	{"NSLSOpDip","$*%s"},   /* path dip string, must copy before reuse */
	{"NSLSOpPath","$+%s"},  /* path name string, copy before reuse */
	{"NSLSOpType","$-%s"},  /* path type string, copy before reuse */
	{"NSLLOpWild","*%s"},	/* [*] = wild array, use when a dummy needed */
	{"NSLLOpSize","&%s"},	/* [&x] = [0..($&x-1)] if ($&x-1)>0, else [0..0] */
	{"NSLLOpNum","|%s"},	/* [|x] = [0..($|x-1)] if ($|x-1)>0, else [0..0] */
	{"NSLLOpHiLo","^%s"},	/* [^x] = [$^x..$_x] */
	{"NSLLOpLoHi","_%s"},	/* [_x] = [$_x..$^x] */
	{"NSLLOpIn","\%%s"},	/* [%x] = [0..($%x-1)] if ($%x-1)>0, else [0..0] */
	{"NSLLOpOut",":%s"},	/* [:x] = [0..($:x-1)] if ($:x-1)>0, else [0..0] */
	{"NSLLOpEval","=%s"},	/* [=x] = eval array string, returns resulting array */
	{"NSLLOpDS",".%s"},     /* [.x] = returns DS ary if in Ds, else [&x] */
	{"NSLLOpCopy","!%s"},	/* [!x] = takes corresponding ary as result */
	{"NSLLOpAdd","+%s"},	/* [+x] = [(1+$^.x)..(1+$^.x+$!-1)] append */
	{"NSLLOpApp","@%s"},	/* [@x] = [(1+$^.x)..(2+$^.x)] append element */
	{"NSLLOpFit","?%s"},	/* [?x] = [+x] if +oelm or var, or [.x] if -oelm */
	{"NSLCOpAbs","@%s"},	/* '@' Absolute coordinates */
	{"NSLCOpCard","|%s"},	/* '|' Cardinal coordinates */
	{"NSLCOpIn","?%s"},		/* '?' Input coordinates */
	{"NSLCOpOut","!%s"},	/* '!' Output coordinates */
	{"NSLCOpBase",".%s"},	/* '.' Base coordinates */
	{NULL,NULL}
};

