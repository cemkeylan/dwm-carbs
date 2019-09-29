/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char statussep         = ';';      /* separator between status bars */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor},
	[SchemeSel]  = { selfgcolor, selbgcolor,  selbordercolor},
};

/* tagging */
static const char *tags[] = { "Dev 1", "Dev 2", "Dev 3", "Dev 4", "News 5", "Chat 6", "Music 7", "Mail 8", "Web 9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title       tags mask     isfloating   monitor */
	{ "Gimp",        NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",     NULL,       NULL,       1 << 8,       0,           -1 },
	{ "qutebrowser", NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Surf",	 NULL,	     NULL,       1 << 8,       0,           -1 },
	{ "tabbed",	 NULL,	     NULL,       1 << 8,       0,           -1 },
	{ NULL,          NULL,       "Mail",     1 << 7,       0,           -1 },
	{ NULL,          NULL,       "Music",    1 << 6,       0,           -1 },
	{ NULL,	         NULL,       "IRC",      1 << 5,       0,           -1 },
	{ NULL,		 NULL,       "Gomuks",   1 << 5,       0,           -1 },
	{ NULL,		 NULL,       "News",     1 << 4,       0,           -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
};


/* key definitions */
#define MODKEY Mod1Mask
#define WINKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#include <X11/XF86keysym.h>
/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browcmd[] = { "qutebrowser", NULL };
static const char *surfcmd[] = { "tabbed", "-c", "surf", "-e", NULL };
static const char *rofiapp[] = { "rofi", "-show", "drun", NULL };
static const char *nighttog[] = { "nightmodetoggle", "click", NULL };
static const char *nightdis[] = { "nightmodetoggle", "disable", NULL };
static const char *shutdown[] = { "yousure", NULL };
static const char *newsboat[] = { "st", "-t", "News", "zsh", "-c", "newsboat", NULL};
static const char *reboot[] = { "yousurereb", NULL };
static const char *suspend[] = { "systemctl", "suspend", NULL };
static const char *brup[] = { "xbacklight", "-inc", "10", NULL };
static const char *brdown[] = { "xbacklight", "-dec", "10", NULL };
static const char *music[] = { "st", "-t", "Music", "zsh", "-c", "ncmpcpp", NULL };
static const char *mail[] = { "st", "-t", "Mail", "zsh", "-c", "neomutt", NULL };
static const char *fm[] = { "st", "zsh", "-c", "lf", NULL};
static const char *volup[] = { "pactl", "set-sink-volume", "0", "+5%", NULL};
static const char *voldown[] = { "pactl", "set-sink-volume", "0", "-5%", NULL};
static const char *volmute[] = { "pactl", "set-sink-mute", "0", "toggle", NULL};
static const char *audioplay[] = { "mpctoggle", NULL};
static const char *audionext[] = { "mpc", "next", NULL};
static const char *audioprev[] = { "mpc", "prev", NULL };
static const char *dmenumount[] = { "dmenumount", NULL };
static const char *dmenuumount[] = { "dmenuumount", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_u,      		  spawn,           {.v = dmenucmd } },
	{ WINKEY,			XK_e,      		  spawn,   	   {.v = nighttog } },
	{ WINKEY|ShiftMask,		XK_e,	   		  spawn,	   {.v = nightdis } },
	{ WINKEY|ShiftMask,		XK_n, 	   		  spawn,	   {.v = shutdown } },
	{ WINKEY|ShiftMask,		XK_r,	   		  spawn,	   {.v = reboot } },
	{ WINKEY|ShiftMask,		XK_s,	   		  spawn,	   {.v = suspend } },
	{ MODKEY,			XK_m,	   		  spawn,           {.v = music } },
	{ MODKEY,			XK_n,	   		  spawn,           {.v = fm } },
	{ MODKEY|ShiftMask,             XK_m,      		  spawn,	   {.v = mail } },
	{ MODKEY,                       XK_Return, 		  spawn,           {.v = termcmd } },
	{ MODKEY,                       XK_o,      		  spawn,           {.v = browcmd } },
	{ MODKEY,			XK_p,	   		  spawn,           {.v = surfcmd } },
	{ MODKEY,			XK_d,	   		  spawn,           {.v = rofiapp } },
	{ MODKEY|ShiftMask,             XK_n,			  spawn,           {.v = newsboat } },
	{ WINKEY|ShiftMask,	 	XK_m,			  spawn,	   {.v = dmenumount } },
	{ WINKEY|ShiftMask,		XK_u,			  spawn,	   {.v = dmenuumount } },
	{ MODKEY,                       XK_b,      		  togglebar,       {0} },
	{ MODKEY,                       XK_j,      		  focusstack,      {.i = +1 } },
	{ MODKEY,                       XK_k,      		  focusstack,      {.i = -1 } },
	{ MODKEY,                       XK_h,      		  setmfact,        {.f = -0.05} },
	{ MODKEY,                       XK_l,      		  setmfact,        {.f = +0.05} },
	{ MODKEY,                       XK_Return, 		  zoom,            {0} },
	{ MODKEY,                       XK_Tab,    		  view,            {0} },
	{ MODKEY|ShiftMask,             XK_q,      		  killclient,      {0} },
	{ WINKEY,                       XK_t,      		  setlayout,       {.v = &layouts[0]} },
	{ WINKEY,                       XK_f,      		  setlayout,       {.v = &layouts[1]} },
	{ WINKEY,                       XK_m,      		  setlayout,       {.v = &layouts[2]} },
	{ WINKEY,			XK_s,      		  setlayout,       {.v = &layouts[3]} },
	{ WINKEY,			XK_d,      		  setlayout,       {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  		  setlayout,       {0} },
	{ MODKEY|ShiftMask,             XK_space,  		  togglefloating,  {0} },
	{ MODKEY,                       XK_0,      		  view,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      		  tag,             {.ui = ~0 } },
	{ MODKEY|ControlMask,           XK_u,      		  focusmon,        {.i = -1 } },
	{ MODKEY|ControlMask,           XK_d,      		  focusmon,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  		  tagmon,          {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                tagmon,          {.i = +1 } },
	{ WINKEY,			XK_j,                     setgaps,         {.i = +1 } },
	{ WINKEY,			XK_k,                     setgaps,         {.i = -1 } },
	{ WINKEY,              		XK_h,                     setgaps,         {.i = 0 } },
	{ WINKEY,                       XK_i,       		  incnmaster,      {.i = +1 } },
	{ WINKEY,                       XK_l,                     incnmaster,      {.i = -1 } },
	{ 0, 			        XF86XK_MonBrightnessUp,   spawn,           {.v = brup } },
	{ 0,				XF86XK_MonBrightnessDown, spawn,           {.v = brdown } },
	{ 0,				XF86XK_AudioRaiseVolume,  spawn,           {.v = volup } },
	{ 0,				XF86XK_AudioLowerVolume,  spawn,           {.v = voldown } },
	{ 0,				XF86XK_AudioMute,         spawn,           {.v = volmute } },
	{ 0,				XF86XK_AudioPlay,         spawn,           {.v = audioplay } },
	{ 0,				XF86XK_AudioNext,	  spawn,           {.v = audionext } },
	{ 0,                            XF86XK_AudioPrev,         spawn,           {.v = audioprev } },
	{ MODKEY|ShiftMask,             XK_e,                     quit,            {0} },
	{ WINKEY,			XK_5,			  xrdb,            {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
