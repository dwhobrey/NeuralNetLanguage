/* NeSeL v2.0 April 1989 */
/* DFace neural net header file */

#define MAX_WIDTH 127 /* max pulse width */
#define PKT_EMPTY ((PktHnd)(0)) /* signifies No-Comms-Packet on channel */
#define RT2PIE 2.50662828 /* sqrt(2*pi) */

#define NUM_FEAS 11 /* num components to face */
#define NUM_EXPS 5 /* num expressions made from state of features */
#define NUM_ESIM 4 /* num simple emotions */
/* The feature detectors */
#define F_NOSE_BRIDGE 0
#define F_NOSE_BOTTOM 1
#define F_MOUTH_LSIDE 2
#define F_MOUTH_LIPS 3
#define F_MOUTH_RSIDE 4
#define F_EYE_LEFT 5
#define F_EYE_RIGHT 6
#define F_BROW_LEFT_L 7
#define F_BROW_LEFT_R 8
#define F_BROW_RIGHT_L 9
#define F_BROW_RIGHT_R 10
/* Note the following double as conx indices */
/* The static expressions */
#define E_MELLOW 0
#define E_HAPPY 1
#define E_ANGRY 2
#define E_SAD 3
#define E_SQUINT 4
/* The dynamic expressions */
#define D_LAUGH 0
#define D_FROWN 1
#define D_CRY 2
#define D_WINK 3

/* Internal code to distinguish feature, expression & emotion nodes */
#define ND_FEA 0
#define ND_EXP 1
#define ND_EMO 2
#define ND_ALT 3
#define ND_PRO 4

/* Position of features on face */
#define F_POSITION_X 45,45,35,35, 0,25,75,30,30,70,70
#define F_POSITION_Y 60,60,85,85, 0,30,30,20,20,20,20
#define F_LENGTH	 30,10,10,30,10,10,10,10,10,10,10

/* Feature angles that define the static expressions */
#define F_MELLOW 1,32,  96,32,32, 32,96, 96,32,96,32
#define F_HAPPY  10,32,112,32,16, 32,96, 80,48,80,48
#define F_ANGRY  5,24,  80,32,48, 40,88, 90,55,72,38
#define F_SAD    16,32, 85,32,43, 16,112,90,24,102,40
#define F_SQUINT 8,32, 115,32,10, 20,96, 110,48,80,48
