/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_cyan  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};


typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = { "alacritty", "-t", "scratchpad1", "-d", "80", "24" };
const char *spcmd2[] = { "alacritty", "-t", "scratchpad2", "-d", "80", "24" };
const char *spcmd3[] = { "alacritty", "-t", "scratchpad3", "-d", "80", "24" };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"scratchpad1",      spcmd1},
	{"scratchpad2",    spcmd2},
	{"scratchpad3",   spcmd3},
};

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "alacritty", "-t", scratchpadname, "-d", "80", "24" } ;
/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",	  NULL,			NULL,		0,				1,			 -1 },
	{ "Firefox",  NULL,			NULL,		1 << 8,			0,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *browsercmd[]  = { "google-chrome-stable", NULL };
static const char *timcmd[] = {"/opt/deepinwine/apps/Deepin-TIM/run.sh", NULL};
static const char *screenshotcmd[] = { "flameshot", "gui", NULL };
static const char *typoracmd[] = { "typora", NULL };
static const char *ideacmd[] = {"/home/happym/.config/dwm/scripts/start-idea.sh", NULL};

static const char *upvol[]   = { "/home/happym/.config/dwm/scripts/vol-up.sh",  NULL };
static const char *downvol[] = { "/home/happym/.config/dwm/scripts/vol-down.sh",  NULL };
static const char *mutevol[] = { "/home/happym/.config/dwm/scripts/vol-toggle.sh",  NULL };
static const char *upbrightness[]   = { "/home/happym/.config/dwm/scripts/brightness-up.sh",  NULL };
static const char *downbrightness[]   = { "/home/happym/.config/dwm/scripts/brightness-down.sh",  NULL };
static const char *trayeropencmd[] = {"/home/happym/.config/dwm/scripts/trayer-open.sh", NULL};
static const char *trayerclosecmd[] = {"/home/happym/.config/dwm/scripts/trayer-close.sh", NULL};

static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
  { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
  { 0,              XF86XK_AudioLowerVolume, spawn,          {.v = downvol } },
  { 0,              XF86XK_AudioMute,        spawn,          {.v = mutevol } },
  { 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = upvol   } },
  { 0,              XF86XK_KbdBrightnessDown,   spawn,          {.v = downbrightness   } },
  { 0,              XF86XK_KbdBrightnessUp,     spawn,          {.v = upbrightness   } },
  { 0,              XK_Print,                spawn,          {.v = screenshotcmd } },
  { MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_w,      spawn,          {.v = trayeropencmd } },
  { MODKEY|ShiftMask,             XK_w,      spawn,          {.v = trayerclosecmd } },
  { MODKEY|ShiftMask,             XK_l,      rotatestack,    {.i = +1 } },
  { MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = -1 } },
  { MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = +1 } },
  { MODKEY|ShiftMask,             XK_i,      rotatestack,    {.i = -1 } },
  { MODKEY,                       XK_l,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
  { MODKEY,                       XK_Down,   focusstack,     {.i = +1 } },
  { MODKEY,                       XK_Up,     focusstack,     {.i = -1 } },
  { MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
  { MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_Left,   setmfact,       {.f = -0.05} },
  { MODKEY|ShiftMask,             XK_Right,  setmfact,       {.f = +0.05} },
  { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
  { MODKEY|ShiftMask,             XK_z,      setlayout,      {.v = &layouts[0]} },
  { MODKEY|ShiftMask,             XK_x,      setlayout,      {.v = &layouts[1]} },
  { MODKEY|ShiftMask,             XK_c,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_f,      fullscreen,     {0} },
  { MODKEY,                       XK_space,  setlayout,      {0} },
  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_c,      spawn,          {.v = browsercmd } },
  { MODKEY,                       XK_i,      spawn,          {.v = ideacmd } },
  { MODKEY,                       XK_n,      spawn,          {.v = typoracmd } },
  { MODKEY|ShiftMask,             XK_t,      spawn,          {.v = timcmd } },
	{ MODKEY,            			      XK_t,  	   togglescratch,  {.v = scratchpadcmd } },
  TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ShiftMask,             XK_e,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

